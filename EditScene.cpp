#pragma warning(disable:4251 4275)   //禁用vc警告

#include "stdafx.h"
#include "EditScene.h"
#include "BaseApplication.h"
#include "MainFrm.h"
#include "SceneLoader.h"
using namespace Ogre;

//创建地形的回调函数///
dReal heightfield_callback( void* pUserData, int x, int z )
{
	EditScene* mscene = (EditScene*)pUserData;
	Terrain* t = mscene->mTerrainGroup->getTerrain(0,0);
	return t->getHeightAtPoint(x,z);
}



EditScene::EditScene()
{
	BaseApplication *appMgr=BaseApplication::getInstance();
	
	SceneNode=appMgr->mSceneMgr->createSceneNode();
	gridNode=appMgr->mSceneMgr->createSceneNode();
	cameraNode=appMgr->mSceneMgr->createSceneNode();
	mCamera=appMgr->mCamera;
	cameraNode->attachObject(mCamera);
	mHero=Hero::getInstance(cameraNode);
	appMgr->mSceneMgr->getRootSceneNode()->addChild(mHero->pnode);

	for (int i=0;i<8;i++)
	{
		diffuseTexture[i] = NULL;
		normalTexture[i] = NULL;
	}

	createScene();
	initScene();
	initPhysics();

	appMgr->mRoot->addFrameListener(this);

}

EditScene::~EditScene(void)
{
}

void EditScene::createScene(void)
{
	meshTag=0; soundTag=0; particaleTag=0; PhysicsBoxTag=0; lightTag=0;
	BaseApplication *appMgr=BaseApplication::getInstance();
	Ogre::Entity* grid=appMgr->mSceneMgr->createEntity("grid.mesh");
	gridNode->attachObject(grid);
	appMgr->mSceneMgr->getRootSceneNode()->addChild(gridNode);
	gridNode->setPosition(0,0,0);
	//appMgr->mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);//相当真实的阴影//
	appMgr->mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
	//appMgr->mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE); //节省性能的阴影//
	//appMgr->mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//appMgr->mSceneMgr->setShadowTextureSelfShadow(true);
	appMgr->mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0,1.0,1.0));


	//Ogre::Light* l = appMgr->mSceneMgr->createLight();
	//l->setType(Ogre::Light::LT_DIRECTIONAL);
	//l->setPosition(100,180,100);
	//l->setDirection(0,-1,0.7);
	//l->setDiffuseColour(ColourValue(0.8f, 0.8f, 0.8f));
	//l->setSpecularColour(ColourValue(0.1f, 0.1f, 0.1f));
	//l->setShadowFarDistance(1500);

	initTerrainSystem();

	
	//appMgr->mSceneMgr->setSkyBox(true,"BoxSky/redsunset_TGA");
	//Ogre::Plane plane;
	//plane.d = 10; plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	//appMgr->mSceneMgr->setSkyPlane(true,plane ,"PlaneSky/SpaceSkyPlane",1500,75);
	//appMgr->mSceneMgr->setSkyDome( true, "DomeSky/alpine", 5, 8,3000,true);
	//appMgr->mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(0.7,0.7,1.0), 0.001,4000,5000);
	ballNode=BaseApplication::getInstance()->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ball = BaseApplication::getInstance()->mSceneMgr->createEntity("足球.mesh");
	Vector3 size=ball->getWorldBoundingBox(true).getSize();
	ballNode->setScale(5/size.x,5/size.y,5/size.z);  //足球直径5分米//
	ballNode->setPosition(50.0,2.0,20.0);
	ballNode->attachObject(ball);
	ballNode->setVisible(false);
	ballNode->showBoundingBox(true);
	
}

bool EditScene::initScene( void )
{
	selectMode=1;
	bOpenPhysis=true;
	bPhysisCreate=false;
	mUnit=0.08;
	FMOD::System_Create(&Fmodsystem);
	Fmodsystem->init(100, FMOD_INIT_NORMAL, NULL);
    Fmodsystem->set3DSettings(1.0, 10, 1.0f); //10是距离缩放因子，1米等于10分米//
	ListenerLastPos=Vector3::ZERO;
	soundUpdateTime=0; //50ms更新一次声音系统//

	gravity=Vector3(0,-98,0);      
	softCFM=0.00001;       
	bEnagleAutoSleep=false;
	contaceMaxCorrectVel=1.0;    
	ContaceSurfaceLayer=0.00001;
	QuicksetpNumIterations=32;
	mTerrainEditMode=TerrainNoneEdit;
	mObjectEditMode=ObjNoneEdit;
	mSplatImage=0;    //选择刷哪个纹理//
	selectTexture=0;  //选择添加或删除哪个纹理///
	bTerrainCreated=false;
	terrainSize = 5000;  //5000分米///
	BrushSize=0.03;  //范围0.01-0.1////
	BrushStrength= 300; //范围(100-500)
	addRow=1; addLine=1;
	mRaySceneQuery =BaseApplication::getInstance()->mSceneMgr->createRayQuery(Ogre::Ray());
	
	tempAddObj.clear();
	selectSceneObj.clear();

	
	initSky(0);
	initFog(0);
	return true;
}


bool EditScene::frameStarted( const Ogre::FrameEvent &evt )
{
	if(BaseApplication::getInstance()->mWindow->isClosed())
		return false;

	if (bTerrainCreated&&bOpenPhysis)
	{
		
		dSpaceCollide (mOdeSceneSpace,this,&nearCallback);
		dWorldQuickStep (mOdeWorld, 0.1);
		dJointGroupEmpty (contactgroup);
		
		const dReal *SPos = dBodyGetPosition(sphbody);
		const dReal *SQua =	dBodyGetQuaternion(sphbody);
		ballNode->setPosition(SPos[0], SPos[1], SPos[2]);
		Ogre::Quaternion qu(SQua[0],SQua[1], SQua[2], SQua[3]);
		ballNode->setOrientation(qu);
		Vector3 herpos=mHero->pnode->getPosition();
		dGeomSetPosition(heroGemo,herpos.x,herpos.y,herpos.z);

	}
	return true;
}


void EditScene::initView()
{

	cameraNode->resetOrientation();
	cameraNode->setPosition(Vector3(0,30,300));
	mCamera->lookAt(0,0,-30);
}


void EditScene::createTerrain(int size, int baseHight, int numVertex,
	int Xtiles, int Ztiles, Ogre::String *texture0,
	Ogre::String *normalTexture0, Ogre::String *HightMap)
{

	destroyTerrain();
		
	terrainXtiles = Xtiles;
	terrainZtiles = Ztiles;
	terrainSize = size * 10;
	mTerrainGroup->setTerrainWorldSize(terrainSize);
	mTerrainGroup->setTerrainSize(numVertex);


	Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = numVertex;
	defaultimp.worldSize = size*10;
	defaultimp.inputScale = 2000;
	// textures
	defaultimp.layerList.resize(1);    //支持8层纹理	
	defaultimp.layerList[0].worldSize = 100;
	//defaultimp.layerList[1].worldSize = 100;   //纹理重复粒度//
	//defaultimp.layerList[1].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	//defaultimp.layerList[1].textureNames.push_back("dirt_grayrocky_normalheight.dds");


	Ogre::String filename,fileFolder;
	if (texture0 != NULL)
	{
		diffuseTexture[0] = texture0;
		Ogre::StringUtil::splitFilename(*texture0,filename,fileFolder); 
		texture0 = new Ogre::String(filename);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(fileFolder,"FileSystem","Terrain");  
		defaultimp.layerList[0].textureNames.push_back(*texture0);
	}
	if (normalTexture0 != NULL)
	{
		normalTexture[0] = normalTexture0;
		Ogre::StringUtil::splitFilename(*normalTexture0,filename,fileFolder); 
		normalTexture0 = new Ogre::String(filename);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(fileFolder,"FileSystem","Terrain");  
		defaultimp.layerList[0].textureNames.push_back(*normalTexture0);
	}			
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Terrain");


	for (int i=0;i<terrainXtiles;i++)
	{
		for (int j=0;j<terrainZtiles;j++)
		{
			if (HightMap == NULL)
			{
				mTerrainGroup->defineTerrain(i, j, baseHight);
			}
			else
			{
				defaultimp.inputScale = baseHight;
				Image img;  //从高度图创建地形///
				getTerrainImage(i % 2 != 0, j % 2 != 0, img);
				mTerrainGroup->defineTerrain(i, j, &img);
			}
			
		}
	}

	mTerrainGroup->loadAllTerrains(true);

	TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
	while(ti.hasMoreElements())
	{
		Terrain* t = ti.getNext()->instance;
		t->dirty();
		t->update();
	}
	bTerrainCreated = true;
	gridNode->setVisible(false);

	createPhysisWorld(terrainSize,terrainSize, 1, numVertex);   //创建物理世界//

/*
	Image image;
	image.load("brush.png", "ET");
	image.resize(32, 32);
	mEditBrush = ET::loadBrushFromImage(image);

	
	initView();
	bTerrainCreated=true;
	gridNode->setVisible(false);
	*/
}

void EditScene::destroyTerrain()
{
	mTerrainEditMode = TerrainNoneEdit;

	if (bTerrainCreated)
	{
		mTerrainGroup->removeAllTerrains();
		mTerrainGroup->getDefaultImportSettings().layerList.clear();
		
		
		CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
		pMain->m_wndProperties.m_wndEditTerrain->checkRadioButton();
		pMain->m_wndProperties.m_wndEditTerrain->initView();  //初始化地形刷视图内容

		bTerrainCreated=false;
		gridNode->setVisible(true);

		destroyPhysisWorld();
	}
}


void EditScene::resizeBrush()
{
	/*
	Image image;
	image.load("brush.png", "ET");
	image.resize(BrushSize, BrushSize);
	mEditBrush = ET::loadBrushFromImage(image);
	*/
}

void EditScene::AddSceneObj()
{
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	for (std::list<SceneObj*>::iterator j=tempAddObj.begin();j!=tempAddObj.end();j++)
	{
		if((*j)->mObjType==MeshObj)
		{
			SceneMeshObj *temp=(SceneMeshObj*)(*j);
			mMeshObjList.push_back(temp);	
			HTREEITEM hitem=pMain->m_wndSceneObjView.hMesh;
			pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),3,3,hitem);
			if (NULL != temp->mPhysicsBox)
			{
				mPhysisBoxList.push_back((PhysicsBox*)temp->mPhysicsBox);
				hitem=pMain->m_wndSceneObjView.hSurroundBox;
				Ogre::String name=((PhysicsBox*)temp->mPhysicsBox)->mUserName;
				pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(name.c_str()),5,5,hitem);
			}

		}
		else if((*j)->mObjType==SoundObj)
		{
			SceneSoundObj *temp=(SceneSoundObj*)(*j);
			mSoundList.push_back(temp);
			HTREEITEM hitem=pMain->m_wndSceneObjView.hSound;
			pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),6,6,hitem);
		}
		else if ((*j)->mObjType==PhysicsBoxObj)
		{
			PhysicsBox* temp=(PhysicsBox*)(*j);
			mPhysisBoxList.push_back(temp);
			HTREEITEM hitem=pMain->m_wndSceneObjView.hSurroundBox;
			pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),5,5,hitem);
		}
		else if((*j)->mObjType==ParticleObj)
		{
			SceneParticleObj* temp=(SceneParticleObj*)(*j);
			mParticleList.push_back(temp);
			HTREEITEM hitem=pMain->m_wndSceneObjView.hParticle;
			pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),4,4,hitem);
		}
		else if ((*j)->mObjType==LightObj)
		{
			SceneLightObj* temp=(SceneLightObj*)(*j);
			mLightObjList.push_back(temp);
			HTREEITEM hitem=pMain->m_wndSceneObjView.hLight;
			pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),7,7,hitem);
		}
	}
	tempAddObj.clear();
}

void EditScene::createSceneObj( Ogre::String resName )
{
	if (mTerrainEditMode==AddObj)
	{
		if (tempAddObj.size()>0)   //创建之前先把原来的移除//
		{
			for (std::list<SceneObj*>::iterator i=tempAddObj.begin(); i!=tempAddObj.end();i++)
			{	
				deletePtr(*i);
			}
			tempAddObj.clear();
		}	
		Ogre::String NameExt,baseName;
		Ogre::StringUtil::splitBaseFilename(resName,baseName,NameExt);
		if (NameExt=="mesh")
		{		
			for (int i=0;i<addRow;i++)   //创建//
			{
				for (int j=0;j<addLine;j++)
				{
					SceneMeshObj *temp=new SceneMeshObj(meshTag,mUnit,resName);
					SceneNode->addChild(temp->mNode);
					if (bOpenPhysis)
					{
						PhysicsBox *tempBox=new PhysicsBox(mOdeSceneSpace,TypeBox,PhysicsBoxTag,temp);
						SceneNode->addChild(tempBox->mNode);
						temp->setmPhysicsBox(tempBox);
						PhysicsBoxTag+=1;
					}
					tempAddObj.push_back(temp);
					meshTag+=1;
				}
			}
		}
		else if (NameExt=="mp3"||NameExt=="ogg"||NameExt=="wav"||NameExt=="mid")
		{
			SceneSoundObj *temp=new SceneSoundObj(soundTag,resName);
			SceneNode->addChild(temp->mNode);
			tempAddObj.push_back(temp);
			soundTag+=1;
		}
		else if (NameExt=="Phy")
		{
			PhysicsBox* temp=NULL;
			if (baseName=="立方体"){ temp=new PhysicsBox(mOdeSceneSpace,TypeBox,PhysicsBoxTag);}
			else if (baseName=="球体"){temp=new PhysicsBox(mOdeSceneSpace,TypeSphere,PhysicsBoxTag);}
			else if (baseName=="圆柱体"){temp=new PhysicsBox(mOdeSceneSpace,TypeCylinder,PhysicsBoxTag);}
			else if (baseName=="胶囊"){temp=new PhysicsBox(mOdeSceneSpace,TypeCapsule,PhysicsBoxTag);}
			else if (baseName=="平"){}
			if (temp!=NULL)
			{
				SceneNode->addChild(temp->mNode);
				tempAddObj.push_back(temp);
			}
				PhysicsBoxTag+=1;
		}
		else if (NameExt=="Light")
		{
			SceneLightObj *temp=NULL;
			if (baseName=="点光源"){ temp=new SceneLightObj(LightPoint,lightTag);}
			else if (baseName=="聚光灯"){temp=new SceneLightObj(LightSpot,lightTag);}
			else if (baseName=="平行光"){temp=new SceneLightObj(Lightdirection,lightTag);}
			SceneNode->addChild(temp->mNode);
			tempAddObj.push_back(temp);
			lightTag+=1;
		}
		else if (NameExt=="particle")
		{
			SceneParticleObj* temp=new SceneParticleObj(particaleTag,resName);
			SceneNode->addChild(temp->mNode);
			tempAddObj.push_back(temp);
			particaleTag+=1;
		}
		scaleRandom();
		distanceRandom();
	}
	
}

void EditScene::distanceRandom()
{

}

void EditScene::scaleRandom()
{
	for(std::list<SceneObj*>::iterator i=tempAddObj.begin();i!=tempAddObj.end();i++)
	{

		
//		i->mNode->setScale()

	}
}

void EditScene::saveScene(std::vector<CString> &resPath,  CString &SavePath)
{
	CStringA straPath(SavePath.GetBuffer(0));
	std::string stdPath=straPath.GetBuffer(0);
	Ogre::String Opath(stdPath.c_str());

	SceneLoader *sloader = new SceneLoader(this);
	sloader->saveScene(resPath,Opath);
}

void EditScene::loadScene(Ogre::String &scenePath)
{
	clearScene();   //加载场景前清空当前场景//
	SceneLoader *sloader = new SceneLoader(this);
	sloader->loadscene(scenePath);
}

void EditScene::readFile(CString &str,CString &path )
{
	char *temp;
	CFile openfile(path,CFile::modeRead);
	int length=openfile.GetLength();
	temp=new char[length];
	openfile.Read(temp,length);
	openfile.Close();
	str=(CString)temp;
	delete []temp;
}

SceneObj* EditScene::findSceneObjByName( Ogre::String &name,int type)
{
	switch (type)
	{
	case 0:      //find mesh
		for (std::list<SceneMeshObj*>::iterator i=mMeshObjList.begin();i!=mMeshObjList.end();++i)
		{
			if ((*i)->mName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 1:   //find sound
		for (std::list<SceneSoundObj*>::iterator i=mSoundList.begin();i!=mSoundList.end();++i)
		{
			if ((*i)->mName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 2:  //find PhysicsBox
		for (std::list<PhysicsBox*>::iterator i=mPhysisBoxList.begin();i!=mPhysisBoxList.end();++i)
		{
			if ((*i)->mName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 3:
		for (std::list<SceneLightObj*>::iterator i=mLightObjList.begin();i!=mLightObjList.end();++i)
		{
			if ((*i)->mName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 4:
		for (std::list<SceneParticleObj*>::iterator i=mParticleList.begin();i!=mParticleList.end();++i)
		{
			if ((*i)->mName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	default:
		return NULL;
		break;
	}

}

SceneObj* EditScene::findSceneObjByUserName( Ogre::String &name, int type )
{
	switch (type)
	{
	case 0:      //find mesh
		for (std::list<SceneMeshObj*>::iterator i=mMeshObjList.begin();i!=mMeshObjList.end();++i)
		{
			if ((*i)->mUserName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 1:   //find sound
		for (std::list<SceneSoundObj*>::iterator i=mSoundList.begin();i!=mSoundList.end();++i)
		{
			if ((*i)->mUserName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 2:  //find PhysicsBox
		for (std::list<PhysicsBox*>::iterator i=mPhysisBoxList.begin();i!=mPhysisBoxList.end();++i)
		{
			if ((*i)->mUserName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 3:
		for (std::list<SceneLightObj*>::iterator i=mLightObjList.begin();i!=mLightObjList.end();++i)
		{
			if ((*i)->mUserName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	case 4:
		for (std::list<SceneParticleObj*>::iterator i=mParticleList.begin();i!=mParticleList.end();++i)
		{
			if ((*i)->mUserName.compare(name)==0)
			{
				SceneObj* Obj=*i;
				return Obj;
			}
		}
		break;
	default:
		return NULL;
		break;
	}

}

void EditScene::createPhysisWorld( int width,int length,int maxHight, int numVeticals)
{
	heightid = dGeomHeightfieldDataCreate();
	float* data = NULL;
	Ogre::AxisAlignedBox AABB;
	TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
	while(ti.hasMoreElements())
	{
		Terrain* t = ti.getNext()->instance;
		data = t->getHeightData();
		AABB = t->getAABB();
		break;
	}

	//dGeomHeightfieldDataBuildSingle(heightid, data,true,width,length,numVeticals,numVeticals,1.0,0.0,10.0f,true);
    //dGeomHeightfieldDataSetBounds( heightid, AABB.getMinimum().y,  AABB.getMaximum().y);  //地形的包围盒//
	//mOdeWorldgeom = dCreateHeightfield( mOdeSceneSpace, heightid, true );
	//dGeomSetPosition(mOdeWorldgeom,0,0,0);



	dGeomHeightfieldDataBuildCallback( heightid, this, heightfield_callback,
		terrainSize, terrainSize, numVeticals, numVeticals,
		REAL( 1.0 ), REAL( 0.0 ), REAL( 2.0 ), 0 );
	//dGeomHeightfieldDataSetBounds( heightid, AABB.getMinimum().y,  AABB.getMaximum().y);
	mOdeWorldgeom = dCreateHeightfield( mOdeSceneSpace, heightid, true );
	dGeomSetPosition(mOdeWorldgeom,2,2,2);





	ballNode->setVisible(true);
	
}

void EditScene::destroyPhysisWorld()
{	
	dGeomDestroy(mOdeWorldgeom);
	dGeomHeightfieldDataDestroy(heightid);
}

void EditScene::initSky( int type )
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	switch(type)
	{
	case 0:
		mySky.type=0;
		mySky.backColor[0]=166; mySky.backColor[1]=202; mySky.backColor[2]=240;
		break;
	case 1:
		mySky.type=1;
		mySky.backColor[0]=166; mySky.backColor[1]=202; mySky.backColor[2]=240;
		mySky.MateralName=Ogre::String("PlaneSky/SpaceSkyPlane");
		mySky.hight=2000;
		mySky.size=1000;
		mySky.pCurvity=10.0;
		mySky.tiles=32;     
		mySky.xcut=1;
		mySky.ycut=1;
		break;
	case 2:
		mySky.type=2;
		mySky.backColor[0]=255; mySky.backColor[1]=255; mySky.backColor[2]=255;
		mySky.MateralName=Ogre::String("DomeSky/CloudySky");
		mySky.hight=5000;
		mySky.tiles=8;
		mySky.pCurvity=10.0;
		mySky.xcut=16;
		mySky.ycut=16;
		break;
	case 3:
		mySky.type=3;
		mySky.backColor[0]=255; mySky.backColor[1]=255; mySky.backColor[2]=255;
		mySky.MateralName=Ogre::String("BoxSky/lostvalley");
		mySky.hight=5000;
		break;
	default:
		break;
	}
	setSky(type);
}

void EditScene::setSky( int type )
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	Ogre::Plane plane; 
	plane.d = mySky.hight; plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	switch(type)
	{
	case 0:
		appMgr->mSceneMgr->setSkyPlaneEnabled(false);
		appMgr->mSceneMgr->setSkyDomeEnabled(false);
		appMgr->mSceneMgr->setSkyBoxEnabled(false);
		appMgr->mWindow->getViewport(0)->setBackgroundColour(Ogre::ColourValue(mySky.backColor[0]/255,mySky.backColor[1]/255,mySky.backColor[2]/255,1.0));
		break;
	case 1:
		appMgr->mSceneMgr->setSkyDomeEnabled(false);
		appMgr->mSceneMgr->setSkyBoxEnabled(false);
		appMgr->mSceneMgr->setSkyPlane(true,plane,mySky.MateralName,mySky.size,mySky.tiles,false,0,mySky.xcut,mySky.ycut);
		appMgr->mWindow->getViewport(0)->setBackgroundColour(Ogre::ColourValue(mySky.backColor[0]/255,mySky.backColor[1]/255,mySky.backColor[2]/255,1.0));
		break;
	case 2:
		appMgr->mSceneMgr->setSkyPlaneEnabled(false);
		appMgr->mSceneMgr->setSkyBoxEnabled(false);
		appMgr->mSceneMgr->setSkyDome(true,mySky.MateralName,mySky.pCurvity,mySky.tiles,mySky.hight,false,Quaternion::IDENTITY,mySky.xcut,mySky.ycut);
		appMgr->mWindow->getViewport(0)->setBackgroundColour(Ogre::ColourValue(mySky.backColor[0]/255,mySky.backColor[1]/255,mySky.backColor[2]/255,1.0));
		break;
	case 3:
		appMgr->mSceneMgr->setSkyPlaneEnabled(false);
		appMgr->mSceneMgr->setSkyDomeEnabled(false);
		appMgr->mSceneMgr->setSkyBox(true,mySky.MateralName,mySky.hight,false);
		appMgr->mWindow->getViewport(0)->setBackgroundColour(Ogre::ColourValue(mySky.backColor[0]/255,mySky.backColor[1]/255,mySky.backColor[2]/255,1.0));
		break;
	default:
		appMgr->mWindow->getViewport(0)->setBackgroundColour(Ogre::ColourValue(mySky.backColor[0]/255,mySky.backColor[1]/255,mySky.backColor[2]/255,1.0));
		break;
	}
}

void EditScene::initFog( int type )
{

	BaseApplication* appMgr=BaseApplication::getInstance();
	switch(type)
	{
	case 0:
		myFog.type=0;
		break;
	case 1:
		myFog.type=1;
		myFog.fogColor[0]=178; myFog.fogColor[1]=178; myFog.fogColor[2]=255;	
		myFog.density=0.001;
		myFog.startDistance=1000;
		myFog.endDistance=3000;
		break;
	case 2:
		myFog.type=2;
		myFog.fogColor[0]=178; myFog.fogColor[1]=178; myFog.fogColor[2]=255;	
		myFog.density=0.00001;
		myFog.startDistance=1000;
		myFog.endDistance=3000;
		break;
	case 3:
		myFog.type=3;
		myFog.fogColor[0]=178; myFog.fogColor[1]=178; myFog.fogColor[2]=255;	
		myFog.density=0.00001;
		myFog.startDistance=1000;
		myFog.endDistance=3000;
		break;
	default:
		break;
	}
	setFog();
}

void EditScene::setFog()
{
	BaseApplication* appMgr=BaseApplication::getInstance();
	switch(myFog.type)
	{
	case 0:
		appMgr->mSceneMgr->setFog(Ogre::FOG_NONE);	
		break;
	case 1:
		appMgr->mSceneMgr->setFog(Ogre::FOG_LINEAR,
			Ogre::ColourValue(myFog.fogColor[0]/255,myFog.fogColor[1]/255,myFog.fogColor[2]/255,1.0),
			myFog.density, myFog.startDistance, myFog.endDistance);	
		break;
	case 2:
		appMgr->mSceneMgr->setFog(Ogre::FOG_EXP,
			Ogre::ColourValue(myFog.fogColor[0]/255,myFog.fogColor[1]/255,myFog.fogColor[2]/255,1.0),
			myFog.density, myFog.startDistance, myFog.endDistance);	
		break;
	case 3:
		appMgr->mSceneMgr->setFog(Ogre::FOG_EXP2,
			Ogre::ColourValue(myFog.fogColor[0]/255,myFog.fogColor[1]/255,myFog.fogColor[2]/255,1.0),
			myFog.density, myFog.startDistance, myFog.endDistance);	
		break;
	default:
		break;
	}
}

void EditScene::deleteSceneObj( SceneObj* obj )
{
	if (obj == NULL)
	{
		return;
	}

	Ogre::String name=obj->mName;
	SceneNode->removeChild(obj->mNode);
	Ogre::String subName=name.substr(0,4);
	if (subName=="Mesh")
	{
		for (std::list<SceneMeshObj*>::iterator i=mMeshObjList.begin();i!=mMeshObjList.end();i++)
		{
			if ((*i)->mName==name)   
			{
				//删除之前先要解除与其所有的绑定关系//
				for (std::list<SceneSoundObj*>::iterator j=mSoundList.begin();j!=mSoundList.end();j++)
				{
					if ((*j)->mBingObj!=NULL)
					{
						if ((*j)->mBingObj->mName==name)
						{
							(*j)->setBingObj(NULL);						
						}
					}

				}
				deletePtr(*i);  
				mMeshObjList.remove(*i);
				break;
			}
		}
	}
	else if (subName=="Soun")
	{
		for (std::list<SceneSoundObj*>::iterator j=mSoundList.begin();j!=mSoundList.end();j++)
		{
			if ((*j)->mName==name)
			{
				deletePtr(*j);  
				mSoundList.remove(*j);
				break;
			}
		}
	}
	else if (subName=="Phys")
	{
		for (std::list<PhysicsBox*>::iterator j=mPhysisBoxList.begin();j!=mPhysisBoxList.end();j++)
		{
			if ((*j)->mName==name)
			{
				for (std::list<SceneMeshObj*>::iterator i=mMeshObjList.begin();i!=mMeshObjList.end();i++)
				{
					if ((*i)->mPhysicsBox!=NULL)
					{
						PhysicsBox* pBox=(PhysicsBox*)((*i)->mPhysicsBox);
						if (pBox->mName == name)
						{
							(*i)->setmPhysicsBox(NULL);
							break;
						}
					}
				}
				deletePtr(*j);  
				mPhysisBoxList.remove(*j);
				break;
			}
		}
	}
	else if (subName=="Ligh")
	{
		SceneLightObj* lightobj=(SceneLightObj*)findSceneObjByName(name,3);
		deletePtr(lightobj);  
		mLightObjList.remove(lightobj);
	}
	else if (subName=="Part")
	{
		SceneParticleObj* particleobj=(SceneParticleObj*)findSceneObjByName(name,4);
		deletePtr(particleobj);
		mParticleList.remove(particleobj);
	}


}

void EditScene::clearScene()
{
	selectSceneObj.clear();
	mhideSceneObj.clear();
	destroyTerrain();
	
	for (std::list<PhysicsBox*>::iterator i=mPhysisBoxList.begin();i!=mPhysisBoxList.end();)
	{
		PhysicsBox* obj = *i;
		i++;
		if (obj !=NULL) { deleteSceneObj(obj); }
	}

	for (std::list<SceneSoundObj*>::iterator i=mSoundList.begin();i!=mSoundList.end();)
	{
		SceneSoundObj* obj = *i;
		i++;
		if (obj !=NULL) { deleteSceneObj(obj); }
	}

	for (std::list<SceneMeshObj*>::iterator i=mMeshObjList.begin();i!=mMeshObjList.end();)
	{
		SceneMeshObj* obj = *i;
		i++;
		if (obj !=NULL) { deleteSceneObj(obj); }
	}
	
	
	for (std::list<SceneParticleObj*>::iterator i=mParticleList.begin();i!=mParticleList.end();)
	{
		SceneParticleObj* obj = *i;
		i++;
		if (obj !=NULL) { deleteSceneObj(obj); }
	}
	for (std::list<SceneLightObj*>::iterator i=mLightObjList.begin();i!=mLightObjList.end();)
	{
		SceneLightObj* obj =*i;
		i++;
		if (obj !=NULL) { deleteSceneObj(obj); }
	}

	if (bPhysisCreate)
	{
		dJointGroupEmpty (contactgroup);
		dJointGroupDestroy (contactgroup);
		dSpaceDestroy (mOdeSceneSpace);
		dWorldDestroy (mOdeWorld);
		dCloseODE();
		bPhysisCreate = false;
	}
	
	SceneNode->removeAndDestroyAllChildren();
	initScene();
	CMainFrame* pMain=(CMainFrame*)AfxGetMainWnd();
	pMain->m_wndSceneObjView.FillSceneTree();

}

void EditScene::initPhysics()
{
	dInitODE2(0);
	mOdeWorld = dWorldCreate();
	mOdeSceneSpace = dHashSpaceCreate(NULL);
	contactgroup = dJointGroupCreate (0);
	dWorldSetGravity (mOdeWorld,gravity.x,gravity.y,gravity.z);
	dWorldSetCFM(mOdeWorld,softCFM);
	dWorldSetAutoDisableFlag (mOdeWorld,bEnagleAutoSleep);  //自动休眠关闭//
	dWorldSetContactSurfaceLayer (mOdeWorld,ContaceSurfaceLayer);
	dWorldSetQuickStepNumIterations (mOdeWorld, QuicksetpNumIterations);
	bPhysisCreate = true;
	
	heroGemo=dCreateBox(mOdeSceneSpace,10,20,10);
	dMass m;
	sphbody = dBodyCreate (mOdeWorld);
	dMassSetSphereTotal(&m,0.2,4);
	dBodySetMass (sphbody,&m);
	sphgeom = dCreateSphere(mOdeSceneSpace, 4);
	dGeomSetBody (sphgeom,sphbody);
	float sx=50.0f, sy=2.0f, sz=20.0f;
	dBodySetPosition (sphbody, sx, sy, sz);
	dBodySetLinearVel (sphbody, 0,0,0);
	dBodySetAngularVel (sphbody, 0,0,0);

	dGeomSetData(sphgeom,ballNode);
}

void EditScene::defineTerrain( long x, long y, bool flat /*= false*/ )
{

}

void EditScene::initBlendMaps( Terrain* terrain )
{
	TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	Real minHeight0 = 70;
	Real fadeDist0 = 40;
	Real minHeight1 = 70;
	Real fadeDist1 = 15;
	float* pBlend1 = blendMap1->getBlendPointer();
	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{

		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Real val = (height - minHeight0) / fadeDist0;
			Math::Clamp(val, (Real)0, (Real)1);

			val = (height - minHeight1) / fadeDist1;
			val = Math::Clamp(val, (Real)0, (Real)1);
			*pBlend1++ = val;
		}

	}
	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();

}

void EditScene::initTerrainSystem()
{
	BaseApplication* appMgr = BaseApplication::getInstance();
	// create terrain manager
	mTerrainGlobals = new TerrainGlobalOptions();
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(50000);
	//mTerrainGlobals->setLightMapDirection(l->getDerivedDirection());//地图光照方向（和实时阴影生成相关）
	mTerrainGlobals->setLightMapDirection(Ogre::Vector3(0,-1,0));
	//mTerrainGlobals->setCompositeMapAmbient(Ogre::ColourValue(0.7,0.7,0.7));
	mTerrainGlobals->setCompositeMapDiffuse(Ogre::ColourValue(0.5,0.5,0.5));
	//mTerrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());
	mTerrainGlobals->setCastsDynamicShadows(true);

	ResourceGroupManager::getSingleton().createResourceGroup("Terrain");
	ResourceGroupManager::getSingleton().addResourceLocation("", "FileSystem", "Terrain");
	MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
	MaterialManager::getSingleton().setDefaultAnisotropy(6);


	mTerrainGroup = new TerrainGroup(appMgr->mSceneMgr, Terrain::ALIGN_X_Z, 129, 5000);   //y up, terrain size, world size
	mTerrainGroup->setFilenameConvention("Terrain", "dat");
	mTerrainGroup->setOrigin(Vector3(0,0,0));  //地形中心点//
	mTerrainGroup->setResourceGroup("Terrain");

	// Configure default import settings for if we use imported image
	Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = 257;
	defaultimp.worldSize = 5000.0f;
	defaultimp.inputScale = 1000;
	defaultimp.minBatchSize = 65;
	defaultimp.maxBatchSize = 65;

}

void EditScene::loadTerrain()
{
	for (int x=0;x<1;x++)
	{
		for (int y=0;y<3;y++)
		{
			String filename = mTerrainGroup->generateFilename(x, y);
			if (ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
			{
				mTerrainGroup->defineTerrain(x, y);
			}
			else
			{
				Image img;  //从高度图创建地形///
				getTerrainImage(x % 2 != 0, y % 2 != 0, img);
				mTerrainGroup->defineTerrain(x, y, &img);
			}
		}
	}
	mTerrainGroup->loadAllTerrains(true);
	mTerrainGroup->freeTemporaryResources();

	for (int x=0;x<1;x++)
	{
		for (int y=0;y<3;y++)
		{
			mTerrainGroup->getTerrain(x,y)->dirtyLightmap();
			mTerrainGroup->getTerrain(x,y)->_setLightMapRequired(true);
		}
	}

	bTerrainCreated = true;
}

void EditScene::getTerrainImage( bool flipX, bool flipY, Image& img )
{	
	img.load("terrain.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}


void nearCallback( void *data, dGeomID o1, dGeomID o2 )
{
	EditScene* mscene=(EditScene*)data;
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
	{
	dSpaceCollide2(o1,o2,data,&nearCallback);
	return;
	}

	const int N = 32;
	dContact contact[N];
	int n = dCollide (o1,o2,N,&(contact[0].geom),sizeof(dContact));

	////debug
	//int geomclass1=dGeomGetClass(o1);
	//int geomclass2=dGeomGetClass(o2);
	//if (dGeomGetData(o1)!=NULL||dGeomGetData(o2)!=NULL)
	//{
	//	if (geomclass1==9||geomclass2==9)   //都不是高度地形//
	//	{	
	//	}	
	//	else
	//	{
	//		if (n>0)
	//		{
	//			mscene->ball->setMaterialName("OgreSE/Yellow");
	//		}
	//		else
	//		{
	//			mscene->ball->setMaterialName("ball");
	//		}
	//	}
	//}
	//

	//if (n > 0) 
	//{
	//	for (int i=0; i<n; i++) 
	//	{
	//		contact[i].surface.slip1 = 0.7;
	//		contact[i].surface.slip2 = 0.7;
	//		contact[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1 | dContactSlip1 | dContactSlip2;
	//		contact[i].surface.mu = 50.0; // was: dInfinity
	//		contact[i].surface.soft_erp = 0.96;
	//		contact[i].surface.soft_cfm = 0.0001;
	//		dJointID c = dJointCreateContact (mscene->mOdeWorld,mscene->contactgroup,&contact[i]);
	//		if (dGeomGetData(o1)!=NULL)
	//		{
	//			if (dGeomGetClass(o2)==9)
	//			{
	//				dJointAttach (c,dGeomGetBody(contact[i].geom.g1),dGeomGetBody(contact[i].geom.g2));   //添加碰撞后的影响//
	//				return;
	//			}
	//			dJointAttach (c,0,dGeomGetBody(contact[i].geom.g2));   //添加碰撞后的影响//
	//		}
	//		if (dGeomGetData(o2)!=NULL)
	//		{
	//			if (dGeomGetClass(o1)==9)
	//			{
	//				dJointAttach (c,dGeomGetBody(contact[i].geom.g1),dGeomGetBody(contact[i].geom.g2));   //添加碰撞后的影响//
	//				return;
	//			}
	//			dJointAttach (c,dGeomGetBody(contact[i].geom.g1),0);   //添加碰撞后的影响//
	//		}
	//		
	//	}
	//}//debug

	
	//旧的碰撞方法//
	if (n > 0) 
	{
		for (int i=0; i<n; i++) 
		{
			contact[i].surface.slip1 = 0.7;
			contact[i].surface.slip2 = 0.7;
			contact[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1 | dContactSlip1 | dContactSlip2;
			contact[i].surface.mu = 50.0; // was: dInfinity
			contact[i].surface.soft_erp = 0.96;
			contact[i].surface.soft_cfm = 0.0001;
			dJointID c = dJointCreateContact (mscene->mOdeWorld,mscene->contactgroup,&contact[i]);
			dJointAttach (c,
				dGeomGetBody(contact[i].geom.g1),
				dGeomGetBody(contact[i].geom.g2));   //添加碰撞后的影响//
		}
	}
	
	
	/*
	//新的碰撞方法//
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

	dContact contact[32];   // up to MAX_CONTACTS contacts per box-box
	for (int i=0; i<32; i++) 
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.1;
		contact[i].surface.bounce_vel = 0.1;
		contact[i].surface.soft_cfm = 0.01;
	}
	if (int numc = dCollide (o1,o2,32,&contact[0].geom,sizeof(dContact))) 
	{
		dMatrix3 RI;
		dRSetIdentity (RI);
		const dReal ss[3] = {0.02,0.02,0.02};
		for (int i=0; i<numc; i++) 
		{
			dJointID c = dJointCreateContact (mscene->mOdeWorld,mscene->contactgroup,contact+i);
			dJointAttach (c,b1,b2);
		}
	}
	*/



}
