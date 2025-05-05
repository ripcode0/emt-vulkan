#pragma once

#define emt_platform emt_platform_win32


#include <vk_config.h>
#include <vector>

inline static PFN_vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessengerEXT = nullptr;
#define vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessengerEXT

#define MAX_SYNC_FRAMES 2

namespace emt
{
struct vk_physical_device
{
    VkPhysicalDevice handle;
    uint32_t family_queue_index = UINT32_MAX;
    VkQueueFlags flags;
    VkPhysicalDeviceMemoryProperties memory_props;
    
    operator VkPhysicalDevice() const noexcept {
        return handle;
    }
};




class vk_context{
public:
    vk_context(uint32_t cx, uint32_t cy, HWND hwnd);
    ~vk_context();

    void create_instance();
    void create_debug_callback();
    void create_device();
    void get_physical_device_from_instance(
        const VkInstance instance, const VkSurfaceKHR surface, vk_physical_device* device);
    void create_surface();

    void create_command_pool();

    void create_swapchain();
    void create_command_buffers();

    VkInstance m_instance{VK_NULL_HANDLE};
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_graphics_queue = nullptr;
    vk_physical_device m_physical_device{};
    

    VkDebugUtilsMessengerEXT m_debug_messenger{};

    // swapchain
    VkSwapchainKHR m_swapchain{};
    std::vector<VkImage> m_swapchain_images;
    std::vector<VkImageView> m_swapchain_image_views;

    // command
    VkCommandPool m_command_pool{};
    std::vector<VkCommandBuffer> m_command_buffers;

    // render pass
    VkRenderPass m_render_pass{};
    void create_render_pass();

    // sync
    VkSemaphore m_present_semaphore[MAX_SYNC_FRAMES];
    VkSemaphore m_render_semaphore[MAX_SYNC_FRAMES];
    VkFence m_fence[MAX_SYNC_FRAMES];

    void create_sync_objects();



    void record_cmd_buffer(VkCommandBuffer cmd, uint32_t image_index);
    void record_cmd_buffer_13(VkCommandBuffer cmd);
    void begin_frame();
    void end_frame();

    VkCommandBuffer get_command_buffer();
//private:
    HWND m_hwnd;
    uint32_t m_cx, m_cy;
    uint32_t m_current_frame{};
    uint32_t m_image_index{};
    
};
} // namespace emt
