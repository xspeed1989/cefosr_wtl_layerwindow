#include "stdafx.h"
#include "RendererApp.h"


RendererApp::RendererApp()
{
}


RendererApp::~RendererApp()
{
}

CefRefPtr<CefRenderProcessHandler> RendererApp::GetRenderProcessHandler()
{
    return this;
}
