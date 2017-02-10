#pragma once

#include "swCommonLib/Common/RTTR.h"
#include <cassert>

class IEnableProperty;



class IProperty
{
protected:

	const rttr::property		m_metaInfo;
	IEnableProperty*			m_ownerObj;

public:

	IProperty( const rttr::property metaInfo, IEnableProperty* object )
		:	m_metaInfo( metaInfo )
		,	m_ownerObj( object )
	{}

	/**@brief Tworzy niepoprawny obiekt IProperty.*/
	IProperty( IEnableProperty* object )
		:	m_ownerObj( object )
	{}

	std::string		GetPropertyName()		{ return m_metaInfo.get_name(); }
	rttr::type		GetPropertyType()		{ return m_metaInfo.get_type(); }

	bool			IsValid()				{ return m_metaInfo.is_valid() && m_ownerObj; }


protected:

	template< typename ClassType, typename ParamType >
	ParamType ClassType::*	GetMemberPointer()
	{
		return static_cast< const rttr::detail::property_wrapper_member_base< ClassType, ParamType >* >( m_metaInfo.m_wrapper )->GetPtr();
	}
};



template< typename PropertyType >
class Property	:	public IProperty
{
private:
public:

	Property( const rttr::property metaInfo, IEnableProperty* object )
		:	IProperty( metaInfo, object )
	{
		//assert( IsValid() );
	}

	/**@brief Tworzy niepoprawny obiekt Property.*/
	Property( IEnableProperty* object )
		:	IProperty( object )
	{}

	inline PropertyType&		operator()( void )
	{
		assert( IsValid() );

		auto memberPtr = GetMemberPointer< IEnableProperty, PropertyType >();
		return m_ownerObj->*memberPtr;
	}

	inline						operator PropertyType&()
	{
		assert( IsValid() );

		auto memberPtr = GetMemberPointer< IEnableProperty, PropertyType >();
		return m_ownerObj->*memberPtr;
	}
};
