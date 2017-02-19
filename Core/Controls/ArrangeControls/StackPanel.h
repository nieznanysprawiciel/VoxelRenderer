#pragma once

#include "IPanel.h"


namespace sw {
namespace gui
{

/**@brief */
class StackPanel : public IPanel
{
	RTTR_ENABLE( IPanel )
private:
protected:
public:
	StackPanel		( IControl* parent );
	virtual		~StackPanel		();

};


}	// gui
}	// sw

