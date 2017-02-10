#pragma once
#include "Animation.h"


//====================================================================================//
//			Constructor	
//====================================================================================//

template< typename KeyType, typename AddressType >
inline		AnimationImpl< KeyType, AddressType >::AnimationImpl( EngineObject* object, const std::string& propertyPath )
	:	Evaluator( object, propertyPath )
{}



//====================================================================================//
//			Animation	 Evaluation
//====================================================================================//

// ================================ //
//
template< typename KeyType, typename AddressType >
inline void			AnimationImpl< KeyType, AddressType >::Animate		( EngineObject* object, TimelineBase* timeline )
{
	AnimationImpl< KeyType, AddressType >::Animate( object, timeline->GetTime() );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline void			AnimationImpl< KeyType, AddressType >::Animate		( EngineObject* object, TimeType time )
{
	Evaluator.Evaluate( object, time );
}


//====================================================================================//
//			Keys manipulation	
//====================================================================================//

// ================================ //
//
template< typename KeyType, typename AddressType  >
inline bool			AnimationImpl< KeyType, AddressType >::AddKey		( TimeType time, const KeyType& value )
{
	return Evaluator.AddKey( time, value );
}

// ================================ //
//
template< typename KeyType, typename AddressType  >
inline bool			AnimationImpl< KeyType, AddressType >::AddKey		( TimeType time, const KeyType& value, InterpolatorType interpolatorType )
{
	return Evaluator.AddKey( time, value, interpolatorType );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimationImpl< KeyType, AddressType >::UpdateKey	( TimeType time, const KeyType& newValue, UPtr< Interpolator >&& interpolator )
{
	return Evaluator.UpdateKey( time, newValue, std::move( interpolator ) );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimationImpl< KeyType, AddressType >::UpdateKey	( TimeType time, const KeyType& newValue )
{
	return Evaluator.UpdateKey( time, newValue );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimationImpl< KeyType, AddressType >::RemoveKey	( TimeType time )
{
	return Evaluator.RemoveKey( time );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimationImpl< KeyType, AddressType >::ChangeInterpolator	( Size idx, UPtr< Interpolator >&& interpolator )
{
	return Evaluator.ChangeInterpolator( idx, std::move( interpolator ) );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline bool			AnimationImpl< KeyType, AddressType >::ChangeInterpolator	( Size idx, InterpolatorType interpolatorType )
{
	return false;
}

// ================================ //
//
template<typename KeyType, typename AddressType >
inline IInterpolator< KeyType >*	AnimationImpl< KeyType, AddressType >::GetInterpolator	( Size idx )
{
	return nullptr;
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline const Key< KeyType >*		AnimationImpl< KeyType, AddressType >::GetKey	( TimeType time )
{
	return Evaluator.GetKey( time );
}

// ================================ //
//
template< typename KeyType, typename AddressType >
inline KeySet< KeyType >&			AnimationImpl< KeyType, AddressType >::GetKeySet()
{
	return Evaluator.GetKeySet();
}

