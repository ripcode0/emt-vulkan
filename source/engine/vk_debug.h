#pragma once

#include <vk_config.h>


namespace emt
{
struct vk_debug
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback,
        void* p_user_data
    );
};

} // namespace emt
