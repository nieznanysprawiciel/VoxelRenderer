#pragma once
/**
@file AnimEvaluator.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/EngineObject.h"

#include "swCommonLib/ParameterAnimation/KeySet.h"
#include "swCommonLib/ParameterAnimation/Timelines/TimelineBase.h"



/**@brief Evaluates animation.

Uses @ref m_keySet to interpolate animation value and sets it to @ref m_param.

Template parameter AddressType is class which is used to set parameter value. You can provide
your own classes or apply build in objects. This library implements parameters using rttr library.

@see DirectProperty, PropertyPath, StringPropertyPath.

@ingroup KeyFrameAnimation*/
template< typename KeyType, typename AddressType >
class AnimEvaluator
{
public:
	typedef IInterpolator< KeyType > Interpolator;
private:
	KeySet< KeyType >			m_keySet;
	AddressType					m_param;

protected:
public:
	/// Constructor for serialization only.
	explicit		AnimEvaluator	() = default;
	explicit		AnimEvaluator	( EngineObject* object, const std::string& propertyPath );
					~AnimEvaluator	() = default;


	void				Evaluate	( EngineObject* object, TimeType time );

	/**@brief Adds key and sets default interpolator.
	@return False if key already exists.*/
	bool	AddKey			( TimeType time, const KeyType& value );
	/**@brief Adds key and sets given interpolator.
	@return False if key already exists.*/
	bool	AddKey			( TimeType time, const KeyType& value, InterpolatorType interpolatorType );
	/**@brief Udates key given by time. Sets new interpolator.
	@return False if There's no key in given time.*/
	bool	UpdateKey		( TimeType time, const KeyType& newValue, UPtr< Interpolator >&& interpolator );
	/**@brief Updates key. Leaves interpolator unchanged.
	@return False if There's no key in given time.*/
	bool	UpdateKey		( TimeType time, const KeyType& newValue );
	/**@brief Removes key in given time.
	@return False if There's no key in given time.*/
	bool	RemoveKey		( TimeType time );

	/**@brief Updates interpolator. Key remains unchanged.
	@return False if There's no key in given time.*/
	bool	ChangeInterpolator	( Size idx, UPtr< Interpolator >&& interpolator );

	/**@brief Return key in given time.*/
	const Key< KeyType >*		GetKey		( TimeType time );

	/**@brief Returns KeySet.*/
	virtual KeySet< KeyType >&	GetKeySet	();
};



//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
template< typename KeyType, typename AddressType >
inline				AnimEvaluator< KeyType, AddressType >::AnimEvaluator( EngineObject* object, const std::string& propertyPath )
	:	m_param( object, propertyPath )
{
	//static_assert( std::is_member_function_pointer< &AddressType::GetValue >::value, "Template parameter AddressType must implement GetValue function." );
	//static_assert( std::is_member_function_pointer< &AddressType::SetValue >::value, "Template parameter AddressType must implement SetValue function." );

	KeyType curValue = m_param.GetValue( object );
	m_keySet.UpdateKey( TimeType( 0.0 ), curValue );
}


// ================================ //
//
template< typename KeyType, typename AddressType >
inline void			AnimEvaluator< KeyType, AddressType >::Evaluate		( EngineObject* object, TimeType time )
{
	KeyType value = m_keySet.Evaluate( time );
	m_param.SetValue( object, value );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimEvaluator< KeyType, AddressType >::AddKey		( TimeType time, const KeyType& value )
{
	return m_keySet.AddKey( time, value );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimEvaluator< KeyType, AddressType >::AddKey		( TimeType time, const KeyType & value, InterpolatorType interpolatorType )
{
	return false;
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimEvaluator< KeyType, AddressType >::UpdateKey	( TimeType time, const KeyType & newValue, UPtr< Interpolator >&& interpolator )
{
	return false;
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimEvaluator< KeyType, AddressType >::UpdateKey	( TimeType time, const KeyType & newValue )
{
	return m_keySet.UpdateKey( time, newValue );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimEvaluator< KeyType, AddressType >::RemoveKey	( TimeType time )
{
	return m_keySet.RemoveKey( time );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimEvaluator< KeyType, AddressType >::ChangeInterpolator	( Size idx, UPtr< Interpolator >&& interpolator )
{
	return m_keySet.ChangeInterpolator( idx, std::move( interpolator ) );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline const Key< KeyType >*			AnimEvaluator< KeyType, AddressType >::GetKey( TimeType time )
{
	return m_keySet.GetKey( time );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline KeySet< KeyType >&				AnimEvaluator< KeyType, AddressType >::GetKeySet()
{
	return m_keySet;
}
