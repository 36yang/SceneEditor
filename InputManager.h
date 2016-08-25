#ifndef __INPUTMANAGER__
#define __INPUTMANAGER__

#include "EditScene.h"
#include "Ogre.h"
#include "Hero.h"
#include "EditPointor.h"


class InputManager : public Ogre::FrameListener
{

public:
	InputManager(EditScene *scene);
	~InputManager();
	static InputManager* PmInstance;
	static InputManager *getInstance();  

	void changeViewMode();

	void keyPressed(MSG* pMsg);
	void keyReleased(MSG* pMsg);
	bool mouseMove(float dx,float dy,float dz);
	bool mousePress(int MBtnid);
	bool mouseRelease(int MBtnid);
	void upDateSoundSystem(float time);


	virtual bool frameStarted(const Ogre::FrameEvent &evt) ;  

	unsigned int h,w,c;    //窗口信息
	int l,t;
	

	EditScene *mscene;
	Hero *mHero;
	int viewSpeed;
	int viewMode;    //浏览模式，1自由模式，2角色仿真浏览模式
	int mouseZViewSpeed;  //鼠标缩放调节远近的速度//
	int pressedMB;      //按下的鼠标按键,-1无，1左键，2右键
	unsigned short int PressedKey;    //用低四位的二进制0|1表示按键wsad是否按下 高四位表示上下左右,Q E R F H LeftControl num0 space
	Ogre::Vector3 oldMousePos, newMousePos;   //记录鼠标坐标，判断移动
	EditPointor* mEditPointor;

};

#endif // #ifndef __INPUTMANAGER__
