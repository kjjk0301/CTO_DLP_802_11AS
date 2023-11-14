#pragma once

void GetHtbInfo(unsigned char *pFwBuf, DWORD * pdwBufIdx, HTBInfo * pHtbInfo);
UINT32 GetEntry(unsigned char *pFwBuf, DWORD * pdwBufIdx);
UINT16 GetSize(unsigned char *pFwBuf, DWORD * pdwBufIdx);
void PreParseBin(unsigned char *pFwBuf, DWORD * pdwBufIdx, DWORD FileSize, BinInfo * pBinInfo);
bool AllocSections(BinInfo * pBinInfo);
void GetSectionInfo(unsigned char *pFwBuf, DWORD * pdwBufIdx, BinInfo * pBinInfo);
CString strClip(CString str, int nType);