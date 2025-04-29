### VkQueue

     → graphics + compute + transfer 다 지원하는 Family queue index 찾음
     → 거기서 큐 하나 만듦
     → queue_priority 1.0f로 설정
     → VkDeviceQueueCreateInfo로 지정
     → vkCreateDevice() 할 때 같이 넘김

* 대부분의 게임 / 그래픽 에선 Queue 는 하나 `우선 순위` 는 1.0f 고정

```cpp
float queue_priority = 1.0f;

VkDeviceQueueCreateInfo queue_info{};
queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
queue_info.queueFamilyIndex = physical_device.family_queue_index;
queue_info.queueCount = 1;
queue_info.pQueuePriorities = &queue_priority;
```