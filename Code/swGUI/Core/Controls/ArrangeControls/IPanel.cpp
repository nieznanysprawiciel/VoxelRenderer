#include "IPanel.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::gui::IPanel >( "sw::gui::IPanel" );
}



namespace sw {
namespace gui
{


IPanel::IPanel( IControl* parent )
	: IControl( parent )
{ }

IPanel::~IPanel()
{ }


}	// gui
}	// sw

