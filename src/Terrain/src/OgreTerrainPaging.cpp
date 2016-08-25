#include "stdafx.h"
#include "OgreTerrainPaging.h"
#include "OgreTerrainPagedWorldSection.h"
#include "OgreTerrainGroup.h"
#include "OgrePagedWorld.h"

namespace Ogre
{
	//---------------------------------------------------------------------
	TerrainPaging::TerrainPaging(PageManager* pageMgr)
		: mManager(pageMgr)
	{
		mManager->addWorldSectionFactory(&mSectionFactory);
	}
	//---------------------------------------------------------------------
	TerrainPaging::~TerrainPaging()
	{
		mManager->removeWorldSectionFactory(&mSectionFactory);
	}
	//---------------------------------------------------------------------
	TerrainPagedWorldSection* TerrainPaging::createWorldSection(
		PagedWorld* world, TerrainGroup* terrainGroup, 
		Real loadRadius, Real holdRadius, int32 minX, int32 minY, int32 maxX, int32 maxY, 
		const String& sectionName, uint32 loadingIntervalMs)
	{
		TerrainPagedWorldSection* ret = static_cast<TerrainPagedWorldSection*>(
			world->createSection(terrainGroup->getSceneManager(), SectionFactory::FACTORY_NAME, sectionName));

		ret->init(terrainGroup);
		ret->setLoadRadius(loadRadius);
		ret->setHoldRadius(holdRadius);
		ret->setPageRange(minX, minY, maxX, maxY);
		ret->setLoadingIntervalMs(loadingIntervalMs);

		return ret;

	}
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	const String TerrainPaging::SectionFactory::FACTORY_NAME("Terrain");

	const String& TerrainPaging::SectionFactory::getName() const
	{
		return FACTORY_NAME;
	}
	//---------------------------------------------------------------------
	PagedWorldSection* TerrainPaging::SectionFactory::createInstance(const String& name, PagedWorld* parent, SceneManager* sm)
	{
		return OGRE_NEW TerrainPagedWorldSection(name, parent, sm);
	}
	//---------------------------------------------------------------------
	void TerrainPaging::SectionFactory::destroyInstance(PagedWorldSection* s)
	{
		OGRE_DELETE s;
	}


}

