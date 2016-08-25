#ifndef __SceneObj_h_
#define __SceneObj_h_

#include "Ogre.h"

using namespace Ogre;

	enum ObjType{SoundObj, MeshObj, LightObj, ParticleObj, PhysicsBoxObj };

class SceneObj :public Ogre::FrameListener
{
public:
	SceneObj(){ }
	virtual ~SceneObj(){ }
	virtual bool frameStarted(const Ogre::FrameEvent &evt){ return true; }  
	virtual void setPosition(Vector3 &pos)
	{
		mNode->setPosition(pos);
		mPostion=pos;
	}
	virtual void setScale(Vector3 &scale)
	{
		mNode->setScale(scale.x,scale.y,scale.z);
		mScale=scale;
	}
	virtual void setRotaion(Vector3 &rotaion)
	{
		mNode->resetOrientation();
		mNode->pitch(Ogre::Degree(rotaion.x),Ogre::Node::TS_PARENT);
		mNode->yaw(Ogre::Degree(rotaion.y),Ogre::Node::TS_PARENT);
		mNode->roll(Ogre::Degree(rotaion.z),Ogre::Node::TS_PARENT);
		mRotaion=rotaion;
	}
	virtual void showBoundingBox(bool bShow)
	{
		mNode->showBoundingBox(bShow);
	}
	virtual void setVisualAble(bool bvisual)
	{
		bVisualable=bvisual;
		if (bvisual)
		{ mNode->setVisible(true);}
		else
		{ mNode->setVisible(false);
		  mNode->showBoundingBox(false);		
		}
	}

	/************************************************************************/
	/* 更新物体在属性面板的显示，由子类实现
	*tag 0坐标，1缩放，2旋转
	*/
	/************************************************************************/
	virtual void onUpdateValueChange(int tag){}

	ObjType			mObjType;
	long			mTag;
	Ogre::String	mResName;
	Ogre::String    mName;
	Ogre::String    mUserName;
	Ogre::Vector3	mPostion;
	Ogre::Vector3   mScale;
	Ogre::Vector3   mRotaion;
	bool            bVisualable;
	SceneNode *mNode;
	Entity *mEntity;
};

#endif // #ifndef __SceneObj_h_
