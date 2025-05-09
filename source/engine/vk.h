#pragma once

#include <vk_config.h>
#include <vk_mem_alloc.h>

#ifdef __cplusplus
extern "C"{
#endif

namespace emt
{
class vk_context;
struct vk_buffer
{
    VkBuffer handle;
    VkDeviceMemory memory;
};

struct vk
{
    static void initialize(vk_context* context);
    static void uninitialize();

    //  static VkResult create_gpu_memory_buffer(
    //     const void* data, const uint32_t size, const VkBufferUsageFlagBits usage, VkBufferEx* p_buffer);
    
    static void create_buffer_vma(
        const void* data,
        uint32_t size,
        VkBufferUsageFlagBits usage,
        VkBuffer& buffer);

    static uint32_t get_memory_type_index(uint32_t type_bit, VkMemoryPropertyFlags flags);
private:
    inline static VkDevice device{};
    inline static VkCommandBuffer cmd{};
    inline static VkQueue queue{};
    inline static VkPhysicalDeviceMemoryProperties mem_props{};
    inline static VkCommandPool cmd_pool{};
    inline static VkFence fence{};
    inline static VmaAllocator vma_allocator{};
    inline static bool is_initialized{false};

};

} // namespace emt
#ifdef __cplusplus
}
#endif

