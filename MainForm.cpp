// MainForm.cpp: 구현 파일
//
#include "pch.h"
#include "DLP_ctrl.h"
#include "MainForm.h"

// MainForm

IMPLEMENT_DYNCREATE(MainForm, CFormView)

MainForm::MainForm()
	: CFormView(IDD_FORMVIEW)
{

}

MainForm::MainForm(UINT id)
	: CFormView(id)
{

}

MainForm::~MainForm()
{
}

void MainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(MainForm, CFormView)
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &MainForm::OnTcnSelchangeTab1)
//	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// MainForm 진단

#ifdef _DEBUG
void MainForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MainForm 메시지 처리기


BOOL MainForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}




int MainForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;





	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void MainForm::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 탭 숨김처리 문제 해결 필요

	int Select = m_Tab.GetCurSel();
	switch (Select)
	{
	case 0:
			m_pform1->ShowWindow(SW_SHOW);
			m_pform2->ShowWindow(SW_HIDE);
			m_pform3->ShowWindow(SW_HIDE);
			break;
	case 1:
			m_pform1->ShowWindow(SW_HIDE);
			m_pform2->ShowWindow(SW_HIDE);
			m_pform3->ShowWindow(SW_SHOW);
			break;
	case 2:
			m_pform1->ShowWindow(SW_HIDE);
			m_pform2->ShowWindow(SW_SHOW );
			m_pform3->ShowWindow(SW_HIDE);
			break;
	
	}


	*pResult = 0;
}


void MainForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//m_pKvCan = new CCanKvaser;

	m_pNet = new CNetHandle;


	CRect rect;
	CCreateContext ccx[3];

	TCITEM *p_Tabitem[3];
	for (int i = 0; i < 3; i++) 
	{
		p_Tabitem[i] = new TCITEM;
		p_Tabitem[i]->mask = TCIF_TEXT;
	}
	p_Tabitem[0]->pszText = _T("Control");
	p_Tabitem[1]->pszText = _T("Update");
	p_Tabitem[2]->pszText = _T("Network");

	m_Tab.GetWindowRect(rect);
	m_Tab.InsertItem(0, p_Tabitem[0]);
	m_Tab.InsertItem(1, p_Tabitem[1]);
	m_Tab.InsertItem(2, p_Tabitem[2]);
	

	//Fix tab Item overproduction
	m_Tab.DeleteItem(3);
	m_Tab.DeleteItem(3);
	m_Tab.DeleteItem(3);

	
	m_Tab.SetCurSel(0);

	CDC* pDC = GetDC();
	int dpiY = pDC->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pDC);
	int tabCtrlStartY = MulDiv(30, dpiY, 96);

	CView* pView = (CView*)RUNTIME_CLASS(DeviceManageForm)->CreateObject();
	ZeroMemory(&ccx[0], sizeof(ccx[0]));
	pView->Create(nullptr, nullptr, WS_CHILD, rect, this, IDD_FORMVIEW1, &ccx[0]);
	m_pform1 = (DeviceManageForm *)pView;
	m_pform1->SetWindowPos(nullptr, 0, tabCtrlStartY, rect.Width() + 15, rect.Height() - (tabCtrlStartY-10), SWP_NOZORDER | SWP_NOREPOSITION);
	m_pform1->ShowWindow(SW_SHOW);
	m_pform1->m_pNet = this->m_pNet;

	pView = (CView*)RUNTIME_CLASS(FirmwareUpdateForm)->CreateObject();
	ZeroMemory(&ccx[1], sizeof(ccx[1]));
	pView->Create(nullptr, nullptr, WS_CHILD, rect, this, IDD_FORMVIEW3, &ccx[1]);
	m_pform3 = (FirmwareUpdateForm*)pView;
	m_pform3->SetWindowPos(nullptr, 0, tabCtrlStartY, rect.Width() + 15, rect.Height() - (tabCtrlStartY - 10), SWP_NOZORDER | SWP_NOREPOSITION);
	m_pform3->ShowWindow(SW_HIDE);
	m_pform3->m_pNet = this->m_pNet;

	pView = (CView*)RUNTIME_CLASS(NetConnectionForm)->CreateObject();
	ZeroMemory(&ccx[2], sizeof(ccx[2]));
	pView->Create(nullptr, nullptr, WS_CHILD, rect, this, IDD_FORMVIEW2, &ccx[2]);
	m_pform2 = (NetConnectionForm*)pView;
	m_pform2->SetWindowPos(nullptr, 0, tabCtrlStartY, rect.Width() + 15, rect.Height() - (tabCtrlStartY-10), SWP_NOZORDER | SWP_NOREPOSITION);
	m_pform2->ShowWindow(SW_HIDE);
	m_pform2->m_pNet = this->m_pNet;

	

	m_pform1->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pform2->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pform3->ModifyStyle(0, WS_CLIPCHILDREN);


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CSize ss;

	//Disable MainFrame Scroll
	ss.cx = 0;
	ss.cy = 0;
	SetScrollSizes(MM_TEXT, ss);
}


//void MainForm::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
//{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

//	CFormView::OnGetMinMaxInfo(lpMMI);
//}
