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

	unsigned int h,w,c;    //������Ϣ
	int l,t;
	

	EditScene *mscene;
	Hero *mHero;
	int viewSpeed;
	int viewMode;    //���ģʽ��1����ģʽ��2��ɫ�������ģʽ
	int mouseZViewSpeed;  //������ŵ���Զ�����ٶ�//
	int pressedMB;      //���µ���갴��,-1�ޣ�1�����2�Ҽ�
	unsigned short int PressedKey;    //�õ���λ�Ķ�����0|1��ʾ����wsad�Ƿ��� ����λ��ʾ��������,Q E R F H LeftControl num0 space
	Ogre::Vector3 oldMousePos, newMousePos;   //��¼������꣬�ж��ƶ�
	EditPointor* mEditPointor;

};

#endif // #ifndef __INPUTMANAGER__
