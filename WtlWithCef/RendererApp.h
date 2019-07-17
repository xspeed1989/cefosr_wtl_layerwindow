#pragma once
#include <include/cef_app.h>
class RendererApp
    : public CefApp
    , public CefRenderProcessHandler
{
public:
    RendererApp();
    virtual ~RendererApp();

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

    IMPLEMENT_REFCOUNTING(RendererApp);
};

