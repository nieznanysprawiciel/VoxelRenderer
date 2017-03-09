#pragma once

#include "Brush.h"


namespace sw {
namespace gui
{

/**@brief */
class SolidColorBrush : public Brush
{
	RTTR_ENABLE( Brush );
	RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit		SolidColorBrush		() = default;
					~SolidColorBrush	() = default;

};


}	// gui
}	// sw




