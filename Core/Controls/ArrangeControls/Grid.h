#pragma once

#include "IPanel.h"


namespace GUI
{

/**@brief */
class Grid : public IPanel
{
	RTTR_ENABLE( IPanel )
	RTTR_REGISTRATION_FRIEND
private:
protected:
public:
				Grid	( IControl* parent );
	virtual		~Grid	();

};


}	// GUI
