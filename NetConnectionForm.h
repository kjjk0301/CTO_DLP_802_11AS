#pragma once
#include "CNetHandle.h"
#include "CFileParser.h"


// FormPage2 폼 보기

class NetConnectionForm : public CFormView
{
	DECLARE_DYNCREATE(NetConnectionForm)

protected:

public:
	NetConnectionForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~NetConnectionForm();
	LRESULT OnMessage(WPARAM length, LPARAM lpara);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW2 };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	CNetHandle * m_pNet;
	CWinThread* m_pMonitorThread;
	bool mContinueousMonitor;


	virtual void OnInitialUpdate();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivateView(BOOL bActivate, CFormView* pActivateView, CFormView* pDeactiveView);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonConnect();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
};

