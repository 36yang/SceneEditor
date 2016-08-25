#include "stdafx.h"
#include "SceneSetPropertiesWnd.h"
#include "Resource.h"
#include "EditScene.h"
#include "BaseApplication.h"

SceneSetPropertiesWnd::SceneSetPropertiesWnd()
{
}

SceneSetPropertiesWnd::~SceneSetPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(SceneSetPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(1,ComBoxSelectChanged)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)       //处理属性变化事件
END_MESSAGE_MAP()



void SceneSetPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient,rectCombo;
	GetClientRect(rectClient);
	m_wndObjectCombo.GetWindowRect(&rectCombo);
	int cyCmb = rectCombo.Size().cy;
	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), rectClient.Height() - cyCmb, SWP_NOACTIVATE | SWP_NOZORDER);
}

int SceneSetPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("未能创建属性组合 \n");
		return -1;  
	}

	m_wndObjectCombo.AddString(_T("物理引擎设置"));
	m_wndObjectCombo.AddString(_T("浏览设置"));
	m_wndObjectCombo.AddString(_T("单位设置"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;  
	}

	//InitPropList();
	AdjustLayout();
	return 0;
}

void SceneSetPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void SceneSetPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void SceneSetPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void SceneSetPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void SceneSetPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void SceneSetPropertiesWnd::OnProperties1()
{
	// TODO: 在此处添加命令处理程序代码
}

void SceneSetPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void SceneSetPropertiesWnd::OnProperties2()
{
	// TODO: 在此处添加命令处理程序代码
}

void SceneSetPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void SceneSetPropertiesWnd::InitPropList()
{
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("外观"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("三维外观"), (_variant_t) false, _T("指定窗口的字体不使用粗体，并且控件将使用三维边框")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一:“无”、“细”、“可调整大小”或“对话框外框”"));
	pProp->AddOption(_T("无"));
	pProp->AddOption(_T("细"));
	pProp->AddOption(_T("可调整大小"));
	pProp->AddOption(_T("对话框外框"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("标题"), (_variant_t) _T("关于"), _T("指定窗口标题栏中显示的文本")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("窗口大小"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定窗口的高度"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("宽度"), (_variant_t) 150l, _T("指定窗口的宽度"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);
	m_wndPropList.AddProperty(pSize);
;

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("杂项"));
	pProp = new CMFCPropertyGridProperty(_T("(名称)"), _T("应用程序"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("窗口颜色"), RGB(210, 192, 254), NULL, _T("指定默认的窗口颜色"));
	pColorProp->EnableOtherButton(_T("其他..."));
	pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定窗口图标")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("文件夹"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("层次结构"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("第一个子级"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("第二个子级"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 1"), (_variant_t) _T("值 1"), _T("此为说明")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 2"), (_variant_t) _T("值 2"), _T("此为说明")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 3"), (_variant_t) _T("值 3"), _T("此为说明")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
}

void SceneSetPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void SceneSetPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
}


void SceneSetPropertiesWnd::ComBoxSelectChanged()
{
	currentSet=m_wndObjectCombo.GetCurSel();
	if (currentSet==0)
	{
		InitUnitSetProp();
	}
	else if (currentSet==1)
	{
		InitViewSetPeop();
	}
	else if (currentSet==2)
	{	
		InitPhsicsSetProp();
	}


}

void SceneSetPropertiesWnd::InitPhsicsSetProp()
{
	EditScene* mscene=BaseApplication::getInstance()->mscene;
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();  //改变的属性用粗体//

	CMFCPropertyGridProperty* pUsePhysics = new CMFCPropertyGridProperty(_T("物理引擎"), _T("启用"), _T("是否启用物理引擎，只有启用物理引擎下面的设置才能生效。请在添加物体之前设置，否则有可能导致显示不正确"));
	pUsePhysics->AddOption(_T("启用"));   
	pUsePhysics->AddOption(_T("关闭"));   
	if (mscene->bOpenPhysis)
	{
		pUsePhysics->SetValue(_T("启用"));
	}
	else pUsePhysics->SetValue(_T("关闭"));
	m_wndPropList.AddProperty(pUsePhysics);
	CMFCPropertyGridProperty* pAutoSleep = new CMFCPropertyGridProperty(_T("自动休眠"), _T("禁止"), _T("当开启此项后，当物体静止时不再模拟，可以节省部分CPU，请谨慎使用"));
	pAutoSleep->AddOption(_T("禁止"));   
	pAutoSleep->AddOption(_T("允许"));
	if (mscene->bEnagleAutoSleep)
	{
		pUsePhysics->SetValue(_T("允许"));
	}
	else pUsePhysics->SetValue(_T("禁止"));
	m_wndPropList.AddProperty(pAutoSleep);
	CMFCPropertyGridProperty* pGravity = new CMFCPropertyGridProperty(_T("重力"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X方向重力"),(_variant_t)mscene->gravity.x, _T("x方向重力"));
	pGravity->AddSubItem(pProp);
	pProp =new CMFCPropertyGridProperty(_T("Y方向重力"),(_variant_t)mscene->gravity.y, _T("y方向重力"));
	pGravity->AddSubItem(pProp);
	pProp =new CMFCPropertyGridProperty(_T("X方向重力"),(_variant_t)mscene->gravity.z, _T("z方向重力"));
	pGravity->AddSubItem(pProp);
	m_wndPropList.AddProperty(pGravity);
	CMFCPropertyGridProperty* pSoftCFm= new CMFCPropertyGridProperty(_T("全局混合力"),(_variant_t)mscene->softCFM, _T("全局混合力，建议取值0.001-1之间"));
	m_wndPropList.AddProperty(pSoftCFm);
	CMFCPropertyGridProperty* pNumIterations= new CMFCPropertyGridProperty(_T("模拟迭代步数"),(_variant_t)mscene->QuicksetpNumIterations, _T("模拟迭代步数，越大越精细准确，速度越慢，默认32，建议16-64之间"));
	m_wndPropList.AddProperty(pNumIterations);
}

void SceneSetPropertiesWnd::InitUnitSetProp()
{
	EditScene* mscene=BaseApplication::getInstance()->mscene;
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();  //改变的属性用粗体//

	CMFCPropertyGridProperty* pUserUnit = new CMFCPropertyGridProperty(_T("模型单位"), _T(""), _T("模型单位设置，默认分米，决定建立模型时,模型的缩放，如果您觉得模型过大或过小请调整此设置，1米=3.28英尺，1英寸=2.54厘米"));
	pUserUnit->AddOption(_T("毫米"));   
	pUserUnit->AddOption(_T("厘米"));  
	pUserUnit->AddOption(_T("分米"));   
	pUserUnit->AddOption(_T("米"));   
	pUserUnit->AddOption(_T("英尺"));   
	pUserUnit->AddOption(_T("英寸"));   
	float unint = BaseApplication::getInstance()->mscene->mUnit; 
	if (unint >= 0.009998 && unint <= 0.010099)
	{
		pUserUnit->SetValue(_T("毫米"));
	}
	else if (unint >= 0.099998 && unint<= 0.100099)
	{
		pUserUnit->SetValue(_T("厘米"));
	}
	else if (unint == 1)
	{
		pUserUnit->SetValue(_T("分米"));
	}
	else if (unint == 10)
	{
		pUserUnit->SetValue(_T("米"));
	}
	else if (unint >= 3.047999 && unint<= 3.048001 )
	{
		pUserUnit->SetValue(_T("英尺"));
	}
	else if (unint >= 0.253898 && unint <= 0.254001)
	{
		pUserUnit->SetValue(_T("英寸"));
	}
	else pUserUnit->SetValue(_T("未定义"));
	m_wndPropList.AddProperty(pUserUnit);
	CMFCPropertyGridProperty* pSelfUnit = new CMFCPropertyGridProperty(_T("自定义比例"),(_variant_t)mscene->mUnit, _T("自定义缩放比例，系统以分米为标准，例如若您的模型以毫米为单位建立，则缩放到分米对应比例是0.01"));
	m_wndPropList.AddProperty(pSelfUnit); 
	
	
}

void SceneSetPropertiesWnd::InitViewSetPeop()
{
	InputManager *InputMgr=InputManager::getInstance();
	if (NULL!=InputMgr)
	{
		m_wndPropList.RemoveAll();
		m_wndPropList.EnableHeaderCtrl(FALSE);
		m_wndPropList.EnableDescriptionArea();
		m_wndPropList.SetVSDotNetLook();
		m_wndPropList.MarkModifiedProperties();  //改变的属性用粗体//

		CMFCPropertyGridProperty* pCamViewSpeed = new CMFCPropertyGridProperty(_T("相机速度"), (_variant_t)InputMgr->viewSpeed, _T("编辑场景时，相机的移动速度"));
		pCamViewSpeed->EnableSpinControl(TRUE,1,50);
		m_wndPropList.AddProperty(pCamViewSpeed);
		CMFCPropertyGridProperty* pMouseZSpeed = new CMFCPropertyGridProperty(_T("滚轮微调速度"), (_variant_t)(int)InputMgr->mouseZViewSpeed, _T("滚轮缩放调节距离的速度，建议设置的比较小，有利于精细调节距离"));
		pMouseZSpeed->EnableSpinControl(TRUE,1,50);
		m_wndPropList.AddProperty(pMouseZSpeed);
		CMFCPropertyGridProperty* pEditspeed = new CMFCPropertyGridProperty(_T("物体编辑速度"), (_variant_t)(int)(InputMgr->mEditPointor->EditSpeed *100), _T("滚轮缩放调节距离的速度，建议设置的比较小，有利于精细调节距离"));
		pEditspeed->EnableSpinControl(TRUE,1,100);
		m_wndPropList.AddProperty(pEditspeed);
		Hero *h=Hero::getInstance();
		if(NULL!=h)
		{
			CMFCPropertyGridProperty* pMoveSpeed = new CMFCPropertyGridProperty(_T("人物移动速度"), (_variant_t)(int)h->mMoveSpeed, _T("浏览模式下，人物的移动速度"));
			pMoveSpeed->EnableSpinControl(TRUE,10,100);
			m_wndPropList.AddProperty(pMoveSpeed);
		}
	}
}



LRESULT SceneSetPropertiesWnd::OnPropertyChanged( WPARAM wParam,LPARAM lParam )
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam; 
	int i= (int)pProp->GetData();
	CString PropName=pProp->GetName();  //被改变的参数名//
	COleVariant value = pProp->GetValue(); //改变之后的值//
	if (PropName==CString(_T("自定义比例")))
	{
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->mUnit = val; 
		return 0;
	}
	else if (PropName==CString(_T("模型单位")))
	{
		CString str;
		str=value.bstrVal;     //从COleVariant 到CString
		if (str == _T("毫米"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 0.01;
			InitUnitSetProp();
		}
		else if (str == _T("厘米"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 0.1;
			InitUnitSetProp();
		}
		else if (str == _T("分米"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 1;
			InitUnitSetProp();
		}
		else if (str == _T("米"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 10;
			InitUnitSetProp();
		}
		else if (str == _T("英尺"))
		{
			BaseApplication::getInstance()->mscene->mUnit =3.048;
			InitUnitSetProp();
		}
		else if (str == _T("英寸"))
		{
			BaseApplication::getInstance()->mscene->mUnit =0.25390;
			InitUnitSetProp();
		}
		
		return 0;
	}
	else if (PropName==CString(_T("相机速度")))
	{
		int val = value.intVal;
		InputManager *InputMgr=InputManager::getInstance();
		if (NULL != InputMgr) { InputMgr->viewSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("滚轮微调速度")))
	{
		int val = value.intVal;
		InputManager *InputMgr=InputManager::getInstance();
		if (NULL != InputMgr) { InputMgr->mouseZViewSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("物体编辑速度")))
	{
		float val = value.intVal / 100.0;
		InputManager *InputMgr=InputManager::getInstance();
		if (NULL != InputMgr) { InputMgr->mEditPointor->EditSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("人物移动速度")))
	{
		int val = value.intVal;
		Hero *h=Hero::getInstance();
		if(NULL!=h) { h->mMoveSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("物理引擎")))
	{
		CString str;
		str=value.bstrVal;     //从COleVariant 到CString
		if (str == _T("启用"))
		{
			BaseApplication::getInstance()->mscene->bOpenPhysis = true;
		}
		else
		{
			BaseApplication::getInstance()->mscene->bOpenPhysis = false;
		}
	}
	else if (PropName==CString(_T("自动休眠")))
	{
		CString str;
		str=value.bstrVal;     //从COleVariant 到CString
		EditScene *scene = BaseApplication::getInstance()->mscene;
		if (str == _T("允许"))
		{
			scene->bEnagleAutoSleep = true;
			dWorldSetAutoDisableFlag (scene->mOdeWorld,scene->bEnagleAutoSleep); 
		}
		else
		{
			BaseApplication::getInstance()->mscene->bEnagleAutoSleep = false;
			dWorldSetAutoDisableFlag (scene->mOdeWorld,scene->bEnagleAutoSleep);
		}
	}
	else if (PropName == CString(_T("X方向重力")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->gravity.x = val;
		dWorldSetGravity (scene->mOdeWorld,scene->gravity.x,scene->gravity.y,scene->gravity.z);
	}
	else if (PropName == CString(_T("Y方向重力")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->gravity.y = val;
		dWorldSetGravity (scene->mOdeWorld,scene->gravity.x,scene->gravity.y,scene->gravity.z);
	}
	else if (PropName == CString(_T("Z方向重力")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->gravity.z = val;
		dWorldSetGravity (scene->mOdeWorld,scene->gravity.x,scene->gravity.y,scene->gravity.z);
	}
	else if (PropName == CString(_T("全局混合力")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		scene->softCFM = val;
		dWorldSetCFM(scene->mOdeWorld,scene->softCFM);
	}
	else if (PropName == CString(_T("模拟迭代步数")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		int val = value.intVal;
		scene->QuicksetpNumIterations = val;
		dWorldSetQuickStepNumIterations (scene->mOdeWorld, scene->QuicksetpNumIterations);
	}

	return 0;
}
