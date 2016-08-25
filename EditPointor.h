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
	Frustum*		mDecalFrustum;  //ͶӰ��///
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
	int				SelectState;   /////0��ʾδѡ�У�1 2 3 4�ֱ���� x y z �����ɷ���//////
	bool            bDruging;
	EditScene*      mscene;
	Ogre::Degree    camDegree;    //��¼�ƶ�����תʱ�����ˮƽ��ת�Ƕ�//
	float EditSpeed;           //�༭ʱ����ק�ƶ�����ת�����ŵ��ٶ�//


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
