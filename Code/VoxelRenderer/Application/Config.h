#pragma once
/**
@file Config.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/Config/ConfigHelper.h"

#include <string>
#include <DirectXMath.h>




//====================================================================================//
//			Config Implementation	
//====================================================================================//


/**@brief Engine start configuration.*/
class Config
{
private:
	DEFINE_CONFIG_VALUE_DEFAULT( bool, EnableVSync, true );

	DEFINE_CONFIG_VALUE_DEFAULT( uint16, ScreenWidth, 1024 );
	DEFINE_CONFIG_VALUE_DEFAULT( uint16, ScreenHeight, 768 );
	DEFINE_CONFIG_VALUE_DEFAULT( float, ViewportSize, 1.0f );
	DEFINE_CONFIG_VALUE_DEFAULT( bool, IsPerspective, false );

	DEFINE_CONFIG_VALUE( DirectX::XMFLOAT3, CameraPosition );
	DEFINE_CONFIG_VALUE( DirectX::XMFLOAT3, CameraDirection );
	DEFINE_CONFIG_VALUE_DEFAULT( float, CameraFov, 45.0f );
	DEFINE_CONFIG_VALUE_DEFAULT( float, CameraFar, 1.0f );
	DEFINE_CONFIG_VALUE_DEFAULT( float, CameraNear, 1000.0f );

	DEFINE_CONFIG_VALUE( DirectX::XMFLOAT3, DirLightDirection );
	DEFINE_CONFIG_VALUE( DirectX::XMFLOAT3, DirLightColor );
	DEFINE_CONFIG_VALUE( float, DirLightIntensity );

	DEFINE_CONFIG_VALUE_DEFAULT( std::string, RaycasterType, "" );
	DEFINE_CONFIG_VALUE_DEFAULT( std::string, OctreeFilePath, "" );
	DEFINE_CONFIG_VALUE_DEFAULT( std::string, ShellMeshFilePath, "" );
	DEFINE_CONFIG_VALUE_DEFAULT( std::string, AnimatedOctreePath, "" );

	DEFINE_CONFIG_VALUE_DEFAULT( std::string, PerformanceFilePath, "Performance.txt" );

public:
	explicit		Config	( const std::string& filePath );
					~Config	() = default;

	bool			SaveConfig		( const std::string& filePath ) const;
};

DEFINE_UPTR_TYPE( Config );




