#pragma once

#include "swGUI/Core/Controls/IControl.h"



namespace sw {
namespace gui
{

/**@brief */
class IPanel : public IControl
{
	RTTR_ENABLE( IControl )
		RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	IPanel		( IControl* parent );
	virtual			~IPanel		();

};



}	// gui
}	// sw

