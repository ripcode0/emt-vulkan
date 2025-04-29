#pragma once

#include <vk_config.h>

namespace vk_init
{
    VkWin32SurfaceCreateInfoKHR surface_create_info(HWND hwnd){
        VkWin32SurfaceCreateInfoKHR info{};
        info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        info.hwnd = hwnd;
        info.hinstance = GetModuleHandle(nullptr);
        return info;
    }
} // namespace vk_init
