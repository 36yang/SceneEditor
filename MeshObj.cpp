#include "stdafx.h"
#include "MeshObj.h"
#include "BaseApplication.h"
#include "PhysicsBox.h"
#include "MainFrm.h"

SceneMeshObj::SceneMeshObj(int tag,float pscale,Ogre::String &resPath)
{
	SceneObj();
	mObjType=MeshObj;
	mPrimaryScale=pscale;
	long t=GetTickCount();
	mTag = tag + t;
	mPhytag=-1;
	char strtemp[30];
	sprintf(strtemp,"MeshObj%i",tag);
	mUserName = strtemp;
	sprintf(strtemp,"%s%ld",strtemp,t);  //%ld表示long类型//
	mName=strtemp;

	bVisualable=true;
	mResName=resPath;
	mPostion=Vector3::ZERO;
	mScale=mPrimaryScale*Vector3(1,1,1);
	mRotaion=Vector3::ZERO;
	mEntity=BaseApplication::getInstance()->mSceneMgr->createEntity(resPath);
	mNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode(mName);
	mNode->attachObject(mEntity);
	mNode->setScale(mScale);
	offsetX=0;
	offsetY=0;
	mWeight=50;
	mPhysicsBox=NULL;
	bAutoSimulate=false;
}

SceneMeshObj::SceneMeshObj()
{
	SceneObj();
}

SceneMeshObj::~SceneMeshObj()
{
	Ogre::LogManager::getSingleton().logMessage("mesh destoryed");
}


void SceneMeshObj::setAutoSimulate( bool bAuto )
{
	if (mPhysicsBox!=NULL)
	{
		BaseApplication* appMgr=BaseApplication::getInstance();
		PhysicsBox* pBox=(PhysicsBox*)mPhysicsBox;
		if (bAutoSimulate&&bAuto)	
		{
			return;
		}
		bAutoSimulate=bAuto;
		if (bAuto)
		{
			pBox->InitBody();
			appMgr->mRoot->addFrameListener(pBox);
		}
		else
		{
			appMgr->mRoot->removeFrameListener(pBox);
		}
	}
	
}

void SceneMeshObj::setmPhysicsBox(void *obj)
{
	if (obj == NULL)
	{
		mPhysicsBox = NULL;
	}
	else
	{
		mPhysicsBox=obj;
		mPhytag = ((PhysicsBox*)obj)->mTag;
	}
}

void SceneMeshObj::setPosition( Vector3 &pos )
{
	Vector3 OffsetPos=pos-mPostion;
	mNode->setPosition(pos);
	mPostion=pos;
	if (mPhysicsBox!=NULL)
	{
		((PhysicsBox*)mPhysicsBox)->setOffsetPosition(OffsetPos);
	}
}

void SceneMeshObj::setScale( Vector3 &scale )
{
	mNode->setScale(scale.x,scale.y,scale.z);
	mScale=scale;
	if (mPhysicsBox!=NULL)
	{
		PhysicsBox* pBox=(PhysicsBox*)mPhysicsBox;
		Ogre::Vector3 size=mEntity->getWorldBoundingBox(true).getSize();
		if (pBox->mType==TypeBox)
		{
			scale.x=size.x/10,scale.y=size.y/10,scale.z=size.z/10;
		}
		if (pBox->mType==TypeSphere)
		{
			size.x=size.x>size.y?size.x:size.y;
			size.x=size.x>size.z?size.x:size.z;
			scale.x=scale.y=scale.z=size.x/10;
		}
		if (pBox->mType==TypeCylinder)
		{
			scale.x=size.x/10,scale.y=size.y/10,scale.z=size.z/10;
		}
		if (pBox->mType==TypeCapsule)
		{
			float maxLen=size.x>size.z?size.x:size.z;
			mScale.x=maxLen/5;  mScale.y=mScale.x;
			size.y-=10;  //减去两个半球的高//
			if (size.y<=0)
			{
				size.y=0.1;
			}
			mScale.z=size.y/10;
		}
		if (pBox->mType==TypePlane)
		{
		}
	
		pBox->setScale(scale);
	}
}

void SceneMeshObj::setRotaion( Vector3 &rotaion )
{
	Vector3 OffsetRot=rotaion-mRotaion;
	mNode->resetOrientation();
	mNode->pitch(Ogre::Degree(rotaion.x),Ogre::Node::TS_PARENT);
	mNode->yaw(Ogre::Degree(rotaion.y),Ogre::Node::TS_PARENT);
	mNode->roll(Ogre::Degree(rotaion.z),Ogre::Node::TS_PARENT);
	mRotaion=rotaion;
	if (mPhysicsBox!=NULL)
	{
		((PhysicsBox*)mPhysicsBox)->setOffsetRotaion(OffsetRot);
	}
}

void SceneMeshObj::onUpdateValueChange( int tag )
{
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	if (pMain->m_wndObjProperties.tempObj!=NULL)
	{
		SceneObj* sceneObj=(SceneObj*)pMain->m_wndObjProperties.tempObj;
		if (sceneObj->mObjType==MeshObj)
		{
			if (tag==0)
			{
				CMFCPropertyGridProperty* prop=pMain->m_wndObjProperties.m_wndPropList.GetProperty(2);
				prop->GetSubItem(0)->SetValue((_variant_t)mPostion.x);
				prop->GetSubItem(1)->SetValue((_variant_t)mPostion.y);
				prop->GetSubItem(2)->SetValue((_variant_t)mPostion.z);
			}
			if (tag==1)
			{
				CMFCPropertyGridProperty* prop=pMain->m_wndObjProperties.m_wndPropList.GetProperty(3);
				prop->GetSubItem(0)->SetValue((_variant_t)mScale.x);
				prop->GetSubItem(1)->SetValue((_variant_t)mScale.y);
				prop->GetSubItem(2)->SetValue((_variant_t)mScale.z);
			}
			if (tag==2)
			{
				CMFCPropertyGridProperty* prop=pMain->m_wndObjProperties.m_wndPropList.GetProperty(4);
				prop->GetSubItem(0)->SetValue((_variant_t)mRotaion.x);
				prop->GetSubItem(1)->SetValue((_variant_t)mRotaion.y);
				prop->GetSubItem(2)->SetValue((_variant_t)mRotaion.z);
			}
		}
	}
}

