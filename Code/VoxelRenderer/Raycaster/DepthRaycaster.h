#pragma once

#include "RaycasterCPU.h"




namespace vr
{

/**@brief Raycaster shows octree internal structure without making raycasting.
For debug purposes.*/
class DepthRaycaster : public RaycasterCPU
{
private:

	float			m_nearClip;
	float			m_farClip;
	float			m_moveFactor;

protected:
public:
	explicit		DepthRaycaster		();
	virtual 		~DepthRaycaster		() = default;


	virtual void	RaycasterThreadImpl		( ThreadData& data, Size threadNumber );
	virtual void	ProcessInput			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )	override;

};


}	// vr

