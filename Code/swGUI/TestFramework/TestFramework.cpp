/**
@file TestFramework.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swGUI/TestFramework/stdafx.h"


#include "TestFramework.h"
#include "swInputLibrary/InputCore/Debugging/DebugInput.h"
#include "swGUI/Native/MockNativeGUI/MockGUI.h"



namespace sw {
namespace gui
{


// ================================ //
//
TestFramework::TestFramework		( int argc, char** argv )
	:	GUISystem( argc, argv, new MockGUI() )
	,	m_eventCapture( new input::EventCapture() )
{}


// ================================ //
//
bool			TestFramework::Initialize()
{
	m_resourceManager = new ResourceManager();
	
	DefaultInitGraphicAPI( true, true );
	InitTesterNativeGUI();
	InitTesterDebugInput();
	DefaultInitRenderingSystem();

	return true;
}

// ================================ //
//
bool			TestFramework::InitTesterNativeGUI		()
{
	NativeGUIInitData nativeGUIInit;
	nativeGUIInit.FocusChanged = fastdelegate::MakeDelegate( this, &GUISystem::OnFocusChanged );

	m_nativeGUI->Init( nativeGUIInit );

	return true;
}

// ================================ //
//
void			TestFramework::InitTesterDebugInput		()
{
	input::InputInitInfo info;
	info.EventCapturer = m_eventCapture;

	m_input = new input::DebugInput();
	
	bool result = m_input->Init( info );
	assert( result );
}


// ================================ //
//
bool			TestFramework::TesterMainStep()
{
	return !MainLoopCore();
}


}	// gui
}	// sw


