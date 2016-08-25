#pragma once

#include "FreeImage.h"
#include "Ogre.h"
#include "EditScene.h"

// SelectTextureDlg �Ի���

class SelectTextureDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(SelectTextureDlg)

public:
	SelectTextureDlg(CWnd* pParent = NULL); 
	virtual ~SelectTextureDlg();
	BOOL OnInitDialog();
	void OnTimer( UINT nIDEvent );
	Ogre::String* openFileDlg();

	enum { IDD = IDD_SELECT_TEX };
	EditScene* mscene;
	Ogre::String *TexturePath0;
	Ogre::String *NormalTexturePath0;
	Ogre::String *TexturePath1;
	Ogre::String *NormalTexturePath1;
	int layerSize;
	FIBITMAP* m_pFImg[2];   //FreeImageλͼ����//  



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedOk();
};
