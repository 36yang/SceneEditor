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

	bool                    bCastShadow;  //是否投射阴影//
	Ogre::SceneNode*        subNode;    //绑定到父节点,根据世界坐标用来计算旋转角度//
	Ogre::Light*			mLight;
	LightType               mType;   //0点光源，1平行光，2聚光灯//
	float					mPower;  //强度//
	Ogre::Vector3			mDirection;
	Ogre::ColourValue       mdiffuseColor; //漫反射//
	Ogre::ColourValue       mspecularColor;  //镜面反射//
	int						farClipDistance;   //阴影远点裁剪距离//
	int						nearClipDistance;  //阴影近点裁剪距离//
	int						shadowFarDistance;  //最远投射阴影距离//
	int						influenceDistance;  //最远可见距离，当离摄像机过远时关闭光效节省资源//
	float                   innerDegree;       //聚光灯外圈弱光与中心轴夹角//
	float                   outDegree;         //聚光灯强光内圈与中心轴夹角，强光通常集中在中心//
	float                   fallOff;          //聚光灯内圈强光中心到外圈弱光中心强度衰减，1.0表示线性衰减，小于1.0表示慢衰减，大于1.0表示快衰减//
	float                   constParam;      //聚光灯或平行光衰减函数的常亮部分//
	float                   lineParam;        //聚光灯或平行光衰减函数的一次变量的参数////////
	float                   quadParam;       //聚光灯或平行光衰减函数的二次变量参数///////
};


#endif // #ifndef __LIGHTOBJ_h_
