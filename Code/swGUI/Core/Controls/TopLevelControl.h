#pragma once

#include "IControl.h"


namespace GUI
{

class HostWindow;

/**@brief */
class TopLevelControl : public IControl
{
	RTTR_ENABLE( IControl )
	RTTR_REGISTRATION_FRIEND
private:
protected:

	HostWindow*			m_host;

public:
	explicit			TopLevelControl( IControl* parent, HostWindow* host )
		:	IControl( parent )
		,	m_host( host )
	{}
						~TopLevelControl() = default;


	HostWindow*			GetHostWindow	()		{ return m_host; }
};



}	// GUI
