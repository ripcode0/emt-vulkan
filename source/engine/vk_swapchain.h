#pragma once

#include <vk_config.h>

#define max_concurent_frames 2

namespace emt
{
class vk_context;
class vk_swapchain
{
public:
    vk_swapchain(const vk_context* context, uint32_t cx, uint32_t cy);
    
    VkDevice m_device;
private:
    const vk_context* m_context;
}; 
} // namespace emt
