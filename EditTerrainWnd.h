#pragma once
#include "BaseApplication.h"
#include "Resource.h"
#include "EditScene.h"
#include "FreeImage.h"

class EditTerainWnd : public CDialogEx 
{
public:
	// 对话框数据
	enum { IDD = IDD_EDIT_TERRAIN };
	int m_page;     //滑动条用到
	EditScene *mscene;
	int numRow,numLine;   //添加物体行列数
	int Depart;         //物体间距>0
	int RandomDepart;   //物体间距随机变化范围0
	float RandomRoll;   //物体随机旋转方位
	float RandomScale;     //物体随机缩放范围0
	FIBITMAP* m_pFImg[8];   //FreeImage位图对象//   



	EditTerainWnd() : CDialogEx(EditTerainWnd::IDD)
	{
		m_page=0;
		mscene=NULL;
		numRow = 1;
		numLine = 1;
		Depart = 10;
		RandomDepart = 0;
		RandomRoll = 0;
		RandomScale = 0;
		for (int i=0; i<8; i++)
		{
			m_pFImg[i]=NULL;
		}
	}

	
	void updateVScrollRange(float ylen) ;
	void initView();
	void checkRadioButton();
	void checkRadioButton2(int n);
	void OnTimer( UINT nIDEvent );
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX)        // DDX/DDV 支持
	{
		CDialogEx::DoDataExchange(pDX);
	}


	virtual BOOL OnInitDialog();
	
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio15();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio12();
	afx_msg void OnBnClickedRadio18();
	afx_msg void OnBnClickedRadio19();
	afx_msg void OnBnClickedRadio20();
	afx_msg void OnBnClickedRadio21();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton20();
};
