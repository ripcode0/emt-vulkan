#include "vk_debug.h"

namespace emt
{
    VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug::debug_utils_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
        VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT *p_callback, void *p_user_data)
    {
        const char* severity = "";
        if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
            severity = "VERBOSE";
        else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
            severity = "INFO";
        else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            severity = "WARNING";
        else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            severity = "ERROR";
        printf("[vulkan debug %s] : %s\n", severity, p_callback->pMessage);
        //char code[256]{};
        //sprintf(code, "[vulkan debug %s] : %s\n", severity, p_callback->pMessage);
        //OutputDebugString(code);

        return VK_FALSE;
    }
} // namespace emt
