#include "LightModule.h"




namespace vr
{

// ================================ //
//
LightModule::LightModule	( Config& config )
{}


// ================================ //
//
LightConstants&			LightModule::GetLights	()
{
	return m_lightsData;
}

// ================================ //
//
void					LightModule::Update		( sw::gui::TimeType time, sw::gui::TimeDiff elapsed )
{}

}


