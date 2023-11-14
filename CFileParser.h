#pragma once

#include "BinType.h"


 


class CFileParser
{
public:
	CFileParser();
	virtual ~CFileParser();

private:
//	unsigned int m_uiEntryPoint;
//	unsigned int m_uiNumSector;


public:
	HANDLE m_hFile;
	unsigned char* m_pFwBuf;
	DWORD m_FileSz;
	CString FileName;
	bool OpenImage(CString FilePathName);
};

