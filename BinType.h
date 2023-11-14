#pragma once


typedef struct HTBInfo
{
	char cPrgCode[32];
	char cVerCode[32];
	char cSizeStr[32];
	DWORD dwBinSize;
	DWORD dwFileSize;
	UINT16 CRC_FILE;
}HTBInfo;

typedef struct SectionInfo 
{
	UINT16  SizeSector;
	UINT32	EntryPoint;
	UINT32	BufOffset;
}SectionInfo;




typedef struct BinInfo
{
	UINT16	MagicId;
	UINT16  NumSections;
	UINT32	EntryPoint;
	UINT32	EndPoint;
	SectionInfo * pSections;
}BinInfo;





