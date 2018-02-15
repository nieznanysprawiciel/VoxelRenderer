/**
@file Config.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "Config.h"

#include "swCommonLib/Serialization/Deserializer.h"
#include "swCommonLib/Serialization/Serializer.h"


// ================================ //
//
Config::Config	( const std::string& filePath )
	:	m_CameraDirection( DirectX::XMFLOAT3( 0.0, 0.0, 1.0 ) )
	,	m_CameraFov( 45 )
	,	m_CameraPosition( DirectX::XMFLOAT3( 0.0, 0.0, 0.0 ) )
{
	IDeserializer deser;
	if( deser.LoadFromFile( filePath, ParsingMode::ParseInsitu ) )
	{
		if( deser.EnterObject( "Config" ) )
		{
			if( deser.EnterObject( "Screen" ) )
			{
				m_ScreenHeight	= deser.GetAttribute( "Height", m_ScreenHeight );
				m_ScreenWidth	= deser.GetAttribute( "Width", m_ScreenWidth );
				m_ViewportSize	= deser.GetAttribute( "ViewportSize", m_ViewportSize );

				m_EnableVSync = deser.GetAttribute( "EnableVSync", m_EnableVSync );

				deser.Exit();
			}

			if( deser.EnterObject( "Debug" ) )
			{
				m_PerformanceFilePath = deser.GetAttribute( "PerformanceFile", m_PerformanceFilePath );

				deser.Exit();
			}

			if( deser.EnterObject( "Camera" ) )
			{
				if( deser.EnterObject( "Position" ) )
				{
					m_CameraPosition.x = (float)deser.GetAttribute( "X", m_CameraPosition.x );
					m_CameraPosition.y = (float)deser.GetAttribute( "Y", m_CameraPosition.y );
					m_CameraPosition.z = (float)deser.GetAttribute( "Z", m_CameraPosition.z );

					deser.Exit();
				}

				if( deser.EnterObject( "Direction" ) )
				{
					m_CameraDirection.x = (float)deser.GetAttribute( "X", m_CameraDirection.x );
					m_CameraDirection.y = (float)deser.GetAttribute( "Y", m_CameraDirection.y );
					m_CameraDirection.z = (float)deser.GetAttribute( "Z", m_CameraDirection.z );

					deser.Exit();
				}

				m_CameraFov = (float)deser.GetAttribute( "FOV", m_CameraDirection.x );
				m_CameraNear = (float)deser.GetAttribute( "NearPlane", m_CameraNear );
				m_CameraFar = (float)deser.GetAttribute( "FarPlane", m_CameraFar );
				m_IsPerspective = deser.GetAttribute( "IsPerspective", m_IsPerspective );

				deser.Exit();
			}

			if( deser.EnterObject( "Lights" ) )
			{
				if( deser.EnterObject( "DirectionalLight" ) )
				{
					if( deser.EnterObject( "Direction" ) )
					{
						m_DirLightDirection.x = (float)deser.GetAttribute( "X", m_DirLightDirection.x );
						m_DirLightDirection.y = (float)deser.GetAttribute( "Y", m_DirLightDirection.y );
						m_DirLightDirection.z = (float)deser.GetAttribute( "Z", m_DirLightDirection.z );

						deser.Exit();
					}

					if( deser.EnterObject( "Color" ) )
					{
						m_DirLightColor.x = (float)deser.GetAttribute( "X", m_DirLightColor.x );
						m_DirLightColor.y = (float)deser.GetAttribute( "Y", m_DirLightColor.y );
						m_DirLightColor.z = (float)deser.GetAttribute( "Z", m_DirLightColor.z );

						deser.Exit();
					}

					m_DirLightIntensity = (float)deser.GetAttribute( "Intensity", m_DirLightIntensity );

					deser.Exit();
				}

				deser.Exit();
			}

			if( deser.EnterObject( "Raycaster" ) )
			{
				m_RaycasterType = deser.GetAttribute( "Type", m_RaycasterType );

				deser.Exit();
			}

			if( deser.EnterObject( "Octree" ) )
			{
				m_OctreeFilePath = deser.GetAttribute( "FilePath", m_OctreeFilePath );

				deser.Exit();
			}

			if( deser.EnterObject( "ShellMesh" ) )
			{
				m_ShellMeshFilePath = deser.GetAttribute( "FilePath", m_ShellMeshFilePath );
				m_AnimatedOctreePath = deser.GetAttribute( "OctreeFile", m_AnimatedOctreePath );

				deser.Exit();
			}

			deser.Exit();
		}
	}
}

// ================================ //
//
bool			Config::SaveConfig	( const std::string& filePath ) const
{
	ISerializer ser( nullptr );
	
	ser.EnterObject( "Config" );

		ser.EnterObject( "Screen" );
			ser.SetAttribute( "Height", m_ScreenHeight );
			ser.SetAttribute( "Width", m_ScreenWidth );
		ser.Exit();

		ser.EnterObject( "Camera" );
			ser.EnterObject( "Position" );
				ser.SetAttribute( "X", m_CameraPosition.x );
				ser.SetAttribute( "Y", m_CameraPosition.y );
				ser.SetAttribute( "Z", m_CameraPosition.z );
			ser.Exit();

			ser.EnterObject( "Direction" );
				ser.SetAttribute( "X", m_CameraDirection.x );
				ser.SetAttribute( "Y", m_CameraDirection.y );
				ser.SetAttribute( "Z", m_CameraDirection.z );
			ser.Exit();

			ser.SetAttribute( "FOV", m_CameraFov );
		ser.Exit();

		ser.EnterObject( "Raycaster" );
			ser.SetAttribute( "Type", m_RaycasterType );
		ser.Exit();

		ser.EnterObject( "Octree" );
			ser.SetAttribute( "FilePath", m_OctreeFilePath );
		ser.Exit();

	ser.Exit();

	return ser.SaveFile( filePath, WritingMode::Readable );
}
