#include "swCommonLib/External/Catch/catch.hpp"

#include "swCommonLib/ParameterAnimation/Animation/Animation.h"

#include "swCommonLib/ParameterAnimation/Parameters/DirectPropertyh.h"
#include "swCommonLib/ParameterAnimation/Parameters/PropertyPath.h"
#include "swCommonLib/ParameterAnimation/Parameters/StringPropertyPath.h"

#include "TestClasses.h"

#include <string>



RTTR_REGISTRATION
{
	rttr::registration::class_< Nested2 >( "Nested2" )
		.property( "SomeValue", &Nested2::SomeValue )
		.property( "IntValue", &Nested2::IntValue )
		.property( "Name", &Nested2::Name );
	
	rttr::registration::class_< Nested >( "Nested" )
		.property( "SomeValue", &Nested::SomeValue )
		.property( "IntValue", &Nested::IntValue )
		.property( "Name", &Nested::Name )
		.property( "Additional", &Nested::Additional ) BIND_AS_PTR;

	rttr::registration::class_< TestAnim >( "TestAnim" )
		.property( "Energy", &TestAnim::m_energy )
		.property( "Mass", &TestAnim::m_mass )
		.property( "Power", &TestAnim::m_power )
		.property( "Shit", &TestAnim::m_shit )
		.property( "OtherData", &TestAnim::m_otherData ) BIND_AS_PTR;
}



TEST_CASE( "Animations", "[AnimationClassTest]" )
{
	TestAnim		AnimClass;

	std::string someValueNestedPath = "OtherData/Additional/SomeValue";
	AnimClass.FloatAnimString = MakePtr< FloatAnimation< StringPropertyPath > >( &AnimClass, someValueNestedPath );

	// Testing FloatAnimation< StringPropertyPath >
	// =========================================================== //

	// First key is added automatically.
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Time == 0.0 );
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Value == 7.0f );

// Add new keys.
	CHECK( AnimClass.FloatAnimString->AddKey( 2.0, 3.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 3.0, 0.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 4.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 5.0, 0.0f ) );

	// Check new added keys.
	CHECK( AnimClass.FloatAnimString->GetKey( 0.0 )->Value == 7.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 2.0 )->Value == 3.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.0 )->Value == 0.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 5.0 )->Value == 0.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == AnimClass.FloatAnimString->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Interpolators.size() == 4 );

// Add key in existing keyframe.
	CHECK( !AnimClass.FloatAnimString->AddKey( 4.0, 2.0f ) );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.0 )->Value == 2.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == AnimClass.FloatAnimString->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Interpolators.size() == 4 );

// Add key between existing keys.
	CHECK( AnimClass.FloatAnimString->AddKey( 1.5, 6.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 3.5, 2.0f ) );
	CHECK( AnimClass.FloatAnimString->AddKey( 4.5, -1.0f ) );

	CHECK( AnimClass.FloatAnimString->GetKey( 1.5 )->Value == 6.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.5 )->Value == 2.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.5 )->Value == -1.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == AnimClass.FloatAnimString->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == 8 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Interpolators.size() == 7 );


// Remove keys
	CHECK( AnimClass.FloatAnimString->RemoveKey( 1.5 ) );
	CHECK( AnimClass.FloatAnimString->RemoveKey( 3.5 ) );
	CHECK( AnimClass.FloatAnimString->RemoveKey( 4.5 ) );

	CHECK( AnimClass.FloatAnimString->GetKey( 1.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.5 ) == nullptr );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == AnimClass.FloatAnimString->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Interpolators.size() == 4 );

// Update keys
	CHECK( AnimClass.FloatAnimString->UpdateKey( 2.0, 4.0f ) );
	CHECK( AnimClass.FloatAnimString->UpdateKey( 3.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimString->UpdateKey( 4.0, -3.0f ) );
	CHECK( AnimClass.FloatAnimString->UpdateKey( 5.0, 2.0f ) );

	CHECK( AnimClass.FloatAnimString->GetKey( 2.0 )->Value == 4.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 3.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 4.0 )->Value == -3.0f );
	CHECK( AnimClass.FloatAnimString->GetKey( 5.0 )->Value == 2.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == AnimClass.FloatAnimString->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimString->GetKeySet().Interpolators.size() == 4 );

	// Testing FloatAnimation< PropertyPath >
	// =========================================================== //
	std::string someValuePath = "OtherData/SomeValue";
	AnimClass.FloatAnimProperty = MakePtr< FloatAnimation< PropertyPath > >( &AnimClass, someValuePath );

	CHECK( AnimClass.FloatAnimProperty->GetKey( 0.0 )->Time == 0.0 );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 0.0 )->Value == 5.0f );

// Add new keys.
	CHECK( AnimClass.FloatAnimProperty->AddKey( 2.0, 3.0f ) );
	CHECK( AnimClass.FloatAnimProperty->AddKey( 3.0, 0.0f ) );
	CHECK( AnimClass.FloatAnimProperty->AddKey( 4.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimProperty->AddKey( 5.0, 0.0f ) );

	// Check new added keys.
	CHECK( AnimClass.FloatAnimProperty->GetKey( 0.0 )->Value == 5.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 2.0 )->Value == 3.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 3.0 )->Value == 0.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 4.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 5.0 )->Value == 0.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() == 4 );

// Add key in existing keyframe.
	CHECK( !AnimClass.FloatAnimProperty->AddKey( 4.0, 2.0f ) );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 4.0 )->Value == 2.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() == 4 );

// Add key between existing keys.
	CHECK( AnimClass.FloatAnimProperty->AddKey( 1.5, 6.0f ) );
	CHECK( AnimClass.FloatAnimProperty->AddKey( 3.5, 2.0f ) );
	CHECK( AnimClass.FloatAnimProperty->AddKey( 4.5, -1.0f ) );

	CHECK( AnimClass.FloatAnimProperty->GetKey( 1.5 )->Value == 6.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 3.5 )->Value == 2.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 4.5 )->Value == -1.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == 8 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() == 7 );

// Remove keys
	CHECK( AnimClass.FloatAnimProperty->RemoveKey( 1.5 ) );
	CHECK( AnimClass.FloatAnimProperty->RemoveKey( 3.5 ) );
	CHECK( AnimClass.FloatAnimProperty->RemoveKey( 4.5 ) );

	CHECK( AnimClass.FloatAnimProperty->GetKey( 1.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 3.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 4.5 ) == nullptr );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() == 4 );

// Update keys
	CHECK( AnimClass.FloatAnimProperty->UpdateKey( 2.0, 4.0f ) );
	CHECK( AnimClass.FloatAnimProperty->UpdateKey( 3.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimProperty->UpdateKey( 4.0, -3.0f ) );
	CHECK( AnimClass.FloatAnimProperty->UpdateKey( 5.0, 2.0f ) );

	CHECK( AnimClass.FloatAnimProperty->GetKey( 2.0 )->Value == 4.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 3.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 4.0 )->Value == -3.0f );
	CHECK( AnimClass.FloatAnimProperty->GetKey( 5.0 )->Value == 2.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimProperty->GetKeySet().Interpolators.size() == 4 );


	// Testing FloatAnimation< DirectProperty >
	// =========================================================== //
	std::string energyPropName = "Energy";
	AnimClass.FloatAnimDirect = MakePtr< FloatAnimation< DirectProperty > >( &AnimClass, energyPropName );

	CHECK( AnimClass.FloatAnimDirect->GetKey( 0.0 )->Time == 0.0 );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 0.0 )->Value == 1.0f );

// Add new keys.
	CHECK( AnimClass.FloatAnimDirect->AddKey( 2.0, 3.0f ) );
	CHECK( AnimClass.FloatAnimDirect->AddKey( 3.0, 0.0f ) );
	CHECK( AnimClass.FloatAnimDirect->AddKey( 4.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimDirect->AddKey( 5.0, 0.0f ) );

	// Check new added keys.
	CHECK( AnimClass.FloatAnimDirect->GetKey( 0.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 2.0 )->Value == 3.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 3.0 )->Value == 0.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 4.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 5.0 )->Value == 0.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() == 4 );

// Add key in existing keyframe.
	CHECK( !AnimClass.FloatAnimDirect->AddKey( 4.0, 2.0f ) );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 4.0 )->Value == 2.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() == 4 );

// Add key between existing keys.
	CHECK( AnimClass.FloatAnimDirect->AddKey( 1.5, 6.0f ) );
	CHECK( AnimClass.FloatAnimDirect->AddKey( 3.5, 2.0f ) );
	CHECK( AnimClass.FloatAnimDirect->AddKey( 4.5, -1.0f ) );

	CHECK( AnimClass.FloatAnimDirect->GetKey( 1.5 )->Value == 6.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 3.5 )->Value == 2.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 4.5 )->Value == -1.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == 8 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() == 7 );

// Remove keys
	CHECK( AnimClass.FloatAnimDirect->RemoveKey( 1.5 ) );
	CHECK( AnimClass.FloatAnimDirect->RemoveKey( 3.5 ) );
	CHECK( AnimClass.FloatAnimDirect->RemoveKey( 4.5 ) );

	CHECK( AnimClass.FloatAnimDirect->GetKey( 1.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 3.5 ) == nullptr );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 4.5 ) == nullptr );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() == 4 );

// Update keys
	CHECK( AnimClass.FloatAnimDirect->UpdateKey( 2.0, 4.0f ) );
	CHECK( AnimClass.FloatAnimDirect->UpdateKey( 3.0, 1.0f ) );
	CHECK( AnimClass.FloatAnimDirect->UpdateKey( 4.0, -3.0f ) );
	CHECK( AnimClass.FloatAnimDirect->UpdateKey( 5.0, 2.0f ) );

	CHECK( AnimClass.FloatAnimDirect->GetKey( 2.0 )->Value == 4.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 3.0 )->Value == 1.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 4.0 )->Value == -3.0f );
	CHECK( AnimClass.FloatAnimDirect->GetKey( 5.0 )->Value == 2.0f );

	// Check interpolators.
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() + 1 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Keys.size() == 5 );
	CHECK( AnimClass.FloatAnimDirect->GetKeySet().Interpolators.size() == 4 );


// Animation
	float epsilon = 0.00001f;
	
	float time = 0.0f;
	float value = 7.0f;

	//
	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - 7.0f ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - 5.0f ) < epsilon );
	CHECK( abs( AnimClass.m_energy - 1.0f ) < epsilon );


	//
	time = 0.2f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - 6.7f ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - 4.9f ) < epsilon );
	CHECK( abs( AnimClass.m_energy - 1.3f ) < epsilon );


	//
	time = 0.7f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - 5.95f ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - 4.65 ) < epsilon );
	CHECK( abs( AnimClass.m_energy - 2.05f ) < epsilon );

	//
	time = 1.4f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - 4.9 ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - 4.3f ) < epsilon );
	CHECK( abs( AnimClass.m_energy - 3.1f ) < epsilon );

	//
	time = 2.0f;
	value = 4.0f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 2.2f;
	value = 3.4f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 2.3f;
	value = 3.1f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 2.8f;
	value = 1.6f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 3.1f;
	value = 0.6f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 3.3f;
	value = -0.2f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 3.5f;
	value = -1.0f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 3.9f;
	value = -2.6f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 4.05f;
	value = -2.75f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 4.17f;
	value = -2.15f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 4.73f;
	value = 0.65f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 5.0f;
	value = 2.0f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 7.0f;
	value = 2.0f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = 12222222.0f;
	value = 2.0f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - value ) < epsilon );
	CHECK( abs( AnimClass.m_energy - value ) < epsilon );

	//
	time = -1.0f;

	AnimClass.FloatAnimString->Animate( &AnimClass, time );
	AnimClass.FloatAnimProperty->Animate( &AnimClass, time );
	AnimClass.FloatAnimDirect->Animate( &AnimClass, time );

	CHECK( abs( AnimClass.m_otherData.Additional.SomeValue - 7.0 ) < epsilon );
	CHECK( abs( AnimClass.m_otherData.SomeValue - 5.0 ) < epsilon );
	CHECK( abs( AnimClass.m_energy - 1.0 ) < epsilon );

}



