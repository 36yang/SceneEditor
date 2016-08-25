#ifndef __Hero_h
#define __Hero_h

#pragma warning(disable:4251 4275)   //����vc����

#include "ogre.h"

using namespace Ogre;

#define NUM_ANIMS 13           // number of animations the character has
#define TURN_SPEED 500.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second
#define GRAVITY 9.0f          // gravity in downward units per squared second


class Hero : public Ogre::FrameListener
{
private:
	Hero(SceneNode *camNode);
	Hero();
public:
	 static Hero* PmInstance;
	 static Hero *getInstance(SceneNode *camNode);    //���������ص���
	 static Hero *getInstance();    //ֻ�ܵõ����У����ܴ���

	 enum AnimID
	 {
		 ANIM_IDLE_BASE,
		 ANIM_IDLE_TOP,
		 ANIM_RUN_BASE,
		 ANIM_RUN_TOP,
		 ANIM_HANDS_CLOSED,
		 ANIM_HANDS_RELAXED,
		 ANIM_DRAW_SWORDS,
		 ANIM_SLICE_VERTICAL,
		 ANIM_SLICE_HORIZONTAL,
		 ANIM_DANCE,
		 ANIM_JUMP_START,
		 ANIM_JUMP_LOOP,
		 ANIM_JUMP_END,
		 ANIM_NONE
	 };

	 SceneManager* sceneMgr;
	 SceneNode* mCameraPivot;
	 SceneNode* mCameraGoal;
	 SceneNode* mCameraNode;
	 float mPivotPitch;        //���Ķ�
	 Vector3 mKeyDirection;      // player's local intended direction based on WASD keys
	 Vector3 mGoalDirection;     // actual intended direction in world-space
	 Real mVerticalVelocity;     // for jumping
	 Real mTimer;                // general timer to see how long animations have been playing
	
	 float mMoveSpeed;
	 float mFocusY;   //hero�����ĸ߶�
	 Vector3 mOldPos;
	 Vector3 mNewPos;
	 Entity* pent;
	 SceneNode* pnode;
	 Entity* mSword1;
	 Entity* mSword2;
	 RibbonTrail* mSwordTrail;
	 AnimationState* mAnims[NUM_ANIMS];    // master animation list
	 AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	 AnimID mTopAnimID;                    // current top (upper-body) animation
	 bool mFadingIn[NUM_ANIMS];            // which animations are fading in
	 bool mFadingOut[NUM_ANIMS];           // which animations are fading out
	 bool mSwordsDrawn;
	 int IsupdateCamera;                //�����жϰ�������ʱ�����ӽ�,0�����£�1��ת2��ת3��ת4��ת


	
	~Hero();
	bool init();
	void setPositon(Vector3 &vec);  //�����ô˷�������λ��
	void initCam();   //��ʼ�������
	void initBody();  //��ʼ������
	void initAnimations();   //��ʼ������
	//�ָ�����״̬
	void updateBody(float dTime);
	void updateCam(float dTime);
	void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);	
	void updateAnimations(float dTime);
	void fadeAnimations(float dTime);
	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);
	

	virtual bool frameStarted(const Ogre::FrameEvent &evt) ;  //��Ⱦǰִ�е�֡��������


};

























#endif // #ifndef __Hero_h