extern void SetDropDownHeight(CComboBox* p_combo, int itemcnt);
extern void UpdateLines(CEdit* pedHandle, CString str);
extern void TreeExpandAll(CTreeCtrl* pTree);
extern HTREEITEM AddTreeItem(CString strItem, HTREEITEM tParent, CTreeCtrl* pTree);
extern void AddHtbInfoTree(CTreeCtrl* pTree, HTBInfo* pHTBInfo, bool IsHtbOk);
extern void AddBinInfoTree(CTreeCtrl* pTree, BinInfo* pBinInfo);
