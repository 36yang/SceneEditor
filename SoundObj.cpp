#include "stdafx.h"
#include "SoundObj.h"
#include "BaseApplication.h"
#include "MainFrm.h"

#include "stdafx.h"
#include <iostream>
#include <atlstr.h> // for CString

#include <windows.h>
#include <lm.h>
using namespace std;


/************************************************************************/
/* 工具函数，将CString 转成 utf8编码的 char*                                                                     */
/************************************************************************/
char* CreateUTF8TextInitWithString(CString strValue)
{
	char *buffer = NULL;
	int  length;

#ifdef _UNICODE
	length = WideCharToMultiByte(CP_UTF8, 0, strValue, -1, NULL, 0, NULL, NULL);
#else
	return NULL;
#endif
	if (length <= 0)
	{
		return NULL;
	}

	buffer = new char[length];
	if (buffer == NULL)
	{
		return NULL;
	}

	ZeroMemory(buffer, length);

#ifdef _UNICODE
	WideCharToMultiByte(CP_UTF8, 0, strValue, -1, buffer, length, NULL, NULL);
#else
	strcpy_s(buffer, length, strValue);
#endif

	return buffer;
}

/************************************************************************/
/* 工具函数，将utf8的字符串转成CString                                                                     */
/************************************************************************/
BOOL CreateString_InitWithUTF8Text(CString& str, char* pUTF8Text)
{
	if (NULL == pUTF8Text)
	{
		return FALSE;
	}

	int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,
		0,
		pUTF8Text,
		-1,
		NULL,
		0 ); 

	wchar_t*  pUnicode = new  wchar_t[unicodeLen+1];
	if (NULL == pUnicode)
	{
		return FALSE;
	}

	MultiByteToWideChar( CP_UTF8, 0, pUTF8Text, -1, (LPWSTR)pUnicode, unicodeLen );

	str = pUnicode;

	delete []pUnicode;
	return TRUE;
}


SceneSoundObj::SceneSoundObj( int tag,Ogre::String mediaPath )
{
	SceneObj();
	mObjType=SoundObj;
	long t=GetTickCount();
	meshTag = -1;
	mTag = tag + t;
	char strtemp[30];
	sprintf(strtemp,"SoundObj%i",tag);
	mUserName = strtemp;
	sprintf(strtemp,"%s%ld",strtemp,t);  //%ld表示long类型//
	mName=strtemp;
	bVisualable=true;
	mResName=mediaPath;
	mPostion=Vector3::ZERO;
	mScale=Vector3(1,1,1);
	mRotaion=Vector3::ZERO;
	mEntity=BaseApplication::getInstance()->mSceneMgr->createEntity("Loudspeaker.mesh");
	mNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode(mName);
	mNode->attachObject(mEntity);
	mNode->setScale(0.1,0.1,0.1);
	Ogre::String groupName=Ogre::ResourceGroupManager::getSingletonPtr()->findGroupContainingResource(mediaPath);
	Ogre::FileInfoListPtr mfileInfo=Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(groupName,mediaPath);
	for (Ogre::FileInfoList::iterator i=mfileInfo->begin();i!=mfileInfo->end();i++)
	{
			Ogre::String mpath=i->archive->getName()+"/"+i->basename;
			CString str(mpath.c_str());
			const char * filename = CreateUTF8TextInitWithString(str);
			BaseApplication::getInstance()->mscene->Fmodsystem->createSound(filename,FMOD_3D, 0, &mSound);

	}
	mSound->set3DMinMaxDistance(5.0f, 1500);
	mSound->setMode(FMOD_LOOP_NORMAL); 
	BaseApplication::getInstance()->mscene->Fmodsystem->playSound(mSound, 0, true, &mChannel);
	FMOD_VECTOR pos={mPostion.x ,mPostion.y, mPostion.z};
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	mChannel->set3DAttributes(&pos, &vel);  //位置和速度//
	mChannel->setPaused(false);
	mValume=1.0;
	mChannel->setVolume(mValume);
	mSoundType = 2;    //1代表2d声音，2代表3d声音//
	bLoop=true;
	mPriority=128;
	mBingObj=NULL;
}

SceneSoundObj::~SceneSoundObj()
{
	mSound->release();
}

bool SceneSoundObj::frameStarted( const Ogre::FrameEvent &evt )
{
	if (mBingObj!=NULL)
	{
		setPosition(mBingObj->mPostion);
	}

	return true;
}

void SceneSoundObj::setPosition( Vector3 &pos )
{
	FMOD_VECTOR mpos={pos.x, pos.y, pos.z};
	FMOD_VECTOR vel={0, 0 ,0};
	mChannel->set3DAttributes(&mpos, &vel);
	mNode->setPosition(pos);
	mPostion=pos;
}

void SceneSoundObj::setScale( Vector3 &scale )
{

}

void SceneSoundObj::setRotaion( Vector3 &rotaion )
{

}

void SceneSoundObj::setSoundType( int type )
{
	if (type==1)
	{
		mChannel->setMode(FMOD_2D);
	}
	if (type==2)
	{
		mChannel->setMode(FMOD_3D);
	}
}

void SceneSoundObj::setLoop( bool bloop )
{
	if (bLoop)
	{
		mChannel->setMode(FMOD_LOOP_NORMAL);
	}
	else
	{
		mChannel->setMode(FMOD_LOOP_OFF);
	}
}

void SceneSoundObj::setPriority( int priority )
{
	if (priority>=0 && priority<=256)
	{
		mChannel->setPriority(priority);
		mPriority=priority;
	}
}

void SceneSoundObj::setBingObj( SceneObj *obj )
{
	if (obj!=NULL)
	{
		mBingObj=obj;
		meshTag = obj->mTag;
		BaseApplication::getInstance()->mRoot->addFrameListener(this);
	}
	else
	{
		mBingObj=NULL;
		meshTag = -1;
		BaseApplication::getInstance()->mRoot->removeFrameListener(this);
	}
}

void SceneSoundObj::setValume( float val )
{
	mChannel->setVolume(val);
	mValume=val;
}

void SceneSoundObj::onUpdateValueChange( int tag )
{
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	if (pMain->m_wndObjProperties.tempObj!=NULL)
	{
		SceneObj* sceneObj=(SceneObj*)pMain->m_wndObjProperties.tempObj;
		if (sceneObj->mObjType==SoundObj)
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
				/*CMFCPropertyGridProperty* prop=pMain->m_wndObjProperties.m_wndPropList.GetProperty(3);
				prop->GetSubItem(0)->SetValue((_variant_t)mScale.x);
				prop->GetSubItem(1)->SetValue((_variant_t)mScale.y);
				prop->GetSubItem(2)->SetValue((_variant_t)mScale.z);*/
			}
			if (tag==2)
			{
				/*CMFCPropertyGridProperty* prop=pMain->m_wndObjProperties.m_wndPropList.GetProperty(4);
				prop->GetSubItem(0)->SetValue((_variant_t)mRotaion.x);
				prop->GetSubItem(1)->SetValue((_variant_t)mRotaion.y);
				prop->GetSubItem(2)->SetValue((_variant_t)mRotaion.z);*/
			}

		}
	}

}
