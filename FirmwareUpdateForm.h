#pragma once
#include "CFileParser.h"
#include "CNetHandle.h"
#include "CProtocols.h"

// CanConnectForm 폼 보기

class FirmwareUpdateForm : public CFormView
{
	DECLARE_DYNCREATE(FirmwareUpdateForm)

protected:

public:
	FirmwareUpdateForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~FirmwareUpdateForm();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW3 };
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
	CFileParser m_FileParser;
	CProtocols m_Protocols;


	CNetHandle* m_pNet;
//	INT32  m_hCan;

	bool m_FileLoad;
	UINT16 m_ProgressCnt;

	virtual void OnInitialUpdate();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedToImage();
	
	CTreeCtrl m_TreeBin;
	CProgressCtrl m_Progress;
	UINT16 m_FocusCanID;
	
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	

	LRESULT OnMessageProgress(WPARAM length, LPARAM lpara);
};


