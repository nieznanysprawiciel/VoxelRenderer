#include "Sizeofs.h"


#include "swCommonLib/Common/TypesDefinitions.h"


#include <iostream>
#include <iomanip>


#include "swGUI/Core/System/GUISystem.h"
#include "swGUI/Core/Controls/IControl.h"

const int NameSize = 30;


// ================================ //
//
template< typename Type >
void			PrintSizeofType		( std::ostream& stream )
{
	stream << typeid( Type ).name();
	stream << std::setw( NameSize );
	stream << sizeof( Type ) << std::endl;
}


// ================================ //
//
void			PrintSizeofs()
{
	std::cout << "Objects name" << std::setw( NameSize ) << "Sizeof" << std::endl;

	PrintSizeofType< sw::gui::IControl >( std::cout );

}
