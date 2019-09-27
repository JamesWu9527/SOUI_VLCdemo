#include "StdAfx.h"
#include "SouiRealWndHandler.h"
#include "SPlayDlg.h"

namespace SOUI
{
    CSouiRealWndHandler::CSouiRealWndHandler(void)
    {
    }

    CSouiRealWndHandler::~CSouiRealWndHandler(void)
    {
    }

    HWND CSouiRealWndHandler::OnRealWndCreate( SRealWnd *pRealWnd )
    {
        const SRealWndParam &param=pRealWnd->GetRealWndParam();
        if(param.m_strClassName==_T("SRealWnd"))
        {
			SPlayDlg *pWnd = new SPlayDlg;
			//mfc.demo�е����� pbtn->Create(param.m_strWindowName,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,::CRect(0,0,0,0),CWnd::FromHandle(pRealWnd->GetContainer()->GetHostHwnd()),pRealWnd->GetID());
			pWnd->Create(pRealWnd->GetContainer()->GetHostHwnd(), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, 0);
            pRealWnd->SetData(pWnd);
            //���سɹ�������Ĵ��ھ��
            return pWnd->m_hWnd;
        }else
        {
            return 0;
        }
    }

    void CSouiRealWndHandler::OnRealWndDestroy( SRealWnd *pRealWnd )
    {
        const SRealWndParam &param=pRealWnd->GetRealWndParam();
        if(param.m_strClassName==_T("SRealWnd"))
        {//�����洰�ڣ��ͷŴ���ռ�õ��ڴ�
			SPlayDlg * pWnd = (SPlayDlg*) pRealWnd->GetData();
            if(pWnd)
            {
				pWnd->DestroyWindow();
                delete pWnd;
            }
        }
    }
    
    //����������FALSE
    BOOL CSouiRealWndHandler::OnRealWndSize( SRealWnd *pRealWnd )
    {
        return FALSE;
    }

    //����������FALSE
    BOOL CSouiRealWndHandler::OnRealWndInit( SRealWnd *pRealWnd )
    {
        return FALSE;
    }
}
