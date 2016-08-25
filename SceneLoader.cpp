#include "stdafx.h"
#include "SceneLoader.h"
#include "InputManager.h"
#include "MainFrm.h"

SceneLoader::SceneLoader( EditScene* scene )
{
	mscene = scene;
}

void SceneLoader::saveScene(std::vector<CString> &resPath, Ogre::String savepath)
{
	initDatabase(savepath);
	saveSetting(resPath);
	if (mscene->bTerrainCreated)
	{
		saveTerrain(savepath);
	}
	savesky();
	savefog();
	saveMesh();
	savePhysicsBox();
	saveSound();
	saveParticle();
	saveLight();
	sqlite3_close(db);

}

void SceneLoader::saveSetting(std::vector<CString> &resPath)
{
	std::string strSQL;
	for (int i=0; i<resPath.size(); i++)
	{
		strSQL = "insert into resource_t VALUES (";
		CStringA stra(resPath.at(i).GetBuffer(0));
		std::string value=stra.GetBuffer(0);
		strSQL += "\"";
		strSQL += value.c_str();
		strSQL += "\")";
		sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	}

	if(mscene->bTerrainCreated)
	{
		for (int i=0; i<=6; i++)
		{
			/*
			strSQL = "insert into tex_t VALUES (";
			if (!CString(mscene->TexturePaths[i].c_str()).IsEmpty())
			{
				Ogre::String OName,OPath;
				Ogre::StringUtil::splitFilename(mscene->TexturePaths[i],OName,OPath);
				strSQL += "\"";
				strSQL += OName.c_str();
				strSQL += "\");";
				sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
			}
			*/
		}
	}
	

	strSQL = "insert into scene_t(mUnit, camspeed, rolespeed ,physicson,bterraincreate,\
			 bautosleep, gravityx, gravityy, gravityz) VALUES (";
	char str[32];
	sprintf(str, "%f,",mscene->mUnit);								strSQL += str;
	sprintf(str, "%d,",InputManager::getInstance()->viewSpeed);		strSQL += str;
	sprintf(str, "%f,",Hero::getInstance()->mMoveSpeed);			strSQL += str;
	sprintf(str, "%d,",mscene->bOpenPhysis);						strSQL +=str;
	sprintf(str, "%d,",mscene->bTerrainCreated);					strSQL += str;
	sprintf(str, "%d,",mscene->bEnagleAutoSleep);					strSQL += str;
	sprintf(str, "%f,",mscene->gravity.x);							strSQL += str;
	sprintf(str, "%f,",mscene->gravity.y);							strSQL += str;
	sprintf(str, "%f",mscene->gravity.z);							strSQL += str;
	strSQL += ");";
	sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
}

void SceneLoader::saveMesh()
{
	std::string strSQL;
	for (std::list<SceneMeshObj*>::iterator i=mscene->mMeshObjList.begin(); i!=mscene->mMeshObjList.end(); ++i)
	{	
		strSQL = "insert into mesh_t(tag, resname, username, posx, posy, posz, \
				 rotx, roty, rotz, bVisualable, scax, scay, scaz, psca, weight, auto, phytag) VALUES (";
		char str[32];
		sprintf(str, "%d,",(*i)->mTag);
		strSQL += str;
		strSQL +="\"";
		strSQL += (*i)->mResName.c_str();
		strSQL +="\",\""; 
		strSQL +=  (*i)->mUserName.c_str();
		strSQL += "\","; 
		sprintf(str, "%f,",(*i)->mPostion.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.y);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.z);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.y);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.z);
		strSQL += str;
		sprintf(str, "%d,",(*i)->bVisualable);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mScale.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mScale.y);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mScale.z);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mPrimaryScale);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mWeight);
		strSQL += str;
		sprintf(str, "%d",(*i)->bAutoSimulate);
		strSQL += str;
		if( NULL != (*i)->mPhysicsBox)
		{
			PhysicsBox *box = (PhysicsBox*)(*i)->mPhysicsBox;
			sprintf(str, ",%d",box->mTag);   strSQL += str;
			
		}
		else 
		{
			sprintf(str, ",%d",-1);       strSQL += str;
		}
		strSQL += ");";
		sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	}
}

void SceneLoader::saveSound()
{
	std::string strSQL;
	for (std::list<SceneSoundObj*>::iterator i=mscene->mSoundList.begin(); i!=mscene->mSoundList.end(); ++i)
	{	
		strSQL = "insert into sound_t(tag, resname, username, posx, posy, posz, \
				 volume,type,meshtag,priority, bvisualable, bloop  ) VALUES (";
		char str[32];
		sprintf(str, "%d,\"",(*i)->mTag);					strSQL += str;									
		strSQL += (*i)->mResName.c_str();					strSQL +="\",\""; 
		strSQL +=  (*i)->mUserName.c_str();					strSQL += "\","; 
		sprintf(str, "%f,",(*i)->mPostion.x);				strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.y);				strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.z);				strSQL += str;
		sprintf(str, "%f,", (*i)->mValume);					strSQL += str;
		sprintf(str, "%d,",(*i)->mSoundType);				strSQL += str;
		sprintf(str, "%d,", (*i)->meshTag);					strSQL += str;
		sprintf(str, "%d,",(*i)->mPriority);				strSQL += str;
		sprintf(str,"%d,",(*i)->bVisualable);				strSQL += str;
		sprintf(str,"%d",(*i)->bLoop);						strSQL += str;
		strSQL += ");";
		sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	}
}

void SceneLoader::saveParticle()
{
	std::string strSQL;
	for (std::list<SceneParticleObj*>::iterator i=mscene->mParticleList.begin(); i!=mscene->mParticleList.end(); ++i)
	{	
		strSQL = "insert into particle_t(tag, resname, username, posx, posy, posz, \
				 rotx, roty, rotz, bVisualable, scax, scay, scaz) values (";
		char str[32];
		sprintf(str, "%d,",(*i)->mTag);
		strSQL += str;
		strSQL +="\"";
		strSQL += (*i)->mResName.c_str();
		strSQL +="\",\""; 
		strSQL +=  (*i)->mUserName.c_str();
		strSQL += "\","; 
		sprintf(str, "%f,",(*i)->mPostion.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.y);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.z);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.y);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.z);
		strSQL += str;
		sprintf(str, "%d,",(*i)->bVisualable);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mScale.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mScale.y);
		strSQL += str;
		sprintf(str, "%f",(*i)->mScale.z);
		strSQL += str;
		strSQL += ");";
		sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	}
}

void SceneLoader::saveLight()
{
	std::string strSQL;
	for (std::list<SceneLightObj*>::iterator i=mscene->mLightObjList.begin(); i!=mscene->mLightObjList.end(); ++i)
	{	
		strSQL = "insert into light_t(tag, type, username, posx, posy, posz, \
			rotx, roty, rotz, bVisualable,bcastshadow,mpower,mdifcolor1,mdifcolor2,mdifcolor3,\
			mspecolor1, mspecolor2,mspecolor3,farclip, nearclip,shadowfar, influentce,\
			innerdegree, outdegree, falloff,constparam,lintparam,quadparam) VALUES (";
		char str[32];
		sprintf(str, "%d,",(*i)->mTag);			strSQL += str;
		sprintf(str, "%d,",(*i)->mType);		strSQL += str;		strSQL +="\"";
		strSQL +=  (*i)->mUserName.c_str();		strSQL += "\","; 
		sprintf(str, "%f,",(*i)->mPostion.x);	strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.y);	strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.z);	strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.x);	strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.y);	strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.z);	strSQL += str;
		sprintf(str, "%d,",(*i)->bVisualable);	strSQL += str;
		sprintf(str, "%d,",(*i)->bCastShadow);	strSQL += str;
		sprintf(str, "%f,",(*i)->mPower);		strSQL += str;		
		sprintf(str, "%f,",(*i)->mdiffuseColor[0]);		strSQL += str;	//ogre的colourValue可以当数组用//
		sprintf(str, "%f,",(*i)->mdiffuseColor[1]);		strSQL += str;
		sprintf(str, "%f,",(*i)->mdiffuseColor[2]);		strSQL += str;
		sprintf(str, "%f,",(*i)->mspecularColor[0]);	strSQL += str;
		sprintf(str, "%f,",(*i)->mspecularColor[1]);	strSQL += str;
		sprintf(str, "%f,",(*i)->mspecularColor[2]);	strSQL += str;
		sprintf(str, "%d,",(*i)->farClipDistance);		strSQL += str;
		sprintf(str, "%d,",(*i)->nearClipDistance);		strSQL += str;
		sprintf(str, "%d,",(*i)->shadowFarDistance);	strSQL += str;
		sprintf(str, "%d,",(*i)->influenceDistance);	strSQL += str;
		sprintf(str, "%f,",(*i)->innerDegree);			strSQL += str;
		sprintf(str, "%f,",(*i)->outDegree);			strSQL += str;
		sprintf(str, "%f,",(*i)->fallOff);				strSQL += str;
		sprintf(str, "%f,",(*i)->constParam);			strSQL += str;
		sprintf(str, "%f,",(*i)->lineParam);			strSQL += str;
		sprintf(str, "%f",(*i)->quadParam);			strSQL += str;
		strSQL += ");";
		sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	}
}

void SceneLoader::savePhysicsBox()
{
	std::string strSQL;
	for (std::list<PhysicsBox*>::iterator i=mscene->mPhysisBoxList.begin(); i!=mscene->mPhysisBoxList.end(); ++i)
	{	
		strSQL = "insert into physics_t(tag, type, username, posx, posy, posz, \
				 rotx, roty, rotz, bVisualable, scax, scay, scaz) VALUES (";
		char str[32];
		sprintf(str, "%d,",(*i)->mTag);
		strSQL += str;
		sprintf(str, "%d,\"",(*i)->mType);
		strSQL += str;
		strSQL +=  (*i)->mUserName.c_str();
		strSQL += "\","; 
		sprintf(str, "%f,",(*i)->mPostion.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.y);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mPostion.z);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.y);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mRotaion.z);
		strSQL += str;
		sprintf(str, "%d,",(*i)->bVisualable);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mScale.x);
		strSQL += str;
		sprintf(str, "%f,",(*i)->mScale.y);
		strSQL += str;
		sprintf(str, "%f",(*i)->mScale.z);
		strSQL += str;
		strSQL += ");";
		sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	}
}

void SceneLoader::savesky()
{
	std::string strSQL = "insert into sky_t(type, backColor1, backColor2, backColor3, MateralName, \
				 hight, xcut, ycut, tiles, size, pcurvity) VALUES (";
		char str[32];
		sprintf(str, "%d,",mscene->mySky.type);					strSQL += str;
		sprintf(str, "%f,",mscene->mySky.backColor[0]);			strSQL += str;
		sprintf(str, "%f,",mscene->mySky.backColor[1]);			strSQL += str;
		sprintf(str, "%f,\"",mscene->mySky.backColor[2]);		strSQL += str;
		strSQL += mscene->mySky.MateralName.c_str();            strSQL += "\",";
		sprintf(str, "%f,",mscene->mySky.hight);				strSQL += str;
		sprintf(str, "%d,",mscene->mySky.xcut);					strSQL += str;
		sprintf(str, "%d,",mscene->mySky.ycut);					strSQL += str;
		sprintf(str, "%d,",mscene->mySky.tiles);				strSQL += str;
		sprintf(str, "%d,",mscene->mySky.size);					strSQL += str;
		sprintf(str, "%f",mscene->mySky.pCurvity);				strSQL += str;
		strSQL += ");";
		sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	

}

void SceneLoader::savefog()
{
	std::string strSQL = "insert into fog_t(type, density, fogcolor1, fogcolor2, fogcolor3, \
						 startDistance, endDistance) VALUES (";
	char str[32];
	sprintf(str, "%d,",mscene->myFog.type);					strSQL += str;
	sprintf(str, "%f,",mscene->myFog.density);				strSQL += str;
	sprintf(str, "%f,",mscene->myFog.fogColor[0]);			strSQL += str;
	sprintf(str, "%f,",mscene->myFog.fogColor[1]);			strSQL += str;
	sprintf(str, "%f,",mscene->myFog.fogColor[2]);			strSQL += str;
	sprintf(str, "%d,",mscene->myFog.startDistance);		strSQL += str;
	sprintf(str, "%d",mscene->myFog.endDistance);			strSQL += str;
	strSQL += ");";
	sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
}

void SceneLoader::saveTerrain( Ogre::String &savePath )
{
	/*
	Ogre::String filePath, fileName, fileExt;
	Ogre::StringUtil::splitFullFilename(savePath,fileName,fileExt,filePath);
	std::string strSQL = "insert into terrain_t(scenename, length, width ,height,\
					 baseheight, numtiles) values(";
	strSQL +="\"";
	strSQL += fileName.c_str();
	strSQL +="\"";
	Ogre::AxisAlignedBox terrainExt = mscene->pTerrainInfo->getExtents();
	Ogre::Vector3 size = terrainExt.getSize();
	char str[32];
	sprintf(str, ",%f,",size.z);
	strSQL += str;
	sprintf(str, "%f,",size.x);
	strSQL += str;
	sprintf(str, "%f,",size.y);
	strSQL += str;	
	sprintf(str, "%f,",mscene->TerrainBaseHight);
	strSQL += str;	
	sprintf(str, "%d",mscene->TerrainNumTiles);
	strSQL += str;
	strSQL += ");";
	int res = sqlite3_exec(db, strSQL.c_str(), 0, 0, 0);
	if (res != SQLITE_OK)
	{
		return;
	}
	Image image;
	ET::saveHeightmapToImage(*(mscene->pTerrainInfo), image);
	image.save(filePath+"/"+fileName+"HightMap.png");

	int numMaps = mscene->mSplatMgr->getNumMaps();
	for (uint i = 0; i < numMaps; ++i)
	{
		mscene->mSplatMgr->saveMapToImage(i, image);
		image.save(filePath+"/"+fileName+"ETcoverage"+StringConverter::toString(i)+".png");
	}
	// now the lightmap
	Image lightmap;
	ET::createTerrainLightmap(*(mscene->pTerrainInfo), lightmap, 512, 512, Vector3(1, -1, 1), ColourValue(1,1,1),
		ColourValue(0.3, 0.3,  0.3));
	lightmap.save(filePath+"/"+fileName+"LightMap.png");

	// generate a base texture for this terrain (could be used for older hardware instead of splatting)
	// create an ImageList of our splatting textures.
	ET::ImageList textures (mscene->TextureNums);
	for (int i = 0; i <mscene->TextureNums; ++i)
	{
		if (!CString(mscene->TexturePaths[i].c_str()).IsEmpty())
		{
			Ogre::String filename1,fileFolder1;
			Ogre::StringUtil::splitFilename(mscene->TexturePaths[i],filename1,fileFolder1); 
			textures[i].load(filename1, "ET");
			Ogre::String newFilePath=filePath+filename1;
			CopyFile(CString(mscene->TexturePaths[i].c_str()),CString(newFilePath.c_str()),true);
		}
	}
	// create the base texture
	Image baseTexture;
	mscene->mSplatMgr->createBaseTexture(baseTexture, 512, 512, textures, 20, 20);
	baseTexture.save(filePath+"/"+fileName+"Base.png");

	// finally create a minimap using the lightmap and the generated base texture
	//Image minimap = ET::createMinimap(baseTexture, lightmap);
	//minimap.save(filePath+"/"+fileName+"Minimap.png");

	*/
}

void SceneLoader::initDatabase(Ogre::String &dbPath)
{
	db = NULL;  
	int nRes = sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,0);  
	if (nRes != SQLITE_OK)  
	{  
		return;
	}
	std::string deleteResTableSQL = "drop table resource_t";
	std::string createResTableSQL = "create table resource_t(respath TEXT);";
	std::string deleteTextureTableSQL = "drop table tex_t";
	std::string createTextureTableSQl = "create table tex_t(texname TEXT);";
	std::string deleteSkyTableSQL = "drop table sky_t";
	std::string createSkyTableSQL = "create table sky_t(type INTEGER, backColor1 REAL, backColor2 REAL, \
		backColor3 REAL, MateralName TEXT, hight REAL, xcut INTEGER, ycut INTEGER, tiles INTEGER, size INTEGER,\
		pcurvity REAL);";
	std::string deleteFogTableSQL = "drop table fog_t;";
	std::string createFogTableSQL = "create table fog_t(type INTEGER, density REAL, fogcolor1 REAL,\
									fogcolor2 REAL, fogcolor3 REAL,startDistance INTEGER, endDistance INTEGER);";
	std::string deleteSceneTableSQL = "drop table scene_t";
	std::string createSceneTableSQL = "create table scene_t(mUnit TEXT, camspeed REAL, rolespeed REAL, \
	  physicson BOOLEAN, bterraincreate BOOLEAN, bautosleep BOOLEAN, gravityx REAL, gravityy REAL, gravityz REAL)";
	std::string deleteTerrainTableSQL = "drop table terrain_t";
	std::string createTerrainTableSQL = "create table terrain_t(scenename TEXT,length REAL,width REAL,\
		height REAL, baseheight REAL, numtiles INTEGER)";
	std::string deleteMeshTableSQL = "drop table mesh_t";
	std::string createMeshTableSQL= "create table mesh_t(tag INTEGER, resname TEXT,username TEXT, \
	posx REAL, posy REAL, posz REAL, rotx REAL, roty REAL ,rotz REAL, bVisualable BOOLEAN, \
	scax REAL, scay REAL, scaz REAL, psca REAL, weight REAL, auto BOOLEAN, phytag INTEGER);";
	std::string deletePhysicsTableSQL = "drop table physics_t";
	std::string createPhysicsTableSQL = "create table physics_t(tag INTEGER, type INTEGER ,username TEXT, \
	posx REAL, posy REAL, posz REAL, rotx REAL, roty REAL ,rotz REAL, bVisualable BOOLEAN, \
	scax REAL, scay REAL, scaz REAL);";
	std::string deleteSoundTableSQL = "drop table sound_t";
	std::string createSoundTableSQL = "create table sound_t(tag INTEGER, resname TEXT, username TEXT,\
	 posx REAL, posy REAL, posz REAL, volume REAL, type INTEGER, meshtag INTEGER, \
	 priority INTEGER, bvisualable BOOLEAN, bloop BOOLEAN);";
	std::string deleteParticleTableSQL = "drop table particle_t";
	std::string createParticleTableSQL = "create table particle_t(tag INTEGER, resname TEXT, username TEXT, \
	 posx REAL, posy REAL, posz REAL, rotx REAL, roty REAL ,rotz REAL, bVisualable BOOLEAN, \
	scax REAL, scay REAL, scaz REAL);";
	std::string deleteLightTableSQL = "drop table light_t";
	std::string createLightTableSQL = "create table light_t(tag INTEGER, type INTEGER, username TEXT, \
	 posx REAL, posy REAL, posz REAL, rotx REAL, roty REAL ,rotz REAL, bVisualable BOOLEAN, \
	 bcastshadow BOOLEAN,mpower REAL,mdifcolor1 REAL,mdifcolor2 REAL, mdifcolor3 REAL,mspecolor1 REAL,\
	 mspecolor2 REAL, mspecolor3 REAL, farclip INTEGER, nearclip INTEGER, shadowfar INTEGER,\
	 influentce INTEGER, innerdegree REAL, outdegree REAL, falloff REAL, constparam REAL, \
	 lintparam REAL, quadparam REAL);";

	sqlite3_exec(db,deleteSceneTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteResTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteTextureTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteSkyTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteFogTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteTerrainTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteMeshTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteSoundTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deletePhysicsTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteLightTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,deleteParticleTableSQL.c_str(),0,0,0);

	sqlite3_exec(db,createResTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createTextureTableSQl.c_str(),0,0,0);
	sqlite3_exec(db,createSceneTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createSkyTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createFogTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createTerrainTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createMeshTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createSoundTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createPhysicsTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createParticleTableSQL.c_str(),0,0,0);
	sqlite3_exec(db,createLightTableSQL.c_str(),0,0,0);
}

void SceneLoader::loadscene( Ogre::String &path )
{
	db = NULL;  
	int nRes = sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,0);  
	if (nRes != SQLITE_OK)  
	{  
		return;
	}
	loadSetting(path);     //先加载设置初始化路径///////
	mscene->initPhysics();
	if (mscene->bTerrainCreated)
	{
		//loadTerrain(path);
		mscene->gridNode->setVisible(false);
		CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
		pMain->calcuateLightMap();
	}
	loadsky();
	loadfog();
	loadMesh();
	loadSound();
	loadParticle();
	loadPhysicsBox();
    loadLight();
	loadfinish();    //当加载完毕后需要将声音和物理碰撞体与模型添加关联//
}

void SceneLoader::loadSetting(Ogre::String &path)
{
	int res = sqlite3_exec(db, "select * from resource_t;", resSettingSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}  
	res = sqlite3_exec(db, "select * from scene_t;", sceneSettingSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}  

	//Ogre::String Opath,Oname;
	//Ogre::StringUtil::splitFilename(path,Oname,Opath);
	//Ogre::String temp;
	//for (unsigned int i = 0; i < mscene->TextureNums; i++)        //添加纹理全路径//
	//{
	//	temp=Opath+mscene->TexturePaths[i];
	//	mscene->TexturePaths[i]= temp;
	//}

}

void SceneLoader::loadMesh()
{
	int res = sqlite3_exec(db, "select * from mesh_t;", meshSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	} 
}

void SceneLoader::loadSound()
{
	int res = sqlite3_exec(db, "select * from sound_t;", soundSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	} 
}

void SceneLoader::loadParticle()
{
	int res = sqlite3_exec(db, "select * from particle_t;", particleSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}
}

void SceneLoader::loadLight()
{
	int res = sqlite3_exec(db, "select * from light_t;", lightSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}
}

void SceneLoader::loadPhysicsBox()
{
	int res = sqlite3_exec(db, "select * from physics_t;", physicsBoxSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}
}

void SceneLoader::loadTerrain(Ogre::String &path)
{	
	/*
	Ogre::String opath, filename, fileEXT;
	Ogre::StringUtil::splitFullFilename(path,filename,fileEXT,opath);
	Image image;
	Ogre::String strSceneName=opath + filename;
	Ogre::String OHightMap=strSceneName + "HightMap.png";
	image.load(OHightMap, "ET");
	ET::loadHeightmapFromImage(mTerrainInfo, image);

	int res = sqlite3_exec(db, "select * from terrain_t;", terrainSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}

	if (mscene->bTerrainCreated)
	{
		mscene->pTerrainInfo = &mscene->mTerrainMgr->getTerrainInfo();

		//创建 splatting manager//
		if (mscene->mSplatMgr) delete mscene->mSplatMgr;	
		mscene->mSplatMgr= new ET::SplattingManager("ETSplatting", "ET", mscene->TerrainNumTiles, mscene->TerrainNumTiles, 3);
		mscene->mSplatMgr->setNumTextures(mscene->TextureNums);
		int numMaps = mscene->mSplatMgr->getNumMaps();
		for (uint i = 0; i <numMaps; ++i) 
		{
			image.load(strSceneName+"ETcoverage"+StringConverter::toString(i)+".png", "ET");
			mscene->mSplatMgr->loadMapFromImage(i, image);
		}


		// load the terrain material and assign it
		mscene->Terrainmaterial = MaterialManager::getSingleton().getByName("ETTerrainMaterial");
		Ogre::Pass::TextureUnitStateIterator i=mscene->Terrainmaterial->getTechnique(0)->getPass(0)->getTextureUnitStateIterator();
		while(i.hasMoreElements())
		{
			i.moveNext();
			i.moveNext();
			if (i.hasMoreElements())
			{
				mscene->Terrainmaterial->getTechnique(0)->getPass(0)->removeTextureUnitState(2);
				i=mscene->Terrainmaterial->getTechnique(0)->getPass(0)->getTextureUnitStateIterator();
			}else
			{
				break;
			}
		}
		int corset=2;	
		for (int i=0; i<mscene->TextureNums;i++)
		{
			Ogre::String filename,fileFolder;
			Ogre::StringUtil::splitFilename(mscene->TexturePaths[i],filename,fileFolder); 
			mscene->Terrainmaterial->getTechnique(0)->getPass(0)->createTextureUnitState(filename,corset);
			corset+=1;	
		}
		mscene->mTerrainMgr->setMaterial(mscene->Terrainmaterial);

		// create a manual lightmap texture
		TexturePtr lightmapTex = TextureManager::getSingleton().createManual(
			"ETLightmap", "ET", TEX_TYPE_2D, mscene->TerrainNumTiles, mscene->TerrainNumTiles, 1, PF_BYTE_RGB);	
		Image lightmap;
		Ogre::String OLightMap=strSceneName + "LightMap.png";
		image.load(OLightMap, "ET");
		ET::createTerrainLightmap(*(mscene->pTerrainInfo), lightmap,  mscene->TerrainNumTiles, 
			mscene->TerrainNumTiles, Vector3(1, -1, 1), ColourValue(1,1,1),ColourValue(0.3, 0.3,  0.3));
		TexturePtr tex = TextureManager::getSingleton().getByName("ETLightmap");
		tex->getBuffer(0, 0)->blitFromMemory(lightmap.getPixelBox(0, 0));

	}
	*/
}

void SceneLoader::loadsky()
{
	int res = sqlite3_exec(db, "select * from sky_t;", skySelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}
}

void SceneLoader::loadfog()
{
	int res = sqlite3_exec(db, "select * from fog_t;", fogSelectResult , this , 0);    
	if (res != SQLITE_OK)  
	{  
		return; 
	}
}

void SceneLoader::loadfinish()
{
	for (std::list<SceneMeshObj*>::iterator i=mscene->mMeshObjList.begin(); i!=mscene->mMeshObjList.end(); i++)
	{
		if ((*i)->mPhytag!=-1)
		{
			for(std::list<PhysicsBox*>::iterator j=mscene->mPhysisBoxList.begin();j!=mscene->mPhysisBoxList.end();j++)
			{
				if ((*j)->mTag == (*i)->mTag)
				{
					(*i)->setmPhysicsBox(*j);
					(*j)->InitBody();
					(*i)->setAutoSimulate((*i)->bAutoSimulate);
					break;
				}
			}
		}
	}

	for(std::list<SceneSoundObj*>::iterator i=mscene->mSoundList.begin();i!=mscene->mSoundList.end();i++)
	{
		if ((*i)->meshTag != -1)
		{
			for (std::list<SceneMeshObj*>::iterator j=mscene->mMeshObjList.begin(); j!=mscene->mMeshObjList.end(); j++)
			{
				if ((*j)->mTag == (*i)->meshTag)
				{
					(*i)->setBingObj(*j);
					break;
				}
			}
		}
	}

}

int resSettingSelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	CString keyStr, valueStr;
	for(int i = 0; i < argc ; i++)  
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr==_T("respath"))
		{
			pMain->AddResPath(valueStr);
		}
	}

	return 0;
}



int sceneSettingSelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	SceneLoader* loader = (SceneLoader*)pObj;
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	CString keyStr, valueStr;
	for(int i = 0; i < argc ; i++)  
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("mUnit")){	loader->mscene->mUnit = atof(argv[i]); }
		else if (keyStr == _T("camspeed")){InputManager::getInstance()->viewSpeed = atoi(argv[i]);}
		else if (keyStr == _T("rolespeed")){Hero::getInstance()->mMoveSpeed = atof(argv[i]);}
		else if (keyStr == _T("physicson")){loader->mscene->bOpenPhysis = atoi(argv[i]);}
		else if (keyStr == _T("bterraincreate")){loader->mscene->bTerrainCreated = atoi(argv[i]);}
		else if (keyStr == _T("bautosleep")){loader->mscene->bEnagleAutoSleep = atoi(argv[i]);}
		else if (keyStr == _T("gravityx")){loader->mscene->gravity.x=atof(argv[i]);}
		else if (keyStr == _T("gravityy")){loader->mscene->gravity.y=atof(argv[i]);}
		else if (keyStr == _T("gravityz")){loader->mscene->gravity.z=atof(argv[i]);}
	}
	return 0;
}

int meshSelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	SceneLoader* loader = (SceneLoader*)pObj;
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	CString keyStr, valueStr;
	int tag=0, phytag=-1;
	Ogre::String resname,username;
	float posx=0,posy=0,posz=0,rotx=0,roty=0,rotz=0;
	float scax=1,scay=1,scaz=1,psca=1,weight=50;
	bool bauto=false,bvisualable=true;

	for(int i = 0; i < argc ; i++)     //argc表示一条数据的字段数//
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("tag")){	tag=atoi(argv[i]); }
		if (keyStr == _T("resname")){resname=Ogre::String(argv[i]);}
		if (keyStr == _T("username")){username=Ogre::String(argv[i]);}
		if (keyStr == _T("posx")){posx=atof(argv[i]);}
		if (keyStr == _T("posy")){posy=atof(argv[i]);}
		if (keyStr == _T("posz")){posz=atof(argv[i]);}
		if (keyStr == _T("rotx")){rotx=atof(argv[i]);}
		if (keyStr == _T("roty")){roty=atof(argv[i]);}
		if (keyStr == _T("rotz")){rotz=atof(argv[i]);}
		if (keyStr == _T("bVisualable")){bvisualable=atoi(argv[i]);}
		if (keyStr == _T("scax")){scax=atof(argv[i]);}
		if (keyStr == _T("scay")){scay=atof(argv[i]);}
		if (keyStr == _T("scaz")){scaz=atof(argv[i]);}
		if (keyStr == _T("psca")){psca=atof(argv[i]);}
		if (keyStr == _T("weight")){weight=atof(argv[i]);}
		if (keyStr == _T("auto")){bauto=atoi(argv[i]);}
		if (keyStr == _T("phytag")){phytag=atoi(argv[i]);}
	}
		SceneMeshObj *temp=new SceneMeshObj(loader->mscene->meshTag,psca,resname);
		loader->mscene->meshTag += 1;
		temp->setPosition(Ogre::Vector3(posx,posy,posz));
		temp->setRotaion(Ogre::Vector3(rotx,roty,rotz));
		temp->setScale(Ogre::Vector3(scax,scay,scaz));
		temp->setVisualAble(bvisualable);
		temp->mTag = tag;
		temp->mUserName = username;
		temp->bAutoSimulate = bauto;
		temp->mWeight = weight;
		temp->mPhytag = phytag;
		loader->mscene->SceneNode->addChild(temp->mNode);	
		loader->mscene->mMeshObjList.push_back(temp);
		HTREEITEM hitem=pMain->m_wndSceneObjView.hMesh;
		pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),3,3,hitem);
		
	return 0;
}

int soundSelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	SceneLoader* loader = (SceneLoader*)pObj;
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	CString keyStr, valueStr;
	int tag=0, type=2,priority=128, meshTag=-1;
	Ogre::String resname,username;
	float posx=0,posy=0,posz=0,valume=1;
	bool bloop=false,bvisualable=true;

	for(int i = 0; i < argc ; i++)     //argc表示一条数据的字段数//
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("tag")){	tag=atoi(argv[i]); }
		else if (keyStr == _T("type")){	type=atoi(argv[i]); }
		else if (keyStr == _T("priority")){	priority=atoi(argv[i]); }
		else if (keyStr == _T("resname")){resname=Ogre::String(argv[i]);}
		else if (keyStr == _T("username")){username=Ogre::String(argv[i]);}
		else if (keyStr == _T("posx")){posx=atof(argv[i]);}
		else if (keyStr == _T("posy")){posy=atof(argv[i]);}
		else if (keyStr == _T("posz")){posz=atof(argv[i]);}
		else if (keyStr == _T("bVisualable")){bvisualable=atoi(argv[i]);}
		else if (keyStr == _T("bloop")){bloop=atoi(argv[i]);}
		else if (keyStr == _T("valume")){valume = atof(argv[i]);}
		else if (keyStr == _T("meshtag")){meshTag = atoi(argv[i]);}
	}
	SceneSoundObj *temp=new SceneSoundObj(loader->mscene->soundTag,resname);
	loader->mscene->soundTag +=1;
	temp->mTag = tag;
	temp->meshTag = meshTag;
	temp->setPosition(Ogre::Vector3(posx,posy,posz));
	temp->setVisualAble(bvisualable);
	temp->mUserName = username;
	temp->setSoundType(type);
	temp->setLoop(bloop);
	temp->setValume(valume);
	temp->setPriority(priority);
	

	loader->mscene->SceneNode->addChild(temp->mNode);	
	loader->mscene->mSoundList.push_back(temp);
	HTREEITEM hitem=pMain->m_wndSceneObjView.hSound;
	pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),6,6,hitem);
	
	return 0;
}

int particleSelectResult( void *pObj, int argc, char **argv, char **azColName )
{

	SceneLoader* loader = (SceneLoader*)pObj;
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	CString keyStr, valueStr;
	int tag=0;
	Ogre::String resname, username;
	float posx=0,posy=0,posz=0,rotx=0,roty=0,rotz=0;
	float scax=1.0,scay=1.0,scaz=1.0;
	bool bvisualable=true;

	for(int i = 0; i < argc ; i++)     //argc表示一条数据的字段数//
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("tag")){	tag=atoi(argv[i]); }
		else if (keyStr == _T("username")){username=Ogre::String(argv[i]);}
		else if (keyStr == _T("resname")){resname=Ogre::String(argv[i]);}
		else if (keyStr == _T("posx")){posx=atof(argv[i]);}
		else if (keyStr == _T("posy")){posy=atof(argv[i]);}
		else if (keyStr == _T("posz")){posz=atof(argv[i]);}
		else if (keyStr == _T("scax")){scax=atof(argv[i]);}
		else if (keyStr == _T("scay")){scay=atof(argv[i]);}
		else if (keyStr == _T("scaz")){scaz=atof(argv[i]);}
		else if (keyStr == _T("rotx")){rotx=atof(argv[i]);}
		else if (keyStr == _T("roty")){roty=atof(argv[i]);}
		else if (keyStr == _T("rotz")){rotz=atof(argv[i]);}
		else if (keyStr == _T("bVisualable")){bvisualable=atoi(argv[i]);}
	}

	SceneParticleObj *temp=new SceneParticleObj(loader->mscene->particaleTag,resname);
	loader->mscene->particaleTag += 1;
	temp->mTag = tag;
	temp->mUserName = username;
	temp->setScale(Ogre::Vector3(scax,scay,scaz));
	temp->setRotaion(Ogre::Vector3(rotx,roty,rotz));
	temp->setPosition(Ogre::Vector3(posx,posy,posz));
	temp->setVisualAble(bvisualable);
	
	loader->mscene->SceneNode->addChild(temp->mNode);	
	loader->mscene->mParticleList.push_back(temp);
	HTREEITEM hitem=pMain->m_wndSceneObjView.hParticle;
	pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),4,4,hitem);
	
	return 0;	
}

int lightSelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	SceneLoader* loader = (SceneLoader*)pObj;
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	CString keyStr, valueStr;
	int tag=0;
	LightType type=(LightType)1;
	Ogre::String username;
	float posx=0,posy=0,posz=0,rotx=0,roty=0,rotz=0;
	bool bvisualable=true, bcastshadow=true;
	float mpower=1.0,mdifcolor1=255,mdifcolor2=255,mdifcolor3=255;
	float mspecolor1=255,mspecolor2=255,mspecolor3=255;
	int farclip=1500,nearclip=200,shadowfar=1000,influence=1500;
	float innerdegree=30,outdegree=60,falloff=0;
	float constparam=1.0,lintparam=1.0,quadparam=0;


	for(int i = 0; i < argc ; i++)     //argc表示一条数据的字段数//
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("tag")){	tag=atoi(argv[i]); }
		if (keyStr == _T("type")){	type=(LightType)atoi(argv[i]); }
		else if (keyStr == _T("username")){username=Ogre::String(argv[i]);}
		else if (keyStr == _T("posx")){posx=atof(argv[i]);}
		else if (keyStr == _T("posy")){posy=atof(argv[i]);}
		else if (keyStr == _T("posz")){posz=atof(argv[i]);}
		else if (keyStr == _T("rotx")){rotx=atof(argv[i]);}
		else if (keyStr == _T("roty")){roty=atof(argv[i]);}
		else if (keyStr == _T("rotz")){rotz=atof(argv[i]);}
		else if (keyStr == _T("bVisualable")){bvisualable=atoi(argv[i]);}
		else if (keyStr == _T("bcastshadow")){bcastshadow=atoi(argv[i]);}
		else if (keyStr == _T("mpower")){bcastshadow=atof(argv[i]);}
		else if (keyStr == _T("mdifcolor1")){mdifcolor1=atof(argv[i]);}
		else if (keyStr == _T("mdifcolor2")){mdifcolor2=atof(argv[i]);}
		else if (keyStr == _T("mdifcolor3")){mdifcolor3=atof(argv[i]);}
		else if (keyStr == _T("mspecolor1")){mspecolor1=atof(argv[i]);}
		else if (keyStr == _T("mspecolor2")){mspecolor2=atof(argv[i]);}
		else if (keyStr == _T("mspecolor3")){mspecolor3=atof(argv[i]);}
		else if (keyStr == _T("farclip")){farclip=atoi(argv[i]);}
		else if (keyStr == _T("nearclip")){nearclip=atoi(argv[i]);}
		else if (keyStr == _T("shadowfar")){shadowfar=atoi(argv[i]);}
		else if (keyStr == _T("influentce")){influence=atoi(argv[i]);}
		else if (keyStr == _T("innerdegree")){innerdegree=atof(argv[i]);}
		else if (keyStr == _T("outdegree")){outdegree=atof(argv[i]);}
		else if (keyStr == _T("falloff")){falloff=atof(argv[i]);}
		else if (keyStr == _T("constparam")){constparam=atof(argv[i]);}
		else if (keyStr == _T("lintparam")){lintparam=atof(argv[i]);}
		else if (keyStr == _T("quadparam")){quadparam=atof(argv[i]);}
	
	}

	SceneLightObj *temp=new SceneLightObj(type,loader->mscene->lightTag);
	loader->mscene->lightTag += 1;
	temp->mTag = tag;
	temp->mUserName = username;
	temp->setRotaion(Ogre::Vector3(rotx,roty,rotz));
	temp->setPosition(Ogre::Vector3(posx,posy,posz));
	temp->setVisualAble(bvisualable);
	temp->mLight->setCastShadows(bcastshadow);
	temp->mLight->setAttenuation(influence,constparam,lintparam,quadparam);
	temp->mLight->setSpotlightRange(Ogre::Degree(innerdegree), Ogre::Degree(outdegree),falloff);
	temp->mLight->setDiffuseColour(Ogre::ColourValue(mdifcolor1,mdifcolor2,mdifcolor3));
	temp->mLight->setSpecularColour(Ogre::ColourValue(mspecolor1,mspecolor2,mspecolor3));
	temp->mLight->setShadowFarDistance(shadowfar);
	temp->mLight->setShadowFarClipDistance(farclip);
	temp->mLight->setShadowNearClipDistance(nearclip);
	

	loader->mscene->SceneNode->addChild(temp->mNode);	
	loader->mscene->mLightObjList.push_back(temp);
	HTREEITEM hitem=pMain->m_wndSceneObjView.hLight;
	pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),7,7,hitem);
	
	return 0;
}

int physicsBoxSelectResult( void *pObj, int argc, char **argv, char **azColName )
{

	SceneLoader* loader = (SceneLoader*)pObj;
	CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
	CString keyStr, valueStr;
	int tag=0;
	PhysicsBoxType type=(PhysicsBoxType)1;
	Ogre::String username;
	float posx=0,posy=0,posz=0,rotx=0,roty=0,rotz=0;
	float scax=1.0,scay=1.0,scaz=1.0;
	bool bvisualable=true;

	for(int i = 0; i < argc ; i++)     //argc表示一条数据的字段数//
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("tag")){	tag=atoi(argv[i]); }
		else if (keyStr == _T("type")){	type=(PhysicsBoxType)atoi(argv[i]); }
		else if (keyStr == _T("username")){username=Ogre::String(argv[i]);}
		else if (keyStr == _T("posx")){posx=atof(argv[i]);}
		else if (keyStr == _T("posy")){posy=atof(argv[i]);}
		else if (keyStr == _T("posz")){posz=atof(argv[i]);}
		else if (keyStr == _T("scax")){scax=atof(argv[i]);}
		else if (keyStr == _T("scay")){scay=atof(argv[i]);}
		else if (keyStr == _T("scaz")){scaz=atof(argv[i]);}
		else if (keyStr == _T("rotx")){rotx=atof(argv[i]);}
		else if (keyStr == _T("roty")){roty=atof(argv[i]);}
		else if (keyStr == _T("rotz")){rotz=atof(argv[i]);}
		else if (keyStr == _T("bVisualable")){bvisualable=atoi(argv[i]);}
	}

	PhysicsBox *temp=new PhysicsBox(loader->mscene->mOdeSceneSpace,type,loader->mscene->PhysicsBoxTag);
	loader->mscene->PhysicsBoxTag += 1;
	temp->mTag = tag;
	temp->mUserName = username;
	temp->setScale(Ogre::Vector3(scax,scay,scaz));
	temp->setRotaion(Ogre::Vector3(rotx,roty,rotz));
	temp->setPosition(Ogre::Vector3(posx,posy,posz));
	temp->setVisualAble(bvisualable);

	loader->mscene->SceneNode->addChild(temp->mNode);	
	loader->mscene->mPhysisBoxList.push_back(temp);
	HTREEITEM hitem=pMain->m_wndSceneObjView.hSurroundBox;
	pMain->m_wndSceneObjView.m_wndSceneObjTree.InsertItem(CString(temp->mUserName.c_str()),5,5,hitem);
	return 0;
}

int terrainSelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	/*
	SceneLoader* loader = (SceneLoader*)pObj;
	CString keyStr, valueStr;
	float length=500, width=500, height=500,BaseHight=0.2;
	int NumTiles=32;
	for(int i = 0; i < argc ; i++)  
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("length")){length = atof(argv[i]); }
		if (keyStr == _T("width")){width = atof(argv[i]); }
		if (keyStr == _T("height")){height = atof(argv[i]); }
		if (keyStr == _T("baseheight")){BaseHight = atof(argv[i]);}
		if (keyStr == _T("numtiles")){NumTiles = atoi(argv[i]);}
	}
	loader->mscene->TerrainBaseHight = BaseHight;
	loader->mscene->TerrainNumTiles = NumTiles;
	loader->mTerrainInfo.setExtents(AxisAlignedBox(-width/2,-height*BaseHight,-length/2,width/2, height-height*BaseHight, length/2));
	loader->mscene->mTerrainMgr->createTerrain(loader->mTerrainInfo);

	loader->mscene->bTerrainCreated = true;

	loader->mscene->createPhysisWorld(width,length,height,NumTiles,BaseHight);


	*/
	return 0;



}

int skySelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	SceneLoader* loader = (SceneLoader*)pObj;
	CString keyStr, valueStr;
	int type=0,xcut=10,ycut=10,tiles=10,size=2000;
	float backColor1=255,backColor2=255,backColor3=255,hight=5000,pcurvity=1.0;
	Ogre::String materialName;
	for(int i = 0; i < argc ; i++)  
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("type")){type = atoi(argv[i]); }
		else if (keyStr == _T("xcut")){xcut = atoi(argv[i]); }
		else if (keyStr == _T("ycut")){ycut = atoi(argv[i]); }
		else if (keyStr == _T("tiles")){tiles = atoi(argv[i]);}
		else if (keyStr == _T("size")){size= atoi(argv[i]);}
		else if (keyStr == _T("backColor1")){backColor1=atof(argv[i]);}
		else if (keyStr == _T("backColor2")){backColor2=atof(argv[i]);}
		else if (keyStr == _T("backColor3")){backColor3=atof(argv[i]);}
		else if (keyStr == _T("hight")){hight=atof(argv[i]);}
		else if (keyStr == _T("pcurvity")){pcurvity=atof(argv[i]);}
		else if (keyStr == _T("MateralName")){materialName=Ogre::String(argv[i]);}
	}
	loader->mscene->mySky.type = type;
	loader->mscene->mySky.hight = hight;
	loader->mscene->mySky.pCurvity = pcurvity;
	loader->mscene->mySky.size = size;
	loader->mscene->mySky.tiles = tiles;
	loader->mscene->mySky.xcut = xcut;
	loader->mscene->mySky.ycut = ycut;
	loader->mscene->mySky.MateralName = materialName;
	loader->mscene->mySky.backColor[0] = backColor1;
	loader->mscene->mySky.backColor[1] = backColor2;
	loader->mscene->mySky.backColor[2] = backColor3;
	loader->mscene->setSky(type);

	return 0;

}

int fogSelectResult( void *pObj, int argc, char **argv, char **azColName )
{
	SceneLoader* loader = (SceneLoader*)pObj;
	CString keyStr, valueStr;
	int type=0,startDistance=500, endDistance=5000;
	float density=0.0001, fogcolor1=255,fogcolor2=255,fogcolor3=255;
	for(int i = 0; i < argc ; i++)  
	{  
		keyStr=CString(azColName[i]);
		valueStr=CString(argv[i]);
		if (keyStr == _T("type")){type = atoi(argv[i]); }
		else if (keyStr == _T("density")){density = atof(argv[i]); }
		else if (keyStr == _T("fogcolor1")){fogcolor1 = atof(argv[i]); }
		else if (keyStr == _T("fogcolor2")){fogcolor2 = atof(argv[i]); }
		else if (keyStr == _T("fogcolor3")){fogcolor3 = atof(argv[i]); }
		else if (keyStr == _T("startDistance")){startDistance = atoi(argv[i]);}
		else if (keyStr == _T("endDistance")){endDistance= atoi(argv[i]);}
	}
	loader->mscene->myFog.type = type;
	loader->mscene->myFog.density = density;
	loader->mscene->myFog.endDistance = endDistance;
	loader->mscene->myFog.startDistance = startDistance;
	loader->mscene->myFog.fogColor[0] = fogcolor1;
	loader->mscene->myFog.fogColor[1] = fogcolor2;
	loader->mscene->myFog.fogColor[2] = fogcolor3;
	loader->mscene->setFog();


	return 0;
}
