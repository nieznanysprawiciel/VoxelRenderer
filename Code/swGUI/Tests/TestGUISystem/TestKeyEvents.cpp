#include "swCommonLib/External/Catch/catch.hpp"

#include "swGUI/TestFramework/TestFramework.h"

using namespace sw;
using namespace sw::gui;


std::vector< input::Keyboard::PhysicalKeys >	upKeys;
std::vector< input::Keyboard::PhysicalKeys >	downKeys;



// ================================ //
//
void		KeyEventReceived			( UIElement* sender, KeyEventArgs* e )
{
	CHECK( e->IsUp != e->IsDown );

	if( e->IsUp )
		upKeys.push_back( e->Key );
	if( e->IsDown )
		downKeys.push_back( e->Key );
	
	CHECK( e->State.IsUp() == e->IsUp );
	CHECK( e->State.IsPressed() == e->IsDown );

	CHECK( e->State.IsKeyUpEvent() == e->IsUp );
	CHECK( e->State.IsKeyDownEvent() == e->IsDown );

	CHECK( e->Keyboard->GetState()[ e->Key ].IsUp() == e->IsUp );
	CHECK( e->Keyboard->GetState()[ e->Key ].IsPressed() == e->IsDown );

	CHECK( e->Keyboard->GetState()[ e->Key ].IsKeyUpEvent() == e->IsUp );
	CHECK( e->Keyboard->GetState()[ e->Key ].IsKeyDownEvent() == e->IsDown );
}

// ================================ //
//
void		PreviewKeyEventReceivedHandled		( UIElement* sender, KeyEventArgs* e )
{
	KeyEventReceived( sender, e );
	e->Handled = true;
}

// ================================ //
//
void		CheckVectorsContent					( const std::vector< input::Keyboard::PhysicalKeys >& testUpKeys, const std::vector< input::Keyboard::PhysicalKeys >& testDownKeys )
{
	CHECK( upKeys.size() == testUpKeys.size() );
	for( Size i = 0; i < std::min( upKeys.size(), testUpKeys.size() ); ++i )
	{
		INFO( i );
		CHECK( upKeys[ i ] == testUpKeys[ i ] );
	}

	CHECK( downKeys.size() == testDownKeys.size() );
	for( Size i = 0; i < std::min( downKeys.size(), testDownKeys.size() ); ++i )
	{
		INFO( i );
		CHECK( downKeys[ i ] == testDownKeys[ i ] );
	}
}



// ================================ //
//
TEST_CASE( "KeyUp/KeyDown event" )
{
	// Initialize framework.
	TestFramework framework( 0, nullptr );
	framework.Init();

	// Create main window (fake window) and get input::EventCapture. 
	HostWindow* window = framework.CreateNativeHostWindow( 400, 400, "TestWindow" );
	input::EventCapture* eventCapturer = framework.GetEventCapturer();

	// Set focus to window.
	framework.OnFocusChanged( window->GetNativeWindow(), true );

	// Add event handlers to tested events.
	window->KeyUp() += EventDelegate< KeyEventArgs >( &KeyEventReceived );
	window->KeyDown() += EventDelegate< KeyEventArgs >( &KeyEventReceived );

	window->PreviewKeyUp() += EventDelegate< KeyEventArgs >( &KeyEventReceived );
	window->PreviewKeyDown() += EventDelegate< KeyEventArgs >( &KeyEventReceived );


// ================================ //
//
	std::vector< input::Keyboard::PhysicalKeys >	testUpKeys;
	std::vector< input::Keyboard::PhysicalKeys >	testDownKeys;


// ================================ //
// Test single down event
	eventCapturer->QueueDownKeyEvent( input::Keyboard::PhysicalKeys::KEY_J );
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_J );			// PreviewKeyDown
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_J );			// KeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );


// ================================ //
// Test up and down event
	eventCapturer->QueueUpKeyEvent( input::Keyboard::PhysicalKeys::KEY_J );
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_J );			// PreviewKeyDown
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_J );			// KeyDown

	eventCapturer->QueueDownKeyEvent( input::Keyboard::PhysicalKeys::KEY_A );
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_A );			// PreviewKeyDown
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_A );			// KeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );

// ================================ //
// Test multiple events in row in the same frame
	eventCapturer->QueueUpKeyEvent( input::Keyboard::PhysicalKeys::KEY_A );
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_A );			// PreviewKeyDown
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_A );			// KeyDown

	eventCapturer->QueueDownKeyEvent( input::Keyboard::PhysicalKeys::KEY_M );
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_M );			// PreviewKeyDown
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_M );			// KeyDown

	eventCapturer->QueueDownKeyEvent( input::Keyboard::PhysicalKeys::KEY_O );
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_O );			// PreviewKeyDown
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_O );			// KeyDown

	eventCapturer->QueueUpKeyEvent( input::Keyboard::PhysicalKeys::KEY_M );
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_M );			// PreviewKeyDown
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_M );			// KeyDown

	eventCapturer->QueueUpKeyEvent( input::Keyboard::PhysicalKeys::KEY_O );
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_O );			// PreviewKeyDown
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_O );			// KeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );

// ================================ //
// Mark event as handled

	window->PreviewKeyUp() -= EventDelegate< KeyEventArgs >( &KeyEventReceived );
	window->PreviewKeyDown() -= EventDelegate< KeyEventArgs >( &KeyEventReceived );

	window->PreviewKeyUp() -= EventDelegate< KeyEventArgs >( &PreviewKeyEventReceivedHandled );
	window->PreviewKeyDown() -= EventDelegate< KeyEventArgs >( &PreviewKeyEventReceivedHandled );


// ================================ //
// Send some events
	eventCapturer->QueueDownKeyEvent( input::Keyboard::PhysicalKeys::KEY_J );
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_J );			// PreviewKeyDown

	eventCapturer->QueueDownKeyEvent( input::Keyboard::PhysicalKeys::KEY_A );
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_A );			// PreviewKeyDown

	eventCapturer->QueueUpKeyEvent( input::Keyboard::PhysicalKeys::KEY_J );
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_J );			// PreviewKeyDown

	eventCapturer->QueueUpKeyEvent( input::Keyboard::PhysicalKeys::KEY_A );
	testUpKeys.push_back( input::Keyboard::PhysicalKeys::KEY_A );			// PreviewKeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );


// ================================ //
// Check state after Main loop

	eventCapturer->QueueDownKeyEvent( input::Keyboard::PhysicalKeys::KEY_Z );
	testDownKeys.push_back( input::Keyboard::PhysicalKeys::KEY_Z );			// PreviewKeyDown

	// Make two loop steps to 
	framework.TesterMainStep();
	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );

	auto& keybord = framework.GetInput()->GetKeyboardDevice()[ 0 ];
	const input::KeyState& state = keybord->GetState()[ input::Keyboard::PhysicalKeys::KEY_Z ];

	CHECK( state.IsKeyDownEvent() == false );
	CHECK( state.IsKeyUpEvent() == false );
	CHECK( state.IsPressed() == true );
	CHECK( state.IsUp() == false );
}
