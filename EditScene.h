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
		int type; //0�ޣ�1ƽ�棬2񷶥��3������//
		float backColor[3];
		Ogre::String MateralName;
		float hight;
		int xcut;       //x��Ƭ//
		int ycut;       //y��Ƭ//
		int tiles;     //�����ظ�����//
		int size;      //���ƽ���С//
		float pCurvity; //����//
	};

	struct Fog
	{
		int type;  //0�ޣ�1������2������3ָ����//
		float density;  //�ܶ�//
		float fogColor[3];  //�����ɫ//
		int startDistance;  //��ʼ����//
		int endDistance;  //��������//
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
	friend void nearCallback( void *data, dGeomID o1, dGeomID o2 );  //����������ײ���ص�//
	void defineTerrain(long x, long y, bool flat = false);
	void initBlendMaps(Terrain* terrain);

	
//ģ�����///////////////////////
	float mUnit;        //ģ�͵�λ���ã�Ĭ�Ϸ��ף���������ģ��ʱ������//
	int addRow, addLine;
	int distance, randomDistace;
	float randonRoll, randomscale;
	int offSetX, offSetY; 
	
//����//////////////////////////
	FMOD::System    *Fmodsystem;
	float soundUpdateTime;
	Ogre::Vector3 ListenerLastPos;
///////////////////////////////////////////////////////////

//��������//////////////////
	bool bOpenPhysis;    //�Ƿ����������//
	bool bPhysisCreate;  //���������Ƿ񴴽�//
	dWorldID mOdeWorld;
	dSpaceID mOdeSceneSpace;
	dHeightfieldDataID heightid;  
	dGeomID  mOdeWorldgeom;
	dJointGroupID contactgroup;
	Vector3 gravity;        //������СĬ��-9.8//
	float softCFM;        //ȫ�ֻ����Ĭ��0.04//
	bool bEnagleAutoSleep;  //�������˶��������Զ�˯��,�ɽ�ʡcpu//
	float contaceMaxCorrectVel;    //Ĭ�����ޣ�����ʹ��,ĳЩ��ײ����Ч���������Ӵ����ٶ�//
	//dWorldSetContactMaxCorrectingVel (world,1);
	float ContaceSurfaceLayer;       //��ײ�Ӵ���ȣ�Ĭ��0����������һ����Сֵ��ֹ��������0.001//
	int QuicksetpNumIterations;    //ģ�����������Խ��Խ��ϸ׼ȷ���ٶ�Խ����Ĭ��32������16-64֮��//
	dBodyID sphbody;
	dGeomID sphgeom;
	SceneNode* ballNode;
	Entity* ball;
	dGeomID heroGemo;

//�༭���/////////////////
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
	int selectMode;  //1ѡ�����壬0ѡ����ײ��//

//scene object that export to map file//
	std::list<SceneMeshObj*>   mMeshObjList;
	std::list<SceneSoundObj*>  mSoundList;
	std::list<PhysicsBox*> mPhysisBoxList;
	std::list<SceneLightObj*> mLightObjList;
	std::list<SceneParticleObj*> mParticleList;
	std::map<int, AnimationState> mAnimations;
	int meshTag, soundTag, particaleTag,PhysicsBoxTag,lightTag;



//�������/////////////////
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
