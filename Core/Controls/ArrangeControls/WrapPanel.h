#pragma once

#include "IPanel.h"



namespace sw {
namespace gui
{

/**@brief */
class WrapPanel : public IPanel
{
	RTTR_ENABLE( IPanel )
		RTTR_REGISTRATION_FRIEND
private:
protected:
public:
	WrapPanel		( IControl* parent );
	virtual			~WrapPanel		();

};



}	// gui
}	// sw

