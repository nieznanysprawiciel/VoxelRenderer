#pragma once
/**
@file IInterpolator.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/TypesDefinitions.h"

/**@defgroup Interpolators Interpolators
@ingroup KeyFrameAnimation

@section Writing interpolators

To write yout own interpolator you should implement some functions from IInterpolator interface.
@todo Complete description.
*/


template< typename KeyType >
struct Key;


enum class InterpolatorType : int16
{
	Discrete,
	Linear,
	Cosinus,
	Bezier
};



/**@brief Base class for interpolators.

@ingroup Interpolators*/
template< typename KeyType >
class IInterpolator
{
private:
protected:
public:
	explicit		IInterpolator	() = default;
	virtual			~IInterpolator	() = default;

	/**@brief Main function invoked by evaluator.*/
	virtual KeyType			Interpolate		( TimeType time, Key< KeyType >& left, Key< KeyType >& right )	= 0;

	/**@brief Function updates interpolator, when left or right key value changes.
	@note Left or right interpolator can be nullptr, but keys always exist.*/
	virtual	void			Update			( const Key< KeyType >& leftKey,
											  const Key< KeyType >& rightKey,
											  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
											  UPtr< const IInterpolator< KeyType > >& rightInterpolator )	= 0;

	/**@brief Returns curve tangent.
	Function can be used by surrounding interpolators to smooth curve.*/
	virtual KeyType			LeftTangent		( const Key< KeyType >& left, const Key< KeyType >& right ) const = 0;
	/**@brief Returns curve tangent.
	Function can be used by surrounding interpolators to smooth curve.*/
	virtual KeyType			RightTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const = 0;
};


/**@brief Temporary interpolator for internal use.*/
template< typename KeyType >
class DummyInterpolator : public IInterpolator< KeyType >
{
private:
protected:
public:

	/**@brief Main function invoked by evaluator.*/
	virtual KeyType			Interpolate		( TimeType time, Key< KeyType >& left, Key< KeyType >& right ) { return KeyType(); }

	/**@brief Function updates interpolator, when left or right key value changes.
	@note Left or right interpolator can be nullptr, but keys always exist.*/
	virtual	void			Update			( const Key< KeyType >& leftKey,
											  const Key< KeyType >& rightKey,
											  UPtr< const IInterpolator< KeyType > >& leftInterpolator,
											  UPtr< const IInterpolator< KeyType > >& rightInterpolator )
	{}

	/**@brief Returns curve tangent.
	Function can be used by surrounding interpolators to smooth curve.*/
	virtual KeyType			LeftTangent		( const Key< KeyType >& left, const Key< KeyType >& right ) const { return KeyType(); }
	/**@brief Returns curve tangent.
	Function can be used by surrounding interpolators to smooth curve.*/
	virtual KeyType			RightTangent	( const Key< KeyType >& left, const Key< KeyType >& right ) const { return KeyType(); }
};
