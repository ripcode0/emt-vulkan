#include "window.h"
#include <vk_context.h>

namespace emt
{
    window::window(uint32_t cx, uint32_t cy, const char *title)
    {
        WNDCLASSEX wc{};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.hbrBackground = (HBRUSH)::GetStockObject(DKGRAY_BRUSH);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpfnWndProc = window::wnd_proc;
        wc.lpszClassName = "wnd++";
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hInstance = GetModuleHandle(nullptr);
        ::RegisterClassEx(&wc);

        int x = (GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
        int y = (GetSystemMetrics(SM_CYSCREEN) - cy) / 2;

        m_hwnd = ::CreateWindowEx(NULL, wc.lpszClassName, title, 
            WS_OVERLAPPEDWINDOW, x, y, cx, cy, nullptr, 0,
            wc.hInstance, nullptr
        );

        RECT rc{};
        ::GetClientRect(m_hwnd, &rc);
        uint32_t w = rc.right - rc.left;
        uint32_t h = rc.bottom - rc.top;

        m_context = new vk_context(w, h, m_hwnd);

        ::ShowWindow(m_hwnd, SW_SHOW);
    }

    window::~window()
    {
        delete m_context;
        
        DestroyWindow(m_hwnd);
        
    }

    int window::exec()
    {
        MSG msg{};
        while (msg.message != WM_QUIT)
        {
            while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if(msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE){
                    PostQuitMessage(0);
                }
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            //TODO : render system
        }
        
        return (int)msg.lParam;
    }

    LRESULT window::wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        return DefWindowProc(hwnd, msg, wp, lp);
    }

} // namespace emt
