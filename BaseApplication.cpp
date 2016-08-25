#pragma warning(disable:4251 4275)

#include "stdafx.h"
#include "BaseApplication.h"


BaseApplication *BaseApplication::PmInstance=NULL;


BaseApplication::BaseApplication(void) 
	: mRoot(0),  
	mCamera(0),
	mSceneMgr(0), 
	mWindow(0),
	mscene(0),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mShutDown(false)
{
	
}

BaseApplication::~BaseApplication(void)
{
	delete mRoot;
}

//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	//mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);     
}

void BaseApplication::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,0,0));
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(0.01);
	mCamera->setFarClipDistance(3000);

}

void BaseApplication::createViewports(void)
{
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BaseApplication::setupResources(void)
{
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}


}

void BaseApplication::createResourceListener(void)
{
}

void BaseApplication::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void BaseApplication::go(void)
{
	if (!setup())
		return;
}

bool BaseApplication::setup(void)
{
	
	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	setupResources();
	createResourceListener();
	loadResources();

	rootNode=mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mscene=new EditScene();
	rootNode->addChild(mscene->SceneNode);
	
	input=new InputManager(mscene);

	mRoot->addFrameListener(input);
	mRoot->addFrameListener(this);

	return true;
} 


BaseApplication* BaseApplication::getInstance()
{
	if (PmInstance==NULL)
	{
		PmInstance=new BaseApplication();
	}
	return PmInstance;
}



bool BaseApplication::frameStarted( const Ogre::FrameEvent &evt )
{

	return true;
}


void BaseApplication::createWindow( HWND m_hWnd, int width, int height )
{
#ifdef _DEBUG
	mPluginsCfg = "plugins_d.cfg";
#else
	mPluginsCfg = "plugins.cfg";
#endif

	mRoot = new Ogre::Root(mPluginsCfg);


	if (!mRoot->restoreConfig())     //直接读取配置文件，否则弹出配置框
	{
		if (!mRoot->showConfigDialog())
		{
			return ;
		}
	} 

	mRoot->initialise(false);//禁止ogre创建新的渲染窗口，而使用MFC的窗口  
	Ogre::NameValuePairList misc;  
	misc["externalWindowHandle"] = Ogre::StringConverter::toString((int)m_hWnd);   
	mWindow = mRoot->createRenderWindow("OgreRenderWindow", width, height, false, &misc);  

}
