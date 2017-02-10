#include "Properties.h"


//====================================================================================//
//			Helpers	
//====================================================================================//

rttr::property			GetPropertyInternal	( TypeID ownerType, Size startOffset, Size endOffset, const std::string& propertyPath )
{
	std::string propName = std::string( propertyPath.begin() + startOffset, propertyPath.begin() + endOffset );
	return ownerType.get_property( propName.c_str() );
}



//====================================================================================//
//			Implementation	
//====================================================================================//



// ================================ //
//
std::vector< rttr::property >					Properties::GetPropertyPath	( EngineObject* object, const std::string& propertyPath, char separator )
{
	return GetPropertyPath( object, propertyPath, 0, separator );
}

// ================================ //
//
std::vector< rttr::property >					Properties::GetPropertyPath	( rttr::variant object, const std::string& propertyPath, Size offset, char separator )
{
	TypeID ownerType = GetRealType( object );

	std::vector< rttr::property > result;

	Size propertyEnd = propertyPath.find_first_of( separator, offset );
	while( propertyEnd != std::string::npos )
	{
		rttr::property resultProperty = GetPropertyInternal( ownerType, offset, propertyEnd, propertyPath );
		
		if( !resultProperty.is_valid() )
			return std::vector< rttr::property >();

		result.push_back( resultProperty );

		offset = propertyEnd + 1;
		propertyEnd = propertyPath.find_first_of( separator, offset );

		object = resultProperty.get_value( object );
		ownerType = GetRealType( object );
	}

	rttr::property resultProperty = GetPropertyInternal( ownerType, offset, std::distance( propertyPath.begin(), propertyPath.end() ), propertyPath );
	
	if( !resultProperty.is_valid() )
		return std::vector< rttr::property >();

	result.push_back( resultProperty );

	return result;
}

// ================================ //
//
std::pair< rttr::variant, rttr::property >		Properties::GetProperty		( EngineObject* object, const std::string& propertyPath, char separator )
{
	return GetProperty( rttr::variant( object ), propertyPath, 0, separator );
}

// ================================ //
//
std::pair< rttr::variant, rttr::property >		Properties::GetProperty		( rttr::variant object, const std::string& propertyPath, Size offset, char separator )
{
	TypeID realType = GetRealType( object );

	Size propertyEnd = propertyPath.find_first_of( separator, offset );
	if( propertyEnd != std::string::npos )
	{
		rttr::property resultProperty = GetPropertyInternal( realType, offset, propertyEnd, propertyPath );

		auto childObject = resultProperty.get_value( object );
		return GetProperty( childObject, propertyPath, propertyEnd + 1, separator );
	}
	else
	{
		rttr::property resultProperty = GetPropertyInternal( realType, offset, std::distance( propertyPath.begin(), propertyPath.end() ), propertyPath );

		if( !resultProperty.is_valid() )
			return std::make_pair( rttr::variant(), resultProperty );

		object.convert( static_cast< rttr::type >( realType ) );	// Note: This static cast is hack to prevent compiler from complaining about two functions matching arguments.
		return std::make_pair( object, resultProperty );
	}
}

// ================================ //
//
std::pair< rttr::variant, rttr::property >		Properties::GetProperty		( EngineObject* object, const std::vector< rttr::property >& propertyPath )
{
	rttr::variant owner = object;

	for( int i = 0; i < propertyPath.size() - 1; ++i )
	{
		rttr::property property = propertyPath[ i ];
		owner = property.get_value( owner );
	}

	return std::make_pair( owner, propertyPath.back() );
}

// ================================ //
//
rttr::property			Properties::EmptyProperty()
{
	TypeID nullType = TypeID::get< void >();
	auto prop = nullType.get_property( "" );

	return prop;
}

/**@brief Gets real type of object.
If class inherited EngineObject, we can check it's real type.*/
TypeID					Properties::GetRealType		( rttr::variant& object )
{
	rttr::instance realObject = object;
	return realObject.get_derived_type();
}

/**@brief Gets real type of object.
If class inherited EngineObject, we can check it's real type.*/
TypeID					Properties::GetRealType		( EngineObject* object )
{
	return object->GetType();
}
