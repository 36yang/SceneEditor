#ifndef __DebugBox_h_
#define __DebugBox_h_

#include "Ogre.h"
#include "ode/ode.h"
#include "MeshObj.h"

using namespace Ogre;

enum PhysicsBoxType{TypeBox=0,TypePlane, TypeSphere, TypeCapsule, TypeCylinder };

class PhysicsBox: public SceneObj
{
public:
	PhysicsBox(dSpaceID mOdeSpace, PhysicsBoxType mType, int tag, SceneMeshObj* Obj=NULL);
	virtual ~PhysicsBox();
	bool frameStarted( const Ogre::FrameEvent &evt );
	void InitGemo(PhysicsBoxType mType);
	void InitBody();
	virtual void setPosition(Vector3 &pos);
	virtual void setScale(Vector3 &scale);
	virtual void setRotaion(Vector3 &rotaion);
	virtual void onUpdateValueChange(int tag);
	virtual void setOffsetPosition(Vector3 &OffsetPos);
	virtual void setOffsetScale(Vector3 &OffsetScale);
	virtual void setOffsetRotaion(Vector3 &OffsetRotaion);
	bool checkScale(Vector3& scale);

	dSpaceID        mSpace;
	dBodyID			mBody;
	dGeomID			mGeom;	
	PhysicsBoxType	mType;
	SceneMeshObj*	mMeshObj;
	float           mRadius;
	float           mXlen, mYlen, mZlen;

	Ogre::SceneNode* capsuleNode;  //╫╨дрльсп//
	Ogre::SceneNode* farNode;
	Ogre::SceneNode* nearNode;
	
};


#endif // #ifndef __SceneObj_h_
