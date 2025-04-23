#pragma once

#include <vulkan/vulkan.h>

namespace emt
{
class vk_context{
public:
    vk_context(uint32_t cx, uint32_t cy, void* hwnd);

    void create_instance();

    VkInstance m_instance{};

    VkPhysicalDevice m_physical_device;

    VkDevice m_device{};

    VkPhysicalDeviceProperties m_properties;

    VkCommandPool m_cmd_pool;

};
} // namespace emt
