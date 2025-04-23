#include "vk_context.h"
#include <vector>
#include <iostream>
#include <Windows.h>
//#include <vulkan/vulkan_win32.h>
#include <vk_config.h>

namespace emt
{
    vk_context::vk_context(uint32_t cx, uint32_t cy, void* hwnd)
    {
        create_instance();

        create_debug_callback();


    }

    vk_context::~vk_context()
    {
        vkDestroyInstance(m_instance, nullptr);

    }

    void vk_context::create_instance()
    {
        uint32_t version = VK_HEADER_VERSION_COMPLETE;

        printf("vulkan sdk version %d.%d.%d\n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));

        std::vector<const char*> instance_layers = {
            "VK_LAYER_KHRONOS_validation"
        };
        std::vector<const char*> instance_extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
        #if defined(_WIN32)
            "VK_KHR_win32_surface",
        #endif
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        };
  
        VkApplicationInfo info{};
        info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        info.pApplicationName = "emt-vulkan";
        info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        info.pEngineName = "vulkan=engine";
        info.engineVersion = VK_MAKE_VERSION(1,0,0);
        info.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &info;
        create_info.enabledLayerCount = instance_layers.size();
        create_info.ppEnabledLayerNames = instance_layers.data();
        create_info.enabledExtensionCount = instance_extensions.size();
        create_info.ppEnabledExtensionNames = instance_extensions.data();

        VK(vkCreateInstance(&create_info, nullptr, &m_instance));
        
        int a = 0;
    }

    void vk_context::create_debug_callback(){

        // VkDebugReportCallbackCreateInfoEXT is deprecated
        // VkDebugUtilsMessengerCreateInfoEXT is newer

        VkDebugUtilsMessengerCreateInfoEXT debug_info{};
        debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_info.pfnUserCallback = vk_context::debug_utils_message_callback;

        //auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");

        //__vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
        //VK_PFUNC(m_instance, PFN_vkCreateDebugReportCallbackEXT, vkCreateDebugUtilsMessengerEXT);
        vk_load_instance_func(m_instance, vkCreateDebugUtilsMessengerEXT);

        //HRESULT res = vkCreateDebugUtilsMessengerEXT(m_instance, nullptr, nullptr, &m_debug_messanger);

        if(vkCreateDebugUtilsMessengerEXT != nullptr){
            printf("registed debug callback\n");
        }

        VkResult res = vkCreateDebugUtilsMessengerEXT(m_instance, &debug_info, nullptr, &m_debug_messanger);

        int a = 0;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL vk_context::debug_utils_message_callback(
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

        return VK_FALSE;
    }

} // namespace emt
