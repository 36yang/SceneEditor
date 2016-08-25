
// SceneEditorView.h : CSceneEditorView 类的接口
//

#pragma once
#include "BaseApplication.h"

class CSceneEditorView : public CView
{
protected: // 仅从序列化创建
	CSceneEditorView();
	DECLARE_DYNCREATE(CSceneEditorView)

// 特性
public:
	CSceneEditorDoc* GetDocument() const;
	BaseApplication *appMgr;
	bool isFistrun;
	friend DWORD WINAPI  RenderThread(void *pArg); 

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CSceneEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SceneEditorView.cpp 中的调试版本
inline CSceneEditorDoc* CSceneEditorView::GetDocument() const
   { return reinterpret_cast<CSceneEditorDoc*>(m_pDocument); }
#endif

