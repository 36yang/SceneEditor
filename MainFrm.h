
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "SceneObjView.h"
#include "OutputWnd.h"
#include "ObjPropertiesWnd.h"
#include "PropertiesWnd.h"
#include "EditTerrainWnd.h"
#include "SceneSetPropertiesWnd.h"


class CMainFrame : public CFrameWndEx
{
	
public: 
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBar       m_wndToolBarEditObj;
	CMFCToolBar       m_wndToolBarViewEdit;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	SceneObjView	  m_wndSceneObjView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	ObjPropertiesWnd  m_wndObjProperties;
	SceneSetPropertiesWnd m_wndSceneSetProperties;
	EditTerainWnd     *m_wndEditTerrain;

	bool showAllEntity;     //显示所有的实体//
	bool showAllCollide;    //显示所有的碰撞体//
	bool showAllSound;      //显示所有的声音//
	bool showAllLight;      //显示所有的灯光//
	bool showAllParticle;   //显示所有的粒子//

	void calcuateLightMap();
	void updateTerrainPhy();

protected:
	CMainFrame();
	virtual ~CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	
	BOOL CreateDockingWindows();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void AddResPath(CString &resPath);
	void selectObj();
	void moveSelectObj();
	void scaleSelectObj();
	void rotateSelectObj();
	void copySelectObj();
	void deleteSelectObj();

	void openPhysics();
	void closePhysics();
	void changePlayMode();
	void changeEditMode();
	void selectBoxMode();
	void selectMeshMode();
	void updateSelectMeshMode(CCmdUI *pCmdUI);
	void updateSelectBoxMode(CCmdUI *pCmdUI);

	void updateToolbarUI(CCmdUI *pCmdUI);
	void showEditToolBar();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void AddResPath();
	afx_msg void SaveScene();
	afx_msg void OpenScene();
	afx_msg void findSceneObj();
	afx_msg void ResetHeroPostion();
	afx_msg void OnUpdateSubMenueShow(UINT id);
	afx_msg void OnUpdateSubMenueShowLook(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewEditBar(CCmdUI *pCmdUI);
	afx_msg void OnViewFileShow();
	afx_msg void OnUpdateViewFileShow(CCmdUI *pCmdUI);
	afx_msg void OnViewSceneShow();
	afx_msg void OnUpdateViewSceneShow(CCmdUI *pCmdUI);
	afx_msg void OnViewTerrainShow();
	afx_msg void OnUpdateViewTerrainShow(CCmdUI *pCmdUI);
	afx_msg void OnViewProShow();
	afx_msg void OnUpdateViewProShow(CCmdUI *pCmdUI);
	afx_msg void OnViewMinimapShow();
	afx_msg void OnUpdateViewMinimapShow(CCmdUI *pCmdUI);
	afx_msg void OnViewSceneSetShow();
	afx_msg void OnUpdateViewSceneSetShow(CCmdUI *pCmdUI);
	afx_msg void OnViewSetBarShow();
	afx_msg void OnUpdateViewSetBarShow(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
};


