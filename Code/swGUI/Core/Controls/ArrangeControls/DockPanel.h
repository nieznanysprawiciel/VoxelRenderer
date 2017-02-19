#pragma once

#include "IPanel.h"


namespace sw {
namespace gui
{

class DockPanel : public IPanel
{
	RTTR_ENABLE( IPanel )
private:
public:
	DockPanel		( IControl* parent );
	virtual		~DockPanel		();
};


}	// gui
}	// sw
