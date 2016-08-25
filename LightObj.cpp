#include "stdafx.h"
#include "LightObj.h"
#include "BaseApplication.h"


SceneLightObj::SceneLightObj( LightType type,int tag )
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	SceneObj();
	mObjType=LightObj;
	mType=type;
	long t=GetTickCount();
	mTag = tag + t;
	char strtemp[30];
	sprintf(strtemp,"LightObj%i",tag);
	mUserName = strtemp;
	sprintf(strtemp,"%s%ld",strtemp,t);  //%ld表示long类型//
	mName=strtemp;

	bVisualable=true;
	mPostion=Vector3::ZERO;
	mScale=Vector3(1,1,1);
	mRotaion=Vector3::ZERO;
	bCastShadow=true;
	mPower=1.0;
	mDirection=Ogre::Vector3::NEGATIVE_UNIT_Y;
	mdiffuseColor=Ogre::ColourValue::Blue;
	mspecularColor=Ogre::ColourValue::White;
	farClipDistance=100;   
	nearClipDistance=2; 
	shadowFarDistance=800;
	influenceDistance=1500; 
	constParam=0;
    lineParam=0.1;
	quadParam=0;
	innerDegree=30;
	outDegree=50;
	fallOff=0.1;
	mNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode(mName);
	subNode=appMgr->mSceneMgr->createSceneNode();
	mNode->addChild(subNode);
	subNode->setPosition(0,-1,0);
	if (type==LightPoint)
	{
		mEntity=appMgr->mSceneMgr->createEntity("pointLight.mesh");
		mEntity->setMaterialName("LightmeshAlph");
		mEntity->setCastShadows(false);
		mResName=Ogre::String("点光源.Light");
		mLight=appMgr->mSceneMgr->createLight();
		mLight->setType(Ogre::Light::LT_POINT);
		mLight->setAttenuation(influenceDistance,constParam,lineParam,quadParam);
	}
	else if (type==LightSpot)
	{
		Ogre::Entity* msub=appMgr->mSceneMgr->createEntity("pointLight.mesh");
		msub->setMaterialName("LightmeshAlph");
		msub->setCastShadows(false);	
		subNode->attachObject(msub);
		subNode->setScale(0.4,0.4,0.4);
		mEntity=appMgr->mSceneMgr->createEntity("spotLight.mesh");
		mEntity->setMaterialName("LightmeshFrame");
		mEntity->setCastShadows(false);
		mResName=Ogre::String("聚光灯.Light");
		mLight=appMgr->mSceneMgr->createLight();
		mLight->setType(Ogre::Light::LT_SPOTLIGHT);
		mLight->setSpotlightRange(Ogre::Degree(innerDegree), Ogre::Degree(outDegree),fallOff);
		mLight->setAttenuation(influenceDistance,constParam,lineParam,quadParam);
	}
	else if (type==Lightdirection)
	{
		mEntity=appMgr->mSceneMgr->createEntity("directionLight.mesh");
		mEntity->setMaterialName("LightmeshAlph");
		mEntity->setCastShadows(false);
		mResName=Ogre::String("平行光.Light");
		mLight=appMgr->mSceneMgr->createLight();
		mLight->setType(Ogre::Light::LT_DIRECTIONAL);
		mLight->setAttenuation(influenceDistance,constParam,lineParam,quadParam);
	}
	mNode->attachObject(mEntity);
	mLight->setCastShadows(bCastShadow);
	mLight->setDiffuseColour(mdiffuseColor);
	mLight->setSpecularColour(mspecularColor);
	mNode->attachObject(mLight);
	mLight->setDirection(mDirection);
	mLight->setShadowFarDistance(shadowFarDistance);
	mLight->setShadowFarClipDistance(farClipDistance);
	mLight->setShadowNearClipDistance(nearClipDistance);
	mLight->setDebugDisplayEnabled(true);

}

SceneLightObj::~SceneLightObj()
{
	BaseApplication::getInstance()->mSceneMgr->destroyLight(mLight);
}

void SceneLightObj::setPosition( Vector3 &pos )
{
	mNode->setPosition(pos);
	//mLight->setPosition(pos);
	mPostion=pos;
}

void SceneLightObj::setScale( Vector3 &scale )
{
	mNode->setScale(scale.x,scale.y,scale.z);
	mScale=scale;
}

void SceneLightObj::setRotaion( Vector3 &rotaion )
{
	mNode->resetOrientation();
	mNode->pitch(Ogre::Degree(rotaion.x),Ogre::Node::TS_PARENT);
	mNode->yaw(Ogre::Degree(rotaion.y),Ogre::Node::TS_PARENT);
	mNode->roll(Ogre::Degree(rotaion.z),Ogre::Node::TS_PARENT);
	mRotaion=rotaion;

	Vector3 subNodePos=subNode->_getDerivedPosition();
	Vector3 localpos=subNode->getPosition();
	mDirection=subNodePos-mNode->getPosition();
	//float x,y,z;
	//x= -Math::Sin(Ogre::Degree(rotaion.z).valueRadians()) * Math::Cos(Ogre::Degree(rotaion.y).valueRadians());
	//y= -Math::Cos(Ogre::Degree(rotaion.z).valueRadians()) * Math::Cos(Ogre::Degree(rotaion.x).valueRadians());
	//z= -Math::Sin(Ogre::Degree(rotaion.x).valueRadians()) * Math::Cos(Ogre::Degree(rotaion.y).valueRadians());
	//mDirection=Vector3(x,y,z);
	mLight->setDirection(mDirection);
	
}

void SceneLightObj::onUpdateValueChange( int tag )
{

}
