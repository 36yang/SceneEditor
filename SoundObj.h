#ifndef __SoundObj_h_
#define __SoundObj_h_

#include "SceneObj.h"
#include "fmod.hpp"
#include "fmod.h"

class SceneSoundObj : public SceneObj
{
public:

	SceneSoundObj(int tag,Ogre::String mediaPath);
	~SceneSoundObj();
	virtual bool frameStarted(const Ogre::FrameEvent &evt);  
	
	int             mSoundType;    //1代表2d声音，2代表3d声音//
	FMOD::Sound     *mSound;
	FMOD::Channel   *mChannel;
	int             mPriority;     //优先级from 0 (most important) to 256 (least important), default = 128//
	bool            bLoop;         //是否循环播放// 
	float           mValume;        //声音大小0-1//
	SceneObj*       mBingObj;       //可以绑定到场景物体//
	long            meshTag;        //绑定物体的tag//

	virtual void setPosition(Vector3 &pos);
	virtual void setScale(Vector3 &scale);
	virtual void setRotaion(Vector3 &rotaion);
	virtual void onUpdateValueChange(int tag);
	void setSoundType(int type);
	void setLoop(bool bloop);
	void setBingObj(SceneObj *obj);
	void setPriority(int priority);
	void setValume(float val);

protected:
private:
};


#endif // #ifndef __SceneObj_h_
