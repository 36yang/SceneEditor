#ifndef __MeshObj_h_
#define __MeshObj_h_

#include "SceneObj.h"
#include "ode/ode.h"



class SceneMeshObj : public SceneObj
{
public:
	bool bAutoSimulate;   //��������������ʱ�Զ�ģ��//
	void* mPhysicsBox;
	float mWeight;
	bool bShowPhysisBox;
	float mPrimaryScale;   //////ԭ��������,�������õĵ�λ�Զ����ű���///////////
	int offsetX,offsetY;
	int mPhytag;
	SceneMeshObj();
	SceneMeshObj(int tag,float pscale, Ogre::String &resPath);
	~SceneMeshObj();
	virtual void setPosition(Vector3 &pos);
	virtual void setScale(Vector3 &scale);
	virtual void setRotaion(Vector3 &rotaion);
	virtual void onUpdateValueChange(int tag);
	void setAutoSimulate(bool bAuto); //�����Զ�ģ��//
	void setmPhysicsBox(void *obj);
};


#endif // #ifndef __SceneObj_h_
