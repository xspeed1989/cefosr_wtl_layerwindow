// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include <include/cef_app.h>
#include "ClientHandler.h"
#include <windowsx.h>

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();
    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    MoveWindow(&rect);
	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
    
    DWORD dwExstyle = GetWindowLong(GWL_EXSTYLE);
    SetWindowLong(GWL_EXSTYLE, dwExstyle | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
    LONG lStyle = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
    ::SetWindowLong(this->m_hWnd, GWL_STYLE, lStyle & ~WS_CAPTION | WS_SYSMENU | WS_MINIMIZE);
    HWND hWnd = *this;

    CefWindowInfo window_info;
    window_info.SetAsWindowless(hWnd);
    window_info.shared_texture_enabled = false;
    window_info.ex_style |= WS_EX_NOACTIVATE;
    CefBrowserSettings browser_settings;
    browser_settings.background_color = CefColorSetARGB(0, 255, 255, 255);
    browser_settings.windowless_frame_rate = 30;
    browser_settings.webgl = STATE_DISABLED;
    handler = new ClientHandler(*this);
    wchar_t szPath[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    wchar_t szConfig[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, szConfig, MAX_PATH);
    PathAppendW(szConfig, LR"(..\config.ini)");
    wchar_t szHtmlPath[MAX_PATH] = { 0 };
    GetPrivateProfileStringW(L"html", L"path", LR"(..\snow\index.html)", szHtmlPath, MAX_PATH, szConfig);
    PathAppendW(szPath, L"..");
    PathAppendW(szPath, szHtmlPath);
    CefBrowserHost::CreateBrowser(window_info, handler, szPath, browser_settings, nullptr, nullptr);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
    handler->close();
}


LRESULT CMainDlg::OnNcHitTest(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
    bHandled = true;
    return GetAsyncKeyState(VK_CONTROL) & (1 << 15)  ? HTCLIENT : HTCAPTION;
}

LRESULT CMainDlg::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    handler->onMouseMove(LOWORD(lParam), HIWORD(lParam));
    return 1;
}

//LRESULT CMainDlg::OnNCCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    if (wParam)
//    {
//        bHandled = true;
//        return 0;
//    }
//    return 0;
//}