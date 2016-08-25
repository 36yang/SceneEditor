#ifndef __Hero_h
#define __Hero_h

#pragma warning(disable:4251 4275)   //禁用vc警告

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
	 static Hero *getInstance(SceneNode *camNode);    //创建并返回单列
	 static Hero *getInstance();    //只能得到单列，不能创建

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
	 float mPivotPitch;        //轴心度
	 Vector3 mKeyDirection;      // player's local intended direction based on WASD keys
	 Vector3 mGoalDirection;     // actual intended direction in world-space
	 Real mVerticalVelocity;     // for jumping
	 Real mTimer;                // general timer to see how long animations have been playing
	
	 float mMoveSpeed;
	 float mFocusY;   //hero的中心高度
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
	 int IsupdateCamera;                //用来判断按键按下时更新视角,0不更新，1右转2左转3上转4下转


	
	~Hero();
	bool init();
	void setPositon(Vector3 &vec);  //必须用此方法设置位置
	void initCam();   //初始化摄像机
	void initBody();  //初始化人物
	void initAnimations();   //初始化动作
	//恢复正常状态
	void updateBody(float dTime);
	void updateCam(float dTime);
	void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);	
	void updateAnimations(float dTime);
	void fadeAnimations(float dTime);
	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);
	

	virtual bool frameStarted(const Ogre::FrameEvent &evt) ;  //渲染前执行的帧监听函数


};

























#endif // #ifndef __Hero_h