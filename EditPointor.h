#ifndef __EditPointor_h_
#define __EditPointor_h_

#include "Ogre.h"
#include "EditScene.h"

using namespace Ogre;

class EditPointor: public Ogre::FrameListener
{
public:
	EditPointor(EditScene *scene);
	virtual ~EditPointor();
	SceneNode*      mPointorNode;
	SceneNode*      mMoveNode;
	SceneNode*      mScaleNode;
	SceneNode*      mRotionNode;
	SceneNode*      mEditTerrainNode;
	Frustum*		mDecalFrustum;  //投影器///
	Entity*         mEditTerrainPointor;
	Entity*         MovePointorX;
	Entity*         MovePointorY;
	Entity*         MovePointorZ;
	Entity*         MovePointorBox;
	Entity*         ScalePointorX;
	Entity*         ScalePointorY;
	Entity*         ScalePointorZ;
	Entity*         ScalePointorBox;
	Entity*         RotionPointorX;
	Entity*         RotionPointorY;
	Entity*         RotionPointorZ;
	SceneNode*      RotionCollisionXNode;
	SceneNode*      RotionCollisionYNode;
    SceneNode*      RotionCollisionZNode;
	bool            bShow;
	bool			bBoxSelect;
	int				SelectState;   /////0表示未选中，1 2 3 4分别代表 x y z 和自由方向//////
	bool            bDruging;
	EditScene*      mscene;
	Ogre::Degree    camDegree;    //记录移动或旋转时相机的水平旋转角度//
	float EditSpeed;           //编辑时，拖拽移动、旋转、缩放的速度//


	void updatePosition();
	void updateScale();
	void querySelect();
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	void createMovePointer();
	void createEdtiTerrainPointor();
	void createScalePointer();
	void createRotationPointor();

};

#endif // #ifndef __SceneObj_h_
