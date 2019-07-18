// WtlWithCef.cpp : main source file for WtlWithCef.exe
//

#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include <include/cef_app.h>
#include "include/cef_sandbox_win.h"
#include "ClientApp.h"
#include "RendererApp.h"
#include "BrowserApp.h"
#include "OtherApp.h"
#include "ClientHandler.h"
#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library may not link successfully with all VS
// versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif
CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{

    HRESULT hRes;

    CefEnableHighDPISupport();

    CefMainArgs main_args(hInstance);
    void* sandbox_info = NULL;
#if defined(CEF_USE_SANDBOX)
    // Manage the life span of the sandbox information object. This is necessary
    // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
    CefScopedSandboxInfo scoped_sandbox;
    sandbox_info = scoped_sandbox.sandbox_info();
#endif

    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(GetCommandLineW());

    ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);

    CefRefPtr<CefApp> app;
    if (ClientApp::BrowserProcess == process_type)
    {
        hRes = ::CoInitialize(NULL);
        ATLASSERT(SUCCEEDED(hRes));

        AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
        app = new BrowserApp();
    }        
    else if (ClientApp::RendererProcess == process_type)
        app = new RendererApp();
    else if (ClientApp::OtherProcess == process_type)
        app = new OtherApp();

    int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
    if (exit_code >= 0)
        return exit_code;
    CefSettings settings;
#ifndef CEF_USE_SANDBOX
    settings.no_sandbox = true;
#endif
    settings.windowless_rendering_enabled = true;
    settings.multi_threaded_message_loop = false;
    CefString(&settings.application_client_id_for_file_scanning)
        .FromString("9A8DE24D-B822-4C6C-8259-5A848FEA1E68");
    CefInitialize(main_args, settings, app, sandbox_info);  

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainDlg dlgMain;

    if (dlgMain.Create(NULL) == NULL)
    {
        ATLTRACE(_T("Main dialog creation failed!\n"));
        return 0;
    }

    dlgMain.ShowWindow(nCmdShow);

    CefRunMessageLoop();
	//int nRet = Run(lpstrCmdLine, nCmdShow);
    _Module.RemoveMessageLoop();
	_Module.Term();
    CefShutdown();
	::CoUninitialize();
    return 1;
}
