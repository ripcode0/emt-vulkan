#include "graphics.h"
#include <vector>
#include <iostream>
#include <Windows.h>
#include <vulkan/vulkan_win32.h>

namespace emt
{
    vk_context::vk_context(uint32_t cx, uint32_t cy, void* hwnd)
    {
        create_instance();
        const char* validataion_layer = "VK_LAYER_KHRONOS_validation";

        uint32_t layer_count = 0;

        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        

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

        VkResult res = vkCreateInstance(&create_info, nullptr, &m_instance);


    }

    void vk_context::create_instance()
    {
        std::vector<const char*> instance_layers;
        std::vector<const char*> instance_extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME
        };
        
        #if defined(WIN32)
            instance_extensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        #endif

        uint32_t ext_count = 0;

        vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

        if(ext_count > 0){
            std::vector<VkExtensionProperties> extensions(ext_count);
            if(vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, &extensions.front()) == VK_SUCCESS){
                for(VkExtensionProperties& ext : extensions){
                    printf("ext : %s\n", ext.extensionName);
                }
            }
        }
        int a = 10;
    }

} // namespace emt
