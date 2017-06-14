#pragma once

#include "RaycasterCPU.h"




namespace vr
{

/**@brief Raycaster shows octree internal structure without making raycasting.
For debug purposes.*/
class PrintOctreeRaycaster : public RaycasterCPU
{
private:
protected:
public:
	explicit		PrintOctreeRaycaster		() = default;
	~PrintOctreeRaycaster	() = default;


	virtual void	RaycasterThreadImpl		( ThreadData& data, Size threadNumber );


private:

	bool			IsFilledVoxel			( RaycasterContext& rayCtx, uint32 octreeLevel, uint32 voxelX, uint32 voxelY, uint32 voxelZ );
};


}	// vr

