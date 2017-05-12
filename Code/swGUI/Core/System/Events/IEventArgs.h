#pragma once
/**
@file IEventArgs.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swCommonLib/Common/RTTR.h"


/**@defgroup Events Events
@brief Event structures.

@ingroup EventSystem*/



namespace sw {
namespace gui
{

/**@brief Base class for events.

@ingroup Events*/
struct IEventArgs
{
	RTTR_ENABLE()
public:
	IEventArgs()
		:	Handled( false )
	{}
public:
	bool			Handled;		///< Mark event as handled to stop further processing.
};

DEFINE_OPTR_TYPE( IEventArgs );

}	// gui
}	// sw

