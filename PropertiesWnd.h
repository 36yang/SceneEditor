#pragma once

#include "EditTerrainWnd.h"


class CPropertiesWnd : public CDockablePane
{
// ππ‘Ï
public:
	CPropertiesWnd();
	void AdjustLayout();

public:
	EditTerainWnd *m_wndEditTerrain;

	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()
};

