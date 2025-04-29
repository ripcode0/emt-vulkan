#include "vk_context.h"
#include <vector>
#include <iostream>
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include <vk_config.h>
#include <vk_init.h>
#include <vk_debug.h>
#include <span>

namespace emt
{
    vk_context::vk_context(uint32_t cx, uint32_t cy, HWND hwnd)
        : m_cx(cx), m_cy(cy), m_hwnd(hwnd)
    {
        create_instance();

        create_debug_callback();

        create_surface();

        create_device();


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
        debug_info.pfnUserCallback = vk_debug::debug_utils_message_callback;

        //auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");

        //__vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
        //VK_PFUNC(m_instance, PFN_vkCreateDebugReportCallbackEXT, vkCreateDebugUtilsMessengerEXT);
        vk_load_instance_func(m_instance, vkCreateDebugUtilsMessengerEXT);

        VK(vkCreateDebugUtilsMessengerEXT(m_instance, &debug_info, nullptr, &m_debug_messenger));
    }

    void vk_context::create_surface()
    {   
        // VkWin32SurfaceCreateInfoKHR surface_info{};
        // surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        // surface_info.hinstance = GetModuleHandle(nullptr);
        // surface_info.hwnd = m_hwnd;

        auto surface_info = vk_init::surface_create_info(m_hwnd);

        VK(vkCreateWin32SurfaceKHR(m_instance, &surface_info, nullptr, &m_surface));
    }

    void vk_context::create_device()
    {
        const std::vector<const char*> device_extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        vk_physical_device physical_device{};
        get_physical_device_from_instance(m_instance, m_surface, &physical_device);

        // check the 1.3 dynamic rendering
        VkPhysicalDeviceDynamicRenderingFeatures dynamic_rendering_feats{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES};
        dynamic_rendering_feats.dynamicRendering = TRUE;
        
        VkPhysicalDeviceFeatures2 feats{
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2
        };
        feats.features = {};
        feats.pNext = &dynamic_rendering_feats;

        float queue_priority = 1.0f;

        VkDeviceQueueCreateInfo queue_info{};
        queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.queueFamilyIndex = physical_device.family_queue_index;
        queue_info.queueCount = 1;
        queue_info.pQueuePriorities = &queue_priority;

        VkDeviceCreateInfo device_info{};
        device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.pNext = &feats;
        device_info.queueCreateInfoCount = 1;
        device_info.pQueueCreateInfos = &queue_info;
        device_info.enabledExtensionCount = device_extensions.size();
        device_info.ppEnabledExtensionNames = device_extensions.data();

        VK(vkCreateDevice(physical_device, &device_info, nullptr, &m_device));
      
        // TODO: fix
        int a = 0;


        
    }

    void vk_context::get_physical_device_from_instance(
        const VkInstance instance, const VkSurfaceKHR surface, vk_physical_device* gpu_device)
    {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
        std::vector<VkPhysicalDevice> phy_devices(device_count);
        vkEnumeratePhysicalDevices(instance, &device_count, phy_devices.data());

        for(const auto& device : phy_devices){
            uint32_t queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

            std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

            for(uint32_t i = 0; i < queue_family_count; ++i){
                VkBool32 present_support = false;
                VkBool32 is_discrete = false;

                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);

                VkPhysicalDeviceProperties props{};
                vkGetPhysicalDeviceProperties(device, &props);

                if(props.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                    is_discrete = true;
                
                VkQueueFlags flags = queue_families[i].queueFlags;    
                if( (flags & VK_QUEUE_GRAPHICS_BIT) &&
                    (flags & VK_QUEUE_TRANSFER_BIT) &&
                    (flags & VK_QUEUE_COMPUTE_BIT) && present_support && is_discrete)
                {
                    
                    printf("found the complete queue index %s : %d\n", props.deviceName, i);
                    gpu_device->handle = device;
                    gpu_device->family_queue_index = i;
                    gpu_device->flags = flags;
                }
            }
        }
    }



} // namespace emt
