#pragma once
/**
@file DefaultInterpolators.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "IInterpolator.h"
#include "swCommonLib/ParameterAnimation/Key.h"

#include <string>


/**@brief Interpolators helpers functions.*/
namespace DefaultInterpolators
{

	/**@brief Trait for enabling types for parameter animation.
	
	Most of basic types can be animated without specializing this trait.
	For some structs user must provide this function and overload operator+ and operator* for scalar multiplication.
	
	You don't have to write this specialization manually. Use macro ENABLE_PARAMETER_ANIMATION instead.*/
	template< typename KeyType >
	struct is_param_animation_enabled
	{
		const static bool value = false;
	};



	template< typename KeyType >
	UPtr< IInterpolator< KeyType > >	CreateLinear	( const Key< KeyType >& leftKey,
														  const Key< KeyType >& rightKey,
														  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
														  UPtr< const IInterpolator< KeyType > >& rightInterpolator );

	template< typename KeyType >
	UPtr< IInterpolator< KeyType > >	CreateDiscrete	( const Key< KeyType >& leftKey,
														  const Key< KeyType >& rightKey,
														  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
														  UPtr< const IInterpolator< KeyType > >& rightInterpolator );

	template< typename KeyType >
	UPtr< IInterpolator< KeyType > >	CreateCosinus	( const Key< KeyType >& leftKey,
														  const Key< KeyType >& rightKey,
														  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
														  UPtr< const IInterpolator< KeyType > >& rightInterpolator );


	template< typename KeyType >
	UPtr< IInterpolator< KeyType > >	Create			( InterpolatorType type,
														  const Key< KeyType >& leftKey,
														  const Key< KeyType >& rightKey,
														  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
														  UPtr< const IInterpolator< KeyType > >& rightInterpolator );



	/**@brief Function for creating default interpolators for different types of keys.
	Specialize this template in .cpp file.
	
	@note Left or right interpolator can be nullptr, but keys always exist.*/
	template< typename KeyType >
	UPtr< IInterpolator< typename std::enable_if< std::is_floating_point< KeyType >::value, KeyType >::type > >
		Create		( const Key< KeyType >& leftKey,
					  const Key< KeyType >& rightKey,
					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					  UPtr< const IInterpolator< KeyType > >& rightInterpolator );

	template< typename KeyType >
	UPtr< IInterpolator< typename std::enable_if< std::is_integral< KeyType >::value, KeyType >::type > >
		Create		( const Key< KeyType >& leftKey,
					  const Key< KeyType >& rightKey,
					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					  UPtr< const IInterpolator< KeyType > >& rightInterpolator );

	template<>
	UPtr< IInterpolator< bool > >
		Create		( const Key< bool >& leftKey,
					  const Key< bool >& rightKey,
					  UPtr< const IInterpolator< bool > >& leftInterpolator,
					  UPtr< const IInterpolator< bool > >& rightInterpolator );

	template< typename KeyType >
	UPtr< IInterpolator< typename std::enable_if< std::is_enum< KeyType >::value, KeyType >::type > >
		Create		( const Key< KeyType >& leftKey,
					  const Key< KeyType >& rightKey,
					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					  UPtr< const IInterpolator< KeyType > >& rightInterpolator );

	template< typename KeyType >
	UPtr< IInterpolator< typename std::enable_if< std::is_same< KeyType, std::basic_string< typename KeyType::value_type > >::value, KeyType >::type > >
		Create		( const Key< KeyType >& leftKey,
					  const Key< KeyType >& rightKey,
					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					  UPtr< const IInterpolator< KeyType > >& rightInterpolator );

	template< typename KeyType >
	UPtr< IInterpolator< typename std::enable_if< is_param_animation_enabled< KeyType >::value, KeyType >::type > >
		Create		( const Key< KeyType >& leftKey,
					  const Key< KeyType >& rightKey,
					  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					  UPtr< const IInterpolator< KeyType > >& rightInterpolator );

//====================================================================================//
//			Specialization declarations	
//====================================================================================//

#define DECLARE_SPECIALIZATON( type, Function )																	\
template																										\
UPtr< IInterpolator< type > >		Function< type >(	const Key< type >& leftKey,								\
														const Key< type >& rightKey,							\
														UPtr< const IInterpolator< type > >& leftInterpolator,	\
														UPtr< const IInterpolator< type > >& rightInterpolator );

DECLARE_SPECIALIZATON( float, CreateLinear );
DECLARE_SPECIALIZATON( double, CreateLinear );
DECLARE_SPECIALIZATON( char, CreateLinear );
DECLARE_SPECIALIZATON( uint8, CreateLinear );
DECLARE_SPECIALIZATON( int8, CreateLinear );
DECLARE_SPECIALIZATON( uint16, CreateLinear );
DECLARE_SPECIALIZATON( int16, CreateLinear );
DECLARE_SPECIALIZATON( uint32, CreateLinear );
DECLARE_SPECIALIZATON( int32, CreateLinear );
DECLARE_SPECIALIZATON( uint64, CreateLinear );
DECLARE_SPECIALIZATON( int64, CreateLinear );

DECLARE_SPECIALIZATON( float, CreateDiscrete );
DECLARE_SPECIALIZATON( double, CreateDiscrete );
DECLARE_SPECIALIZATON( char, CreateDiscrete );
DECLARE_SPECIALIZATON( uint8, CreateDiscrete );
DECLARE_SPECIALIZATON( int8, CreateDiscrete );
DECLARE_SPECIALIZATON( uint16, CreateDiscrete );
DECLARE_SPECIALIZATON( int16, CreateDiscrete );
DECLARE_SPECIALIZATON( uint32, CreateDiscrete );
DECLARE_SPECIALIZATON( int32, CreateDiscrete );
DECLARE_SPECIALIZATON( uint64, CreateDiscrete );
DECLARE_SPECIALIZATON( int64, CreateDiscrete );
DECLARE_SPECIALIZATON( bool, CreateDiscrete );
DECLARE_SPECIALIZATON( std::wstring, CreateDiscrete );
DECLARE_SPECIALIZATON( std::string, CreateDiscrete );

DECLARE_SPECIALIZATON( float, CreateCosinus );
DECLARE_SPECIALIZATON( double, CreateCosinus );
DECLARE_SPECIALIZATON( char, CreateCosinus );
DECLARE_SPECIALIZATON( uint8, CreateCosinus );
DECLARE_SPECIALIZATON( int8, CreateCosinus );
DECLARE_SPECIALIZATON( uint16, CreateCosinus );
DECLARE_SPECIALIZATON( int16, CreateCosinus );
DECLARE_SPECIALIZATON( uint32, CreateCosinus );
DECLARE_SPECIALIZATON( int32, CreateCosinus );
DECLARE_SPECIALIZATON( uint64, CreateCosinus );
DECLARE_SPECIALIZATON( int64, CreateCosinus );

#undef DECLARE_SPECIALIZATON

//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
template< typename KeyType >
UPtr< IInterpolator< KeyType > >	Create			( InterpolatorType type,
													  const Key< KeyType >& leftKey,
													  const Key< KeyType >& rightKey,
													  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
													  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	switch( type )
	{
		case InterpolatorType::Discrete:
			return CreateDiscrete< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator );
		case InterpolatorType::Linear:
			return CreateLinear< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator );
		case InterpolatorType::Cosinus:
			return CreateCosinus< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator );
		case InterpolatorType::Bezier:
			/// @todo Implement Bezier interpolator.
			return DummyInterpolator< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator );
		default:
			return DummyInterpolator< KeyType >( leftKey, rightKey, leftInterpolator, rightInterpolator );
	}
}

// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< typename std::enable_if< std::is_floating_point< KeyType >::value, KeyType >::type > >
	Create		( const Key< KeyType >& leftKey,
					const Key<KeyType>& rightKey,
					UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return CreateLinear( leftKey, rightKey, leftInterpolator, rightInterpolator );
}


// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< typename std::enable_if< std::is_integral< KeyType >::value, KeyType >::type > >
	Create		( const Key< KeyType >& leftKey,
					const Key< KeyType >& rightKey,
					UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return CreateLinear( leftKey, rightKey, leftInterpolator, rightInterpolator );
}


// ================================ //
//
template<>
inline UPtr< IInterpolator< bool > >
	Create		( const Key< bool >& leftKey,
					const Key< bool >& rightKey,
					UPtr< const IInterpolator< bool > >& leftInterpolator,
					UPtr< const IInterpolator< bool > >& rightInterpolator )
{
	return CreateDiscrete( leftKey, rightKey, leftInterpolator, rightInterpolator );
}

// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< typename std::enable_if< std::is_enum< KeyType >::value, KeyType >::type > >
	Create		( const Key< KeyType >& leftKey,
					const Key<KeyType>& rightKey,
					UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return CreateDiscrete( leftKey, rightKey, leftInterpolator, rightInterpolator );
}

// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< typename std::enable_if< std::is_same< KeyType, std::basic_string< typename KeyType::value_type > >::value, KeyType >::type > >
	Create		( const Key< KeyType >& leftKey,
					const Key<KeyType>& rightKey,
					UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return CreateDiscrete( leftKey, rightKey, leftInterpolator, rightInterpolator );
}

// ================================ //
//
template< typename KeyType >
inline UPtr< IInterpolator< typename std::enable_if< is_param_animation_enabled< KeyType >::value, KeyType >::type > >
	Create		( const Key< KeyType >& leftKey,
					const Key< KeyType >& rightKey,
					UPtr< const IInterpolator< KeyType > >& leftInterpolator,
					UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	return CreateLinear( leftKey, rightKey, leftInterpolator, rightInterpolator );
}



}	// DefaultInterpolators
