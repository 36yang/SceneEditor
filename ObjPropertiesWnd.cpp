
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
	//ON_WM_NCACTIVATE()//失去焦点/////
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)       //处理属性变化事件
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
	m_wndPropList.RemoveAll();   //删除所有属性//
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
		TRACE0("未能创建属性网格\n");
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
	CString PropName=pProp->GetName();  //被改变的参数名//
	COleVariant value = pProp->GetValue(); //改变之后的值//
	if (PropName==CString(_T("名称")))
	{
		CString str;
		str=value.bstrVal;     //从COleVariant 到CString
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
	else if (PropName==CString(_T("绕x轴（度）")))
	{
		float x=value.fltVal;
		if (x>-36000&&x<36000)
		{
			tempObj->mRotaion.x=x;
			tempObj->setRotaion(tempObj->mRotaion);
		}
		return 0;
	}
	else if (PropName==CString(_T("绕y轴（度）")))
	{
		float y=value.fltVal;
		if (y>-36000&&y<36000)
		{
			tempObj->mRotaion.y=y;
			tempObj->setRotaion(tempObj->mRotaion);
		}
		return 0;
	}
	else if (PropName==CString(_T("绕z轴（度）")))
	{
		float z=value.fltVal;
		if (z>-36000&&z<36000)
		{
			tempObj->mRotaion.z=z;
			tempObj->setRotaion(tempObj->mRotaion);
		}
		return 0;
	}
	else if (PropName==CString(_T("优先级")))
	{
		int pri=value.intVal;
		((SceneSoundObj*)tempObj)->setPriority(pri);
		return 0;
	}
	else if (PropName==CString(_T("循环播放")))
	{
		CString str=value.bstrVal;     
		if (!str.IsEmpty())
		{
			if (str==CString(_T("是")))
			{	((SceneSoundObj*)tempObj)->setLoop(true);	}
			else if (str==CString(_T("否")))
			{	((SceneSoundObj*)tempObj)->setLoop(false);	}
		}	
		return 0;
	}
	else if (PropName==CString(_T("音量")))
	{
		int val=value.intVal;
		((SceneSoundObj*)tempObj)->setValume((float)val/10);
		return 0;
	}
	else if (PropName==CString(_T("绑定物体")))
	{
		CString str=value.bstrVal; 
		if (str==CString(_T("无")))
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
	else if (PropName==CString(_T("碰撞体类型")))
	{
		CString str=value.bstrVal;     
		EditScene *scene=NULL;
		scene=BaseApplication::getInstance()->mscene;
		if (scene!=NULL)
		{
			if (!str.IsEmpty())
			{
				if (str==CString(_T("无")))
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
					if (str==CString(_T("立方体"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeBox,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("球体"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeSphere,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("圆柱体"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCylinder,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("胶囊体"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCapsule,scene->PhysicsBoxTag,obj->mMeshObj);}
					else if (str==CString(_T("平面"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypePlane,scene->PhysicsBoxTag,obj->mMeshObj);}	
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
					if (str==CString(_T("立方体"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeBox,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("球体"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeSphere,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("圆柱体"))){Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCylinder,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("胶囊体"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypeCapsule,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
					else if (str==CString(_T("平面"))){ Newobj=new PhysicsBox(scene->mOdeSceneSpace,TypePlane,scene->PhysicsBoxTag,((PhysicsBox*)tempObj)->mMeshObj);}
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
	else if (PropName==CString(_T("质量")))
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
	else if (PropName==CString(_T("自动模拟")))
	{
		CString str=value.bstrVal;     
		if (!str.IsEmpty())
		{
			if (str==CString(_T("开")))
			{
				((SceneMeshObj*)tempObj)->setAutoSimulate(true);
			}
			if (str==CString(_T("关")))
			{
				((SceneMeshObj*)tempObj)->setAutoSimulate(false);
			}
		}
	}
	else if (PropName==CString(_T("漫反射")))
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
	else if (PropName==CString(_T("镜面反射")))
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
	else if (PropName==CString(_T("投射阴影")))
	{
		CString str=value.bstrVal;     
		if (!str.IsEmpty())
		{
			if (str==CString(_T("是")))
			{
				((SceneLightObj*)tempObj)->bCastShadow=true;
				((SceneLightObj*)tempObj)->mLight->setCastShadows(true);
				InitPropList(tempObj);
			}
			if (str==CString(_T("否")))
			{
				((SceneLightObj*)tempObj)->bCastShadow=false;
				((SceneLightObj*)tempObj)->mLight->setCastShadows(false);
				InitPropList(tempObj);
			}
		}
		return 0;
	}
	else if (PropName==CString(_T("阴影最大长度")))
	{
		int val=value.intVal;
		((SceneLightObj*)tempObj)->farClipDistance=val;
		((SceneLightObj*)tempObj)->mLight->setShadowFarClipDistance(val);
	}
	else if (PropName==CString(_T("阴影开始距离")))
	{
		int val=value.intVal;
		((SceneLightObj*)tempObj)->nearClipDistance=val;
		((SceneLightObj*)tempObj)->mLight->setShadowNearClipDistance(val);
	}
	else if (PropName==CString(_T("产生阴影距离")))
	{
		int val=value.intVal;
		((SceneLightObj*)tempObj)->shadowFarDistance=val;
		((SceneLightObj*)tempObj)->mLight->setShadowFarDistance(val);
	}
	else if (PropName==CString(_T("最远影响距离")))
	{
		SceneLightObj* temp=(SceneLightObj*)tempObj;
		int val=value.intVal;
		temp->influenceDistance=val;
		temp->mLight->setAttenuation(val,temp->constParam,temp->lineParam,temp->quadParam);
	}
	else if (PropName==CString(_T("常量系数")))
	{
		SceneLightObj* temp=(SceneLightObj*)tempObj;
		float val=value.fltVal;
		temp->constParam=val;
		temp->mLight->setAttenuation(temp->influenceDistance,temp->constParam,temp->lineParam,temp->quadParam);
	}
	else if (PropName==CString(_T("一次变量系数")))
	{
		SceneLightObj* temp=(SceneLightObj*)tempObj;
		float val=value.fltVal;
		temp->lineParam=val;
		temp->mLight->setAttenuation(temp->influenceDistance,temp->constParam,temp->lineParam,temp->quadParam);
	}
	else if (PropName==CString(_T("二次变量系数")))
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

	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("名称"),  _T("天空"), _T("天空"));
	CMFCPropertyGridProperty* pSkyClass = new CMFCPropertyGridProperty(_T("天空类别"), _T(""), _T("天空的种类，可以使用天空平面、天空穹顶、立方体包围盒"));
	pSkyClass->AddOption(_T("无"));   
	pSkyClass->AddOption(_T("平面"));   
	pSkyClass->AddOption(_T("穹顶")); 
	pSkyClass->AddOption(_T("立方体"));

	pName->AllowEdit(false);
	pSkyClass->AllowEdit(false);
	CMFCPropertyGridColorProperty* pBackColor = new CMFCPropertyGridColorProperty(_T("背景色"), RGB(0, 125, 200), NULL, _T("指定场景的默认背景色"));
	pBackColor->EnableOtherButton(_T("其他..."));
	pBackColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pSkyClass);
	m_wndPropList.AddProperty(pBackColor);

	switch(type)
	{
	case 0: 
		pSkyClass->SetValue(_T("无"));
		pBackColor->SetColor(RGB(mscene->mySky.backColor[0],mscene->mySky.backColor[1],mscene->mySky.backColor[2]));
		break;
	case 1:  //天空面//
		{
			pSkyClass->SetValue(_T("平面"));
			pBackColor->SetColor(RGB(166,202,240));
			CMFCPropertyGridProperty* pMaterail = new CMFCPropertyGridProperty(_T("天空材质"), (_variant_t)mscene->mySky.MateralName.c_str(),_T("定义天空的材质,可以使用系统自带或自己定义的"));
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
			CMFCPropertyGridProperty* psize = new CMFCPropertyGridProperty(_T("尺寸"), (_variant_t)mscene->mySky.size, _T("正方形天空面的尺寸，单位米"));
			CMFCPropertyGridProperty* pHight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)mscene->mySky.hight, _T("天空距离大地的距离"));
			CMFCPropertyGridProperty* pXcut = new CMFCPropertyGridProperty(_T("x分段"), (_variant_t)mscene->mySky.xcut, _T("天空面在x轴方向的切片数，切片数越高模型面数越多，过度越平滑但性能开销越大"));
			CMFCPropertyGridProperty* pZcut = new CMFCPropertyGridProperty(_T("z分段"), (_variant_t)mscene->mySky.ycut, _T("天空面在z轴方向的切片数，切片数越高模型面数越多，过度越平滑但性能开销越大"));	
			CMFCPropertyGridProperty* pCurvity = new CMFCPropertyGridProperty(_T("曲率"), (_variant_t)mscene->mySky.pCurvity, _T("天空的弯曲度"));
			CMFCPropertyGridProperty* pTiles = new CMFCPropertyGridProperty(_T("纹理重复数"), (_variant_t)mscene->mySky.tiles,_T("纹理重复的次数越多效果越清晰但性能开销越大"));
			m_wndPropList.AddProperty(pMaterail);
			m_wndPropList.AddProperty(pHight);
			m_wndPropList.AddProperty(pXcut);
			m_wndPropList.AddProperty(pZcut);
			m_wndPropList.AddProperty(pCurvity);
			m_wndPropList.AddProperty(pTiles);
			m_wndPropList.AddProperty(psize);
		}	
		break;
	case 2:  //天空盒//
		{
			pSkyClass->SetValue(_T("穹顶"));
			CMFCPropertyGridProperty* pMaterail = new CMFCPropertyGridProperty(_T("天空材质"), (_variant_t)mscene->mySky.MateralName.c_str(),_T("定义天空的材质,可以使用系统自带或自己定义的"));
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
			CMFCPropertyGridProperty* pHight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)mscene->mySky.hight, _T("天空距离大地的距离"));
			CMFCPropertyGridProperty* pXcut = new CMFCPropertyGridProperty(_T("x分段"), (_variant_t)mscene->mySky.xcut, _T("天空面在x轴方向的切片数，切片数越高模型面数越多，过度越平滑但性能开销越大"));
			CMFCPropertyGridProperty* pZcut = new CMFCPropertyGridProperty(_T("z分段"), (_variant_t)mscene->mySky.ycut, _T("天空面在z轴方向的切片数，切片数越高模型面数越多，过度越平滑但性能开销越大"));	
			CMFCPropertyGridProperty* pCurvity = new CMFCPropertyGridProperty(_T("曲率"), (_variant_t)mscene->mySky.pCurvity, _T("天空的弯曲度"));
			CMFCPropertyGridProperty* pTiles = new CMFCPropertyGridProperty(_T("纹理重复数"), (_variant_t)mscene->mySky.tiles,_T("纹理重复的次数越多效果越清晰但性能开销越大"));
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
			pSkyClass->SetValue(_T("立方体"));
			CMFCPropertyGridProperty* pMaterail = new CMFCPropertyGridProperty(_T("天空材质"), (_variant_t)mscene->mySky.MateralName.c_str(),_T("定义天空的材质,可以使用系统自带或自己定义的"));
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
			CMFCPropertyGridProperty* pHight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)mscene->mySky.hight, _T("天空距离大地的距离"));
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

	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("名称"),  _T("雾效"), _T("雾效设置"));
	CMFCPropertyGridProperty* pFogClass = new CMFCPropertyGridProperty(_T("雾的类型"), _T(""), _T("雾的种类，可以使用线性变化的雾，变化均匀；曲线变化的雾，逐渐加深；指数变化的雾，越远越密集变化速度快"));
	pFogClass->AddOption(_T("无"));   
	pFogClass->AddOption(_T("线性雾"));   
	pFogClass->AddOption(_T("曲线雾")); 
	pFogClass->AddOption(_T("指数雾"));
	pName->AllowEdit(false);
	pFogClass->AllowEdit(false);
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pFogClass);
	if (mscene->myFog.type==0)
	{
		pFogClass->SetValue(_T("无"));
		return;
	}
	else
	{

		CMFCPropertyGridColorProperty* pFogColor = new CMFCPropertyGridColorProperty(_T("雾的颜色"), RGB(0, 125, 200), NULL, _T("指定场景的默认背景色"));
		pFogColor->EnableOtherButton(_T("其他..."));
		pFogColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
		pFogColor->SetColor(RGB(mscene->myFog.fogColor[0],mscene->myFog.fogColor[1],mscene->myFog.fogColor[2]));
		CMFCPropertyGridProperty*  pDensity= new CMFCPropertyGridProperty(_T("密度"), (_variant_t)mscene->myFog.density, _T("雾的密度，建议小于0.01"));
		CMFCPropertyGridProperty* pStartDistance = new CMFCPropertyGridProperty(_T("开始距离"), (_variant_t)mscene->myFog.startDistance, _T("雾距相机的开始距离"));
		CMFCPropertyGridProperty* pEndDistance = new CMFCPropertyGridProperty(_T("结束距离"), (_variant_t)mscene->myFog.endDistance, _T("雾距相机的结束距离"));
		m_wndPropList.AddProperty(pFogColor);
		m_wndPropList.AddProperty(pDensity);
		m_wndPropList.AddProperty(pStartDistance);
		m_wndPropList.AddProperty(pEndDistance);
	
		if (mscene->myFog.type==1)	{pFogClass->SetValue(_T("线性雾"));}
		else if (mscene->myFog.type==2) {pFogClass->SetValue(_T("曲线雾")); }
		else if (mscene->myFog.type==3) {pFogClass->SetValue(_T("指数雾")); }

	}
	
}

void ObjPropertiesWnd::InitMeshObjProp()
{
	SceneMeshObj* meshObj=(SceneMeshObj*)tempObj;

	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)meshObj->mName.c_str(), _T("场景物体的名字"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("资源"), (_variant_t)meshObj->mResName.c_str(), _T("物体所用文件资源"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("坐标:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)meshObj->mPostion.x, _T("x坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)meshObj->mPostion.y, _T("y坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)meshObj->mPostion.z, _T("z坐标"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("缩放:"));
	pProp = new CMFCPropertyGridProperty(_T("ScaleX:"),(_variant_t)meshObj->mScale.x, _T("x方向缩放"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleY:"),(_variant_t)meshObj->mScale.y, _T("y方向缩放"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleZ:"),(_variant_t)meshObj->mScale.z, _T("z方向缩放"));
	pScale->AddSubItem(pProp);
	pScale->AllowEdit(true);
	m_wndPropList.AddProperty(pScale);

	CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("旋转:"));
	pProp = new CMFCPropertyGridProperty(_T("绕x轴（度）"),(_variant_t)meshObj->mRotaion.x, _T("绕x轴旋转"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("绕y轴（度）"),(_variant_t)meshObj->mRotaion.y, _T("绕y轴旋转"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("绕z轴（度）"),(_variant_t)meshObj->mRotaion.z, _T("绕z轴旋转"));
	pRotation->AddSubItem(pProp);
	pRotation->AllowEdit(true);
	m_wndPropList.AddProperty(pRotation);


	CMFCPropertyGridProperty* CameraRotation= new CMFCPropertyGridProperty(_T("相机旋转:"));
	Ogre::Vector3 v3=Ogre::Vector3::NEGATIVE_UNIT_Z;
	Ogre::Vector3 camDirection=mscene->mCamera->getDerivedDirection();
	camDirection.y=0;
	Ogre::Degree de=v3.angleBetween(Vector3(camDirection.x,0,camDirection.z));   //算出相机和被选物体的水平旋转角度,弧度自动转角度//	
	float devalue=de.valueDegrees();
	if (camDirection.x>0)
	{
		devalue=360-devalue;
	}
	pProp = new CMFCPropertyGridProperty(_T("水平（度）"),(_variant_t)devalue, _T("水平方向旋转"));
	CameraRotation->AddSubItem(pProp);
	m_wndPropList.AddProperty(CameraRotation);


	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("碰撞体"));
	if (meshObj->mPhysicsBox==NULL){pProp = new CMFCPropertyGridProperty(_T("碰撞体类型"), _T("无"),_T("与模型关联的碰撞体的类型，有立方体、球体、圆柱体、胶囊等"));}
	else if (meshObj->mPhysicsBox!=NULL)
	{
		PhysicsBox* pBox=(PhysicsBox*)meshObj->mPhysicsBox;
		if (pBox->mType==TypeBox)
		{
			pProp = new CMFCPropertyGridProperty(_T("碰撞体类型"), _T("立方体"),_T("与模型关联的碰撞体的类型，有立方体、球体、圆柱体、胶囊等"));
		}
		if (pBox->mType==TypeSphere)
		{
			pProp = new CMFCPropertyGridProperty(_T("碰撞体类型"), _T("球体"),_T("与模型关联的碰撞体的类型，有立方体、球体、圆柱体、胶囊等"));
		}
		if (pBox->mType==TypeCylinder)
		{
			pProp = new CMFCPropertyGridProperty(_T("碰撞体类型"), _T("圆柱体"),_T("与模型关联的碰撞体的类型，有立方体、球体、圆柱体、胶囊等"));
		}
		if (pBox->mType==TypeCapsule)
		{
			pProp = new CMFCPropertyGridProperty(_T("碰撞体类型"), _T("胶囊体"),_T("与模型关联的碰撞体的类型，有立方体、球体、圆柱体、胶囊等"));
		}
		if (pBox->mType==TypePlane)
		{
		}
	}
	pProp->AddOption(_T("无"));
	pProp->AddOption(_T("立方体"));
	pProp->AddOption(_T("球体"));
	pProp->AddOption(_T("圆柱体"));
	pProp->AddOption(_T("胶囊体"));
	pProp->AddOption(_T("平面"));
	pGroup3->AddSubItem(pProp);
	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("物理特性"));
	if (meshObj->bAutoSimulate && meshObj->mPhysicsBox!=NULL)
	{
		pProp = new CMFCPropertyGridProperty(_T("自动模拟"), _T("开"),_T("是否开启物理模拟，必须与碰撞体关联才能使用"));
	}
	else
	{
		pProp = new CMFCPropertyGridProperty(_T("自动模拟"), _T("关"),_T("是否开启物理模拟，必须与碰撞体关联才能使用"));
	}
	pProp->AddOption(_T("开"));
	pProp->AddOption(_T("关"));
	pProp->AllowEdit(false);
	pGroup4->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("质量"),(_variant_t)meshObj->mWeight,_T("物体的质量，请根据重力设置适当的参数，否则可能模拟异常"));
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
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)soundObj->mName.c_str(), _T("场景物体的名字"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("资源"), (_variant_t)soundObj->mResName.c_str(), _T("物体所用文件资源"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("坐标:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)soundObj->mPostion.x, _T("x坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)soundObj->mPostion.y, _T("y坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)soundObj->mPostion.z, _T("z坐标"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pPriority = new CMFCPropertyGridProperty(_T("优先级"), (_variant_t)soundObj->mPriority, _T("该声音的优先级，值越小优先级越大，当声音过多时优先级较小的声音可能停止"));
	pPriority->EnableSpinControl(TRUE,0,256);
	m_wndPropList.AddProperty(pPriority);
	CMFCPropertyGridProperty* pLoop=NULL;
	if (soundObj->bLoop){ pLoop = new CMFCPropertyGridProperty(_T("循环播放"), _T("是"), _T("循环播放该声音"));}
	else{ pLoop = new CMFCPropertyGridProperty(_T("循环播放"), _T("否"), _T("循环播放该声音")); }
	pLoop->AddOption(_T("是"));
	pLoop->AddOption(_T("否"));
	m_wndPropList.AddProperty(pLoop);

	CMFCPropertyGridProperty* pValume = new CMFCPropertyGridProperty(_T("音量"),(_variant_t)(int)(soundObj->mValume*10), _T("该声音的音量，取值范围0-10"));
	pValume->EnableSpinControl(TRUE,0,10);
	m_wndPropList.AddProperty(pValume);

	CMFCPropertyGridProperty* pBingObj = NULL;
	if (soundObj->mBingObj==NULL)
	{
		pBingObj = new CMFCPropertyGridProperty(_T("绑定物体"),_T("无"), _T("将声音与一个场景物体绑定，声音的位置随物体位置的改变而改变"));
	}
	else
	{
		SceneMeshObj* Obj=(SceneMeshObj*)soundObj->mBingObj;
		pBingObj = new CMFCPropertyGridProperty(_T("绑定物体"), (_variant_t)Obj->mName.c_str(),_T("将声音与一个场景物体绑定，声音的位置随物体位置的改变而改变"));
	}
	pBingObj->AddOption(_T("无"));
	for (std::list<SceneMeshObj*>::iterator i=mscene->mMeshObjList.begin();i!=mscene->mMeshObjList.end();i++)
	{
		pBingObj->AddOption(CString((*i)->mName.c_str()));
	}
	m_wndPropList.AddProperty(pBingObj);
}

void ObjPropertiesWnd::InitPhysicsBoxProp()
{
	PhysicsBox* PBoxObj=(PhysicsBox*)tempObj;
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)CString(PBoxObj->mName.c_str()), _T("场景物体的名字"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("资源"), (_variant_t)CString(PBoxObj->mResName.c_str()), _T("物体所用文件资源"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("坐标:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)PBoxObj->mPostion.x, _T("x坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)PBoxObj->mPostion.y, _T("y坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)PBoxObj->mPostion.z, _T("z坐标"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pScale = new CMFCPropertyGridProperty(_T("缩放:"));
	pProp = new CMFCPropertyGridProperty(_T("ScaleX:"),(_variant_t)PBoxObj->mScale.x, _T("x方向缩放"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleY:"),(_variant_t)PBoxObj->mScale.y, _T("y方向缩放"));
	pScale->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ScaleZ:"),(_variant_t)PBoxObj->mScale.z, _T("z方向缩放"));
	pScale->AddSubItem(pProp);
	pScale->AllowEdit(true);
	m_wndPropList.AddProperty(pScale);

	CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("旋转:"));
	pProp = new CMFCPropertyGridProperty(_T("绕x轴（度）"),(_variant_t)PBoxObj->mRotaion.x, _T("绕x轴旋转"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("绕y轴（度）"),(_variant_t)PBoxObj->mRotaion.y, _T("绕y轴旋转"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("绕z轴（度）"),(_variant_t)PBoxObj->mRotaion.z, _T("绕z轴旋转"));
	pRotation->AddSubItem(pProp);
	pRotation->AllowEdit(true);
	m_wndPropList.AddProperty(pRotation);

	pProp = new CMFCPropertyGridProperty(_T("碰撞体类型"), _T("立方体"),_T("与模型关联的碰撞体的类型，有立方体、球体、圆柱体、胶囊等"));
	pProp->AddOption(_T("立方体"));
	pProp->AddOption(_T("球体"));
	pProp->AddOption(_T("圆柱体"));
	pProp->AddOption(_T("胶囊体"));
	pProp->AddOption(_T("平面"));
	if (PBoxObj->mType==TypeBox){	pProp->SetValue(_T("立方体"));}
	else if (PBoxObj->mType==TypeSphere){ pProp->SetValue(_T("球体"));}
	else if (PBoxObj->mType==TypeCylinder){pProp->SetValue(_T("圆柱体"));}
	else if (PBoxObj->mType==TypeCapsule){pProp->SetValue(_T("胶囊")); }
	else if (PBoxObj->mType==TypePlane)
	{
	}
	m_wndPropList.AddProperty(pProp);

	pProp = new CMFCPropertyGridProperty(_T("父物体"), _T("无"),_T("与该碰撞体绑定的物体"));
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
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)lightObj->mName.c_str(), _T("场景物体的名字"));
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("类型"), (_variant_t)lightObj->mResName.c_str(), _T("光源的类型，分点光源、聚光灯、平行光（模拟无穷远发射的太阳光，只有方向属性对其有效）"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pProp);
	pName->AllowEdit(false);
	pProp->AllowEdit(false);

	CMFCPropertyGridColorProperty*	pColorProp = new CMFCPropertyGridColorProperty(_T("漫反射"), RGB(0, 125, 200), NULL, _T("灯光的漫反射颜色"));
	pColorProp->SetColor(RGB(lightObj->mdiffuseColor.r * 255,lightObj->mdiffuseColor.g * 255, lightObj->mdiffuseColor.b * 255));
	pColorProp->EnableOtherButton(_T("其他..."));
	pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	m_wndPropList.AddProperty(pColorProp);
	pColorProp = new CMFCPropertyGridColorProperty(_T("镜面反射"), RGB(0, 125, 200), NULL, _T("灯光的镜面反射颜色"));
	pColorProp->SetColor(RGB(lightObj->mspecularColor.r * 255,lightObj->mspecularColor.g * 255, lightObj->mspecularColor.b * 255));
	pColorProp->EnableOtherButton(_T("其他..."));
	pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	m_wndPropList.AddProperty(pColorProp);

	if (lightObj->mType==LightPoint||lightObj->mType==LightSpot)
	{
		CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("坐标:"));
		pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)lightObj->mPostion.x, _T("x坐标"));
		pPosition->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)lightObj->mPostion.y, _T("y坐标"));
		pPosition->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)lightObj->mPostion.z, _T("z坐标"));
		pPosition->AddSubItem(pProp);;
		pPosition->AllowEdit(true);
		m_wndPropList.AddProperty(pPosition);
	}

	if (lightObj->mType==Lightdirection||lightObj->mType==LightSpot)
	{
		CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("旋转:"));
		pProp = new CMFCPropertyGridProperty(_T("绕x轴（度）"),(_variant_t)lightObj->mRotaion.x, _T("绕x轴旋转"));
		pRotation->AddSubItem(pProp);
		pProp = new CMFCPropertyGridProperty(_T("绕y轴（度）"),(_variant_t)lightObj->mRotaion.y, _T("绕y轴旋转"));
		pRotation->AddSubItem(pProp);
		pProp = new CMFCPropertyGridProperty(_T("绕z轴（度）"),(_variant_t)lightObj->mRotaion.z, _T("绕z轴旋转"));
		pRotation->AddSubItem(pProp);
		pRotation->AllowEdit(true);
		m_wndPropList.AddProperty(pRotation);
	}
	

	CMFCPropertyGridProperty* pCastShadow = new CMFCPropertyGridProperty(_T("投射阴影"), _T("是"), _T("该灯光是否投射阴影，建议投射阴影的灯光不要过多"));
	pCastShadow->AddOption(_T("是"));
	pCastShadow->AddOption(_T("否"));
	m_wndPropList.AddProperty(pCastShadow);
	if (lightObj->bCastShadow)
	{
		pCastShadow->SetValue(_T("是"));
		CMFCPropertyGridProperty* pShadow = new CMFCPropertyGridProperty(_T("阴影:"));
		pProp= new CMFCPropertyGridProperty(_T("阴影最大长度"),(_variant_t)lightObj->farClipDistance, _T("灯光产生的阴影的最大长度，当灯光与物体距离非常近时会产生无穷远的阴影，这时对性能造成较大影响，必须对阴影裁剪，并且最好避免灯光与物体距离过近"));
		pProp->EnableSpinControl(true,1,1000);
		pShadow->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("阴影开始距离"),(_variant_t)lightObj->nearClipDistance, _T("阴影的起点距离物体的距离，一般设置为0"));
		pProp->EnableSpinControl(true,1,500);
		pShadow->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("产生阴影距离"),(_variant_t)lightObj->shadowFarDistance, _T("灯光能够产生阴影的距离，当灯光与物体的距离大于此值时不产生阴影"));
		pProp->EnableSpinControl(true,1,1500);
		pShadow->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("最远影响距离"),(_variant_t)lightObj->influenceDistance, _T("灯光能够产生影响的最远距离，通常距离灯光发射点越远，灯光强度衰减的越厉害"));
		pProp->EnableSpinControl(true,1,2000);
		pShadow->AddSubItem(pProp);
		m_wndPropList.AddProperty(pShadow);
	}
	else
	{
		pCastShadow->SetValue(_T("否"));
	}

	if (lightObj->mType==LightPoint||lightObj->mType==LightSpot)
	{
		CMFCPropertyGridProperty* pAttenuation = new CMFCPropertyGridProperty(_T("衰减函数:"));
		pAttenuation->SetDescription(_T("光线强度随着与发射点的距离衰减，衰减函数的系数可以调整"));
		pProp= new CMFCPropertyGridProperty(_T("常量系数"),(_variant_t)lightObj->constParam, _T("衰减函数的常量部分"));
		pAttenuation->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("一次变量系数"),(_variant_t)lightObj->lineParam, _T("衰减函数的一次变量前的参数，如果二次变量参数为零，则线性变化"));
		pAttenuation->AddSubItem(pProp);
		pProp= new CMFCPropertyGridProperty(_T("二次变量系数"),(_variant_t)lightObj->quadParam, _T("衰减函数的二次变量前的参数，如果不为0则按照2次曲线变化"));
		pAttenuation->AddSubItem(pProp);
		m_wndPropList.AddProperty(pAttenuation);
	}
	
}

void ObjPropertiesWnd::InitParticleObjProp()
{
	SceneParticleObj* particleObj=(SceneParticleObj*)tempObj;
	CMFCPropertyGridProperty* pName = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)particleObj->mName.c_str(), _T("场景物体的名字"));
	CMFCPropertyGridProperty* pResName = new CMFCPropertyGridProperty(_T("资源"), (_variant_t)particleObj->mResName.c_str(), _T("物体所用文件资源"));
	m_wndPropList.AddProperty(pName);
	m_wndPropList.AddProperty(pResName);
	pName->AllowEdit(false);
	pResName->AllowEdit(false);
	CMFCPropertyGridProperty* pPosition = new CMFCPropertyGridProperty(_T("坐标:"));
	CMFCPropertyGridProperty* pProp= new CMFCPropertyGridProperty(_T("X:"),(_variant_t)particleObj->mPostion.x, _T("x坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Y:"),(_variant_t)particleObj->mPostion.y, _T("y坐标"));
	pPosition->AddSubItem(pProp);
	pProp= new CMFCPropertyGridProperty(_T("Z:"),(_variant_t)particleObj->mPostion.z, _T("z坐标"));
	pPosition->AddSubItem(pProp);;
	pPosition->AllowEdit(true);
	m_wndPropList.AddProperty(pPosition);

	CMFCPropertyGridProperty* pRotation= new CMFCPropertyGridProperty(_T("旋转:"));
	pProp = new CMFCPropertyGridProperty(_T("绕x轴（度）"),(_variant_t)particleObj->mRotaion.x, _T("绕x轴旋转"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("绕y轴（度）"),(_variant_t)particleObj->mRotaion.y, _T("绕y轴旋转"));
	pRotation->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("绕z轴（度）"),(_variant_t)particleObj->mRotaion.z, _T("绕z轴旋转"));
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
		CString PropName=pProp->GetName();  //被改变的参数名//
		COleVariant value = pProp->GetValue(); //改变之后的值//
		if (PropName==CString(_T("天空类别")))
		{
			CString str=value.bstrVal;     
			if (!str.IsEmpty())
			{
				if (str==CString(_T("无"))){mscene->mySky.type=0;}
				else if (str==CString(_T("平面"))){mscene->mySky.type=1;}
				else if (str==CString(_T("穹顶"))){mscene->mySky.type=2;}
				else if (str==CString(_T("立方体"))){mscene->mySky.type=3;}
			}
			mscene->initSky(mscene->mySky.type);
			InitSkyProp(mscene->mySky.type);
			return;
		}
		else if (PropName==CString(_T("背景色")))
		{
			CMFCPropertyGridColorProperty* p=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=p->GetColor();
			mscene->mySky.backColor[0]=GetRValue(color);
			mscene->mySky.backColor[1]=GetGValue(color);
			mscene->mySky.backColor[2]=GetBValue(color);
			mscene->setSky(mscene->mySky.type);
			return;
		}
		else if (PropName==CString(_T("天空材质")))
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
		else if (PropName==CString(_T("尺寸")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.size=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("高度")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.hight=value.fltVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("x分段")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.xcut=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("z分段")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.ycut=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("曲率")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.pCurvity=value.fltVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("纹理重复数")))
		{
			if (mscene->mySky.type!=0)
			{
				mscene->mySky.tiles=value.intVal;
				mscene->setSky(mscene->mySky.type);
			}
			return;
		}
		else if (PropName==CString(_T("雾的类型")))   //雾的属性//
		{
			CString str=value.bstrVal;     
			if (!str.IsEmpty())
			{
				if (str==CString(_T("无"))){mscene->myFog.type=0;}
				else if (str==CString(_T("线性雾"))){mscene->myFog.type=1;}
				else if (str==CString(_T("曲线雾"))){mscene->myFog.type=2;}
				else if (str==CString(_T("指数雾"))){mscene->myFog.type=3;}
			}
			mscene->initFog(mscene->myFog.type);
			InitFogProp(mscene->myFog.type);
			return;
		}
		else if (PropName==CString(_T("雾的颜色")))
		{
			CMFCPropertyGridColorProperty* p=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=p->GetColor();
			mscene->myFog.fogColor[0]=GetRValue(color);
			mscene->myFog.fogColor[1]=GetGValue(color);
			mscene->myFog.fogColor[2]=GetBValue(color);
			mscene->setFog();
			return;
		}
		else if (PropName==CString(_T("密度")))
		{
			mscene->myFog.density=value.intVal;
			mscene->setFog();
			return;
		}
		else if (PropName==CString(_T("开始距离")))
		{
			mscene->myFog.startDistance=value.intVal;
			mscene->setFog();
			return;
		}
		else if (PropName==CString(_T("结束距离")))
		{
			mscene->myFog.endDistance=value.intVal;
			mscene->setFog();
			return;
		}


	}

}
