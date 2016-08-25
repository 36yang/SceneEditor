#ifndef __SceneLoader_h_
#define __SceneLoader_h_

#include "Ogre.h"
#include "EditScene.h"
#include "src/sqlite/sqlite3.h"

using namespace Ogre;


class SceneLoader
{
private:
	sqlite3* db;

public:
	EditScene* mscene;

	SceneLoader(EditScene* scene);
     ~SceneLoader();
	void saveScene(std::vector<CString> &resPath, Ogre::String savepath);
	void initDatabase(Ogre::String &dbPath);
	void saveSetting(std::vector<CString> &resPath);
	void savesky();
	void savefog();
	void saveMesh();
	void saveSound();
	void saveParticle();
	void saveLight();
	void savePhysicsBox();
	void saveTerrain(Ogre::String &savePath);


	void loadscene(Ogre::String &path);
	void loadSetting(Ogre::String &path);
	void loadsky();
	void loadfog();
	void loadMesh();
	void loadSound();
	void loadParticle();
	void loadLight();
	void loadPhysicsBox();
	void loadTerrain(Ogre::String &path);
	void loadfinish();

	friend int resSettingSelectResult(void *pObj, int argc, char **argv, char **azColName); 
	friend int sceneSettingSelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int skySelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int fogSelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int meshSelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int soundSelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int particleSelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int lightSelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int physicsBoxSelectResult(void *pObj, int argc, char **argv, char **azColName);
	friend int terrainSelectResult(void *pObj, int argc, char **argv, char **azColName);

};

#endif // #ifndef __SceneLoader_h_
