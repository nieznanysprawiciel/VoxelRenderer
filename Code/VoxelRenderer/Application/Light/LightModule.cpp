#include "LightModule.h"




namespace vr
{

// ================================ //
//
LightModule::LightModule	( Config& config )
{
	auto & dirLight = m_lightsData.DirectionalLight;
	dirLight.Color = config.DirLightColor();
	dirLight.Direction = config.DirLightDirection();
	dirLight.Intensity = config.DirLightIntensity();

	// Disable rest of lights.
	m_lightsData.LightData[ 0 ].ClampRadius = 0.0f;
	m_lightsData.LightData[ 1 ].ClampRadius = 0.0f;
}


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


