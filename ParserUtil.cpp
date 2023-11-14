#include "pch.h"
#include "BinType.h"





void GetHtbInfo(unsigned char *pFwBuf, DWORD * pdwBufIdx, HTBInfo * pHtbInfo)
{
	const char * pBuf = (const char *)(pFwBuf);
	DWORD	Idx = *pdwBufIdx;


	sscanf_s(&pBuf[Idx], "%s\n", pHtbInfo->cPrgCode, 32);
	Idx += (DWORD) (strlen(pHtbInfo->cPrgCode) + 1);

	sscanf_s(&pBuf[Idx], "%s\n", pHtbInfo->cVerCode, 32);
	Idx += (DWORD) (strlen(pHtbInfo->cVerCode) + 1);

	sscanf_s(&pBuf[Idx], "%s\n", pHtbInfo->cSizeStr, 32);
	Idx += (DWORD) (strlen(pHtbInfo->cSizeStr) + 1);

	sscanf_s(pHtbInfo->cSizeStr, "%d", &(pHtbInfo->dwBinSize));

	pHtbInfo->CRC_FILE = (pBuf[Idx++] & 0xff);
    pHtbInfo->CRC_FILE |= (pBuf[Idx++] & 0xff) << 8;


	*pdwBufIdx = Idx;
}

UINT32 GetEntry(unsigned char *pFwBuf, DWORD * pdwBufIdx)
{
	DWORD	Idx = *pdwBufIdx;
	UINT32 TLV = 0;

	TLV = (pFwBuf[Idx++] & 0xFF)  <<16;
	TLV |= (pFwBuf[Idx++] & 0xFF) <<24; 
	TLV |=	(pFwBuf[Idx++] & 0xFF);
	TLV |=	(pFwBuf[Idx++] & 0xFF)<<8;
	*pdwBufIdx = Idx;

	return TLV;
}

UINT16 GetSize(unsigned char *pFwBuf, DWORD * pdwBufIdx)
{
	DWORD	Idx = *pdwBufIdx;
	UINT16	TLV = 0;

	TLV |= (pFwBuf[Idx++] & 0xFF);
	TLV |= (pFwBuf[Idx++] & 0xFF) << 8;
	
	*pdwBufIdx = Idx;
	return TLV;
}


#if 0
UINT32 GetByteTLV(unsigned char *pFwBuf, DWORD * pdwBufIdx,unsigned int ByteCnt)
{
	DWORD	Idx = *pdwBufIdx;
	UINT32 TLV = 0;

	if (ByteCnt > 4) return 0;

	for(int i=0;i< ByteCnt;i++)
		TLV |= (pFwBuf[Idx++] & 0xFF) << (8*i);

	*pdwBufIdx = Idx;

	return TLV;
}
#endif



void PreParseBin(unsigned char *pFwBuf, DWORD * pdwBufIdx, DWORD FileSize,BinInfo * pBinInfo)
{
	UINT32 TLV = 0;
	UINT16 BLK_SIZE = 0;
	DWORD Idx = *pdwBufIdx;
	UINT16 NumSections = 0;


	// 2Byte Magic Word 0x08AA
	TLV = GetSize(pFwBuf, &Idx); // Not size, just magic word
	pBinInfo->MagicId = (UINT16)TLV;

	// Skip dummy
	Idx += 16;

	// Get Global entry
	TLV = GetEntry(pFwBuf, &Idx);
	pBinInfo->EntryPoint = TLV;

	*pdwBufIdx = Idx; 
	
	for(;;)
	{
		BLK_SIZE = GetSize(pFwBuf, &Idx);
		TLV = GetEntry(pFwBuf, &Idx);
		Idx += BLK_SIZE*2;

		if ((Idx <= FileSize) && (BLK_SIZE != 0x0000)) // End of binary 0x0000
			NumSections++;
		else 
			break;
	} 

	pBinInfo ->NumSections= NumSections ;

}


bool AllocSections(BinInfo * pBinInfo)
{
	if (pBinInfo->NumSections > 0)
	{
		if (pBinInfo->pSections != NULL)
		{
			return false;
		}
		else
		{
			pBinInfo->pSections = (SectionInfo *)malloc(sizeof(SectionInfo)*pBinInfo->NumSections);
			if (pBinInfo->pSections == NULL)
			{
				return false;
			}
			else
				return true;

		}
	}
	else
		return false;
}

void GetSectionInfo(unsigned char *pFwBuf, DWORD * pdwBufIdx, BinInfo * pBinInfo)
{
	DWORD Idx = *pdwBufIdx;

	for (int i = 0; i < pBinInfo->NumSections; i++)
	{
		pBinInfo->pSections[i].SizeSector	= GetSize(pFwBuf, &Idx);
		pBinInfo->pSections[i].EntryPoint	= GetEntry(pFwBuf, &Idx);
		pBinInfo->pSections[i].BufOffset	= Idx;

		Idx += pBinInfo->pSections[i].SizeSector * 2;
	}
	pBinInfo->EndPoint = Idx;
}



int GetNumSector(unsigned char *pFwBuf, DWORD dwBufIdx, DWORD dwFileSize)
{
	DWORD	Idx = dwBufIdx;
	unsigned int SectorSize=0;

	//while (Idx<= dwFileSize)
	//{
	//	SectorSize = pFwBuf


	//}


//	*pdwBufIdx = Idx;
	return 1;
}

CString strClip(CString str, int nType)
{
	//파일 Full Path를 복사
	TCHAR szTmp[4096];
	StrCpy(szTmp, str);
	CString strTmp;
	CString strResult = _T("");

	switch (nType)
	{
	case 0:
		//파일의 경로만 복사.
		PathRemoveFileSpec(szTmp);
		strResult = szTmp;

		break;
	case 1:
		// 1: 파일 이름만 복사
		strResult = PathFindFileName(szTmp);
		//strResult = szTmp;
		break;
	case 2:
		// 2: 파일 확장자 복사
		strResult = PathFindExtension(szTmp);
		break;
	case 3:
		// 3: 확장자를 뺀 파일명 복사

		strTmp = PathFindFileName(szTmp);
		ZeroMemory(szTmp, 4096);
		StrCpy(szTmp, strTmp);
		PathRemoveExtension(szTmp);
		strResult = szTmp;
		break;

	case 4:
		// 4: 2번케이스의 파일 확장자에서 .을 뺌.
		strResult = PathFindExtension(szTmp);
		strResult = strResult.Right(strResult.GetLength() - 1);
		break;
	}

	return strResult;

}