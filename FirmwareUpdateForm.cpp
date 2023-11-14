// CanConnectForm.cpp: 구현 파일
//

#include "pch.h"
#include "DLP_ctrl.h"
#include "FirmwareUpdateForm.h"
#include "Util.h"
#include "Typesdef.h"


// CanConnectForm

IMPLEMENT_DYNCREATE(FirmwareUpdateForm, CFormView)

FirmwareUpdateForm::FirmwareUpdateForm()
	: CFormView(IDD_FORMVIEW3)
{

}

FirmwareUpdateForm::~FirmwareUpdateForm()
{
}

void FirmwareUpdateForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_FWTREE, m_TreeBin);
//	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}

BEGIN_MESSAGE_MAP(FirmwareUpdateForm, CFormView)
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &FirmwareUpdateForm::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &FirmwareUpdateForm::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_TOAPP3, &FirmwareUpdateForm::OnBnClickedToImage)

	ON_MESSAGE(WM_PROGRESS, &FirmwareUpdateForm::OnMessageProgress)

END_MESSAGE_MAP()




#ifdef _DEBUG
void FirmwareUpdateForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FirmwareUpdateForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CanConnectForm 메시지 처리기


void FirmwareUpdateForm::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFormView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void FirmwareUpdateForm::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	CFormView::OnPaint(); // FormView 클래스의 OnPaint() 함수 호출
	CWnd::BringWindowToTop();
}


void FirmwareUpdateForm::OnInitialUpdate()
{
//	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_TreeBin.SubclassDlgItem(IDC_FWTREE, this);
	m_Progress.SubclassDlgItem(IDC_PROGRESS1, this);
	m_Progress.SetRange(0, 100);
	m_FocusCanID = 0x103;

}


void FirmwareUpdateForm::OnBnClickedButtonFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일( *.BIN) | *.BIN;|모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.bin"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())

	{
		//CString pathName = dlg.GetPathName();
		m_FileParser.FileName = dlg.GetPathName();
		MessageBox(m_FileParser.FileName);

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FILE);
		pCombo->AddString(m_FileParser.FileName);
		pCombo->SetCurSel(0);
	}

}


void FirmwareUpdateForm::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_FileLoad)
	{


//		bool ParseOK = m_FileParser.OpenFirmware(m_FileParser.FileName);
		bool ParseOK = m_FileParser.OpenImage(m_FileParser.FileName);

		if (ParseOK)
		{
			m_FileLoad = true;
			GetDlgItem(IDC_BUTTON_LOAD)->SetWindowTextW(_T("CLOSE"));
		}
		else
		{
			MessageBox(_T("File Load Error"), MB_OK);
		}

	}
	else // unload file
	{
		free(m_FileParser.m_pFwBuf);
		m_FileParser.m_pFwBuf = NULL;
		CloseHandle(m_FileParser.m_hFile);
		m_FileLoad = false;

		GetDlgItem(IDC_BUTTON_LOAD)->SetWindowTextW(_T("LOAD"));
	}
}

void FirmwareUpdateForm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}






void FirmwareUpdateForm::OnBnClickedToImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD SentSize = 0;
	int nCmd;
	unsigned char* pPtr;


	CString Itemname;
	CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_STAT);

	if (m_Protocols.EraseMMC(m_pNet))
	{
		Itemname.Format(_T("Erase MMC Success  \r\n"));
		UpdateLines(pMsgEdt, Itemname);

		pPtr = m_FileParser.m_pFwBuf;
		while (SentSize < m_FileParser.m_FileSz)
		{
			int ImgCnt = 0;
			for (int i = 0; i < TRANSFER_NUM; i++)
			{
				bool bDown=m_Protocols.SendImagePacket(m_pNet, pPtr, i);
				if (bDown)
				{
					Itemname.Format(_T("Rcvd Ack %X %X %X %X  \r\n"), m_pNet->m_RcvData[0], m_pNet->m_RcvData[1], m_pNet->m_RcvData[2], m_pNet->m_RcvData[3]);
				}
				else
				{
					Itemname.Format(_T("Rcvd Ack Fail  \r\n"));
				}
				UpdateLines(pMsgEdt, Itemname);


				//Sleep(10);
#if 0
				if (m_Protocols.SendImagePacket(m_pNet, pPtr, i))
				{
					Itemname.Format(_T("Rcvd Ack %X %X %X %X  \r\n"), m_pNet->m_RcvData[0], m_pNet->m_RcvData[1], m_pNet->m_RcvData[2], m_pNet->m_RcvData[3]);
				}
				else
				{
					Itemname.Format(_T("Rcvd Ack Fail  \r\n"));
				}
				UpdateLines(pMsgEdt, Itemname);
#endif
			}

/*
			if (m_Protocols.PacketCheck(m_pNet))
			{
				Itemname.Format(_T("Image Done %d \r\n"), ImgCnt);
			}
			else
			{
				Itemname.Format(_T("Rcvd Ack Fail  \r\n"));
			}
			UpdateLines(pMsgEdt, Itemname);
*/

			if (m_Protocols.EraseMMC(m_pNet))
			{
				Itemname.Format(_T("Image writing Done %d \r\n"), ImgCnt);
			}
			else
			{
				Itemname.Format(_T("Image writing Fail %d \r\n"), ImgCnt);
			}
			UpdateLines(pMsgEdt, Itemname);



//			m_Protocols.MmdWriteCmd();
//			m_pNet->UdpSend(m_pNet->m_strIPL, m_pNet->m_strPORTL, (char*)m_Protocols.m_CMDS, 4);
//			nCmd = m_pNet->UdpReceive();

			ImgCnt++;
			pPtr += 40448;
			SentSize += 40448;

		}
	}
	else
	{
		Itemname.Format(_T("Erase MMC fail  \r\n"));
		UpdateLines(pMsgEdt, Itemname);
	}


#if 0
	m_Protocols.MmdWriteStardCmd();
	m_pNet->UdpSend(m_pNet->m_strIPL, m_pNet->m_strPORTL, (char*)m_Protocols.m_CMDS, 4);
	nCmd = m_pNet->UdpReceive();

	if (nCmd > 0)
	{

		Itemname.Format(_T("Rcvd Ack Len %d  %X %X %X %X  \r\n"), nCmd, m_pNet->m_RcvData[0], m_pNet->m_RcvData[1], m_pNet->m_RcvData[2], m_pNet->m_RcvData[3]);


	}
	else
	{
		Itemname.Format(_T("Rcvd Ack Fail  \r\n"));


	}
	UpdateLines(pMsgEdt, Itemname);
#endif
	

#if 0
	pPtr = m_FileParser.m_pFwBuf;
	while (SentSize < m_FileParser.m_FileSz)
	{



		for (int i = 0; i < 79; i++)
		{	
			if(i == 0)
				m_Protocols.MakePayload(pPtr,0,i, 512);	   // Start of chunk
			else  if(i == 78)
				m_Protocols.MakePayload(pPtr, 10, i, 512); // End of chunk
			else
				m_Protocols.MakePayload(pPtr, 1, i, 512);  // Inbetween transfer

			m_pNet->UdpSend(m_pNet->m_strIPL, m_pNet->m_strPORTL, (char*)m_Protocols.m_CMDS, 512 + 8);
			nCmd = m_pNet->UdpReceive();
			if (nCmd > 0)
			{
				Itemname.Format(_T("Rcvd Ack Len %d  %X %X %X %X  \r\n"), nCmd, m_pNet->m_RcvData[0], m_pNet->m_RcvData[1], m_pNet->m_RcvData[2], m_pNet->m_RcvData[3]);
			}
			else
			{
				Itemname.Format(_T("Rcvd Ack Fail  \r\n"));
			}
			UpdateLines(pMsgEdt, Itemname);


		}

		m_Protocols.MmdWriteCmd();
		m_pNet->UdpSend(m_pNet->m_strIPL, m_pNet->m_strPORTL, (char*)m_Protocols.m_CMDS, 4);
		nCmd = m_pNet->UdpReceive();
		//			if (nCmd != 4)
		//			{
		//
		//
		//			}

		pPtr += 40448;
		SentSize += 40448;
	}
#endif
}


LRESULT FirmwareUpdateForm::OnMessageProgress(WPARAM length, LPARAM lpara)
{

//	m_ProgressCnt++;
//	m_Progress.SetPos((m_ProgressCnt * 100 / m_FileParser.m_BinInfo.NumSections));
	return 0;
}


#if 0

void FormPage2::OnBnClickedTobtl()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT16 res;

	//	res = m_Protocols.CheckDspStat(m_hCan, m_FocusCanID, m_pKvCan);

	CString Itemname;
	res = 0x0a;
	Itemname.Format(_T("CPU stat : %x \r\n"), res);

	CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_STAT);
	UpdateLine(pMsgEdt, Itemname);

	if (res == STAT_APP)
	{
		Itemname.Format(_T("Changing State \r\n"));
		UpdateLine(pMsgEdt, Itemname);
		//	Erase Flag
		bool Result = true;//m_Protocols.SwitchMode(m_hCan, m_FocusCanID, m_pKvCan, STAT_BTL);
		if (Result)
		{
			Itemname.Format(_T("Changing Sucess \r\n"));
			UpdateLine(pMsgEdt, Itemname);
		}
		else
		{
			Itemname.Format(_T("Changing Fail \r\n"));
			UpdateLine(pMsgEdt, Itemname);
		}
	}
	else if (res == STAT_BTL)
	{
		Itemname.Format(_T("in BTL State \r\n"));
		UpdateLine(pMsgEdt, Itemname);
	}
	else
	{
		Itemname.Format(_T("Undefined State \r\n"));
		UpdateLine(pMsgEdt, Itemname);
	}
}


void FormPage2::OnBnClickedDirup()
{
	bool State;
	int ReturnCode;
	CString strMsg;

	CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_STAT);

	//m_pKvCan->m_Progress = 0;
	m_ProgressCnt = 0;
	m_Progress.SetPos(0);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_hCan >= 0) {

		//State=m_Protocols.EraseAppArea(m_hCan, m_FocusCanID, m_pKvCan);
		State = true;
		if (!State)
		{
			strMsg.Format(_T("Erase Failed! \r\n"));
			UpdateLine(pMsgEdt, strMsg);
		}
		else
		{
			ReturnCode = m_Protocols.DnLoadFirmware(m_hCan, m_FocusCanID, m_pKvCan, &m_FileParser, IN_UPDATE);
			strMsg.Format(_T("DownFirmware end: %d \r\n"), ReturnCode);
			UpdateLine(pMsgEdt, strMsg);


		}
	}
	else
	{

		strMsg.Format(_T("Can Socket not opend \r\n"));
		UpdateLine(pMsgEdt, strMsg);
	}

}

void FormPage2::OnBnClickedToapp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// APP 으로 전환
	UINT16 res;
	res = 0x0b; m_Protocols.CheckDspStat(m_hCan, m_FocusCanID, m_pKvCan);

	CString Itemname;
	Itemname.Format(_T("CPU stat : %x \r\n"), res);

	CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_STAT);
	UpdateLine(pMsgEdt, Itemname);

	if (res == STAT_BTL)
	{
		Itemname.Format(_T("Changing State \r\n"));
		UpdateLine(pMsgEdt, Itemname);
		//	Erase Flag
		bool Result = true;//m_Protocols.SwitchMode(m_hCan, m_FocusCanID, m_pKvCan, STAT_APP);

		if (Result)
		{
			Itemname.Format(_T("Changing Sucess \r\n"));
			UpdateLine(pMsgEdt, Itemname);
		}
		else
		{
			Itemname.Format(_T("Changing Fail \r\n"));
			UpdateLine(pMsgEdt, Itemname);
		}
	}
	else if (res == STAT_APP)
	{
		Itemname.Format(_T("in APP State \r\n"));
		UpdateLine(pMsgEdt, Itemname);
	}
	else
	{
		Itemname.Format(_T("Undefined State \r\n"));
		UpdateLine(pMsgEdt, Itemname);
	}
}


void FormPage2::OnBnClickedImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bool State;
	int ReturnCode;
	CString strMsg;

	CEdit* pMsgEdt = (CEdit*)GetDlgItem(IDC_EDIT_STAT);

	//m_pKvCan->m_Progress = 0;
	m_ProgressCnt = 0;
	m_Progress.SetPos(0);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_hCan >= 0) {

		//State=m_Protocols.EraseAppArea(m_hCan, m_FocusCanID, m_pKvCan);
		State = true;
		if (!State)
		{
			strMsg.Format(_T("Erase Failed! \r\n"));
			UpdateLine(pMsgEdt, strMsg);
		}
		else
		{
			ReturnCode = m_Protocols.DnLoadImage(m_hCan, m_FocusCanID, m_pKvCan, &m_FileParser, IN_UPDATE);
			strMsg.Format(_T("DownFirmware end: %d \r\n"), ReturnCode);
			UpdateLine(pMsgEdt, strMsg);


		}
	}
	else
	{

		strMsg.Format(_T("Can Socket not opend \r\n"));
		UpdateLine(pMsgEdt, strMsg);
	}

}
#endif