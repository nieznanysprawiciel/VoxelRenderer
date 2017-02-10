#pragma once
/**
@file LinearInterpolator.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "IInterpolator.h"

#include <assert.h>


/**@brief Linear interpolator.

@ingroup Interpolators*/
template< typename KeyType >
class LinearInterpolator : public IInterpolator< KeyType >
{
private:
protected:
public:
	/// Constructor for serialization.
	explicit		LinearInterpolator		() = default;
	/// Constructor for default interpolator creation function.
	explicit		LinearInterpolator		( const Key< KeyType >& leftKey,
											  const Key< KeyType >& rightKey,
											  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
											  UPtr< const IInterpolator< KeyType > >& rightInterpolator );
					~LinearInterpolator		() = default;


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

protected:
	KeyType				Tangent		( const Key< KeyType >& left, const Key< KeyType >& right ) const;
};

//====================================================================================//
//			Implementation	
//====================================================================================//


// ================================ //
//
template< typename KeyType >
inline				LinearInterpolator< KeyType >::LinearInterpolator	( const Key< KeyType >& leftKey,
																		  const Key< KeyType >& rightKey,
																		  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																		  UPtr< const IInterpolator< KeyType >> & rightInterpolator )
{}

// ================================ //
//
template< typename KeyType >
inline KeyType		LinearInterpolator< KeyType >::Interpolate	( TimeType time, Key< KeyType >& left, Key< KeyType >& right )
{
	TimeType timeInterval = right.Time - left.Time;
	TimeType progress = ( time - left.Time ) / timeInterval;

	auto leftResult = ( 1.0 + (-1.0f) * progress ) * left.Value;	// Multiply by -1.0f to avoid calling operator-
	auto rightResult = progress * right.Value;

	return static_cast< KeyType >( leftResult + rightResult );
}

// ================================ //
//
template< typename KeyType >
inline void			LinearInterpolator< KeyType >::Update		( const Key< KeyType >& leftKey,
																  const Key< KeyType >& rightKey,
																  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{
	// Intencionally empty.
}

// ================================ //
//
template< typename KeyType >
inline KeyType		LinearInterpolator< KeyType >::LeftTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const
{
	return Tangent( left, right );
}

// ================================ //
//
template< typename KeyType >
inline KeyType		LinearInterpolator< KeyType >::RightTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const
{
	return Tangent( left, right );
}


//====================================================================================//
//			Internal	
//====================================================================================//

// ================================ //
//
template< typename KeyType >
inline KeyType		LinearInterpolator< KeyType >::Tangent		( const Key< KeyType >& left, const Key< KeyType >& right ) const
{
	KeyType valueInterval = right.Value - left.Value;
	TimeType timeInterval = right.Time - left.Time;

	return static_cast< KeyType >( ( TimeType( 1.0f ) / timeInterval )* valueInterval );
}
