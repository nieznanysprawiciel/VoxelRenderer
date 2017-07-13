#pragma once

#include "RaycasterCPU.h"




namespace vr
{

/**@brief Raycaster shows octree internal structure without making raycasting.
For debug purposes.*/
class NormalsRaycaster : public RaycasterCPU
{
private:

	bool			m_inverse;

protected:
public:
	explicit		NormalsRaycaster		();
	virtual 		~NormalsRaycaster		() = default;


	virtual void	RaycasterThreadImpl		( ThreadData& data, Size threadNumber );
	virtual void	ProcessInput			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )	override;

};


}	// vr

