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
{
	IDeserializer deser;
	if( deser.LoadFromFile( filePath, ParsingMode::ParseInsitu ) )
	{
		if( deser.EnterObject( "Config" ) )
		{
			if( deser.EnterObject( "Screen" ) )
			{
				m_ScreenWidth	= deser.GetAttribute( "Height", m_ScreenWidth );
				m_ScreenHeight	= deser.GetAttribute( "Width", m_ScreenHeight );

				deser.Exit();
			}

			if( deser.EnterObject( "Camera" ) )
			{


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

	ser.Exit();

	return ser.SaveFile( filePath, WritingMode::Readable );
}
