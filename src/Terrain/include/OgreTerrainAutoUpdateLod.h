#ifndef __Ogre_TerrainAutoUpdateLod_H__
#define __Ogre_TerrainAutoUpdateLod_H__

#include "OgreTerrainPrerequisites.h"
#include "OgreTerrain.h"

namespace Ogre
{
	/** \addtogroup Optional Components
	*  @{
	*/
	/** \addtogroup Terrain
	*  Some details on the terrain auto load
	*  @{
	*/

	/** Terrain automatic LOD loading
	@par
		This set of classes is used for automatic change of terrain LOD level. Base is TerrainAutoUpdateLod interface with just
		one public method autoUpdateLod. This method gets called by terrain whenever user thinks something has
		changed(typically in application's main loop) what could affect terrain's LOD level. It is designed in such a way
		so user can use whatever algorithm he likes to change terrain's LOD level. For example see TerrainAutoUpdateLod
		implementation TerrainAutoUpdateLodByDistance.
		It is also used as a null object for auto-LOD-updating.
	*/
	class  TerrainAutoUpdateLod : public TerrainAlloc
	{
	public:
        virtual ~TerrainAutoUpdateLod() {}
		/** Method to be called to change terrain's LOD level.
			@param terrain Instance of Terrain which LOD level is going to be changed
			@param synchronous Run this as part of main thread or in background
			@param data Any user specific data.
		*/
		virtual void autoUpdateLod(Terrain *terrain, bool synchronous, const Any &data) = 0;
		virtual uint32 getStrategyId() = 0;
	};

	// other Strategy's id start from 2
	enum TerrainAutoUpdateLodStrategy
	{
		NONE = 0,
		BY_DISTANCE = 1
	};

	/** Class implementing TerrainAutoUpdateLod interface. It does LOD level increase/decrease according to camera's
		distance to Terrain.
	*/
	class  TerrainAutoUpdateLodByDistance : public TerrainAutoUpdateLod
	{
	public:
		virtual void autoUpdateLod(Terrain *terrain, bool synchronous, const Any &data);
		virtual uint32 getStrategyId() { return BY_DISTANCE; }

	protected:
		/** Modifies Terrain's LOD level according to it's distance from camera.
			@param holdDistance How far ahead of terrain's LOD level change this LOD level should be loaded.
		*/
		void autoUpdateLodByDistance(Terrain *terrain, bool synchronous, const Real holdDistance);
		/// Traverse Terrain's QuadTree and calculate what LOD level is needed.
		int traverseTreeByDistance(TerrainQuadTreeNode *node, const Camera *cam, Real cFactor, const Real holdDistance);
	};

	class  TerrainAutoUpdateLodFactory
	{
	public:
		static TerrainAutoUpdateLod* getAutoUpdateLod( uint32 strategy )
		{
			switch(strategy)
			{
			case BY_DISTANCE:
				return OGRE_NEW TerrainAutoUpdateLodByDistance;
			case NONE:
			default:
				return 0;
			}
			return 0;
		}
	};
	/** @} */
	/** @} */
}

#endif 
