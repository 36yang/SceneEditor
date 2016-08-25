#ifndef __EditScene_h_
#define __EditScene_h_

#include "Ogre.h"
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"


#include <list>
#include "SceneObj.h"
#include "MeshObj.h"
#include "SoundObj.h"
#include "PhysicsBox.h"
#include "LightObj.h"
#include "ParticleObj.h"
#include "Hero.h"	
#include "ode/ode.h"
#include "fmod.hpp"
#include "fmod.h"



	enum TerrainEditMode{TerrainUp, TerrainDown, TerrainToLevel, TerrainSmooth, TerrainPlatUp,
	 TerrainPlatDown,AddObj,TerrainSelectObj, TerrainNoise,TerrainNoneEdit};
	
	enum ObjectEditMode{ObjMove, ObjScale, ObjRoll,ObjCopy,ObjDelete, ObjFind, ObjNoneEdit};

	struct Sky
	{
		int type; //0无，1平面，2穹顶，3立方体//
		float backColor[3];
		Ogre::String MateralName;
		float hight;
		int xcut;       //x切片//
		int ycut;       //y切片//
		int tiles;     //纹理重复次数//
		int size;      //天空平面大小//
		float pCurvity; //曲率//
	};

	struct Fog
	{
		int type;  //0无，1线性雾，2曲线雾，3指数雾//
		float density;  //密度//
		float fogColor[3];  //雾的颜色//
		int startDistance;  //开始距离//
		int endDistance;  //结束距离//
	};


class EditScene : public Ogre::FrameListener
{

public:
	EditScene();
	virtual ~EditScene();

	void createScene(void);     
	bool initScene(void);
	void initTerrainSystem();
	void loadTerrain();
	void getTerrainImage(bool flipX, bool flipY, Image& img);
	void initSky(int type);
	void setSky(int type);
	void initFog(int type);
	void setFog();
	void saveScene(std::vector<CString> &resPath, CString &SavePath);
	void loadScene(Ogre::String &scenePath);
	void clearScene();
	void initView();
	virtual bool frameStarted(const Ogre::FrameEvent &evt) ;  
	void createTerrain(int size,int baseHight, int numVertex,
		 int Xtiles, int Ztiles, Ogre::String *texture0,
		 Ogre::String *normalTexture0, Ogre::String *HightMap);
	void destroyTerrain();
	void initPhysics();
	void createPhysisWorld(int width,int length,int maxHight, int numVeticals);
	void destroyPhysisWorld();
	void resizeBrush();
	void createSceneObj(Ogre::String resName);
	void distanceRandom();
	void scaleRandom();
	void AddSceneObj();
	void deleteSceneObj(SceneObj* obj);
	void readFile(CString &str,CString &path );
	SceneObj* findSceneObjByName(Ogre::String &name,int type);    //type 0-mesh 1-sound 2-partical//
	SceneObj* findSceneObjByUserName(Ogre::String &name, int type);
	friend void nearCallback( void *data, dGeomID o1, dGeomID o2 );  //物理世界碰撞检测回调//
	void defineTerrain(long x, long y, bool flat = false);
	void initBlendMaps(Terrain* terrain);

	
//模型相关///////////////////////
	float mUnit;        //模型单位设置，默认分米，决定建立模型时的缩放//
	int addRow, addLine;
	int distance, randomDistace;
	float randonRoll, randomscale;
	int offSetX, offSetY; 
	
//声音//////////////////////////
	FMOD::System    *Fmodsystem;
	float soundUpdateTime;
	Ogre::Vector3 ListenerLastPos;
///////////////////////////////////////////////////////////

//物理引擎//////////////////
	bool bOpenPhysis;    //是否打开物理引擎//
	bool bPhysisCreate;  //物理世界是否创建//
	dWorldID mOdeWorld;
	dSpaceID mOdeSceneSpace;
	dHeightfieldDataID heightid;  
	dGeomID  mOdeWorldgeom;
	dJointGroupID contactgroup;
	Vector3 gravity;        //重力大小默认-9.8//
	float softCFM;        //全局混合力默认0.04//
	bool bEnagleAutoSleep;  //允许不再运动的物体自动睡眠,可节省cpu//
	float contaceMaxCorrectVel;    //默认无限，谨慎使用,某些碰撞会无效，最大允许接触的速度//
	//dWorldSetContactMaxCorrectingVel (world,1);
	float ContaceSurfaceLayer;       //碰撞接触深度，默认0，建议设置一个极小值防止抖动例如0.001//
	int QuicksetpNumIterations;    //模拟迭代步数，越大越精细准确，速度越慢，默认32，建议16-64之间//
	dBodyID sphbody;
	dGeomID sphgeom;
	SceneNode* ballNode;
	Entity* ball;
	dGeomID heroGemo;

//编辑相关/////////////////
	Sky mySky;
	Fog myFog;
	Hero *mHero;
	Ogre::SceneNode* gridNode;
	std::list<SceneObj*> tempAddObj;
	std::vector<SceneObj*> selectSceneObj;	
	std::vector<SceneObj*> mhideSceneObj;
	Ogre::SceneNode *SceneNode;
	Ogre::Camera *mCamera;	
	Ogre::SceneNode *cameraNode;
	int selectMode;  //1选择物体，0选择碰撞体//

//scene object that export to map file//
	std::list<SceneMeshObj*>   mMeshObjList;
	std::list<SceneSoundObj*>  mSoundList;
	std::list<PhysicsBox*> mPhysisBoxList;
	std::list<SceneLightObj*> mLightObjList;
	std::list<SceneParticleObj*> mParticleList;
	std::map<int, AnimationState> mAnimations;
	int meshTag, soundTag, particaleTag,PhysicsBoxTag,lightTag;



//地形相关/////////////////
	TerrainEditMode mTerrainEditMode;
	ObjectEditMode mObjectEditMode;
	int mSplatImage;
	int selectTexture;
	int terrainXtiles, terrainZtiles;
	int terrainSize;
	bool bTerrainCreated;
	TerrainGlobalOptions* mTerrainGlobals;
	TerrainGroup* mTerrainGroup;
	Ogre::String* diffuseTexture[8];
	Ogre::String* normalTexture[8];
	Ogre::RaySceneQuery *mRaySceneQuery;     // The ray scene query pointer
	float BrushSize;
	float BrushStrength;


};

#endif // #ifndef __Scene2_h_
