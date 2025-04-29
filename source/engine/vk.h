#pragma once

#include <vk_config.h>

#ifdef __cplusplus
extern "C"{
#endif

#define emt_define_handle(obj) typedef struct obj##_t* obj;

emt_define_handle(VkGPUDevice)
emt_define_handle(emt_gpu_memory)

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


#ifdef __cplusplus
}
#endif

