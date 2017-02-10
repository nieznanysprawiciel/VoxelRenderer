#pragma once

#include "swCommonLib/Common/EngineObject.h"
#include "swCommonLib/Common/RTTR.h"

#include <vector>
#include <string>


class Properties
{
private:
public:

	static std::vector< rttr::property >					GetPropertyPath	( EngineObject* object, const std::string& propertyPath, char separator = '/' );
	static std::vector< rttr::property >					GetPropertyPath	( rttr::variant object, const std::string& propertyPath, Size offset = 0, char separator = '/' );

	static std::pair< rttr::variant, rttr::property >		GetProperty		( EngineObject* object, const std::string& propertyPath, char separator = '/' );
	static std::pair< rttr::variant, rttr::property >		GetProperty		( rttr::variant object, const std::string& propertyPath, Size offset = 0, char separator = '/' );
	static std::pair< rttr::variant, rttr::property >		GetProperty		( EngineObject* object, const std::vector< rttr::property >& propertyPath );
	
	template< typename Type >
	static Type						GetValue		( EngineObject* object, const std::vector< rttr::property >& propertyPath );

	static rttr::property			EmptyProperty	();
	static TypeID					GetRealType		( rttr::variant& object );
	static TypeID					GetRealType		( EngineObject* object );
};

//====================================================================================//
//			Implementation	
//====================================================================================//


template< typename Type >
inline Type			Properties::GetValue		( EngineObject* object, const std::vector< rttr::property >& propertyPath )
{
	rttr::variant propertyVal = object;

	for( auto& property : propertyPath )
	{
		propertyVal = property.get_value( propertyVal );
	}

	return propertyVal.get_value< Type >();
}
