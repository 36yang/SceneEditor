
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "SceneEditor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	m_wndEditTerrain->SetWindowPos(NULL,rectClient.left,rectClient.top,rectClient.Width(),500,SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndEditTerrain->updateVScrollRange(rectClient.Height());
	UpdateWindow();
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndEditTerrain=new EditTerainWnd();
	m_wndEditTerrain->Create(IDD_EDIT_TERRAIN,this);
	m_wndEditTerrain->ShowWindow(SW_SHOW);

	//CRect re;
	//GetWindowRect(re);
	//re.left=re.left-60;   //调整窗口宽度
	//MoveWindow(re,TRUE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	//调整布局//
	CRect rectClient;
	GetClientRect(rectClient);
	m_wndEditTerrain->SetWindowPos(NULL,rectClient.left, rectClient.top,rectClient.Width(),rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndEditTerrain->updateVScrollRange(rectClient.Height());

	UpdateWindow();	
	Invalidate(true);  //强制重绘窗口//
}


void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
}
