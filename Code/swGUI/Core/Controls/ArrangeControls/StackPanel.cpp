#include "StackPanel.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::gui::StackPanel >( "sw::gui::StackPanel" );
}

namespace sw {
namespace gui
{




StackPanel::StackPanel( IControl* parent )
	: IPanel( parent )
{ }

StackPanel::~StackPanel()
{ }


}	// gui
}	// sw

