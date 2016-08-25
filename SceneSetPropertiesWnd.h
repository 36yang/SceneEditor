#pragma once


class SceneSetPropertiesWnd : public CDockablePane
{
public:
	SceneSetPropertiesWnd();
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	void AdjustLayout();
	void ComBoxSelectChanged();


protected:
	CComboBox m_wndObjectCombo;
	CMFCPropertyGridCtrl m_wndPropList;
	int currentSet;  //0单位设置, 1浏览设置, 2物理引擎//

public:
	virtual ~SceneSetPropertiesWnd();
	void InitPropList();
	void InitPhsicsSetProp();
	void InitUnitSetProp();
	void InitViewSetPeop();


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam,LPARAM lParam); 

	DECLARE_MESSAGE_MAP()
};

