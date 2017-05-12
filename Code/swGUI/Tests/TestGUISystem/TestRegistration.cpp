#include "swCommonLib/External/Catch/catch.hpp"

#include "swGUI/Core/Controls/UIElement.h"
#include "swGUI/Core/System/Events/IEventArgs.h"


#include "Classes/TestUIElementClass.h"



using namespace sw::gui;




int validationDelegateCounter = 0;
int valid1Counter = 0;
int valid2Counter = 0;
int valid3Counter = 0;
int valid4Counter = 0;


// ================================ //
//
void		CleanCounters()
{
	validationDelegateCounter = 0;
	valid1Counter = 0;
	valid2Counter = 0;
	valid3Counter = 0;
	valid4Counter = 0;
}

// ================================ //
//
void		ValidationDelegate	( UIElement* sender, ValidationEventArgs* e )
{
	validationDelegateCounter++;
	valid1Counter++;
}


// ================================ //
//
void		ValidationDelegate2	( UIElement* sender, ValidationEventArgs* e )
{
	validationDelegateCounter++;
	valid2Counter++;
}

// ================================ //
//
void		ValidationDelegate3	( UIElement* sender, ValidationEventArgs* e )
{
	validationDelegateCounter++;
	valid3Counter++;
}

// ================================ //
//
void		ValidationDelegate4	( UIElement* sender, ValidationEventArgs* e )
{
	validationDelegateCounter++;
	valid4Counter++;
}

//====================================================================================//
//			Test code	
//====================================================================================//


TEST_CASE( "Raising direct events" )
{
	CleanCounters();

	TestUIElementClass testClass;

	testClass.ValidationStarted() += EventDelegate< ValidationEventArgs >( &ValidationDelegate );

	CHECK( testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	testClass.EventRaisingFunction();
	CHECK( validationDelegateCounter == 1 );
	CHECK( valid1Counter == 1 );

	testClass.ValidationStarted() -= EventDelegate< ValidationEventArgs >( &ValidationDelegate );

	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	// Test adding two delegates for the same object
	CleanCounters();

	testClass.ValidationStarted() += EventDelegate< ValidationEventArgs >( &ValidationDelegate );
	testClass.ValidationStarted() += EventDelegate< ValidationEventArgs >( &ValidationDelegate );

	CHECK( testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	testClass.EventRaisingFunction();
	CHECK( validationDelegateCounter == 1 );
	CHECK( valid1Counter == 1 );
}


TEST_CASE( "Adding/removing delegates" )
{
	CleanCounters();

	TestUIElementClass testClass;

	testClass.ValidationStarted() += EventDelegate< ValidationEventArgs >( &ValidationDelegate );

	CHECK( testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	testClass.EventRaisingFunction();
	CHECK( validationDelegateCounter == 1 );
	CHECK( valid1Counter == 1 );
	CHECK( valid2Counter == 0 );
	CHECK( valid3Counter == 0 );
	CHECK( valid4Counter == 0 );

	// Add new delegate
	testClass.ValidationStarted() += EventDelegate< ValidationEventArgs >( &ValidationDelegate2 );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );
	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 3 );
	CHECK( valid1Counter == 2 );
	CHECK( valid2Counter == 1 );
	CHECK( valid3Counter == 0 );
	CHECK( valid4Counter == 0 );

	// Add new delegate
	testClass.ValidationStarted() += EventDelegate< ValidationEventArgs >( &ValidationDelegate3 );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );
	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 6 );
	CHECK( valid1Counter == 3 );
	CHECK( valid2Counter == 2 );
	CHECK( valid3Counter == 1 );
	CHECK( valid4Counter == 0 );

	// Add new delegate
	testClass.ValidationStarted() += EventDelegate< ValidationEventArgs >( &ValidationDelegate4 );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );
	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 10 );
	CHECK( valid1Counter == 4 );
	CHECK( valid2Counter == 3 );
	CHECK( valid3Counter == 2 );
	CHECK( valid4Counter == 1 );

	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 14 );
	CHECK( valid1Counter == 5 );
	CHECK( valid2Counter == 4 );
	CHECK( valid3Counter == 3 );
	CHECK( valid4Counter == 2 );

	// Delete delegate
	testClass.ValidationStarted() -= EventDelegate< ValidationEventArgs >( &ValidationDelegate );
	CHECK( testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 17 );
	CHECK( valid1Counter == 5 );
	CHECK( valid2Counter == 5 );
	CHECK( valid3Counter == 4 );
	CHECK( valid4Counter == 3 );

	// Delete delegate
	testClass.ValidationStarted() -= EventDelegate< ValidationEventArgs >( &ValidationDelegate2 );
	CHECK( testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 19 );
	CHECK( valid1Counter == 5 );
	CHECK( valid2Counter == 5 );
	CHECK( valid3Counter == 5 );
	CHECK( valid4Counter == 4 );

	// Delete delegate
	testClass.ValidationStarted() -= EventDelegate< ValidationEventArgs >( &ValidationDelegate3 );
	CHECK( testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 20 );
	CHECK( valid1Counter == 5 );
	CHECK( valid2Counter == 5 );
	CHECK( valid3Counter == 5 );
	CHECK( valid4Counter == 5 );

	// Delete delegate
	testClass.ValidationStarted() -= EventDelegate< ValidationEventArgs >( &ValidationDelegate4 );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sValidationStarted->ID ) );
	CHECK( !testClass.ExistsDelegatesContainer( TestUIElementClass::sUnusedEvent->ID ) );

	testClass.EventRaisingFunction();

	CHECK( validationDelegateCounter == 20 );
	CHECK( valid1Counter == 5 );
	CHECK( valid2Counter == 5 );
	CHECK( valid3Counter == 5 );
	CHECK( valid4Counter == 5 );
}

