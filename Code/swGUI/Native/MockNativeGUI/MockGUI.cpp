/**
@file MockGUI.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "MockGUI.h"
#include "MockWindow.h"

#include "swInputLibrary/InputCore/Debugging/DebugInput.h"



namespace sw {
namespace gui
{



// ================================ //
//
MockGUI::MockGUI()
{}


/**@brief Creates WinAPIGUI object.*/
MockGUI*		MockGUI::Create()
{
	return new MockGUI();
}



//====================================================================================//
//			Window creation	
//====================================================================================//



// ================================ //
///
sw::input::IInput*		MockGUI::UseNativeInput()
{
	input::InputInitInfo info;
	input::IInput* input = new input::DebugInput();
	
	if( input->Init( info ) )
		return input;

	return nullptr;
}



/**@copydoc INativeGUI::CreateWindow*/
INativeWindow*	MockGUI::CreateWindow		( NativeWindowDescriptor& descriptor )
{
	MockWindow* newWindow = MockWindow::CreateWindowInstance( descriptor );
	return newWindow;
}


/**@copydoc INativeGUI::Init*/
bool		MockGUI::Init				( const NativeGUIInitData& initData )
{
	assert( !initData.FocusChanged.empty() );

	m_initData = initData;
	return true;
}



//====================================================================================//
//			Window Message handler
//====================================================================================//


/**@copydoc INativeGUI::MainLoop*/
bool		MockGUI::MainLoop                 ( bool blockingMode )
{
	return false;
}



}	// gui
}	// sw
