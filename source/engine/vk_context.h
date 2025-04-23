#pragma once

#include <vulkan/vulkan.h>

//

// #define VK_PFUNC(instance, type, func, name) \
//   do{ \
//     if(!func) {       \
//         func = (type)vkGetInstanceProcAddr(instance, #name);\
//     } \
//   }while(false)
inline static PFN_vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessengerEXT = nullptr;
#define vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessengerEXT


namespace emt
{
class vk_context{
public:
    vk_context(uint32_t cx, uint32_t cy, void* hwnd);
    ~vk_context();

    void create_instance();
    void create_debug_callback();

    VkInstance m_instance{VK_NULL_HANDLE};

    VkDebugUtilsMessengerEXT m_debug_messenger{VK_NULL_HANDLE};


    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* p_callback,
        void* p_user_data
    );

    VkDebugUtilsMessengerEXT m_debug_messanger;
};
} // namespace emt
