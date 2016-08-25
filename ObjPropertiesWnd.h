#pragma once
#include "EditScene.h"
#include "SceneObj.h"

class ObjPropertiesWnd : public CDockablePane
{
public:
	ObjPropertiesWnd();
	virtual ~ObjPropertiesWnd();


	CMFCPropertyGridCtrl m_wndPropList;
	EditScene* mscene;
	SceneObj* tempObj;
	void AdjustLayout();
	void OnTimer( UINT nIDEvent );
	void InitPropList(SceneObj* obj);
	void InitMeshObjProp();
	void InitSoundObjProp();
	void InitPhysicsBoxProp();
	void InitLightObjProp();
	void InitParticleObjProp();
	void InitSkyProp(int type);
	void InitFogProp(int type);
	void OnSkyandFogPropertyChanged(CMFCPropertyGridProperty* pProp); 

public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam,LPARAM lParam); 

	DECLARE_MESSAGE_MAP()
};

