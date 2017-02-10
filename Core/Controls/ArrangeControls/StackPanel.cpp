#include "StackPanel.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< GUI::StackPanel >( "GUI::StackPanel" );
}

namespace GUI
{




StackPanel::StackPanel( IControl* parent )
	:	IPanel( parent )
{ }

StackPanel::~StackPanel()
{ }


}	// GUI