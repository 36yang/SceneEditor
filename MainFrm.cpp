
// MainFrm.cpp : CMainFrame ���ʵ��//

#include "stdafx.h"
#include "SceneEditor.h"
#include "MainFrm.h"
#include "BaseApplication.h"
#include "InputManager.h"
#include "ParticleUniverseSystemManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND_RANGE(32843,32858,&CMainFrame::OnUpdateSubMenueShow)
	ON_UPDATE_COMMAND_UI_RANGE(32843, 32858, &CMainFrame::OnUpdateSubMenueShowLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_ADD_RES, &CMainFrame::AddResPath)
	ON_COMMAND(ID_SCENE_SAVE, &CMainFrame::SaveScene)
	ON_COMMAND(ID_SCENE_OPEN, &CMainFrame::OpenScene)
	ON_COMMAND(ID_SELECT_OBJ, &CMainFrame::selectObj)
	ON_COMMAND(ID_MOVE_OBJ, &CMainFrame::moveSelectObj)
	ON_COMMAND(ID_ROTATE_OBJ, &CMainFrame::rotateSelectObj)
	ON_COMMAND(ID_SCALE_OBJ, &CMainFrame::scaleSelectObj)
	ON_COMMAND(ID_COPY_OBJ, &CMainFrame::copySelectObj)
	ON_COMMAND(ID_DELETE_OBJ, &CMainFrame::deleteSelectObj)
	ON_COMMAND(ID_FIND_OBJ, &CMainFrame::findSceneObj)
	ON_COMMAND(ID_PHX_ON, &CMainFrame::openPhysics)
	ON_COMMAND(ID_PHX_OFF, &CMainFrame::closePhysics)
	ON_COMMAND(ID_SELECT_BOX, &CMainFrame::selectBoxMode)
	ON_COMMAND(ID_Reset_Hero, &CMainFrame::ResetHeroPostion)
	ON_COMMAND(ID_Light_Map, &CMainFrame::calcuateLightMap)
	ON_COMMAND(ID_Update_TerrainPhy, &CMainFrame::updateTerrainPhy)
	ON_UPDATE_COMMAND_UI(ID_SELECT_BOX, &CMainFrame::updateSelectBoxMode)
	ON_COMMAND(ID_SELECT_MESH, &CMainFrame::selectMeshMode)
	ON_UPDATE_COMMAND_UI(ID_SELECT_MESH, &CMainFrame::updateSelectMeshMode)
	ON_COMMAND(ID_EDIT_MODE, &CMainFrame::changePlayMode)
	ON_COMMAND(ID_PLAY_MODE, &CMainFrame::changePlayMode)
	ON_COMMAND(ID_VIEW_EDIT_BAR,&CMainFrame::showEditToolBar)
	ON_UPDATE_COMMAND_UI_RANGE(32788,32795,&CMainFrame::updateToolbarUI)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDIT_BAR, &CMainFrame::OnUpdateViewEditBar)
	ON_COMMAND(ID_VIEW_FILE_SHOW, &CMainFrame::OnViewFileShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILE_SHOW, &CMainFrame::OnUpdateViewFileShow)
	ON_COMMAND(ID_VIEW_SCENE_SHOW, &CMainFrame::OnViewSceneShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCENE_SHOW, &CMainFrame::OnUpdateViewSceneShow)
	ON_COMMAND(ID_VIEW_TERRAIN_SHOW, &CMainFrame::OnViewTerrainShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TERRAIN_SHOW, &CMainFrame::OnUpdateViewTerrainShow)
	ON_COMMAND(ID_VIEW_PRO_SHOW, &CMainFrame::OnViewProShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PRO_SHOW, &CMainFrame::OnUpdateViewProShow)
	ON_COMMAND(ID_VIEW_MINIMAP_SHOW, &CMainFrame::OnViewMinimapShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MINIMAP_SHOW, &CMainFrame::OnUpdateViewMinimapShow)
	ON_COMMAND(ID_VIEW_SCENESET, &CMainFrame::OnViewSceneSetShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCENESET, &CMainFrame::OnUpdateViewSceneSetShow)
	ON_COMMAND(ID_VIEW_SET_BAR, &CMainFrame::OnViewSetBarShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SET_BAR,&CMainFrame::OnUpdateViewSetBarShow)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��//
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame()
{
	showAllEntity=true;
	showAllCollide=true;    
	showAllSound=true;      
	showAllLight=true;
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	OnApplicationLook(theApp.m_nAppLook);    // ���ڳ־�ֵ�����Ӿ�����������ʽ//

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC| CBRS_TOOLTIPS|CBRS_TOOLTIPS);
	CMFCPopupMenu::SetForceMenuFocus(FALSE);   // ��ֹ�˵����ڼ���ʱ��ý���//


	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1; 
	}
	CString WndName;
    BOOL bNameValid = WndName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(WndName);
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);


//������1��������//
	m_wndToolBarEditObj.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBarEditObj.LoadToolBar(IDR_TOOLBAR_SCENEOBJ);
	WndName.LoadString(IDS_TOOLBAR_STANDARD);
	m_wndToolBarEditObj.SetWindowText(WndName);
	m_wndToolBarEditObj.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//======


//������2��������//
	m_wndToolBarViewEdit.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBarViewEdit.LoadToolBar(IDR_TOOLBAR_VIEWEDIT);
	WndName.LoadString(IDS_TOOLBAR_STANDARD);
	m_wndToolBarViewEdit.SetWindowText(WndName);
	m_wndToolBarViewEdit.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//======
	
	m_wndToolBarViewEdit.GetButton(9)->SetVisible(false);
	m_wndToolBarViewEdit.GetButton(12)->SetVisible(false);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// ��������ͣ������//
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	//�����ϣ���������Ͳ˵�����ͣ��������ɾ�������⼸��//
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	m_wndToolBarEditObj.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBarEditObj);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
	DockPaneLeftOf(&m_wndToolBar,&m_wndToolBarEditObj);
	m_wndToolBarViewEdit.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBarViewEdit);
	DockPaneLeftOf(&m_wndToolBarViewEdit,&m_wndToolBarEditObj);

	
	CDockingManager::SetDockingMode(DT_SMART);   //����Visual Studio 2005 ��ʽͣ��������Ϊ//
	EnableAutoHidePanes(CBRS_ALIGN_ANY);    	// ����Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ//
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSceneObjView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndSceneObjView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndFileView.ShowPane(TRUE,FALSE,TRUE);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	m_wndObjProperties.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSceneSetProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndObjProperties);
	CDockablePane* pTabbedBar2 = NULL;
	m_wndProperties.AttachToTabWnd(&m_wndObjProperties,DM_SHOW, TRUE, &pTabbedBar2);
	m_wndSceneSetProperties.AttachToTabWnd(&m_wndObjProperties,DM_SHOW, TRUE, &pTabbedBar2);
	m_wndOutput.DockToWindow(pTabbedBar2,CBRS_BOTTOM);
	
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�//
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ//

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSceneObjView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ�����������ͼ������\n");
		return FALSE;
	}

	//�����ļ���ͼ//
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE;
	}

	//�����������//
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 130, 130), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE;
	}

	//�������α༭����//
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 270, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ��������Ρ�����\n");
		return FALSE;
	}

	//���������������Դ���//
	CString strObjPropertiesWnd;
	bNameValid = strObjPropertiesWnd.LoadString(IDS_OBJPro_Wnd);
	ASSERT(bNameValid);
	if (!m_wndObjProperties.Create(strObjPropertiesWnd, this, CRect(0, 0, 270, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ��������ԡ�����\n");
		return FALSE;
	}

	//�����������ô���//
	CString strSceneSetWnd;
	bNameValid = strSceneSetWnd.LoadString(IDS_SCENE_SET);
	ASSERT(bNameValid);
	if (!m_wndSceneSetProperties.Create(strSceneSetWnd, this, CRect(0, 0, 270, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������������á�����\n");
		return FALSE;
	}

	return TRUE;
}


// CMainFrame ���//

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������//

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���//

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	//Ϊ�����û������������Զ��尴ť//
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


BOOL CMainFrame::PreTranslateMessage( MSG* pMsg )
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	InputManager *inputMgr=InputManager::getInstance();

	if (inputMgr!=NULL)
	{
		if (::GetForegroundWindow()==m_hWnd)   //��ǰ�����ǻ���ڣ�������Ӧ����¼�//
		{
			if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)  
				MessageBox(_T("�����̳���"));

			//������//
			if(pMsg->message==WM_MOUSEMOVE)
			{
				inputMgr->mouseMove(0,0,0);
			}
			if (pMsg->message==WM_LBUTTONDOWN||pMsg->message==WM_TOUCH)    //�������//
			{
				inputMgr->mousePress(1);
			}
			if (pMsg->message==WM_RBUTTONDOWN)    //�Ҽ�����//
			{
				unsigned int h,w,c;
				int l,t;
				BaseApplication::getInstance()->mWindow->getMetrics(w,h,c,l,t);
				h+=t;  w+=l;
				CPoint point;
				GetCursorPos(&point);
				if (point.x>l&&point.x<w&&point.y>t&&point.y<h)
				{
					inputMgr->mousePress(2);
				}
			}
			if (pMsg->message==WM_LBUTTONUP)    //�����̧��//
			{
				inputMgr->mouseRelease(-1);
			} 
			else if(pMsg->message==WM_RBUTTONUP)  //�Ҽ�̧��//
			{
				inputMgr->mouseRelease(-2);
			}
			if (pMsg->message==WM_MOUSEWHEEL)       //������//
			{
				float zDelta   = (short)HIWORD(pMsg->wParam);    
				inputMgr->mouseMove(0,0,zDelta);
			}
		
		} //�Ƿ�����//

		//���̰�������//
		if(pMsg->message==WM_KEYDOWN)
		{
			inputMgr->keyPressed(pMsg);
		}
		if(pMsg->message==WM_KEYUP)
		{
			inputMgr->keyReleased(pMsg);
		}
		
	}   //inputMgr!=NULL 

	return false;
}

//�����Դ·���Ի���//
class AddResDlg : public CDialogEx
{
public:
	//�Ի�������//
	enum { IDD = IDD_VIEW_RES };
	CString temptoAddPath;
	
	

	AddResDlg() : CDialogEx(AddResDlg::IDD)
	{
		temptoAddPath=_T("");
	}

	CString* addFilePath()
	{
		CString *FilePath=new CString();
		Ogre::String *filePath=NULL;
	
		BROWSEINFO bInfo;
		ZeroMemory(&bInfo, sizeof(bInfo));
		bInfo.hwndOwner =GetSafeHwnd();
		bInfo.lpszTitle = _T("��ѡ���ļ���: ");
		bInfo.ulFlags = BIF_NEWDIALOGSTYLE |BIF_EDITBOX;
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolder(&bInfo) ;
		if(lpDlist != NULL)
		{ 
			TCHAR chPath[MAX_PATH];
			SHGetPathFromIDList(lpDlist, chPath);
			*FilePath = chPath;         
		}

		return FilePath;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX)        // DDX/DDV ֧��//
	{
		CDialogEx::DoDataExchange(pDX);
	}

	virtual BOOL OnInitDialog()
	{
		CMainFrame *pMain=(CMainFrame*)AfxGetMainWnd();

		GetDlgItem(IDC_EDIT1)->SetWindowText(_T("������Դ�ļ���·��"));
		((CListBox *)GetDlgItem(IDC_LIST2))->ResetContent();
		for(int i=0; i<pMain->m_wndFileView.mResPaths.size(); i++)
		{
			CString str=pMain->m_wndFileView.mResPaths.at(i);
			((CListBox *)GetDlgItem(IDC_LIST2))->AddString(str);
		}
		return TRUE;
	}

	afx_msg void OnBnClickedButton10()
	{
		CMainFrame *pMain=(CMainFrame*)AfxGetMainWnd();
		temptoAddPath=*addFilePath();	
		if (!temptoAddPath.IsEmpty())
		{
			GetDlgItem(IDC_EDIT1)->SetWindowText(temptoAddPath);
		}
	}

	afx_msg void OnBnClickedButton1()
	{
		CMainFrame *pMain=(CMainFrame*)AfxGetMainWnd();
		GetDlgItem(IDC_EDIT1)->GetWindowTextW(temptoAddPath);


		if (!temptoAddPath.IsEmpty())
		{
			if (PathIsDirectory(temptoAddPath) || PathIsRoot(temptoAddPath))    //�ж�·���Ϸ�//
			{  
				pMain->AddResPath(temptoAddPath);
				OnInitDialog();
				temptoAddPath.Empty();
			}  
			else  
			{  
				 MessageBoxW(_T("����·���Ƿ�Ϸ�"));
			}  
		
		}else{  MessageBoxW(_T("·��Ϊ��"));  }
	}




	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(AddResDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON10, &AddResDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON1, &AddResDlg::OnBnClickedButton1)
END_MESSAGE_MAP()



void CMainFrame::AddResPath()
{
	AddResDlg mAddResDlg;
	mAddResDlg.DoModal();
}

void CMainFrame::AddResPath( CString &resPath )
{  
		USES_CONVERSION;         //CStringת��std::string�ַ���Ҫ�ô˺�//
		std::string str1 = W2A(resPath.GetBuffer());
		Ogre::String filePath(str1);
		char groupName[20];
		sprintf(groupName,"Group%i",m_wndFileView.tempGroup);
		Ogre::String strGroup(groupName);
		m_wndFileView.tempGroup+=1;
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(filePath,"FileSystem",strGroup);  
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(strGroup);

		m_wndFileView.mResPaths.push_back(resPath);
		
		//��ʾ�����ڵ�//
		HTREEITEM hMeshTemp=NULL;
		HTREEITEM hSoundTemp=NULL;
		std::list<CString> fileList;
		m_wndFileView.TraverseFolder(resPath,fileList);
		CString folder;
		int npos=resPath.ReverseFind('\\');
		if (npos!=-1)
		{
			folder=resPath.Right(resPath.GetLength()-npos-1);
		}


		CString temp;
		for (std::list<CString>::iterator i=fileList.begin(); i!=fileList.end(); ++i)
		{
			temp=*i;
			int nPos = temp.Find('.');  
			CString fileextion;
			fileextion = temp.Right(temp.GetLength()-nPos-1);
			if (fileextion=="ogg"||fileextion=="mid"||fileextion=="mp3"||fileextion=="wav")
			{
				if (hSoundTemp==NULL)
				{ hSoundTemp=m_wndFileView.m_wndFileView.InsertItem(folder,0,0,m_wndFileView.hSound); }
				   m_wndFileView.m_wndFileView.InsertItem(temp, 4, 4, hSoundTemp);
			}

			if (fileextion=="mesh")
			{
				if (hMeshTemp==NULL)
				{ hMeshTemp=m_wndFileView.m_wndFileView.InsertItem(folder,0,0,m_wndFileView.hMesh); }
				m_wndFileView.m_wndFileView.InsertItem(temp, 4, 4, hMeshTemp);
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
				m_wndFileView.m_wndFileView.InsertItem(CString(particleName.c_str()), 4, 4,m_wndFileView.hParticle);
				i++;
			}
		}

}

void CMainFrame::SaveScene()
{
	CString defaultDir("../../Scene"); //Ĭ�ϱ���·��//
	CString defaultFile("Scene1.db");//Ĭ�ϱ����ļ���//
	CString filer("��������|*.db|�����ļ�|*.*||");
	CFileDialog dlg(false, defaultDir, defaultFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		filer, NULL);        //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	CString filePath=defaultDir+"\\"+defaultFile;
	if(dlg.DoModal()==IDOK)
	{
		filePath=dlg.GetPathName();    //�ļ����ͱ���·��������FilePath��
		BaseApplication::getInstance()->mscene->saveScene(m_wndFileView.mResPaths, filePath);
	}
}

void CMainFrame::OpenScene()
{
	//MessageBoxW(_T("��Ҫ��ʧ���г������ݣ��Ƿ����"),_T("����"),MB_OKCANCEL | MB_ICONQUESTION);

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		(LPCTSTR)_TEXT("���������ļ�|*.db|�����ļ�|*.*||"), NULL);        //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

	if(dlg.DoModal()==IDOK)
	{
		CString Scenepath=dlg.GetPathName();
		USES_CONVERSION;
		std::string str1=W2A(Scenepath.GetBuffer());
		Ogre::String OScenePath(str1);
		Ogre::String filePath, fileName, fileExt;
		Ogre::StringUtil::splitFullFilename(OScenePath,fileName,fileExt,filePath);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(filePath,"FileSystem","ET",true);  
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("ET");
		BaseApplication::getInstance()->mscene->loadScene(OScenePath); //���س�������//
		m_wndProperties.m_wndEditTerrain->initView();  //��ʼ�����α༭���//	
	}


}

void CMainFrame::selectObj()
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	if (appMgr->mscene->mTerrainEditMode==TerrainSelectObj)
	{
		m_wndProperties.m_wndEditTerrain->OnBnClickedRadio8();    //�رջ�ˢ//
	}
	else
	{
		m_wndProperties.m_wndEditTerrain->OnBnClickedRadio8();    //�رջ�ˢ//
		appMgr->mscene->mTerrainEditMode=TerrainSelectObj;
	}
	
	
}

void CMainFrame::moveSelectObj()
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	if (appMgr->mscene->mObjectEditMode==ObjMove)
	{
		appMgr->mscene->mObjectEditMode=ObjNoneEdit;
	}
	else appMgr->mscene->mObjectEditMode=ObjMove;
	
}

void CMainFrame::scaleSelectObj()
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	if (appMgr->mscene->mObjectEditMode==ObjScale)
	{
		appMgr->mscene->mObjectEditMode=ObjNoneEdit;
	}
	else appMgr->mscene->mObjectEditMode=ObjScale;
}

void CMainFrame::rotateSelectObj()
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	if (appMgr->mscene->mObjectEditMode==ObjRoll)
	{
		appMgr->mscene->mObjectEditMode=ObjNoneEdit;
	}
	else appMgr->mscene->mObjectEditMode=ObjRoll;
}

void CMainFrame::copySelectObj()
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	if (appMgr->mscene->mObjectEditMode==ObjCopy)
	{
		appMgr->mscene->mObjectEditMode=ObjNoneEdit;
	}
	else appMgr->mscene->mObjectEditMode=ObjCopy;
}

void CMainFrame::deleteSelectObj()
{
	BaseApplication *appMgr=BaseApplication::getInstance();

	if(appMgr->mscene->selectSceneObj.size()>0)
	{
		INT_PTR nRes=NULL;  
		nRes = MessageBox(_T("��ȷ��Ҫɾ��ѡ��������"), _T("ɾ������"), MB_OKCANCEL | MB_ICONQUESTION);   
		if (IDCANCEL == nRes)   
			return;
	}
	appMgr->mscene->mObjectEditMode==ObjNoneEdit;
	while (appMgr->mscene->selectSceneObj.size()>0)
	{
		SceneObj* obj=appMgr->mscene->selectSceneObj.back();
		if (obj->mObjType==MeshObj)
		{
			if (((SceneMeshObj*)obj)->mPhysicsBox != NULL)
			{
				PhysicsBox* pBox=(PhysicsBox*)((SceneMeshObj*)obj)->mPhysicsBox;
				appMgr->mscene->deleteSceneObj(pBox);
			}
		}
		appMgr->mscene->deleteSceneObj(obj);
		appMgr->mscene->selectSceneObj.pop_back();
	}
	 m_wndObjProperties.InitPropList(NULL);
	 m_wndSceneObjView.FillSceneTree();     //refresh object in scene view tree
}

void CMainFrame::findSceneObj()
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	if (appMgr->mscene->mObjectEditMode==ObjFind)
	{
		appMgr->mscene->mObjectEditMode==ObjNoneEdit;
	}
	else appMgr->mscene->mObjectEditMode=ObjFind;
}

void CMainFrame::updateToolbarUI( CCmdUI *pCmdUI )
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	if (NULL!=appMgr&&appMgr->mscene!=NULL&&appMgr->mscene->bTerrainCreated)
	{

		switch(pCmdUI->m_nID)
		{
			case 32788: 
				pCmdUI->SetRadio(appMgr->mscene->mObjectEditMode==ObjMove);
				break;
			case 32790:
				pCmdUI->SetRadio(appMgr->mscene->mObjectEditMode==ObjRoll);
				break;
			case 32791:
				pCmdUI->SetRadio(appMgr->mscene->mObjectEditMode==ObjScale);
				break;
			case 32792:
				pCmdUI->SetRadio(appMgr->mscene->mObjectEditMode==ObjCopy);
				break;
			case 32793:
				pCmdUI->SetRadio(appMgr->mscene->mObjectEditMode==ObjDelete);
				break;
			case 32794:
				pCmdUI->SetRadio(appMgr->mscene->mObjectEditMode==ObjFind);
				break;
			case 32795:
				pCmdUI->SetRadio(appMgr->mscene->mTerrainEditMode==TerrainSelectObj);
				break;
			default:
				break;
		}
	}
	


}

void CMainFrame::showEditToolBar()
{
	m_wndToolBarEditObj.ShowWindow(!m_wndToolBarEditObj.IsVisible());
}


void CMainFrame::OnUpdateViewEditBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndToolBarEditObj.IsVisible());
}


void CMainFrame::OnViewFileShow()
{
	m_wndFileView.ShowPane(!m_wndFileView.IsVisible(),FALSE,TRUE);
}


void CMainFrame::OnUpdateViewFileShow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndFileView.IsVisible());
}


void CMainFrame::OnViewSceneShow()
{
	m_wndSceneObjView.ShowPane(!m_wndSceneObjView.IsVisible(),FALSE,TRUE);
}


void CMainFrame::OnUpdateViewSceneShow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndSceneObjView.IsVisible());

}


void CMainFrame::OnViewTerrainShow()
{
	m_wndProperties.ShowPane(!m_wndProperties.IsVisible(),FALSE,TRUE);
}


void CMainFrame::OnUpdateViewTerrainShow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndProperties.IsVisible());
}


void CMainFrame::OnViewProShow()
{
	m_wndObjProperties.ShowPane(!m_wndObjProperties.IsVisible(),FALSE,TRUE);
}


void CMainFrame::OnUpdateViewProShow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndObjProperties.IsVisible());
}


void CMainFrame::OnViewMinimapShow()
{
	m_wndOutput.ShowPane(!m_wndOutput.IsVisible(),FALSE,TRUE);
}


void CMainFrame::OnUpdateViewMinimapShow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}

void CMainFrame::OnViewSceneSetShow()
{
	m_wndSceneSetProperties.ShowPane(!m_wndSceneSetProperties.IsVisible(),FALSE,TRUE);
}

void CMainFrame::OnUpdateViewSceneSetShow( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(m_wndSceneSetProperties.IsVisible());
}

void CMainFrame::OnViewSetBarShow()
{
	m_wndToolBarViewEdit.ShowPane(!m_wndToolBarViewEdit.IsVisible(),FALSE,TRUE);
}

void CMainFrame::OnUpdateViewSetBarShow( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck(m_wndToolBarViewEdit.IsVisible());
}

void CMainFrame::OnUpdateSubMenueShow( UINT id )
{
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (appMgr->mscene!=NULL)
		{
			switch (id)
			{
			case ID_HIDE_CURRENT:    //����ѡ������//
				while (appMgr->mscene->selectSceneObj.size()>0)
				{
					SceneObj* obj = appMgr->mscene->selectSceneObj.at(appMgr->mscene->selectSceneObj.size()-1);
					obj->setVisualAble(false);
					appMgr->mscene->selectSceneObj.pop_back();
					appMgr->mscene->mhideSceneObj.push_back(obj);
				}
				break;
			case ID_SHOW_HIDE:
				if (appMgr->mscene->mhideSceneObj.size()>0)
				{
					appMgr->mscene->mhideSceneObj.back()->setVisualAble(true);
					appMgr->mscene->mhideSceneObj.pop_back();
				}
				break;
			case ID_SHOW_ALLHIDE:
				while(appMgr->mscene->mhideSceneObj.size()>0)
				{
					SceneObj* obj = appMgr->mscene->mhideSceneObj.at(appMgr->mscene->mhideSceneObj.size()-1);
					obj->setVisualAble(true);
					appMgr->mscene->mhideSceneObj.pop_back();
				}
				break;
			case ID_HIDE_OTHER:
				if (appMgr->mscene->selectSceneObj.size()>0)
				{
					for (std::list<SceneMeshObj*>::iterator j=appMgr->mscene->mMeshObjList.begin();
						j!=appMgr->mscene->mMeshObjList.end();j++)
					{
						Vector3 pos1=appMgr->mscene->selectSceneObj.back()->mPostion;
						Ogre::String name1=(*j)->mName;
						Ogre::String name2=appMgr->mscene->selectSceneObj.back()->mName;
						if (pos1.distance((*j)->mPostion)<=1500 && ( name1 != name2 ))
						{
							(*j)->setVisualAble(false);
							appMgr->mscene->mhideSceneObj.push_back(*j);
						}
					}
					for (std::list<SceneParticleObj*>::iterator j=appMgr->mscene->mParticleList.begin();
						j!=appMgr->mscene->mParticleList.end();j++)
					{

						Vector3 pos1=appMgr->mscene->selectSceneObj.back()->mPostion;
						if (pos1.distance((*j)->mPostion)<=1500)
						{
							(*j)->setVisualAble(false);
							appMgr->mscene->mhideSceneObj.push_back(*j);
						}
					}

				}
				break;
			case ID_FRAME_MODE:
				appMgr->mscene->mCamera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
				break;
			case ID_ENTITY_MODE:
				appMgr->mscene->mCamera->setPolygonMode(Ogre::PolygonMode::PM_SOLID);
				break;
			case ID_SHOW_ALL:  //��ʾ����ʵ��//
				showAllEntity=!showAllEntity;
				if (showAllEntity)
				{	
					for (std::vector<SceneObj*>::iterator j=appMgr->mscene->mhideSceneObj.begin();
						j!=appMgr->mscene->mhideSceneObj.end();)
					{
						if ((*j)->mObjType==MeshObj)
						{
							(*j)->setVisualAble(true);
							j=appMgr->mscene->mhideSceneObj.erase(j); //��ɾ��һ��Ԫ�غ�������Զ�ָ���һ���˲���Ҫ��//
						}
						else
						{
							j++;
						}
					}
				}
				else
				{
					for(std::list<SceneMeshObj*>::iterator i=appMgr->mscene->mMeshObjList.begin();
						i!=appMgr->mscene->mMeshObjList.end();i++)
					{
						(*i)->setVisualAble(false);
						appMgr->mscene->mhideSceneObj.push_back(*i);
					}
				}
			break;
			case ID_SHOW_ALL_COLLIDE:
				showAllCollide=!showAllCollide;
				if (showAllCollide)
				{	
					for (std::vector<SceneObj*>::iterator j=appMgr->mscene->mhideSceneObj.begin();
						j!=appMgr->mscene->mhideSceneObj.end();)
					{
						if ((*j)->mObjType==PhysicsBoxObj)
						{
							(*j)->setVisualAble(true);
							j=appMgr->mscene->mhideSceneObj.erase(j); //��ɾ��һ��Ԫ�غ�������Զ�ָ���һ���˲���Ҫ��//
						}
						else
						{
							j++;
						}
					}
				}
				else
				{
					for(std::list<PhysicsBox*>::iterator i=appMgr->mscene->mPhysisBoxList.begin();
						i!=appMgr->mscene->mPhysisBoxList.end();i++)
					{
						(*i)->setVisualAble(false);
						appMgr->mscene->mhideSceneObj.push_back(*i);
					}
				}
				break;
			case ID_SHOW_ALL_LIGHT:
				showAllLight=!showAllLight;
				if (showAllLight)
				{	
					for (std::vector<SceneObj*>::iterator j=appMgr->mscene->mhideSceneObj.begin();
						j!=appMgr->mscene->mhideSceneObj.end();)
					{
						if ((*j)->mObjType==LightObj)
						{
							(*j)->setVisualAble(true);
							j=appMgr->mscene->mhideSceneObj.erase(j); //��ɾ��һ��Ԫ�غ�������Զ�ָ���һ���˲���Ҫ��//
						}
						else
						{
							j++;
						}
					}
				}
				else
				{
					for(std::list<SceneLightObj*>::iterator i=appMgr->mscene->mLightObjList.begin();
						i!=appMgr->mscene->mLightObjList.end();i++)
					{
						(*i)->setVisualAble(false);
						appMgr->mscene->mhideSceneObj.push_back(*i);
					}
				}
				break;
			case ID_SHOW_ALL_SOUND:
				showAllSound=!showAllSound;
				if (showAllSound)
				{	
					for (std::vector<SceneObj*>::iterator j=appMgr->mscene->mhideSceneObj.begin();
						j!=appMgr->mscene->mhideSceneObj.end();)
					{
						if ((*j)->mObjType==SoundObj)
						{
							(*j)->setVisualAble(true);
							j=appMgr->mscene->mhideSceneObj.erase(j); //��ɾ��һ��Ԫ�غ�������Զ�ָ���һ���˲���Ҫ��//
						}
						else
						{
							j++;
						}
					}
				}
				else
				{
					for(std::list<SceneSoundObj*>::iterator i=appMgr->mscene->mSoundList.begin();
						i!=appMgr->mscene->mSoundList.end();i++)
					{
						(*i)->setVisualAble(false);
						appMgr->mscene->mhideSceneObj.push_back(*i);
					}
				}
				break;
			case ID_SHOW_ALL_PAETICLE:
				showAllParticle=!showAllParticle;
				if (showAllParticle)
				{	
					for (std::vector<SceneObj*>::iterator j=appMgr->mscene->mhideSceneObj.begin();
						j!=appMgr->mscene->mhideSceneObj.end();)
					{
						if ((*j)->mObjType==ParticleObj)
						{
							(*j)->setVisualAble(true);
							j=appMgr->mscene->mhideSceneObj.erase(j); //��ɾ��һ��Ԫ�غ�������Զ�ָ���һ���˲���Ҫ��//
						}
						else
						{
							j++;
						}
					}
				}
				else
				{
					for(std::list<SceneParticleObj*>::iterator i=appMgr->mscene->mParticleList.begin();
						i!=appMgr->mscene->mParticleList.end();i++)
					{
						(*i)->setVisualAble(false);
						appMgr->mscene->mhideSceneObj.push_back(*i);
					}
				}
				break;
			default: 
				break;
		}


	}//scene!=null
  }//appMgr!=NULL
	
}

void CMainFrame::OnUpdateSubMenueShowLook( CCmdUI* pCmdUI )
{
	switch(pCmdUI->m_nID)
	{
	case ID_SHOW_ALL:
		pCmdUI->SetCheck(showAllEntity);
		break;
	case ID_SHOW_ALL_COLLIDE:
		pCmdUI->SetCheck(showAllCollide);
		break;
	case ID_SHOW_ALL_LIGHT:
		pCmdUI->SetCheck(showAllLight);
		break;
	case ID_SHOW_ALL_SOUND:
		pCmdUI->SetCheck(showAllSound);
		break;
	case ID_SHOW_ALL_PAETICLE:
		pCmdUI->SetCheck(showAllParticle);
	default:
		break;
	}
}


void CMainFrame::openPhysics()
{
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			appMgr->mscene->bOpenPhysis=false;  //�ر���������//
			m_wndToolBarViewEdit.GetButton(8)->SetVisible(false);
			m_wndToolBarViewEdit.GetButton(9)->SetVisible(true);
			m_wndToolBarViewEdit.AdjustLayout();
		}
	}
}


void CMainFrame::closePhysics()
{
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			appMgr->mscene->bOpenPhysis=true;  //����������//
			m_wndToolBarViewEdit.GetButton(9)->SetVisible(false);
			m_wndToolBarViewEdit.GetButton(8)->SetVisible(true);
			m_wndToolBarViewEdit.AdjustLayout();
		}
	}
}


void CMainFrame::changePlayMode()
{
	InputManager *inputMgr=NULL;
	inputMgr=InputManager::getInstance();
	if (inputMgr!=NULL)
	{
		if (inputMgr->mscene!=NULL)
		{
			inputMgr->changeViewMode();
		}
		if (inputMgr->viewMode == 1 )
		{
			m_wndToolBarViewEdit.GetButton(12)->SetVisible(false);
			m_wndToolBarViewEdit.GetButton(11)->SetVisible(true);
		}
		if (inputMgr->viewMode == 2)			
		{
			m_wndToolBarViewEdit.GetButton(11)->SetVisible(false);
			m_wndToolBarViewEdit.GetButton(12)->SetVisible(true);
		}
		m_wndToolBarViewEdit.AdjustLayout();
	}
	
}

void CMainFrame::selectBoxMode()
{
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			appMgr->mscene->selectMode=0;
		}
	}

}

void CMainFrame::selectMeshMode()
{
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			appMgr->mscene->selectMode=1;
		}
	}
}


void CMainFrame::updateSelectMeshMode( CCmdUI *pCmdUI )
{
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			pCmdUI->SetCheck(appMgr->mscene->selectMode == 1);

		}
	}
}

void CMainFrame::updateSelectBoxMode( CCmdUI *pCmdUI )
{
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			bool bChecked=(appMgr->mscene->selectMode == 0);
			pCmdUI->SetCheck(bChecked);
		}
	}
}

void CMainFrame::ResetHeroPostion()
{
	/*
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			if (appMgr->mscene->bTerrainCreated)
			{
				Ogre::Vector3 pos = Hero::getInstance()->pnode->getPosition();
				pos.y = 100000;  //�����޸ߵĵط����������//////
				Ogre::Ray mRay(pos, Ogre::Vector3::NEGATIVE_UNIT_Y);
				std::pair<bool, Ogre::Vector3> result = appMgr->mscene->pTerrainInfo->rayIntersects(mRay);
				if (result.first)
				{
					Ogre::Vector3 newPos = result.second;
					newPos.y += 15;
					Hero::getInstance()->setPositon(newPos);
				}	
			}
		}
	}

	*/
}

void CMainFrame::calcuateLightMap()
{
	/*
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			if (appMgr->mscene->bTerrainCreated)
			{
			
				Ogre::ColourValue ambientColor = appMgr->mSceneMgr->getAmbientLight();
				Ogre::ColourValue lightColor(1.0, 1.0, 1.0);  
				Ogre::Vector3 lightDirection(0.2, -1, 0.2);
				for (std::list<SceneLightObj*>::iterator i=appMgr->mscene->mLightObjList.begin();
					i!=appMgr->mscene->mLightObjList.end(); i++)
				{
					if ((*i)->mType == Lightdirection)
					{
						lightColor = (*i)->mdiffuseColor;
						lightDirection = (*i)->mLight->getDirection();
						break;
					}
				}

				Image lightmap;
				ET::createTerrainLightmap(*(appMgr->mscene->pTerrainInfo), lightmap,  appMgr->mscene->TerrainNumTiles, 
					appMgr->mscene->TerrainNumTiles, Vector3(0.4,-1,0.4), lightColor,ambientColor);
				TexturePtr tex = TextureManager::getSingleton().getByName("ETLightmap");
				tex->getBuffer(0, 0)->blitFromMemory(lightmap.getPixelBox(0, 0));
			}
		}
	}
	*/
}

void CMainFrame::updateTerrainPhy()
{
	
	BaseApplication* appMgr=NULL;
	appMgr=BaseApplication::getInstance();
	if (appMgr!=NULL)
	{
		if (NULL!=appMgr->mscene)
		{
			if (appMgr->mscene->bTerrainCreated && appMgr->mscene->bPhysisCreate )
			{
				int numVeticals = 129;
			
				TerrainGroup::TerrainIterator ti = appMgr->mscene->mTerrainGroup->getTerrainIterator();
				while(ti.hasMoreElements())
				{
					Terrain* t = ti.getNext()->instance;
					numVeticals = t->getSize();				
					break;
				}
				appMgr->mscene->destroyPhysisWorld();
				appMgr->mscene->createPhysisWorld(appMgr->mscene->terrainSize,appMgr->mscene->terrainSize,1,numVeticals);
			}
		}
	}
	
}
