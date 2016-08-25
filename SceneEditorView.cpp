
// SceneEditorView.cpp : CSceneEditorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "SceneEditor.h"
#endif

#include "SceneEditorDoc.h"
#include "SceneEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MainFrm.h"
#include "Ogre.h"

using namespace Ogre;
// CSceneEditorView

IMPLEMENT_DYNCREATE(CSceneEditorView, CView)

BEGIN_MESSAGE_MAP(CSceneEditorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSceneEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER() 
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CSceneEditorView ����/����

CSceneEditorView::CSceneEditorView()
{
	isFistrun=true;

}

CSceneEditorView::~CSceneEditorView()
{
}

BOOL CSceneEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSceneEditorView ����

void CSceneEditorView::OnDraw(CDC* /*pDC*/)
{
	CSceneEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	
	if (isFistrun)
	{
		isFistrun=false;

		CRect   rect;  
		GetClientRect(&rect);  
		appMgr=BaseApplication::getInstance();
		appMgr->createWindow(m_hWnd,rect.Width(),rect.Height());
		appMgr->go();

		CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
		CRect rc;
		pMain->m_wndOutput.GetClientRect(rc);
		NameValuePairList parms2;
		parms2["externalWindowHandle"] = StringConverter::toString((long)pMain->m_wndOutput.m_hWnd);
		pMain->m_wndOutput.mRenderwidow2 = appMgr->mRoot->createRenderWindow("mini Window", rc.Width(), rc.Height(), false, &parms2);

		pMain->m_wndOutput.mCamera2 = appMgr->mSceneMgr->createCamera("Cam2");
		pMain->m_wndOutput.mCamera2->setPosition(0,200,0);
		pMain->m_wndOutput.mCamera2->pitch(Degree(-90));
		pMain->m_wndOutput.mCamera2->setNearClipDistance(0.1);
		pMain->m_wndOutput.mCamera2->setFarClipDistance(10000);
		pMain->m_wndOutput.mCamera2->setAspectRatio(Ogre::Real(rc.Width()) / Ogre::Real(rc.Height()));
		appMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode("Cam2Node")->attachObject(pMain->m_wndOutput.mCamera2);
		pMain->m_wndOutput.mViewport2 = pMain->m_wndOutput.mRenderwidow2->addViewport(pMain->m_wndOutput.mCamera2);
		pMain->m_wndOutput.mViewport2->setBackgroundColour(ColourValue(0,0,0.1));

		pMain->m_wndFileView.FillFileView();   

		pMain->m_wndToolBarViewEdit.AdjustLayout();
		pMain->m_wndSceneSetProperties.InitUnitSetProp();
		pMain->m_wndProperties.m_wndEditTerrain->mscene = BaseApplication::getInstance()->mscene;

	    SetTimer(1,20,NULL);
		//DWORD dwThreadID = 0;
		//HANDLE hThread=CreateThread(NULL,0,RenderThread,this,NULL,&dwThreadID);//������Ⱦ�߳�////
		
	}

}


// CSceneEditorView ��ӡ


void CSceneEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSceneEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSceneEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSceneEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CSceneEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSceneEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{

	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);

}


void CSceneEditorView::OnTimer( UINT nIDEvent )
{
	if (nIDEvent == 1)
	{
		appMgr->mRoot->renderOneFrame();
	}
	
}

DWORD WINAPI RenderThread( void *pArg )
{
	CSceneEditorView* pView=(CSceneEditorView*)pArg;
	pView->appMgr->mRoot->startRendering();
	return 0;
}

void CSceneEditorView::OnSize( UINT nType, int cx, int cy )
{
	CRect rect; 
	GetClientRect(&rect);
	if(appMgr!=NULL)
	{
		if (!isFistrun && rect.Width() != 0 && rect.Height() != 0&&appMgr->mCamera!=NULL) 
		{
			appMgr->mCamera->setAspectRatio((Ogre::Real)rect.Width() / (Ogre::Real)rect.Height()); 
		}
	}

}



// CSceneEditorView ���

#ifdef _DEBUG
void CSceneEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CSceneEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSceneEditorDoc* CSceneEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSceneEditorDoc)));
	return (CSceneEditorDoc*)m_pDocument;
}

#endif //_DEBUG


// CSceneEditorView ��Ϣ�������

