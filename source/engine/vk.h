#pragma once

#include <vk_config.h>
#include <vk_mem_alloc.h>
#include <vector>
#include <string>

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

struct vk_buffer_vma
{
    VkBuffer handle{};
    VmaAllocation allocation{};
};


struct vk
{
    static void initialize(vk_context* context);
    static void uninitialize();

    static void create_buffer(
        const void* data,
        uint32_t size,
        VkBufferUsageFlagBits usage,
        VkBuffer* pp_buffer);
    static void release_buffer(VkBuffer buffer);
    static uint32_t get_memory_type_index(uint32_t type_bit, VkMemoryPropertyFlags flags);

    static void create_shader_module(const std::string& filename, VkShaderModule* pp_shader);

    static std::vector<char> read_binary(const std::string& filename);

    static void create_pipeline(
            const uint32_t shader_count, 
            const VkShaderModule* shader_modules,
            VkPipelineVertexInputStateCreateInfo& vertex_input_info,
            VkPipelineLayout piepline_layout,
            VkPipeline* pp_pipeline
            );
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

