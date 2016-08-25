#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include "OgreResourceGroupManager.h"
#include "OgreLogManager.h"
#include "EditScene.h"
#include "InputManager.h"


class BaseApplication :public Ogre::FrameListener
{ 
public:
	BaseApplication(void);
	 ~BaseApplication(void);
	 void go(void);
	 static BaseApplication* PmInstance;
	 static BaseApplication *getInstance();  

	Ogre::SceneNode *rootNode;  //场景根节点
	EditScene *mscene;
	InputManager *input;
	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mPluginsCfg;
	bool mShutDown;

	void createWindow(HWND m_hWnd, int width, int height);

private:
	bool setup();
	void chooseSceneManager(void);
	void createCamera(void);
	void createViewports(void);
	void setupResources(void);
	void createResourceListener(void);
	void loadResources(void);
	virtual bool frameStarted(const Ogre::FrameEvent &evt);  //帧监听函数


};

#endif // #ifndef __BaseApplication_h_
