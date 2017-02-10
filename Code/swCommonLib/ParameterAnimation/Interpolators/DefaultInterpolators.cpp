/**
@file DefaultInterpolators.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "DefaultInterpolators.h"
#include "DefaultInterpolators.inl"

#include "LinearInterpolator.h"
#include "DiscreteInterpolator.h"
#include "CosinusInterpolator.h"

namespace DefaultInterpolators
{

//====================================================================================//
//			Instantiantions	
//====================================================================================//

#define DEFINE_SPECIALIZATION( type, Function )																	\
template																										\
UPtr< IInterpolator< type > >		Function< type >( const Key< type >& leftKey,								\
													  const Key< type >& rightKey,								\
													  UPtr< const IInterpolator< type > >& leftInterpolator,	\
													  UPtr< const IInterpolator< type > >& rightInterpolator );

DEFINE_SPECIALIZATION( float, CreateLinear );
DEFINE_SPECIALIZATION( double, CreateLinear );
DEFINE_SPECIALIZATION( char, CreateLinear );
DEFINE_SPECIALIZATION( uint8, CreateLinear );
DEFINE_SPECIALIZATION( int8, CreateLinear );
DEFINE_SPECIALIZATION( uint16, CreateLinear );
DEFINE_SPECIALIZATION( int16, CreateLinear );
DEFINE_SPECIALIZATION( uint32, CreateLinear );
DEFINE_SPECIALIZATION( int32, CreateLinear );
DEFINE_SPECIALIZATION( uint64, CreateLinear );
DEFINE_SPECIALIZATION( int64, CreateLinear );

DEFINE_SPECIALIZATION( float, CreateDiscrete );
DEFINE_SPECIALIZATION( double, CreateDiscrete );
DEFINE_SPECIALIZATION( char, CreateDiscrete );
DEFINE_SPECIALIZATION( uint8, CreateDiscrete );
DEFINE_SPECIALIZATION( int8, CreateDiscrete );
DEFINE_SPECIALIZATION( uint16, CreateDiscrete );
DEFINE_SPECIALIZATION( int16, CreateDiscrete );
DEFINE_SPECIALIZATION( uint32, CreateDiscrete );
DEFINE_SPECIALIZATION( int32, CreateDiscrete );
DEFINE_SPECIALIZATION( uint64, CreateDiscrete );
DEFINE_SPECIALIZATION( int64, CreateDiscrete );
DEFINE_SPECIALIZATION( bool, CreateDiscrete );


DEFINE_SPECIALIZATION( float, CreateCosinus );
DEFINE_SPECIALIZATION( double, CreateCosinus );
DEFINE_SPECIALIZATION( char, CreateCosinus );
DEFINE_SPECIALIZATION( uint8, CreateCosinus );
DEFINE_SPECIALIZATION( int8, CreateCosinus );
DEFINE_SPECIALIZATION( uint16, CreateCosinus );
DEFINE_SPECIALIZATION( int16, CreateCosinus );
DEFINE_SPECIALIZATION( uint32, CreateCosinus );
DEFINE_SPECIALIZATION( int32, CreateCosinus );
DEFINE_SPECIALIZATION( uint64, CreateCosinus );
DEFINE_SPECIALIZATION( int64, CreateCosinus );



}	// DefaultInterpolators

