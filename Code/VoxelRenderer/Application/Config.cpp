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
	:	m_screenWidth( 1024 )
	,	m_screenHeight( 768 )
{
	IDeserializer deser;
	if( deser.LoadFromFile( filePath, ParsingMode::ParseInsitu ) )
	{
		if( deser.EnterObject( "Config" ) )
		{
			if( deser.EnterObject( "Screen" ) )
			{
				m_screenHeight	= deser.GetAttribute( "Height", m_screenHeight );
				m_screenWidth	= deser.GetAttribute( "Width", m_screenWidth );

				deser.Exit();
			}

			deser.Exit();
		}
	}
}

// ================================ //
//
uint16			Config::ScreenWidth() const
{
	return m_screenWidth;
}

// ================================ //
//
uint16			Config::ScreenHeight() const
{
	return m_screenHeight;
}

// ================================ //
//
bool			Config::SaveConfig	( const std::string& filePath ) const
{
	ISerializer ser( nullptr );
	
	ser.EnterObject( "Config" );

		ser.EnterObject( "Screen" );
			ser.SetAttribute( "Height", m_screenHeight );
			ser.SetAttribute( "Width", m_screenWidth );
		ser.Exit();

	ser.Exit();

	return ser.SaveFile( filePath, WritingMode::Readable );
}
