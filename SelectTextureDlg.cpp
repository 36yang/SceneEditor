// SelectTextureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "SelectTextureDlg.h"
#include "afxdialogex.h"
#include "BaseApplication.h"
#include "MainFrm.h"

// SelectTextureDlg 对话框

BEGIN_MESSAGE_MAP(SelectTextureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SelectTextureDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SelectTextureDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &SelectTextureDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &SelectTextureDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &SelectTextureDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


//IMPLEMENT_DYNAMIC(SelectTextureDlg, CDialogEx)

SelectTextureDlg::SelectTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SelectTextureDlg::IDD, pParent)
{
	mscene = NULL;
	TexturePath0 = NULL;
	NormalTexturePath0 = NULL;
	TexturePath1 = NULL;
	NormalTexturePath1 = NULL;
	layerSize = 50;
	m_pFImg[0] = NULL; m_pFImg[1] = NULL; 
}

SelectTextureDlg::~SelectTextureDlg()
{
}

BOOL SelectTextureDlg::OnInitDialog()
{
	mscene = BaseApplication::getInstance()->mscene;
	TexturePath0 = mscene->diffuseTexture[mscene->selectTexture];
	NormalTexturePath0 = mscene->normalTexture[mscene->selectTexture];
	TexturePath1 = TexturePath0; NormalTexturePath1 = NormalTexturePath0;
	if(TexturePath0 != NULL)
	{
		TerrainGroup::TerrainIterator ti = mscene->mTerrainGroup->getTerrainIterator();	
		while(ti.hasMoreElements())
		{
			Terrain* t = ti.getNext()->instance;
			layerSize = t->getLayerWorldSize(mscene->selectTexture);		
			break;
		}
		FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
		fif=FreeImage_GetFileType(TexturePath0->c_str());
		if (fif==FIF_UNKNOWN)
		{
			fif=FreeImage_GetFIFFromFilename( TexturePath0->c_str());
		}
		if (fif!=FIF_UNKNOWN&&FreeImage_FIFSupportsReading(fif))
		{
			m_pFImg[0]=FreeImage_Load(fif,TexturePath0->c_str());
		}
	}
	char str[12];
	sprintf(str,"%i",layerSize);
	GetDlgItem(IDC_EDIT1)->SetWindowText(CString(str));
	if (NormalTexturePath0 != NULL)
	{
		FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
		fif=FreeImage_GetFileType(NormalTexturePath0->c_str());
		if (fif==FIF_UNKNOWN)
		{
			fif=FreeImage_GetFIFFromFilename( NormalTexturePath0->c_str());
		}
		if (fif!=FIF_UNKNOWN&&FreeImage_FIFSupportsReading(fif))
		{
			m_pFImg[1]=FreeImage_Load(fif,NormalTexturePath0->c_str());
		}
	}

	SetTimer(11, 60 , NULL);
	return TRUE;
}


void SelectTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


Ogre::String* SelectTextureDlg::openFileDlg()
{
	Ogre::String *filePath=NULL;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		(LPCTSTR)_TEXT("图片|*.png;*.jpg;*.bmp;*.dds;*.tgs|所有文件|*.*||"), NULL);        //TRUE为OPEN对话框，FALSE为SAVE AS对话框

	if(dlg.DoModal()==IDOK)
	{
		CString FilePathName=dlg.GetPathName(); //文件名保存在了FilePathName里
		USES_CONVERSION;         //CString转换std::string字符串要用此宏
		std::string str1 = W2A(FilePathName.GetBuffer());
		filePath=new Ogre::String(str1);
	}
	return filePath;
}


void SelectTextureDlg::OnBnClickedButton1()   //选择纹理////
{
	TexturePath1 = openFileDlg();
	if (TexturePath1 != NULL)
	{
		if (m_pFImg[0])
		{
			FreeImage_Unload(m_pFImg[0]);
			m_pFImg[0]=NULL;
		}
		FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
		fif=FreeImage_GetFileType(TexturePath1->c_str());
		if (fif==FIF_UNKNOWN)
		{
			fif=FreeImage_GetFIFFromFilename(TexturePath1->c_str());
		}
		if (fif!=FIF_UNKNOWN&&FreeImage_FIFSupportsReading(fif))
		{
			m_pFImg[0]=FreeImage_Load(fif,TexturePath1->c_str());
		}
	}

}


void SelectTextureDlg::OnBnClickedButton2()  //删除纹理///
{
	int texCount = 0;
	TerrainGroup::TerrainIterator ti = mscene->mTerrainGroup->getTerrainIterator();
	while(ti.hasMoreElements())
	{
		Terrain* t = ti.getNext()->instance;
		texCount = t->getLayerCount();
		break;
	}

	if (texCount == 1)
	{
		AfxMessageBox(_T("必须至少保留一张纹理贴图"));
		return;
	}
	TexturePath1 = NULL;	
	if (m_pFImg[0])
	{
		FreeImage_Unload(m_pFImg[0]);
		m_pFImg[0]=NULL;
		CRect clientRect;
		GetDlgItem(IDC_STATIC_PIC1)->GetWindowRect(&clientRect);
		ScreenToClient(clientRect);
		CDC *pDC=GetDC();
		pDC->DrawFrameControl(clientRect,0,0);      //画原始的button//
		ReleaseDC(pDC);
	}
}


void SelectTextureDlg::OnBnClickedButton3()  //选择法线贴图////
{
	NormalTexturePath1 = openFileDlg();
	if (NormalTexturePath1 != NULL)
	{
		if (m_pFImg[1])
		{
			FreeImage_Unload(m_pFImg[1]);
			m_pFImg[1]=NULL;
		}
		FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
		fif=FreeImage_GetFileType(NormalTexturePath1->c_str());
		if (fif==FIF_UNKNOWN)
		{
			fif=FreeImage_GetFIFFromFilename(NormalTexturePath1->c_str());
		}
		if (fif!=FIF_UNKNOWN&&FreeImage_FIFSupportsReading(fif))
		{
			m_pFImg[1]=FreeImage_Load(fif,NormalTexturePath1->c_str());
		}
	}
}


void SelectTextureDlg::OnBnClickedButton4()  //删除法线贴图///
{
	NormalTexturePath1 = NULL;	
	if (m_pFImg[1])
	{
		FreeImage_Unload(m_pFImg[1]);
		m_pFImg[1]=NULL;
		CRect clientRect;
		GetDlgItem(IDC_STATIC_PIC2)->GetWindowRect(&clientRect);
		ScreenToClient(clientRect);
		CDC *pDC=GetDC();
		pDC->DrawFrameControl(clientRect,0,0);      //画原始的button//
		ReleaseDC(pDC);
	}
}


void SelectTextureDlg::OnBnClickedOk()
{
	bool setLayersize = true;
	TerrainGroup::TerrainIterator ti= mscene->mTerrainGroup->getTerrainIterator();
	CString str;
	GetDlgItemText(IDC_EDIT1,str);
	int layerSize1= _ttoi(str);
	int texCount = 0;
	while(ti.hasMoreElements())
	{
		Terrain* t = ti.getNext()->instance;
		texCount = t->getLayerCount();
		break;
	}

	//添加新的纹理///
	if (mscene->selectTexture > texCount-1  && TexturePath1!=NULL)
	{
		setLayersize = false;
		Ogre::StringVector *textures = new Ogre::StringVector();
		mscene->diffuseTexture[mscene->selectTexture] = new Ogre::String(TexturePath1->c_str());
		Ogre::String filename,fileFolder;
		Ogre::StringUtil::splitFilename(*TexturePath1,filename,fileFolder);
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(fileFolder,"FileSystem","Terrain");
		textures->push_back(filename);
		if (NormalTexturePath1 != NULL)
		{
			mscene->normalTexture[mscene->selectTexture] = new Ogre::String(NormalTexturePath1->c_str());
			Ogre::StringUtil::splitFilename(*NormalTexturePath1,filename,fileFolder);
			Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(fileFolder,"FileSystem","Terrain");
			textures->push_back(filename);
		}
		ti= mscene->mTerrainGroup->getTerrainIterator();
		while(ti.hasMoreElements())
		{
			Terrain* t = ti.getNext()->instance;
			t->addLayer(mscene->selectTexture,layerSize1,textures);
			t->dirty();
			t->update();
		}
		CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
		pMain->m_wndProperties.m_wndEditTerrain->checkRadioButton();
		pMain->m_wndProperties.m_wndEditTerrain->initView();
	}

	//删除旧的纹理///
	if (TexturePath1 == NULL && mscene->selectTexture <= texCount-1)
	{
		setLayersize = false;
		mscene->diffuseTexture[mscene->selectTexture] = NULL;
		mscene->normalTexture[mscene->selectTexture] = NULL;
		delete TexturePath0; delete NormalTexturePath0;
		ti= mscene->mTerrainGroup->getTerrainIterator();
		while(ti.hasMoreElements())
		{
			Terrain* t = ti.getNext()->instance;
			t->removeLayer(mscene->selectTexture);
			t->dirty();
			t->update();
		}
		CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
		pMain->m_wndProperties.m_wndEditTerrain->checkRadioButton();
		pMain->m_wndProperties.m_wndEditTerrain->initView();
	}

	//删除或替换或添加法线贴图//
	bool b0 = mscene->selectTexture<=texCount-1 && TexturePath0 == TexturePath1 && NormalTexturePath0 != NormalTexturePath1;
	//替换旧的纹理///
	bool b1 = mscene->selectTexture<=texCount-1 && TexturePath1 != NULL && 0 != TexturePath1->compare(TexturePath0->c_str());
	if (b0 || b1)
	{
		setLayersize = false;
		Ogre::StringVector *textures = new Ogre::StringVector();
		TexturePath0 = new Ogre::String(TexturePath1->c_str());
		mscene->diffuseTexture[mscene->selectTexture] = NULL;
		mscene->diffuseTexture[mscene->selectTexture] = TexturePath0;
		Ogre::String filename,fileFolder;
		Ogre::StringUtil::splitFilename(*TexturePath0,filename,fileFolder);
		textures->push_back(filename);
		Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(fileFolder,"FileSystem","Terrain");
		if (NormalTexturePath1 != NULL)
		{
			NormalTexturePath0 = new Ogre::String(NormalTexturePath1->c_str());
			if (mscene->normalTexture[mscene->selectTexture] != NULL)
			{
				mscene->normalTexture[mscene->selectTexture]->clear();
			}	
			mscene->normalTexture[mscene->selectTexture] = NormalTexturePath0;
			Ogre::StringUtil::splitFilename(*NormalTexturePath0,filename,fileFolder);
			Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(fileFolder,"FileSystem","Terrain");
			textures->push_back(filename);
		}
		else 
		{
			mscene->normalTexture[mscene->selectTexture] = NULL;
			textures->push_back("");  //没有法线贴图必须放个空字符串不然出错///////////////
		}
		ti= mscene->mTerrainGroup->getTerrainIterator();
		while(ti.hasMoreElements())
		{
			Terrain* t = ti.getNext()->instance;
			if (mscene->selectTexture == 0)
			{
				//t->removeLayer(0);
				//t->addLayer(0,layerSize1,textures);
				t->replaceLayer(mscene->selectTexture,true,layerSize1,textures);
			}
			else
			{
				t->replaceLayer(mscene->selectTexture,true,layerSize1,textures);
			}
			t->dirty();
			t->update();
		}
			
		CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
		pMain->m_wndProperties.m_wndEditTerrain->checkRadioButton();
		pMain->m_wndProperties.m_wndEditTerrain->initView();
	}

	if (setLayersize)
	{
		if (layerSize1 != layerSize)
		{
			ti = mscene->mTerrainGroup->getTerrainIterator();
			while(ti.hasMoreElements())
			{
				Terrain* t = ti.getNext()->instance;
				if (mscene->selectTexture <= (t->getLayerCount()-1) )
				{
					t->setLayerWorldSize(mscene->selectTexture,layerSize1);
					t->dirty();
					t->update();
				}
			}
		}
	}


	CDialogEx::OnOK();
}

void SelectTextureDlg::OnTimer( UINT nIDEvent )
{
	if (nIDEvent == 11)
	{
		CDC *pDC=GetDC();
		for (int i=0;i<2;i++)
		{
			if (NULL != m_pFImg[i])
			{
				CRect clientRect;
				if (i==0)
				{  GetDlgItem(IDC_STATIC_PIC1)->GetWindowRect(&clientRect); }
				if (i==1)
				{  GetDlgItem(IDC_STATIC_PIC2)->GetWindowRect(&clientRect); }
				ScreenToClient(clientRect);

				BYTE *pBits=FreeImage_GetBits(m_pFImg[i]);
				BITMAPINFO *pif=FreeImage_GetInfo(m_pFImg[i]);
				int W=FreeImage_GetWidth(m_pFImg[i]);
				int H=FreeImage_GetHeight(m_pFImg[i]);
				int dstW=clientRect.Width();
				int dstH=clientRect.Height();
				::SetStretchBltMode(pDC->GetSafeHdc(),COLORONCOLOR);
				::StretchDIBits(pDC->GetSafeHdc(),clientRect.left,clientRect.top,dstW,dstH,0,0,W,H,pBits,pif,DIB_RGB_COLORS,SRCCOPY);
			}
		}
		ReleaseDC(pDC);
	}
}


