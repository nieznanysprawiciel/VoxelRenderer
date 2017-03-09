#pragma once
/**
@file Brush.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/EngineObject.h"



namespace sw {
namespace gui
{

/**@brief Brush base class.*/
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

