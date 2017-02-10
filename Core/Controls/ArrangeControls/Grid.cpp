#include "Grid.h"

RTTR_REGISTRATION
{
	rttr::registration::class_< GUI::Grid >( "GUI::Grid" );
}


namespace GUI
{

/**@brief */
Grid::Grid( IControl* parent )
	:IPanel( parent )
{}

/**@brief */
Grid::~Grid()
{ }

}	// GUI