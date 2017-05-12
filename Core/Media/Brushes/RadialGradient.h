#pragma once
/**
@file RadialGradient.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "GradientBrush.h"


namespace sw {
namespace gui
{

/**@brief 
@ingroup Brushes*/
class RadialGradient : public GradientBrush
{
	RTTR_ENABLE( GradientBrush );
private:
protected:
public:
	explicit		RadialGradient		() = default;
	~RadialGradient	() = default;

};


}	// gui
}	// sw
