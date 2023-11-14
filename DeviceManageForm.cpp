// FormPage1.cpp: 구현 파일
//

#include "pch.h"
#include "DLP_ctrl.h"
#include "DeviceMAnageForm.h"
#include "Util.h"
#include "Typesdef.h"

using namespace std;

// FormPage1

IMPLEMENT_DYNCREATE(DeviceManageForm, CFormView)
extern bool CharToHex(char* pBuf, char NumDigit, UINT32* pVal);
extern tCmdSetInfo* gCmdTabs;
extern string gStrDataLabel[8];
extern UINT32 gNField;
extern UINT32 gNSets;

DeviceManageForm::DeviceManageForm()
	: CFormView(IDD_FORMVIEW1)
{

}

DeviceManageForm::~DeviceManageForm()
{
}

void DeviceManageForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARAMS, mListCmd);
	DDX_Control(pDX, IDC_CMDIDX_G, m_EdtIdx);
	DDX_Control(pDX, IDC_COMBO_CMD_TYPE, mComboCmdType);
	DDX_Control(pDX, IDC_COMBO_DEST_TYPE, mComboDest);
	DDX_Control(pDX, IDC_LIST_SET, mListSet);
	DDX_Control(pDX, IDC_PARAMS2, mListCmdSet);
}

BEGIN_MESSAGE_MAP(DeviceManageForm, CFormView)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PARAMS, &DeviceManageForm::OnLvnItemchangedParams)


	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	//ON_CBN_SELCHANGE(IDC_COMBO1, &DeviceManageForm::OnCbnSelchangeCombo1)

	ON_NOTIFY(NM_DBLCLK, IDC_PARAMS, &DeviceManageForm::OnNMDblclkParams)
	ON_NOTIFY(NM_CLICK, IDC_PARAMS, &DeviceManageForm::OnNMClickParams)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SET, &DeviceManageForm::OnNMClickListSet)
	ON_NOTIFY(NM_CLICK, IDC_PARAMS2, &DeviceManageForm::OnNMClickParams2)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &DeviceManageForm::OnBnClickedModify)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &DeviceManageForm::OnBnClickedAddCmd)
	ON_BN_CLICKED(IDC_BUTTON_CLR, &DeviceManageForm::OnBnClickedButtonClr)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &DeviceManageForm::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &DeviceManageForm::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_DELCMD, &DeviceManageForm::OnBnClickedButtonDelcmd)
	ON_BN_CLICKED(IDC_BUTTON_COPYTO, &DeviceManageForm::OnBnClickedButtonCopyto)
	ON_BN_CLICKED(IDC_BUTTON_INSERTTO, &DeviceManageForm::OnBnClickedButtonInsertto)
	ON_BN_CLICKED(IDC_BUTTON_SELCMD, &DeviceManageForm::OnBnClickedButtonSelcmd)
	ON_BN_CLICKED(IDC_BUTTON_SELCMD2, &DeviceManageForm::OnBnClickedButtonSelcmd2)
	ON_BN_CLICKED(IDC_BUTTON_INSERT2, &DeviceManageForm::OnBnClickedButtonInsert2)
	ON_BN_CLICKED(IDC_BUTTON_DELCMD2, &DeviceManageForm::OnBnClickedButtonDelcmd2)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &DeviceManageForm::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_SEND_CMD, &DeviceManageForm::OnBnClickedButtonSendCmd)
END_MESSAGE_MAP()


// FormPage1 진단

#ifdef _DEBUG
void DeviceManageForm::AssertValid() const
{
	//CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void DeviceManageForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormPage1 메시지 처리기



void DeviceManageForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString FieldName;
	CRect ListCtrlRect;
	

// Main Params Table Initilaize
	mRawCnt = gCmdTabs[0].nCmd;
	mListCmd.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
#if 1
	// Table Label Init
	gStrDataLabel[0] = "Index";  // For errornous CSV read, this field Hard coded. ASPL
	for (int i = 0; i < gNField	; i++)
	{
		FieldName = gStrDataLabel[i].c_str();
		mListCmd.InsertColumn(i, FieldName, LVCFMT_CENTER, gStrDataLabel[i].length() * 10);
	}
	mListCmd.SetColumnWidth(6, 175);
	
	// Set Position of list control
	mListCmd.GetWindowRect(ListCtrlRect);
	ScreenToClient(ListCtrlRect);
	ListCtrlRect.right = ListCtrlRect.right;
	ListCtrlRect.bottom = ListCtrlRect.bottom;
	mListCmd.MoveWindow(ListCtrlRect);

	// Display Content
	DisplayParams(&mListCmd ,&gCmdTabs[0],mRawCnt);
	// Combo control init
	mComboCmdType.AddString(_T("W"));
	mComboCmdType.AddString(_T("R"));
	mComboCmdType.AddString(_T("F"));
	// Combo control init
	mComboDest.AddString(_T("L"));
	mComboDest.AddString(_T("R"));
#endif

	mListSet.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	FieldName = _T("Index");
	mListSet.InsertColumn(0, FieldName, LVCFMT_CENTER, 50);

	FieldName = _T("CmdSet Name");
	mListSet.InsertColumn(1, FieldName, LVCFMT_CENTER, 200);

	FieldName = _T("Lines");
	mListSet.InsertColumn(2, FieldName, LVCFMT_CENTER, 50);

	FieldName = _T("Descriptions");
	mListSet.InsertColumn(3, FieldName, LVCFMT_CENTER, 250);

#if 1
	for (int i = 1; i < gNSets; i++) // Index 0 is for Main Params Table
	{
		FieldName.Format(_T("%d"), i);
		mListSet.InsertItem(i-1, FieldName);
		
		FieldName = (LPSTR)gCmdTabs[i].strTableName;
		mListSet.SetItemText(i-1, 1, FieldName);

		FieldName.Format(_T("%d"), gCmdTabs[i].nCmd);
		mListSet.SetItemText(i - 1, 2, FieldName);

		FieldName = (LPSTR)gCmdTabs[i].strDesc;
		mListSet.SetItemText(i - 1, 3, FieldName);
	}
	mCmdSetSelectIdx = 1; // 
	mListCmdSet.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	for (int i = 0; i < gNField; i++)
	{
		FieldName = gStrDataLabel[i].c_str();
		mListCmdSet.InsertColumn(i, FieldName, LVCFMT_CENTER, gStrDataLabel[i].length() * 10);
	}
	mListCmdSet.SetColumnWidth(6, 175);
	mRawCnt_Set = gCmdTabs[mCmdSetSelectIdx].nCmd;
	DisplayParams(&mListCmdSet ,&gCmdTabs[mCmdSetSelectIdx], mRawCnt_Set);


#endif
	mSelectIdx = -1;
	mCopyIdx = -1;
	mSelectIdx2 = -1;
	mCrossCopyIdx = -1;
	mCurrentSetId = 0;

}



void DeviceManageForm::OnActivateView(BOOL bActivate, CFormView* pActivateView, CFormView* pDeactiveView)
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		Invalidate();
	}
}

void DeviceManageForm::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);

	Invalidate(); // 활성화 시 화면 갱신
}

#if 1
void DeviceManageForm::OnPaint()
{
	CFormView::OnPaint(); // FormView 클래스의 OnPaint() 함수 호출
	CWnd::BringWindowToTop();
}
#endif

#if 0
BOOL DeviceManageForm::OnEraseBkgnd(CDC* pDC)
{
	// 그리기 영역을 지운다.
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, GetSysColor(COLOR_BTNFACE));

	return TRUE;
}
#endif

void DeviceManageForm::GetParam(CListCtrl* ListControl, int index)
{
	CString temp;
	int CmdSel = 0;
	int DestSel = 0;

	temp = ListControl->GetItemText(index, 0);
	m_EdtIdx.SetWindowTextW(temp);
	temp = ListControl->GetItemText(index, 1);
	GetDlgItem(IDC_EDIT_CMD_NAME)->SetWindowTextW(temp);

	temp = ListControl->GetItemText(index, 2);
	if (!temp.CompareNoCase(_T("W")))
		CmdSel = iWrite;
	else if (!temp.CompareNoCase(_T("R")))
		CmdSel = iRead;
	else if (!temp.CompareNoCase(_T("F")))
		CmdSel = iFlash;
	else
		CmdSel = -1;
	mComboCmdType.SetCurSel(CmdSel);


	temp = ListControl->GetItemText(index, 3);
	if (!temp.CompareNoCase(_T("L")))
		DestSel = iLeft;
	else if (!temp.CompareNoCase(_T("R")))
		DestSel = iRight;
	else
		DestSel = -1;
	mComboDest.SetCurSel(DestSel);

	temp = ListControl->GetItemText(index, 4);
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowTextW(temp);
	temp = ListControl->GetItemText(index, 5);
	GetDlgItem(IDC_EDIT_VALUE)->SetWindowTextW(temp);
	temp = ListControl->GetItemText(index, 6);
	GetDlgItem(IDC_EDIT_DESC)->SetWindowTextW(temp);
}

void DeviceManageForm::SetParam(CListCtrl* ListControl, tCmdSetInfo* pCmdTabs, int SelectIdx)
{
	CString FieldName;
	GetDlgItemTextW(IDC_EDIT_CMD_NAME, FieldName);
	ListControl->SetItemText(SelectIdx, 1, FieldName);
	strcpy_s(pCmdTabs->CmdsInfoTab[SelectIdx].strCmdName, CT2A(FieldName));

	int CmdSel = mComboCmdType.GetCurSel();
	switch (CmdSel)
	{
	case iWrite:
		FieldName.Format(_T("W"));
		break;
	case iRead:
		FieldName.Format(_T("R"));
		break;
	case iFlash:
		FieldName.Format(_T("F"));
		break;
	default:
		FieldName.Format(_T(""));
		break;
	}
	ListControl->SetItemText(SelectIdx, 2, FieldName);
	strcpy_s(pCmdTabs->CmdsInfoTab[SelectIdx].strCmdType, CT2A(FieldName));
	pCmdTabs->CmdsInfoTab[SelectIdx].nCmdType = CmdSel;

	CmdSel = mComboDest.GetCurSel();
	switch (CmdSel)
	{
	case iLeft:
		FieldName.Format(_T("L"));
		break;
	case iRight:
		FieldName.Format(_T("R"));
		break;
	default:
		FieldName.Format(_T(""));
		break;
	}
	mListCmd.SetItemText(SelectIdx, 3, FieldName);
	strcpy_s(pCmdTabs->CmdsInfoTab[SelectIdx].strDestiNation, CT2A(FieldName));
	pCmdTabs->CmdsInfoTab[SelectIdx].nDestType = CmdSel;


	GetDlgItemTextW(IDC_EDIT_ADDRESS, FieldName);
	ListControl->SetItemText(SelectIdx, 4, FieldName);
	strcpy_s(pCmdTabs->CmdsInfoTab[SelectIdx].strRegAddress, CT2A(FieldName));
	UINT32 RegVal = 0;
	if (!CharToHex(pCmdTabs->CmdsInfoTab[SelectIdx].strRegAddress, 4, &RegVal))
	{
		pCmdTabs->CmdsInfoTab[SelectIdx].nCmdType = iErrorReg;
		RegVal = 0;
	}
	pCmdTabs->CmdsInfoTab[SelectIdx].nRegAddress = RegVal & 0xFFFF;


	if (pCmdTabs->CmdsInfoTab[SelectIdx].nCmdType == iWrite)
	{
		GetDlgItemTextW(IDC_EDIT_VALUE, FieldName);
	}
	else
	{
		SetDlgItemTextW(IDC_EDIT_VALUE, _T(""));
		FieldName = "";
	}
	ListControl->SetItemText(SelectIdx, 5, FieldName);
	strcpy_s(pCmdTabs->CmdsInfoTab[SelectIdx].strRegData, CT2A(FieldName));
	RegVal = 0;
	if (pCmdTabs->CmdsInfoTab[SelectIdx].nCmdType == iWrite)
	{

		if (!CharToHex(pCmdTabs->CmdsInfoTab[SelectIdx].strRegData, 8, &RegVal))
		{
			pCmdTabs->CmdsInfoTab[SelectIdx].nCmdType = iErrorVal;
			RegVal = 0;
		}
		pCmdTabs->CmdsInfoTab[SelectIdx].nRegData = RegVal & 0xFFFF;
	}
	else
	{
		pCmdTabs->CmdsInfoTab[SelectIdx].nRegData = 0;
	}

	GetDlgItemTextW(IDC_EDIT_DESC, FieldName);
	ListControl->SetItemText(SelectIdx, 6, FieldName);
	strcpy_s(pCmdTabs->CmdsInfoTab[SelectIdx].strDesc, CT2A(FieldName));
}

void DeviceManageForm::DisplayParams(CListCtrl* ListControl, tCmdSetInfo* pCmdTabs, UINT32 nIdx)
{
	CString FieldName;

	for (int i = 0; i < nIdx; i++)
	{
		FieldName.Format(_T("%d"), pCmdTabs->CmdsInfoTab[i].index);
		ListControl->InsertItem(i, FieldName);

		FieldName = (LPSTR)pCmdTabs->CmdsInfoTab[i].strCmdName;
		ListControl->SetItemText(i, 1, FieldName);

		FieldName = (LPSTR)pCmdTabs->CmdsInfoTab[i].strCmdType;
		ListControl->SetItemText(i, 2, FieldName);

		FieldName = (LPSTR)pCmdTabs->CmdsInfoTab[i].strDestiNation;
		ListControl->SetItemText(i, 3, FieldName);

		FieldName = (LPSTR)pCmdTabs->CmdsInfoTab[i].strRegAddress;
		ListControl->SetItemText(i, 4, FieldName);

		FieldName = (LPSTR)pCmdTabs->CmdsInfoTab[i].strRegData;
		ListControl->SetItemText(i, 5, FieldName);

		FieldName = (LPSTR)pCmdTabs->CmdsInfoTab[i].strDesc;
		ListControl->SetItemText(i, 6, FieldName);
	}
}


void DeviceManageForm::ClearParams(CListCtrl* ListControl, UINT32 nIdx)
{
	for (int i = nIdx - 1; i >= 0; i--)
	{
		ListControl->DeleteItem(i);
	}

}


void DeviceManageForm::OnLvnItemchangedParams(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void DeviceManageForm::OnNMDblclkParams(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString temp;
	int ClickIndex;

	UpdateData(true);
	ClickIndex = mListCmd.GetSelectionMark();
	GetParam(&mListCmd,ClickIndex);
	mSelectIdx = ClickIndex;
	mCurrentSetId = 0;
	*pResult = 0;
}

void DeviceManageForm::OnNMClickParams(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ClickIndex;

	UpdateData(true);
	mCurrentSetId = 0;
	ClickIndex = mListCmd.GetSelectionMark();
	if (mCopyIdx >= 0 && mCopyIdx < mRawCnt)
	{
		SetParam(&mListCmd, &gCmdTabs[0],ClickIndex);
		mCopyIdx = -1;
	}
	else
	{
		GetParam(&mListCmd,ClickIndex);
		mSelectIdx = ClickIndex;
	}
	*pResult = 0;
}

void DeviceManageForm::OnNMClickListSet(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ClickIndex;
	UpdateData(true);

	ClickIndex = mListSet.GetSelectionMark();
	mCmdSetSelectIdx = ClickIndex + 1;
	ClearParams(&mListCmdSet, mRawCnt_Set);
	mRawCnt_Set = gCmdTabs[mCmdSetSelectIdx].nCmd;
	DisplayParams(&mListCmdSet, &gCmdTabs[mCmdSetSelectIdx], mRawCnt_Set);
	*pResult = 0;
}

void DeviceManageForm::OnBnClickedModify()
{
	if (mSelectIdx >= 0)
		SetParam(&mListCmd, &gCmdTabs[0], mSelectIdx);
	else 
		AfxMessageBox(_T("No Command Selected"));

}

void DeviceManageForm::OnBnClickedAddCmd()
{
	// TODO: 프로토콜 테스트
	CString FieldName;
	int NewItemIndex = mRawCnt;
	{
		FieldName.Format(_T("%d"), NewItemIndex+1);
		mListCmd.InsertItem(NewItemIndex, FieldName);
		gCmdTabs[0].nCmd++;
		gCmdTabs[0].CmdsInfoTab = (tCmdInfo*)realloc(gCmdTabs[0].CmdsInfoTab,sizeof(tCmdInfo) * gCmdTabs[0].nCmd);
		SetParam(&mListCmd, &gCmdTabs[0], NewItemIndex);
		mRawCnt++;
	}
}

void DeviceManageForm::OnBnClickedButtonInsert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gCmdTabs[0].nCmd++;
	gCmdTabs[0].CmdsInfoTab = (tCmdInfo*)realloc(gCmdTabs[0].CmdsInfoTab, sizeof(tCmdInfo) * gCmdTabs[0].nCmd);
	// 0 1 2 3 4 5  ,1 6
	//               5,6 4,5 3,4 2,3 1,2 

	for (int i = mRawCnt - 1; i >= mSelectIdx; i--)
	{
		memcpy((void*)&gCmdTabs[0].CmdsInfoTab[i + 1], (void*)&gCmdTabs[0].CmdsInfoTab[i], sizeof(tCmdInfo));
		gCmdTabs[0].CmdsInfoTab[i + 1].index = i + 2;
	}
	SetParam(&mListCmd, &gCmdTabs[0], mSelectIdx+1);
	ClearParams(&mListCmd, mRawCnt);
	mRawCnt++;
	DisplayParams(&mListCmd ,&gCmdTabs[0], mRawCnt);
}

void DeviceManageForm::OnBnClickedButtonDelcmd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = mSelectIdx; i < mRawCnt - 1; i++)
	{
		memcpy((void*)&gCmdTabs[0].CmdsInfoTab[i], (void*)&gCmdTabs[0].CmdsInfoTab[i + 1], sizeof(tCmdInfo));
		gCmdTabs[0].CmdsInfoTab[i].index = i + 1;
	}
	gCmdTabs[0].nCmd--;
	gCmdTabs[0].CmdsInfoTab = (tCmdInfo*)realloc(gCmdTabs[0].CmdsInfoTab, sizeof(tCmdInfo) * gCmdTabs[0].nCmd);

	ClearParams(&mListCmd, mRawCnt);
	mRawCnt--;
	DisplayParams(&mListCmd, &gCmdTabs[0], mRawCnt);
}

void DeviceManageForm::OnBnClickedButtonClr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString FieldName;

	FieldName = _T("");
	SetDlgItemTextW(IDC_CMDIDX_G, FieldName);
	SetDlgItemTextW(IDC_EDIT_CMD_NAME, FieldName);
	SetDlgItemTextW(IDC_EDIT_ADDRESS, FieldName);
	SetDlgItemTextW(IDC_EDIT_VALUE, FieldName);
	SetDlgItemTextW(IDC_EDIT_DESC, FieldName);
	mComboCmdType.SetCurSel(-1);
	mComboDest.SetCurSel(-1);
}

void DeviceManageForm::OnBnClickedButtonCopy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mCopyIdx = mSelectIdx;
}



UINT MonitorFunc(LPVOID pParam)
{
#if 0
	UINT16 arr[4]{ 0, };
	INT16 tmp;
	INT32 index;

	DeviceManageForm* pDlg = (DeviceManageForm*)pParam;


	while (pDlg->mContinueousMonitor)
	{
		if (*(pDlg->p_hNet) == -1) 
		{
			AfxMessageBox(_T("Can Socket not opened"));
			pDlg->mContinueousMonitor = FALSE;
			pDlg->GetDlgItem(IDC_BUTTON3)->SetWindowTextW(_T("MONITOR"));
			return 0;
		}

		for (index = 0; index < Table10Context.UnitItemCnt; index++)
		{
			pDlg->m_Protocols.ReadCmd(*(pDlg->p_hNet), pDlg->m_pNetHandle, arr, index, pDlg->m_FocusCanID);
			Table10Context.pArrayUnitItems[index].Current = arr[0];
			CString str;
			str.Format(L"%d", Table10Context.pArrayUnitItems[index].Current);
			pDlg->m_ParamList.SetItemText(index, 5, str);
		}

		index = pDlg->GetDlgItemInt(IDC_EDIT6);
		pDlg->SetDlgItemInt(IDC_EDIT1, Table10Context.pArrayUnitItems[index].Current);
		Sleep(1);
	}
#endif
	return 0;
}

void DeviceManageForm::OnBnClickedButtonCopyto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mCrossCopyIdx = mSelectIdx;
}


void DeviceManageForm::OnNMClickParams2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ClickIndex;

	mCurrentSetId = mCmdSetSelectIdx;
	UpdateData(true);
	ClickIndex = mListCmdSet.GetSelectionMark();
	if (mCrossCopyIdx >= 0 && mCrossCopyIdx < mRawCnt)
	{
		SetParam(&mListCmdSet, &gCmdTabs[1], ClickIndex);
		mCrossCopyIdx = -1;
	}
	mSelectIdx2 = ClickIndex;
	*pResult = 0;
}


void DeviceManageForm::OnBnClickedButtonInsertto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos=mListCmd.GetFirstSelectedItemPosition();
	UINT uSelectedCount = mListCmd.GetSelectedCount();
	UINT * SelectedList;

	SelectedList = (UINT*)calloc(uSelectedCount,sizeof(UINT));

	int i = 0;
	while (pos)
	{
		int nSelected = mListCmd.GetNextSelectedItem(pos);
		SelectedList[i++] = nSelected;
	}
// 
	mRawCnt_Set = gCmdTabs[mCmdSetSelectIdx].nCmd;
	
	gCmdTabs[mCmdSetSelectIdx].nCmd+=uSelectedCount;
	gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab = (tCmdInfo*)realloc(gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab, sizeof(tCmdInfo) * gCmdTabs[mCmdSetSelectIdx].nCmd);

	for (int i = mRawCnt_Set - 1; i >= mSelectIdx2; i--)
	{
		memcpy((void*)&gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i + uSelectedCount], (void*)&gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i], sizeof(tCmdInfo));
		gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i + uSelectedCount].index = i + uSelectedCount-1;
	}
	for (int i = 0; i < gCmdTabs[mCmdSetSelectIdx].nCmd; i++)
	{
		gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i].index = i+1;
	}

	ClearParams(&mListCmdSet, mRawCnt_Set);
	mRawCnt_Set = gCmdTabs[mCmdSetSelectIdx].nCmd;
	//

	for (i = 0; i < uSelectedCount; i++)
	{
		GetParam(&mListCmd,SelectedList[i]);
		SetParam(&mListCmdSet, &gCmdTabs[mCmdSetSelectIdx], mSelectIdx2+1+i);
	}

	DisplayParams(&mListCmdSet, &gCmdTabs[mCmdSetSelectIdx], mRawCnt_Set);

	free(SelectedList);

	CString FieldName;
	FieldName.Format(_T("%d"), gCmdTabs[mCmdSetSelectIdx].nCmd);
	mListSet.SetItemText(mCmdSetSelectIdx - 1, 2, FieldName);
}

void DeviceManageForm::OnBnClickedButtonSelcmd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetParam(&mListCmdSet, mSelectIdx2);
	
}


void DeviceManageForm::OnBnClickedButtonSelcmd2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetParam(&mListCmdSet, &gCmdTabs[mCmdSetSelectIdx], mSelectIdx2);

}

void DeviceManageForm::OnBnClickedButtonInsert2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gCmdTabs[mCmdSetSelectIdx].nCmd++;
	gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab = (tCmdInfo*)realloc(gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab, sizeof(tCmdInfo) * gCmdTabs[mCmdSetSelectIdx].nCmd);

	for (int i = mRawCnt_Set - 1; i >= mSelectIdx2; i--)
	{
		memcpy((void*)&gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i + 1], (void*)&gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i], sizeof(tCmdInfo));
		gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i + 1].index = i + 2;
	}
	SetParam(&mListCmdSet, &gCmdTabs[mCmdSetSelectIdx], mSelectIdx2 + 1);
	ClearParams(&mListCmdSet, mRawCnt_Set);
	mRawCnt_Set= gCmdTabs[mCmdSetSelectIdx].nCmd;
	DisplayParams(&mListCmdSet, &gCmdTabs[mCmdSetSelectIdx], mRawCnt_Set);

	CString FieldName;
	FieldName.Format(_T("%d"), gCmdTabs[mCmdSetSelectIdx].nCmd);
	mListSet.SetItemText(mCmdSetSelectIdx - 1, 2, FieldName);
}


void DeviceManageForm::OnBnClickedButtonDelcmd2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = mSelectIdx2; i < mRawCnt_Set - 1; i++)
	{
		memcpy((void*)&gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i], (void*)&gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i + 1], sizeof(tCmdInfo));
		gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab[i].index = i + 1;
	}
	gCmdTabs[mCmdSetSelectIdx].nCmd--;
	gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab = (tCmdInfo*)realloc(gCmdTabs[mCmdSetSelectIdx].CmdsInfoTab, sizeof(tCmdInfo) * gCmdTabs[mCmdSetSelectIdx].nCmd);

	ClearParams(&mListCmdSet, mRawCnt_Set);
	mRawCnt_Set= gCmdTabs[mCmdSetSelectIdx].nCmd;
	DisplayParams(&mListCmdSet, &gCmdTabs[mCmdSetSelectIdx], mRawCnt_Set);

	// Update Count
	CString FieldName;
	FieldName.Format(_T("%d"), gCmdTabs[mCmdSetSelectIdx].nCmd);
	mListSet.SetItemText(mCmdSetSelectIdx - 1, 2, FieldName);

}






#if 0
UINT MonitorFuncDeviceMg(LPVOID pParam)
{

	UINT16 arr[4]{ 0, };
	INT16 tmp;
	INT32 len;

	DeviceManageForm* pDlg = (DeviceManageForm*)pParam;

	CEdit* pMsgEdt = (CEdit*)pDlg->GetDlgItem(IDC_EDIT_DESC2);
	CString strMsg = _T("HEX>> ");

	while (pDlg->mContinueousMonitor)
	{
		len = pDlg->m_pNet->UdpReceive();
		if (len > 0)
		{
			for (int i = 0; i < len; i++)
			{
				CString ByteData;

				ByteData.Format(_T("%2x"), pDlg->m_pNet->m_RcvData[i]);

				strMsg += ByteData;
			}

			strMsg += _T("   is rcvd_ \r\n");

			UpdateLines(pMsgEdt, strMsg);
			strMsg = _T("HEX>> ");
		}
	}
	return 0;
}
#endif

void DeviceManageForm::OnBnClickedButtonSendCmd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	INT32 Idx;
	UINT16 Cmd;
	UINT16 nCmd;
	char cmdData[32];

	Idx = GetDlgItemInt(IDC_CMDIDX_G);
	// Get Address;
	cmdData[1] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[Idx - 1].nRegAddress) & 0xFF;
	cmdData[2] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[Idx - 1].nRegAddress >> 8) & 0xFF;
	// Command
	Cmd = gCmdTabs[mCurrentSetId].CmdsInfoTab[Idx - 1].nCmdType;
	switch (Cmd)
	{
	case iWrite:
		cmdData[0] = 'W';
		cmdData[3] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[Idx - 1].nRegData) & 0xFF;
		cmdData[4] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[Idx - 1].nRegData >> 8) & 0xFF;
		cmdData[5] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[Idx - 1].nRegData >> 16) & 0xFF;
		cmdData[6] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[Idx - 1].nRegData >> 24) & 0xFF;
		nCmd = 7;
		break;
	case iRead:
		cmdData[0] = 'R';
		nCmd = 3;
		break;
	case iFlash:
		cmdData[0] = 'F';
		nCmd = 1;
		break;
	default:
		return;
	}

	CString IPs = _T("192.168.1.200");
	CString PORTs = _T("3002");
	CString strMsg;

	CEdit* pMsgEdt =(CEdit *) GetDlgItem(IDC_EDIT_DESC2);

	m_pNet->UdpSend(IPs, PORTs, cmdData, nCmd);

	nCmd = m_pNet->UdpReceive();
	if (nCmd > 0)
	{
		for (int i = 0; i < nCmd; i++)
		{
			CString ByteData;

			ByteData.Format(_T(" %02x"), m_pNet->m_RcvData[i]);

			strMsg += ByteData;
		}

		strMsg += _T("   is rcvd_ \r\n");

		UpdateLines(pMsgEdt, strMsg);
		strMsg = _T("HEX>> ");




	}
}


void DeviceManageForm::OnBnClickedButtonRun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	INT32 Idx;
	UINT16 Cmd;
	UINT16 nCmd;
	char cmdData[32];

	//Idx = GetDlgItemInt(IDC_CMDIDX_G);

	for (int i = 0; i < gCmdTabs[mCurrentSetId].nCmd; i++)
	{
		// Get Address;
		cmdData[1] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[i].nRegAddress) & 0xFF;
		cmdData[2] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[i].nRegAddress >> 8) & 0xFF;
		// Command
		Cmd = gCmdTabs[mCurrentSetId].CmdsInfoTab[i].nCmdType;
		switch (Cmd)
		{
		case iWrite:
			cmdData[0] = 'W';
			cmdData[3] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[i].nRegData) & 0xFF;
			cmdData[4] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[i].nRegData >> 8) & 0xFF;
			cmdData[5] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[i].nRegData >> 16) & 0xFF;
			cmdData[6] = (gCmdTabs[mCurrentSetId].CmdsInfoTab[i].nRegData >> 24) & 0xFF;
			nCmd = 7;
			break;
		case iRead:
			cmdData[0] = 'R';
			nCmd = 3;
			break;
		case iFlash:
			cmdData[0] = 'F';
			nCmd = 1;
			break;
		default:
			return;
		}

		CString IPs = _T("192.168.1.200");
		CString PORTs = _T("3002");
		CString strMsg=_T("HEX>> ");;

		CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_DESC2);

		m_pNet->UdpSend(IPs, PORTs, cmdData, nCmd);

		nCmd = m_pNet->UdpReceive();
		if (nCmd > 0)
		{
			for (int i = 0; i < nCmd; i++)
			{
				CString ByteData;

				ByteData.Format(_T(" %02x"), m_pNet->m_RcvData[i]);

				strMsg += ByteData;
			}

			strMsg += _T("   is rcvd_ \r\n");

			UpdateLines(pMsgEdt, strMsg);
			strMsg = _T("HEX>> ");
		}

		Sleep(10);

	}


}