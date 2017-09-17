#pragma once
/**
@file FrameTime.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/TypesDefinitions.h"


namespace sw {
namespace gui
{

typedef double TimeType;
typedef double TimeDiff;
typedef double TimeRatio;
typedef uint64 Ticks;


// ================================ //
//
struct FrameTime
{
	TimeType		Time;			///< Time from start.
	TimeDiff		Elapsed;		///< Time from last frame.
};


}	// gui
}	// sw

