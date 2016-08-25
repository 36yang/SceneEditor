#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SceneEditor.h"
#include "MainFrm.h"

#include "SceneEditorDoc.h"
#include "SceneEditorView.h"
#include "BaseApplication.h"
#include "afxwin.h"
#include "FreeImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSceneEditorApp

BEGIN_MESSAGE_MAP(CSceneEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CSceneEditorApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_SCENE_NEW, &CSceneEditorApp::newScene)
	ON_COMMAND(ID_SCENE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_Terrain_New, &CSceneEditorApp::CreateTerrain)
	ON_COMMAND(ID_Destroy_Terrain, &CSceneEditorApp::DestroyTerrain)
END_MESSAGE_MAP()


// CSceneEditorApp 构造

CSceneEditorApp::CSceneEditorApp()
{
	m_bHiColorIcons = TRUE;
	m_bSaveState=FALSE;     //不保存程序布局状态
	m_bLoadWindowPlacement=FALSE;   //不加载程序布局状态

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SceneEditor.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CSceneEditorApp 对象

CSceneEditorApp theApp;


// CSceneEditorApp 初始化

BOOL CSceneEditorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSceneEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CSceneEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

int CSceneEditorApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() : CDialogEx(CAboutDlg::IDD)
	{
	}

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	}


	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



//新建场景地形对话框
class NewSceneDlg : public CDialogEx
{
public:
	// 对话框数据
	enum { IDD = IDD_NEW_SCENE };
	int TerrainSize;
	int numVertex;
	int Xtiles, Ztiles;
	int baseHight;
	Ogre::String *BaseTexturePath;
	Ogre::String *BaseNormalTexturePath;
	Ogre::String *HightMapPath;
	FIBITMAP* m_pFImg[3];   //FreeImage位图对象//   

	NewSceneDlg() : CDialogEx(NewSceneDlg::IDD)
	{
		
		TerrainSize=500;
		numVertex=129;
		Xtiles=1;
		Ztiles=1;
		baseHight=0;
		BaseTexturePath = new Ogre::String("../media/materials/textures/Terrain/grass_green-01_diffusespecular.dds");
		BaseNormalTexturePath = NULL;
		HightMapPath=NULL;
		m_pFImg[0] = NULL; m_pFImg[1]=NULL; m_pFImg[2]=NULL;
		m_pFImg[0]=LoadFIBITMAP(CString("../media/materials/textures/Terrain/grass_green-01_diffusespecular.dds"));	
		
	
	}

	FIBITMAP* LoadFIBITMAP(CString path)
	{	
		FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
		USES_CONVERSION; 
		std::string str1 = W2A(path.GetBuffer());
		fif=FreeImage_GetFileType(str1.c_str());
		if (fif==FIF_UNKNOWN)
		{
			fif=FreeImage_GetFIFFromFilename(str1.c_str());
		}
		if (fif!=FIF_UNKNOWN&&FreeImage_FIFSupportsReading(fif))
		{
			FIBITMAP *dib=FreeImage_Load(fif,str1.c_str());
			return dib;
		}
		return NULL;
	}

	void DrawFIBitmap(FIBITMAP *pFI,CRect &rectClient)
	{
		if (pFI)
		{
			CDC *pDC=GetDC();
			BYTE *pBits=FreeImage_GetBits(pFI);
			BITMAPINFO *pif=FreeImage_GetInfo(pFI);
			int W=FreeImage_GetWidth(pFI);
			int H=FreeImage_GetHeight(pFI);
			int dstW=rectClient.Width();
			int dstH=rectClient.Height();
			::SetStretchBltMode(pDC->GetSafeHdc(),COLORONCOLOR);
			::StretchDIBits(pDC->GetSafeHdc(),rectClient.left,rectClient.top,dstW,dstH,0,0,W,H,pBits,pif,DIB_RGB_COLORS,SRCCOPY);
			ReleaseDC(pDC);  //不释放会内存溢出///
		}
	}

	Ogre::String* openFile()
	{
		CString *FilePathName=new CString();
		Ogre::String *filePath=NULL;

		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			(LPCTSTR)_TEXT("常用位图 (*.png *.jpg *.bmp)|*.png;*.jpg;*.bmp|特殊图片 (*.dds)|*.dds|特殊图片 (*.tga)|*.tga|All Files (*.*)|*.*||"), NULL);        //TRUE为OPEN对话框，FALSE为SAVE AS对话框

		if(dlg.DoModal()==IDOK)
		{
			*FilePathName=dlg.GetPathName(); //文件名保存在了FilePathName里
			
			USES_CONVERSION;         //CString转换std::string字符串要用此宏
			std::string str1 = W2A(FilePathName->GetBuffer());
			filePath=new Ogre::String(str1);
		}
		return filePath;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX)        // DDX/DDV 支持
	{
		CDialogEx::DoDataExchange(pDX);
	}


	virtual BOOL OnInitDialog()
	{
		char str[8];
		sprintf(str,"%i",TerrainSize);
		GetDlgItem(IDC_EDIT1)->SetWindowText(CString(str));
		sprintf(str,"%i",baseHight);
		GetDlgItem(IDC_EDIT3)->SetWindowText(CString(str));
		float totalX = TerrainSize * Xtiles;
		sprintf(str,"%.0f",totalX);
		GetDlgItem(IDC_EDIT4)->SetWindowText(CString(str));
		float totalZ = TerrainSize * Ztiles;
		sprintf(str,"%.0f",totalZ);
		GetDlgItem(IDC_EDIT5)->SetWindowText(CString(str));

		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("1"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("2"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("3"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("4"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("5"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("6"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("7"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("8"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("9"));
		((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_T("10"));

		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("1"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("2"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("3"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("4"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("5"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("6"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("7"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("8"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("9"));
		((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_T("10"));

		((CComboBox *)GetDlgItem(IDC_COMBO4))->AddString(_T("129"));
		((CComboBox *)GetDlgItem(IDC_COMBO4))->AddString(_T("257"));
		((CComboBox *)GetDlgItem(IDC_COMBO4))->AddString(_T("513"));
		((CComboBox *)GetDlgItem(IDC_COMBO4))->AddString(_T("1025"));
		((CComboBox *)GetDlgItem(IDC_COMBO4))->AddString(_T("2049"));

		((CComboBox *)GetDlgItem(IDC_COMBO2))->SetCurSel(0);
		((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
		((CComboBox *)GetDlgItem(IDC_COMBO4))->SetCurSel(1);

		SetTimer(1,60,NULL);
		return TRUE;
	}

	afx_msg void OnBnClickedOk()
	{
		CString str;
		GetDlgItemText(IDC_EDIT1,str);
		TerrainSize= _ttoi(str);
		GetDlgItemText(IDC_EDIT3,str);
		baseHight=_ttoi(str);
		
		if (TerrainSize>=1&&TerrainSize<=65000&&baseHight>=-10000&&baseHight<=10000)
		{
			BaseApplication::getInstance()->mscene->createTerrain(TerrainSize,baseHight,
				numVertex,Xtiles,Ztiles,BaseTexturePath,BaseNormalTexturePath,HightMapPath);
			CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
			pMain->m_wndProperties.m_wndEditTerrain->checkRadioButton();
			pMain->m_wndProperties.m_wndEditTerrain->initView();  //初始化地形刷视图内容
			CDialogEx::OnOK();
		}
		else
		{
			MessageBox(_T("你输入的数据不合法，地形长宽必须在1-65000之间，高度必须在-10000-10000之间的数据"));
		}

	}

	
	afx_msg void OnCbnSelchangeCombo1()
	{
		int n = ((CComboBox *)GetDlgItem(IDC_COMBO1))->GetCurSel();
		if (0<=n && n<10)
		{	
			Ztiles = n+1;	
			float totalZ = Ztiles * TerrainSize;
			char str[16];
			sprintf(str,"%.0f",totalZ);
			GetDlgItem(IDC_EDIT5)->SetWindowText(CString(str));
		}	
	}

	afx_msg void OnCbnSelchangeCombo2()
	{
		int n = ((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel();
		if (0<=n && n<10)
		{	
			Xtiles = n+1;	
			float totalX = TerrainSize * Xtiles;
			char str[16];
			sprintf(str,"%.0f",totalX);
			GetDlgItem(IDC_EDIT4)->SetWindowText(CString(str));
		}	
	}

	afx_msg void OnCbnSelchangeCombo3()
	{
		int n = ((CComboBox *)GetDlgItem(IDC_COMBO3))->GetCurSel();
		baseHight = n / 10;
	}

	afx_msg void OnCbnSelchangeCombo4()
	{
		int n = ((CComboBox *)GetDlgItem(IDC_COMBO4))->GetCurSel();
		numVertex = 128;
		for (int i=0; i<n;++i)
		{
			numVertex = numVertex * 2;
		}
		numVertex += 1;
	}



	afx_msg void OnBnClickedButton1()
	{
		Ogre::String *s=openFile();
		if (s!=NULL)
		{
			BaseTexturePath = s;
			/*CImage image;
			image.Load(CString(TexturePaths[0].c_str()).GetString());
			HBITMAP hBitmap=image.Detach();
			((CButton *)GetDlgItem(IDC_BUTTON1))->SetBitmap(hBitmap);*/
			if (m_pFImg[0])
			{
				FreeImage_Unload(m_pFImg[0]);  //打开位图前必须先释放以前的////
				m_pFImg[0]=NULL;
			}
			m_pFImg[0]=LoadFIBITMAP(CString(BaseTexturePath->c_str()));			
			

		}	
		s=NULL;
	}


	afx_msg void OnBnClickedButton2()
	{
		BaseNormalTexturePath=openFile();
		if (BaseNormalTexturePath!=NULL)
		{
			if (m_pFImg[1])
			{
				FreeImage_Unload(m_pFImg[1]);
				m_pFImg[1]=NULL;
			}
			m_pFImg[1]=LoadFIBITMAP(CString(BaseNormalTexturePath->c_str()));			
		}
	}


	afx_msg void OnBnClickedButton3()
	{
		HightMapPath=openFile();
		if (HightMapPath!=NULL)
		{
			if (m_pFImg[2])
			{
				FreeImage_Unload(m_pFImg[2]);
				m_pFImg[2]=NULL;
			}
			m_pFImg[2]=LoadFIBITMAP(CString(HightMapPath->c_str()));			
		}
	}

	afx_msg void OnEnChangeEdit1()
	{
		CString str;
		GetDlgItemText(IDC_EDIT1,str);
		TerrainSize= _ttoi(str);
		if (TerrainSize>=1 && TerrainSize<=65000)
		{
			float totalX = TerrainSize * Xtiles;
			char s[16];
			sprintf(s,"%.0f",totalX);
			GetDlgItem(IDC_EDIT4)->SetWindowText(CString(s));
			float totalZ = Ztiles * TerrainSize;			
			sprintf(s,"%.0f",totalZ);
			GetDlgItem(IDC_EDIT5)->SetWindowText(CString(s));
		}
		else 
		{
			MessageBox(_T("你输入的数据不合法，地形大小必须在1-65000之间"));
		}

	}

	afx_msg void OnEnChangeEdit3()
	{
		CString str;
		GetDlgItemText(IDC_EDIT3,str);
		baseHight= _ttoi(str);
	}


	void OnTimer( UINT nIDEvent )
	{

		if (nIDEvent == 1)
		{
			for (int i=0;i<3;i++)
			{
				if ( NULL != m_pFImg[i])
				{
					CRect clientRect;
					if (i==0)
					{   GetDlgItem(IDC_BUTTON1)->GetWindowRect(&clientRect);}
					if (i==1)
					{  GetDlgItem(IDC_BUTTON2)->GetWindowRect(&clientRect);}
					if (i==2)
					{  GetDlgItem(IDC_BUTTON3)->GetWindowRect(&clientRect);}
					ScreenToClient(clientRect);
					DrawFIBitmap(m_pFImg[i],clientRect);
				}
			}
		}
	}


	DECLARE_MESSAGE_MAP()
	
};
BEGIN_MESSAGE_MAP(NewSceneDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewSceneDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &NewSceneDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &NewSceneDlg::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &NewSceneDlg::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &NewSceneDlg::OnCbnSelchangeCombo4)
	ON_BN_CLICKED(IDC_BUTTON1, &NewSceneDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &NewSceneDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &NewSceneDlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT1, &NewSceneDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT3, &NewSceneDlg::OnEnChangeEdit3)
	ON_WM_TIMER() 
	ON_CBN_SELCHANGE(IDC_COMBO1, &NewSceneDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &NewSceneDlg::OnBnClickedButton3)
END_MESSAGE_MAP()




// 用于运行对话框的应用程序命令
void CSceneEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSceneEditorApp 自定义加载/保存方法

void CSceneEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CSceneEditorApp::LoadCustomState()
{
}

void CSceneEditorApp::SaveCustomState()
{
}

void CSceneEditorApp::newScene()
{
	if(::MessageBox(NULL, _T("将要清空当前场景，是否继续？"),_T("新建场景"), MB_YESNO) == IDYES)
	{
		BaseApplication::getInstance()->mscene->clearScene();
	}
}

void CSceneEditorApp::CreateTerrain()
{
	NewSceneDlg NewDlg;
	NewDlg.DoModal();
}

void CSceneEditorApp::DestroyTerrain()
{
	if(::MessageBox(NULL, _T("将要删除当前场景中的地形，是否继续？"),_T("销毁地形"), MB_YESNO) == IDYES)
	{
		BaseApplication::getInstance()->mscene->destroyTerrain();
	}
	
}





