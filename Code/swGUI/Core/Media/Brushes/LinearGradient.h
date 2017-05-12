#pragma once

#include "GradientBrush.h"



namespace sw {
namespace gui
{

/**@brief 
@ingroup Brushes*/
class LinearGradient : public GradientBrush
{
	RTTR_ENABLE( GradientBrush );
private:
protected:
public:
	explicit		LinearGradient		() = default;
	~LinearGradient	() = default;

};


}	// gui
}	// sw

