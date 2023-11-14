#include "pch.h"
#include "DLP_ctrl.h"
#include "FirmwareUpdateForm.h"


void SetDropDownHeight(CComboBox* p_combo, int itemcnt)
{
	CRect rectComboBox, rectDropDown;
	p_combo->GetClientRect(&rectComboBox);
	p_combo->GetDroppedControlRect(&rectDropDown);
	p_combo->GetParent()->ScreenToClient(&rectDropDown);
	rectDropDown.bottom = rectDropDown.top + rectDropDown.Height() + 10 * itemcnt;
	p_combo->MoveWindow(&rectDropDown);
}


void UpdateLines(CEdit* pedHandle, CString str)
{
	static int LineCount = 0;


	if (LineCount > 64)
	{
		LineCount = 0;
		CString Dummy;
		Dummy = "";
		pedHandle->SetWindowTextW(Dummy);
	}
	LineCount++;
	pedHandle->ReplaceSel(str);
	str = "";
}

void TreeExpandAll(CTreeCtrl* pTree)
{
	HTREEITEM hItem;

	hItem = pTree->GetFirstVisibleItem();
	while (hItem != NULL)
	{
		pTree->Expand(hItem, TVE_EXPAND);
		hItem = pTree->GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}

HTREEITEM AddTreeItem(CString strItem, HTREEITEM tParent, CTreeCtrl* pTree)
{
	HTREEITEM hItem;

	hItem = pTree->InsertItem(strItem, 0, 0, tParent, TVI_LAST);

	return hItem;
}

void AddHtbInfoTree(CTreeCtrl* pTree, HTBInfo* pHTBInfo, bool IsHtbOk)
{
	CString ItemName;
	HTREEITEM hItem, hChild;

	// Tier 1
	{
		ItemName.Format(_T("HTB_Header Info"));
		hItem = AddTreeItem(ItemName, TVI_ROOT, pTree);

		if (IsHtbOk)
		{
			// Tier 2
			{
				ItemName.Format(_T("ProgramCode"));
				hChild = AddTreeItem(ItemName, hItem, pTree);
				// Tier 3
				{
					ItemName = pHTBInfo->cPrgCode;
					AddTreeItem(ItemName, hChild, pTree);
				}

			}
			// Tier 2
			{
				ItemName.Format(_T("VersionCode"));
				hChild = AddTreeItem(ItemName, hItem, pTree);
				// Tier 3
				{
					ItemName = pHTBInfo->cVerCode;
					AddTreeItem(ItemName, hChild, pTree);
				}
			}

			// Tier 2
			{
				ItemName.Format(_T("Bin Size"));
				hChild = AddTreeItem(ItemName, hItem, pTree);
				// Tier 3
				{
					ItemName = pHTBInfo->cSizeStr;
					AddTreeItem(ItemName, hChild, pTree);
				}
			}

			// Tier 2
			{
				ItemName.Format(_T("File Size"));
				hChild = AddTreeItem(ItemName, hItem, pTree);
				// Tier 3
				{
					ItemName.Format(_T("%x"), pHTBInfo->dwFileSize);
					AddTreeItem(ItemName, hChild, pTree);
				}
			}


			// Tier 2
			{
				ItemName.Format(_T("File CRC"));
				hChild = AddTreeItem(ItemName, hItem, pTree);
				// Tier 3
				{
					ItemName.Format(_T("%02x"), pHTBInfo->CRC_FILE);
					AddTreeItem(ItemName, hChild, pTree);
				}
			}

		}
		else
		{
			// Tier 2
			ItemName.Format(_T("NULL"));
			hChild = AddTreeItem(ItemName, hItem, pTree);
		}
	}

}

void AddBinInfoTree(CTreeCtrl* pTree, BinInfo* pBinInfo)
{
	//	HTREEITEM tiBin, tiEntry;

	CString ItemName;
	HTREEITEM hItem, hChild;


	// Tier1
	ItemName.Format(_T("BIN_Header Info"));
	hItem = hItem = AddTreeItem(ItemName, TVI_ROOT, pTree);
	{
		// Tier2
		ItemName.Format(_T("MagicWord"));
		hChild = AddTreeItem(ItemName, hItem, pTree);
		// Tier 3
		{
			ItemName.Format(_T("%02x"), pBinInfo->MagicId);
			AddTreeItem(ItemName, hChild, pTree);
		}

		// Tier2
		ItemName.Format(_T("EntryAddr"));
		hChild = AddTreeItem(ItemName, hItem, pTree);
		// Tier 3
		{
			ItemName.Format(_T("%04x"), pBinInfo->EntryPoint);
			AddTreeItem(ItemName, hChild, pTree);
		}

		// Tier2
		ItemName.Format(_T("NumSector"));
		hChild = AddTreeItem(ItemName, hItem, pTree);
		// Tier 3
		{
			ItemName.Format(_T("%d"), pBinInfo->NumSections);
			AddTreeItem(ItemName, hChild, pTree);
		}
	}
	for (int i = 0; i < pBinInfo->NumSections; i++)
	{
		// Tier2
		ItemName.Format(_T("Section %d"), i);
		hChild = AddTreeItem(ItemName, hItem, pTree);
		// Tier 3
		{
			ItemName.Format(_T("%02x"), pBinInfo->pSections[i].SizeSector);
			AddTreeItem(ItemName, hChild, pTree);

			ItemName.Format(_T("%04x"), pBinInfo->pSections[i].EntryPoint);
			AddTreeItem(ItemName, hChild, pTree);

			ItemName.Format(_T("%04x"), pBinInfo->pSections[i].BufOffset);
			AddTreeItem(ItemName, hChild, pTree);
		}
	}
	{
		// Tier2
		ItemName.Format(_T("EndOffset"));
		hChild = AddTreeItem(ItemName, hItem, pTree);
		// Tier 3
		{
			ItemName.Format(_T("%04x"), pBinInfo->EndPoint);
			AddTreeItem(ItemName, hChild, pTree);
		}
	}
}