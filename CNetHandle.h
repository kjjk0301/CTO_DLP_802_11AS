#pragma once
#include "canlib.h"
#include "afxwin.h"
#include <iostream>
#include <winsock2.h> // 윈속 헤더 포함 
#include <windows.h> 
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib") // 윈속 라이브러리 링크



#define LOCAL_PORT   2470 // 서버 RCV  포트번호



enum UDP_INIT_STAT
{
	UDP_OK = 0,
	WSA_ERROR,
	SOCK_ERROR,
	BIND_ERROR,
};

#define WM_CANMESSAGE	(WM_USER+1)
#define WM_PROGRESS		(WM_USER+2)

#define GUI_ENV 1
#define CAN_BUF_SIZE 8
#define CAN_RD_RETRY 3


typedef struct KvCanChannelList
{
	UINT16 Num;
	UINT16 ChId;
}KvCanChannelList;

typedef struct KvCanFormat
{
	long ID;
	unsigned int DLC;
	unsigned int FLAGS;
	unsigned long TIME;
	unsigned char MSG[8];
}KvCanFormat;


class CNetHandle
{
public:
	CNetHandle();
	virtual ~CNetHandle();


public:
	HWND	m_hWnd_net;
	CString m_NetCtrlMessage;
	UINT16 m_Progress;
	UINT16 m_NumChan;
	INT32  m_hNet;
	INT32  m_nRcvLen;
	UINT8  m_RcvData[1024];

	SOCKET mServerSocket1; // 소켓 선언


	CString m_strIPL;
	CString m_strPORTL;

	CString m_strIPR;
	CString m_strPORTR;

	CString m_strBINDIP;
	CString m_strBINDPORT;


	int OpenNetHandle(HWND hWnd);
	int OpenNetwork(CString strIPs, CString strPORTs);
	void CloseNet();
	int UdpSend(CString strIPs, CString strPORTs, char* pBuf, int Size);

	int UdpReceive(void);

#if 1

	
#endif
};


