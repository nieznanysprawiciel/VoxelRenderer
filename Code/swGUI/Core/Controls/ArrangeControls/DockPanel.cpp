#include "DockPanel.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< sw::gui::DockPanel >( "sw::gui::DockPanel" );
}



namespace sw {
namespace gui
{


DockPanel::DockPanel( IControl* parent )
	: IPanel( parent )
{ }

DockPanel::~DockPanel()
{ }


}	// gui
}	// sw

