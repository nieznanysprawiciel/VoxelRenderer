#include "swCommonLib/External/Catch/catch.hpp"


#include "swCommonLib/Common/Properties/Properties.h"

#include "TestClasses.h"




TEST_CASE( "Property helpers" )
{
	TestAnim		AnimClass;

	SECTION( "Properties::GetProperty" )
	{
		std::string someValueNestedPath = "OtherData/Additional/SomeValue";

		auto address = Properties::GetProperty( &AnimClass, someValueNestedPath );

		auto object = address.first;
		auto property = address.second;

		CHECK( object.get_type() == TypeID::get< Nested2* >() );
		CHECK( property.get_type() == TypeID::get< float >() );
		CHECK( property.get_declaring_type() == TypeID::get< Nested2 >() );

		std::string someValuePath = "OtherData/SomeValue";

		address = Properties::GetProperty( &AnimClass, someValuePath );

		object = address.first;
		property = address.second;

		CHECK( object.get_type() == TypeID::get< Nested* >() );
		CHECK( property.get_type() == TypeID::get< float >() );
		CHECK( property.get_declaring_type() == TypeID::get< Nested >() );

		someValuePath = "Energy";

		address = Properties::GetProperty( &AnimClass, someValuePath );

		object = address.first;
		property = address.second;

		/// Note: Behavior changed from previous version.
		//CHECK( object.get_type() == TypeID::get< TestAnim* >() );
		CHECK( object.get_type() == TypeID::get< EngineObject* >() );
		CHECK( property.get_type() == TypeID::get< float >() );
		CHECK( property.get_declaring_type() == TypeID::get< TestAnim >() );

		someValuePath = "Fake";

		address = Properties::GetProperty( &AnimClass, someValuePath );

		object = address.first;
		property = address.second;

		CHECK( !object.get_type().is_valid() );
		CHECK( !property.get_type().is_valid() );
		CHECK( !property.get_declaring_type().is_valid() );
	}

	SECTION( "Properties::GetPropertyPath" )
	{
		std::string someValueNestedPath = "OtherData/Additional/SomeValue";

		auto path = Properties::GetPropertyPath( &AnimClass, someValueNestedPath );

		REQUIRE( path.size() == 3 );

		CHECK( path[ 0 ].get_type() == TypeID::get< Nested* >() );
		CHECK( path[ 1 ].get_type() == TypeID::get< Nested2* >() );
		CHECK( path[ 2 ].get_type() == TypeID::get< float >() );

		std::string someValuePath = "OtherData/SomeValue";

		path = Properties::GetPropertyPath( &AnimClass, someValuePath );

		REQUIRE( path.size() == 2 );

		CHECK( path[ 0 ].get_type() == TypeID::get< Nested* >() );
		CHECK( path[ 1 ].get_type() == TypeID::get< float >() );

		someValuePath = "Energy";
		path = Properties::GetPropertyPath( &AnimClass, someValuePath );

		REQUIRE( path.size() == 1 );

		CHECK( path[ 0 ].get_type() == TypeID::get< float >() );

		someValuePath = "Fake";
		path = Properties::GetPropertyPath( &AnimClass, someValuePath );

		REQUIRE( path.size() == 0 );
	}
}


