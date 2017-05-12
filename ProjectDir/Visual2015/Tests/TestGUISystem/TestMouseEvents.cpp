#include "swCommonLib/External/Catch/catch.hpp"

#include "swGUI/TestFramework/TestFramework.h"

using namespace sw;
using namespace sw::gui;


std::vector< input::Mouse::PhysicalButtons >	upKeys;
std::vector< input::Mouse::PhysicalButtons >	downKeys;
std::vector< float >							mousePos;
std::vector< float >							mouseDeltas;


void		MouseEventReceived			( UIElement* sender, MouseEventArgs* e );


// ================================ //
//
void		MouseButtonEventReceived			( UIElement* sender, MouseButtonEventArgs* e )
{
	CHECK( e->IsUp != e->IsDown );

	if( e->IsUp )
		upKeys.push_back( e->Button );
	if( e->IsDown )
		downKeys.push_back( e->Button );
	
	CHECK( e->State.IsUp() == e->IsUp );
	CHECK( e->State.IsPressed() == e->IsDown );

	CHECK( e->State.IsKeyUpEvent() == e->IsUp );
	CHECK( e->State.IsKeyDownEvent() == e->IsDown );

	CHECK( e->Mouse->GetState()[ e->Button ].IsUp() == e->IsUp );
	CHECK( e->Mouse->GetState()[ e->Button ].IsPressed() == e->IsDown );

	CHECK( e->Mouse->GetState()[ e->Button ].IsKeyUpEvent() == e->IsUp );
	CHECK( e->Mouse->GetState()[ e->Button ].IsKeyDownEvent() == e->IsDown );

	MouseEventReceived( sender, e );
}

// ================================ //
//
void		PreviewKeyEventReceivedHandled		( UIElement* sender, MouseButtonEventArgs* e )
{
	MouseButtonEventReceived( sender, e );
	e->Handled = true;
}

// ================================ //
//
void		MouseEventReceived			( UIElement* sender, MouseEventArgs* e )
{
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::LEFT_BUTTON ].IsUp() == e->LeftButton->IsUp() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::LEFT_BUTTON ].IsPressed() == e->LeftButton->IsPressed() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::LEFT_BUTTON ].IsKeyUpEvent() == e->LeftButton->IsKeyUpEvent() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::LEFT_BUTTON ].IsKeyDownEvent() == e->LeftButton->IsKeyDownEvent() );

	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::RIGHT_BUTTON ].IsUp() == e->RightButton->IsUp() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::RIGHT_BUTTON ].IsPressed() == e->RightButton->IsPressed() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::RIGHT_BUTTON ].IsKeyUpEvent() == e->RightButton->IsKeyUpEvent() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::RIGHT_BUTTON ].IsKeyDownEvent() == e->RightButton->IsKeyDownEvent() );

	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::MIDDLE_BUTTON ].IsUp() == e->MiddleButton->IsUp() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::MIDDLE_BUTTON ].IsPressed() == e->MiddleButton->IsPressed() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::MIDDLE_BUTTON ].IsKeyUpEvent() == e->MiddleButton->IsKeyUpEvent() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::MIDDLE_BUTTON ].IsKeyDownEvent() == e->MiddleButton->IsKeyDownEvent() );

	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON1 ].IsUp() == e->XButton1->IsUp() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON1 ].IsPressed() == e->XButton1->IsPressed() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON1 ].IsKeyUpEvent() == e->XButton1->IsKeyUpEvent() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON1 ].IsKeyDownEvent() == e->XButton1->IsKeyDownEvent() );

	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON2 ].IsUp() == e->XButton2->IsUp() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON2 ].IsPressed() == e->XButton2->IsPressed() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON2 ].IsKeyUpEvent() == e->XButton2->IsKeyUpEvent() );
	CHECK( e->Mouse->GetState()[ input::Mouse::PhysicalButtons::XBUTTON2 ].IsKeyDownEvent() == e->XButton2->IsKeyDownEvent() );

	mousePos.push_back( e->WindowPosX );
	mousePos.push_back( e->WindowPosY );

	CHECK( e->WindowPosX == e->Mouse->GetState().GetPositionX() );
	CHECK( e->WindowPosY == e->Mouse->GetState().GetPositionY() );
}


// ================================ //
//
void		MouseMoveEventReceived			( UIElement* sender, MouseMoveEventArgs* e )
{
	MouseEventReceived( sender, e );

	///
	mouseDeltas.push_back( e->MouseDeltaX );
	mouseDeltas.push_back( e->MouseDeltaY );
}

// ================================ //
//
void		MouseMoveEventReceivedHandled	( UIElement* sender, MouseMoveEventArgs* e )
{
	MouseMoveEventReceived( sender, e );
	e->Handled = true;
}


// ================================ //
//
template< typename VecElement >
void		CompareContent						( const std::vector< VecElement >& testVec, const std::vector< VecElement >& refVec )
{
	CHECK( testVec.size() == refVec.size() );
	for( Size i = 0; i < std::min( testVec.size(), refVec.size() ); ++i )
	{
		INFO( i );
		CHECK( testVec[ i ] == refVec[ i ] );
	}
}


// ================================ //
//
void		CheckVectorsContent					( const std::vector< input::Mouse::PhysicalButtons >& testUpKeys, const std::vector< input::Mouse::PhysicalButtons >& testDownKeys )
{
	CompareContent( upKeys, testUpKeys );
	CompareContent( downKeys, testDownKeys );
}




// ================================ //
//
TEST_CASE( "MouseButtonUp/MouseButtonDown event" )
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
	window->MouseDown() += EventDelegate< MouseButtonEventArgs >( &MouseButtonEventReceived );
	window->MouseUp() += EventDelegate< MouseButtonEventArgs >( &MouseButtonEventReceived );

	window->PreviewMouseUp() += EventDelegate< MouseButtonEventArgs >( &MouseButtonEventReceived );
	window->PreviewMouseDown() += EventDelegate< MouseButtonEventArgs >( &MouseButtonEventReceived );


// ================================ //
//
	std::vector< input::Mouse::PhysicalButtons >	testUpKeys;
	std::vector< input::Mouse::PhysicalButtons >	testDownKeys;


// ================================ //
// Test single down event
	eventCapturer->QueueMouseClick( input::Mouse::PhysicalButtons::LEFT_BUTTON );
	testDownKeys.push_back( input::Mouse::PhysicalButtons::LEFT_BUTTON );			// PreviewKeyDown
	testDownKeys.push_back( input::Mouse::PhysicalButtons::LEFT_BUTTON );			// KeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );


// ================================ //
// Test up and down event
	eventCapturer->QueueMouseUp( input::Mouse::PhysicalButtons::LEFT_BUTTON );
	testUpKeys.push_back( input::Mouse::PhysicalButtons::LEFT_BUTTON );			// PreviewKeyDown
	testUpKeys.push_back( input::Mouse::PhysicalButtons::LEFT_BUTTON );			// KeyDown

	eventCapturer->QueueMouseClick( input::Mouse::PhysicalButtons::RIGHT_BUTTON );
	testDownKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// PreviewKeyDown
	testDownKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// KeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );

// ================================ //
// Test multiple events in row in the same frame
	eventCapturer->QueueMouseUp( input::Mouse::PhysicalButtons::RIGHT_BUTTON );
	testUpKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// PreviewKeyDown
	testUpKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// KeyDown

	eventCapturer->QueueMouseClick( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );
	testDownKeys.push_back( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );			// PreviewKeyDown
	testDownKeys.push_back( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );			// KeyDown

	eventCapturer->QueueMouseClick( input::Mouse::PhysicalButtons::RIGHT_BUTTON );
	testDownKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// PreviewKeyDown
	testDownKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// KeyDown

	eventCapturer->QueueMouseUp( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );
	testUpKeys.push_back( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );			// PreviewKeyDown
	testUpKeys.push_back( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );			// KeyDown

	eventCapturer->QueueMouseUp( input::Mouse::PhysicalButtons::RIGHT_BUTTON );
	testUpKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// PreviewKeyDown
	testUpKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// KeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );

// ================================ //
// Mark event as handled

	window->PreviewMouseUp() -= EventDelegate< MouseButtonEventArgs >( &MouseButtonEventReceived );
	window->PreviewMouseDown() -= EventDelegate< MouseButtonEventArgs >( &MouseButtonEventReceived );

	window->PreviewMouseUp() -= EventDelegate< MouseButtonEventArgs >( &PreviewKeyEventReceivedHandled );
	window->PreviewMouseDown() -= EventDelegate< MouseButtonEventArgs >( &PreviewKeyEventReceivedHandled );


// ================================ //
// Send some events
	eventCapturer->QueueMouseClick( input::Mouse::PhysicalButtons::LEFT_BUTTON );
	testDownKeys.push_back( input::Mouse::PhysicalButtons::LEFT_BUTTON );			// PreviewKeyDown

	eventCapturer->QueueMouseClick( input::Mouse::PhysicalButtons::RIGHT_BUTTON );
	testDownKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// PreviewKeyDown

	eventCapturer->QueueMouseUp( input::Mouse::PhysicalButtons::LEFT_BUTTON );
	testUpKeys.push_back( input::Mouse::PhysicalButtons::LEFT_BUTTON );			// PreviewKeyDown

	eventCapturer->QueueMouseUp( input::Mouse::PhysicalButtons::RIGHT_BUTTON );
	testUpKeys.push_back( input::Mouse::PhysicalButtons::RIGHT_BUTTON );			// PreviewKeyDown

	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );


// ================================ //
// Check state after Main loop

	eventCapturer->QueueMouseClick( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );
	testDownKeys.push_back( input::Mouse::PhysicalButtons::MIDDLE_BUTTON );			// PreviewKeyDown

	// Make two loop steps to 
	framework.TesterMainStep();
	framework.TesterMainStep();
	CheckVectorsContent( testUpKeys, testDownKeys );

	auto& mouse = framework.GetInput()->GetMouseDevice()[ 0 ];
	const input::KeyState& state = mouse->GetState()[ input::Mouse::PhysicalButtons::MIDDLE_BUTTON ];

	CHECK( state.IsKeyDownEvent() == false );
	CHECK( state.IsKeyUpEvent() == false );
	CHECK( state.IsPressed() == true );
	CHECK( state.IsUp() == false );
}


// ================================ //
//
void		AddMouseMove		( input::EventCapture* eventCapturer, std::vector< float >& testMouseDeltas, int deltaX, int deltaY, bool handled = false )
{
	eventCapturer->QueueMouseMove( deltaX, deltaY );
	testMouseDeltas.push_back( (float)deltaX );
	testMouseDeltas.push_back( (float)deltaY );

	if( !handled )
	{
		testMouseDeltas.push_back( (float)deltaX );
		testMouseDeltas.push_back( (float)deltaY );
	}
}

// ================================ //
//
TEST_CASE( "MouseMove event" )
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
	window->MouseMove() += EventDelegate< MouseMoveEventArgs >( &MouseMoveEventReceived );
	window->PreviewMouseMove() += EventDelegate< MouseMoveEventArgs >( &MouseMoveEventReceived );


// ================================ //
//
	std::vector< float >	testMousePos;
	std::vector< float >	testMouseDeltas;

	AddMouseMove( eventCapturer, testMouseDeltas, 40, -30 );

	framework.TesterMainStep();
	CompareContent( testMouseDeltas, mouseDeltas );

// ================================ //
//
	AddMouseMove( eventCapturer, testMouseDeltas, 1000, 1000 );

	framework.TesterMainStep();
	CompareContent( testMouseDeltas, mouseDeltas );

// ================================ //
//
	AddMouseMove( eventCapturer, testMouseDeltas, -1000, -1000 );

	framework.TesterMainStep();
	CompareContent( testMouseDeltas, mouseDeltas );

// ================================ //
//
	window->PreviewMouseMove() -= EventDelegate< MouseMoveEventArgs >( &MouseMoveEventReceived );
	window->PreviewMouseMove() += EventDelegate< MouseMoveEventArgs >( &MouseMoveEventReceivedHandled );

// ================================ //
//
	AddMouseMove( eventCapturer, testMouseDeltas, 1000, 1000, true );

	framework.TesterMainStep();
	CompareContent( testMouseDeltas, mouseDeltas );
}


