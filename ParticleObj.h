#ifndef __PARTICLEOBJ_h_
#define __PARTICLEOBJ_h_

#include "Ogre.h"
#include "SceneObj.h"
#include "ParticleUniverseSystemManager.h"


using namespace Ogre;


class SceneParticleObj : public SceneObj
{
public:
	SceneParticleObj(int tag, Ogre::String ParticleName);
	virtual ~SceneParticleObj();
	virtual void onUpdateValueChange(int tag);
	virtual void showBoundingBox(bool bShow);
	Ogre::SceneNode* mParticleNode;
	Ogre::SceneNode* mEntityNode;
	ParticleUniverse::ParticleSystem* mParticle;

};


#endif // #ifndef __LIGHTOBJ_h_
