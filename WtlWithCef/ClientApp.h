#pragma once
#include <include/cef_app.h>
class ClientApp
{
public:
    ClientApp();
    virtual ~ClientApp();

    enum ProcessType
    {
        BrowserProcess,
        RendererProcess,
        OtherProcess
    };

    static ProcessType GetProcessType(CefRefPtr<CefCommandLine> command_line);
};

