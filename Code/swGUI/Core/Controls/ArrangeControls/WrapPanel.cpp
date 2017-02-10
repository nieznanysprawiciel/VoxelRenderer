#include "WrapPanel.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< GUI::WrapPanel >( "GUI::WrapPanel" );
}



namespace GUI
{


WrapPanel::WrapPanel( IControl* parent )
	:	IPanel( parent )
{ }

WrapPanel::~WrapPanel()
{ }


}	// GUI