
#pragma once

#include "ViewTree.h"
#include <vector>
#include <list>

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 构造
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void TraverseFolder( const CString& strDir,std::list<CString>& listFile );
	void FillFileView();
	virtual ~CFileView();


// 特性
public:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;
	std::vector<CString> mResPaths;
	int tempGroup;
	HTREEITEM hRoot;
	HTREEITEM hMesh;
	HTREEITEM hLight;
	HTREEITEM hMeshSystem;
	HTREEITEM hSound;
	HTREEITEM hSoundSystem;
	HTREEITEM hParticle;
	HTREEITEM hParticleSystem;
	HTREEITEM hPhysicsBox;



protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

