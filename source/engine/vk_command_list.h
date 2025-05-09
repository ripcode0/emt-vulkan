#pragma once

#include <vk_config.h>

namespace emt
{
struct vk_command_list
{
    vk_command_list(const VkDevice device, const VkCommandPool pool);
    
    VkDevice m_device{};
    VkCommandPool m_command_pool{};
    VkCommandBuffer m_command_buffer{};
};
   
} // namespace emt
