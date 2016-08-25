
#include "stdafx.h"
#include "MainFrm.h"
#include "SceneObjView.h"
#include "Resource.h"
#include "SceneEditor.h"
#include "BaseApplication.h"
#include "InputManager.h"


SceneObjView::SceneObjView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

SceneObjView::~SceneObjView()
{
}

BEGIN_MESSAGE_MAP(SceneObjView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_NOTIFY(TVN_SELCHANGED, 2, OnSelChanged)
	ON_NOTIFY(NM_CUSTOMDRAW,2, OnCustomDrawMyTree)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int SceneObjView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN|TVS_SHOWSELALWAYS ;

	if (!m_wndSceneObjTree.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("未能创建类视图\n");
		return -1;  
	}
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);
	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	FillSceneTree();

	return 0;
}

void SceneObjView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void SceneObjView::FillSceneTree()
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	m_wndSceneObjTree.DeleteAllItems(); 

	hRoot = m_wndSceneObjTree.InsertItem(_T("场景"));
	m_wndSceneObjTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	
	hEnvironment = m_wndSceneObjTree.InsertItem(_T("环境"), 0, 0, hRoot);
	m_wndSceneObjTree.InsertItem(_T("天空"), 1, 1, hEnvironment);
	m_wndSceneObjTree.InsertItem(_T("雾"), 2, 2, hEnvironment);
	m_wndSceneObjTree.Expand(hRoot, TVE_EXPAND);
	hMesh = m_wndSceneObjTree.InsertItem(_T("模型"), 0, 0, hRoot);
	hSurroundBox = m_wndSceneObjTree.InsertItem(_T("碰撞体"), 0, 0, hRoot);
	hLight = m_wndSceneObjTree.InsertItem(_T("光源"), 0, 0, hRoot);
	hSound = m_wndSceneObjTree.InsertItem(_T("声音"), 0, 0, hRoot);
	hParticle = m_wndSceneObjTree.InsertItem(_T("粒子"), 0, 0, hRoot);

	if (appMgr->mscene!=NULL)
	{
		for (std::list<SceneMeshObj*>::iterator i=appMgr->mscene->mMeshObjList.begin();i!=appMgr->mscene->mMeshObjList.end();++i)
		{
			m_wndSceneObjTree.InsertItem(CString((*i)->mUserName.c_str()),3,3,hMesh);
		}
		for (std::list<SceneSoundObj*>::iterator i=appMgr->mscene->mSoundList.begin();i!=appMgr->mscene->mSoundList.end();++i)
		{
			m_wndSceneObjTree.InsertItem(CString((*i)->mUserName.c_str()),6,6,hSound);
		}
		for (std::list<PhysicsBox*>::iterator i=appMgr->mscene->mPhysisBoxList.begin();i!=appMgr->mscene->mPhysisBoxList.end();++i)
		{
			m_wndSceneObjTree.InsertItem(CString((*i)->mUserName.c_str()),5,5,hSurroundBox);
		}
		for (std::list<SceneLightObj*>::iterator i=appMgr->mscene->mLightObjList.begin();i!=appMgr->mscene->mLightObjList.end();++i)
		{
			m_wndSceneObjTree.InsertItem(CString((*i)->mUserName.c_str()),7,7,hLight);
		}
		for (std::list<SceneParticleObj*>::iterator i=appMgr->mscene->mParticleList.begin();i!=appMgr->mscene->mParticleList.end();++i)
		{
			m_wndSceneObjTree.InsertItem(CString((*i)->mUserName.c_str()),4,4,hParticle);
		}

	}
	
	m_wndSceneObjTree.Expand(hEnvironment,TVE_EXPAND);
	m_wndSceneObjTree.Expand(hMesh,TVE_EXPAND);
	m_wndSceneObjTree.Expand(hParticle,TVE_EXPAND);
	m_wndSceneObjTree.Expand(hSound,TVE_EXPAND);
	m_wndSceneObjTree.Expand(hSurroundBox,TVE_EXPAND);
}

void SceneObjView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndSceneObjTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void SceneObjView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndSceneObjTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL SceneObjView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}





void SceneObjView::OnNewFolder()
{
	AfxMessageBox(_T("新建文件夹..."));
}

void SceneObjView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndSceneObjTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void SceneObjView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndSceneObjTree.SetFocus();
}

void SceneObjView::OnChangeVisualStyle()
{
	mSceneObjImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	mSceneObjImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	mSceneObjImages.Add(&bmp, RGB(255, 0, 0));

	m_wndSceneObjTree.SetImageList(&mSceneObjImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}


////when TreeControl lost Focus make the select item still light show/////
void SceneObjView::OnCustomDrawMyTree( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
	LPNMTVCUSTOMDRAW pDraw=(LPNMTVCUSTOMDRAW)pNMHDR;
	DWORD dwDrawStage=pDraw->nmcd.dwDrawStage;
	UINT uItemState =pDraw->nmcd.uItemState;
	*pResult=CDRF_NOTIFYITEMDRAW;  //|CDRF_NOTIFYPOSTPAINT|CDRF_NOTIFYSUBITEMDRAW|CDRF_NOTIFYPOSTERASE;
	CDC* pdc=CDC::FromHandle(pDraw->nmcd.hdc);
	CRect rc;
	HTREEITEM hItem=(HTREEITEM) pDraw->nmcd.dwItemSpec;
	m_wndSceneObjTree.GetItemRect(hItem,&rc,TRUE);//FALSE);text only
	CString txt=m_wndSceneObjTree.GetItemText(hItem);
	*pResult |=CDRF_DODEFAULT;  //draw bitmap and grid
	if((dwDrawStage & CDDS_ITEM) && (uItemState & CDIS_SELECTED))
	{
		pdc->FillSolidRect(&rc,RGB(255,165,197));//clr
		pdc->SetTextColor(RGB(255,255,255));//white
		pdc->SetBkColor(RGB(255,165,197));//clr

		CFont* pfnt=pdc->GetCurrentFont();
		pdc->TextOut(rc.left+2,rc.top+2,txt);
		pdc->SelectObject(pfnt);
	}
	else //without these ,1st blue !
	{
		pdc->FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));
		pdc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pdc->TextOut(rc.left+2, rc.top+2, txt);
	}
}

void SceneObjView::OnSelChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);   
	BaseApplication *appMgr=BaseApplication::getInstance();
	InputManager *inputMgr=InputManager::getInstance();
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	*pResult = 0;   
	CString strText;
	HTREEITEM hItem = m_wndSceneObjTree.GetSelectedItem();   
	strText = m_wndSceneObjTree.GetItemText(hItem);   
	if (CString(_T("天空"))==strText)
	{
		pMain->m_wndObjProperties.InitSkyProp(appMgr->mscene->mySky.type);
		pMain->m_wndObjProperties.tempObj=NULL;
	}
	else if (CString(_T("雾"))==strText)
	{
		pMain->m_wndObjProperties.InitFogProp(appMgr->mscene->myFog.type);
		pMain->m_wndObjProperties.tempObj=NULL;
	}
	

	if (NULL!=appMgr->mscene && NULL!=inputMgr)
	{
		if (inputMgr->viewMode==1 )
		{
			USES_CONVERSION;
			std::string str1 = W2A(strText.GetBuffer());
			Ogre::String name(str1);
			SceneObj *Obj=NULL;
			Ogre::String subName=name.substr(0,4);
			if (subName =="Mesh"){Obj=appMgr->mscene->findSceneObjByUserName(name,0);}
			else if (subName =="Soun"){Obj=appMgr->mscene->findSceneObjByUserName(name,1);}
			else if (subName =="Phys"){Obj=appMgr->mscene->findSceneObjByUserName(name,2);}
			else if (subName =="Ligh"){Obj=appMgr->mscene->findSceneObjByUserName(name,3);}
			else if (subName=="Part"){ Obj=appMgr->mscene->findSceneObjByUserName(name,4);}
			if(NULL!=Obj)
			{
				bool bControlDown = inputMgr->PressedKey&0x0100;
				if (!bControlDown) //control is up//
				{
					for(std::vector<SceneObj*>::iterator i=appMgr->mscene->selectSceneObj.begin();i!=appMgr->mscene->selectSceneObj.end();i++)
					{
						(*i)->showBoundingBox(false);
					}					
					appMgr->mscene->selectSceneObj.clear();

				}			
				Obj->showBoundingBox(true);
				appMgr->mscene->selectSceneObj.push_back(Obj);
				if (m_wndSceneObjTree.GetSafeHwnd()==::GetFocus())  //在场景点时不自动对焦//
				{
					moveCameraTo(Obj);
				}			
				pMain->m_wndObjProperties.InitPropList(Obj);
			}
		}//viewMode==1
	}//mscene!=null &&inputMgr!=NULL 
}

void SceneObjView::moveCameraTo(SceneObj* Obj)
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	Ogre::Vector3 halfsize = Obj->mEntity->getWorldBoundingBox(true).getHalfSize();
	float fRadius = (halfsize.x + halfsize.y +halfsize.z)/3;
	//float fRadius=Obj->mEntity->getWorldBoundingSphere().getRadius();
	float camToObject = (fRadius * 4 + appMgr->mscene->mCamera->getNearClipDistance());        //相机到物体的距离///
	Vector3 camEnd = Obj->mPostion-camToObject *appMgr->mscene->mCamera->getDerivedDirection();

	//匀速移动相机到物体//
	const Ogre::Vector3& camStart = appMgr->mscene->mCamera->getParentSceneNode()->getPosition();
	Ogre::Timer* pTimer = Ogre::Root::getSingletonPtr()->getTimer();
	unsigned int motionTime = 500;
	unsigned long timeStart = pTimer->getMilliseconds();
	unsigned long timeNow;
	Ogre::Vector3 vTemp;
	do
	{
		timeNow = pTimer->getMilliseconds();
		vTemp = (float)__min( motionTime, ( timeNow - timeStart ) ) / (float)motionTime
			* ( camEnd - camStart ) + camStart;
		 appMgr->mscene->mCamera->getParentSceneNode()->setPosition(vTemp);
		 appMgr->mWindow->update();
		Sleep( 10 );
	} while( timeNow - timeStart <= motionTime);
}
