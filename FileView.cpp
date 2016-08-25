
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "SceneEditor.h"
#include "BaseApplication.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_NOTIFY(TVN_SELCHANGED, 4, OnSelChanged)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_SHOWSELALWAYS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	tempGroup=0;

	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	hRoot = m_wndFileView.InsertItem(_T("资源文件"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	hMesh = m_wndFileView.InsertItem(_T("模型"), 0, 0, hRoot);
	hMeshSystem=m_wndFileView.InsertItem(_T("系统"), 0, 0, hMesh);
	hPhysicsBox=m_wndFileView.InsertItem(_T("碰撞体"), 0, 0, hRoot);
	hLight=m_wndFileView.InsertItem(_T("光源"),0,0,hRoot);
	hSound = m_wndFileView.InsertItem(_T("声音"), 0, 0, hRoot);
	hSoundSystem=m_wndFileView.InsertItem(_T("系统"), 0, 0, hSound);
	hParticle = m_wndFileView.InsertItem(_T("粒子"), 0, 0, hRoot);
	hParticleSystem=m_wndFileView.InsertItem(_T("系统"), 0, 0, hParticle);

	m_wndFileView.InsertItem(_T("立方体.Phy"), 5, 5, hPhysicsBox);
	m_wndFileView.InsertItem(_T("球体.Phy"), 5, 5, hPhysicsBox);
	m_wndFileView.InsertItem(_T("圆柱体.Phy"), 5, 5, hPhysicsBox);
	m_wndFileView.InsertItem(_T("胶囊.Phy"), 5, 5, hPhysicsBox);
	m_wndFileView.InsertItem(_T("平面.Phy"), 5, 5, hPhysicsBox);

	m_wndFileView.InsertItem(_T("点光源.Light"), 7, 7, hLight);
	m_wndFileView.InsertItem(_T("聚光灯.Light"), 7, 7, hLight);
	m_wndFileView.InsertItem(_T("平行光.Light"), 7, 7, hLight);


	std::list<CString> listFile;
	TraverseFolder(CString(_T("../media/sound")), listFile);
	TraverseFolder(CString(_T("../media/models")), listFile);

	CString temp;
	for (std::list<CString>::iterator i=listFile.begin(); i!=listFile.end(); ++i)
	{
		temp=*i;
		int nPos = temp.Find('.');  
		CString fileextion;
		fileextion = temp.Right(temp.GetLength()-nPos-1);
		if (fileextion=="ogg"||fileextion=="mid"||fileextion=="mp3")
		{
			m_wndFileView.InsertItem(temp, 6, 6, hSoundSystem);
		}

		if (fileextion=="mesh")
		{
			m_wndFileView.InsertItem(temp, 3, 3, hMeshSystem);
		}

	}
	ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	if (pManager!=NULL)
	{
		ParticleUniverse::vector<ParticleUniverse::String> ParticleNameList;
		pManager->particleSystemTemplateNames(ParticleNameList);
		ParticleUniverse::vector<ParticleUniverse::String>::iterator i=ParticleNameList.begin();
		while(i!=ParticleNameList.end())
		{
			Ogre::String particleName=*i;
			particleName.append(".particle");
			m_wndFileView.InsertItem(CString(particleName.c_str()), 4, 4,hParticle);
			i++;
		}
	}
	
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hMeshSystem, TVE_EXPAND);
	m_wndFileView.Expand(hSoundSystem, TVE_EXPAND);
	m_wndFileView.Expand(hParticleSystem,TVE_EXPAND);

}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
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
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("属性...."));

}

void CFileView::OnFileOpen()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnFileOpenWith()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnDummyCompile()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditCut()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditCopy()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditClear()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;
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
	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::TraverseFolder( const CString& strDir,std::list<CString>& listFile )
{
	WIN32_FIND_DATA FindFileData;

	CString strDirTmp;
	strDirTmp = strDir;
	strDirTmp += "/*.*";

	HANDLE hFind=::FindFirstFile(strDirTmp,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}

	while(true)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			/*if(FindFileData.cFileName[0]!= _T('.'))
			{
			strDirTmp = strDir;
			strDirTmp += FindFileData.cFileName;
			strDirTmp+="\\";
			TraverseFolder(strDirTmp,listFile) ;

			}*/
		}
		else
		{
			/*strDirTmp = strDir;
			strDirTmp += FindFileData.cFileName;
			listFile.push_back(strDirTmp);*/
			listFile.push_back(FindFileData.cFileName);
		}
		if(!FindNextFile(hFind,&FindFileData))    break;
	}

	FindClose(hFind);

}

void CFileView::OnSelChanged( NMHDR* pNMHDR, LRESULT* pResult )
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);   
	*pResult = 0;   
	CString strText;
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();   
	strText = m_wndFileView.GetItemText(hItem);   
	BaseApplication *appMgr;
	appMgr=BaseApplication::getInstance();
	if (appMgr->mscene!=NULL)
	{
		USES_CONVERSION;
		std::string str1 = W2A(strText.GetBuffer());
		Ogre::String resName(str1);
		appMgr->mscene->createSceneObj(resName);
	}

	//MessageBox(strText);
}


