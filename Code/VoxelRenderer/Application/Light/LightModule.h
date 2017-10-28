#pragma once

#include "LightConstants.h"

#include "swGUI/Core/System/Time/FrameTime.h"
#include "VoxelRenderer/Application/Config.h"


namespace vr
{




// ================================ //
//
class LightModule
{
private:

	LightConstants			m_lightsData;

protected:
public:
	explicit		LightModule		( Config& config );
	~LightModule	() = default;


	LightConstants &		GetLights	();

	void					Update		( sw::gui::TimeType time, sw::gui::TimeDiff elapsed );
};

DEFINE_PTR_TYPE( LightModule )

}	// vr

