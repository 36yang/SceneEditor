#ifndef __LIGHTOBJ_h_
#define __LIGHTOBJ_h_

#include "Ogre.h"
#include "SceneObj.h"

using namespace Ogre;

enum LightType{LightPoint=0,Lightdirection, LightSpot };

class SceneLightObj : public SceneObj
{
public:
	SceneLightObj(LightType type,int tag);
	virtual ~SceneLightObj();
	virtual void setPosition(Vector3 &pos);
	virtual void setScale(Vector3 &scale);
	virtual void setRotaion(Vector3 &rotaion);
	virtual void onUpdateValueChange(int tag);

	bool                    bCastShadow;  //�Ƿ�Ͷ����Ӱ//
	Ogre::SceneNode*        subNode;    //�󶨵����ڵ�,����������������������ת�Ƕ�//
	Ogre::Light*			mLight;
	LightType               mType;   //0���Դ��1ƽ�й⣬2�۹��//
	float					mPower;  //ǿ��//
	Ogre::Vector3			mDirection;
	Ogre::ColourValue       mdiffuseColor; //������//
	Ogre::ColourValue       mspecularColor;  //���淴��//
	int						farClipDistance;   //��ӰԶ��ü�����//
	int						nearClipDistance;  //��Ӱ����ü�����//
	int						shadowFarDistance;  //��ԶͶ����Ӱ����//
	int						influenceDistance;  //��Զ�ɼ����룬�����������Զʱ�رչ�Ч��ʡ��Դ//
	float                   innerDegree;       //�۹����Ȧ������������н�//
	float                   outDegree;         //�۹��ǿ����Ȧ��������нǣ�ǿ��ͨ������������//
	float                   fallOff;          //�۹����Ȧǿ�����ĵ���Ȧ��������ǿ��˥����1.0��ʾ����˥����С��1.0��ʾ��˥��������1.0��ʾ��˥��//
	float                   constParam;      //�۹�ƻ�ƽ�й�˥�������ĳ�������//
	float                   lineParam;        //�۹�ƻ�ƽ�й�˥��������һ�α����Ĳ���////////
	float                   quadParam;       //�۹�ƻ�ƽ�й�˥�������Ķ��α�������///////
};


#endif // #ifndef __LIGHTOBJ_h_
