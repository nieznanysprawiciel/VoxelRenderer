#pragma once

#include "IPanel.h"


namespace GUI
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


}	// GUI

