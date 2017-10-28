#include "LightModule.h"




namespace vr
{

// ================================ //
//
LightModule::LightModule	( Config& config )
{
	// Set data for directional light which is first in array..
	auto & dirLight = m_lightsData.LightData[ 0 ];
	dirLight.Color = config.DirLightColor();
	dirLight.Direction = config.DirLightDirection();
	dirLight.Intensity = config.DirLightIntensity();
	dirLight.Type = LightType::DirectionalLight;

	m_lightsData.AmbientColor = DirectX::XMFLOAT3( 0.1f, 0.1f, 0.1f );

	// Disable rest of lights.
	m_lightsData.NumLights = 1;
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


