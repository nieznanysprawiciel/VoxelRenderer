#include "DockPanel.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< GUI::DockPanel >( "GUI::DockPanel" );
}



namespace GUI
{


DockPanel::DockPanel( IControl* parent )
	:	IPanel( parent )
{ }

DockPanel::~DockPanel()
{ }


}	// GUI