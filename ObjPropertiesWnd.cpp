
#include "stdafx.h"

#include "ObjPropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "SceneEditor.h"
#include "BaseApplication.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

ObjPropertiesWnd::ObjPropertiesWnd()
{
	tempObj=NULL;
	mscene=NULL;
}

ObjPropertiesWnd::~ObjPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(ObjPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//ON_WM_NCACTIVATE()//ʧȥ����/////
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)       //�������Ա仯�¼�
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void ObjPropertiesWnd::OnTimer( UINT nIDEvent )
{
	if (mscene==NULL&&nIDEvent==2)
	{
		mscene=BaseApplication::getInstance()->mscene;
	}

	if (mscene!=NULL)
	{
		if (mscene->bTerrainCreated&&mscene->selectSceneObj.size()>0)
		{
			SceneObj* mobj=mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1);
			InitPropList(mobj);
		}
	}
}


void ObjPropertiesWnd::InitPropList( SceneObj* obj )
{
	tempObj = obj;
	m_wndPropList.RemoveAll();   //ɾ����������//
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	if (obj!=NULL)
	{
		mscene=BaseApplication::getInstance()->mscene;
		if (obj->mObjType==MeshObj)
		{
			InitMeshObjProp();
		}
		else if (obj->mObjType==SoundObj)
		{
			InitSoundObjProp();
		}
		else if (obj->mObjType==PhysicsBoxObj)
		{
			InitPhysicsBoxProp();
		}
		else if (obj->mObjType==LightObj)
		{
			InitLightObjProp();
		}
		else if (obj->mObjType==ParticleObj)
		{
			InitParticleObjProp();
		}
	}

}

void ObjPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

int ObjPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�����������\n");
		return -1;  
	}
	//InitPropList();
	AdjustLayout();
	return 0;
}

void ObjPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	//CRect cr(0,20,250,200);
	//m_wndEditTerrain->MoveWindow(cr);

	AdjustLayout();
}



void ObjPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
	//MessageBox(_T("fsfs"));

}

void ObjPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
}


LRESULT ObjPropertiesWnd::OnPropertyChanged( WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam; 
	OnSkyandFogPropertyChanged(pProp);
	if (tempObj==NULL) return 0;
	CString PropName=pProp->GetName();  //���ı�Ĳ�����//
	COleVariant value = pProp->GetValue(); //�ı�֮���ֵ//
	if (PropName==CString(_T("����")))
	{
		CString str;
		str=value.bstrVal;     //��COleVariant ��CString
		if (!str.IsEmpty())
		{
			USES_CONVERSION;
			std::string str1 = W2A(str.GetBuffer());
			Ogre::String meshName(str1);
			tempObj->mName=str1;
		}
		return 0;
	}
	else if (PropName==CString(_T("X:")))
	{
		float x=value.fltVal;
		if (x>-600000&&x<600000)
		{
			tempObj->mPostion.x=x;
			tempObj->setPosition(tempObj->mPostion);
		}
		return 0;
	}
	else if (PropName==CString(_T("Y:")))
	{
		float y=value.fltVal;
		if (y>-600000&&y<600000)
		{
			tempObj->mPostion.y=y;
			tempObj->setPosition(tempObj->mPostion);
		}
		return 0;
	}
	else if (PropName==CString(_T("Z:")))
	{
		float z=value.fltVal;
		if (z>-600000&&z<600000)
		{
			tempObj->mPostion.z=z;
			tempObj->setPosition(tempObj->mPostion);
		}
		return 0;
	}
	else if (PropName==CString(_T("ScaleX:")))
	{
		float x=value.fltVal;
		if (x>-10000&&x<10000)
		{
			tempObj->mScale.x=x;
			tempObj->setScale(tempObj->mScale);
		}
		return 0;
	}
	else if (PropName==CString(_T("ScaleY:")))
	{
		float y=value.fltVal;
		if (y>-10000&&y<10000)
		{
			tempObj->mScale.y=y;
			tempObj->setScale(tempObj->mScale);
		}
		return 0;
	}
	else if (PropName==CString(_T("ScaleZ:")))
	{
		float z=value.fltVal;
		if (z>-10000&&z<10000)
		{
			tempObj->mScale.z=z;
			tempObj->setScale(tempObj->mScale);
		}
		return 0;
	}
	else if (PropName==CString(_T("��x�ᣨ�ȣ�")))
	{
		float x=value.fltVal;
		if (x>-36000&&x<36000)
		{
			tempObj->mRotaion.x=x;
			tempObj->setRotaion(tempObj->mRotaion);
		}
		return 0;
	}
	else if (PropName==CString(_T("��y�ᣨ�ȣ�")))
	{
		float y=value.fltVal;
		if (y>-36000&&y<36000)
		{
			tempObj->mRotaion.y=y;
			tempObj->setRotaion(tempObj->mRotaion);
		}
		return 0;
	}
	else if (PropName==CString(_T("��z�ᣨ�ȣ�")))
	{
		float z=value.fltVal;
		if (z>-36000&&z<36000)
		{
			tempObj->mRotaion.z=z;
			tempObj->setRotaion(tempObj->mRotaion);
		}
		return 0;
	}
	else if (PropName==CString(_T("���ȼ�")))
	{
		int pri=value.intVal;
		((SceneSoundObj*)tempObj)->setPriority(pri);
		return 0;
	}
	else if (PropName==CString(_T("ѭ������")))
	{
		CString str=value.bstrVal;     
		if (!str.IsEmpty())
		{
			if (str==CString(_T("��")))
			{	((SceneSoundObj*)tempObj)->setLoop(true);	}
			else if (str==CString(_T("��")))
			{	((SceneSoundObj*)tempObj)->setLoop(false);	}
		}	
		return 0;
	}
	else if (PropName==CString(_T("����")))
	{
		int val=value.intVal;
		((SceneSoundObj*)tempObj)->setValume((float)val/10);
		return 0;
	}
	else if (PropName==CString(_T("������")))
	{
		CString str=value.bstrVal; 
		if (str==CString(_T("��")))
		{
			((SceneSoundObj*)tempObj)->setBingObj(NULL);
			return 0;
		}
		USES_CONVERSION;
		std::string str1 = W2A(str.GetBuffer());
		Ogre::String ObjName(str1);
		Ogre::String subName=ObjName.substr(0,4);
		if (subName == "Mesh")
		{
			for (std::list<SceneMeshObj*>::iterator i=mscene->mMeshObjList.begin();i!=mscene->mMeshObjList.end();i++)
			{
				if (ObjName==(*i)->mName)
				{
					((SceneSoundObj*)tempObj)->setBingObj(*i);
					return 0;
				}
			}
		}
		if (subName=="Part")
		{
			return 0;
		}
	
		return 0;
	}
	else if (PropName==CString(_T("��ײ������")))
	{
		CString str=value.bstrVal;     
		EditScene *scene=NULL;
		scene=BaseApplication::getInstance()->mscene;
		if (scene!=NULL)
		{
			if (!str.IsEmpty())
			{
				if (str==CString(_T("��")))
				{
					PhysicsBox* obj=(PhysicsBox*)(((SceneMeshObj*)tempObj)->mPhysicsBox);
					for (std::list<PhysicsBox*>::iterator j=scene->mPhysisBoxList.begin();j!=scene->mPhysisBoxList.end();j++)
					{
						if ((*j)->mName==obj->mName)
						{
							scene->SceneNode->removeChild(obj->mName);
							deletePtr(*j);  
							scene->mPhysisBoxList.remove(*j);
							((SceneMeshObj*)tempObj)->setmPhysicsBox(NULL);
							CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
							pMain->m_wndSceneObjView.FillSceneTree(); 				
							break;
						}
					}
					return 0;
				}
				
				PhysicsBox* Newobj=NULL;	
				if (tempObj->mObjType==MeshObj)
				{
					PhysicsBox* obj=(PhysicsBox*)(((SceneMeshObj*)tempObj)->mPhysicsBox);
					if (str==CString(_T("������"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeBox,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("����"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeSphere,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("Բ����"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCylinder,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("������"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCapsule,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("ƽ��"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypePlane,scene->PhysicsBoxTag,obj->mMeshObj);}	
					scene->SceneNode->addChild(Newobj->mNode);
					scene->mPhysisBoxList.push_back(Newobj);
					scene->PhysicsBoxTag+=1;	
					Newobj->setPosition(obj->mPostion);
					Newobj->setRotaion(obj->mRotaion);
					Newobj->setScale(obj->mScale);
					Newobj->mUserName=obj->mUserName;
					for (std::list<PhysicsBox*>::iterator j=scene->mPhysisBoxList.begin();j!=scene->mPhysisBoxList.end();j++)
					{
						if ((*j)->mName==obj->mName)
						{
							scene->SceneNode->removeChild(obj->mName);
							deletePtr(*j);  
							scene->mPhysisBoxList.remove(*j);
							((SceneMeshObj*)tempObj)->setmPhysicsBox(Newobj);
							CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
							pMain->m_wndSceneObjView.FillSceneTree(); 				
							break;
						}
					}
					InitPropList((SceneObj*)tempObj);
				}
				else
				{
					if (str==CString(_T("������"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeBox,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("����"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeSphere,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("Բ����"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCylinder,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("������"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCapsule,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("ƽ��"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypePlane,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					scene->SceneNode->addChild(Newobj->mNode);
					scene->mPhysisBoxList.push_back(Newobj);
					scene->PhysicsBoxTag+=1;	
					Newobj->setPosition(tempObj->mPostion);
					Newobj->setRotaion(tempObj->mRotaion);
					Newobj->setScale(tempObj->mScale);
					Newobj->mUserName=tempObj->mUserName;
					Newobj->mMeshObj=((PhysicsBox*)tempObj)->mMeshObj;
					if (Newobj->mMeshObj!=NULL)
					{
						Newobj->mMeshObj->setmPhysicsBox(Newobj);
					}
					for (std::list<PhysicsBox*>::iterator j=scene->mPhysisBoxList.begin();j!=scene->mPhysisBoxList.end();j++)
					{
						if ((*j)->mName==tempObj->mName)
						{
							scene->SceneNode->removeChild(tempObj->mName);
							deletePtr(*j);  
							scene->mPhysisBoxList.remove(*j);
							scene->selectSceneObj.pop_back();
							CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
							pMain->m_wndSceneObjView.FillSceneTree(); 
							scene->selectSceneObj.push_back(Newobj);
							break;
						}
					}
					InitPropList(Newobj);
				}
			}
		} //scene!=NULL//
		return 0;
	}
	else if (PropName==CString(_T("����")))
	{
		float val=value.fltVal;
		((SceneMeshObj*)tempObj)->mWeight=val;
		if (((SceneMeshObj*)tempObj)->mPhysicsBox!=NULL)
		{
			SceneMeshObj* obj=(SceneMeshObj*)tempObj;
			PhysicsBox* pBox=(PhysicsBox*)(obj->mPhysicsBox);
			pBox->InitBody();
		}
		return 0;
	}
	else if (PropName==CString(_T("�Զ�ģ��")))
	{
		CString str=value.bstrVal;     
		if (!str.IsEmpty())
		{
			if (str==CString(_T("��")))
			{
				((SceneMeshObj*)tempObj)->setAutoSimulate(true);
			}
			if (str==CString(_T("��")))
			{
				((SceneMeshObj*)tempObj)->setAutoSimulate(false);
			}
		}
	}
	else if (PropName==CString(_T("������")))
	{
		CMFCPropertyGridColorProperty* p=(CMFCPropertyGridColorProperty*)pProp;
		COLORREF color=p->GetColor();
		float r,g,b;
		r=GetRValue(color);
		g=GetGValue(color);
		b=GetBValue(color);
		SceneLightObj* lightobj=(SceneLightObj*)tempObj;
		lightobj->mdiffuseColor=Ogre::ColourValue(r/255,g/255,b/255);
		lightobj->mLight->setDiffuseColour(lightobj->mdiffuseColor);
		return 0;
	}
	else if (PropName==CString(_T("���淴��")))
	{
		CMFCPropertyGridColorProperty* p=(CMFCPropertyGridColorProperty*)pProp;
		COLORREF color=p->GetColor();
		float r,g,b;
		r=GetRValue(color);
		g=GetGValue(color);
		b=GetBValue(color);
		SceneLightObj* lightobj=(SceneLightObj*)tempObj;
		lightobj->mspecularColor=Ogre::ColourValue(r/255,g/255,b/255);
		lightobj->mLight->setSpecularColour(lightobj->mspecularColor);
		return 0;
	}
	else if (PropName==CString(_T("Ͷ����Ӱ")))
	{
		CString str=value.bstrVal;     
		if (!str.IsEmpty())
		{
			if (str==CString(_T("��")))
			{
				((SceneLightObj*)tempObj)->bCastShadow=true;
				((SceneLightObj*)tempObj)->mLight->setCastShadows(true);
				InitPropList(tempObj);
			}
			if (str==CString(_T("��")))
			{
				((SceneLightObj*)tempObj)->bCastShadow=false;
				((SceneLightObj*)tempObj)->mLight->setCastShadows(false);
				InitPropList(tempObj);
			}
		}
		return 0;
	}
	else if (PropName==CString(_T("��Ӱ��󳤶�")))
	{
		int val=value.intVal;
		((SceneLightObj*)tempObj)->farClipDistance=val;
		((SceneLightObj*)tempObj)->mLight->setShadowFarClipDistance(val);
	}
	else if (PropName==CString(_T("��Ӱ��ʼ����")))
	{
		int val=value.intVal;
		((SceneLightObj*)tempObj)->nearClipDistance=val;
		((SceneLightObj*)tempObj)->mLight->setShadowNearClipDistance(val);
	}
	else if (PropName==CString(_T("������Ӱ����")))
	{
		int val=value.intVal;
		((SceneLightObj*)tempObj)->shadowFarDistance=val;
		((SceneLightObj*)tempObj)->mLight->setShadowFarDistance(val);
	}
	else if (PropName==CString(_T("��ԶӰ�����")))
	{
		SceneLightObj* temp=(SceneLightObj*)tempObj;
		int val=value.intVal;
		temp->influenceDistance=val;
		temp->mLight->setAttenuation(val,temp->constParam,temp->lineParam,temp->quadParam);
	}
	else if (PropName==CString(_T("����ϵ��")))
	{
		SceneLightObj* temp=(SceneLightObj*)tempObj;
		float val=value.fltVal;
		temp->constParam=val;
		temp->mLight->setAttenuation(temp->influenceDistance,temp->constParam,temp->lineParam,temp->quadParam);
	}
	else if (PropName==CString(_T("һ�α���ϵ��")))
	{
		SceneLightObj* temp=(SceneLightObj*)tempObj;
		float val=value.fltVal;
		temp->lineParam=val;
		temp->mLight->setAttenuation(temp->influenceDistance,temp->constParam,temp->lineParam,temp->quadParam);
	}
	else if (PropName==CString(_T("���α���ϵ��")))
	{
		SceneLightObj* temp=(SceneLightObj*)tempObj;
		float val=value.fltVal;
		temp->quadParam=val;
		temp->mLight->setAttenuation(temp->influenceDistance,temp->constParam,temp->lineParam,temp->quadParam);
	}
	return 0;
}

void ObjPropertiesWnd::InitSkyProp(int type)
{
	mscene=BaseApplication::getInstance()->mscene;
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("����"),  _T("���"), _T("���"));
	CMFCPropertyGridProperty* pSkyClass = new CMFCPropertyGridProperty(_T("������"), _T(""), _T("��յ����࣬����ʹ�����ƽ�桢���񷶥���������Χ��"));
	pSkyClass->AddOption(_T("��"));   
	pSkyClass->AddOption(_T("ƽ��"));   
	pSkyClass->AddOption(_T("񷶥")); 
	pSkyClass->AddOption(_T("������"));

	pName->AllowEdit(false);
	pSkyClass->AllowEdit(false);
	CMFCPropertyGridColorProperty* pBackColor = new CMFCPropertyGridColorProperty(_T("����ɫ"), RGB(0, 125, 200), NULL, _T("ָ��������Ĭ�ϱ���ɫ"));
	pBackColor->EnableOtherButton(_T("����..."));
	pBackColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pSkyClass);
	m_wndPropList.AddProperty(pBackColor);

	switch(type)
	{
	case 0: 
		pSkyClass->SetValue(_T("��"));
		pBackColor->SetColor(RGB(mscene->mySky.backColor[0],mscene->mySky.backColor[1],mscene->mySky.backColor[2]));
		break;
	case 1:  //�����//
		{
			pSkyClass->SetValue(_T("ƽ��"));
			pBackColor->SetColor(RGB(166,202,240));
			CMFCPropertyGridProperty* pMaterail = new CMFCPropertyGridProperty(_T("��ղ���"), (_variant_t)mscene->mySky.MateralName.c_str(),_T("������յĲ���,����ʹ��ϵͳ�Դ����Լ������"));
			Ogre::ResourceManager::ResourceMapIterator matIter=Ogre::MaterialManager::getSingleton().getResourceIterator();
			while (matIter.hasMoreElements())
			{
				Ogre::ResourcePtr ptr=matIter.peekNextValue();
				Ogre::String matName=ptr->getName();
				Ogre::String subName=matName.substr(0,8);
				if (subName=="PlaneSky")
				{
					pMaterail->AddOption(CString(matName.c_str()));

				}
				matIter.moveNext();
			}
			CMFCPropertyGridProperty* psize = new CMFCPropertyGridProperty(_T("�ߴ�"), (_variant_t)mscene->mySky.size, _T("�����������ĳߴ磬��λ��"));
			CMFCPropertyGridProperty* pHight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)mscene->mySky.hight, _T("��վ����صľ���"));
			CMFCPropertyGridProperty* pXcut = new CMFCPropertyGridProperty(_T("x�ֶ�"), (_variant_t)mscene->mySky.xcut, _T("�������x�᷽�����Ƭ������Ƭ��Խ��ģ������Խ�࣬����Խƽ�������ܿ���Խ��"));
			CMFCPropertyGridProperty* pZcut = new CMFCPropertyGridProperty(_T("z�ֶ�"), (_variant_t)mscene->mySky.ycut, _T("�������z�᷽�����Ƭ������Ƭ��Խ��ģ������Խ�࣬����Խƽ�������ܿ���Խ��"));	
			CMFCPropertyGridProperty* pCurvity = new CMFCPropertyGridProperty(_T("����"), (_variant_t)mscene->mySky.pCurvity, _T("��յ�������"));
			CMFCPropertyGridProperty* pTiles = new CMFCPropertyGridProperty(_T("�����ظ���"), (_variant_t)mscene->mySky.tiles,_T("�����ظ��Ĵ���Խ��Ч��Խ���������ܿ���Խ��"));
			m_wndPropList.AddProperty(pMaterail);
			m_wndPropList.AddProperty(pHight);
			m_wndPropList.AddProperty(pXcut);
			m_wndPropList.AddProperty(pZcut);
			m_wndPropList.AddProperty(pCurvity);
			m_wndPropList.AddProperty(pTiles);
			m_wndPropList.AddProperty(psize);
		}	
		break;
	case 2:  //��պ�//
		{
			pSkyClass->SetValue(_T("񷶥"));
			CMFCPropertyGridProperty* pMaterail = new CMFCPropertyGridProperty(_T("��ղ���"), (_variant_t)mscene->mySky.MateralName.c_str(),_T("������յĲ���,����ʹ��ϵͳ�Դ����Լ������"));
			Ogre::ResourceManager::ResourceMapIterator matIter=Ogre::MaterialManager::getSingleton().getResourceIterator();
			while (matIter.hasMoreElements())
			{
				Ogre::ResourcePtr ptr=matIter.peekNextValue();
				Ogre::String matName=ptr->getName();
				Ogre::String subName=matName.substr(0,7);
				if (subName=="DomeSky")
				{
					pMaterail->AddOption(CString(matName.c_str()));

				}
				matIter.moveNext();
			}
			CMFCPropertyGridProperty* pHight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)mscene->mySky.hight, _T("��վ����صľ���"));
			CMFCPropertyGridProperty* pXcut = new CMFCPropertyGridProperty(_T("x�ֶ�"), (_variant_t)mscene->mySky.xcut, _T("�������x�᷽�����Ƭ������Ƭ��Խ��ģ������Խ�࣬����Խƽ�������ܿ���Խ��"));
			CMFCPropertyGridProperty* pZcut = new CMFCPropertyGridProperty(_T("z�ֶ�"), (_variant_t)mscene->mySky.ycut, _T("�������z�᷽�����Ƭ������Ƭ��Խ��ģ������Խ�࣬����Խƽ�������ܿ���Խ��"));	
			CMFCPropertyGridProperty* pCurvity = new CMFCPropertyGridProperty(_T("����"), (_variant_t)mscene->mySky.pCurvity, _T("��յ�������"));
			CMFCPropertyGridProperty* pTiles = new CMFCPropertyGridProperty(_T("�����ظ���"), (_variant_t)mscene->mySky.tiles,_T("�����ظ��Ĵ���Խ��Ч��Խ���������ܿ���Խ��"));
			m_wndPropList.AddProperty(pMaterail);
			m_wndPropList.AddProperty(pHight);
			m_wndPropList.AddProperty(pXcut);
			m_wndPropList.AddProperty(pZcut);
			m_wndPropList.AddProperty(pCurvity);
			m_wndPropList.AddProperty(pTiles);
		}	
		break;
	case 3:
		{
			pSkyClass->SetValue(_T("������"));
			CMFCPropertyGridProperty* pMaterail = new CMFCPropertyGridProperty(_T("��ղ���"), (_variant_t)mscene->mySky.MateralName.c_str(),_T("������յĲ���,����ʹ��ϵͳ�Դ����Լ������"));
			Ogre::ResourceManager::ResourceMapIterator matIter=Ogre::MaterialManager::getSingleton().getResourceIterator();
			while (matIter.hasMoreElements())
			{
				Ogre::ResourcePtr ptr=matIter.peekNextValue();
				Ogre::String matName=ptr->getName();
				Ogre::String subName=matName.substr(0,6);
				if (subName=="BoxSky")
				{
					pMaterail->AddOption(CString(matName.c_str()));

				}
				matIter.moveNext();
			}
			CMFCPropertyGridProperty* pHight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)mscene->mySky.hight, _T("��վ����صľ���"));
			m_wndPropList.AddProperty(pMaterail);
			m_wndPropList.AddProperty(pHight);
		}	
		break;
	}
}

void ObjPropertiesWnd::InitFogProp( int type )
{
	mscene=BaseApplication::getInstance()->mscene;
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("����"),  _T("��Ч"), _T("��Ч����"));
	CMFCPropertyGridProperty* pFogClass = new CMFCPropertyGridProperty(_T("�������"), _T(""), _T("������࣬����ʹ�����Ա仯�����仯���ȣ����߱仯�����𽥼��ָ���仯����ԽԶԽ�ܼ��仯�ٶȿ�"));
	pFogClass->AddOption(_T("��"));   
	pFogClass->AddOption(_T("������"));   
	pFogClass->AddOption(_T("������")); 
	pFogClass->AddOption(_T("ָ����"));
	pName->AllowEdit(false);
	pFogClass->AllowEdit(false);
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pFogClass);
	if (mscene->myFog.type==0)
	{
		pFogClass->SetValue(_T("��"));
		return;
	}
	else
	{

		CMFCPropertyGridColorProperty* pFogColor = new CMFCPropertyGridColorProperty(_T("�����ɫ"), RGB(0, 125, 200), NULL, _T("ָ��������Ĭ�ϱ���ɫ"));
		pFogColor->EnableOtherButton(_T("����..."));
		pFogColor->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
		pFogColor->SetColor(RGB(mscene->myFog.fogColor[0],mscene->myFog.fogColor[1],mscene->myFog.fogColor[2]));
		CMFCPropertyGridProperty*  pDensity= new CMFCPropertyGridProperty(_T("�ܶ�"), (_variant_t)mscene->myFog.density, _T("����ܶȣ�����С��0.01"));
		CMFCPropertyGridProperty* pStartDistance = new CMFCPropertyGridProperty(_T("��ʼ����"), (_variant_t)mscene->myFog.startDistance, _T("�������Ŀ�ʼ����"));
		CMFCPropertyGridProperty* pEndDistance = new CMFCPropertyGridProperty(_T("��������"), (_variant_t)mscene->myFog.endDistance, _T("�������Ľ�������"));
		m_wndPropList.AddProperty(pFogColor);
		m_wndPropList.AddProperty(pDensity);
		m_wndPropList.AddProperty(pStartDistance);
		m_wndPropList.AddProperty(pEndDistance);
	
		if (mscene->myFog.type==1)	{pFogClass->SetValue(_T("������"));}
		else if (mscene->myFog.type==2) {pFogClass->SetValue(_T("������")); }
		else if (mscene->myFog.type==3) {pFogClass->SetValue(_T("ָ����")); }

	}
	
}

void ObjPropertiesWnd::InitMeshObjProp()
{
	SceneMeshObj* meshObj=(SceneMeshObj*)tempObj;

	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("����"), (_variant_t)meshObj->mName.c_str(), _T("�������������"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("��Դ"), (_variant_t)meshObj->mResName.c_str(), _T("���������ļ���Դ"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("����:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)meshObj->mPostion.x, _T("x����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)meshObj->mPostion.y, _T("y����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)meshObj->mPostion.z, _T("z����"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("����:"));
	pProp = new CMFCPropertyGridProperty(_T("ScaleX:"),(_variant_t)meshObj->mScale.x, _T("x��������"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleY:"),(_variant_t)meshObj->mScale.y, _T("y��������"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleZ:"),(_variant_t)meshObj->mScale.z, _T("z��������"));
	pScale->AddSubItem(pProp);
	pScale->AllowEdit(true);
	m_wndPropList.AddProperty(pScale);

	CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("��ת:"));
	pProp = new CMFCPropertyGridProperty(_T("��x�ᣨ�ȣ�"),(_variant_t)meshObj->mRotaion.x, _T("��x����ת"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("��y�ᣨ�ȣ�"),(_variant_t)meshObj->mRotaion.y, _T("��y����ת"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("��z�ᣨ�ȣ�"),(_variant_t)meshObj->mRotaion.z, _T("��z����ת"));
	pRotation->AddSubItem(pProp);
	pRotation->AllowEdit(true);
	m_wndPropList.AddProperty(pRotation);


	CMFCPropertyGridProperty* CameraRotation= new CMFCPropertyGridProperty(_T("�����ת:"));
	Ogre::Vector3 v3=Ogre::Vector3::NEGATIVE_UNIT_Z;
	Ogre::Vector3 camDirection=mscene->mCamera->getDerivedDirection();
	camDirection.y=0;
	Ogre::Degree de=v3.angleBetween(Vector3(camDirection.x,0,camDirection.z));   //�������ͱ�ѡ�����ˮƽ��ת�Ƕ�,�����Զ�ת�Ƕ�//	
	float devalue=de.valueDegrees();
	if (camDirection.x>0)
	{
		devalue=360-devalue;
	}
	pProp = new CMFCPropertyGridProperty(_T("ˮƽ���ȣ�"),(_variant_t)devalue, _T("ˮƽ������ת"));
	CameraRotation->AddSubItem(pProp);
	m_wndPropList.AddProperty(CameraRotation);


	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("��ײ��"));
	if (meshObj->mPhysicsBox==NULL){pProp = new CMFCPropertyGridProperty(_T("��ײ������"), _T("��"),_T("��ģ�͹�������ײ������ͣ��������塢���塢Բ���塢���ҵ�"));}
	else if (meshObj->mPhysicsBox!=NULL)
	{
		PhysicsBox* pBox=(PhysicsBox*)meshObj->mPhysicsBox;
		if (pBox->mType==TypeBox)
		{
			pProp = new CMFCPropertyGridProperty(_T("��ײ������"), _T("������"),_T("��ģ�͹�������ײ������ͣ��������塢���塢Բ���塢���ҵ�"));
		}
		if (pBox->mType==TypeSphere)
		{
			pProp = new CMFCPropertyGridProperty(_T("��ײ������"), _T("����"),_T("��ģ�͹�������ײ������ͣ��������塢���塢Բ���塢���ҵ�"));
		}
		if (pBox->mType==TypeCylinder)
		{
			pProp = new CMFCPropertyGridProperty(_T("��ײ������"), _T("Բ����"),_T("��ģ�͹�������ײ������ͣ��������塢���塢Բ���塢���ҵ�"));
		}
		if (pBox->mType==TypeCapsule)
		{
			pProp = new CMFCPropertyGridProperty(_T("��ײ������"), _T("������"),_T("��ģ�͹�������ײ������ͣ��������塢���塢Բ���塢���ҵ�"));
		}
		if (pBox->mType==TypePlane)
		{
		}
	}
	pProp->AddOption(_T("��"));
	pProp->AddOption(_T("������"));
	pProp->AddOption(_T("����"));
	pProp->AddOption(_T("Բ����"));
	pProp->AddOption(_T("������"));
	pProp->AddOption(_T("ƽ��"));
	pGroup3->AddSubItem(pProp);
	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("��������"));
	if (meshObj->bAutoSimulate && meshObj->mPhysicsBox!=NULL)
	{
		pProp = new CMFCPropertyGridProperty(_T("�Զ�ģ��"), _T("��"),_T("�Ƿ�������ģ�⣬��������ײ���������ʹ��"));
	}
	else
	{
		pProp = new CMFCPropertyGridProperty(_T("�Զ�ģ��"), _T("��"),_T("�Ƿ�������ģ�⣬��������ײ���������ʹ��"));
	}
	pProp->AddOption(_T("��"));
	pProp->AddOption(_T("��"));
	pProp->AllowEdit(false);
	pGroup4->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("����"),(_variant_t)meshObj->mWeight,_T("�������������������������ʵ��Ĳ������������ģ���쳣"));
	pGroup4->AddSubItem(pProp);
	if (meshObj->mPhysicsBox==NULL)
	{
		pGroup4->Enable(false);
	}
	pGroup4->Expand(true);
	m_wndPropList.AddProperty(pGroup4);	

}

void ObjPropertiesWnd::InitSoundObjProp()
{
	SceneSoundObj* soundObj=(SceneSoundObj*)tempObj;
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("����"), (_variant_t)soundObj->mName.c_str(), _T("�������������"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("��Դ"), (_variant_t)soundObj->mResName.c_str(), _T("���������ļ���Դ"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("����:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)soundObj->mPostion.x, _T("x����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)soundObj->mPostion.y, _T("y����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)soundObj->mPostion.z, _T("z����"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pPriority = new CMFCPropertyGridProperty(_T("���ȼ�"), (_variant_t)soundObj->mPriority, _T("�����������ȼ���ֵԽС���ȼ�Խ�󣬵���������ʱ���ȼ���С����������ֹͣ"));
	pPriority->EnableSpinControl(TRUE,0,256);
	m_wndPropList.AddProperty(pPriority);
	CMFCPropertyGridProperty* pLoop=NULL;
	if (soundObj->bLoop){ pLoop = new CMFCPropertyGridProperty(_T("ѭ������"), _T("��"), _T("ѭ�����Ÿ�����"));}
	else{ pLoop = new CMFCPropertyGridProperty(_T("ѭ������"), _T("��"), _T("ѭ�����Ÿ�����")); }
	pLoop->AddOption(_T("��"));
	pLoop->AddOption(_T("��"));
	m_wndPropList.AddProperty(pLoop);

	CMFCPropertyGridProperty* pValume = new CMFCPropertyGridProperty(_T("����"),(_variant_t)(int)(soundObj->mValume*10), _T("��������������ȡֵ��Χ0-10"));
	pValume->EnableSpinControl(TRUE,0,10);
	m_wndPropList.AddProperty(pValume);

	CMFCPropertyGridProperty* pBingObj = NULL;
	if (soundObj->mBingObj==NULL)
	{
		pBingObj = new CMFCPropertyGridProperty(_T("������"),_T("��"), _T("��������һ����������󶨣�������λ��������λ�õĸı���ı�"));
	}
	else
	{
		SceneMeshObj* Obj=(SceneMeshObj*)soundObj->mBingObj;
		pBingObj = new CMFCPropertyGridProperty(_T("������"), (_variant_t)Obj->mName.c_str(),_T("��������һ����������󶨣�������λ��������λ�õĸı���ı�"));
	}
	pBingObj->AddOption(_T("��"));
	for (std::list<SceneMeshObj*>::iterator i=mscene->mMeshObjList.begin();i!=mscene->mMeshObjList.end();i++)
	{
		pBingObj->AddOption(CString((*i)->mName.c_str()));
	}
	m_wndPropList.AddProperty(pBingObj);
}

void ObjPropertiesWnd::InitPhysicsBoxProp()
{
	PhysicsBox* PBoxObj=(PhysicsBox*)tempObj;
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("����"), (_variant_t)CString(PBoxObj->mName.c_str()), _T("�������������"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("��Դ"), (_variant_t)CString(PBoxObj->mResName.c_str()), _T("���������ļ���Դ"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("����:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)PBoxObj->mPostion.x, _T("x����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)PBoxObj->mPostion.y, _T("y����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)PBoxObj->mPostion.z, _T("z����"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("����:"));
	pProp = new CMFCPropertyGridProperty(_T("ScaleX:"),(_variant_t)PBoxObj->mScale.x, _T("x��������"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleY:"),(_variant_t)PBoxObj->mScale.y, _T("y��������"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleZ:"),(_variant_t)PBoxObj->mScale.z, _T("z��������"));
	pScale->AddSubItem(pProp);
	pScale->AllowEdit(true);
	m_wndPropList.AddProperty(pScale);

	CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("��ת:"));
	pProp = new CMFCPropertyGridProperty(_T("��x�ᣨ�ȣ�"),(_variant_t)PBoxObj->mRotaion.x, _T("��x����ת"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("��y�ᣨ�ȣ�"),(_variant_t)PBoxObj->mRotaion.y, _T("��y����ת"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("��z�ᣨ�ȣ�"),(_variant_t)PBoxObj->mRotaion.z, _T("��z����ת"));
	pRotation->AddSubItem(pProp);
	pRotation->AllowEdit(true);
	m_wndPropList.AddProperty(pRotation);

	pProp = new CMFCPropertyGridProperty(_T("��ײ������"), _T("������"),_T("��ģ�͹�������ײ������ͣ��������塢���塢Բ���塢���ҵ�"));
	pProp->AddOption(_T("������"));
	pProp->AddOption(_T("����"));
	pProp->AddOption(_T("Բ����"));
	pProp->AddOption(_T("������"));
	pProp->AddOption(_T("ƽ��"));
	if (PBoxObj->mType==TypeBox){	pProp->SetValue(_T("������"));}
	else if (PBoxObj->mType==TypeSphere){ pProp->SetValue(_T("����"));}
	else if (PBoxObj->mType==TypeCylinder){pProp->SetValue(_T("Բ����"));}
	else if (PBoxObj->mType==TypeCapsule){pProp->SetValue(_T("����")); }
	else if (PBoxObj->mType==TypePlane)
	{
	}
	m_wndPropList.AddProperty(pProp);

	pProp = new CMFCPropertyGridProperty(_T("������"), _T("��"),_T("�����ײ��󶨵�����"));
	if (PBoxObj->mMeshObj!=NULL)
	{
		pProp->SetValue(CString(PBoxObj->mMeshObj->mName.c_str()));
	}
	pProp->AllowEdit(false);
	m_wndPropList.AddProperty(pProp);
}

void ObjPropertiesWnd::InitLightObjProp()
{
	SceneLightObj* lightObj=(SceneLightObj*)tempObj;
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("����"), (_variant_t)lightObj->mName.c_str(), _T("�������������"));
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("����"), (_variant_t)lightObj->mResName.c_str(), _T("��Դ�����ͣ��ֵ��Դ���۹�ơ�ƽ�й⣨ģ������Զ�����̫���⣬ֻ�з������Զ�����Ч��"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pProp);
	pName->AllowEdit(false);
	pProp->AllowEdit(false);

	CMFCPropertyGridColorProperty*	pColorProp = new CMFCPropertyGridColorProperty(_T("������"), RGB(0, 125, 200), NULL, _T("�ƹ����������ɫ"));
	pColorProp->SetColor(RGB(lightObj->mdiffuseColor.r * 255,lightObj->mdiffuseColor.g * 255, lightObj->mdiffuseColor.b * 255));
	pColorProp->EnableOtherButton(_T("����..."));
	pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	m_wndPropList.AddProperty(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("���淴��"), RGB(0, 125, 200), NULL, _T("�ƹ�ľ��淴����ɫ"));
	pColorProp->SetColor(RGB(lightObj->mspecularColor.r * 255,lightObj->mspecularColor.g * 255, lightObj->mspecularColor.b * 255));
	pColorProp->EnableOtherButton(_T("����..."));
	pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
	m_wndPropList.AddProperty(pColorProp);

	if (lightObj->mType==LightPoint||lightObj->mType==LightSpot)
	{
		CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("����:"));
		pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)lightObj->mPostion.x, _T("x����"));
		pPosition->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)lightObj->mPostion.y, _T("y����"));
		pPosition->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)lightObj->mPostion.z, _T("z����"));
		pPosition->AddSubItem(pProp);;
		pPosition->AllowEdit(true);
		m_wndPropList.AddProperty(pPosition);
	}

	if (lightObj->mType==Lightdirection||lightObj->mType==LightSpot)
	{
		CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("��ת:"));
		pProp = new CMFCPropertyGridProperty(_T("��x�ᣨ�ȣ�"),(_variant_t)lightObj->mRotaion.x, _T("��x����ת"));
		pRotation->AddSubItem(pProp);
		pProp = new CMFCPropertyGridProperty(_T("��y�ᣨ�ȣ�"),(_variant_t)lightObj->mRotaion.y, _T("��y����ת"));
		pRotation->AddSubItem(pProp);
		pProp = new CMFCPropertyGridProperty(_T("��z�ᣨ�ȣ�"),(_variant_t)lightObj->mRotaion.z, _T("��z����ת"));
		pRotation->AddSubItem(pProp);
		pRotation->AllowEdit(true);
		m_wndPropList.AddProperty(pRotation);
	}
	

	CMFCPropertyGridProperty* pCastShadow = new CMFCPropertyGridProperty(_T("Ͷ����Ӱ"), _T("��"), _T("�õƹ��Ƿ�Ͷ����Ӱ������Ͷ����Ӱ�ĵƹⲻҪ����"));
	pCastShadow->AddOption(_T("��"));
	pCastShadow->AddOption(_T("��"));
	m_wndPropList.AddProperty(pCastShadow);
	if (lightObj->bCastShadow)
	{
		pCastShadow->SetValue(_T("��"));
		CMFCPropertyGridProperty* pShadow = new CMFCPropertyGridProperty(_T("��Ӱ:"));
		pProp= new CMFCPropertyGridProperty(_T("��Ӱ��󳤶�"),(_variant_t)lightObj->farClipDistance, _T("�ƹ��������Ӱ����󳤶ȣ����ƹ����������ǳ���ʱ���������Զ����Ӱ����ʱ��������ɽϴ�Ӱ�죬�������Ӱ�ü���������ñ���ƹ�������������"));
		pProp->EnableSpinControl(true,1,1000);
		pShadow->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("��Ӱ��ʼ����"),(_variant_t)lightObj->nearClipDistance, _T("��Ӱ������������ľ��룬һ������Ϊ0"));
		pProp->EnableSpinControl(true,1,500);
		pShadow->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("������Ӱ����"),(_variant_t)lightObj->shadowFarDistance, _T("�ƹ��ܹ�������Ӱ�ľ��룬���ƹ�������ľ�����ڴ�ֵʱ��������Ӱ"));
		pProp->EnableSpinControl(true,1,1500);
		pShadow->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("��ԶӰ�����"),(_variant_t)lightObj->influenceDistance, _T("�ƹ��ܹ�����Ӱ�����Զ���룬ͨ������ƹⷢ���ԽԶ���ƹ�ǿ��˥����Խ����"));
		pProp->EnableSpinControl(true,1,2000);
		pShadow->AddSubItem(pProp);
		m_wndPropList.AddProperty(pShadow);
	}
	else
	{
		pCastShadow->SetValue(_T("��"));
	}

	if (lightObj->mType==LightPoint||lightObj->mType==LightSpot)
	{
		CMFCPropertyGridProperty* pAttenuation = new CMFCPropertyGridProperty(_T("˥������:"));
		pAttenuation->SetDescription(_T("����ǿ�������뷢���ľ���˥����˥��������ϵ�����Ե���"));
		pProp= new CMFCPropertyGridProperty(_T("����ϵ��"),(_variant_t)lightObj->constParam, _T("˥�������ĳ�������"));
		pAttenuation->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("һ�α���ϵ��"),(_variant_t)lightObj->lineParam, _T("˥��������һ�α���ǰ�Ĳ�����������α�������Ϊ�㣬�����Ա仯"));
		pAttenuation->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("���α���ϵ��"),(_variant_t)lightObj->quadParam, _T("˥�������Ķ��α���ǰ�Ĳ����������Ϊ0����2�����߱仯"));
		pAttenuation->AddSubItem(pProp);
		m_wndPropList.AddProperty(pAttenuation);
	}
	
}

void ObjPropertiesWnd::InitParticleObjProp()
{
	SceneParticleObj* particleObj=(SceneParticleObj*)tempObj;
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("����"), (_variant_t)particleObj->mName.c_str(), _T("�������������"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("��Դ"), (_variant_t)particleObj->mResName.c_str(), _T("���������ļ���Դ"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("����:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)particleObj->mPostion.x, _T("x����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)particleObj->mPostion.y, _T("y����"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)particleObj->mPostion.z, _T("z����"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("��ת:"));
	pProp = new CMFCPropertyGridProperty(_T("��x�ᣨ�ȣ�"),(_variant_t)particleObj->mRotaion.x, _T("��x����ת"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("��y�ᣨ�ȣ�"),(_variant_t)particleObj->mRotaion.y, _T("��y����ת"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("��z�ᣨ�ȣ�"),(_variant_t)particleObj->mRotaion.z, _T("��z����ת"));
	pRotation->AddSubItem(pProp);
	pRotation->AllowEdit(true);
	m_wndPropList.AddProperty(pRotation);
}

void ObjPropertiesWnd::OnSkyandFogPropertyChanged( CMFCPropertyGridProperty* pProp )
{
	mscene=NULL;
	mscene=BaseApplication::getInstance()->mscene;
	if (mscene!=NULL)
	{
		CString PropName=pProp->GetName();  //���ı�Ĳ�����//
		COleVariant value = pProp->GetValue(); //�ı�֮���ֵ//
		if (PropName==CString(_T("������")))
		{
			CString str=value.bstrVal;     
			if (!str.IsEmpty())
			{
				if (str==CString(_T("��"))){mscene->mySky.type=0;}
				else if (str==CString(_T("ƽ��"))){mscene->mySky.type=1;}
				else if (str==CString(_T("񷶥"))){mscene->mySky.type=2;}
				else if (str==CString(_T("������"))){mscene->mySky.type=3;}
			}
			mscene->initSky(mscene->mySky.type);
			InitSkyProp(mscene->mySky.type);
			return;
		}
		else if (PropName==CString(_T("����ɫ")))
		{
			CMFCPropertyGridColorProperty* p=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=p->GetColor();
			mscene->mySky.backColor[0]=GetRValue(color);
			mscene->mySky.backColor[1]=GetGValue(color);
			mscene->mySky.backColor[2]=GetBValue(color);
			mscene->setSky(mscene->mySky.type);
			return;
		}
		else if (PropName==CString(_T("��ղ���")))
		{
			if (mscene->mySky.type!=0)
			{
				CString str=value.bstrVal;     
				if (!str.IsEmpty())
				{
					USES_CONVERSION;
					std::string str1 = W2A(str.GetBuffer());
					Ogre::String materialName(str1);
					mscene->mySky.MateralName=materialName;
					mscene->setSky(mscene->mySky.type);	
				}
			}
			return;
		}
		else if (PropName==CString(_T("�ߴ�")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.size=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("�߶�")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.hight=value.fltVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("x�ֶ�")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.xcut=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("z�ֶ�")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.ycut=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("����")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.pCurvity=value.fltVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("�����ظ���")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.tiles=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("�������")))   //�������//
		{
			CString str=value.bstrVal;     
			if (!str.IsEmpty())
			{
				if (str==CString(_T("��"))){mscene->myFog.type=0;}
				else if (str==CString(_T("������"))){mscene->myFog.type=1;}
				else if (str==CString(_T("������"))){mscene->myFog.type=2;}
				else if (str==CString(_T("ָ����"))){mscene->myFog.type=3;}
			}
			mscene->initFog(mscene->myFog.type);
			InitFogProp(mscene->myFog.type);
			return;
		}
		else if (PropName==CString(_T("�����ɫ")))
		{
			CMFCPropertyGridColorProperty* p=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=p->GetColor();
			mscene->myFog.fogColor[0]=GetRValue(color);
			mscene->myFog.fogColor[1]=GetGValue(color);
			mscene->myFog.fogColor[2]=GetBValue(color);
			mscene->setFog();
			return;
		}
		else if (PropName==CString(_T("�ܶ�")))
		{
			mscene->myFog.density=value.intVal;
			mscene->setFog();
			return;
		}
		else if (PropName==CString(_T("��ʼ����")))
		{
			mscene->myFog.startDistance=value.intVal;
			mscene->setFog();
			return;
		}
		else if (PropName==CString(_T("��������")))
		{
			mscene->myFog.endDistance=value.intVal;
			mscene->setFog();
			return;
		}


	}

}
