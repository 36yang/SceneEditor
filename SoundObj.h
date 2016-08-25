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
	
	int             mSoundType;    //1����2d������2����3d����//
	FMOD::Sound     *mSound;
	FMOD::Channel   *mChannel;
	int             mPriority;     //���ȼ�from 0 (most important) to 256 (least important), default = 128//
	bool            bLoop;         //�Ƿ�ѭ������// 
	float           mValume;        //������С0-1//
	SceneObj*       mBingObj;       //���԰󶨵���������//
	long            meshTag;        //�������tag//

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
