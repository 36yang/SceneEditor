#include "stdafx.h"
#include "PhysicsBox.h"
#include "BaseApplication.h"
#include "MainFrm.h"

PhysicsBox::PhysicsBox(dSpaceID mOdeSpace, PhysicsBoxType mType, int tag, SceneMeshObj* Obj/*=NULL*/)
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	mGeom=NULL;
	mBody=NULL;
	mSpace=mOdeSpace;
	this->mType=mType;
	long t=GetTickCount();
	mTag = tag + t;
	char strtemp[30];
	sprintf(strtemp,"Physics%i",tag);
	mUserName = strtemp;
	sprintf(strtemp,"%s%ld",strtemp,t);  //%ld表示long类型//
	mName=strtemp;

	bVisualable=true;
	mObjType=PhysicsBoxObj;
	mNode=appMgr->mSceneMgr->createSceneNode(mName);
	mPostion=Vector3::ZERO;
	mScale=Vector3(1,1,1);
	mRotaion=Vector3::ZERO;
	mRadius=5.0; mXlen=10; mYlen=10; mZlen=10;
	mMeshObj=Obj;
	if (mType==TypeBox)
	{
		mEntity=appMgr->mSceneMgr->createEntity("box.mesh"); 
		mNode->attachObject(mEntity);
		mResName=Ogre::String("立方体.Phy");
	}
	else if (mType==TypeSphere)
	{
		mEntity=appMgr->mSceneMgr->createEntity("sphere.mesh");
		mNode->attachObject(mEntity);
		mResName=Ogre::String("球体.Phy");
	}
	else if (mType==TypeCapsule)
	{
		Ogre::String name=mName+"Xcapsule0";  //X便于分割字符串进行查找//
		capsuleNode=appMgr->mSceneMgr->createSceneNode(name);
		name=mName+"Xcapsule1";
		farNode=appMgr->mSceneMgr->createSceneNode(name);
		name=mName+"Xcapsule2";
		nearNode=appMgr->mSceneMgr->createSceneNode(name);
		mResName=Ogre::String("胶囊.Phy");
		mEntity=appMgr->mSceneMgr->createEntity("capsuleMid.mesh");
		capsuleNode->attachObject(mEntity);
		mEntity->setMaterialName("SEsphere/lambert1");
		Ogre::Entity* farSphere=appMgr->mSceneMgr->createEntity("farSphere.mesh");
		farSphere->setMaterialName("SEsphere/lambert1");
		Ogre::Entity* nearSphere=appMgr->mSceneMgr->createEntity("nearSphere.mesh");
		nearSphere->setMaterialName("SEsphere/lambert1");
		farNode->attachObject(farSphere);
		nearNode->attachObject(nearSphere);
		mNode->addChild(capsuleNode);
		mNode->addChild(farNode);
		mNode->addChild(nearNode);
		farNode->setPosition(0,0,-5);
		nearNode->setPosition(0,0,5);
	}
	else if (mType==TypeCylinder)
	{
		mEntity=appMgr->mSceneMgr->createEntity("cylinder.mesh");
		mEntity->setMaterialName("SEsphere/lambert1");
		mNode->attachObject(mEntity);
		mResName=Ogre::String("圆柱体.Phy");
	}
	else if (mType==TypePlane)
	{
		mEntity=appMgr->mSceneMgr->createEntity("plane.mesh");
		mNode->attachObject(mEntity);
		mResName=Ogre::String("平面.Phy");
	}
	
	InitGemo(mType);
}




PhysicsBox::~PhysicsBox()
{
	dGeomDestroy(mGeom);   // (because dWorldDestroy() destroys body connected to geom; must call first!)
	if (mBody)
	{
		dBodyDestroy(mBody);
	}
	if (mMeshObj!=NULL)
	{
		mMeshObj->setmPhysicsBox(NULL);
	}

	Ogre::LogManager::getSingleton().logMessage("PhysicsBox destoryed");
}

bool PhysicsBox::frameStarted( const Ogre::FrameEvent &evt )
{
	if (mBody!=NULL)
	{
		if (mMeshObj!=NULL&&mMeshObj->bAutoSimulate)
		{	
		
			const dReal *SPos = dBodyGetPosition(mBody);
			const dReal *SQua =	dBodyGetQuaternion(mBody);
			mPostion=Vector3(SPos[0], SPos[1], SPos[2]);
			mNode->setPosition(mPostion);
			Ogre::Quaternion qu(SQua[0],SQua[1], SQua[2], SQua[3]);
			mNode->resetOrientation();
			mNode->setOrientation(qu);
			mRotaion=Vector3(qu.getPitch().valueDegrees(), qu.getYaw().valueDegrees(), qu.getRoll().valueDegrees());


			mMeshObj->mNode->setPosition(mPostion);
			mMeshObj->mPostion=mPostion;
			mMeshObj->mRotaion=mRotaion;
			mMeshObj->mRotaion.x=mMeshObj->mRotaion.x-90;
			mMeshObj->mNode->resetOrientation();
			mMeshObj->mNode->setOrientation(qu);
			mMeshObj->mNode->pitch(Ogre::Degree(-90),Ogre::Node::TS_LOCAL);
		
		}


	}

	return true;
}

void PhysicsBox::InitGemo( PhysicsBoxType mType )
{
	if (mGeom!=NULL)
	{
		dGeomDestroy(mGeom);
		mGeom=NULL;
	}
	
	if (mType==TypeBox){ mGeom=dCreateBox(mSpace,mXlen,mYlen,mZlen);}
	else if (mType==TypeCapsule){mGeom=dCreateCapsule(mSpace,mRadius,10);}
	else if (mType==TypeCylinder){mGeom=dCreateCylinder(mSpace,mRadius,mYlen);}
	else if (mType==TypeSphere){mGeom=dCreateSphere(mSpace,mRadius);}
	else if (mType==TypePlane){}

	mRotaion=Vector3(90,0,0);
	if (mMeshObj!=NULL)
	{
		Ogre::Vector3 Objsize=mMeshObj->mEntity->getWorldBoundingBox(true).getSize();
		mPostion=mMeshObj->mEntity->getWorldBoundingBox(true).getCenter();
		if (mType==TypeBox)
		{
			mScale=Vector3(Objsize.x/10,Objsize.y/10,Objsize.z/10);
		}
		if (mType==TypeSphere)
		{
			float maxLen=Objsize.x>Objsize.y?Objsize.x:Objsize.y;
			maxLen=maxLen>Objsize.z?maxLen:Objsize.z;
			mScale=Vector3(maxLen/5, maxLen/5, maxLen/5);
		}
		if (mType==TypeCylinder)
		{
			float maxLen=Objsize.x>Objsize.z?Objsize.x:Objsize.z;
			mScale.x=maxLen/5;  mScale.y=mScale.x;
			mScale.z=Objsize.y/10;
		}
		if (mType==TypeCapsule)
		{
			float maxLen=Objsize.x>Objsize.z?Objsize.x:Objsize.z;
			mScale.x=maxLen/5;  mScale.y=mScale.x;
			Objsize.y-=10;  //减去两个半球的高//
			if (Objsize.y<=0)
			{
				Objsize.y=0.1;
			}
			mScale.z=Objsize.y/10;
			
		}
		if (mType==TypePlane)
		{
		}
	}
	setPosition(mPostion);
	setScale(mScale);
	setRotaion(mRotaion);
}

void PhysicsBox::InitBody()
{
	if (mBody!=NULL)
	{
		dBodyDestroy(mBody);
		mBody=NULL;
	}
	if (mMeshObj!=NULL)
	{
		dMass m;
		BaseApplication* appMgr=BaseApplication::getInstance();
		mBody=dBodyCreate(appMgr->mscene->mOdeWorld);
		if (mType==TypeBox)
		{ 
			dMassSetBoxTotal(&m,mMeshObj->mWeight,mXlen,mYlen,mZlen);		
		}
		else if (mType==TypeCapsule)
		{
			dMassSetCapsuleTotal(&m,mMeshObj->mWeight,3,mRadius,mZlen); //方向取值1,2,3带表x,y,z//
		}
		else if (mType==TypeCylinder)
		{
			dMassSetCylinderTotal(&m,mMeshObj->mWeight,3,mRadius,mZlen);
		}
		else if (mType==TypeSphere)
		{
			dMassSetSphereTotal(&m,mMeshObj->mWeight,mRadius);
		}
		else if (mType==TypePlane){}
		dBodySetMass (mBody,&m);
		dBodySetPosition(mBody,mPostion.x,mPostion.y,mPostion.z);
		dReal SQua[4];
		dGeomGetQuaternion(mGeom,SQua);
		dBodySetQuaternion(mBody,SQua);
		
		dGeomSetBody(mGeom,mBody);
		dBodySetAngularVel(mBody,0,0,0);
		dBodySetLinearVel(mBody,0,0,0);

	}
	
}

void PhysicsBox::setPosition( Vector3 &pos )
{
	mPostion=pos;
	mNode->setPosition(pos);
	dGeomSetPosition(mGeom,pos.x,pos.y,pos.z);
	//dBodySetPosition(mBody,mPostion.x,mPostion.y,mPostion.z);
}

/************************************************************************/
/* 原始版本，坐标z轴与y轴未交换//                                                                     */
/************************************************************************/
/*
void PhysicsBox::setScale( Vector3 &scale )
{           
	checkScale(scale);
	//mRadius=mXlen>mYlen?mXlen/2:mYlen/2;
	
	if (mType==TypeBox)
	{
		mScale=scale;
		mNode->setScale(scale.x,scale.y,scale.z);
		Ogre::Vector3 size=mEntity->getWorldBoundingBox(true).getSize();
		mXlen=size.x; mYlen=size.y; mZlen=size.z;
		dGeomBoxSetLengths(mGeom,mXlen,mYlen,mZlen);
	}
	else if (mType==TypeSphere)
	{
		Vector3 OffsetScale=scale-mScale;
		OffsetScale.z=Math::Abs(OffsetScale.z); OffsetScale.x=Math::Abs(OffsetScale.x); OffsetScale.y=Math::Abs(OffsetScale.y);
		float maxScale= OffsetScale.x>OffsetScale.z?OffsetScale.x:OffsetScale.z;
		scale.x = OffsetScale.x>OffsetScale.z?scale.x:scale.z;
		mScale.x=mScale.y=mScale.z=scale.x;
		if (maxScale<OffsetScale.y)
		{
			mScale.x=mScale.y=mScale.z=scale.y;
		}
		mNode->setScale(mScale);
		Ogre::Vector3 size=mEntity->getWorldBoundingBox(true).getSize();
		mXlen=size.x; mYlen=size.y; mZlen=size.z;
		mRadius=5*mScale.x;
		dGeomSphereSetRadius(mGeom,mRadius);
	}
	else if(mType==TypeCylinder)
	{
		Vector3 OffsetScale=scale-mScale;
		OffsetScale.z=Math::Abs(OffsetScale.z); OffsetScale.x=Math::Abs(OffsetScale.x); OffsetScale.y=Math::Abs(OffsetScale.y);
		scale.x = OffsetScale.x>OffsetScale.y?scale.x:scale.y;
		scale.y=scale.x;
		mScale=scale;
		mNode->setScale(mScale);
		mRadius=5*mScale.x;
		mXlen=mRadius*2; mZlen=10*mScale.z; mYlen=mRadius*2;
		dGeomCylinderSetParams(mGeom,mRadius,mZlen);
	}
	else if (mType==TypeCapsule)
	{
		Vector3 OffsetScale=scale-mScale;
		OffsetScale.z=Math::Abs(OffsetScale.z); OffsetScale.x=Math::Abs(OffsetScale.x); OffsetScale.y=Math::Abs(OffsetScale.y);
		scale.x = OffsetScale.x>OffsetScale.y?scale.x:scale.y;
		scale.y=scale.x;
		mScale=scale;
		farNode->setScale(Vector3(mScale.x,mScale.x,mScale.x));
		nearNode->setScale(Vector3(mScale.x,mScale.x,mScale.x));
		capsuleNode->setScale(mScale);
		mRadius=5*mScale.x;
		mXlen=5*mScale.x; mYlen=mXlen; mZlen=10*mScale.z;
		farNode->setPosition(0,0,-mZlen/2);
		nearNode->setPosition(0,0,mZlen/2);
		dGeomCapsuleSetParams(mGeom,mRadius,mZlen);
	}
	else if(mType==TypePlane)
	{
		//dGeomPlaneSetParams(mGeom,);
	}
	else if (mType==TypeTransform)
	{
	}

}
*/





/************************************************************************/
/* 修正版，坐标z轴和坐标y轴在实际缩放时发生了交换//                                                                     */
/************************************************************************/
void PhysicsBox::setScale( Vector3 &scale )
{           
	checkScale(scale);
	mScale=scale;
	//mRadius=mXlen>mYlen?mXlen/2:mYlen/2;

	if (mType==TypeBox)
	{
		mNode->setScale(scale.x,scale.z,scale.y);
		mXlen=10*scale.x; mYlen=10*scale.z; mZlen=10*scale.y;
		dGeomBoxSetLengths(mGeom,mXlen,mYlen,mZlen);
	}
	else if (mType==TypeSphere)
	{
		Vector3 OffsetScale=scale-mScale;
		OffsetScale.z=Math::Abs(OffsetScale.z); OffsetScale.x=Math::Abs(OffsetScale.x); OffsetScale.y=Math::Abs(OffsetScale.y);
		float maxScale= OffsetScale.x>OffsetScale.z?OffsetScale.x:OffsetScale.z;
		scale.x = OffsetScale.x>OffsetScale.z?scale.x:scale.z;
		mScale.x=mScale.y=mScale.z=scale.x;
		if (maxScale<OffsetScale.y)
		{
			mScale.x=mScale.y=mScale.z=scale.y;
		}
		mNode->setScale(mScale);
		mXlen=10*scale.x; mYlen=10*scale.z; mZlen=10*scale.y;
		mRadius=5*mScale.x;
		dGeomSphereSetRadius(mGeom,mRadius);
	}
	else if(mType==TypeCylinder)
	{
		Vector3 OffsetScale=scale-mScale;
		OffsetScale.z=Math::Abs(OffsetScale.z); OffsetScale.x=Math::Abs(OffsetScale.x); OffsetScale.y=Math::Abs(OffsetScale.y);
		scale.x = OffsetScale.x>OffsetScale.z?scale.x:scale.z;
		scale.z=scale.x;
		mNode->setScale(scale.x,scale.x,scale.y);
		mRadius=5*scale.x;
		mXlen=mRadius*2; mYlen=mRadius*2;; mZlen=10*scale.y;
		dGeomCylinderSetParams(mGeom,mRadius,mZlen);
	}
	else if (mType==TypeCapsule)
	{
		Vector3 OffsetScale=scale-mScale;
		OffsetScale.z=Math::Abs(OffsetScale.z); OffsetScale.x=Math::Abs(OffsetScale.x); OffsetScale.y=Math::Abs(OffsetScale.y);
		scale.x = OffsetScale.x>OffsetScale.z?scale.x:scale.z;
		scale.z=scale.x;
		farNode->setScale(Vector3(scale.x,scale.x,scale.x));
		nearNode->setScale(Vector3(scale.x,scale.x,scale.x));
		capsuleNode->setScale(scale.x,scale.x,scale.y);
		mRadius=5*scale.x;
		mXlen=mRadius*2; mYlen=mRadius*2; mZlen=10*mScale.y;
		farNode->setPosition(0,0,-mZlen/2);
		nearNode->setPosition(0,0,mZlen/2);
		dGeomCapsuleSetParams(mGeom,mRadius,mZlen);
	}
	else if(mType==TypePlane)
	{
		//dGeomPlaneSetParams(mGeom,);
	}

}

void PhysicsBox::setRotaion( Vector3 &rotaion )
{
	mNode->resetOrientation();
	mNode->pitch(Ogre::Degree(rotaion.x),Ogre::Node::TS_PARENT);
	mNode->yaw(Ogre::Degree(rotaion.y),Ogre::Node::TS_PARENT);
	mNode->roll(Ogre::Degree(rotaion.z),Ogre::Node::TS_PARENT);
	mRotaion=rotaion;
	Ogre::Quaternion q=mNode->getOrientation();
	float qu[4]={q.w, q.x, q.y, q.z};
	dGeomSetQuaternion(mGeom,qu);
}

void PhysicsBox::setOffsetPosition( Vector3 &OffsetPos )
{
	mPostion+=OffsetPos;
	setPosition(mPostion);
}

void PhysicsBox::setOffsetScale( Vector3 &OffsetScale )
{
	Vector3 scale=mScale+OffsetScale;
	setScale(scale);
}

void PhysicsBox::setOffsetRotaion( Vector3 &OffsetRotaion )
{
	Vector3 rotion=mRotaion+OffsetRotaion;
	setRotaion(rotion);
}

/************************************************************************/
/* 检查缩放，并进行修正，最小缩放为0.1，ode的参数不能为零和负数//                                                                   */
/************************************************************************/
bool PhysicsBox::checkScale( Vector3& scale )
{
	if (scale.x<=0)
	{
		scale.x=0.1;
	}
	if (scale.y<=0)
	{
		scale.y=0.1;
	}
	if (scale.z<=0)
	{
		scale.z=0.1;
	}

	return true;
}

void PhysicsBox::onUpdateValueChange( int tag )
{
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	if (pMain->m_wndObjProperties.tempObj!=NULL)
	{
		SceneObj* sceneObj=(SceneObj*)pMain->m_wndObjProperties.tempObj;
		if (sceneObj->mObjType==PhysicsBoxObj)		{
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
