
#include "stdafx.h"
#include "EditTerrainWnd.h"
#include "SelectTextureDlg.h"

BEGIN_MESSAGE_MAP(EditTerainWnd, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &EditTerainWnd::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &EditTerainWnd::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_RADIO1, &EditTerainWnd::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &EditTerainWnd::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &EditTerainWnd::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &EditTerainWnd::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &EditTerainWnd::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &EditTerainWnd::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &EditTerainWnd::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &EditTerainWnd::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO15, &EditTerainWnd::OnBnClickedRadio15)
	ON_BN_CLICKED(IDC_RADIO9, &EditTerainWnd::OnBnClickedRadio9)
	ON_BN_CLICKED(IDC_RADIO10, &EditTerainWnd::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, &EditTerainWnd::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, &EditTerainWnd::OnBnClickedRadio12)
	ON_BN_CLICKED(IDC_RADIO18, &EditTerainWnd::OnBnClickedRadio18)
	ON_BN_CLICKED(IDC_RADIO19, &EditTerainWnd::OnBnClickedRadio19)
	ON_BN_CLICKED(IDC_RADIO20, &EditTerainWnd::OnBnClickedRadio20)
	ON_BN_CLICKED(IDC_RADIO21, &EditTerainWnd::OnBnClickedRadio21)
	ON_BN_CLICKED(IDC_BUTTON5, &EditTerainWnd::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON12, &EditTerainWnd::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON6, &EditTerainWnd::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &EditTerainWnd::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &EditTerainWnd::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON19, &EditTerainWnd::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON18, &EditTerainWnd::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON20, &EditTerainWnd::OnBnClickedButton20)
END_MESSAGE_MAP()


int oldPos = 0;
void EditTerainWnd::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	SCROLLINFO info;  
	info.cbSize = sizeof(SCROLLINFO);  
	info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;  
	GetScrollInfo(SB_VERT, &info);  
	const int minPos = info.nMin;  
	const int maxPos = info.nMax - (info.nPage - 1);  

	int newPos = 0;
	switch (nSBCode)   
	{   
		// ������Ϲ���һ�У���pos��1  
	case SB_LINEUP:   
		newPos = (info.nPos-1>minPos)?info.nPos-1 : minPos;   
		break;   
		// ������¹���һ�У���pos��1  
	case SB_LINEDOWN:   
		newPos = (info.nPos+1<maxPos)?info.nPos+1 : maxPos;   
		break;   
		// ������Ϲ���һҳ����pos��10  
	case SB_PAGEUP:   
		newPos = (info.nPos-(int)info.nPage>minPos)?info.nPos-(int)info.nPage : minPos;
		break;   
		// ������¹���һҳ����pos��10  
	case SB_PAGEDOWN:   
		newPos = (info.nPos+(int)info.nPage<maxPos)?info.nPos+(int)info.nPage : maxPos; 
		break;   
		// �����������ˣ���posΪ1  
	case SB_TOP:   
		newPos = minPos;   
		break;   
		// �����������׶ˣ���posΪ100  
	case SB_BOTTOM:   
		newPos = maxPos;   
		break;     
		// ����϶������������ָ��λ�ã���pos��ֵΪnPos��ֵ  
	case SB_THUMBTRACK:   
		newPos = info.nTrackPos;   
		break;   
		// �����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧�������ڴ˴����ñ༭������ʾ��ֵ  
	default:   
		return;   
	}   

	// ���ù�����λ�� ////////////
	SetScrollPos(SB_VERT, newPos, TRUE);  
	ScrollWindow(0, oldPos - newPos);  
	oldPos = newPos;
}

BOOL EditTerainWnd::OnInitDialog()
{
	char str[10];
	sprintf(str,"%i",numRow);
	CString s1(str);
	GetDlgItem(IDC_EDIT3)->SetWindowText(s1);
	sprintf(str,"%i",numLine);
	CString s2(str);
	GetDlgItem(IDC_EDIT4)->SetWindowText(s2);
	sprintf(str,"%i",Depart);
	CString s3(str);
	GetDlgItem(IDC_EDIT7)->SetWindowText(s3);
	sprintf(str,"%i",RandomDepart);
	CString s4(str);
	GetDlgItem(IDC_EDIT6)->SetWindowText(s4);
	sprintf(str,"%f",RandomScale);
	CString s5(str);
	GetDlgItem(IDC_EDIT9)->SetWindowText(s5);
	sprintf(str,"%f",RandomRoll);
	CString s6(str);
	GetDlgItem(IDC_EDIT5)->SetWindowText(s6);
	return TRUE;

}

void EditTerainWnd::initView()
{
	mscene = BaseApplication::getInstance()->mscene;

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetRange(0,100,true);   //�����4-32�ķ�Χת����0-100
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetRange(0,100,true);   //�����0.05-1�ķ�Χת����0-100
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(30);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->UpdateWindow();
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->SetPos(50);

	((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(TRUE);   //Ĭ�رջ�ˢ��ѡ��//////
	((CButton *)GetDlgItem(IDC_RADIO9))->SetCheck(TRUE);   //Ĭ������1��ѡ��//

	char str[10];
	sprintf(str,"%i",((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos());
	GetDlgItem(IDC_EDIT1)->SetWindowText(CString(str));
	sprintf(str,"%i",((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos());
	GetDlgItem(IDC_EDIT2)->SetWindowText(CString(str));

	for (int i=0;i<8;i++)
	{
		if (m_pFImg[i])
		{
			FreeImage_Unload(m_pFImg[i]);  //��λͼǰ�������ͷ���ǰ��////
			m_pFImg[i]=NULL;
		}
		if (mscene->diffuseTexture[i] != NULL)
		{
			CString strPath(mscene->diffuseTexture[i]->c_str());
			if (!strPath.IsEmpty())
			{
				FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
				USES_CONVERSION; 
				std::string str1 = W2A(strPath.GetBuffer());
				fif=FreeImage_GetFileType(str1.c_str());
				if (fif==FIF_UNKNOWN)
				{
					fif=FreeImage_GetFIFFromFilename(str1.c_str());
				}
				if (fif!=FIF_UNKNOWN&&FreeImage_FIFSupportsReading(fif))
				{
					m_pFImg[i]=FreeImage_Load(fif,str1.c_str());
				}

			}  //!strPath.IsEmpty()
		}
		else           //�����ǰ��ͼƬ//
		{
			CRect clientRect;
			if (i==0)
			{  GetDlgItem(IDC_BUTTON5)->GetWindowRect(&clientRect); }
			else if (i==1)
			{  GetDlgItem(IDC_BUTTON12)->GetWindowRect(&clientRect); }
			else if (i==2)
			{  GetDlgItem(IDC_BUTTON6)->GetWindowRect(&clientRect); }
			else if (i==3)
			{ GetDlgItem(IDC_BUTTON8)->GetWindowRect(&clientRect); }
			else if (i==4)
			{ GetDlgItem(IDC_BUTTON7)->GetWindowRect(&clientRect); }
			else if (i==5)
			{ GetDlgItem(IDC_BUTTON19)->GetWindowRect(&clientRect); }
			else if (i==6)
			{ GetDlgItem(IDC_BUTTON18)->GetWindowRect(&clientRect); }
			else if (i==7)
			{ GetDlgItem(IDC_BUTTON20)->GetWindowRect(&clientRect); }
			ScreenToClient(clientRect);
			CDC *pDC=GetDC();
			pDC->DrawFrameControl(clientRect,0,0);      //��ԭʼ��button//
			ReleaseDC(pDC);
		}
	}

	SetTimer(10, 50 , NULL);

}

void EditTerainWnd::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	int nPos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
	CString str=_T("");
	str.Format(_T("%d%%"),nPos);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);
	GetDlgItem(IDC_EDIT1)->UpdateWindow();

	if (mscene!=NULL)
	{
		mscene->BrushSize = 0.01 + (float)nPos * 0.001;
		//mscene->resizeBrush();
	}
	*pResult = 0;
}

void EditTerainWnd::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	int nPos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos();
	CString str=_T("");
	str.Format(_T("%d%%"),nPos);
	GetDlgItem(IDC_EDIT2)->SetWindowText(str);
	GetDlgItem(IDC_EDIT2)->UpdateWindow();

	if (mscene!=NULL)
	{
		mscene->BrushStrength = 100+ nPos * 4;
	}

	*pResult = 0;
}

void EditTerainWnd::OnBnClickedRadio1()   // ����//
{
	if (mscene->bTerrainCreated)
	{
		mscene->mTerrainEditMode=TerrainUp;
	}
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio2()   // �½�//
{
	if (mscene->bTerrainCreated)
	{
		mscene->mTerrainEditMode=TerrainDown;
	}
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio3()    // �������//
{
	mscene->mTerrainEditMode=AddObj;
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio4()   // ƽ��//
{	
	if (mscene->bTerrainCreated)
	{
		mscene->mTerrainEditMode=TerrainToLevel;
	}
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio5()   	// ƽ��//
{
	if (mscene->bTerrainCreated)
	{
		mscene->mTerrainEditMode=TerrainSmooth;
	}
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio6()  //���//
{
	if (mscene->bTerrainCreated)
	{
		mscene->mTerrainEditMode=TerrainNoise;
	}
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio7()  // �������//
{
	if (mscene->bTerrainCreated)
	{
		mscene->mTerrainEditMode=TerrainPlatUp;
	}
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio8()   // �رջ�ˢ//
{	
	mscene->mTerrainEditMode=TerrainNoneEdit;
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio15()  //�������//
{
	if (mscene->bTerrainCreated)
	{
		mscene->mTerrainEditMode=TerrainPlatDown;
	}
	checkRadioButton();
}

void EditTerainWnd::OnBnClickedRadio9()   // ѡ������1//
{
	checkRadioButton2(9);
}

void EditTerainWnd::OnBnClickedRadio10()    // ѡ������2//
{
	checkRadioButton2(10);
}

void EditTerainWnd::OnBnClickedRadio11()    // ѡ������3//
{
	checkRadioButton2(11);
}

void EditTerainWnd::OnBnClickedRadio12()
{
	checkRadioButton2(12);
}

void EditTerainWnd::OnBnClickedRadio18()
{
	checkRadioButton2(18);
}

void EditTerainWnd::OnBnClickedRadio19()
{
	checkRadioButton2(19);
}

void EditTerainWnd::OnBnClickedRadio20()
{
	checkRadioButton2(20);
}

void EditTerainWnd::OnBnClickedRadio21()
{
	checkRadioButton2(21);
}

void EditTerainWnd::OnTimer( UINT nIDEvent )
{
	if (nIDEvent == 10)
	{
		CDC *pDC=GetDC();
		for (int i=0;i<8;i++)
		{
			if (NULL != m_pFImg[i])
			{
				CRect clientRect;
				if (i==0)
				{  GetDlgItem(IDC_BUTTON5)->GetWindowRect(&clientRect); }
				else if (i==1)
				{  GetDlgItem(IDC_BUTTON12)->GetWindowRect(&clientRect); }
				else if (i==2)
				{  GetDlgItem(IDC_BUTTON6)->GetWindowRect(&clientRect); }
				else if (i==3)
				{ GetDlgItem(IDC_BUTTON8)->GetWindowRect(&clientRect); }
				else if (i==4)
				{ GetDlgItem(IDC_BUTTON7)->GetWindowRect(&clientRect); }
				else if (i==5)
				{ GetDlgItem(IDC_BUTTON19)->GetWindowRect(&clientRect); }
				else if (i==6)
				{ GetDlgItem(IDC_BUTTON18)->GetWindowRect(&clientRect); }
				else if (i==7)
				{ GetDlgItem(IDC_BUTTON20)->GetWindowRect(&clientRect); }
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

void EditTerainWnd::checkRadioButton()
{
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO7))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO15))->SetCheck(FALSE);	//��ѡ��//

	switch (mscene->mTerrainEditMode)
	{
	case TerrainUp:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(true);	//����//
		break;
	case TerrainDown:
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(true);	//�½�//
		break;
	case AddObj:
		((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(true);	//�������//
		break;
	case TerrainToLevel:
		((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(true);	//ƽ��//
		break;
	case TerrainSmooth:
		((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(true);	//ƽ��//
		break;
	case TerrainNoise:
		((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(true);	//���//
		break;
	case TerrainPlatUp:
		((CButton *)GetDlgItem(IDC_RADIO7))->SetCheck(true);	//�������//
		break;
	case TerrainPlatDown:
		((CButton *)GetDlgItem(IDC_RADIO15))->SetCheck(true);	//�������//
		break;
	case TerrainNoneEdit:
		((CButton *)GetDlgItem(IDC_RADIO8))->SetCheck(true);	//�رջ�ˢ//
		break;
	default:
		break;
	}

}

void EditTerainWnd::checkRadioButton2( int n )
{
	((CButton *)GetDlgItem(IDC_RADIO9))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO10))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO11))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO12))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO18))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO19))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO20))->SetCheck(FALSE);	//��ѡ��//
	((CButton *)GetDlgItem(IDC_RADIO21))->SetCheck(FALSE);	//��ѡ��//

	if (mscene!=NULL && mscene->bTerrainCreated)
	{
		int numTextures = 0;
		for (int i=0; i<8; i++)
		{
			if (mscene->diffuseTexture[i] != NULL)
			{
				numTextures+=1;
			}
			else break;
		}
		bool bChecked = false;
		switch (n)
		{
		case 9:
			if (numTextures>=1){ ((CButton *)GetDlgItem(IDC_RADIO9))->SetCheck(true); mscene->mSplatImage=0; bChecked = true;}
			break;
		case 10:
			if (numTextures>=2){ ((CButton *)GetDlgItem(IDC_RADIO10))->SetCheck(true); mscene->mSplatImage=1; bChecked = true;}
			break;
		case 11:
			if (numTextures>=3){ ((CButton *)GetDlgItem(IDC_RADIO11))->SetCheck(true); mscene->mSplatImage=2; bChecked = true;}
			break;
		case 12:
			if (numTextures>=4){ ((CButton *)GetDlgItem(IDC_RADIO12))->SetCheck(true); mscene->mSplatImage=3; bChecked = true;}
			break;
		case 18:
			if (numTextures>=5){ ((CButton *)GetDlgItem(IDC_RADIO18))->SetCheck(true); mscene->mSplatImage=4; bChecked = true;}
			break;
		case 19:
			if (numTextures>=6){ ((CButton *)GetDlgItem(IDC_RADIO19))->SetCheck(true); mscene->mSplatImage=5; bChecked = true;}
			break;
		case 20:
			if (numTextures>=7){ ((CButton *)GetDlgItem(IDC_RADIO20))->SetCheck(true); mscene->mSplatImage=6; bChecked = true;}
			break;
		case 21:
			if (numTextures>=8){ ((CButton *)GetDlgItem(IDC_RADIO21))->SetCheck(true); mscene->mSplatImage=7; bChecked = true;}
			break;
		default:
			break;
		}
		if (!bChecked)
		{
			if(mscene->mSplatImage == 0){ ((CButton *)GetDlgItem(IDC_RADIO9))->SetCheck(true);}
			else if(mscene->mSplatImage == 1){ ((CButton *)GetDlgItem(IDC_RADIO10))->SetCheck(true);}
			else if(mscene->mSplatImage == 2){ ((CButton *)GetDlgItem(IDC_RADIO11))->SetCheck(true);}
			else if(mscene->mSplatImage == 3){ ((CButton *)GetDlgItem(IDC_RADIO12))->SetCheck(true);}
			else if(mscene->mSplatImage == 4){ ((CButton *)GetDlgItem(IDC_RADIO18))->SetCheck(true);}
			else if(mscene->mSplatImage == 5){ ((CButton *)GetDlgItem(IDC_RADIO19))->SetCheck(true);}
			else if(mscene->mSplatImage == 6){ ((CButton *)GetDlgItem(IDC_RADIO20))->SetCheck(true);}
			else if(mscene->mSplatImage == 7){ ((CButton *)GetDlgItem(IDC_RADIO21))->SetCheck(true);}
		}

	}

}


//���´�ֱ�������ķ�Χ/////
void EditTerainWnd::updateVScrollRange( float ylen )
{
	SCROLLINFO ySi;	
	ySi.cbSize = sizeof (ySi);
	ySi.fMask  = SIF_ALL;
	ySi.nMin = 0;      
	ySi.nMax = 550;     //�ܹ���������//
	if (ylen>=550)  //���ÿҳ�ɼ����ش��������أ������������ʧ������ʧǰ�Ƚ�λ�ø�ԭ/////////////////////////////////////
	{
		ScrollWindow(0, oldPos);
		oldPos = 0;		 
	}
	ySi.nPage =ylen;     //ÿҳ�ܿ�����������//
	ySi.nPos = 0;
	ySi.nTrackPos = 0;
	SetScrollInfo(SB_VERT,&ySi);
	//SetScrollRange(SB_VERT,0,100);  ���ù�����Χ


}




void EditTerainWnd::OnBnClickedButton5()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 0;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}


void EditTerainWnd::OnBnClickedButton12()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 1;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}


void EditTerainWnd::OnBnClickedButton6()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 2;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}


void EditTerainWnd::OnBnClickedButton8()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 3;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}


void EditTerainWnd::OnBnClickedButton7()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 4;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}


void EditTerainWnd::OnBnClickedButton19()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 5;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}


void EditTerainWnd::OnBnClickedButton18()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 6;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}


void EditTerainWnd::OnBnClickedButton20()
{
	if (mscene!= NULL && mscene->bTerrainCreated)
	{
		mscene->selectTexture = 7;
		SelectTextureDlg selDlg;
		selDlg.DoModal();
	}
}
