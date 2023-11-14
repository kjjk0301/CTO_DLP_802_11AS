// FormPage2.cpp: 구현 파일
//

#include "pch.h"
#include "DLP_ctrl.h"
#include "NetConnectionForm.h"
#include "Util.h"



// FormPage2

IMPLEMENT_DYNCREATE(NetConnectionForm, CFormView)

NetConnectionForm::NetConnectionForm()
	: CFormView(IDD_FORMVIEW2)
{

}

NetConnectionForm::~NetConnectionForm()
{
}

LRESULT NetConnectionForm::OnMessage(WPARAM length, LPARAM lpara)
{
	CString strMsg = m_pNet->m_NetCtrlMessage;

	CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_STAT);

	UpdateLines(pMsgEdt, strMsg);
	return 0;
}

void NetConnectionForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TREE1, m_TreeBin);
	//DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}

void NetConnectionForm::OnActivateView(BOOL bActivate, CFormView* pActivateView, CFormView* pDeactiveView)
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		Invalidate();
	}
}

BEGIN_MESSAGE_MAP(NetConnectionForm, CFormView)
	//ON_CBN_SELCHANGE(IDC_COMBO1, &FormPage2::OnCbnSelchangeCombo1)
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_CANMESSAGE, &NetConnectionForm::OnMessage)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &NetConnectionForm::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON1, &NetConnectionForm::OnBnClickedButton1)
END_MESSAGE_MAP()


// FormPage2 진단

#ifdef _DEBUG
void NetConnectionForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void NetConnectionForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


//TODO: ADD CAN MESSAGE SENDER(ForomPage1 & new Class)
void NetConnectionForm::OnInitialUpdate()
{
	m_pNet->OpenNetHandle(GetSafeHwnd());

	SetDlgItemTextW(IDC_EDIT_BINDIP, m_pNet->m_strBINDIP);
	SetDlgItemTextW(IDC_EDIT_BINDPORT, m_pNet->m_strBINDPORT);

	SetDlgItemTextW(IDC_EDIT_IPL, m_pNet->m_strIPL);
	SetDlgItemTextW(IDC_EDIT_PORTL, m_pNet->m_strPORTL);

	SetDlgItemTextW(IDC_EDIT_IPR, m_pNet->m_strIPR);
	SetDlgItemTextW(IDC_EDIT_PORTR, m_pNet->m_strPORTR);
}

void NetConnectionForm::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);

	Invalidate(); // 활성화 시 화면 갱신
}

#if 1
void NetConnectionForm::OnPaint()
{
	CFormView::OnPaint(); // FormView 클래스의 OnPaint() 함수 호출
	CWnd::BringWindowToTop();
}
#endif

BOOL NetConnectionForm::OnEraseBkgnd(CDC* pDC)
{
	// 그리기 영역을 지운다.
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, GetSysColor(COLOR_BTNFACE));

	return TRUE;
}

UINT MonitorFuncNetwork(LPVOID pParam)
{

	UINT16 arr[4]{ 0, };
	INT16 tmp;
	INT32 len;

	NetConnectionForm* pDlg = (NetConnectionForm*)pParam;

	CEdit* pMsgEdt = (CEdit * )pDlg->GetDlgItem(IDC_EDIT_STAT);
	CString strMsg=_T("HEX>> ");

	while (pDlg->mContinueousMonitor)
	{
		len= pDlg->m_pNet->UdpReceive();
		if (len > 0)
		{
			for (int i = 0; i < len; i++)
			{
				CString ByteData;
					
				ByteData.Format(_T("%2x"),pDlg->m_pNet->m_RcvData[i]);

				strMsg += ByteData;
			}

			strMsg += _T("   is rcvd_ \r\n");

			UpdateLines(pMsgEdt, strMsg);
			strMsg = _T("HEX>> ");
		}
	}
	return 0;
}


void NetConnectionForm::OnBnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pNet->m_hNet < 0)
	{
		m_pNet->m_hNet =m_pNet->OpenNetwork(m_pNet->m_strBINDIP, m_pNet->m_strBINDPORT);
		if (m_pNet->m_hNet == 0)
		{
			mContinueousMonitor = true;
			GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowTextW(_T("CLOSE"));
			//m_pMonitorThread=AfxBeginThread(MonitorFunc2, this);
		}
		else
		{
			m_pNet->m_hNet = -1;
		}
	}
	else
	{
		mContinueousMonitor = false;
		Sleep(10);
		m_pNet->CloseNet();
		m_pNet->m_hNet = -1;
		GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowTextW(_T("CONNECT"));
	}
}







void NetConnectionForm::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strMsg;
	CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_STAT);



	if (m_pNet->m_hNet == 0)
	{
		strMsg.Format(_T("Sending Test Message \r\n"));
		char NetMsg[256];

		sprintf_s(NetMsg, "MsgMSGMSG sending");

		m_pNet->UdpSend(m_pNet->m_strIPL, m_pNet->m_strPORTL,NetMsg,strlen(NetMsg));

	}
	else
	{
		strMsg.Format(_T("Connection Fail \r\n"));
	}
	UpdateLines(pMsgEdt, strMsg);


}
