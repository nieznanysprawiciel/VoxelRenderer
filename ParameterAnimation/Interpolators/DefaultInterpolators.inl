#pragma once
/**
@file DefaultInterpolators.inl
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "DefaultInterpolators.h"

#include "LinearInterpolator.h"
#include "DiscreteInterpolator.h"
#include "CosinusInterpolator.h"


// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< KeyType > >			DefaultInterpolators::CreateLinear	( const Key< KeyType >& leftKey,
																					  const Key<KeyType>& rightKey,
																					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																					  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return UPtr< IInterpolator< KeyType > >( new LinearInterpolator< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator ) );
}


// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< KeyType > >			DefaultInterpolators::CreateDiscrete( const Key< KeyType >& leftKey,
																					  const Key<KeyType>& rightKey,
																					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																					  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return UPtr< IInterpolator< KeyType > >( new DiscreteInterpolator< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator ) );
}

// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< KeyType > >			DefaultInterpolators::CreateCosinus	( const Key< KeyType >& leftKey,
																					  const Key<KeyType>& rightKey,
																					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																					  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return UPtr< IInterpolator< KeyType > >( new CosinusInterpolator< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator ) );
}




#define ENABLE_PARAMETER_ANIMATION( type )							\
namespace DefaultInterpolators										\
{																	\
template<>															\
struct is_param_animation_enabled< type >							\
{																	\
	const static bool value = true;									\
};																	\
}



