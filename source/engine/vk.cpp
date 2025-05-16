#include "vk.h"
#include <stdint.h>
#include <stdlib.h>
#include <vk_context.h>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <fstream>
#include <sstream>

namespace emt
{
void vk::initialize(vk_context* context)
{
    if(is_initialized) return;

    device = context->m_device;
    mem_props = context->m_physical_device.memory_props;
    queue = context->m_graphics_queue;
    cmd_pool = context->m_command_pool;
    
    //create the command buffers
    VkCommandBufferAllocateInfo alloc_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    alloc_info.commandPool = cmd_pool;
    alloc_info.commandBufferCount = 1;

    VK(vkAllocateCommandBuffers(device, &alloc_info, &cmd));

    // create the fence
    VkFenceCreateInfo fence_info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    VK(vkCreateFence(device, &fence_info, nullptr, &fence));

    //vma
    VmaAllocatorCreateInfo vma_alloc_info{};
    vma_alloc_info.device = device;
    vma_alloc_info.physicalDevice = context->m_physical_device;
    vma_alloc_info.instance = context->m_instance;

    VK(vmaCreateAllocator(&vma_alloc_info, &vma_allocator));

    is_initialized = true;
}

void vk::uninitialize() {
    vmaDestroyAllocator(vma_allocator);
}

// VkResult vk::create_gpu_memory_buffer(const void *data, const uint32_t size, const VkBufferUsageFlagBits usage, VkBufferEx* p_buffer)
// {
//     if((size == 0) || (!data) || (*p_buffer)) return VK_ERROR_INITIALIZATION_FAILED;

//     VkBufferEx buffer = (VkBufferEx)malloc(sizeof(VkBufferEx_T));

//     if(!buffer) return VK_ERROR_OUT_OF_HOST_MEMORY;

//     VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
//     buffer_info.size = size;
//     buffer_info.usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
//     buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//     VkResult res = vkCreateBuffer(device, &buffer_info, nullptr, &buffer->handle);
//     if(res != VK_SUCCESS){
//         free(buffer);
//         return res;
//     }

//     return VkResult();
// }

void vk::create_buffer(const void *data, uint32_t size, VkBufferUsageFlagBits usage ,VkBuffer* pp_buffer)
{
    assert(is_initialized && "vk::initialize() must be initialized");

    // create the staging buffer
    VkBuffer staging_buffer{};
    VmaAllocation staging_alloc{};

    VkBufferCreateInfo staging_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    staging_info.size = size;
    staging_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VmaAllocationCreateInfo staging_alloc_info{};
    staging_alloc_info.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    vmaCreateBuffer(vma_allocator, &staging_info, &staging_alloc_info, &staging_buffer, &staging_alloc, nullptr);

    // data copy
    void* mapped;
    vmaMapMemory(vma_allocator, staging_alloc, &mapped);
    memcpy(mapped, data, size);
    vmaUnmapMemory(vma_allocator, staging_alloc);

    //create the gpu buffer
    VkBuffer gpu_buffer{};
    VmaAllocation gpu_allocation{};

    VkBufferCreateInfo gpu_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    gpu_info.size = size;
    gpu_info.usage = usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    VmaAllocationCreateInfo gpu_alloc_info{};
    gpu_alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VK(vmaCreateBuffer(vma_allocator, &gpu_info, &gpu_alloc_info, &gpu_buffer, &gpu_allocation, nullptr));

    // record the copy command
    VkCommandBufferBeginInfo cmd_begin_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    vkBeginCommandBuffer(cmd, &cmd_begin_info);

    VkBufferCopy copy_region{0, 0, size};
    vkCmdCopyBuffer(cmd, staging_buffer, gpu_buffer, 1, &copy_region);

    vkEndCommandBuffer(cmd);

    // excute and wait
    VkSubmitInfo submit_info{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &cmd;

    vkQueueSubmit(queue, 1, &submit_info, fence);

    vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);

    // reset and wait for reuse
    VK(vkResetFences(device, 1, &fence));
    vkResetCommandBuffer(cmd, 0);

    *pp_buffer = gpu_buffer;

    
}

void vk::release_buffer(VkBuffer buffer) {
    //vmaDestroyBuffer(vma_allocator, buffer, )
}

uint32_t vk::get_memory_type_index(uint32_t type_bit, VkMemoryPropertyFlags props)
{
    assert(is_initialized && "vk is not initialized");
    for(uint32_t i = 0; i < mem_props.memoryTypeCount; i++){
        if((type_bit & 1) == 1){
            if((mem_props.memoryTypes[i].propertyFlags & props) == props){
                return i;
            }
        }
        type_bit >>= 1;
    }
    throw "could not find a suitable memory type";
}

void vk::create_shader_module(const std::string& filename, VkShaderModule* pp_shader)
{
    std::printf("loading spir-v shader module : %s\n", filename.c_str());

    const auto code = vk::read_binary(filename);

    if(code.size() == 0){
        throw std::runtime_error("invalid shader module file");
    }

    VkShaderModuleCreateInfo shader_info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    shader_info.codeSize = code.size();
    shader_info.pCode = (uint32_t*)code.data();

    VkShaderModule shader{};

    VK(vkCreateShaderModule(device, &shader_info, nullptr, &shader));

    *pp_shader = shader;
}

std::vector<char> vk::read_binary(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file.is_open()){
        char buffer[128]{};
        sprintf(buffer, "failed open file %s", filename.c_str());
        fprintf(stderr, "%s\n", buffer);
        //assert(false);
        std::abort();
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    return buffer;
}

void vk::create_pipeline(
            const uint32_t shader_count, 
            const VkShaderModule* shader_modules,
            VkPipelineVertexInputStateCreateInfo& vertex_input_info,
            VkPipelineLayout piepline_layout,
            VkPipeline* pp_pipeline
            )
    {
        VkPipelineInputAssemblyStateCreateInfo input_assembly{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly.primitiveRestartEnable = VK_FALSE;
        

        const VkDynamicState dynamic_states[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
        dynamic_state.dynamicStateCount = _countof(dynamic_states);
        dynamic_state.pDynamicStates = dynamic_states;

        VkPipelineViewportStateCreateInfo viewport_state{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        viewport_state.viewportCount = 1;
        viewport_state.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo raster_state{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        raster_state.polygonMode = VK_POLYGON_MODE_FILL;
        //raster_state.cullMode = VK_CULL_MODE_BACK_BIT;
        raster_state.cullMode = VK_CULL_MODE_NONE;
        raster_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        raster_state.lineWidth = 1.0f;

        VkPipelineMultisampleStateCreateInfo multisample_state{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorblend_attachment{};
        colorblend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        
        VkPipelineColorBlendStateCreateInfo colorblend_state{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
        colorblend_state.attachmentCount = 1;
        colorblend_state.pAttachments = &colorblend_attachment;

        // dynamic rendering structure

        VkFormat color_format = VK_FORMAT_B8G8R8A8_SRGB;

        VkPipelineRenderingCreateInfo rendering_info{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
        rendering_info.colorAttachmentCount = 1;
        rendering_info.pColorAttachmentFormats = &color_format;

        VkPipelineShaderStageCreateInfo shader_stages[2] = {};
        shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shader_stages[0].module = shader_modules[0];
        shader_stages[0].pName = "main";

        shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shader_stages[1].module = shader_modules[1];
        shader_stages[1].pName = "main";

        // VkPipelineLayoutCreateInfo pipeline_layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        // pipeline_layout_info.setLayoutCount = 0;
        // pipeline_layout_info.pSetLayouts = nullptr;
        // pipeline_layout_info.pushConstantRangeCount = 0;
        // pipeline_layout_info.pPushConstantRanges = nullptr;

        // vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &m_pipeline_layout);


        VkGraphicsPipelineCreateInfo pipeline_info{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};

        pipeline_info.pNext = &rendering_info;
        pipeline_info.stageCount = shader_count;
        pipeline_info.pStages = shader_stages;
        pipeline_info.pVertexInputState = &vertex_input_info;
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &raster_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pColorBlendState = &colorblend_state;
        pipeline_info.pDynamicState = &dynamic_state;
        pipeline_info.layout = piepline_layout;
        pipeline_info.renderPass = VK_NULL_HANDLE;

        VkPipeline pipeline{};
        VK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline));
        
        *pp_pipeline = pipeline;
    }

} // namespace emt
