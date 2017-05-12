#pragma once
/**
@file Brush.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/EngineObject.h"


/**@defgroup Brushes Brushes
@brief Brushes.
@ingroup Media*/


namespace sw {
namespace gui
{

/**@brief Brush base class.
@ingroup Brushes*/
class Brush : public EngineObject
{
	RTTR_ENABLE( EngineObject );
	RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit		Brush		() = default;
	virtual			~Brush		() = default;

};


}	// gui
}	// sw

