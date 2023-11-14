#include "pch.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>
#include "Typesdef.h"


#define MAX_COL 32


using namespace std;
using std::cout;
using std::endl;

std::vector<std::string> csv_read_row(std::istream& in, char delimiter);
std::vector<std::string> csv_read_row(std::string& in, char delimiter);
std::vector<std::string> csv_read_row2(std::istream& in, char delimiter, int* nCol);
std::vector<std::string> csv_read_row2(std::string& line, char delimiter, int* nCol);
//extern "C" ParamContext Table10Context;

#define LOG_LV	0
string FilePathNameDefault = { "CmdTabs.csv" };

tCmdSetInfo * gCmdTabs;
string gStrDataLabel[8];
UINT32 gNField=0;
UINT32 gNSets = 0;


std::vector<std::string> csv_read_row(std::string& line, char delimiter)
{
	std::stringstream ss(line);
	return csv_read_row(ss, delimiter);
}

std::vector<std::string> csv_read_row(std::istream& in, char delimiter)
{
	std::stringstream ss;
	bool inquotes = false;
	std::vector<std::string> row;//relying on RVO
	while (in.good())
	{
		char c = in.get();
		if (!inquotes && c == '"') //beginquotechar
		{
			inquotes = true;
		}
		else if (inquotes && c == '"') //quotechar
		{
			if (in.peek() == '"')//2 consecutive quotes resolve to 1
			{
				ss << (char)in.get();
			}
			else //endquotechar
			{
				inquotes = false;
			}
		}
		else if (!inquotes && c == delimiter) //end of field
		{
			row.push_back(ss.str());
			ss.str("");
		}
		else if (!inquotes && (c == '\r' || c == '\n'))
		{
			if (in.peek() == '\n') { in.get(); }
			row.push_back(ss.str());
			return row;
		}
		else
		{
			ss << c;
		}
	}
}


std::vector<std::string> csv_read_row2(std::string& line, char delimiter, int* nCol)
{
	int Cnt;
	std::vector<std::string> row;//relying on RVO
	std::stringstream ss(line);
	row = csv_read_row2(ss, delimiter,&Cnt);
	*nCol = Cnt;
	return row;
}

#if 1
std::vector<std::string> csv_read_row2(std::istream& in, char delimiter, int * nCol)
{
	std::stringstream ss;
	bool inquotes = false;
	std::vector<std::string> row;//relying on RVO
	int ColCount = 0;
	// nCol is used to count the total Number of count
	*nCol = 0;


	while (in.good())
	{
		char c = in.get();
		if (!inquotes && c == '"') //beginquotechar
		{
			inquotes = true;
		}
		else if (inquotes && c == '"') //quotechar
		{
			if (in.peek() == '"')//2 consecutive quotes resolve to 1
			{
				ss << (char)in.get();
			}
			else //endquotechar
			{
				inquotes = false;
			}
		}
		else if (!inquotes && c == delimiter) //end of field
		{
			row.push_back(ss.str());
			ss.str("");
			ColCount++;
		}
		else if (!inquotes && (c == '\r' || c == '\n'))
		{
			if (in.peek() == '\n') { in.get(); }
			row.push_back(ss.str());
			*nCol = ColCount;
			return row;
		}
		else
		{
			ss << c;
		}
	}
}
#endif


bool CharToHex(char *pBuf, char NumDigit,UINT32 * pVal)
{
	UINT32 Val = 0;

		// Check Hex 
		// first char must be "0" and second one must be "X" or "x"
		if (pBuf[0] != '0')
			return false;
		if ((pBuf[1] != 'X') && (pBuf[1] != 'x'))
			return false;		

		Val = 0;
		for (int i = 0; i < NumDigit; i++)
		{
			Val = Val << 4;
			if (pBuf[2+i] >= '0' && pBuf[2+i] <= '9')
				Val |= pBuf[2+i] - '0';// 48, 0x30 ~ 57, 0x39
			else if (pBuf[2+i] >= 'a' && pBuf[2+i] <= 'f')
				Val |= pBuf[2+i] - 'a' + 10;// 97, 0x61 ~ 102, 0x66
			else if (pBuf[2+i] >= 'A' && pBuf[2+i] <= 'F')
				Val |= pBuf[2+i] - 'A' + 10;// 65, 0x41 ~ 70, 0x46
			else
				return false;
		}
		*pVal = Val;

		return true;
}





int LoadCmdSet(string FilePathName)///, tCmdSetInfo * pCmdsInforTab)
{
	int nColCnt = 0;
	int nSet = 0;
	

	// 1. First Open FIle, Just Look Ahead and Count Number of Items
	std::ifstream in(FilePathName);
	if (in.fail())
	{
		cout << "File not found (open)" << endl;
		return  eFileOpen;
	}
	// 첫줄
	{
		std::vector<std::string> row = csv_read_row2(in, ',', &nColCnt);
		gNField = nColCnt + 1;
		for (int i = 0; i < gNField ; i++)
			gStrDataLabel[i] = row[i];

		while (in.good())
		{
			std::vector<std::string> row = csv_read_row2(in, ',', &nColCnt);

			if (row[iDlpCmdIndex] == "_")
			{
				nSet++;
			}
		}
	}
	in.close();

	gNSets = nSet;

	// 2. Memory allocation for No of Items
	if (nSet > 0)
	{
		gCmdTabs = (tCmdSetInfo*) malloc(sizeof(tCmdSetInfo) * nSet);

		if (gCmdTabs == NULL)
			return eMalloc;

	}
	else
		return eBadFile;		// Parse Error

	int nSetId = 0;
	int nCmdperSet = 0;

	in.open(FilePathName);
	if (in.fail())
	{
		cout << "File not found (open)" << endl;
		return  eFileOpen;
	}
	{
		std::vector<std::string> row = csv_read_row(in, ',');
		while (in.good())
		{
			std::vector<std::string> row = csv_read_row(in, ',');

			if (row[iDlpCmdIndex] == "_")
			{
				gCmdTabs[nSetId].nCmd = nCmdperSet;
				strcpy_s(gCmdTabs[nSetId].strTableName, row[iDlpCmdName].c_str());
				strcpy_s(gCmdTabs[nSetId].strDesc, row[iDlpCmdName+1].c_str());
				nSetId++;
				nCmdperSet = 0;
			}
			else
			{
				nCmdperSet++;
			}
		}
	}
	in.close();


	for (int i = 0; i < nSet; i++)
	{
		gCmdTabs[i].CmdsInfoTab = (tCmdInfo*)malloc(sizeof(tCmdInfo) * gCmdTabs[i].nCmd);
		if (gCmdTabs[i].CmdsInfoTab == NULL)
		{
			for (int j = 0; i < i-1; j++) free(gCmdTabs[i].CmdsInfoTab);

			free(gCmdTabs);
			return  eMalloc;
		}
	}

	nSetId = 0;
	nCmdperSet = 0;

	in.open(FilePathName);
	if (in.fail())
	{
		cout << "File not found (open)" << endl;
		return  eFileOpen;
	}
	{
		std::vector<std::string> row = csv_read_row(in, ',');
		while (in.good())
		{
			std::vector<std::string> row = csv_read_row(in, ',');

			if (row[iDlpCmdIndex] == "_")
			{
				nSetId++;
				nCmdperSet = 0;
			}
			else
			{
				gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].index = stoi(row[iDlpCmdIndex]);

				strcpy_s(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strCmdName, row[iDlpCmdName].c_str());
				strcpy_s(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strCmdType, row[iOpType].c_str());
				strcpy_s(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strDestiNation, row[iDestination].c_str());
				strcpy_s(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strRegAddress, row[iAddress].c_str());
				strcpy_s(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strRegData, row[iRegVal].c_str());
				strcpy_s(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strDesc, row[iDesc].c_str());

				// Parse & Check Valid
				if (row[iOpType] == "W" || row[iOpType] == "w")
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nCmdType = iWrite;
				else if (row[iOpType] == "R" || row[iOpType] == "r")
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nCmdType = iRead;
				else if (row[iOpType] == "F" || row[iOpType] == "f")
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nCmdType = iFlash;
				else
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nCmdType = iUndef;

				if (row[iDestination] == "L" || row[iDestination] == "l")
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nDestType = iLeft;
				else if (row[iDestination] == "R" || row[iDestination] == "r")
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nDestType = iRight;
				else
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nDestType = iUndef;


				UINT32 RegVal=0;
				if(!CharToHex(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strRegAddress, 4, &RegVal))
				{ 
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nCmdType = iErrorReg;
					RegVal = 0;
				}
				gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nRegAddress = RegVal & 0xFFFF;


				if (gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nCmdType == iWrite)
				{
					RegVal = 0;
					if (!CharToHex(gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].strRegData, 8, &RegVal))
					{
						gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nCmdType = iErrorVal;
						RegVal = 0;
					}
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nRegData = RegVal & 0xFFFF;
				}
				else
				{
					gCmdTabs[nSetId].CmdsInfoTab[nCmdperSet].nRegData = 0;
				}

				nCmdperSet++;
			}
		}
	}
	in.close();
}




#if 0


int SaveCsv(string FilePathName, ParamContext* pParseContext)
{
	string FileName;
	unsigned int temp;

	FileName = FilePathName + "vf1.csv";

	// 1. First Open FIle, look ahead count
	std::ofstream out(FileName);
	if (out.fail())
	{
		cout << "File error (open)" << endl;
		return  eFileOpen;
	}
	out << "Global Table" << std::endl;

	out << "No" << ",";
	out << "Index" << ",";
	out << "SubIndex" << ",";
	out << "Type" << ",";
	out << "TypeLen" << ",";
	out << "TypeOffsetLen" << ",";
	out << "VProperty" << ",";
	out << "IProperty" << ",";
	out << "RWProperty" << ",";
	out << "SDProperty" << ",";
	out << "Offset" << ",";
	out << "PacketOffset" << ",";
	out << "Fwr" << ",";
	out << "Mwr" << ",";
	out << "Frd" << ",";
	out << "Mrd" << std::endl;

	for (int i = 0; i < pParseContext->UnitItemCnt; i++)
	{
		out << i << ",";
		out << std::hex << pParseContext->pArrayUnitItems[i].Index << ",";
//		out << std::hex << pParseContext->pArrayUnitItems[i].SubIndex << ",";
		temp = pParseContext->pArrayUnitItems[i].Type;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].TypeLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].TypeOffsetLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].VProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].RWProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].Offset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].PacketOffset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].Fwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].Mwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItems[i].Mrd;
		out << std::dec << temp << std::endl;
	}
	out << std::endl;
	out << std::endl;
	out << "Mem Rd Table" << std::endl;

	for (int i = 0; i < pParseContext->MemRdCnt; i++)
	{
		out << i << ",";
		out << std::hex << pParseContext->pArrayUnitItemsMemRd[i].Index << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].Type;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].TypeLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].TypeOffsetLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].VProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].RWProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].Offset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].PacketOffset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].Fwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].Mwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemRd[i].Mrd;
		out << std::dec << temp << std::endl;
	}


	out << std::endl;
	out << std::endl;
	out << "Mem Wr Table" << std::endl;

	for (int i = 0; i < pParseContext->MemWrCnt; i++)
	{
		out << i << ",";
		out << std::hex << pParseContext->pArrayUnitItemsMemWr[i].Index << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].Type;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].TypeLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].TypeOffsetLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].VProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].RWProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].Offset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].PacketOffset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].Fwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].Mwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsMemWr[i].Mrd;
		out << std::dec << temp << std::endl;
	}

	out << std::endl;
	out << std::endl;
	out << "Fls Wr Table" << std::endl;
	for (int i = 0; i < pParseContext->FLSWrCnt; i++)
	{
		out << i << ",";
		out << std::hex << pParseContext->pArrayUnitItemsFlsWr[i].Index << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].Type;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].TypeLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].TypeOffsetLen;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].VProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].RWProperty;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].Offset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].PacketOffset;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].Fwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].Mwr;
		out << std::dec << temp << ",";
		temp = pParseContext->pArrayUnitItemsFlsWr[i].Mrd;
		out << std::dec << temp << std::endl;
	}

	out.close();


	return ParseSuccess;
}

#if 0
//Debug
int main()
{
	int i = LoadCsv(FilePathName10, pTestContext, 0, DataLable);
	cout << i << endl;
}
#endif
#endif