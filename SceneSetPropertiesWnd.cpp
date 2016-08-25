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
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)       //�������Ա仯�¼�
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
		TRACE0("δ�ܴ���������� \n");
		return -1;  
	}

	m_wndObjectCombo.AddString(_T("������������"));
	m_wndObjectCombo.AddString(_T("�������"));
	m_wndObjectCombo.AddString(_T("��λ����"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�����������\n");
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
	// TODO: �ڴ˴���������������
}

void SceneSetPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: �ڴ˴����������� UI ����������
}

void SceneSetPropertiesWnd::OnProperties2()
{
	// TODO: �ڴ˴���������������
}

void SceneSetPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: �ڴ˴����������� UI ����������
}

void SceneSetPropertiesWnd::InitPropList()
{
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("���"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��ά���"), (_variant_t) false, _T("ָ�����ڵ����岻ʹ�ô��壬���ҿؼ���ʹ����ά�߿�")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("�߿�"), _T("�Ի������"), _T("����֮һ:���ޡ�����ϸ�������ɵ�����С���򡰶Ի������"));
	pProp->AddOption(_T("��"));
	pProp->AddOption(_T("ϸ"));
	pProp->AddOption(_T("�ɵ�����С"));
	pProp->AddOption(_T("�Ի������"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("���ڴ�С"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t) 250l, _T("ָ�����ڵĸ߶�"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("���"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);
	m_wndPropList.AddProperty(pSize);
;

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("����"));
	pProp = new CMFCPropertyGridProperty(_T("(����)"), _T("Ӧ�ó���"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), RGB(210, 192, 254), NULL, _T("ָ��Ĭ�ϵĴ�����ɫ"));
	pColorProp->EnableOtherButton(_T("����..."));
	pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("ͼ���ļ�(*.ico)|*.ico|�����ļ�(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("ͼ��"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("ָ������ͼ��")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("�ļ���"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("��νṹ"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("��һ���Ӽ�"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�ڶ����Ӽ�"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 1"), (_variant_t) _T("ֵ 1"), _T("��Ϊ˵��")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 2"), (_variant_t) _T("ֵ 2"), _T("��Ϊ˵��")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 3"), (_variant_t) _T("ֵ 3"), _T("��Ϊ˵��")));

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
	m_wndPropList.MarkModifiedProperties();  //�ı�������ô���//

	CMFCPropertyGridProperty* pUsePhysics = new CMFCPropertyGridProperty(_T("��������"), _T("����"), _T("�Ƿ������������棬ֻ����������������������ò�����Ч�������������֮ǰ���ã������п��ܵ�����ʾ����ȷ"));
	pUsePhysics->AddOption(_T("����"));   
	pUsePhysics->AddOption(_T("�ر�"));   
	if (mscene->bOpenPhysis)
	{
		pUsePhysics->SetValue(_T("����"));
	}
	else pUsePhysics->SetValue(_T("�ر�"));
	m_wndPropList.AddProperty(pUsePhysics);
	CMFCPropertyGridProperty* pAutoSleep = new CMFCPropertyGridProperty(_T("�Զ�����"), _T("��ֹ"), _T("����������󣬵����徲ֹʱ����ģ�⣬���Խ�ʡ����CPU�������ʹ��"));
	pAutoSleep->AddOption(_T("��ֹ"));   
	pAutoSleep->AddOption(_T("����"));
	if (mscene->bEnagleAutoSleep)
	{
		pUsePhysics->SetValue(_T("����"));
	}
	else pUsePhysics->SetValue(_T("��ֹ"));
	m_wndPropList.AddProperty(pAutoSleep);
	CMFCPropertyGridProperty* pGravity = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X��������"),(_variant_t)mscene->gravity.x, _T("x��������"));
	pGravity->AddSubItem(pProp);
	pProp =new CMFCPropertyGridProperty(_T("Y��������"),(_variant_t)mscene->gravity.y, _T("y��������"));
	pGravity->AddSubItem(pProp);
	pProp =new CMFCPropertyGridProperty(_T("X��������"),(_variant_t)mscene->gravity.z, _T("z��������"));
	pGravity->AddSubItem(pProp);
	m_wndPropList.AddProperty(pGravity);
	CMFCPropertyGridProperty* pSoftCFm= new CMFCPropertyGridProperty(_T("ȫ�ֻ����"),(_variant_t)mscene->softCFM, _T("ȫ�ֻ����������ȡֵ0.001-1֮��"));
	m_wndPropList.AddProperty(pSoftCFm);
	CMFCPropertyGridProperty* pNumIterations= new CMFCPropertyGridProperty(_T("ģ���������"),(_variant_t)mscene->QuicksetpNumIterations, _T("ģ�����������Խ��Խ��ϸ׼ȷ���ٶ�Խ����Ĭ��32������16-64֮��"));
	m_wndPropList.AddProperty(pNumIterations);
}

void SceneSetPropertiesWnd::InitUnitSetProp()
{
	EditScene* mscene=BaseApplication::getInstance()->mscene;
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();  //�ı�������ô���//

	CMFCPropertyGridProperty* pUserUnit = new CMFCPropertyGridProperty(_T("ģ�͵�λ"), _T(""), _T("ģ�͵�λ���ã�Ĭ�Ϸ��ף���������ģ��ʱ,ģ�͵����ţ����������ģ�͹�����С����������ã�1��=3.28Ӣ�ߣ�1Ӣ��=2.54����"));
	pUserUnit->AddOption(_T("����"));   
	pUserUnit->AddOption(_T("����"));  
	pUserUnit->AddOption(_T("����"));   
	pUserUnit->AddOption(_T("��"));   
	pUserUnit->AddOption(_T("Ӣ��"));   
	pUserUnit->AddOption(_T("Ӣ��"));   
	float unint = BaseApplication::getInstance()->mscene->mUnit; 
	if (unint >= 0.009998 && unint <= 0.010099)
	{
		pUserUnit->SetValue(_T("����"));
	}
	else if (unint >= 0.099998 && unint<= 0.100099)
	{
		pUserUnit->SetValue(_T("����"));
	}
	else if (unint == 1)
	{
		pUserUnit->SetValue(_T("����"));
	}
	else if (unint == 10)
	{
		pUserUnit->SetValue(_T("��"));
	}
	else if (unint >= 3.047999 && unint<= 3.048001 )
	{
		pUserUnit->SetValue(_T("Ӣ��"));
	}
	else if (unint >= 0.253898 && unint <= 0.254001)
	{
		pUserUnit->SetValue(_T("Ӣ��"));
	}
	else pUserUnit->SetValue(_T("δ����"));
	m_wndPropList.AddProperty(pUserUnit);
	CMFCPropertyGridProperty* pSelfUnit = new CMFCPropertyGridProperty(_T("�Զ������"),(_variant_t)mscene->mUnit, _T("�Զ������ű�����ϵͳ�Է���Ϊ��׼������������ģ���Ժ���Ϊ��λ�����������ŵ����׶�Ӧ������0.01"));
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
		m_wndPropList.MarkModifiedProperties();  //�ı�������ô���//

		CMFCPropertyGridProperty* pCamViewSpeed = new CMFCPropertyGridProperty(_T("����ٶ�"), (_variant_t)InputMgr->viewSpeed, _T("�༭����ʱ��������ƶ��ٶ�"));
		pCamViewSpeed->EnableSpinControl(TRUE,1,50);
		m_wndPropList.AddProperty(pCamViewSpeed);
		CMFCPropertyGridProperty* pMouseZSpeed = new CMFCPropertyGridProperty(_T("����΢���ٶ�"), (_variant_t)(int)InputMgr->mouseZViewSpeed, _T("�������ŵ��ھ�����ٶȣ��������õıȽ�С�������ھ�ϸ���ھ���"));
		pMouseZSpeed->EnableSpinControl(TRUE,1,50);
		m_wndPropList.AddProperty(pMouseZSpeed);
		CMFCPropertyGridProperty* pEditspeed = new CMFCPropertyGridProperty(_T("����༭�ٶ�"), (_variant_t)(int)(InputMgr->mEditPointor->EditSpeed *100), _T("�������ŵ��ھ�����ٶȣ��������õıȽ�С�������ھ�ϸ���ھ���"));
		pEditspeed->EnableSpinControl(TRUE,1,100);
		m_wndPropList.AddProperty(pEditspeed);
		Hero *h=Hero::getInstance();
		if(NULL!=h)
		{
			CMFCPropertyGridProperty* pMoveSpeed = new CMFCPropertyGridProperty(_T("�����ƶ��ٶ�"), (_variant_t)(int)h->mMoveSpeed, _T("���ģʽ�£�������ƶ��ٶ�"));
			pMoveSpeed->EnableSpinControl(TRUE,10,100);
			m_wndPropList.AddProperty(pMoveSpeed);
		}
	}
}



LRESULT SceneSetPropertiesWnd::OnPropertyChanged( WPARAM wParam,LPARAM lParam )
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam; 
	int i= (int)pProp->GetData();
	CString PropName=pProp->GetName();  //���ı�Ĳ�����//
	COleVariant value = pProp->GetValue(); //�ı�֮���ֵ//
	if (PropName==CString(_T("�Զ������")))
	{
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->mUnit = val; 
		return 0;
	}
	else if (PropName==CString(_T("ģ�͵�λ")))
	{
		CString str;
		str=value.bstrVal;     //��COleVariant ��CString
		if (str == _T("����"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 0.01;
			InitUnitSetProp();
		}
		else if (str == _T("����"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 0.1;
			InitUnitSetProp();
		}
		else if (str == _T("����"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 1;
			InitUnitSetProp();
		}
		else if (str == _T("��"))
		{
			BaseApplication::getInstance()->mscene->mUnit = 10;
			InitUnitSetProp();
		}
		else if (str == _T("Ӣ��"))
		{
			BaseApplication::getInstance()->mscene->mUnit =3.048;
			InitUnitSetProp();
		}
		else if (str == _T("Ӣ��"))
		{
			BaseApplication::getInstance()->mscene->mUnit =0.25390;
			InitUnitSetProp();
		}
		
		return 0;
	}
	else if (PropName==CString(_T("����ٶ�")))
	{
		int val = value.intVal;
		InputManager *InputMgr=InputManager::getInstance();
		if (NULL != InputMgr) { InputMgr->viewSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("����΢���ٶ�")))
	{
		int val = value.intVal;
		InputManager *InputMgr=InputManager::getInstance();
		if (NULL != InputMgr) { InputMgr->mouseZViewSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("����༭�ٶ�")))
	{
		float val = value.intVal / 100.0;
		InputManager *InputMgr=InputManager::getInstance();
		if (NULL != InputMgr) { InputMgr->mEditPointor->EditSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("�����ƶ��ٶ�")))
	{
		int val = value.intVal;
		Hero *h=Hero::getInstance();
		if(NULL!=h) { h->mMoveSpeed = val; }
		return 0;
	}
	else if (PropName==CString(_T("��������")))
	{
		CString str;
		str=value.bstrVal;     //��COleVariant ��CString
		if (str == _T("����"))
		{
			BaseApplication::getInstance()->mscene->bOpenPhysis = true;
		}
		else
		{
			BaseApplication::getInstance()->mscene->bOpenPhysis = false;
		}
	}
	else if (PropName==CString(_T("�Զ�����")))
	{
		CString str;
		str=value.bstrVal;     //��COleVariant ��CString
		EditScene *scene = BaseApplication::getInstance()->mscene;
		if (str == _T("����"))
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
	else if (PropName == CString(_T("X��������")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->gravity.x = val;
		dWorldSetGravity (scene->mOdeWorld,scene->gravity.x,scene->gravity.y,scene->gravity.z);
	}
	else if (PropName == CString(_T("Y��������")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->gravity.y = val;
		dWorldSetGravity (scene->mOdeWorld,scene->gravity.x,scene->gravity.y,scene->gravity.z);
	}
	else if (PropName == CString(_T("Z��������")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		BaseApplication::getInstance()->mscene->gravity.z = val;
		dWorldSetGravity (scene->mOdeWorld,scene->gravity.x,scene->gravity.y,scene->gravity.z);
	}
	else if (PropName == CString(_T("ȫ�ֻ����")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		float val = value.fltVal;
		scene->softCFM = val;
		dWorldSetCFM(scene->mOdeWorld,scene->softCFM);
	}
	else if (PropName == CString(_T("ģ���������")))
	{
		EditScene *scene = BaseApplication::getInstance()->mscene;
		int val = value.intVal;
		scene->QuicksetpNumIterations = val;
		dWorldSetQuickStepNumIterations (scene->mOdeWorld, scene->QuicksetpNumIterations);
	}

	return 0;
}
