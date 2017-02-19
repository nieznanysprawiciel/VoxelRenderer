#include "WrapPanel.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::gui::WrapPanel >( "sw::gui::WrapPanel" );
}



namespace sw {
namespace gui
{


WrapPanel::WrapPanel( IControl* parent )
	: IPanel( parent )
{ }

WrapPanel::~WrapPanel()
{ }


}	// gui
}	// sw

