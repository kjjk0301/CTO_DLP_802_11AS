#include "pch.h"
#include "CProtocols.h"



//#define WM_PROGRESS	(WM_USER+2)

CProtocols::CProtocols()
{
}


CProtocols::~CProtocols()
{
}

void Wait(DWORD dwMsec)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < dwMsec)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}


void  CProtocols::MakePayload(unsigned char * pSrc, UINT16 nStat, UINT16 nIdx, UINT16 nSz)
{
	
	memcpy((void*)&m_CMDS[8], (void*)&pSrc[512 * nIdx], nSz);
	m_CMDS[0] = 'F';
	m_CMDS[1] = 'D';
	if (nStat == STARTIMG_FLG)
		m_CMDS[2] = 'S';
	else if (nStat == ENDIMG_FLG)
		m_CMDS[2] = 'E';
	else
		m_CMDS[2] = 'D';
	m_CMDS[3] = (char)nIdx;
}

void CProtocols::MmdWriteCmd()
{
	m_CMDS[0] = 'F';
	m_CMDS[1] = 'W';
	m_CMDS[2] = 'W';
	m_CMDS[3] = 'F';

}

void CProtocols::MmdWriteStardCmd()
{
	m_CMDS[0] = 'F';
	m_CMDS[1] = 'E';
	m_CMDS[2] = 'E';
	m_CMDS[3] = 'F';
}

void CProtocols::SeqCheckCmd()
{
	m_CMDS[0] = 'F';
	m_CMDS[1] = 'C';
	m_CMDS[2] = 'C';
	m_CMDS[3] = 'F';
}

void CProtocols::CmdCheck()
{
	m_CMDS[0] = 'C';
	m_CMDS[1] = 'C';
	m_CMDS[2] = 'C';
	m_CMDS[3] = 'C';
}

bool CProtocols::CheckCmdMatch(CNetHandle* hNet,char * pOrg ,int nCmd)
{
	
	for (int i = 0; i < nCmd - 2; i++)
	{
		if (pOrg[i] != hNet->m_RcvData[i]) return false;
	}

	return true;
}


bool CProtocols::CheckLastCmd(CNetHandle* hNet)
{
	int nCmd;
	CmdCheck();
	hNet->UdpSend(hNet->m_strIPL, hNet->m_strPORTL, (char*)m_CMDS, 4);
	nCmd = hNet->UdpReceive();
	if (nCmd == 4)
		return true;
	else
		return false;
}

bool CProtocols::CheckAck(CNetHandle* hNet,char *pOrg, bool CheckType)
{

	int nCmd = hNet->UdpReceive();

	if (nCmd == DEF_PACK_SZ)
	{
		if (CheckCmdMatch(hNet, pOrg, DEF_PACK_SZ))
		{
			if (CheckType)
			{
				if (hNet->m_RcvData[3] == REMOTE_ACK)
					return true;
				else
					return false;
			}
			else
			{
				if (hNet->m_RcvData[3] == pOrg[3])
					return true;
				else
					return false;
			}
		}
		else
			return false;
	}
	else
	{
		if (CheckLastCmd(hNet))
		{
			MmdWriteStardCmd();
			if (CheckCmdMatch(hNet, pOrg, DEF_PACK_SZ))
			{
				if (CheckType)
				{
					if (hNet->m_RcvData[3] == REMOTE_ACK)
						return true;
					else
						return false;
				}
				else
				{
					if (hNet->m_RcvData[3] == pOrg[3])
						return true;
					else
						return false;
				}


			}
			else
				return false;
		}
		else
			return false;
	}


}



bool CProtocols::EraseMMC(CNetHandle* hNet)
{
	int nCmd;
	char Test[4];

	MmdWriteStardCmd();
	memcpy(Test, m_CMDS, DEF_PACK_SZ);
	hNet->UdpSend(hNet->m_strIPL, hNet->m_strPORTL, (char*)m_CMDS, DEF_PACK_SZ);

	return (CheckAck(hNet,Test,true));


#if 0
	nCmd = hNet->UdpReceive();
	if (nCmd == DEF_PACK_SZ)
	{
		if (CheckCmdMatch(hNet, Test, DEF_PACK_SZ))
		{
			if (hNet->m_RcvData[3] == REMOTE_ACK)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else 
	{
		if (CheckLastCmd(hNet))
		{
			MmdWriteStardCmd();
			if (CheckCmdMatch(hNet, Test,DEF_PACK_SZ))
			{
				if (hNet->m_RcvData[3] == REMOTE_ACK)
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}

#endif
}


bool CProtocols::ProgramMMC(CNetHandle* hNet)
{
	int nCmd;
	char Test[4];

	MmdWriteCmd();
	memcpy(Test, m_CMDS, DEF_PACK_SZ);
	hNet->UdpSend(hNet->m_strIPL, hNet->m_strPORTL, (char*)m_CMDS, DEF_PACK_SZ);

	return (CheckAck(hNet, Test, true));


#if 0
	nCmd = hNet->UdpReceive();
	if (nCmd == DEF_PACK_SZ)
	{
		if (CheckCmdMatch(hNet, Test, DEF_PACK_SZ))
		{
			if (hNet->m_RcvData[3] == REMOTE_ACK)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else
	{
		if (CheckLastCmd(hNet))
		{
			MmdWriteStardCmd();
			if (CheckCmdMatch(hNet, Test, DEF_PACK_SZ))
			{
				if (hNet->m_RcvData[3] == REMOTE_ACK)
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}

#endif
}

bool CProtocols::SendImagePacket(CNetHandle* hNet,unsigned char * pPtr,int nIdx)
{
	char Test[4];

	if (nIdx == 0)
		MakePayload(pPtr, STARTIMG_FLG, nIdx, TRANSFER_SZ);	// Start of chunk
	else  if (nIdx == (TRANSFER_NUM - 1))
		MakePayload(pPtr, ENDIMG_FLG, nIdx, TRANSFER_SZ);	// End of chunk
	else
		MakePayload(pPtr, SENDINGIMG_FLG, nIdx, TRANSFER_SZ);	// IN transfer

	memcpy(Test, m_CMDS, DEF_PACK_SZ);
	hNet->UdpSend(hNet->m_strIPL, hNet->m_strPORTL, (char*)m_CMDS, TRANSFERPACKET_SZ);


	return(CheckAck(hNet, Test, false));

#if 0 
	int nCmd = hNet->UdpReceive();
	if (nCmd == 4)
	{
		if (CheckCmdMatch(hNet, Test,DEF_PACK_SZ))
			return true;
		else
			return false;
	}
	else
	{
		if (CheckLastCmd(hNet))
		{
			if (CheckCmdMatch(hNet, Test,DEF_PACK_SZ))
				return true;
			else
				return false;
		}
		else
			return false;
	}
#endif
}

bool CProtocols::PacketCheck(CNetHandle* hNet)
 {
	int nCmd = hNet->UdpReceive();
	if (nCmd == 4)
	{
		if (hNet->m_RcvData[0] == 'F' && hNet->m_RcvData[1] == 'E' && hNet->m_RcvData[2] == 'E' && hNet->m_RcvData[3] == 'O')
			return true;
		else
			return false;
	}
	else
	{
		nCmd = hNet->UdpReceive();
		if (nCmd == 4)
		{
			if (hNet->m_RcvData[0] == 'F' && hNet->m_RcvData[1] == 'E' && hNet->m_RcvData[2] == 'E' && hNet->m_RcvData[3] == 'O')
				return true;
			else
				return false;
		}
		else
			return false;
	}
}


#if 0

void CProtocols::SendCheckDspCpuStat(INT32 hCan, unsigned int ReceiverID, CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.

	pKvCan->m_TxCan.MSG[0]	= HEADER_CODE;
	pKvCan->m_TxCan.MSG[1]	= CHECK_STAT;
	pKvCan->m_TxCan.MSG[2]	= 0xFA;
	pKvCan->m_TxCan.DLC		= 3;
	pKvCan->m_TxCan.ID    = ReceiverID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD); 
	pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
}



void CProtocols::SendCommand(INT32 hCan, unsigned int ReceiverID, unsigned char MessageCode, CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = MessageCode;
	pKvCan->m_TxCan.MSG[2] = TAIL_CODE;
	pKvCan->m_TxCan.DLC    = 3;

	pKvCan->m_TxCan.ID = ReceiverID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
}

void CProtocols::SendCommand(INT32 hCan, unsigned int ReceiverID, unsigned char MessageCode, unsigned char OptionCode, CNetHandle* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = MessageCode;
	pKvCan->m_TxCan.MSG[2] = OptionCode;
	pKvCan->m_TxCan.MSG[3] = TAIL_CODE;
	pKvCan->m_TxCan.DLC    = 4;

	pKvCan->m_TxCan.ID = ReceiverID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
}

void CProtocols::SendCommandDAL(INT32 hCan, UINT32 RcvID, UINT8 MessageCode, UINT8 OptionCode, CNetHandle* pKvCan, UINT32 Parameter)
{
	// TODO: 여기에 구현 코드 추가.
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = MessageCode;
	if (OptionCode == DN_ADDRESS)
	{
		pKvCan->m_TxCan.MSG[2] = (Parameter >> 16) & 0xff;
		pKvCan->m_TxCan.MSG[3] = (Parameter >> 24) & 0xff;
		pKvCan->m_TxCan.MSG[4] = (Parameter) & 0xff;
		pKvCan->m_TxCan.MSG[5] = (Parameter >> 8) & 0xff;
	}
	else
	{	
		UINT16 Size = Parameter & 0xFFFF;
		pKvCan->m_TxCan.MSG[2] = (Size) & 0xff;
		pKvCan->m_TxCan.MSG[3] = (Size >> 8) & 0xff;
		pKvCan->m_TxCan.MSG[4] = 0;
		pKvCan->m_TxCan.MSG[5] = 0;
	}
	pKvCan->m_TxCan.MSG[6] = OptionCode;
	pKvCan->m_TxCan.MSG[7] = TAIL_CODE;

	pKvCan->m_TxCan.DLC = CMD_LEN_TYPE8;
	pKvCan->m_TxCan.ID = RcvID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
}

bool CProtocols::WriteCmd(INT32 hCan, CNetHandle* pKvCan, UINT16* data, INT32 index,INT16 TargetId)
{
	// TODO: 여기에 구현 코드 추가.
	int offset = Table10Context.pArrayUnitItems[index].Offset;
//	int typeLen = Table10Context.pArrayUnitItems[index].TypeLen;
//	if (type >= 4) type++;
//	if (type > 6) type = 7;

	// W0
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = 0x10;
//	pKvCan->m_TxCan.MSG[2] = 0 & 0xFF;
	pKvCan->m_TxCan.MSG[2] = (offset) & 0xFF;
	pKvCan->m_TxCan.MSG[3] = (offset >> 8) & 0xFF;
	pKvCan->m_TxCan.MSG[4] = Table10Context.pArrayUnitItems[index].Type;
	pKvCan->m_TxCan.MSG[5] = Table10Context.pArrayUnitItems[index].TypeOffsetLen;
	pKvCan->m_TxCan.MSG[6] = TAIL_CODE;

	pKvCan->m_TxCan.DLC = 8;
	pKvCan->m_TxCan.ID = TargetId;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
	if (!rst) return false;

	// ACK 수신 확인
	UINT16 tmp = this->RcvMsgType0(hCan, pKvCan, 0x10,0x10); 
	if(tmp != 0) tmp= this->RcvMsgType0(hCan, pKvCan, 0x10, 0x10);
	if (tmp != 0) return false;
	// else ACK 수신


	// W1
	memset(pKvCan->m_TxCan.MSG, 0, sizeof(unsigned char) * 8);
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = 0x11;
	pKvCan->m_TxCan.MSG[2] = data[0];
	pKvCan->m_TxCan.MSG[3] = data[1];
	pKvCan->m_TxCan.MSG[4] = data[2];
	pKvCan->m_TxCan.MSG[5] = data[3];
//	pKvCan->m_TxCan.MSG[6] = 0xFF;
	pKvCan->m_TxCan.MSG[6] = TAIL_CODE;

	pKvCan->m_TxCan.DLC = 8;
	pKvCan->m_TxCan.ID = TargetId;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
	if (!rst) return false;

	// ACK 수신 확인
	tmp = this->RcvMsgType0(hCan, pKvCan, 0x11, 0x10);
	if (tmp != 0) tmp = this->RcvMsgType0(hCan, pKvCan, 0x11, 0x10);
	if (tmp != 0) return false;

#if 0
	tmp = this->RcvStatMsgW(hCan, pKvCan, 0x10);
	if (tmp == 0) this->RcvStatMsgW(hCan, pKvCan, 0x10);
	else if (tmp == NAK)
		return false;
#endif
	return true; // Write 성공
}

bool CProtocols::ReadCmd(INT32 hCan, CNetHandle* pKvCan, UINT16* data, INT32 index, INT16 TargetId)
{
	int offset = Table10Context.pArrayUnitItems[index].Offset;

	// R0
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = 0x20;
//	pKvCan->m_TxCan.MSG[2] = 0 & 0xFF;
	pKvCan->m_TxCan.MSG[2] = (offset) & 0xFF;
	pKvCan->m_TxCan.MSG[3] = (offset >> 8) & 0xFF;
	pKvCan->m_TxCan.MSG[4] = Table10Context.pArrayUnitItems[index].Type;
	pKvCan->m_TxCan.MSG[5] = Table10Context.pArrayUnitItems[index].TypeOffsetLen;
	pKvCan->m_TxCan.MSG[6] = TAIL_CODE;

	pKvCan->m_TxCan.DLC = 8;
	pKvCan->m_TxCan.ID = TargetId;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
	if (!rst) return false;

	
	UINT16 tmp = RcvMsgType1(hCan, pKvCan, 0x20,0x0010); 
	if (tmp != 0) tmp = RcvMsgType1(hCan, pKvCan, 0x20, 0x0010);
	
	if (tmp == 0) // NAK 수신 시 중단
	{
		memset(data, 0x00, 8);
		switch (Table10Context.pArrayUnitItems[index].Type)
		{
		case tUINT8:
		case tINT8:
		case tBOOL:
			data[0] = pKvCan->m_RxCan.MSG[2] &0xff;
			break;

		case tUINT16:
		case tINT16:
			data[0] = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] <<8);
			break;

		case tUINT32:
		case tINT32:
			data[0] = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] << 8);
			data[1] = (pKvCan->m_RxCan.MSG[4] & 0xff) | (pKvCan->m_RxCan.MSG[5] << 8);
			break;

		default:
			return false;
			break;
		}

		return true;
	}
	else
		return false;
}

#if 0
void CProtocols::SendCommandDA(INT32 hCan, UINT32 RcvID, unsigned char MessageCode, CCanKvaser * pKvCan, UINT32 SendBlockAddr)
{
	// TODO: 여기에 구현 코드 추가.
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = MessageCode;
	pKvCan->m_TxCan.MSG[2] = (SendBlockAddr >> 16) & 0xff;
	pKvCan->m_TxCan.MSG[3] = (SendBlockAddr >> 24) & 0xff;
	pKvCan->m_TxCan.MSG[4] = (SendBlockAddr) & 0xff;
	pKvCan->m_TxCan.MSG[5] = (SendBlockAddr >> 8) & 0xff;
	pKvCan->m_TxCan.MSG[6] = TAIL_CODE;

	pKvCan->m_TxCan.DLC = CMD_EXT_LEN;
	pKvCan->m_TxCan.ID = RcvID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
}

void CProtocols::SendCommandDL(INT32 hCan, UINT32 RcvID, unsigned char MessageCode, CCanKvaser * pKvCan, UINT32 LEN, UINT32 blkNum)
{
	// TODO: 여기에 구현 코드 추가.
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = MessageCode;
	pKvCan->m_TxCan.MSG[2] = (LEN) & 0xff;
	pKvCan->m_TxCan.MSG[3] = (LEN >> 8) & 0xff;
	pKvCan->m_TxCan.MSG[4] = (blkNum) & 0xff;
	pKvCan->m_TxCan.MSG[5] = (blkNum >> 8) & 0xff;
	pKvCan->m_TxCan.MSG[6] = TAIL_CODE;

	pKvCan->m_TxCan.DLC = CMD_EXT_LEN;
	pKvCan->m_TxCan.ID = RcvID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
}
#endif

void CProtocols::JumpToBtl(INT32 hCan, UINT32 RcvID,CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.

	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = 0;
	pKvCan->m_TxCan.MSG[2] = 0;
	pKvCan->m_TxCan.MSG[3] = 0;
	pKvCan->m_TxCan.MSG[4] = 0;
	pKvCan->m_TxCan.MSG[5] = 'M';
	pKvCan->m_TxCan.MSG[6] = 0;
	pKvCan->m_TxCan.MSG[7] = 0;

	pKvCan->m_TxCan.DLC = 8;
	pKvCan->m_TxCan.ID = SWITCH_BTL_ID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);




	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
}


void CProtocols::JumpToApp(INT32 hCan,UINT32 EntryAddr, UINT32 RcvID, CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = (START_MSG);
	pKvCan->m_TxCan.MSG[2] = (EntryAddr >> 16) & 0xff;
	pKvCan->m_TxCan.MSG[3] = (EntryAddr >> 24) & 0xff;
	pKvCan->m_TxCan.MSG[4] = (EntryAddr      ) & 0xff;
	pKvCan->m_TxCan.MSG[5] = (EntryAddr >>  8) & 0xff;;
	pKvCan->m_TxCan.MSG[6] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[7] = TAIL_CODE;

	pKvCan->m_TxCan.DLC = 8;
	pKvCan->m_TxCan.ID = RcvID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
	if (!rst) pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);// 실패시 한번더 시도
}


UINT16 CProtocols::RcvRspMsg(INT32 hCan, CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// 명령을 보낸 후 DSP에서 수신되는 ACK/NAK를 수신
	// check status는 btl/app의 상태를 return
	// 다른 명령어는 받은 명령을 되돌려 보냄.

	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecific(hCan, PCS_CAN_ID, &pKvCan->m_RxCan, 9000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.
	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == PCS_CAN_ID)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류
			UINT16 PLAN = 0;
			if (pKvCan->m_RxCan.DLC == 0)
			{
				switch (pKvCan->m_RxCan.MSG[1])
				{
				case STATUS_MSG:
				case VERSION_MSG:
					if (pKvCan->m_RxCan.MSG[7] == 0xF0)
						pKvCan->m_RxCan.DLC = 8;
					else if(pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else 
						pKvCan->m_RxCan.DLC = 0;
					break;

				case DNLOAD_MSG:
				case ERASE_MSG:
				case START_MSG:
				case MARK_MSG:
				case EXIT_UPDATE:
				case REBOOT:
				case ERROR_MSG:

					pKvCan->m_RxCan.DLC = 3;
					break;
				
				case VERIFY_MSG:
				case PROGRAM_MSG:
					if (pKvCan->m_RxCan.MSG[4] == 0xF0)
						pKvCan->m_RxCan.DLC = 5;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case CPUSTAT_PACK:
					if (pKvCan->m_RxCan.MSG[5] == 0xF0)
						pKvCan->m_RxCan.DLC = 6;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;




				}

			}
			if (pKvCan->m_RxCan.DLC == 0)
				return 0;
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] != RTAIL_CODE)
				return 0; // 테일 오류

			MessageCode = (UINT16)pKvCan->m_RxCan.MSG[1];
			if (MessageCode == CHECK_STAT)  // Check status의 응답이면 status의 조합을 return
			{
				MessageCode = (UINT16)((pKvCan->m_RxCan.MSG[3] & 0x0f) | (pKvCan->m_RxCan.MSG[4] & 0xf0)); // Dual core stat
			}
			else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG ) // 이들 명령어는 CRC를 반고,
			//else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG || MessageCode == EEPROM_WRITE_MSG) // 이들 명령어는 CRC를 반고,
			{
				m_CRC_RD = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] & 0xff) << 8;
			}

			return MessageCode; // 돌아온 메시지를 return, 예외: check status는 DSP의 상태를 return함
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
}
#if 0
UINT16 CProtocols::RcvRspMsg(INT32 hCan, UINT32 RcvID, CCanKvaser* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// 명령을 보낸 후 DSP에서 수신되는 ACK/NAK를 수신
	// check status는 btl/app의 상태를 return
	// 다른 명령어는 받은 명령을 되돌려 보냄.

	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecific(hCan, RcvID, &pKvCan->m_RxCan, 9000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.
	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == RcvID)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류
			UINT16 PLAN = 0;
			if (pKvCan->m_RxCan.DLC == 0)
			{
				switch (pKvCan->m_RxCan.MSG[1])
				{
				case STATUS_MSG:
				case VERSION_MSG:
					if (pKvCan->m_RxCan.MSG[7] == 0xF0)
						pKvCan->m_RxCan.DLC = 8;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case DNLOAD_MSG:
				case ERASE_MSG:
				case START_MSG:
				case MARK_MSG:
				case EXIT_UPDATE:
				case REBOOT:
				case ERROR_MSG:

					pKvCan->m_RxCan.DLC = 3;
					break;

				case VERIFY_MSG:
				case PROGRAM_MSG:
					if (pKvCan->m_RxCan.MSG[4] == 0xF0)
						pKvCan->m_RxCan.DLC = 5;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case CPUSTAT_PACK:
					if (pKvCan->m_RxCan.MSG[5] == 0xF0)
						pKvCan->m_RxCan.DLC = 6;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;
				}

			}
			if (pKvCan->m_RxCan.DLC == 0)
				return 0;
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] != RTAIL_CODE)
				return 0; // 테일 오류

			MessageCode = (UINT16)pKvCan->m_RxCan.MSG[1];
			if (MessageCode == CHECK_STAT)  // Check status의 응답이면 status의 조합을 return
			{
				MessageCode = (UINT16)((pKvCan->m_RxCan.MSG[3] & 0x0f) | (pKvCan->m_RxCan.MSG[4] & 0xf0)); // Dual core stat
			}
			else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG) // 이들 명령어는 CRC를 반고,
				//else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG || MessageCode == EEPROM_WRITE_MSG) // 이들 명령어는 CRC를 반고,
			{
				m_CRC_RD = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] & 0xff) << 8;
			}

			return MessageCode; // 돌아온 메시지를 return, 예외: check status는 DSP의 상태를 return함
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
}
#endif
UINT16 CProtocols::RcvRspMsg(INT32 hCan, CNetHandle* pKvCan,UINT32 TimeOut)
{
	// TODO: 여기에 구현 코드 추가.
	// 명령을 보낸 후 DSP에서 수신되는 ACK/NAK를 수신
	// check status는 btl/app의 상태를 return
	// 다른 명령어는 받은 명령을 되돌려 보냄.

	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecificNoRetry(hCan, PCS_CAN_ID, &pKvCan->m_RxCan, TimeOut); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.
	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == PCS_CAN_ID)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류
			UINT16 PLAN = 0;
			if (pKvCan->m_RxCan.DLC == 0)
			{
				switch (pKvCan->m_RxCan.MSG[1])
				{
				case STATUS_MSG:
				case VERSION_MSG:
					if (pKvCan->m_RxCan.MSG[7] == 0xF0)
						pKvCan->m_RxCan.DLC = 8;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case DNLOAD_MSG:
				case ERASE_MSG:
				case START_MSG:
				case MARK_MSG:
				case EXIT_UPDATE:
				case REBOOT:
				case ERROR_MSG:

					pKvCan->m_RxCan.DLC = 3;
					break;

				case VERIFY_MSG:
				case PROGRAM_MSG:
					if (pKvCan->m_RxCan.MSG[4] == 0xF0)
						pKvCan->m_RxCan.DLC = 5;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case CPUSTAT_PACK:
					if (pKvCan->m_RxCan.MSG[5] == 0xF0)
						pKvCan->m_RxCan.DLC = 6;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;
				}

			}
			if (pKvCan->m_RxCan.DLC == 0)
				return 0;
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] != RTAIL_CODE)
				return 0; // 테일 오류

			MessageCode = (UINT16)pKvCan->m_RxCan.MSG[1];
			if (MessageCode == CHECK_STAT)  // Check status의 응답이면 status의 조합을 return
			{
				MessageCode = (UINT16)((pKvCan->m_RxCan.MSG[3] & 0x0f) | (pKvCan->m_RxCan.MSG[4] & 0xf0)); // Dual core stat
			}
			else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG) // 이들 명령어는 CRC를 반고,
			//else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG || MessageCode == EEPROM_WRITE_MSG) // 이들 명령어는 CRC를 반고,
			{
				m_CRC_RD = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] & 0xff) << 8;
			}

			return MessageCode; // 돌아온 메시지를 return, 예외: check status는 DSP의 상태를 return함
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
}


UINT16 CProtocols::RcvRspMsgId(INT32 hCan, UINT32 Id, CNetHandle* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// 명령을 보낸 후 DSP에서 수신되는 ACK/NAK를 수신
	// check status는 btl/app의 상태를 return
	// 다른 명령어는 받은 명령을 되돌려 보냄.

	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecific(hCan, Id, &pKvCan->m_RxCan, 9000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.
	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == Id)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류
			UINT16 PLAN = 0;
			if (pKvCan->m_RxCan.DLC == 0)
			{
				switch (pKvCan->m_RxCan.MSG[1])
				{
				case STATUS_MSG:
				case VERSION_MSG:
					if (pKvCan->m_RxCan.MSG[7] == 0xF0)
						pKvCan->m_RxCan.DLC = 8;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case DNLOAD_MSG:
				case ERASE_MSG:
				case START_MSG:
				case MARK_MSG:
				case EXIT_UPDATE:
				case REBOOT:
				case ERROR_MSG:

					pKvCan->m_RxCan.DLC = 3;
					break;

				case VERIFY_MSG:
				case PROGRAM_MSG:
					if (pKvCan->m_RxCan.MSG[4] == 0xF0)
						pKvCan->m_RxCan.DLC = 5;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case CPUSTAT_PACK:
					if (pKvCan->m_RxCan.MSG[5] == 0xF0)
						pKvCan->m_RxCan.DLC = 6;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;




				}

			}
			if (pKvCan->m_RxCan.DLC == 0)
				return 0;
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] != RTAIL_CODE)
				return 0; // 테일 오류

			MessageCode = (UINT16)pKvCan->m_RxCan.MSG[1];
			if (MessageCode == CHECK_STAT)  // Check status의 응답이면 status의 조합을 return
			{
				MessageCode = (UINT16)((pKvCan->m_RxCan.MSG[3] & 0x0f) | (pKvCan->m_RxCan.MSG[4] & 0xf0)); // Dual core stat
			}
			else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG) // 이들 명령어는 CRC를 반고,
				//else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG || MessageCode == EEPROM_WRITE_MSG) // 이들 명령어는 CRC를 반고,
			{
				m_CRC_RD = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] & 0xff) << 8;
			}

			return MessageCode; // 돌아온 메시지를 return, 예외: check status는 DSP의 상태를 return함
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
}

UINT16 CProtocols::RcvRspMsgId(INT32 hCan, UINT32 Id, CNetHandle* pKvCan, UINT32 TimeOut)
{
	// TODO: 여기에 구현 코드 추가.
	// 명령을 보낸 후 DSP에서 수신되는 ACK/NAK를 수신
	// check status는 btl/app의 상태를 return
	// 다른 명령어는 받은 명령을 되돌려 보냄.

	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecificNoRetry(hCan, Id, &pKvCan->m_RxCan, TimeOut); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.
	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == Id)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류
			UINT16 PLAN = 0;
			if (pKvCan->m_RxCan.DLC == 0)
			{
				switch (pKvCan->m_RxCan.MSG[1])
				{
				case STATUS_MSG:
				case VERSION_MSG:
					if (pKvCan->m_RxCan.MSG[7] == 0xF0)
						pKvCan->m_RxCan.DLC = 8;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case DNLOAD_MSG:
				case ERASE_MSG:
				case START_MSG:
				case MARK_MSG:
				case EXIT_UPDATE:
				case REBOOT:
				case ERROR_MSG:

					pKvCan->m_RxCan.DLC = 3;
					break;

				case VERIFY_MSG:
				case PROGRAM_MSG:
					if (pKvCan->m_RxCan.MSG[4] == 0xF0)
						pKvCan->m_RxCan.DLC = 5;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;

				case CPUSTAT_PACK:
					if (pKvCan->m_RxCan.MSG[5] == 0xF0)
						pKvCan->m_RxCan.DLC = 6;
					else if (pKvCan->m_RxCan.MSG[2] == 0xFF)
						pKvCan->m_RxCan.DLC = 3;
					else
						pKvCan->m_RxCan.DLC = 0;
					break;
				}

			}
			if (pKvCan->m_RxCan.DLC == 0)
				return 0;
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] != RTAIL_CODE)
				return 0; // 테일 오류

			MessageCode = (UINT16)pKvCan->m_RxCan.MSG[1];
			if (MessageCode == CHECK_STAT)  // Check status의 응답이면 status의 조합을 return
			{
				MessageCode = (UINT16)((pKvCan->m_RxCan.MSG[3] & 0x0f) | (pKvCan->m_RxCan.MSG[4] & 0xf0)); // Dual core stat
			}
			else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG) // 이들 명령어는 CRC를 반고,
				//else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG || MessageCode == EEPROM_WRITE_MSG) // 이들 명령어는 CRC를 반고,
			{
				m_CRC_RD = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] & 0xff) << 8;
			}

			return MessageCode; // 돌아온 메시지를 return, 예외: check status는 DSP의 상태를 return함
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
}








INT16 CProtocols::RcvMsgType0(INT32 hCan, CNetHandle* pKvCan, UINT16 Msg, long RcvId)
{
	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecificNoRetry(hCan, RcvId, &pKvCan->m_RxCan, 9000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.

	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == RcvId)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.DLC != 3)				// Type0 Len
				return -1;
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)	// Header Check
				return -2;
			if (pKvCan->m_RxCan.MSG[1] != Msg)			// Sended Msg
				return -3;
			if ((pKvCan->m_RxCan.MSG[2]&0xF0) != RTAIL_CODE) 	// Invalid Ack
				return -4;

			return (pKvCan->m_RxCan.MSG[2] & 0x0F);		// ACK
		}
		else
			return -5;// ID 오류( 일어날 수 없음)
	}
	else
		return -6;// 통신 오류( Timeout을 포함한 )

}


INT16 CProtocols::RcvMsgType1(INT32 hCan, CNetHandle* pKvCan, UINT16 Msg, long RcvId)
{
	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecificNoRetry(hCan, RcvId, &pKvCan->m_RxCan, 9000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.

	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == RcvId)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.DLC != 8)				// Type0 Len
				return -1;
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)	// Header Check
				return -2;
			if (pKvCan->m_RxCan.MSG[1] != Msg)			// Sended Msg
				return -3;
			if ((pKvCan->m_RxCan.MSG[7] & 0xF0) != RTAIL_CODE) 	// Invalid Ack
				return -4;

			return (pKvCan->m_RxCan.MSG[7] & 0x0F);		// ACK
		}
		else
			return -5;// ID 오류( 일어날 수 없음)
	}
	else
		return -6;// 통신 오류( Timeout을 포함한 )

}


#if 0
UINT16 CProtocols::RcvStatMsgW(INT32 hCan, CCanKvaser* pKvCan, long RcvId)
{
	// TODO: 여기에 구현 코드 추가.
	// 명령을 보낸 후 DSP에서 수신되는 ACK/NAK를 수신
	// ACK / NAK 여부 확인 후 반환
	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecificNoRetry(hCan, RcvId, &pKvCan->m_RxCan, 9000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.

#if 0
	if (Rst)
	{	
		if (pKvCan->m_RxCan.ID == RcvId)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류

			UINT16 PLAN = 0;
			if (pKvCan->m_RxCan.DLC != 0)
			{
				if (pKvCan->m_RxCan.MSG[1] != checkProp)
					return 0; //무결성 검증 실패
			}
			if (pKvCan->m_RxCan.DLC == 0)
				return 0;
			


			//메시지 확인
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] == RTAIL_CODE) return ACK; // ACK 확인됨
			else if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] == HEADER_CODE) return NAK; // NAK 확인됨
			else return 0; //RETAIL ERROR
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
#endif
	return 0; // 수신실패 timeout
}



// RcvDat -> Type:INT16, len: INT16 * 4
UINT16 CProtocols::RcvStatMsgR(INT32 hCan, CCanKvaser* pKvCan, UINT16* RcvDat, UINT16 RcvId)
{
	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecificNoRetry(hCan, RcvId, &pKvCan->m_RxCan, 9000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.
	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == RcvId)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류
			UINT16 PLAN = 0;
			if (pKvCan->m_RxCan.DLC != 0)
			{
				if (pKvCan->m_RxCan.MSG[1] != R0)
					return 0; //무결성 검증 실패
			}
			if (pKvCan->m_RxCan.DLC == 0)
				return 0;

			//메시지 확인
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] == RTAIL_CODE) 
			{
				if (pKvCan->m_RxCan.DLC != 8) return 0; //메시지 길이 불일치
				memcpy(RcvDat, pKvCan->m_RxCan.MSG + sizeof(UINT16) * 2, sizeof(UINT16) * 4);
				return ACK; // ACK 확인됨
			}
			else if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] == HEADER_CODE) return NAK; // NAK 확인됨
			else return 0; //RETAIL ERROR
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
}
#endif

UINT16 CProtocols::CheckDspStat(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// //cpuID를 실어서 상태 체크 명령을 보냄,
	// boot loader / application 중 하나를 return, 실패시 0 return;
	UINT16 ReturnCode;

	SendCheckDspCpuStat(hCan, RcvID, pKvCan);
	ReturnCode = RcvRspMsg(hCan, pKvCan);

	return ReturnCode;
}

UINT16 CProtocols::CheckDspStat(INT32 hCan, UINT32 RcvID, CNetHandle* pKvCan, UINT32 TimeOut)
{
	// TODO: 여기에 구현 코드 추가.
	// //cpuID를 실어서 상태 체크 명령을 보냄,
	// boot loader / application 중 하나를 return, 실패시 0 return;
	UINT16 ReturnCode;

	SendCheckDspCpuStat(hCan, RcvID, pKvCan);
	ReturnCode = RcvRspMsg(hCan, pKvCan, TimeOut);

	return ReturnCode;
}


bool CProtocols::SwitchMode(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan, UINT16 Mode)
{
	// TODO: 여기에 구현 코드 추가.
#if 1
	UINT16 ReturnCode;

	// 입력 모드 오류
	if (((Mode != STAT_APP) && (Mode != STAT_BTL)))
		return false;


	// 현재 상태 확인
	ReturnCode = CheckDspStat(hCan, RcvID, pKvCan);

	// 확인시 Error 처리
	if (ReturnCode == 0)
	{
		Wait(100);
		ReturnCode = CheckDspStat(hCan, RcvID, pKvCan); // try one more time
		if (ReturnCode == 0 || ((ReturnCode != STAT_APP) && (ReturnCode != STAT_BTL)))
			return false;
	}

	// 원하는 상태가 현재 상태와 같으면 true;
	if (Mode == ReturnCode)
		return true;
	// 다른 경우
	else
	{
		// BTL 로 전환 
		if (Mode == STAT_BTL)
		{
			SendCommand(hCan, RcvID, ERASE_MSG, ERASE_PRGFLAG, pKvCan);
			ReturnCode = RcvRspMsg(hCan, pKvCan);
			if (ReturnCode == ERASE_MSG)
			{
				SendCommand(hCan, RcvID, REBOOT, pKvCan);
				ReturnCode = RcvRspMsg(hCan, pKvCan);
				if (ReturnCode != REBOOT) return false;
			}
		}
		// APP으로 전환 
		else if (Mode == STAT_APP)
		{

			SendCommand(hCan, RcvID, REBOOT, pKvCan);
			ReturnCode = RcvRspMsg(hCan, pKvCan);
			if (ReturnCode != REBOOT) return false;
		}


		Sleep(4000);
		ReturnCode = CheckDspStat(hCan, RcvID, pKvCan); // try one more time

		// 정상적인 처리
		if (ReturnCode == Mode) return true;
		// 커맨드를 보냈으나 실패
		else return false;
	}
#endif
}



bool CProtocols::EraseAppArea(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// 내부 App area와 Mark 영역을 지우는 동작

	UINT16 ReturnCode;
	UINT16 RetryCnt=0;
	

	// 1. DSP의 상태확인
StartofErase:
	ReturnCode= CheckDspStat(hCan, RcvID, pKvCan);
	if(ReturnCode==0) ReturnCode = CheckDspStat(hCan, RcvID,pKvCan); // 한번 더 시도
	if (ReturnCode != STAT_BTL)
	{
		if (RetryCnt == 3)
		{
			pKvCan->m_CanCtrlMessage.Format(_T("3 times of fail to change mode. return \r\n"));
			SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
			return false;
		}

		if(ReturnCode != STAT_APP)
		{
			pKvCan->m_CanCtrlMessage.Format(_T("Undefined State. Recommand Initial program writing.  \r\n"));
			SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
			return false;
		}
		else
		{
			pKvCan->m_CanCtrlMessage.Format(_T("Application is running. Changing it to Update Mode.  \r\n"));
			SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);

			SendCommand(hCan, RcvID, START_UPDATE, pKvCan);
			ReturnCode = RcvRspMsg(hCan, pKvCan);

			if (ReturnCode != START_UPDATE)
			{
				pKvCan->m_CanCtrlMessage.Format(_T("Fail in Changing to Update Mode.  \r\n"));
				SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
				return false; // NAK 면 종료
			}

			Sleep(1000);

			RetryCnt++;
			goto StartofErase;
		}
	}


    //bootloader 상태인 경우 ERASE 진행
	// 2. ERASE MESSAGE 전송후 ACK 수신
	SendCommand(hCan, RcvID,ERASE_MSG, ERASE_PRG,pKvCan); // Program 영역을 지움
	ReturnCode = RcvRspMsg(hCan, pKvCan);
	if (ReturnCode != ERASE_MSG)
		return false; // NAK 면 종료

	Sleep(100);

	return true; // 정상 동작
}

bool CProtocols::EraseImgArea(INT32 hCan, UINT32 RcvID, CNetHandle* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// 내부 Img 영역을 지우는 동작
	UINT16 ReturnCode;

	// 1. 8Byte Erase Message
	//SendCommandDAL(hCan, RcvID, ERASE_MSG, 0, pKvCan, 0x00000000);
	SendCommand(hCan, RcvID, ERASE_MSG, ERASE_PRG, pKvCan);
	ReturnCode = RcvRspMsgId(hCan,0x10,pKvCan);
	if (ReturnCode != ERASE_MSG)
		return false; // NAK 면 종료

	Sleep(100);

	return true; // 정상 동작
}


bool CProtocols::MarkBootFlag(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	UINT16 ReturnCode;

	// 1. DSP의 상태확인
	ReturnCode = CheckDspStat(hCan, RcvID, pKvCan);
	if (ReturnCode == 0) ReturnCode = CheckDspStat(hCan, RcvID, pKvCan); // 한번 더 시도
	if (ReturnCode != STAT_BTL)
		return false; // bootloader 상태가 아니면 종료
	//bootloader 상태인 경우 진행

	// 2. MARK MESSAGE 전송후 ACK 수신
	SendCommand(hCan, RcvID, MARK_MSG, pKvCan);
	ReturnCode = RcvRspMsg(hCan, pKvCan);
	if (ReturnCode != MARK_MSG)
		return false; // NAK 면 종료


	return true; // 정상 동작
}


int CProtocols::DnLoadFirmware(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan, CFileParser *pFIleParser,bool Mode)
{
	// TODO: 여기에 구현 코드 추가.
	UINT16 ReturnCode;
	UINT16 CRC_MEM, CRC_FLS, CRC_CC, CRC_RD;
	UINT32 SendBlockAddr, SendBlockSize;
	DWORD	BufIdx;
	int i, j;


	//if (hCan < 0)
	//{
	//	pKvCan->m_NetCtrlMessage.Format(_T("CAN NOT OPENED or ERROR \r\n"));
	//	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);

	//	return eCANSOCK;
	//}


	// 1. DSP의 상태확인
	//ReturnCode = CheckDspStat(hCan, RcvID, pKvCan);
	//if (ReturnCode == 0) ReturnCode = CheckDspStat(hCan, RcvID, pKvCan); // 한번 더 시도
	//if (ReturnCode != STAT_BTL)
	//	return false; // bootloader 상태가 아니면 종료
	//
	//				  
	//				  //bootloader 상태인 경우 진행

	if (pFIleParser->m_BinInfo.NumSections < 1)
	{
		pKvCan->m_CanCtrlMessage.Format(_T("FILE NOT OPENED or ERROR \r\n"));
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);

		return eFILE;
	}

	if (Mode)
	{
		pKvCan->m_CanCtrlMessage.Format(_T("Update FLASH \r\n"));
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
	}

	CRC_MEM = 0;
	CRC_FLS = 0;

	for (i = 0; i < pFIleParser->m_BinInfo.NumSections; i++)
	{

		SendBlockAddr = pFIleParser->m_BinInfo.pSections[i].EntryPoint;
		SendBlockSize = pFIleParser->m_BinInfo.pSections[i].SizeSector;
		BufIdx = pFIleParser->m_BinInfo.pSections[i].BufOffset;

		pKvCan->m_CanCtrlMessage.Format(_T("Sending %d th block E:%08X  S:%08X : "), i, SendBlockAddr, SendBlockSize*2);
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);

		while (SendBlockSize > 0)
		{
			//SendCommandDAL(hCan, RcvID, DNLOAD_MSG, DN_ADDRESS, pKvCan, SendBlockAddr);
			//ReturnCode = RcvRspMsg(hCan, pKvCan);
			//if (ReturnCode != DNLOAD_MSG)
			//	return eDNLOADADDR;


			if (SendBlockSize <= DNLOAD_SIZE)
			{
				//SendCommandDAL(hCan, RcvID, DNLOAD_MSG, DN_SIZE, pKvCan, SendBlockSize);
				//ReturnCode = RcvRspMsg(hCan, pKvCan);
				//if (ReturnCode != DNLOAD_MSG)
				//	return eDNLOADLEN;

				j = SendBlockSize;
			}
			else
			{
				//SendCommandDAL(hCan, RcvID, DNLOAD_MSG, DN_SIZE, pKvCan, DNLOAD_SIZE);
				//ReturnCode = RcvRspMsg(hCan, pKvCan);
				//if (ReturnCode != DNLOAD_MSG)
				//	return eDNLOADLEN;

				j = DNLOAD_SIZE;
			}

			//CRC_CC = crc16(&pFIleParser->m_pFwBuf[BufIdx], j * 2);
			SendBlockSize -= j;// Decrease DNLOAD_SIZE
			SendBlockAddr += j;// Increase DNLOAD_SI
			CRC_CC = 0;
			CRC_MEM += CRC_CC;

			while (j > 0)
			{
				if (j >= 4)
				{
					memcpy((void *)pKvCan->m_TxCan.MSG, (void *)&pFIleParser->m_pFwBuf[BufIdx], 8);
					BufIdx += 8;
					j -= 4;
				}
				else
				{
					memcpy((void *)pKvCan->m_TxCan.MSG, (void *)&pFIleParser->m_pFwBuf[BufIdx], j * 2);
					for (int k = j * 2; k < 8; k++) pKvCan->m_TxCan.MSG[k] = 0xff;  // Stuffing
					j = 0;
				}

				pKvCan->m_TxCan.DLC = 8;
				pKvCan->m_TxCan.ID = RcvID;
				bool rst=pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
				//if (!rst) return eCANTX;
				//ReturnCode = RcvRspMsg(hCan, pKvCan);
				//if (ReturnCode != DNLOAD_MSG)
				bool rtn = pKvCan->CanReadSpecific(hCan, 512, &pKvCan->m_RxCan, 10);
				//	return eCANACK;
			}

			// VERIFY
			//SendCommand(hCan, RcvID, VERIFY_MSG, pKvCan);
			//ReturnCode = RcvRspMsg(hCan, pKvCan);
			//if (ReturnCode != VERIFY_MSG)
			//	return eVERIFY;

			CRC_RD = m_CRC_RD;// (pKvCan->m_RxCan.MSG[2]) | (pKvCan->m_RxCan.MSG[3] << 8);

			CRC_CC = CRC_RD; 
			if (CRC_CC != CRC_RD)
			{
				pKvCan->m_CanCtrlMessage.Format(_T("x"));
				SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
				return eFILECRC;
			}
			else
			{
//				pKvCan->m_NetCtrlMessage.Format(_T("D"));
//				SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
			}

			CRC_RD = 0;


//			if (Mode)
//			{
//				SendCommand(hCan, RcvID, PROGRAM_MSG, pKvCan);
//				ReturnCode = RcvRspMsg(hCan, pKvCan);
//				if (ReturnCode != PROGRAM_MSG)
//					return ePROGRAM;
//				else
//				{
////					pKvCan->m_NetCtrlMessage.Format(_T("P"));
////					SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
//				}
//			}

			CRC_RD = m_CRC_RD;
			CRC_FLS += CRC_RD;

		}
		pKvCan->m_CanCtrlMessage.Format(_T(" --> Done\r\n"));
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);

		pKvCan->m_Progress =(i / pFIleParser->m_BinInfo.NumSections) * 100;
		SendMessage(pKvCan->m_hWnd_net, WM_PROGRESS, 0, (LPARAM)this);

	}
	CRC_FLS = CRC_MEM;
	if (CRC_FLS != CRC_MEM)
	{
		pKvCan->m_CanCtrlMessage.Format(_T(" Flash CRC ERROR \r\n"));
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
		return eMEMCRC;
	}
	else
	{
		pKvCan->m_CanCtrlMessage.Format(_T(" Flash Success \r\n"));
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
	}

	Sleep(1000);

	if (Mode)
	{
		//SendCommand(hCan, RcvID, MARK_MSG, pKvCan);
		//ReturnCode = RcvRspMsg(hCan, pKvCan);
		//if (ReturnCode != MARK_MSG)
		//{
		//	pKvCan->m_NetCtrlMessage.Format(_T(" Flag Mark ERROR \r\n"));
		//	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);

		//	return ePROGRAM;
		//}
		//else
		{
			pKvCan->m_CanCtrlMessage.Format(_T("Update Firmware  All Done\r\n"));
			SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
		}
	}
	return true;
}

int CProtocols::DnLoadImage(INT32 hCan, UINT32 RcvID, CNetHandle* pKvCan, CFileParser* pFIleParser, bool Mode)
{
	// TODO: 여기에 구현 코드 추가.
	UINT16 ReturnCode;
	UINT16 CRC_MEM, CRC_FLS, CRC_CC, CRC_RD;
	UINT32 SendBlockAddr, SendBlockSize;
	DWORD	BufIdx;
	int i, j;

	if (pFIleParser->m_BinInfo.NumSections < 1)
	{
		pKvCan->m_CanCtrlMessage.Format(_T("FILE NOT OPENED or ERROR \r\n"));
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);

		return eFILE;
	}

	if (Mode)
	{
		pKvCan->m_CanCtrlMessage.Format(_T("Update FLASH \r\n"));
		SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
	}

	CRC_MEM = 0;
	CRC_FLS = 0;



	//Send Image
	DWORD TransferFileSize = pFIleParser->m_pHTBHeader->dwFileSize;
	BufIdx = 0;
	SendBlockAddr = 0;
	
	
	while (TransferFileSize>0)
	{
		if (TransferFileSize > 2048)
			j = DNLOAD_SIZE;
		else
			j = TransferFileSize;
#if 0		
		SendCommandDAL(hCan, RcvID, DNLOAD_MSG, DN_ADDRESS, pKvCan, SendBlockAddr);
		ReturnCode = RcvRspMsgId(hCan,0x10, pKvCan);
		if (ReturnCode != DNLOAD_MSG)
			return eDNLOADADDR;
#endif

		SendCommandDAL(hCan, RcvID, DNLOAD_MSG, DN_SIZE, pKvCan, j);
		ReturnCode = RcvRspMsgId(hCan, 0x10, pKvCan);
		if (ReturnCode != DNLOAD_MSG)
			return eDNLOADLEN;

		CRC_CC = crc16(&pFIleParser->m_pFwBuf[BufIdx], j * 2);
		TransferFileSize -= j;// Decrease DNLOAD_SIZE
		SendBlockAddr += j;// Increase DNLOAD_SI
		CRC_MEM += CRC_CC;



		while (j > 0)
		{
			if (j >= 4)
			{
				memcpy((void*)pKvCan->m_TxCan.MSG, (void*)&pFIleParser->m_pFwBuf[BufIdx], 8);
				BufIdx += 8;
				j -= 4;
			}
			else
			{
				memcpy((void*)pKvCan->m_TxCan.MSG, (void*)&pFIleParser->m_pFwBuf[BufIdx], j * 2);
				for (int k = j * 2; k < 8; k++) pKvCan->m_TxCan.MSG[k] = 0xff;  // Stuffing
				j = 0;
			}

			pKvCan->m_TxCan.DLC = 8;
			pKvCan->m_TxCan.ID = RcvID;
			bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
			if (!rst) return eCANTX;
			ReturnCode = RcvRspMsgId(hCan,0x10, pKvCan);
			if (ReturnCode != DNLOAD_MSG)
				return eCANACK;
		}

		SendCommand(hCan, RcvID, VERIFY_MSG, pKvCan);
		ReturnCode = RcvRspMsgId(hCan,0x10,pKvCan);
		if (ReturnCode != VERIFY_MSG)
			return eVERIFY;

		CRC_RD = m_CRC_RD;
		if (CRC_CC != CRC_RD)
		{
			pKvCan->m_CanCtrlMessage.Format(_T("x"));
			SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
			return eFILECRC;
		}
		else
		{
			pKvCan->m_CanCtrlMessage.Format(_T("D"));
			SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
		}
		CRC_FLS += CRC_RD;
		

		if (Mode)
		{
			SendCommand(hCan, RcvID, PROGRAM_MSG, pKvCan);
			ReturnCode = RcvRspMsgId(hCan, 0x10, pKvCan);
			if (ReturnCode != PROGRAM_MSG)
				return ePROGRAM;
			else
			{
				pKvCan->m_CanCtrlMessage.Format(_T("P"));
				SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);
			}
		}

		pKvCan->m_Progress = ((pFIleParser->m_pHTBHeader->dwFileSize - TransferFileSize) / pFIleParser->m_pHTBHeader->dwFileSize) * 100;
		SendMessage(pKvCan->m_hWnd_net, WM_PROGRESS, 0, (LPARAM)this);

	}


	pKvCan->m_CanCtrlMessage.Format(_T(" --> Done\r\n"));
	SendMessage(pKvCan->m_hWnd_net, WM_CANMESSAGE, 0, (LPARAM)this);



	return true;
}






UINT16 CProtocols::crc16(unsigned char* data_p, int length)
{
	// TODO: 여기에 구현 코드 추가.
	UINT8 x;
	UINT16 crc = 0xFFFF;

	while (length--) {
		x = crc >> 8 ^ *data_p++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
	}
	return crc;
}




#if 0
void CProtocols::SendCheckDspCpuStatSPI(INT32 hCan, unsigned int ReceiverID, CCanKvaser* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.

	pKvCan->m_TxCan.MSG[0] = HEADER_CODE;
	pKvCan->m_TxCan.MSG[1] = CHECK_STAT;
	pKvCan->m_TxCan.MSG[2] = 0xFA;
	pKvCan->m_TxCan.DLC = 3;

	pKvCan->m_TxCan.ID = ReceiverID;
	pKvCan->m_TxCan.FLAGS = (canMSG_STD);

	pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
}

UINT16 CProtocols::CheckDspStat2(INT32 hCan, CCanKvaser* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// //cpuID를 실어서 상태 체크 명령을 보냄,
	// boot loader / application 중 하나를 return, 실패시 0 return;
	UINT16 ReturnCode;

	SendCheckDspCpuStatSPI(hCan, MBU_CAN_ID, pKvCan);
	ReturnCode = RcvRspMsg2(hCan, pKvCan);

	return ReturnCode;
}

bool CProtocols::SwitchMode2(INT32 hCan, CCanKvaser* pKvCan, UINT16 Mode)
{
	// TODO: 여기에 구현 코드 추가.
#if 0
	UINT16 ReturnCode;




	// 입력 모드 오류
	if (((Mode != STAT_APP) && (Mode != STAT_BTL)))
		return false;
	JumpToApp(hCan, DEFAULT_ENTRY, MBU_CAN_ID, pKvCan);

	//// 현재 상태 확인
	//ReturnCode = CheckDspStat(hCan, pKvCan);

	//// 확인시 Error 처리
	//if (ReturnCode == 0)
	//{
	//	Wait(100);
	//	ReturnCode = CheckDspStat(hCan, pKvCan); // try one more time
	//	if (ReturnCode == 0 || ((ReturnCode != STAT_APP) && (ReturnCode != STAT_BTL)))
	//		return false;
	//}

	//// 원하는 상태가 현재 상태와 같으면 true;
	//if (Mode == ReturnCode)
	//	return true;
	//// 다른 경우
	//else
	//{
	//	// BTL 로 전환 
	//	if (Mode == STAT_BTL)
	//		JumpToBtl(hCan, pKvCan);
	//	// APP으로 전환 
	//	else if (Mode == STAT_APP)
	//		JumpToApp(hCan, DEFAULT_ENTRY, BCU_CAN_ID, pKvCan);

	//	//else // 다른 경우는 없다.

	//	// switch 할때 까지 대기
	//	Wait(250);
	//	ReturnCode = CheckDspStat(hCan, pKvCan); // try one more time

	//	// 정상적인 처리
	//	if (ReturnCode == Mode) return true;
	//	// 커맨드를 보냈으나 실패
	//	else return false;
	//}
#endif
	return true;

}

bool CProtocols::EraseAppArea2(INT32 hCan, UINT32 RcvID, CCanKvaser* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// 내부 App area와 Mark 영역을 지우는 동작

	UINT16 ReturnCode;


	//// 1. DSP의 상태확인
//	ReturnCode = CheckDspStat2(hCan, pKvCan);
//	if (ReturnCode == 0) ReturnCode = CheckDspStat2(hCan, pKvCan); // 한번 더 시도
//	if (ReturnCode != STAT_BTL+1)
//		return false; // bootloader 상태가 아니면 종료
	////bootloader 상태인 경우 ERASE 진행

	// 2. ERASE MESSAGE 전송후 ACK 수신
	SendCommand(hCan, RcvID, ERASE_MSG, pKvCan);


	ReturnCode = RcvRspMsg2(hCan, pKvCan);
	if (ReturnCode != ERASE_MSG)
		return false; // NAK 면 종료

	//// 3. DEL_MARK_MSG 전송후 ACK 수신
	//SendCommand(hCan, RcvID, DEL_MARK_MSG, pKvCan);
	//ReturnCode = RcvRspMsg(hCan, pKvCan);
	//if (ReturnCode != DEL_MARK_MSG)
	//	return false; // NAK 면 종료

	Sleep(1000);

	return true; // 정상 동작
}
int CProtocols::DnLoadFirmware2(INT32 hCan, UINT32 RcvID, CCanKvaser* pKvCan, CFileParser* pFIleParser, bool Mode)
{
	// TODO: 여기에 구현 코드 추가.
	UINT16 ReturnCode;
	UINT16 CRC_MEM, CRC_FLS, CRC_CC, CRC_RD;
	UINT32 SendBlockAddr, SendBlockSize;
	DWORD	BufIdx;
	int i, j;

#if 0

	if (hCan < 0)
	{
		pKvCan->m_CanCtrlMessage.Format(_T("CAN NOT OPENED or ERROR \r\n"));
		SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);

		return eCANSOCK;
	}

#if 0
	//// 1. DSP의 상태확인
	//ReturnCode = CheckDspStat(hCan, pKvCan);
	//if (ReturnCode == 0) ReturnCode = CheckDspStat(hCan, pKvCan); // 한번 더 시도
	//if (ReturnCode != STAT_BTL)
	//	return false; // bootloader 상태가 아니면 종료
	////bootloader 상태인 경우 진행
#endif

	if (pFIleParser->m_BinInfo.NumSections < 1)
	{
		pKvCan->m_CanCtrlMessage.Format(_T("FILE NOT OPENED or ERROR \r\n"));
		SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
		return eFILE;
	}

	if (Mode)
	{
		pKvCan->m_CanCtrlMessage.Format(_T("Update FLASH to CPU2 \r\n"));
		SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
	}
	//else
	//{

	//	pKvCan->m_NetCtrlMessage.Format(_T("Update Extenal EEPROM "));
	//	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);

	//	SendCommandDA(hCan, RcvID, BLK_STRUC_MSG, pKvCan, pFIleParser->m_BinInfo.EntryPoint);
	//	ReturnCode = RcvRspMsg(hCan, pKvCan);
	//	if (ReturnCode != BLK_STRUC_MSG)
	//		return eDNLOADADDR;

	//	SendCommandDL(hCan, RcvID, BLK_STRUC_MSG, pKvCan, 0xFFFF, pFIleParser->m_BinInfo.NumSections);
	//	ReturnCode = RcvRspMsg(hCan, pKvCan);
	//	if (ReturnCode != BLK_STRUC_MSG)
	//		return eDNLOADLEN;

	//}

	CRC_MEM = 0;
	CRC_FLS = 0;

	for (i = 0; i < pFIleParser->m_BinInfo.NumSections; i++)
	{

		SendBlockAddr = pFIleParser->m_BinInfo.pSections[i].EntryPoint;
		SendBlockSize = pFIleParser->m_BinInfo.pSections[i].SizeSector;
		BufIdx = pFIleParser->m_BinInfo.pSections[i].BufOffset;

		//if(!Mode)
		//{
		//	SendCommandDA(hCan, RcvID, BLK_STRUC_MSG, pKvCan, SendBlockAddr);
		//	ReturnCode = RcvRspMsg(hCan, pKvCan);
		//	if (ReturnCode != BLK_STRUC_MSG)
		//		return eDNLOADADDR;

		//	SendCommandDL(hCan, RcvID, BLK_STRUC_MSG, pKvCan, SendBlockSize, i);
		//	ReturnCode = RcvRspMsg(hCan, pKvCan);
		//	if (ReturnCode != BLK_STRUC_MSG)
		//		return eDNLOADLEN;
		//}

		pKvCan->m_CanCtrlMessage.Format(_T("Sending %d th block E:%08X  S:%08X : "), i, SendBlockAddr, SendBlockSize * 2);
		SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);

		while (SendBlockSize > 0)
		{
			SendCommandDA(hCan, RcvID, DNLOAD_MSG, pKvCan, SendBlockAddr);
			ReturnCode = RcvRspMsg2(hCan, pKvCan);
			if (ReturnCode != DNLOAD_MSG)
				return eDNLOADADDR;


			if (SendBlockSize <= DNLOAD_SIZE)
			{
				SendCommandDL(hCan, RcvID, DNLOAD_MSG, pKvCan, SendBlockSize, i);
				ReturnCode = RcvRspMsg2(hCan, pKvCan);
				if (ReturnCode != DNLOAD_MSG)
					return eDNLOADLEN;

				j = SendBlockSize;
			}
			else
			{
				SendCommandDL(hCan, RcvID, DNLOAD_MSG, pKvCan, DNLOAD_SIZE, i);
				ReturnCode = RcvRspMsg2(hCan, pKvCan);
				if (ReturnCode != DNLOAD_MSG)
					return eDNLOADLEN;

				j = DNLOAD_SIZE;
			}

			CRC_CC = crc16(&pFIleParser->m_pFwBuf[BufIdx], j * 2);
			SendBlockSize -= j;// Decrease DNLOAD_SIZE
			SendBlockAddr += j;// Increase DNLOAD_SI

			CRC_MEM += CRC_CC;

			while (j > 0)
			{
				if (j >= 4)
				{
					memcpy((void*)pKvCan->m_TxCan.MSG, (void*)&pFIleParser->m_pFwBuf[BufIdx], 8);
					BufIdx += 8;
					j -= 4;
				}
				else
				{
					memcpy((void*)pKvCan->m_TxCan.MSG, (void*)&pFIleParser->m_pFwBuf[BufIdx], j * 2);
					for (int k = j * 2; k < 8; k++) pKvCan->m_TxCan.MSG[k] = 0xff;  // Stuffing
					j = 0;
				}

				pKvCan->m_TxCan.DLC = 8;
				pKvCan->m_TxCan.ID = RcvID;
				bool rst = pKvCan->CanWrite(hCan, &pKvCan->m_TxCan);
				if (!rst) return eCANTX;
				ReturnCode = RcvRspMsg2(hCan, pKvCan);
				if (ReturnCode != DNLOAD_MSG)
					return eCANACK;
				//Sleep(800);
			}

			//// VERIFY
			SendCommand(hCan, RcvID, VERIFY_MSG, pKvCan);
			ReturnCode = RcvRspMsg(hCan, pKvCan);
			if (ReturnCode != VERIFY_MSG)
				return eVERIFY;

			CRC_RD = m_CRC_RD;// (pKvCan->m_RxCan.MSG[2]) | (pKvCan->m_RxCan.MSG[3] << 8);

			if (CRC_CC != CRC_RD)
			{
				pKvCan->m_CanCtrlMessage.Format(_T("x"));
				SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
				//			return eFILECRC;
			}
			else
			{
				pKvCan->m_CanCtrlMessage.Format(_T("D"));
				SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
			}

			CRC_RD = 0;


			if (Mode)
			{
				SendCommand(hCan, RcvID, PROGRAM_MSG, pKvCan);
				ReturnCode = RcvRspMsg2(hCan, pKvCan);
				if (ReturnCode != PROGRAM_MSG)
					return ePROGRAM;
				else
				{
					pKvCan->m_CanCtrlMessage.Format(_T("P"));
					SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
				}
			}
			//else
			//{
			//	SendCommand(hCan, RcvID, EEPROM_WRITE_MSG, pKvCan);
			//	ReturnCode = RcvRspMsg(hCan, pKvCan);
			//	if (ReturnCode != EEPROM_WRITE_MSG)
			//		return ePROGRAM;
			//	else
			//	{
			//		pKvCan->m_NetCtrlMessage.Format(_T("P"));
			//		SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
			//	}
			//}



			CRC_RD = m_CRC_RD;
			CRC_FLS += CRC_RD;

		}
		pKvCan->m_CanCtrlMessage.Format(_T(" --> Done\r\n"));
		SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);

	}

	//if (CRC_FLS != CRC_MEM)
	//{
	//	pKvCan->m_NetCtrlMessage.Format(_T(" Flash CRC ERROR \r\n"));
	//	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
	//	return eMEMCRC;
	//}
	//else
	//{
	pKvCan->m_CanCtrlMessage.Format(_T(" Flash Success \r\n"));
	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
	//}

	Sleep(1000);

	//if (Mode)
	//{
		//SendCommand(hCan, RcvID, MARK_MSG, pKvCan);
		//ReturnCode = RcvRspMsg(hCan, pKvCan);
		//if (ReturnCode != MARK_MSG)
		//{
		//	pKvCan->m_NetCtrlMessage.Format(_T(" Flag Mark ERROR \r\n"));
		//	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);

		//	return ePROGRAM;
		//}
		//else
		//{
	pKvCan->m_CanCtrlMessage.Format(_T("Update Firmware  All Done\r\n"));
	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
	//}
//}
//else
//{
//	pKvCan->m_NetCtrlMessage.Format(_T("Update Firmware  All Done\r\n"));
//	SendMessage(pKvCan->m_hWnd, WM_CANMESSAGE, 0, (LPARAM)this);
//}
#endif
	return true;
}

UINT16 CProtocols::RcvRspMsg2(INT32 hCan, CCanKvaser* pKvCan)
{
	// TODO: 여기에 구현 코드 추가.
	// 명령을 보낸 후 DSP에서 수신되는 ACK/NAK를 수신
	// check status는 btl/app의 상태를 return
	// 다른 명령어는 받은 명령을 되돌려 보냄.

	UINT16 MessageCode;
	bool Rst = pKvCan->CanReadSpecific(hCan, PCS_CAN_ID, &pKvCan->m_RxCan, 200000); // Kvaser의 경우 여기서 ID가 체크됨. 500ms 타임아웃을 3회 반복함.
	if (Rst)
	{
		if (pKvCan->m_RxCan.ID == PCS_CAN_ID)  // 정상 수신의 경우 다시한번 체크
		{
			if (pKvCan->m_RxCan.MSG[0] != HEADER_CODE)
				return 0; // 헤더 오류
			if (pKvCan->m_RxCan.MSG[pKvCan->m_RxCan.DLC - 1] != RTAIL_CODE)
				return 0; // 테일 오류

			MessageCode = (UINT16)pKvCan->m_RxCan.MSG[1];
			if (MessageCode == CHECK_STAT)  // Check status의 응답이면 status의 조합을 return
			{
				MessageCode = (UINT16)((pKvCan->m_RxCan.MSG[3] & 0x0f) | (pKvCan->m_RxCan.MSG[4] & 0xf0)); // Dual core stat
			}
			//else if (MessageCode == VERIFY_MSG || MessageCode == PROGRAM_MSG || MessageCode == EEPROM_WRITE_MSG) // 이들 명령어는 CRC를 반고,
			//{
			//	m_CRC_RD = (pKvCan->m_RxCan.MSG[2] & 0xff) | (pKvCan->m_RxCan.MSG[3] & 0xff) << 8;
			//}

			return MessageCode; // 돌아온 메시지를 return, 예외: check status는 DSP의 상태를 return함
		}
		else
			return 0; // 수신 ID 다름
	}
	else
		return 0; // 수신실패 timeout
}
#endif

#endif