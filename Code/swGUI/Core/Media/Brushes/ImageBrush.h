#pragma once

#include "Brush.h"



namespace sw {
namespace gui
{

/**@brief */
class ImageBrush : public Brush
{
	RTTR_ENABLE( Brush );
	RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	explicit		ImageBrush		() = default;
	~ImageBrush	() = default;

};


}	// gui
}	// sw


