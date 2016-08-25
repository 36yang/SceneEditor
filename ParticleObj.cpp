#include "stdafx.h"
#include "ParticleObj.h"
#include "BaseApplication.h"



SceneParticleObj::SceneParticleObj( int tag, Ogre::String ParticleName )
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	SceneObj();
	mObjType=ParticleObj;
	long t=GetTickCount();
	mTag = tag + t;
	char strtemp[30];
	sprintf(strtemp,"ParticleObj%i",tag);
	mUserName = strtemp;
	sprintf(strtemp,"%s%ld",strtemp,t);  //%ld表示long类型//
	mName=strtemp;

	bVisualable=true;
	mResName=ParticleName;
	mPostion=Vector3::ZERO;
	mScale=Vector3(1,1,1);
	mRotaion=Vector3::ZERO;
	mEntity=appMgr->mSceneMgr->createEntity("ParticlePointor.mesh");
	mEntity->setMaterialName("ParticlePointor");
	mNode=appMgr->mSceneMgr->createSceneNode();
	mEntityNode=appMgr->mSceneMgr->createSceneNode(mName);
	mEntityNode->attachObject(mEntity);
	mParticleNode=appMgr->mSceneMgr->createSceneNode();
	Ogre::String baseName, nameExt;
	Ogre::StringUtil::splitBaseFilename(mResName,baseName,nameExt);
	ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	mParticle = pManager->createParticleSystem(mUserName,baseName, appMgr->mSceneMgr);
	mParticleNode->attachObject(mParticle);
	mNode->addChild(mEntityNode);
	mNode->addChild(mParticleNode);
	mParticle->prepare();
	mParticle->start();

}

SceneParticleObj::~SceneParticleObj()
{
	ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	pManager->destroyParticleSystem(mUserName,BaseApplication::getInstance()->mSceneMgr);
}

void SceneParticleObj::onUpdateValueChange( int tag )
{

}

void SceneParticleObj::showBoundingBox(bool bShow)
{
	mEntityNode->showBoundingBox(bShow);
}