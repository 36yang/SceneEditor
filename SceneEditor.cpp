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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_SCENE_NEW, &CSceneEditorApp::newScene)
	ON_COMMAND(ID_SCENE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_Terrain_New, &CSceneEditorApp::CreateTerrain)
	ON_COMMAND(ID_Destroy_Terrain, &CSceneEditorApp::DestroyTerrain)
END_MESSAGE_MAP()


// CSceneEditorApp ����

CSceneEditorApp::CSceneEditorApp()
{
	m_bHiColorIcons = TRUE;
	m_bSaveState=FALSE;     //��������򲼾�״̬
	m_bLoadWindowPlacement=FALSE;   //�����س��򲼾�״̬

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SceneEditor.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CSceneEditorApp ����

CSceneEditorApp theApp;


// CSceneEditorApp ��ʼ��

BOOL CSceneEditorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSceneEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CSceneEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}

int CSceneEditorApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() : CDialogEx(CAboutDlg::IDD)
	{
	}

	// �Ի�������
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



//�½��������ζԻ���
class NewSceneDlg : public CDialogEx
{
public:
	// �Ի�������
	enum { IDD = IDD_NEW_SCENE };
	int TerrainSize;
	int numVertex;
	int Xtiles, Ztiles;
	int baseHight;
	Ogre::String *BaseTexturePath;
	Ogre::String *BaseNormalTexturePath;
	Ogre::String *HightMapPath;
	FIBITMAP* m_pFImg[3];   //FreeImageλͼ����//   

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
			ReleaseDC(pDC);  //���ͷŻ��ڴ����///
		}
	}

	Ogre::String* openFile()
	{
		CString *FilePathName=new CString();
		Ogre::String *filePath=NULL;

		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			(LPCTSTR)_TEXT("����λͼ (*.png *.jpg *.bmp)|*.png;*.jpg;*.bmp|����ͼƬ (*.dds)|*.dds|����ͼƬ (*.tga)|*.tga|All Files (*.*)|*.*||"), NULL);        //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���

		if(dlg.DoModal()==IDOK)
		{
			*FilePathName=dlg.GetPathName(); //�ļ�����������FilePathName��
			
			USES_CONVERSION;         //CStringת��std::string�ַ���Ҫ�ô˺�
			std::string str1 = W2A(FilePathName->GetBuffer());
			filePath=new Ogre::String(str1);
		}
		return filePath;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX)        // DDX/DDV ֧��
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
			pMain->m_wndProperties.m_wndEditTerrain->initView();  //��ʼ������ˢ��ͼ����
			CDialogEx::OnOK();
		}
		else
		{
			MessageBox(_T("����������ݲ��Ϸ������γ��������1-65000֮�䣬�߶ȱ�����-10000-10000֮�������"));
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
				FreeImage_Unload(m_pFImg[0]);  //��λͼǰ�������ͷ���ǰ��////
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
			MessageBox(_T("����������ݲ��Ϸ������δ�С������1-65000֮��"));
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




// �������жԻ����Ӧ�ó�������
void CSceneEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSceneEditorApp �Զ������/���淽��

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
	if(::MessageBox(NULL, _T("��Ҫ��յ�ǰ�������Ƿ������"),_T("�½�����"), MB_YESNO) == IDYES)
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
	if(::MessageBox(NULL, _T("��Ҫɾ����ǰ�����еĵ��Σ��Ƿ������"),_T("���ٵ���"), MB_YESNO) == IDYES)
	{
		BaseApplication::getInstance()->mscene->destroyTerrain();
	}
	
}





