#include "pch.h"
#include "CFileParser.h"
#include "ParserUtil.h"

CFileParser::CFileParser()
{
	m_hFile = NULL;
	m_pFwBuf = NULL;
}

CFileParser::~CFileParser()
{
	if (m_hFile != NULL)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	if (m_pFwBuf != NULL)
	{
		free(m_pFwBuf);
		m_pFwBuf = NULL;
	}
}


bool CFileParser::OpenImage(CString FilePathName)
{
	DWORD dwFileSize = 0;
	DWORD dwBufIdx = 0;
	DWORD dwRead = 0;

	m_hFile = NULL;
	m_pFwBuf = NULL;

	CString NameExt = strClip(FilePathName, 2);

	m_hFile = CreateFile(FilePathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(m_hFile, NULL);
		m_FileSz = dwFileSize;
		if (dwFileSize != 0xFFFFFFFF)
		{
			m_pFwBuf = (unsigned char*)malloc(dwFileSize);
			if (m_pFwBuf == NULL)
			{
				CloseHandle(m_hFile);
				m_hFile = NULL;
				m_FileSz = 0;
				AfxMessageBox(_T("Memory alloc error"), MB_OK);
				return false;
			}
			else
			{
				// Read all file contents and close file handle
				ZeroMemory(m_pFwBuf, sizeof(unsigned char) * (dwFileSize));
				dwRead = 0;
				ReadFile(m_hFile, m_pFwBuf, dwFileSize, &dwRead, NULL);
				CloseHandle(m_hFile);
				m_hFile = NULL;

				return true;
			}
		}
		else
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
			m_FileSz = 0;
			AfxMessageBox(_T("FileSize error"), MB_OK);
			
			return false;

		}
	}
	else
	{
		m_hFile = NULL;
		m_FileSz = 0;
		AfxMessageBox(_T("FileOpen error"), MB_OK);
		return false;
	}
}
