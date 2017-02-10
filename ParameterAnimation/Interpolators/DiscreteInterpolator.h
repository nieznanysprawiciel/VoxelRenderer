#pragma once
/**
@file DiscreteInterpolator.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "IInterpolator.h"



/**@brief 

@ingroup Interpolators*/
template< typename KeyType >
class DiscreteInterpolator : public IInterpolator< KeyType >
{
private:
protected:
public:
	/// Constructor for serialization.
	explicit		DiscreteInterpolator	() = default;
	/// Constructor for default interpolator creation function.
	explicit		DiscreteInterpolator	( const Key< KeyType >& leftKey,
											  const Key< KeyType >& rightKey,
											  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
											  UPtr< const IInterpolator< KeyType > >& rightInterpolator );
					~DiscreteInterpolator() = default;

	/**@brief Interpolates value.*/
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
inline				DiscreteInterpolator< KeyType >::DiscreteInterpolator	( const Key< KeyType >& leftKey,
																			  const Key< KeyType >& rightKey,
																			  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
																			  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{}

// ================================ //
//
template< typename KeyType >
inline KeyType		DiscreteInterpolator< KeyType >::Interpolate	( TimeType time, Key< KeyType >& left, Key< KeyType >& right )
{
	return left.Value;
}

// ================================ //
//
template< typename KeyType >
inline void			DiscreteInterpolator< KeyType >::Update			( const Key< KeyType >& leftKey, const Key< KeyType >& rightKey, UPtr< const IInterpolator< KeyType > >& leftInterpolator, UPtr< const IInterpolator< KeyType > >& rightInterpolator )
{}

// ================================ //
//
template< typename KeyType >
inline KeyType		DiscreteInterpolator< KeyType >::LeftTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const
{
	return KeyType( 0 );
}

// ================================ //
//
template< typename KeyType >
inline KeyType		DiscreteInterpolator< KeyType >::RightTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const
{
	return KeyType( 0 );
}
