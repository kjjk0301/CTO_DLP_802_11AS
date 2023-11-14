#pragma once
#include "CNetHandle.h"
#include "CFileParser.h"
#include "Typesdef.h"
#if 0
#include "canlib.h"
#endif


#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

#define CANBMUID 			0x555
#define CAN_MASTER_DSPID 	0x551
#define CAN_SLAVE_DSPID 	0x552
#define SWITCH_BTL_ID 		0x630
#define CHIP_STATUS_ID 		0x730
//#define CHIP_STATUS_ID 		0x551


#define PCS_CAN_ID 			0x100  // HOST
//#define MBU_CAN_ID 			0x200  // BCU
#define MBU_CAN_ID 			0x301  // BCU
#define CT1_CAN_ID 			0x300  // BCU

#define MS_1    (0x4D)  // Single CPU


#define HEADER_CODE		0xFF
#define TAIL_CODE		0xFA
#define RTAIL_CODE		0xF0

#define CHECK_STAT		0x73
#define STAT_BTL		0x0B
#define STAT_APP		0x0A

enum UPDATE_CMD
{
	ERROR_MSG = 0,	
	STATUS_MSG,		
	VERSION_MSG,	
	DNLOAD_MSG,		
	VERIFY_MSG,
	ERASE_MSG,
	PROGRAM_MSG,
	START_MSG,
	MARK_MSG,
#if 0   // COMMAND MESSAGE used for EXT EEPROM
	DEL_MARK_MSG,
	EEPROM_WRITE_MSG,
	MARK_EEPROM_MSG,
	BLK_STRUC_MSG,
	EEPROM_TO_FLASH_MSG,
#endif
	START_UPDATE,
	EXIT_UPDATE,
	REBOOT,
	CPUSTAT_PACK = 0x73
};

// SUB COMMAND ID in DOWNLOAD MESSAGE
enum OPTION_CMD
{
	DN_ADDRESS = 0,
	DN_SIZE,
	ERASE_PRG,
	ERASE_IMG,
	ERASE_PRGFLAG,
	ERASE_IMGFLAG,
	JUMP_PRG,
	RESTART_WARM,
	RESTART_COLD
};


// CMD PACKET LENGTH
enum CMD_PACK_LEN
{
	CMD_LEN_TYPE3 = 3,     // NORMAL CMD
	CMD_LEN_TYPE4,          // ERASE MSG
	CMD_LEN_TYPE5,          //
	CMD_LEN_TYPE6,          //
	CMD_LEN_TYPE7,          //
	CMD_LEN_TYPE8           // DOWNLOAD MSG
};

// ACK PACKET LENGTH
enum ACK_PACK_LEN
{
	ACK_LEN_N = 3,
	ACK_LEN_CRC = 5,
	ACK_LEN_CSTAT = 6,
	ACK_LEN_STAT = 8
};


#define CMD_EXT_LEN     7
#define CMD_EXT2_LEN     5


#define DEFAULT_ENTRY	0x87000
#define DNLOAD_SIZE     64
//#define DNLOAD_SIZE   2048

#define IN_UPDATE		true
#define EXT_UPDATE		false

extern ParamContext Table10Context;

enum ActionStatus { SUCCESS = 0, eFILECODE, eVERCODE, eCANSOCK, eFILE, eFILECRC, eFLASHERASE, eDNLOADADDR, eDNLOADLEN, eCANTX, eCANACK, eVERIFY, ePROGRAM, eMARK, eSTART, eNORES, eMEMCRC };
enum RwStatus { R0 = 0, W0, W1 };
enum ACKStat {ACK = 1, NAK = 2};

class CProtocols
{
public:
	CProtocols();
	~CProtocols();

private:
	UINT16 crc16(unsigned char* data_p, int length);

private:
#if 0
	// Send Command
	void SendCommand(INT32 hCan, unsigned int ReceiverID, unsigned char MessageCode, CNetHandle * pKvCan);
	void SendCommand(INT32 hCan, unsigned int ReceiverID, unsigned char MessageCode, unsigned char OptionCode, CNetHandle* pKvCan);
	void SendCommandDAL(INT32 hCan, UINT32 RcvID, UINT8 MessageCode, UINT8 OptionCode,CNetHandle * pKvCan, UINT32 Parameter);

	void JumpToBtl(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan);
	void JumpToApp(INT32 hCan, UINT32 EntryAddr, UINT32 RcvID, CNetHandle * pKvCan);
	void SendCheckDspCpuStat(INT32 hCan, unsigned int ReceiverID, CNetHandle* pKvCan);
		// Rcv Ack
	UINT16 RcvRspMsg(INT32 hCan, CNetHandle * pKvCan);
//	UINT16 RcvRspMsg(INT32 hCan, UINT32 RcvID, CCanKvaser* pKvCan);
	UINT16 RcvRspMsg(INT32 hCan, CNetHandle * pKvCan, UINT32 TimeOut);
//	UINT16 RcvRspMsg(INT32 hCan, UINT32 RcvID, CCanKvaser* pKvCan, UINT32 TimeOut);

#if 0
	UINT16 RcvStatMsgW(INT32 hCan, CCanKvaser* pKvCan, long RcvId);
	UINT16 RcvStatMsgR(INT32 hCan, CCanKvaser* pKvCan, UINT16* RcvDat, UINT16 RcvId);
#endif 

	UINT16 RcvRspMsgId(INT32 hCan,UINT32 Id ,CNetHandle* pKvCan);
	UINT16 RcvRspMsgId(INT32 hCan,UINT32 Id,CNetHandle* pKvCan, UINT32 TimeOut);



	INT16 RcvMsgType0(INT32 hCan, CNetHandle* pKvCan, UINT16 Msg, long RcvId);
	INT16 RcvMsgType1(INT32 hCan, CNetHandle* pKvCan, UINT16 Msg, long RcvId);

#endif
	



public:
	UINT16 m_CRC_RD;
	HWND	m_hWnd;
	char  m_CMDS[1024];

	void MakePayload(UINT8* pSrc, UINT16 nStat, UINT16 nIdx, UINT16 nSz);
	void MmdWriteCmd();
	void MmdWriteStardCmd();
	bool CheckCmdMatch(CNetHandle* hNet, char *pOrg,int nCmd);
	void CmdCheck();
	void SeqCheckCmd();
	bool CheckAck(CNetHandle* hNet, char* pOrg, bool CheckType);

	bool CheckLastCmd(CNetHandle* hNet);
	bool EraseMMC(CNetHandle* hNet);
	bool SendImagePacket(CNetHandle* hNet, unsigned char* pPtr, int nIdx);
	bool ProgramMMC(CNetHandle* hNet);
	bool PacketCheck(CNetHandle* hNet);
	

	
//	int DnLoadImage2INT32 hCan, UINT32 RcvID, CNetHandle* pKvCan, CFileParser* pFIleParser, bool Mode);

#if 0
// DSP status check
	UINT16 CheckDspStat(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan);
	UINT16 CheckDspStat(INT32 hCan, UINT32 RcvID, CNetHandle* pKvCan,UINT32 TimeOut);
// Switch Mode
	bool SwitchMode(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan, UINT16 Mode);
// Erase Flash
	bool EraseAppArea(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan);
	bool EraseImgArea(INT32 hCan, UINT32 RcvID, CNetHandle* pKvCan);
// Mark Boot Flag
	bool MarkBootFlag(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan);
// Update Sequence
	int DnLoadFirmware(INT32 hCan, UINT32 RcvID, CNetHandle * pKvCan, CFileParser *pFIleParser, bool Mode);
	int DnLoadImage(INT32 hCan, UINT32 RcvID, CNetHandle* pKvCan, CFileParser* pFIleParser, bool Mode);
// Device status check

// Write command data
	bool WriteCmd(INT32 hCan, CNetHandle* pKvCan, UINT16* data, INT32 index, INT16 TargetId);
	bool ReadCmd(INT32 hCan, CNetHandle* pKvCan, UINT16* data, INT32 index, INT16 TargetId);
#if 0

	void SendCommandDA(INT32 hCan, UINT32 RcvID, unsigned char MessageCode, CCanKvaser* pKvCan, UINT32 SendBlockAddr);
	void SendCommandDL(INT32 hCan, UINT32 RcvID, unsigned char MessageCode, CCanKvaser * pKvCan, UINT32 LEN, UINT32 blkNum);

	UINT16 CheckDspStat2(INT32 hCan, CCanKvaser* pKvCan);
	bool SwitchMode2(INT32 hCan, CCanKvaser* pKvCan, UINT16 Mode);
	bool EraseAppArea2(INT32 hCan, UINT32 RcvID, CCanKvaser* pKvCan);
	int DnLoadFirmware2(INT32 hCan, UINT32 RcvID, CCanKvaser* pKvCan, CFileParser* pFIleParser, bool Mode);
	UINT16 RcvRspMsg2(INT32 hCan, CCanKvaser* pKvCan);
#endif
#endif
};

