#include "IControl.h"

#include "swGUI/Core/Controls/TopLevelControl.h"
#include "swGUI/Core/System/HostWindow.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::gui::IControl >( "sw::gui::IControl" );
	rttr::registration::class_< sw::gui::TopLevelControl >( "sw::gui::TopLevelControl" );
}


namespace sw {
namespace gui
{


/**@brief */
IControl::IControl( IControl* parent )
	: m_parent( parent )
{ }

/**@brief */
IControl::~IControl()
{ }


/**@brief Zwraca najwy¿sz¹ w hierarchi kontrolkê.*/
TopLevelControl*	IControl::GetRootControl()
{
	if( m_parent )
		return m_parent->GetRootControl();
	else
		return static_cast<TopLevelControl*>( this );
}

/**@brief Zwraca obiekt zarz¹dzaj¹cy GUI.*/
HostWindow*			IControl::GetHost()
{
	TopLevelControl* topLevel = GetRootControl();
	return topLevel->GetHostWindow();
}

///**@brief Gets name of control if it was set. Otherwise it returns empty string.*/
//const std::string&	IControl::GetName()
//{
//	auto host = GetHost();
//	return host->GetControlName( this );
//}

}	// gui
}	// sw
