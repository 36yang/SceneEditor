#include "stdafx.h"
#include "EditPointor.h"
#include "BaseApplication.h"


EditPointor::EditPointor( EditScene *scene )
{
	mscene=scene;
	mPointorNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("mPointorNode");
	mMoveNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("mMoveNode");
	mScaleNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("mScaleNode");
	mRotionNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("mRotionNode");
	mEditTerrainNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("mEditTerrainNode");
	createMovePointer();
	createEdtiTerrainPointor();
	createRotationPointor();
	createScalePointer();
	BaseApplication::getInstance()->mSceneMgr->getRootSceneNode()->addChild(mPointorNode);
	mPointorNode->addChild(mMoveNode);
	mPointorNode->addChild(mScaleNode);
	mPointorNode->addChild(mRotionNode);
	mPointorNode->addChild(mEditTerrainNode);
	mPointorNode->setVisible(false);
	bBoxSelect=false;
	SelectState=0;   
	bDruging=false;
	bShow=false;
	EditSpeed=0.25;
}

EditPointor::~EditPointor()
{

}

void EditPointor::updatePosition()
{
	if (mscene->mTerrainEditMode==TerrainNoneEdit||mscene->mTerrainEditMode==TerrainSelectObj||mscene->mTerrainEditMode==AddObj)
	{
		mEditTerrainNode->setVisible(false);
	}
	else {mEditTerrainNode->setVisible(true);}

	if (mscene->mObjectEditMode==ObjMove&&mscene->selectSceneObj.size()>0)
	{
		mMoveNode->setVisible(true);
	}
	else mMoveNode->setVisible(false);
	if (mscene->mObjectEditMode==ObjRoll&&mscene->selectSceneObj.size()>0)
	{
		mRotionNode->setVisible(true);
		RotionCollisionXNode->setVisible(false);
		RotionCollisionYNode->setVisible(false);
		RotionCollisionZNode->setVisible(false);
	}
	else mRotionNode->setVisible(false);
	if (mscene->mObjectEditMode==ObjScale&&mscene->selectSceneObj.size()>0)
	{
		mScaleNode->setVisible(true);
	}
	else mScaleNode->setVisible(false);


	if (mscene->bTerrainCreated&&mscene->mTerrainEditMode!=TerrainNoneEdit)
	{
		
		unsigned int h,w,c;    //窗口信息//
		int l,t;
		BaseApplication::getInstance()->mWindow->getMetrics(w,h,c,l,t);
		CPoint point;
		GetCursorPos(&point);
		Ogre::Ray mouseRay =mscene->mCamera->getCameraToViewportRay((point.x-l)/float(w), (point.y-t)/float(h));
		TerrainGroup::RayResult rayResult = mscene->mTerrainGroup->rayIntersects(mouseRay);
		if (rayResult.hit)
		{
			mPointorNode->setPosition(rayResult.position);
		}	
	}
	if (mscene->selectSceneObj.size()>0&&mscene->mObjectEditMode!=ObjNoneEdit)
	{
		SceneObj* obj=mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1);
		AxisAlignedBox AABB=obj->mEntity->getWorldBoundingBox(true);
		Vector3 pos=AABB.getCenter();
		if (mscene->mObjectEditMode==ObjMove||mscene->mObjectEditMode==ObjScale)
		{
			Vector3 Halfsize=AABB.getHalfSize();
			pos.y-=Halfsize.y;
			mPointorNode->setPosition(pos);
			return;
		}
		if (mscene->mObjectEditMode==ObjRoll)
		{
			mPointorNode->setPosition(pos);
			return;
		}
	
	}
}

void EditPointor::updateScale()
{
	int scale=5;
	if (mscene->mObjectEditMode==ObjMove&&mscene->selectSceneObj.size()>0)
	{
		mMoveNode->setScale(scale,scale,scale);
	}
	else if (mscene->mObjectEditMode==ObjScale&&mscene->selectSceneObj.size()>0)
	{
		mScaleNode->setScale(scale,scale,scale);
	}
	else if (mscene->mObjectEditMode==ObjRoll&&mscene->selectSceneObj.size()>0)
	{
		scale=15;
		mRotionNode->setScale(scale,scale,scale);
	}
	else if (mscene->mTerrainEditMode==TerrainDown ||mscene->mTerrainEditMode==TerrainUp)
	{
		scale=1;
		mEditTerrainNode->setScale(scale,scale,scale);
	}
	else if (mscene->mTerrainEditMode==TerrainPlatUp || mscene->mTerrainEditMode==TerrainPlatDown)
	{
		scale=1;
		mEditTerrainNode->setScale(scale,scale,scale);
	}

	
}

void EditPointor::querySelect()
{
	if (!bDruging)
	{
		unsigned int h,w,c;    //窗口信息//
		int l,t;
		BaseApplication::getInstance()->mWindow->getMetrics(w,h,c,l,t);
		CPoint point;
		GetCursorPos(&point);
		Ogre::Ray mouseRay =mscene->mCamera->getCameraToViewportRay((point.x-l)/float(w), (point.y-t)/float(h));
		mscene->mRaySceneQuery->setRay(mouseRay);
		mscene->mRaySceneQuery->setSortByDistance(true);    /////////设置按深度排序，方便得到第一个相交物体//////////////
		RaySceneQueryResult &result =mscene->mRaySceneQuery->execute();
		RaySceneQueryResult::iterator itr = result.begin( );
		for (;itr!=result.end();++itr)
		{
			Ogre::String name=itr->movable->getParentNode()->getName();
			if (mscene->mObjectEditMode==ObjMove)
			{
				if (name=="PointorMoveX")
				{
					SelectState=1;
					bBoxSelect=false;
					MovePointorX->setMaterialName("OgreSE/Yellow");
					break;
				}
				else
				{
					SelectState=0;
					MovePointorX->setMaterialName("OgreSE/RedMat");
				}
				if (name=="PointorMoveY")
				{
					SelectState=2;
					bBoxSelect=false;
					MovePointorY->setMaterialName("OgreSE/Yellow");
					break;
				}else
				{
					SelectState=0;
					MovePointorY->setMaterialName("OgreSE/BlueMat");
				}
				if (name=="PointorMoveZ")
				{
					SelectState=3;
					bBoxSelect=false;
					MovePointorZ->setMaterialName("OgreSE/Yellow");	
					break;
				}else
				{
					SelectState=0;
					MovePointorZ->setMaterialName("OgreSE/GreenMat");
				}
				if (name=="PointorMoveBox")
				{	
					bBoxSelect=true;
				}
			} //objmove//

			if (mscene->mObjectEditMode==ObjRoll)
			{
				if (name=="RotionCollisionXNode")
				{
					SelectState=1;
					RotionPointorX->setMaterialName("OgreSE/Yellow");
					break;
				}else
				{
					SelectState=0;
					RotionPointorX->setMaterialName("OgreSE/RedMat");
				}
				if (name=="RotionCollisionYNode")
				{
					SelectState=2;
					RotionPointorY->setMaterialName("OgreSE/Yellow");
					break;
				}else
				{
					SelectState=0;
					RotionPointorY->setMaterialName("OgreSE/BlueMat");
				}
				if (name=="RotionCollisionZNode")
				{
					SelectState=3;
					RotionPointorZ->setMaterialName("OgreSE/Yellow");
					break;
				}else
				{
					SelectState=0;
					RotionPointorZ->setMaterialName("OgreSE/GreenMat");
				}
			}//Objroll//

			if (mscene->mObjectEditMode==ObjScale)
			{
				if (name=="PointorScaleX")
				{
					SelectState=1;
					bBoxSelect=false;
					ScalePointorX->setMaterialName("OgreSE/Yellow");
					break;
				}
				else
				{
					SelectState=0;
					ScalePointorX->setMaterialName("OgreSE/RedMat");
				}
				if (name=="PointorScaleY")
				{
					SelectState=2;
					bBoxSelect=false;
					ScalePointorY->setMaterialName("OgreSE/Yellow");
					break;
				}else
				{
					SelectState=0;
					ScalePointorY->setMaterialName("OgreSE/BlueMat");
				}
				if (name=="PointorScaleZ")
				{
					SelectState=3;
					bBoxSelect=false;
					ScalePointorZ->setMaterialName("OgreSE/Yellow");	
					break;
				}else
				{
					SelectState=0;
					ScalePointorZ->setMaterialName("OgreSE/GreenMat");
				}
				if (name=="PointorScaleBox")
				{	
					bBoxSelect=true;
				}
			}//ObjScale//


		}   //for//
		if (bBoxSelect&&SelectState!=1&&SelectState!=2&&SelectState!=3)
		{
			SelectState=4;
			bBoxSelect=false;
			if (mscene->mObjectEditMode==ObjMove){MovePointorBox->setMaterialName("OgreSE/Yellow");}
			if (mscene->mObjectEditMode==ObjScale){ScalePointorBox->setMaterialName("OgreSE/Yellow");}
		}
		else
		{
			bBoxSelect=false;
			if (SelectState!=1&&SelectState!=2&&SelectState!=3)
			{
				SelectState=0;
			}
			if (mscene->mObjectEditMode==ObjMove){MovePointorBox->setMaterialName("OgreSE/ObjectIndicatorWhiteMat");}
			if (mscene->mObjectEditMode==ObjScale){ScalePointorBox->setMaterialName("OgreSE/ObjectIndicatorWhiteMat");}
		}
	}

}


bool EditPointor::frameStarted( const Ogre::FrameEvent& evt )
{
	updateScale();
	updatePosition();
	querySelect();
	if (BaseApplication::getInstance()->mscene!=NULL)
	{
		if (BaseApplication::getInstance()->mscene->selectSceneObj.size()==0)
		{
			bDruging=false;
		}
	}
	return true;
}


void EditPointor::createMovePointer()
{
	MovePointorX=BaseApplication::getInstance()->mSceneMgr->createEntity("pointor.mesh");
	MovePointorY=BaseApplication::getInstance()->mSceneMgr->createEntity("pointor.mesh");
	MovePointorZ=BaseApplication::getInstance()->mSceneMgr->createEntity("pointor.mesh");
	MovePointorBox=BaseApplication::getInstance()->mSceneMgr->createEntity("cube.mesh");
	MovePointorX->setMaterialName("OgreSE/RedMat");
	MovePointorX->setCastShadows(false);
	MovePointorY->setMaterialName("OgreSE/BlueMat");
	MovePointorY->setCastShadows(false);
	MovePointorZ->setMaterialName("OgreSE/GreenMat");
	MovePointorZ->setCastShadows(false);
	MovePointorBox->setMaterialName("OgreSE/ObjectIndicatorWhiteMat");
	MovePointorBox->setCastShadows(false);
	SceneNode* MoveNodeX=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorMoveX");
	MoveNodeX->attachObject(MovePointorX);
	MoveNodeX->setDirection(Vector3::NEGATIVE_UNIT_X);
	MoveNodeX->setPosition(0.001,0,0);
	SceneNode* MoveNodeY=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorMoveY");
	MoveNodeY->attachObject(MovePointorY);
	MoveNodeY->setDirection(Vector3::NEGATIVE_UNIT_Y);
	SceneNode* MoveNodeZ=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorMoveZ");
	MoveNodeZ->attachObject(MovePointorZ);
	MoveNodeZ->setDirection(Vector3::NEGATIVE_UNIT_Z);
	MoveNodeZ->setPosition(0,0,0.001);
	SceneNode* MoveBoxNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorMoveBox");
	MoveBoxNode->attachObject(MovePointorBox);
	MoveBoxNode->setScale(0.025,0.025,0.025);
	Vector3 size=MovePointorBox->getWorldBoundingBox(true).getHalfSize();
	MoveBoxNode->setPosition(0,size.y,0);
	mMoveNode->addChild(MoveBoxNode);
	mMoveNode->addChild(MoveNodeX);
	mMoveNode->addChild(MoveNodeY);
	mMoveNode->addChild(MoveNodeZ);
}

void EditPointor::createEdtiTerrainPointor()
{
	mEditTerrainPointor=BaseApplication::getInstance()->mSceneMgr->createEntity("mEditTerrainPointor","sphere.mesh");
	mEditTerrainNode->attachObject(mEditTerrainPointor);
	mEditTerrainNode->setPosition(Ogre::Vector3(0,0,0));
	//MaterialPtr material = MaterialManager::getSingletonPtr()->create("q", "General");
	//material->getTechnique(0)->getPass(0)->createTextureUnitState("Girl_Diffuse.png");
	//mEditTerrainPointor->setMaterial(material);

	//mDecalFrustum = new Frustum();  //投影器，相当于相机//////
	//mDecalFrustum->setProjectionType(PT_ORTHOGRAPHIC);
	//mDecalFrustum->setNearClipDistance(25);
	//mEditTerrainNode->attachObject(mDecalFrustum);

}

void EditPointor::createScalePointer()
{
	ScalePointorX=BaseApplication::getInstance()->mSceneMgr->createEntity("pointor.mesh");
	ScalePointorY=BaseApplication::getInstance()->mSceneMgr->createEntity("pointor.mesh");
	ScalePointorZ=BaseApplication::getInstance()->mSceneMgr->createEntity("pointor.mesh");
	ScalePointorBox=BaseApplication::getInstance()->mSceneMgr->createEntity("pointLight.mesh");
	ScalePointorX->setMaterialName("OgreSE/RedMat");
	ScalePointorX->setCastShadows(false);
	ScalePointorY->setMaterialName("OgreSE/BlueMat");
	ScalePointorY->setCastShadows(false);
	ScalePointorZ->setMaterialName("OgreSE/GreenMat");
	ScalePointorZ->setCastShadows(false);
	ScalePointorBox->setMaterialName("OgreSE/ObjectIndicatorWhiteMat");
	ScalePointorBox->setCastShadows(false);
	SceneNode* ScaleBoxNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorScaleBox");
	ScaleBoxNode->attachObject(ScalePointorBox);
	ScaleBoxNode->setScale(0.4,0.4,0.4);
	SceneNode* ScaleNodeX=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorScaleX");
	ScaleNodeX->attachObject(ScalePointorX);
	ScaleNodeX->setDirection(Vector3::NEGATIVE_UNIT_X);
	ScaleNodeX->setPosition(0.001,0,0);
	SceneNode* ScaleNodeY=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorScaleY");
	ScaleNodeY->attachObject(ScalePointorY);
	ScaleNodeY->setDirection(Vector3::NEGATIVE_UNIT_Y);
	SceneNode* ScaleNodeZ=BaseApplication::getInstance()->mSceneMgr->createSceneNode("PointorScaleZ");
	ScaleNodeZ->attachObject(ScalePointorZ);
	ScaleNodeZ->setDirection(Vector3::NEGATIVE_UNIT_Z);
	ScaleNodeZ->setPosition(0,0,0.001);

	mScaleNode->addChild(ScaleBoxNode);
	mScaleNode->addChild(ScaleNodeX);
	mScaleNode->addChild(ScaleNodeY);
	mScaleNode->addChild(ScaleNodeZ);
}

void EditPointor::createRotationPointor()
{
	RotionPointorX=BaseApplication::getInstance()->mSceneMgr->createEntity("RotionPointorX","RotationRingVisible.mesh");
	RotionPointorX->setMaterialName("OgreSE/RedMat");
	RotionPointorX->setCastShadows(false);
    RotionPointorY=BaseApplication::getInstance()->mSceneMgr->createEntity("RotionPointorY","RotationRingVisible.mesh");
	RotionPointorY->setMaterialName("OgreSE/BlueMat");
	RotionPointorY->setCastShadows(false);
	RotionPointorZ=BaseApplication::getInstance()->mSceneMgr->createEntity("RotionPointorZ","RotationRingVisible.mesh");
	RotionPointorZ->setMaterialName("OgreSE/GreenMat");
	RotionPointorZ->setCastShadows(false);
	Entity* RotionCollisionX=BaseApplication::getInstance()->mSceneMgr->createEntity("RotionCollisionX","RotationRingCollision.mesh");
	Entity* RotionCollisionY=BaseApplication::getInstance()->mSceneMgr->createEntity("RotionCollisionY","RotationRingCollision.mesh");
	Entity* RotionCollisionZ=BaseApplication::getInstance()->mSceneMgr->createEntity("RotionCollisionZ","RotationRingCollision.mesh");

	SceneNode* RotionPointorXNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("RotionPointorXNode");
	SceneNode* RotionPointorYNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("RotionPointorYNode");
	SceneNode* RotionPointorZNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("RotionPointorZNode");
	RotionCollisionXNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("RotionCollisionXNode");
	RotionCollisionYNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("RotionCollisionYNode");
	RotionCollisionZNode=BaseApplication::getInstance()->mSceneMgr->createSceneNode("RotionCollisionZNode");

	RotionPointorXNode->attachObject(RotionPointorX);
	RotionPointorXNode->setScale(1,5,1);
	RotionPointorYNode->attachObject(RotionPointorY);
	RotionPointorYNode->setScale(1,5,1);
	RotionPointorYNode->setDirection(Ogre::Vector3::UNIT_Y);
	RotionPointorZNode->attachObject(RotionPointorZ);
	RotionPointorZNode->setScale(1,5,1);
	RotionPointorZNode->setDirection(Ogre::Vector3::UNIT_Y);
	RotionPointorZNode->roll(Ogre::Degree(90));
	RotionCollisionXNode->attachObject(RotionCollisionX);
	RotionCollisionXNode->setScale(1,2,1);
	RotionCollisionXNode->setVisible(false);
	RotionCollisionYNode->attachObject(RotionCollisionY);
	RotionCollisionYNode->setScale(1,2,1);
	RotionCollisionYNode->setVisible(false);
	RotionCollisionYNode->setDirection(Ogre::Vector3::UNIT_Y);
	RotionCollisionZNode->attachObject(RotionCollisionZ);
	RotionCollisionZNode->setScale(1,2,1);

	RotionCollisionZNode->setVisible(false);
	RotionCollisionZNode->setDirection(Ogre::Vector3::UNIT_Y);
	RotionCollisionZNode->roll(Ogre::Degree(90));

	mRotionNode->addChild(RotionPointorXNode);
	mRotionNode->addChild(RotionPointorYNode);
	mRotionNode->addChild(RotionPointorZNode);
	mRotionNode->addChild(RotionCollisionXNode);
	mRotionNode->addChild(RotionCollisionYNode);
	mRotionNode->addChild(RotionCollisionZNode);
}
