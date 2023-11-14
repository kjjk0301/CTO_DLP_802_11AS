#pragma once
#include <string>

typedef struct ParamStruct
{
	UINT16  Index;
	UINT16  IDProperty;
	UINT16	Offset;
	UINT16	PacketOffset;
	UINT8	Type;
	UINT8	TypeLen;
	UINT8	TypeOffsetLen;
	UINT8	RWProperty;
	UINT8	VProperty;
	INT16	Min;
	INT16	Max;
	UINT16	SCProperty;
	UINT8	Mwr;
	UINT8	Fwr;
	UINT8	Mrd;
	UINT16	SDProperty;
	UINT16	Current;
	char    UintStr[16];
	char	TName[16];
	char	KName[32];
	char	EName[32];
	char	Rw[16];
	char	VNVName[32];
	char	Visible[16];
	bool	MinDef;
	bool	MaxDef;
	bool	ScDef;
	bool	Ofdef;
	bool	Dfdef;
}ParamStruct;
 
typedef struct ParamContext
{
	ParamStruct* pArrayUnitItems;
	ParamStruct* pArrayUnitItemsMemWr;
	ParamStruct* pArrayUnitItemsFlsWr;
	ParamStruct* pArrayUnitItemsMemRd;
	UINT16 UnitItemCnt;
	UINT16 UnitFieldCnt;
	UINT16 MemWrCnt;
	UINT16 FLSWrCnt;
	UINT16 MemRdCnt;
}ParamContext;



typedef struct tCmdInfo
{
	unsigned int	index;
	unsigned int	nCmdType;
	unsigned int	nDestType;
	unsigned int	nRegAddress;
	unsigned int	nRegData;
	char		strCmdName[32];
	char		strCmdType[8];
	char		strDestiNation[8];
	char		strRegAddress[8];
	char		strRegData[16];
	char		strDesc[128];
}tCmdInfo_s;




typedef struct tCmdSetInfo
{
	int nCmd;
	char strTableName[32];
	char strDesc[128];
	tCmdInfo_s* CmdsInfoTab;
}tCmdSetInfo_s;

typedef enum ParsingState { ParseSuccess = 0, eFileOpen, eBadFile,eMalloc } ParsingState;
//typedef enum CsvRowPosition { IndexRow = 0, SubIndexRow, KNameRow, ENameRow, ENameValRow, DefaultRow, UnitRow, MinRow, MaxRow, TypeRow, RWRow, SDRow, VNVRow, UpdateTimeRow, VarRow } CsvRowPosition;
typedef enum ItemProperty { NDEFINE = 0, RO_P, RW_P, RSVD_P, STATIC_P, DYNAMIC_P, VOLATILE_P, NVOLATILE_P, IMMEDIATE_P, AFTERBOOT_P, EPRESSURE_P, ECURRENT_P } CsvItemProperty;
typedef enum CsvColumnPosition { iCANID = 0, iIndex, iType, iKName, iName, iMin, iMax, iUnit, iScale, iOffset, iRw, iDefault, iVolatile, iImmediately, iVisible }CsvColumnPosition;
typedef enum CsvTypeClass { tUINT8 = 0, tINT8, tUINT16, tINT16, tUINT32, tINT32, tUINT64, tINT64, tBOOL, tSTRING}CsvTypeClass;

typedef enum CsvColumnPositionDlp{iDlpCmdIndex, iDlpCmdName,iOpType,iDestination,iAddress,iRegVal,iDesc}CsvColumnPositionDlp;
typedef enum CsvCmdTypeDlp { iErrorReg = -3, iErrorVal = -2,iUndef=-1, iWrite, iRead, iFlash }CsvCmdTypeDlp;
typedef enum CsvDestTypeDlp { iDestUndef = -1, iLeft, iRight }CsvDestTypeDlp;






typedef enum TxFlagEnum{ STARTIMG_FLG = 0, SENDINGIMG_FLG, ENDIMG_FLG}TxFlagEnum;




#define IMG_SZ				40448
#define TRANSFER_SZ			512
#define TRANSFER_NUM		40     //(IMG_SZ/TRANSFER_SZ)
#define TRANSFERPACKET_SZ	520   // TRANSFER_SZ+8

#define DEF_PACK_SZ			4
#define REMOTE_ACK			'O'


