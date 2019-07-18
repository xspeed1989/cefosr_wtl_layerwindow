#pragma once
#include <include/cef_client.h>
class ClientHandler
    : public CefClient
    , public CefRenderHandler
    , public CefLifeSpanHandler
{
public:
    ClientHandler(HWND hWnd);
    virtual ~ClientHandler();

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
    virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    /*virtual bool ClientHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info) override;*/
    void close();
    void onMouseMove(int x, int y);
    
private:
    HWND hParent;
    CefRefPtr<CefBrowser> m_Browser;
    IMPLEMENT_REFCOUNTING(ClientHandler);
};

