#include <window.h>
#include <core/platform.h>
#include <iostream>
#include "01-triangle.h"

int main(int args, char* argv[])
{
    emt_check_memory_Leak;
    
    emt::window* window = new emt::window(1024, 780, "vulkan");

    emt::traiangle_dynamic_rendering scene(window->m_context);

    int exit_code = window->exec(&scene);

    safe_delete(window);

    return exit_code;
}