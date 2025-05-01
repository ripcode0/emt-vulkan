#pragma once

#define emt_platform emt_platform_win32


#include <vk_config.h>
#include <vector>

inline static PFN_vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessengerEXT = nullptr;
#define vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessengerEXT


namespace emt
{
// struct vk_gpu_device
// {
//     VkPhysicalDevice handle;
//     VkPhysicalDeviceProperties props;
//     VkPhysicalDeviceMemoryProperties mem_props;
//     VkPhysicalDeviceFeatures features;
//     VkSurfaceCapabilitiesKHR surface_caps;
//     std::vector<VkSurfaceFormatKHR> surface_formats;
//     std::vector<VkPresentModeKHR> present_modes;
//     uint32_t queue_family_index;
// };

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

    void record_cmd_buffer(VkCommandBuffer cmd, uint32_t image_index);

    void begin_frame();
    void end_frame();
private:
    HWND m_hwnd;
    uint32_t m_cx, m_cy;
};
} // namespace emt
