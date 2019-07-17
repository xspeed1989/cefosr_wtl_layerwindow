#include "stdafx.h"
#include "BrowserApp.h"
const char kMultiThreadedMessageLoop[] = "multi-threaded-message-loop";
const char kExternalMessagePump[] = "external-message-pump";
const char kCachePath[] = "cache-path";
const char kUrl[] = "url";
const char kOffScreenRenderingEnabled[] = "off-screen-rendering-enabled";
const char kOffScreenFrameRate[] = "off-screen-frame-rate";
const char kTransparentPaintingEnabled[] = "transparent-painting-enabled";
const char kShowUpdateRect[] = "show-update-rect";
const char kSharedTextureEnabled[] = "shared-texture-enabled";
const char kExternalBeginFrameEnabled[] = "external-begin-frame-enabled";
const char kMouseCursorChangeDisabled[] = "mouse-cursor-change-disabled";
const char kRequestContextPerBrowser[] = "request-context-per-browser";
const char kRequestContextSharedCache[] = "request-context-shared-cache";
const char kBackgroundColor[] = "background-color";
const char kEnableGPU[] = "enable-gpu";
const char kFilterURL[] = "filter-url";
const char kUseViews[] = "use-views";
const char kHideFrame[] = "hide-frame";
const char kHideControls[] = "hide-controls";
const char kAlwaysOnTop[] = "always-on-top";
const char kHideTopMenu[] = "hide-top-menu";
const char kWidevineCdmPath[] = "widevine-cdm-path";
const char kSslClientCertificate[] = "ssl-client-certificate";
const char kCRLSetsPath[] = "crl-sets-path";
const char kLoadExtension[] = "load-extension";
const char kNoActivate[] = "no-activate";

BrowserApp::BrowserApp()
{
}


BrowserApp::~BrowserApp()
{
}

CefRefPtr<CefBrowserProcessHandler> BrowserApp::GetBrowserProcessHandler()
{
    return this;
}


void BrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
    if (process_type.empty()) {
        // Pass additional command-line flags when off-screen rendering is enabled.
        //if (command_line->HasSwitch(kOffScreenRenderingEnabled) &&
        //    !command_line->HasSwitch(kSharedTextureEnabled)) {
        //    // Use software rendering and compositing (disable GPU) for increased FPS
        //    // and decreased CPU usage. This will also disable WebGL so remove these
        //    // switches if you need that capability.
        //    // See https://bitbucket.org/chromiumembedded/cef/issues/1257 for details.
        if (!command_line->HasSwitch(kEnableGPU)) {
            command_line->AppendSwitch("disable-gpu");
            command_line->AppendSwitch("disable-gpu-compositing");
        }
        /*}*/

        if (command_line->HasSwitch(kUseViews) &&
            !command_line->HasSwitch("top-chrome-md")) {
            // Use non-material mode on all platforms by default. Among other things
            // this causes menu buttons to show hover state. See usage of
            // MaterialDesignController::IsModeMaterial() in Chromium code.
            command_line->AppendSwitchWithValue("top-chrome-md", "non-material");
        }

        if (!command_line->HasSwitch(kCachePath) &&
            !command_line->HasSwitch("disable-gpu-shader-disk-cache")) {
            // Don't create a "GPUCache" directory when cache-path is unspecified.
            command_line->AppendSwitch("disable-gpu-shader-disk-cache");
        }
    }
}
