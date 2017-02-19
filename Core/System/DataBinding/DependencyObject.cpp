/**
@file DependencyObject.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "DependencyObject.h"


RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_< sw::gui::DependencyObject >( "sw::gui::DependencyObject" );


};



namespace sw {
namespace gui
{


// ================================ //
//
DependencyObject::DependencyObject()
	: m_dataContext( nullptr )
{ }




}	// gui
}	// sw

