#pragma once
/**
@file TimnelineTypes.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/TypesDefinitions.h"



/**@brief Wrapping mode at beginning or at the end of timelines.*/
enum class WrapMode : uint8
{
	Clamp,
	Mirror,
	Repeat
};

