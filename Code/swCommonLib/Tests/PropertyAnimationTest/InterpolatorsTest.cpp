#include "swCommonLib/External/Catch/catch.hpp"

#include "swCommonLib/ParameterAnimation/Animation/Animation.h"
#include "swCommonLib/ParameterAnimation/Interpolators/CosinusInterpolator.h"

#include "swCommonLib/ParameterAnimation/Parameters/DirectPropertyh.h"
#include "swCommonLib/ParameterAnimation/Parameters/PropertyPath.h"
#include "swCommonLib/ParameterAnimation/Parameters/StringPropertyPath.h"

#include "TestClasses.h"

#include <string>


RTTR_REGISTRATION
{

	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT2 )
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT3 )
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( DirectX::XMFLOAT4 )

	rttr::registration::class_< DirectX::XMFLOAT4 >( "DirectX::XMFLOAT4" )
		.property( "X", &DirectX::XMFLOAT4::x )
		.property( "Y", &DirectX::XMFLOAT4::y )
		.property( "Z", &DirectX::XMFLOAT4::z )
		.property( "W", &DirectX::XMFLOAT4::w );

	rttr::registration::class_< DirectX::XMFLOAT3 >( "DirectX::XMFLOAT3" )
		.property( "X", &DirectX::XMFLOAT3::x )
		.property( "Y", &DirectX::XMFLOAT3::y )
		.property( "Z", &DirectX::XMFLOAT3::z );

	rttr::registration::class_< DirectX::XMFLOAT2 >( "DirectX::XMFLOAT2" )
		.property( "X", &DirectX::XMFLOAT2::x )
		.property( "Y", &DirectX::XMFLOAT2::y );



	rttr::registration::enumeration< Methods >( "Methods" )
	(
		rttr::value( "Discrete", Methods::Discrete ),
		rttr::value( "Integral", Methods::Integral ),
		rttr::value( "Linear", Methods::Linear )
	);

	rttr::registration::class_< TestInterpolators >( "TestAnim" )
		.property( "DoubleField", &TestInterpolators::DoubleField )
		.property( "FloatField", &TestInterpolators::FloatField )
		.property( "UIntField64", &TestInterpolators::UIntField64 )
		.property( "IntField64", &TestInterpolators::IntField64 )
		.property( "UIntField32", &TestInterpolators::UIntField32 )
		.property( "IntField32", &TestInterpolators::IntField32 )
		.property( "UIntField16", &TestInterpolators::UIntField16 )
		.property( "IntField16", &TestInterpolators::IntField16 )
		.property( "UIntField8", &TestInterpolators::UIntField8 )
		.property( "IntField8", &TestInterpolators::IntField8 )
		.property( "CharField", &TestInterpolators::CharField )
		.property( "BoolField", &TestInterpolators::BoolField )
		.property( "StringField", &TestInterpolators::StringField )
		.property( "WStringField", &TestInterpolators::WStringField )
		.property( "EnumField", &TestInterpolators::EnumField )
		.property( "Color4F", &TestInterpolators::Color4F )
		.property( "Position3F", &TestInterpolators::Position3F ) BIND_AS_PTR;
}

#include "swCommonLib/ParameterAnimation/Interpolators/DefaultInterpolators.inl"


namespace
{

// ================================ //
//
bool				operator==	( const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2 )
{
	if( vec1.x != vec2.x )
		return false;
	if( vec1.y != vec2.y )
		return false;
	if( vec1.z != vec2.z )
		return false;
	return true;
}

// ================================ //
//
bool				operator==	( const DirectX::XMFLOAT4& vec1, const DirectX::XMFLOAT4& vec2 )
{
	if( vec1.x != vec2.x )
		return false;
	if( vec1.y != vec2.y )
		return false;
	if( vec1.z != vec2.z )
		return false;
	if( vec1.w != vec2.w )
		return false;
	return true;
}

// ================================ //
//
DirectX::XMFLOAT4	operator*	( TimeType scalar, DirectX::XMFLOAT4 vec2 )
{
	vec2.x = float( scalar * vec2.x );
	vec2.y = float( scalar * vec2.y );
	vec2.z = float( scalar * vec2.z );
	vec2.w = float( scalar * vec2.w );

	return vec2;
}

// ================================ //
//
DirectX::XMFLOAT4	operator+	( const DirectX::XMFLOAT4& vec1, const DirectX::XMFLOAT4& vec2 )
{
	DirectX::XMFLOAT4 result;
	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	result.z = vec1.z + vec2.z;
	result.w = vec1.w + vec2.w;

	return result;
}


}

ENABLE_PARAMETER_ANIMATION( DirectX::XMFLOAT4 );


// ================================ //
//
template< typename PropType >
void			TestProperty		( TestInterpolators& animClass, const std::string& propertyPath )
{
	SECTION( propertyPath.c_str() )
	{
		Ptr< AnimationImpl< PropType, PropertyPath< PropType > > > animator = MakePtr< AnimationImpl< PropType, PropertyPath< PropType > > >( &animClass, propertyPath );

		CHECK( animator->GetKey( 0.0 )->Value == 4 );

		// Add new keys.
		CHECK( animator->AddKey( 2.0, 8 ) );
		CHECK( animator->AddKey( 3.0, 5 ) );
		CHECK( animator->AddKey( 4.0, 1 ) );
		CHECK( animator->AddKey( 5.0, 0 ) );

		// Check new added keys.
		CHECK( animator->GetKey( 0.0 )->Value == 4 );
		CHECK( animator->GetKey( 2.0 )->Value == 8 );
		CHECK( animator->GetKey( 3.0 )->Value == 5 );
		CHECK( animator->GetKey( 4.0 )->Value == 1 );
		CHECK( animator->GetKey( 5.0 )->Value == 0 );


		//
		animator->Animate( &animClass, 0.0f );
		CHECK( animClass.GetField< PropType >() == 4 );

		animator->Animate( &animClass, 0.6f );
		CHECK( animClass.GetField< PropType >() == 5 );

		animator->Animate( &animClass, 1.01f );
		CHECK( animClass.GetField< PropType >() == 6 );

		animator->Animate( &animClass, 1.53f );
		CHECK( animClass.GetField< PropType >() == 7 );

		animator->Animate( &animClass, 2.0f );
		CHECK( animClass.GetField< PropType >() == 8 );

		animator->Animate( &animClass, 2.5f );
		CHECK( animClass.GetField< PropType >() == 6 );

		animator->Animate( &animClass, 3.0f );
		CHECK( animClass.GetField< PropType >() == 5 );

		animator->Animate( &animClass, 3.5f );
		CHECK( animClass.GetField< PropType >() == 3 );

		animator->Animate( &animClass, 4.0f );
		CHECK( animClass.GetField< PropType >() == 1 );

		animator->Animate( &animClass, 5.0f );
		CHECK( animClass.GetField< PropType >() == 0 );

		animator->Animate( &animClass, 6.0f );
		CHECK( animClass.GetField< PropType >() == 0 );
	}

}


template< typename PropType, Size ListSize >
void			TestPropertyDiscrete	( TestInterpolators& animClass, const std::string& propertyPath, PropType (&valuesList)[ ListSize ], TimeType (&timeList)[ ListSize ] )
{
	SECTION( propertyPath.c_str() )
	{
		Ptr< AnimationImpl< PropType, PropertyPath< PropType > > > animator = MakePtr< AnimationImpl< PropType, PropertyPath< PropType > > >( &animClass, propertyPath );

		Size init = 0;
		if( timeList[ init ] == 0.0f )
		{
			// Should be false !!
			CHECK( !animator->AddKey( timeList[ init ], valuesList[ init ] ) );
			init = 1;
		}

		for( ; init < ListSize; ++init )
		{
			INFO( "Adding keys, key time : " << timeList[ init ] );
			CHECK( animator->AddKey( timeList[ init ], valuesList[ init ] ) );
		}

		REQUIRE( animator->GetKeySet().Keys.size() == ListSize );
		REQUIRE( animator->GetKeySet().Interpolators.size() == ListSize - 1 );

		for( Size i = 0; i < ListSize; ++i )
		{
			INFO( "Checking keys, key time: " << timeList[ i ] );
			CHECK( animator->GetKey( timeList[ i ] )->Value == valuesList[ i ] );
		}

		int divisions = 10;

		for( Size i = 0; i < ListSize - 1; ++i )
		{
			TimeType timeInterval = timeList[ i + 1 ] - timeList[ i ];
								
			for( Size j = 0; j < divisions; ++ j )
			{
				TimeType curTime = timeList[ i ] + j * timeInterval / divisions;
				INFO( "Checking animation, time: " << curTime );

				animator->Animate( &animClass, curTime );
				CHECK( animClass.GetField< PropType >() == valuesList[ i ] );
			}
		}

		// Check what happens after last key
		TimeType timeInterval = 104.1f;
		for( int i = 0; i < 20; ++i )
		{
			TimeType curTime = timeList[ ListSize - 1 ] + i * timeInterval;

			INFO( "Checking animation, time: " << curTime );

			animator->Animate( &animClass, curTime );
			CHECK( animClass.GetField< PropType >() == valuesList[ ListSize - 1 ] );
		}

	}
}



TEST_CASE( "Interpolators", "[Interpolators Types]" )
{
	TestInterpolators		AnimClass;

	TestProperty< uint32 >( AnimClass, "UIntField32" );
	TestProperty< int32 >( AnimClass, "IntField32" );
	TestProperty< uint64 >( AnimClass, "UIntField64" );
	TestProperty< int64 >( AnimClass, "IntField64" );
	TestProperty< uint16 >( AnimClass, "UIntField16" );
	TestProperty< int16 >( AnimClass, "IntField16" );
	TestProperty< uint8 >( AnimClass, "UIntField8" );
	TestProperty< int8 >( AnimClass, "IntField8" );

	{
		std::string valuesArray[] = { "Value 1", "Something important", "Something not important", "Interesting message", "Not interesting message" };
		TimeType timeArray[] ={ 0.0f, 3.0f, 5.0f, 6.1f, 11.0f };

		TestPropertyDiscrete( AnimClass, "StringField", valuesArray, timeArray );
	}

	{
		std::wstring valuesArray[] = { L"Value 1", L"Something important", L"Something not important", L"Interesting message", L"Not interesting message" };
		TimeType timeArray[] ={ 0.0f, 3.0f, 5.0f, 6.1f, 11.0f };

		TestPropertyDiscrete( AnimClass, "WStringField", valuesArray, timeArray );
	}

	{
		bool valuesArray[] = { true, false, true, false, true };
		TimeType timeArray[] ={ 0.0f, 3.0f, 5.0f, 6.1f, 11.0f };

		TestPropertyDiscrete( AnimClass, "BoolField", valuesArray, timeArray );
	}

	{
		Methods valuesArray[] = { Methods::Discrete, Methods::Linear, Methods::Integral, Methods::Discrete, Methods::Integral };
		TimeType timeArray[] ={ 0.0f, 3.0f, 5.0f, 6.1f, 11.0f };

		TestPropertyDiscrete( AnimClass, "EnumField", valuesArray, timeArray );
	}

	{
		DirectX::XMFLOAT4 valuesArray[] =
		{
			DirectX::XMFLOAT4( 1.0f, 0.2f, 0.0f, 1.0f ),
			DirectX::XMFLOAT4( 0.2f, 0.6f, 0.0f, 1.0f ),
			DirectX::XMFLOAT4( 1.0f, 0.2f, 5.0f, 1.0f ),
			DirectX::XMFLOAT4( 0.0f, 0.2f, 3.0f, 1.0f ), 
			DirectX::XMFLOAT4( 1.0f, 0.9f, 3.0f, 1.0f )
		};
		TimeType timeArray[] ={ 0.0f, 3.0f, 5.0f, 6.1f, 11.0f };

		//TestPropertyDiscrete( AnimClass, "Color4F", valuesArray, timeArray );
	}
}


TEST_CASE( "Interpolators change" )
{
	TestInterpolators		AnimClass;

	Ptr< AnimationImpl< float, PropertyPath< float > > > animator = MakePtr< AnimationImpl< float, PropertyPath< float > > >( &AnimClass, "FloatField" );

	// Add new keys.
	CHECK( animator->AddKey( 2.0, 8 ) );
	CHECK( animator->AddKey( 3.0, 5 ) );
	CHECK( animator->AddKey( 4.0, 1 ) );
	CHECK( animator->AddKey( 5.0, 0, InterpolatorType::Cosinus ) );

	// Check new added keys.
	CHECK( animator->GetKey( 0.0 )->Value == 2 );
	CHECK( animator->GetKey( 2.0 )->Value == 8 );
	CHECK( animator->GetKey( 3.0 )->Value == 5 );
	CHECK( animator->GetKey( 4.0 )->Value == 1 );
	CHECK( animator->GetKey( 5.0 )->Value == 0 );

	// Change interpolators to cosinus.
	CHECK( animator->ChangeInterpolator( 0, MakeUPtr< CosinusInterpolator< float > >() ) );
	CHECK( animator->ChangeInterpolator( 1, InterpolatorType::Cosinus ) );
	CHECK( animator->ChangeInterpolator( 2, MakeUPtr< CosinusInterpolator< float > >() ) );

	// Check interpolators existance
	for( int i = 0; i < 4; ++i )
	{
		IInterpolator< float >* interpolatorPtr = animator->GetInterpolator( 0 );
		REQUIRE( interpolatorPtr != nullptr );
		CHECK( dynamic_cast< CosinusInterpolator< float >* >( interpolatorPtr ) != nullptr );
	}


}
