#pragma once
/**
@file Device.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"


namespace sw {
namespace input
{


/**@brief Interface class for input devices.

This class exist to provide one base class for all Devices. Thanks to that they can be hold in one vector.

@ingroup Input*/
class Device
{
private:
protected:
public:
	explicit		Device		() = default;
	virtual			~Device		() = default;

};

DEFINE_OPTR_TYPE( Device );


}	// input
}	// sw


