#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>

namespace emt
{
class scene;
class vk_context;
class window{
public:
    window(uint32_t cx, uint32_t cy, const char* title);
    ~window();

    HWND m_hwnd;
    vk_context* m_context;

    int exec(scene* scene);

    static LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};
} // namespace emt
