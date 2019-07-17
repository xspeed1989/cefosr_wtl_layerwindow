#include "stdafx.h"
#include "ClientHandler.h"
#include <gdiplus.h>
#include <include/cef_task.h>
#include <include/cef_app.h>
#include <ShellScalingApi.h>
#pragma comment(lib, "gdiplus.lib")

#define DPI_1X 96.0f
ClientHandler::ClientHandler(HWND hWnd)
    : hParent(hWnd)
{
}


ClientHandler::~ClientHandler()
{
}

CefRefPtr<CefRenderHandler> ClientHandler::GetRenderHandler()
{
    return this;
}

void ClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    RECT rct;
    GetWindowRect(hParent, &rct);
    int width = rct.right - rct.left;
    int height = rct.bottom - rct.top;
    if (0 == width)
        width = 1;
    if (0 == height)
        width = 1;
    rect.Set(0, 0, width, height);
}

void ClientHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{
    HDC hDc = GetDC(hParent);
    HDC hMemDC = CreateCompatibleDC(hDc);
    RECT rct;
    GetClientRect(hParent, &rct);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;

    DWORD *pvBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hDc, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&pvBits), NULL, 0);
    const int iTotal = width * height * 4;
    for (int i = 0; i != height; ++i)
    {
        for (int j = 0; j != width; ++j)
        {
            pvBits[i * width + j] = reinterpret_cast<const DWORD*>(buffer)[(height - i - 1) * width + j];
            byte *pByte = (byte*)&pvBits[i * width + j];

            pByte[0] = pByte[0] * pByte[3] / 255;

            pByte[1] = pByte[1] * pByte[3] / 255;

            pByte[2] = pByte[2] * pByte[3] / 255;
        }
    }

    GdiFlush();
    SelectObject(hMemDC, hBitmap);

    POINT ptSrc;
    ptSrc.x = 0;
    ptSrc.y = 0;
    POINT ptDst = ptSrc;
    ClientToScreen(hParent, &ptDst);
    SIZE szSrc;
    szSrc.cx = width;
    szSrc.cy = height;
    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 255;
    bf.AlphaFormat = AC_SRC_ALPHA;
    PAINTSTRUCT ps;
    BeginPaint(hParent, &ps);
    EndPaint(hParent, &ps);
    UpdateLayeredWindow(hParent, hDc, &ptDst, &szSrc, hMemDC, &ptSrc, 0, &bf, ULW_ALPHA);
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);
    ReleaseDC(hParent, hDc);
}

CefRefPtr<CefLifeSpanHandler> ClientHandler::GetLifeSpanHandler()
{
    return this;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    m_Browser = nullptr;
    CloseWindow(hParent);
    DestroyWindow(hParent);
    CefQuitMessageLoop();
}


void ClientHandler::close()
{
    m_Browser->GetHost()->CloseBrowser(false);
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    m_Browser = browser;
}

float GetDeviceScaleFactor() {
    static float scale_factor = 1.0;
    static bool initialized = false;

    if (!initialized) {
        // This value is safe to cache for the life time of the app since the user
        // must logout to change the DPI setting. This value also applies to all
        // screens.
        HDC screen_dc = ::GetDC(NULL);
        int dpi_x = GetDeviceCaps(screen_dc, LOGPIXELSX);
        scale_factor = static_cast<float>(dpi_x) / 96.0f;
        ::ReleaseDC(NULL, screen_dc);
        initialized = true;
    }

    return scale_factor;
}

bool IsProcessPerMonitorDpiAware() {
    enum class PerMonitorDpiAware {
        UNKNOWN = 0,
        PER_MONITOR_DPI_UNAWARE,
        PER_MONITOR_DPI_AWARE,
    };
    static PerMonitorDpiAware per_monitor_dpi_aware = PerMonitorDpiAware::UNKNOWN;
    if (per_monitor_dpi_aware == PerMonitorDpiAware::UNKNOWN) {
        per_monitor_dpi_aware = PerMonitorDpiAware::PER_MONITOR_DPI_UNAWARE;
        HMODULE shcore_dll = ::LoadLibrary(L"shcore.dll");
        if (shcore_dll) {
            typedef HRESULT(WINAPI * GetProcessDpiAwarenessPtr)(
                HANDLE, PROCESS_DPI_AWARENESS*);
            GetProcessDpiAwarenessPtr func_ptr =
                reinterpret_cast<GetProcessDpiAwarenessPtr>(
                    ::GetProcAddress(shcore_dll, "GetProcessDpiAwareness"));
            if (func_ptr) {
                PROCESS_DPI_AWARENESS awareness;
                if (SUCCEEDED(func_ptr(nullptr, &awareness)) &&
                    awareness == PROCESS_PER_MONITOR_DPI_AWARE)
                    per_monitor_dpi_aware = PerMonitorDpiAware::PER_MONITOR_DPI_AWARE;
            }
        }
    }
    return per_monitor_dpi_aware == PerMonitorDpiAware::PER_MONITOR_DPI_AWARE;
}

float GetWindowScaleFactor(HWND hwnd) {
    if (hwnd && IsProcessPerMonitorDpiAware()) {
        typedef UINT(WINAPI * GetDpiForWindowPtr)(HWND);
        static GetDpiForWindowPtr func_ptr = reinterpret_cast<GetDpiForWindowPtr>(
            GetProcAddress(GetModuleHandle(L"user32.dll"), "GetDpiForWindow"));
        if (func_ptr)
            return static_cast<float>(func_ptr(hwnd)) / DPI_1X;
    }
    return GetDeviceScaleFactor();
}

//bool ClientHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
//{
//    if (!::IsWindow(hParent))
//        return false;
//
//    CefRect view_rect;
//    GetViewRect(browser, view_rect);
//
//    screen_info.device_scale_factor = GetWindowScaleFactor(hParent);
//
//    // The screen info rectangles are used by the renderer to create and position
//    // popups. Keep popups inside the view rectangle.
//    screen_info.rect = view_rect;
//    screen_info.available_rect = view_rect;
//    return true;
//}
