#pragma once
#include "Typesdef.h"
#include "CNetHandle.h"
#include "CProtocols.h"
#include <string>

using namespace std;

extern ParamContext Table10Context;
extern string DataLable[];
// FormPage1 폼 보기

class DeviceManageForm : public CFormView
{
	DECLARE_DYNCREATE(DeviceManageForm)

protected:
	

public:
	DeviceManageForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~DeviceManageForm();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW1 };
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
	CNetHandle* m_pNet;
	CProtocols m_Protocols;

	// // PARAMETER LIST에 대한 control
	CListCtrl mListCmd;
	CListCtrl mListSet;
	CListCtrl mListCmdSet;
	CEdit m_EdtIdx;
	CComboBox mComboCmdType;
	CComboBox mComboDest;
	INT32 mSelectIdx;
	INT32 mSelectIdx2;
	INT32 mCopyIdx;
	INT32 mCrossCopyIdx;
	UINT16 mRawCnt;
	UINT16 mRawCnt_Set;
	INT32 mCmdSetSelectIdx;
	INT32 mCurrentSetId;


	CWinThread* m_pMonitorThread;
	virtual void OnInitialUpdate();

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivateView(BOOL bActivate, CFormView* pActivateView, CFormView* pDeactiveView);
	afx_msg void OnPaint();
	afx_msg void OnLvnItemchangedParams(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkParams(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickParams(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedAddCmd();
	afx_msg void OnBnClickedButtonClr();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonCopy();


	//static UINT MonitorFunc(LPVOID param);
	void GetParam(CListCtrl* ListControl, int index);
	void SetParam(CListCtrl* ListControl, tCmdSetInfo* pCmdTabs,int SelectIdx);
	void DisplayParams(CListCtrl * ListControl, tCmdSetInfo* pCmdTabs, UINT32 nIdx);
	void ClearParams(CListCtrl* ListControl, UINT32 nIdx);


	afx_msg void OnBnClickedButtonDelcmd();

	afx_msg void OnNMClickListSet(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonCopyto();
	afx_msg void OnNMClickParams2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonInsertto();
	afx_msg void OnBnClickedButtonSelcmd();
	afx_msg void OnBnClickedButtonSelcmd2();
	afx_msg void OnBnClickedButtonInsert2();
	afx_msg void OnBnClickedButtonDelcmd2();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonSendCmd();
};


