#include "stdafx.h"
#include "Hero.h"
#include "BaseApplication.h"

Hero *Hero::PmInstance=NULL;


Hero::Hero()
{
	init();
}

Hero::Hero(SceneNode *camNode)
{
	this->mCameraNode=camNode;
	sceneMgr=BaseApplication::getInstance()->mSceneMgr;
	// create main model
	pnode=sceneMgr->createSceneNode();
	pent=sceneMgr->createEntity("sinbad","Sinbad.mesh");
	pnode->attachObject(pent);
	pnode->setScale(2,2,2);
	Vector3 maxpos = pent->getWorldBoundingBox(true).getMaximum();
	Vector3 minpos = pent->getWorldBoundingBox(true).getMinimum();
	mFocusY=(maxpos.y-minpos.y)/2+1.5;    //hero 的中心点y值//
	pnode->setPosition(0,mFocusY,0);
	mOldPos=Vector3(0,0,0);
	mNewPos=Vector3(0,0,0);
	mMoveSpeed=40;
	init();
}

Hero::~Hero()
 {
	 delete PmInstance;
 }

Hero * Hero::getInstance(SceneNode *camNode)
{
	if(PmInstance == NULL)
	{ 
		PmInstance = new Hero(camNode);  
	}
	return PmInstance;
}

Hero * Hero::getInstance()     
{
	return PmInstance;
}

bool Hero::init()
{
	IsupdateCamera=0;
	initBody();
	initCam();	
	initAnimations();
	mCameraNode->setPosition(0,20,60);

	return true;
}
void Hero::initCam()
{
	/// create a pivot at roughly the character's shoulder
	mCameraPivot =sceneMgr->createSceneNode();  //视点中心//
	// this is where the camera should be soon, and it spins around the pivot
	mCameraGoal = mCameraPivot->createChildSceneNode(Vector3(0, 0, 30));
	// this is where the camera actually is
	mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

	mCameraPivot->setFixedYawAxis(true);
	mCameraGoal->setFixedYawAxis(true);
	mCameraNode->setFixedYawAxis(true);

	// our model is quite small, so reduce the clipping planes	
	sceneMgr->getCamera("PlayerCam")->setNearClipDistance(0.5);
	sceneMgr->getCamera("PlayerCam")->setFarClipDistance(10000);
	sceneMgr->getCamera("PlayerCam")->getParentNode()->setPosition(Ogre::Vector3(0,10,0));
	sceneMgr->getCamera("PlayerCam")->lookAt(Ogre::Vector3(0,0,-30));


	mPivotPitch = 0;
}

void Hero::initBody()
{
	// create swords and attach to sheath
	LogManager::getSingleton().logMessage("Creating swords");
	mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	pent->attachObjectToBone("Sheath.L", mSword1);
	pent->attachObjectToBone("Sheath.R", mSword2);

	LogManager::getSingleton().logMessage("Creating the chains");
	// create a couple of ribbon trails for the swords, just for fun
	NameValuePairList params;
	params["numberOfChains"] = "2";
	params["maxElements"] = "80";
	mSwordTrail = (RibbonTrail*)sceneMgr->createMovableObject("RibbonTrail", &params);
	mSwordTrail->setMaterialName("Examples/LightRibbonTrail");
	mSwordTrail->setTrailLength(20);
	mSwordTrail->setVisible(false);
	sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);


	for (int i = 0; i < 2; i++)
	{
		mSwordTrail->setInitialColour(i, 1, 0.8, 0);
		mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
		mSwordTrail->setWidthChange(i, 1);
		mSwordTrail->setInitialWidth(i, 0.5);
	}

	mKeyDirection = Vector3::ZERO;
	mVerticalVelocity = 0;
}

void Hero::initAnimations()
{
	// this is very important due to the nature of the exported animations
	pent->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

	String animNames[] =
	{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
	"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = pent->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_IDLE_BASE);
	setTopAnimation(ANIM_IDLE_TOP);

	// relax the hands since we're not holding anything
	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

	mSwordsDrawn = false;
}

bool Hero::frameStarted( const Ogre::FrameEvent &evt )
{
	
	float camreaSpeed=150.0;
	if (IsupdateCamera==1)   //视角右转
	{
		updateCameraGoal(-camreaSpeed* evt.timeSinceLastFrame, 0, 0);   
	}
	else if (IsupdateCamera==2)   //视角左转
	{
		updateCameraGoal(camreaSpeed*evt.timeSinceLastFrame, 0, 0);    
	}
	else if (IsupdateCamera==3)   //视角上转
	{
		updateCameraGoal(0, -camreaSpeed*evt.timeSinceLastFrame, 0); 
	}
	else if (IsupdateCamera==4)   //视角下转
	{
		updateCameraGoal(0, camreaSpeed*evt.timeSinceLastFrame, 0);  
	}
	updateBody(evt.timeSinceLastFrame);
	updateCam(evt.timeSinceLastFrame);
	updateAnimations(evt.timeSinceLastFrame);


	mNewPos=pnode->getPosition();
	if (BaseApplication::getInstance()->mscene->bTerrainCreated)
	{
		Ogre::Ray mouseRay =Ogre::Ray(Vector3(mNewPos.x,12000,mNewPos.z),Vector3(0,-1,0));
		TerrainGroup::RayResult rayResult =BaseApplication::getInstance()->mscene->mTerrainGroup->rayIntersects(mouseRay);
		if (rayResult.hit)
		{
			if (mOldPos!=mNewPos)
			{
				float distance=mNewPos.distance(mOldPos);
				float hight=rayResult.position.y-mNewPos.y+mFocusY;   //人物中心点高度mfocusy//
				if (distance<=hight)   //斜率大于1，把人物拉回来
				{
					setPositon(mOldPos);
				}
				else
				{
					mNewPos.y=rayResult.position.y+mFocusY;
					setPositon(mNewPos);
				}
			}	

		} //rayResult.hit

	}

	

	return true;

}

void Hero::updateBody( float dTime )
{
	mGoalDirection = Vector3::ZERO;   // we will calculate this

	if (mKeyDirection != Vector3::ZERO && mBaseAnimID != ANIM_DANCE)
	{
		// calculate actually goal direction in world based on player's key directions
		mGoalDirection += mKeyDirection.z * mCameraNode->getOrientation().zAxis();   //z轴
		mGoalDirection += mKeyDirection.x * mCameraNode->getOrientation().xAxis();
		mGoalDirection.y = 0;
		mGoalDirection.normalise();

		Quaternion toGoal = pnode->getOrientation().zAxis().getRotationTo(mGoalDirection); 

		// calculate how much the character has to turn to face goal direction
		Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * dTime * TURN_SPEED;
		// reduce "turnability" if we're in midair
		if (mBaseAnimID == ANIM_JUMP_LOOP) yawAtSpeed *= 0.2f;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
		else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);

		pnode->yaw(Degree(yawToGoal));

		// move in current body direction (not the goal direction)
		pnode->translate(0, 0, dTime * mMoveSpeed * mAnims[mBaseAnimID]->getWeight(),Node::TS_LOCAL);
	}

	if (mBaseAnimID == ANIM_JUMP_LOOP)
	{
		// if we're jumping, add a vertical offset too, and apply gravity
		pnode->translate(0, mVerticalVelocity * dTime, 0, Node::TS_LOCAL);
		mVerticalVelocity -= GRAVITY * dTime;

		Vector3 pos = pnode->getPosition();
		if (pos.y <= mFocusY)
		{
			// if we've hit the ground, change to landing state
			pos.y = mFocusY;
			pnode->setPosition(pos);
			setBaseAnimation(ANIM_JUMP_END, true);
			mTimer = 0;
		}
	}
}

void Hero::updateCam( float dTime )
{
	// 设置视点中心在人物肩膀//
	mCameraPivot->setPosition(pnode->getPosition() + Vector3::UNIT_Y * mFocusY);  //UNITY相当于方向向量 （0,1,0）*2 =（0,2,0）
	// move the camera smoothly to the goal
	Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
	mCameraNode->translate(goalOffset * dTime * 9.0f);
	// always look at the pivot
	mCameraNode->lookAt(mCameraPivot->_getDerivedPosition(), Node::TS_WORLD);
	updateCameraGoal(0,0,0);
}



void Hero::updateAnimations( float dTime )
{
	Real baseAnimSpeed = 1;
	Real topAnimSpeed = 1;

	mTimer += dTime;

	if (mTopAnimID == ANIM_DRAW_SWORDS)
	{
		// flip the draw swords animation if we need to put it back
		topAnimSpeed = mSwordsDrawn ? -1 : 1;

		// half-way through the animation is when the hand grasps the handles...
		if (mTimer >= mAnims[mTopAnimID]->getLength() / 2 &&
			mTimer - dTime < mAnims[mTopAnimID]->getLength() / 2)
		{
			// so transfer the swords from the sheaths to the hands
			pent->detachAllObjectsFromBone();
			pent->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L", mSword1);
			pent->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R", mSword2);
			// change the hand state to grab or let go
			mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);

			// toggle sword trails
			if (mSwordsDrawn)
			{
				mSwordTrail->setVisible(false);
				mSwordTrail->removeNode(mSword1->getParentNode());
				mSwordTrail->removeNode(mSword2->getParentNode());
			}
			else
			{
				mSwordTrail->setVisible(true);
				mSwordTrail->addNode(mSword1->getParentNode());
				mSwordTrail->addNode(mSword2->getParentNode());
			}
		}

		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
			else
			{
				setTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
			mSwordsDrawn = !mSwordsDrawn;
		}
	}
	else if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL)
	{
		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
			else
			{
				setTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
		}

		// don't sway hips from side to side when slicing. that's just embarrassing.
		if (mBaseAnimID == ANIM_IDLE_BASE) baseAnimSpeed = 0;
	}
	else if (mBaseAnimID == ANIM_JUMP_START)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// takeoff animation finished, so time to leave the ground!
			setBaseAnimation(ANIM_JUMP_LOOP, true);
			// apply a jump acceleration to the character
			mVerticalVelocity = JUMP_ACCEL;
		}
	}
	else if (mBaseAnimID == ANIM_JUMP_END)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// safely landed, so go back to running or idling
			if (mKeyDirection == Vector3::ZERO)
			{
				setBaseAnimation(ANIM_IDLE_BASE);
				setTopAnimation(ANIM_IDLE_TOP);
			}
			else
			{
				setBaseAnimation(ANIM_RUN_BASE, true);
				setTopAnimation(ANIM_RUN_TOP, true);
			}
		}
	}

	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(dTime * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(dTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(dTime);
}

void Hero::fadeAnimations( float dTime )
{
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Real newWeight = mAnims[i]->getWeight() + dTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Real newWeight = mAnims[i]->getWeight() - dTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void Hero::setBaseAnimation( AnimID id, bool reset /*= false*/ )
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void Hero::setTopAnimation( AnimID id, bool reset /*= false*/ )
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void Hero::updateCameraGoal( Real deltaYaw, Real deltaPitch, Real deltaZoom )
{
	mCameraPivot->yaw(Degree(deltaYaw), Node::TS_WORLD);    //水平旋转

	// 限制竖直旋转角度
	if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
		!(mPivotPitch + deltaPitch < -60 && deltaPitch < 0))
	{
		mCameraPivot->pitch(Degree(deltaPitch), Node::TS_LOCAL);
		mPivotPitch += deltaPitch;
	}

	Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
	Real distChange = deltaZoom * dist;

	//限制缩放距离
	if (!(dist + distChange < 25 && distChange < 0.5) &&
		!(dist + distChange > 80 && distChange > 0.5))
	{
		mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
	}
}

void Hero::setPositon( Vector3 &vec )
{
	pnode->setPosition(vec);
	mOldPos=vec;
	mNewPos=vec;

}
