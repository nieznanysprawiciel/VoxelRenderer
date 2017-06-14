#pragma once

#include "RaycasterCPU.h"




namespace vr
{

/**@brief Raycaster shows octree internal structure without making raycasting.
For debug purposes.*/
class PrintOctreeRaycaster : public RaycasterCPU
{
private:

	int32			m_octreeLevel;
	int32			m_octreeDepth;

protected:
public:
	explicit		PrintOctreeRaycaster		();
	virtual 		~PrintOctreeRaycaster		() = default;


	virtual void	RaycasterThreadImpl		( ThreadData& data, Size threadNumber );
	virtual void	ProcessInput			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )	override;

private:

	bool			IsFilledVoxel			( RaycasterContext& rayCtx, uint32 octreeLevel, uint32 voxelX, uint32 voxelY, uint32 voxelZ );
};


}	// vr

