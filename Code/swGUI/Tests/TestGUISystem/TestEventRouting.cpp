#include "swCommonLib/External/Catch/catch.hpp"

#include "swGUI/Core/Controls/UIElement.h"
#include "swGUI/Core/System/Events/IEventArgs.h"


#include "Classes/TestUIElementClass.h"


using namespace sw::gui;



int validateElement0 = 0;
int validateElement1 = 0;
int validateElement2 = 0;
int validateElement3 = 0;
int validateElement4 = 0;
int validateElement5 = 0;

std::vector< int > eventsOrder;


// ================================ //
//
void		CleanGlobals()
{
	validateElement0 = 0;
	validateElement1 = 0;
	validateElement2 = 0;
	validateElement3 = 0;
	validateElement4 = 0;
	validateElement5 = 0;

	eventsOrder.clear();
}

// ================================ //
//
void		CheckUIElement0	( UIElement* sender, ValidationEventArgs* e )
{
	validateElement0++;
	eventsOrder.push_back( 0 );
}

// ================================ //
//
void		CheckUIElement1	( UIElement* sender, ValidationEventArgs* e )
{
	validateElement1++;
	eventsOrder.push_back( 1 );
}

// ================================ //
//
void		CheckUIElement2	( UIElement* sender, ValidationEventArgs* e )
{
	validateElement2++;
	eventsOrder.push_back( 2 );
}

// ================================ //
//
void		CheckUIElement3	( UIElement* sender, ValidationEventArgs* e )
{
	validateElement3++;
	eventsOrder.push_back( 3 );
}

// ================================ //
//
void		CheckUIElement4	( UIElement* sender, ValidationEventArgs* e )
{
	validateElement4++;
	eventsOrder.push_back( 4 );
}

// ================================ //
//
void		CheckUIElement5	( UIElement* sender, ValidationEventArgs* e )
{
	validateElement5++;
	eventsOrder.push_back( 5 );
}


//====================================================================================//
//			Test code	
//====================================================================================//


TEST_CASE( "Tunnel Events Routing" )
{
	CleanGlobals();

	// Build tree
	TestUIElementClassOPtr root = TestUIElementClassOPtr( new TestUIElementClass() );
	
	TestUIElementClass* level1Child1 = new TestUIElementClass();
	TestUIElementClass* level1Child2 = new TestUIElementClass();
	TestUIElementClass* level1Child3 = new TestUIElementClass();

	TestUIElementClass* level2Child4 = new TestUIElementClass();
	TestUIElementClass* level2Child5 = new TestUIElementClass();

	bool result = root->AddChild( TestUIElementClassOPtr( level1Child1 ) );
	CHECK( result );
	CHECK( level1Child1->GetParent() == root.get() );

	result = root->AddChild( TestUIElementClassOPtr( level1Child2 ) );
	CHECK( result );
	CHECK( level1Child2->GetParent() == root.get() );

	result = root->AddChild( TestUIElementClassOPtr( level1Child3 ) );
	CHECK( result );
	CHECK( level1Child3->GetParent() == root.get() );

	result = level1Child1->AddChild( TestUIElementClassOPtr( level2Child4 ) );
	CHECK( result );
	CHECK( level2Child4->GetParent() == level1Child1 );

	result = level1Child1->AddChild( TestUIElementClassOPtr( level2Child5 ) );
	CHECK( result );
	CHECK( level2Child5->GetParent() == level1Child1 );

	// Assign delegates
	root->PreviewTunnelEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement0 );
	level1Child1->PreviewTunnelEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement1 );
	level1Child2->PreviewTunnelEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement2 );
	level1Child3->PreviewTunnelEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement3 );
	level2Child4->PreviewTunnelEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement4 );
	level2Child5->PreviewTunnelEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement5 );

	std::vector< int > eventsOrderValidation;

// ================================ //
//

	// 
	level2Child4->RaiseTunnelEvent();

	CHECK( validateElement0 == 1 );
	CHECK( validateElement1 == 1 );
	CHECK( validateElement2 == 0 );
	CHECK( validateElement3 == 0 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 0 );

	eventsOrderValidation.push_back( 0 );
	eventsOrderValidation.push_back( 1 );
	eventsOrderValidation.push_back( 4 );

	//
	level2Child5->RaiseTunnelEvent();

	CHECK( validateElement0 == 2 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 0 );
	CHECK( validateElement3 == 0 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 0 );
	eventsOrderValidation.push_back( 1 );
	eventsOrderValidation.push_back( 5 );

	//
	level1Child3->RaiseTunnelEvent();

	CHECK( validateElement0 == 3 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 0 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 0 );
	eventsOrderValidation.push_back( 3 );

	//
	level1Child2->RaiseTunnelEvent();

	CHECK( validateElement0 == 4 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 0 );
	eventsOrderValidation.push_back( 2 );

	//
	root->RaiseTunnelEvent();

	CHECK( validateElement0 == 5 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 0 );

	//
	level1Child1->RaiseTunnelEvent();

	CHECK( validateElement0 == 6 );
	CHECK( validateElement1 == 3 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 0 );
	eventsOrderValidation.push_back( 1 );


	level1Child1->PreviewTunnelEvent() -= EventDelegate< ValidationEventArgs >( CheckUIElement1 );

	//
	level2Child4->RaiseTunnelEvent();

	CHECK( validateElement0 == 7 );
	CHECK( validateElement1 == 3 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 2 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 0 );
	eventsOrderValidation.push_back( 4 );

	//
	level2Child5->RaiseTunnelEvent();

	CHECK( validateElement0 == 8 );
	CHECK( validateElement1 == 3 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 2 );
	CHECK( validateElement5 == 2 );

	eventsOrderValidation.push_back( 0 );
	eventsOrderValidation.push_back( 5 );

	REQUIRE( eventsOrderValidation.size() == eventsOrder.size() );
	for( int i = 0; i < eventsOrderValidation.size(); ++i )
	{
		INFO( i );
		CHECK( eventsOrderValidation[ i ] == eventsOrder[ i ] );
	}
}



TEST_CASE( "Tunnel Bubble Routing" )
{
	CleanGlobals();

	// Build tree
	TestUIElementClassOPtr root = TestUIElementClassOPtr( new TestUIElementClass() );

	TestUIElementClass* level1Child1 = new TestUIElementClass();
	TestUIElementClass* level1Child2 = new TestUIElementClass();
	TestUIElementClass* level1Child3 = new TestUIElementClass();

	TestUIElementClass* level2Child4 = new TestUIElementClass();
	TestUIElementClass* level2Child5 = new TestUIElementClass();

	bool result = root->AddChild( TestUIElementClassOPtr( level1Child1 ) );
	CHECK( result );
	CHECK( level1Child1->GetParent() == root.get() );

	result = root->AddChild( TestUIElementClassOPtr( level1Child2 ) );
	CHECK( result );
	CHECK( level1Child2->GetParent() == root.get() );

	result = root->AddChild( TestUIElementClassOPtr( level1Child3 ) );
	CHECK( result );
	CHECK( level1Child3->GetParent() == root.get() );

	result = level1Child1->AddChild( TestUIElementClassOPtr( level2Child4 ) );
	CHECK( result );
	CHECK( level2Child4->GetParent() == level1Child1 );

	result = level1Child1->AddChild( TestUIElementClassOPtr( level2Child5 ) );
	CHECK( result );
	CHECK( level2Child5->GetParent() == level1Child1 );

	// Assign delegates
	root->BubbleEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement0 );
	level1Child1->BubbleEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement1 );
	level1Child2->BubbleEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement2 );
	level1Child3->BubbleEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement3 );
	level2Child4->BubbleEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement4 );
	level2Child5->BubbleEvent() += EventDelegate< ValidationEventArgs >( CheckUIElement5 );

	std::vector< int > eventsOrderValidation;

// ================================ //
//

	// 
	level2Child4->RaiseBubbleEvent();

	CHECK( validateElement0 == 1 );
	CHECK( validateElement1 == 1 );
	CHECK( validateElement2 == 0 );
	CHECK( validateElement3 == 0 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 0 );

	eventsOrderValidation.push_back( 4 );
	eventsOrderValidation.push_back( 1 );
	eventsOrderValidation.push_back( 0 );

	//
	level2Child5->RaiseBubbleEvent();

	CHECK( validateElement0 == 2 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 0 );
	CHECK( validateElement3 == 0 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 5 );
	eventsOrderValidation.push_back( 1 );
	eventsOrderValidation.push_back( 0 );

	//
	level1Child3->RaiseBubbleEvent();

	CHECK( validateElement0 == 3 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 0 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 3 );
	eventsOrderValidation.push_back( 0 );

	//
	level1Child2->RaiseBubbleEvent();

	CHECK( validateElement0 == 4 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 2 );
	eventsOrderValidation.push_back( 0 );

	//
	root->RaiseBubbleEvent();

	CHECK( validateElement0 == 5 );
	CHECK( validateElement1 == 2 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 0 );

	//
	level1Child1->RaiseBubbleEvent();

	CHECK( validateElement0 == 6 );
	CHECK( validateElement1 == 3 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 1 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 1 );
	eventsOrderValidation.push_back( 0 );

	level1Child1->BubbleEvent() -= EventDelegate< ValidationEventArgs >( CheckUIElement1 );

	//
	level2Child4->RaiseBubbleEvent();

	CHECK( validateElement0 == 7 );
	CHECK( validateElement1 == 3 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 2 );
	CHECK( validateElement5 == 1 );

	eventsOrderValidation.push_back( 4 );
	eventsOrderValidation.push_back( 0 );

	//
	level2Child5->RaiseBubbleEvent();

	CHECK( validateElement0 == 8 );
	CHECK( validateElement1 == 3 );
	CHECK( validateElement2 == 1 );
	CHECK( validateElement3 == 1 );
	CHECK( validateElement4 == 2 );
	CHECK( validateElement5 == 2 );

	eventsOrderValidation.push_back( 5 );
	eventsOrderValidation.push_back( 0 );


	REQUIRE( eventsOrderValidation.size() == eventsOrder.size() );
	for( int i = 0; i < eventsOrderValidation.size(); ++i )
	{
		INFO( i );
		CHECK( eventsOrderValidation[ i ] == eventsOrder[ i ] );
	}
}
