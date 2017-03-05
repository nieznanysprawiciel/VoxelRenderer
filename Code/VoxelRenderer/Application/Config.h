#pragma once
/**
@file Config.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/TypesDefinitions.h"

#include <string>


/**@brief Engine start configuration.*/
class Config
{
private:

	uint16			m_screenWidth;
	uint16			m_screenHeight;

protected:
public:
	explicit		Config	( const std::string& filePath );
					~Config	() = default;


	uint16			ScreenWidth		() const;
	uint16			ScreenHeight	() const;

	bool			SaveConfig		( const std::string& filePath ) const;
};

DEFINE_UPTR_TYPE( Config );




