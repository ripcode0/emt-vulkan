#include "vk.h"
#include <stdint.h>
#include <stdlib.h>
#include <vk_context.h>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

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

void vk::create_buffer_vma(const void *data, uint32_t size, VkBufferUsageFlagBits usage ,VkBuffer &buffer)
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

} // namespace emt
