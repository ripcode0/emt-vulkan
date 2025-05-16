#include "01-triangle.h"
#include <vk_context.h>
#include <vk_pipeline.h>


namespace emt
{
    traiangle_dynamic_rendering::traiangle_dynamic_rendering(vk_context* context)
        : scene(context)
    {
        
    }

    void traiangle_dynamic_rendering::init_frame()
    {
        printf("the scene initialiezed\n");
        create_buffers();
        printf("create buffers\n");
    }

    void traiangle_dynamic_rendering::update_frame(float dt)
    {
        
    }

    void traiangle_dynamic_rendering::render_frame()
    {
        const VkCommandBuffer cmd = m_context->get_command_buffer();

        VkClearAttachment color_att{};
        color_att.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_att.clearValue = {{0.2,0.4,0.2,1.0f}};
        color_att.colorAttachment = 0;

        VkClearRect rc{};
        rc.rect.extent = {m_context->m_cx, m_context->m_cy};
        rc.rect.offset = {0,0};
        rc.layerCount = 1;
        rc.baseArrayLayer = 0;

        vkCmdClearAttachments(cmd, 1, &color_att, 1, &rc);

        VkViewport viewport{0.f, (float)m_context->m_cy, (float)m_context->m_cx, -(float)m_context->m_cy, 0.f, 1.f };
        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor { {0,0}, {m_context->m_cx, m_context->m_cy}};
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

        VkDeviceSize offset{0};
        vkCmdBindVertexBuffers(cmd, 0, 1, &m_vertex_buffer, &offset);

        vkCmdBindIndexBuffer(cmd, m_index_buffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(cmd, 3, 1, 0, 0, 0);

    }

    void traiangle_dynamic_rendering::release_frame()
    {
        
    }

    void traiangle_dynamic_rendering::create_buffers(){
        VkDevice device = m_context->m_device;

        const std::vector<vertex> vertices = {
            {{ 0.0f,  0.5f, 0.0f}, { 1.f, 0.f, 0.f}},
            {{ 0.5f, -0.5f, 0.0f}, { 0.f, 1.f, 0.f}},
            {{-0.5f, -0.5f, 0.0f}, { 0.f, 0.f, 1.f}}
        };

        const std::vector<uint32_t> indices = {0,1,2};

        uint32_t vertices_size = vertices.size() * sizeof(vertex);
        uint32_t indices_size = indices.size() * sizeof(uint32_t);

        vk::create_buffer(vertices.data(), vertices_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &m_vertex_buffer);

        vk::create_buffer(indices.data(), indices_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &m_index_buffer);

        vk::create_shader_module("./data/shader/basic_vs.spv", &m_vs_shader);
        vk::create_shader_module("./data/shader/basic_ps.spv", &m_ps_shader);

        // # raw vulkan functions for input layout
        VkVertexInputBindingDescription binding_desc{};
        binding_desc.binding = 0;
        binding_desc.stride = sizeof(vertex);
        binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        VkVertexInputAttributeDescription attrib_desc[2];
        attrib_desc[0].binding = 0;
        attrib_desc[0].location = 0;
        attrib_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attrib_desc[0].offset = offsetof(vertex, pos);

        attrib_desc[1].binding = 0;
        attrib_desc[1].location = 1;
        attrib_desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attrib_desc[1].offset = offsetof(vertex, color);

        VkPipelineVertexInputStateCreateInfo vertexinput_state{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        vertexinput_state.vertexBindingDescriptionCount =1;
        vertexinput_state.pVertexBindingDescriptions = &binding_desc;        
        vertexinput_state.vertexAttributeDescriptionCount = _countof(attrib_desc);
        vertexinput_state.pVertexAttributeDescriptions = attrib_desc;

        // # wraped function for input layout
        vk_input_layout input_layout;
        input_layout.binding({0, sizeof(vertex), VK_VERTEX_INPUT_RATE_VERTEX});
        // attribute : { location | binding | format | offset }
        input_layout.attribute({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, pos)});
        input_layout.attribute({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, color)});        

        

        const VkShaderModule shaders[] = {
            m_vs_shader, m_ps_shader
        };

        VkPipelineLayoutCreateInfo pipeline_layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        pipeline_layout_info.setLayoutCount = 0;
        pipeline_layout_info.pSetLayouts = nullptr;
        pipeline_layout_info.pushConstantRangeCount = 0;
        pipeline_layout_info.pPushConstantRanges = nullptr;

        VK(vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &m_pipeline_layout));

        vk::create_pipeline(ARRAYSIZE(shaders), shaders, input_layout, m_pipeline_layout, &m_pipeline);

        vk_safe_destroy(device, m_vs_shader);
        vk_safe_destroy(device, m_ps_shader);

        
    }

    void traiangle_dynamic_rendering::create_pipeline()
    {
        VkPipelineMultisampleStateCreateInfo multi_sample{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        
    }

    // void traiangle_dynamic_rendering::create_buffers1()
    // {
    //     VkDevice device = m_context->m_device;

    //     const std::vector<vertex> vertices = {
    //         {{1.0f, 1.0f, 0.0f}, { 0.f, 1.f, 0.f}},
    //         {{-1.0f, 1.0f, 0.0f},{ 0.f, 1.f, 0.f}},
    //         {{1.0f, -1.0f, 0.0f},{ 0.f, 1.f, 0.f}}
    //     };

    //     const std::vector<uint32_t> indices = {0,1,2};

    //     uint32_t vertices_size = vertices.size() * sizeof(vertex);
    //     uint32_t indices_size = indices.size() * sizeof(uint32_t);

    //     VkMemoryAllocateInfo mem_alloc_info{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    //     VkMemoryRequirements mem_req{};

    //     vk_buffer staging_buffer;
    //     VkBufferCreateInfo buffer_create_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    //     buffer_create_info.size = vertices_size + indices_size;
    //     buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    //     VK(vkCreateBuffer(m_context->m_device, &buffer_create_info, nullptr, &staging_buffer.handle));

    //     vkGetBufferMemoryRequirements(m_context->m_device, staging_buffer.handle, &mem_req);

    //     mem_alloc_info.allocationSize = mem_req.size;

    //     mem_alloc_info.memoryTypeIndex = vk::get_memory_type_index(mem_req.memoryTypeBits, 
    //         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    //     VK(vkAllocateMemory(m_context->m_device, &mem_alloc_info, nullptr, &staging_buffer.memory));
    //     VK(vkBindBufferMemory(m_context->m_device, staging_buffer.handle, staging_buffer.memory, 0));
        
    //     uint8_t* data = nullptr;
    //     VK(vkMapMemory(device, staging_buffer.memory, 0, mem_alloc_info.allocationSize, 0, (void**)&data));
    //     memcpy(data, vertices.data(), vertices_size);
    //     data += vertices_size;
    //     memcpy(data, indices.data(), indices_size);

    //     VkBufferCreateInfo vertex_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    //     vertex_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    //     vertex_info.size = vertices_size;
    //     VK(vkCreateBuffer(device, &vertex_info, nullptr, &m_vertex_buffer.handle));
    //     vkGetBufferMemoryRequirements(device, m_vertex_buffer.handle, &mem_req);
    //     mem_alloc_info.allocationSize = mem_req.size;
    //     mem_alloc_info.memoryTypeIndex = m_context->get_memory_type_index(mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    //     VK(vkAllocateMemory(device, &mem_alloc_info, nullptr, &m_vertex_buffer.memory));
    //     VK(vkBindBufferMemory(device, m_vertex_buffer.handle, m_vertex_buffer.memory, 0));

    //     VkBufferCreateInfo index_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    //     index_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    //     index_info.size = indices_size;
    //     VK(vkCreateBuffer(device, &index_info, nullptr, &m_index_buffer.handle));
    //     vkGetBufferMemoryRequirements(device, m_index_buffer.handle, &mem_req);
    //     mem_alloc_info.allocationSize = mem_req.size;
    //     mem_alloc_info.memoryTypeIndex = m_context->get_memory_type_index(mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    //     VK(vkAllocateMemory(device, &mem_alloc_info, nullptr, &m_index_buffer.memory));
    //     VK(vkBindBufferMemory(device, m_index_buffer.handle, m_index_buffer.memory, 0));

    //     VkCommandBuffer copy_cmd{};

    //     VkCommandBufferAllocateInfo cmd_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    //     cmd_info.commandPool = m_context->m_command_pool;
    //     cmd_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    //     cmd_info.commandBufferCount = 1;
    //     VK(vkAllocateCommandBuffers(device, &cmd_info, &copy_cmd));

    //     VkCommandBufferBeginInfo cmd_begin_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

    //     VK(vkBeginCommandBuffer(copy_cmd, &cmd_begin_info));

    //     VkBufferCopy copy_region{};
    //     copy_region.size = vertices_size;
    //     vkCmdCopyBuffer(copy_cmd, staging_buffer.handle, m_vertex_buffer.handle, 1, &copy_region);
    //     copy_region.size = indices_size;
    //     copy_region.srcOffset = vertices_size;
    //     vkCmdCopyBuffer(copy_cmd, staging_buffer.handle, m_index_buffer.handle, 1, &copy_region);

    //     VK(vkEndCommandBuffer(copy_cmd));

    //     VkSubmitInfo submit_info{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    //     submit_info.commandBufferCount = 1;
    //     submit_info.pCommandBuffers = &copy_cmd;

    //     VkFenceCreateInfo fence_info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    //     VkFence fence{};
    //     VK(vkCreateFence(device, &fence_info, nullptr, &fence));

    //     VK(vkQueueSubmit(m_context->m_graphics_queue, 1, &submit_info, fence));

    //     VK(vkWaitForFences(device, 1, &fence, VK_TRUE, VK_DEFAULT_FENCE_TIMEOUT));

    //     vkDestroyFence(device, fence, nullptr);
    //     vkFreeCommandBuffers(device, m_context->m_command_pool, 1, &copy_cmd);

    //     vkDestroyBuffer(device, staging_buffer.handle, nullptr);
    //     vkFreeMemory(device, staging_buffer.memory, nullptr);
    // }

} // namespace emt
