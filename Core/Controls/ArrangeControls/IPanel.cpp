#include "IPanel.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< GUI::IPanel >( "GUI::IPanel" );
}



namespace GUI
{


IPanel::IPanel( IControl* parent )
	:	IControl( parent )
{ }

IPanel::~IPanel()
{ }


}	// GUI