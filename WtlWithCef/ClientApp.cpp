#include "stdafx.h"
#include "ClientApp.h"


ClientApp::ClientApp()
{
}


ClientApp::~ClientApp()
{
}


ClientApp::ProcessType ClientApp::GetProcessType(CefRefPtr<CefCommandLine> command_line)
{
    if (!command_line->HasSwitch("type"))
        return BrowserProcess;
    if (command_line->GetSwitchValue("type") == "renderer")
        return RendererProcess;
    return OtherProcess;
}