#pragma once

#include "RaycasterCPU.h"




namespace vr
{

/**@brief Raycaster shows octree internal structure without making raycasting.
For debug purposes.*/
class DepthRaycaster : public RaycasterCPU
{
private:

	int32			m_octreeLevel;
	int32			m_octreeDepth;

protected:
public:
	explicit		DepthRaycaster		();
	virtual 		~DepthRaycaster		() = default;


	virtual void	RaycasterThreadImpl		( ThreadData& data, Size threadNumber );
	virtual void	ProcessInput			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )	override;

};


}	// vr

