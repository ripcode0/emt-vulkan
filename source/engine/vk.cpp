#include "vk.h"
#include <stdint.h>
#include <stdlib.h>


struct VkGPUDevice_t
{
    uint32_t graphics_queue_index;
    uint32_t transfer_queue_index;
    bool present_support;
};

struct emt_gpu_memory_t
{
    uint32_t graphics_queue;
    uint32_t transfer_queue;
};


ERESULT vkCreateGPUDevice(const VkGPUDeviceCreateInfo *info, VkGPUDevice* device)
{
    if(!info) return EMT_INVAILID_ARGS;
    *device = (VkGPUDevice)malloc(sizeof(VkGPUDevice_t));
    (*device)->graphics_queue_index = info->count;
    return 0x12;
}

emt_result emt_create_gpu_memory(emt_gpu_memory *memory)
{
    if(memory) return 0;

    *memory = (emt_gpu_memory)malloc(sizeof(emt_gpu_memory));

    (*memory)->graphics_queue = 10;
    
    return EMT_OK;
}

VkResult vkCreatePhysicalDeviceEx(const VkPhysicalDeviceExCreateInfo* info, VkPhysicalDeviceEx device_ex)
{
    if(!info || !info->physical_device || device_ex != nullptr)
        return VK_ERROR_INITIALIZATION_FAILED;
    
    device_ex = (VkPhysicalDeviceEx)malloc(sizeof(VkPhysicalDeviceEx_T));
    device_ex->handle = info->physical_device;

    
    //vkGetPhysicalDeviceProperties(info->physical_device, )

    return VK_SUCCESS;
}

// VkResult vulkan_create_instance(VkInstance *instance)
// {
//     VkDeviceCreateInfo info{};
    
//     return VK_SUCCESS;
// }
