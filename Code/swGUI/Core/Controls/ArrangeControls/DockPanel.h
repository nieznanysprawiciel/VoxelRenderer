#pragma once

#include "IPanel.h"


namespace GUI
{

class DockPanel : public IPanel
{
	RTTR_ENABLE( IPanel )
private:
public:
				DockPanel		( IControl* parent );
	virtual		~DockPanel		();
};


}	// GUI

