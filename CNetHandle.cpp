#include "pch.h"
#include "CNetHandle.h"


SOCKET tServerSocket1;


CNetHandle::CNetHandle()
{
	m_hNet = -1;
}

CNetHandle::~CNetHandle()
{

}


enum NET_ERR_TYPE
{
	NET_OK = 0,
	NET_OPEN_ERR = 1,
};


int CNetHandle::OpenNetHandle(HWND hWnd)
{
	m_hWnd_net = hWnd;

	m_NetCtrlMessage.Format(_T("Network Module Loaded\r\n"));
	SendMessage(m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);

	m_strBINDIP = _T("192.168.1.100");
	m_strBINDPORT = _T("1470");

	m_strIPL = _T("192.168.1.200");
	m_strPORTL = _T("3002");

	m_strIPR = _T("192.168.1.201");
	m_strPORTR = _T("3002");

	return NET_OK;
}




int CNetHandle::OpenNetwork(CString strIPs, CString strPORTs)
{
	// TODO: 여기에 구현 코드 추가.
	WSADATA wsaData; // 윈속 데이터 구조체
	SOCKADDR_IN LocalInfo; // 서버 주소정보 구조체
	char strIP[16];
	char strPORT[16];
	short LocalPort;// = LOCAL_PORT; // 서버 포트번호

	strcpy_s(strIP,   CT2A(strIPs));
	strcpy_s(strPORT, CT2A(strPORTs));
	LocalPort = atoi(strPORT);

	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) // WSAStartup 설정에서 문제 발생하면
	{
		m_NetCtrlMessage.Format(_T("Winsock InitError\r\n"));
		SendMessage(m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
		WSACleanup(); // WS2_32.DLL의 사용 끝냄
		m_hNet = -1;
		return WSA_ERROR;
	}

	memset(&LocalInfo, 0, sizeof(LocalInfo)); // 0으로 초기화

	LocalInfo.sin_family = AF_INET;
	inet_pton(AF_INET, strIP, &(LocalInfo.sin_addr.s_addr));
	LocalInfo.sin_port = htons(LocalPort); // 포트번호

	tServerSocket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // udp용 소켓 생성. SOCK_DGRAM : UDP 사용
	if (tServerSocket1 == INVALID_SOCKET) // 에러 발생시
	{
		m_NetCtrlMessage.Format(_T("Socket Error\r\n"));
		SendMessage(m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
		closesocket(tServerSocket1);
		m_hNet = -1;
		return SOCK_ERROR;
	}


	DWORD optVal = 200;
	setsockopt(tServerSocket1, SOL_SOCKET, SO_RCVTIMEO, (char*)&optVal, sizeof(optVal));

	if (bind(tServerSocket1, (struct sockaddr*)&LocalInfo, sizeof(LocalInfo)) == SOCKET_ERROR)//바인드 소켓에 서버정보 부여
	{
		m_NetCtrlMessage.Format(_T("Bind Error\r\n"));
		SendMessage(m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
		closesocket(tServerSocket1);
		m_hNet = -1;
		return BIND_ERROR;
	}

	m_NetCtrlMessage.Format(_T("Network Connect Sucess\r\n"));
	SendMessage(m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
	
	m_hNet = 0;
	return UDP_OK;
}

void CNetHandle::CloseNet()
{
	// TODO: 여기에 구현 코드 추가

	m_NetCtrlMessage.Format(_T("Connection Closed\r\n"));
	SendMessage(m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
	
	closesocket(tServerSocket1);
	m_hNet = -1;
}



int CNetHandle::UdpSend(CString strIPs, CString strPORTs, char* pBuf, int Size)
{
	int SendSize;
	int PortNum;
	char strIP[16];
	char strPORT[16];

	SOCKADDR_IN DestInfo1;

	strcpy_s(strIP, CT2A(strIPs));
	strcpy_s(strPORT, CT2A(strPORTs));
	PortNum = atoi(strPORT);

	DestInfo1.sin_family = AF_INET;
	inet_pton(AF_INET, strIP, &(DestInfo1.sin_addr.s_addr));
	DestInfo1.sin_port = htons(PortNum); // 포트번호

	//  패킷송신
	SendSize = sendto(tServerSocket1, pBuf, Size, 0, (struct sockaddr*)&DestInfo1, sizeof(DestInfo1));

	return SendSize;
}

int CNetHandle::UdpReceive(void)
{
	// 메시지 수신, 수신한 메시지 길이 반환
	m_nRcvLen = recvfrom(tServerSocket1,(char *) m_RcvData, sizeof(m_RcvData), 0, NULL, NULL);
	if (m_nRcvLen < 0)
	{
		m_nRcvLen = recvfrom(tServerSocket1, (char*)m_RcvData, sizeof(m_RcvData), 0, NULL, NULL);
	}

	return m_nRcvLen;
}
