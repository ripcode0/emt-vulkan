#pragma once

#include <vk_config.h>

#ifdef __cplusplus
extern "C"{
#endif

#define emt_define_handle(obj) typedef struct obj##_t* obj;

emt_define_handle(VkGPUDevice)
emt_define_handle(emt_gpu_memory)

#define EMT_BARRIER_STATE_RENDER_TARGET 0 < 1
#define EMT_BARRIER_STATE_PRESENT 0 < 2



typedef long ERESULT;
typedef long emt_result;

#define EMT_OK              ((ERESULT)0x00000000)
#define EMT_INVAILID_ARGS   ((ERESULT)0x80000001)

struct VkGPUDeviceCreateInfo
{
    int count;    
};

ERESULT vkCreateGPUDevice(const VkGPUDeviceCreateInfo* info, VkGPUDevice* device);

emt_result emt_create_gpu_memory(emt_gpu_memory* memory);

VkResult vulkan_create_instance(VkInstance* instance);

VK_DEFINE_HANDLE(VkPhysicalDeviceEx)

typedef struct VkPhysicalDeviceEx_T{
    // the VkPhysicalDevice not created directly.
    // it's acquired from what the Vulkan driver manages internally
    VkPhysicalDevice handle = VK_NULL_HANDLE;
    uint32_t family_queue_index = UINT32_MAX;
    operator VkPhysicalDevice() const noexcept{
        return handle;
    }
}VkPhysicalDeviceEx_T;

struct VkPhysicalDeviceExCreateInfo{
    uint32_t sType;
    const VkPhysicalDevice physical_device;
    const VkInstance instance;
    const VkSurfaceFormatKHR format;

};
VkResult vkCreatePhysicalDeviceEx(const VkPhysicalDeviceExCreateInfo* info, VkPhysicalDeviceEx device_ex);


#ifdef __cplusplus
}
#endif

