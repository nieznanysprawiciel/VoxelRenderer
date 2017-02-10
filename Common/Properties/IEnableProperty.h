#pragma once

#include "RTTR.h"
#include "swCommonLib/Common/Properties/Property.h"


class IEnableProperty
{
	RTTR_ENABLE()
private:
public:
	
	//inline IProperty			GetProperty		( const char* propertyName )
	//{
	//	const IMetaProperty* metaProperty = GetMetaProperty( propertyName );
	//	return IProperty( metaProperty, this );
	//}

	template< typename PropertyType >
	inline Property< PropertyType >		GetTypedProperty		( const char* propertyName )
	{
		auto metaProperty = rttr::type::get( *this ).get_property( propertyName );

		if( metaProperty.is_valid() && metaProperty.get_type().is_derived_from< PropertyType >() )
			return Property< PropertyType >( metaProperty, this );

		// Types are inconsistent. Return invalid property.
		return Property< PropertyType >( this );
	}

	//inline std::vector< IProperty >		GetProperties			()
	//{
	//	auto typeInfo = getTypeInfo();
	//	auto propertyVec = typeInfo.GetProperties();

	//	std::vector< IProperty >	properties;

	//	for( auto metaProperty : propertyVec )
	//	{
	//		properties.push_back( IProperty( metaProperty, this ) );
	//	}

	//	return properties;
	//}
};
//RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS( IEnableProperty )
