
#pragma once

#include "ViewTree.h"
#include "SceneObj.h"

class CSceneObjToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class SceneObjView : public CDockablePane
{
public:
	SceneObjView();
	virtual ~SceneObjView();

	CSceneObjToolBar m_wndToolBar;
	CViewTree m_wndSceneObjTree;
	CImageList mSceneObjImages;
	UINT m_nCurrSort;
	HTREEITEM hRoot;
	HTREEITEM hEnvironment;
	HTREEITEM hMesh;
	HTREEITEM hSurroundBox;
	HTREEITEM hSound;
	HTREEITEM hLight;
	HTREEITEM hParticle;



	void AdjustLayout();
	void OnChangeVisualStyle();
	void FillSceneTree();
	void moveCameraTo(SceneObj* Obj);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCustomDrawMyTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()
};

