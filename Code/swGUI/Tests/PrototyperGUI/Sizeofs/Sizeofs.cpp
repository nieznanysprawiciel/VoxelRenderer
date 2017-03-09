#include "Sizeofs.h"


#include "swCommonLib/Common/TypesDefinitions.h"


#include <iostream>
#include <iomanip>


#include "swGUI/Core/System/GUISystem.h"
#include "swGUI/Core/Controls/IControl.h"
#include "swGUI/Core/Controls/UIElement.h"
#include "swGUI/Core/Controls/Visual.h"



class VirtualEmptyClass : std::enable_shared_from_this< VirtualEmptyClass >
{

	virtual ~VirtualEmptyClass() { }
};



const int NameSize = 60;


// ================================ //
//
template< typename Type >
void			PrintSizeofType		( std::ostream& stream )
{
	stream << std::left;
	stream << std::setw( NameSize );
	stream << typeid( Type ).name();
	stream << sizeof( Type ) << std::endl;
}


// ================================ //
//
void			PrintSizeofs()
{
	std::cout << std::left;
	std::cout << "GUI Elements:" << std::endl;
	std::cout << std::setw( NameSize ) << "Objects name"  << "Sizeof" << std::endl;

	PrintSizeofType< sw::gui::IControl >( std::cout );
	PrintSizeofType< sw::gui::Visual >( std::cout );
	PrintSizeofType< sw::gui::UIElement >( std::cout );


	std::cout << std::endl << "Other:" << std::endl;
	std::cout << std::setw( NameSize ) << "Objects name" << "Sizeof" << std::endl;
	
	std::cout << std::setw( NameSize ) << "std::enable_shared_from_this additional size" << ( sizeof( VirtualEmptyClass ) - sizeof( VirtualEmptyClass* ) ) << std::endl;
}
