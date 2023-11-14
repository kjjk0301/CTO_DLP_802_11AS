#pragma once
#include "DeviceManageForm.h"
#include "NetConnectionForm.h"
#include "FirmwareUpdateForm.h"
//#include "CCanKvaser.h"


// MainForm 폼 보기

class MainForm : public CFormView
{
	DECLARE_DYNCREATE(MainForm)

protected:
	MainForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	
	virtual ~MainForm();

public:
	CNetHandle* m_pNet;

	NetConnectionForm* m_pform2;

	DeviceManageForm* m_pform1;
	FirmwareUpdateForm* m_pform3;


//	CCanKvaser* m_pKvCan;

	MainForm(UINT id);



#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW };
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
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	// Tab control
	CTabCtrl m_Tab;
	virtual void OnInitialUpdate();
};


