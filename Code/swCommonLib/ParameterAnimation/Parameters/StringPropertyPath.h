#pragma once
/**
@file StringPropertyPath.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Serialization/PropertySerialization/Serialization.h"


/**@brief */
template< typename KeyType >
class StringPropertyPath
{
private:
	std::string		m_path;

protected:
public:
	explicit		StringPropertyPath	();
	explicit		StringPropertyPath	( EngineObject* object, const std::string& propertyPath );
					~StringPropertyPath	() = default;


	KeyType			GetValue		( EngineObject* object );
	void			SetValue		( EngineObject* object, KeyType& value );
};


//====================================================================================//
//			Implementation	
//====================================================================================//


// ================================ //
//
template< typename KeyType >
inline		StringPropertyPath< KeyType >::StringPropertyPath()
	:	m_path( "" )
{}

// ================================ //
//
template< typename KeyType >
inline		StringPropertyPath<	KeyType >::StringPropertyPath( EngineObject* object, const std::string& propertyPath )
	:	m_path( propertyPath )
{}

// ================================ //
//
template< typename KeyType >
inline KeyType		StringPropertyPath< KeyType >::GetValue		( EngineObject* object )
{
	auto finalPair = Properties::GetProperty( object, m_path );
	auto& owner = finalPair.first;
	auto& prop = finalPair.second;

	return Serialization::GetPropertyValue< KeyType >( prop, owner );
}

template< typename KeyType >
inline void			StringPropertyPath< KeyType >::SetValue		( EngineObject* object, KeyType& value )
{
	auto finalPair = Properties::GetProperty( object, m_path );
	auto& owner = finalPair.first;
	auto& prop = finalPair.second;

	Serialization::SetPropertyValue( prop, owner, value );
}
