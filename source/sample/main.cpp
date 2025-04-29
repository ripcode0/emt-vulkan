#include <window.h>
#include <core/platform.h>

#include <iostream>
#include <span>

#include <vk.h>

void test(std::span<const char*> v){
    for(const char* e : v){
        printf("e : %s\n", e);
    }
}

int main(int args, char* argv[])
{
    emt_check_memory_Leak;
    const char* ex[] = {"hello", "las"};

    test(ex);

    VkGPUDevice device{};
    vkCreateGPUDevice(nullptr, &device);

    emt_gpu_memory mem;
    emt_create_gpu_memory(&mem);
    
    //emt::window window(1024, 780, "emt-vulkan 1.0.0");
    emt::window* window = new emt::window(400, 500, "vulkan");

    auto exit_code = window->exec();

    int* a = (int*)emt_alloc(10, false);

    return exit_code;
}