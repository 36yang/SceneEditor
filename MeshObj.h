#ifndef __MeshObj_h_
#define __MeshObj_h_

#include "SceneObj.h"
#include "ode/ode.h"



class SceneMeshObj : public SceneObj
{
public:
	bool bAutoSimulate;   //当开启物理引擎时自动模拟//
	void* mPhysicsBox;
	float mWeight;
	bool bShowPhysisBox;
	float mPrimaryScale;   //////原本的缩放,根据设置的单位自动缩放比例///////////
	int offsetX,offsetY;
	int mPhytag;
	SceneMeshObj();
	SceneMeshObj(int tag,float pscale, Ogre::String &resPath);
	~SceneMeshObj();
	virtual void setPosition(Vector3 &pos);
	virtual void setScale(Vector3 &scale);
	virtual void setRotaion(Vector3 &rotaion);
	virtual void onUpdateValueChange(int tag);
	void setAutoSimulate(bool bAuto); //设置自动模拟//
	void setmPhysicsBox(void *obj);
};


#endif // #ifndef __SceneObj_h_
