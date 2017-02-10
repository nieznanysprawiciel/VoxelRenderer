#pragma once
/**
@file CosinusInterpolator.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "IInterpolator.h"

#define _USE_MATH_DEFINES
#include <math.h>


/**@brief Cosinus interpolator
@ingroup Interpolators*/
template< typename KeyType >
class CosinusInterpolator : public IInterpolator< KeyType >
{
private:
protected:
public:
	explicit		CosinusInterpolator		() = default;
	/// Constructor for default interpolator creation function.
	explicit		CosinusInterpolator		( const Key< KeyType >& leftKey,
											  const Key< KeyType >& rightKey,
											  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
											  UPtr< const IInterpolator< KeyType > >& rightInterpolator );
					~CosinusInterpolator	() = default;


	/**@brief Main function invoked by evaluator.*/
	virtual KeyType			Interpolate		( TimeType time, Key< KeyType >& left, Key< KeyType >& right ) override;

	/**@brief Function updates interpolator, when left or right key value changes.
	@note Left or right interpolator can be nullptr, but keys always exist.*/
	virtual	void			Update			( const Key< KeyType >& leftKey,
											  const Key< KeyType >& rightKey,
											  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
											  UPtr< const IInterpolator< KeyType > >& rightInterpolator ) override;

	/**@brief Returns curve tangent.
	Function can be used by surrounding interpolators to smooth curve.*/
	virtual KeyType			LeftTangent		( const Key< KeyType >& left, const Key< KeyType >& right ) const override;
	/**@brief Returns curve tangent.
	Function can be used by surrounding interpolators to smooth curve.*/
	virtual KeyType			RightTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const override;
};


//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
template< typename KeyType >
inline				CosinusInterpolator< KeyType >::CosinusInterpolator	( const Key< KeyType >& leftKey,
																		  const Key< KeyType >& rightKey,
																		  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																		  UPtr< const IInterpolator< KeyType >> & rightInterpolator )
{}


// ================================ //
//
template< typename KeyType >
inline KeyType			CosinusInterpolator< KeyType >::Interpolate		( TimeType time, Key< KeyType >& left, Key< KeyType >& right )
{
	TimeType timeInterval = right.Time - left.Time;
	TimeType progress = ( time - left.Time ) / timeInterval;

	progress = 0.5 + 0.5 * cos( progress * M_PI );

	auto leftResult = ( 1.0 + (-1.0f) * progress ) * left.Value;	// Multiply by -1.0f to avoid calling operator-
	auto rightResult = progress * right.Value;

	return static_cast< KeyType >( leftResult + rightResult );
}

// ================================ //
//
template< typename KeyType >
inline void				CosinusInterpolator< KeyType >::Update			( const Key< KeyType >& leftKey,
																		  const Key< KeyType >& rightKey,
																		  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																		  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{}

// ================================ //
//
template< typename KeyType >
inline KeyType			CosinusInterpolator< KeyType >::LeftTangent		( const Key< KeyType >& left, const Key< KeyType >& right ) const
{
	return KeyType( 0 );
}

// ================================ //
//
template< typename KeyType >
inline KeyType			CosinusInterpolator< KeyType >::RightTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const
{
	return KeyType( 0 );
}
