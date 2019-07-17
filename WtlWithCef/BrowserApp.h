#pragma once
#include <include/cef_app.h>

class BrowserApp
    : public CefApp
    , public CefBrowserProcessHandler
{
public:
    BrowserApp();
    virtual ~BrowserApp();
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
    virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;

private:
    IMPLEMENT_REFCOUNTING(BrowserApp);
};

