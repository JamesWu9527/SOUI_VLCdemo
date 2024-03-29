// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"


CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
	m_inst = nullptr;
	m_player = nullptr;
	m_media = nullptr;
}

CMainDlg::~CMainDlg()
{
	if (m_inst)
	{
		libvlc_release(m_inst);
	}
	m_inst = nullptr;

	ReleaseVlc();
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;

	m_inst = libvlc_new(0, NULL);

	return 0;
}

void CMainDlg::OnPlay()
{
	//SMessageBox(NULL, L"点击了播放", L"提示", MB_OK);
	ReleaseVlc();
	//处理播放的逻辑
	SRealWnd* pWnd = (SRealWnd*)FindChildByName(L"wnd_play");
	SASSERT(pWnd);

	SStringT strPath = L"";
	SEdit* pEdit = FindChildByName2<SEdit>(L"edit_playurl");
	strPath = pEdit->GetWindowTextW();
	
	

	//对路径进行一个编码的转换 转换成UTF8来进行调用
	wchar_t* wchar = strPath.GetBuffer(strPath.GetLength());
	strPath.ReleaseBuffer();
	size_t len = wcslen(wchar) + 1;
	size_t converted = 0;
	char* format = (char*)malloc(len * sizeof(char));
	wcstombs_s(&converted, format, len, wchar, _TRUNCATE);

	//根据strPath判断播放的类型 如果是本地则使用本地的播放方法 区别 m_media 的创建方法
	int type = strPath.Find(L"http");
	m_media = strPath.Find(L"http") == -1 ? libvlc_media_new_path(m_inst, format) : libvlc_media_new_location(m_inst, format);
	if (!m_media)
	{
		SMessageBox(NULL, L"输入文件名有误，请重新输入！\n正确输入例如：F:\\1.jpg", L"提示", MB_OK);
		return;
	}
	m_player = libvlc_media_player_new_from_media(m_media);
	libvlc_media_release(m_media);
	libvlc_media_player_set_hwnd(m_player, (void*)pWnd->GetRealHwnd());
	int ret = libvlc_media_player_play(m_player);
	if (0 != ret)
	{
		SMessageBox(NULL,L"播放文件失败",L"提示",MB_OK);
	}
	free(format);
}

void CMainDlg::ReleaseVlc()
{
	if (m_player != nullptr)
	{
		libvlc_media_player_stop(m_player);
		libvlc_media_player_release(m_player);
	}
	m_player = nullptr;

}

//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}

void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}

void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

