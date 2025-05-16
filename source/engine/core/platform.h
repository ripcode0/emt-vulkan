#pragma once

#include <stdlib.h>
#include <crtdbg.h>

typedef struct platform_state
{
    void* internal_state;
} platform_state;

#define emt_malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef _MSC_VER
#define emt_check_memory_Leak _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
#define emt_check_memory_Leak (void()0)
#endif

void* emt_alloc(size_t size, bool aligned) {
    return malloc(size);
}