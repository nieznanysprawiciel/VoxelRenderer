#pragma once
/**
@file KeySet.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "Key.h"
#include "Interpolators/IInterpolator.h"
#include "Interpolators/DefaultInterpolators.h"

#include <vector>
#include <algorithm>
#include <assert.h>


/**@brief Set of animation keys and interpolators.

@attention You can directly access Keys and Interpolators arrays but it's better
to use functions designed to do this.

@ingroup KeyFrameAnimation*/
template< typename KeyType >
struct KeySet
{
public:
	typedef IInterpolator< KeyType > Interpolator;
public:
	std::vector< Key< KeyType > >		Keys;
	std::vector< UPtr< Interpolator > >	Interpolators;

public:
	/// Constructor insert default KeyType in time 0.0;
	explicit				KeySet	();

	Key< KeyType >*			GetKey		( TimeType time );
	bool					AddKey		( TimeType time, const KeyType& value );
	bool					AddKey		( TimeType time, const KeyType& value, InterpolatorType type );
	bool					UpdateKey	( TimeType time, const KeyType& value );
	bool					RemoveKey	( TimeType time );

	KeyType					Evaluate	( TimeType time );

	bool					ChangeInterpolator	( Size idx, UPtr< Interpolator >&& interpolator );

private:

	typename std::vector< Key< KeyType > >::iterator		FindKey			( TimeType time );
	typename std::vector< Key< KeyType > >::iterator		FindPlace		( TimeType time );

	/**@brief Adds default interpolator for given type.
	Adds interpolator defined by function DefaultInterpolators::Create< KeyType> for key added under given index.
	@param[in] keyIndex Index of added key.*/
	void					AddDefaultInterpolator			( Size keyIndex );
	/**@brief Updates interpolator under index. If interpolator doesn't exists, nothing bad happens.*/
	void					UpdateInterpolator				( Size index );
};


//====================================================================================//
//			Implementation	
//====================================================================================//

extern const TimeType epsilon;

// ================================ //
//
template< typename KeyType, template< typename KeyType > class InterpolatorType >
UPtr< const IInterpolator< KeyType > >		CastConst		( UPtr< InterpolatorType< KeyType > >& interpolator )
{
	return std::static_pointer_cast< const IInterpolator< KeyType > >( interpolator );
}

// ================================ //
//
template< typename KeyType >
inline					KeySet< KeyType >::KeySet()
{
	Key< KeyType > firstKey;
	firstKey.Time = TimeType( 0.0 );
	firstKey.Value = KeyType();

	Keys.push_back( std::move( firstKey ) );
}

// ================================ //
//
template< typename KeyType >
inline Key< KeyType >*	KeySet< KeyType >::GetKey		( TimeType time )
{
	auto iter = FindKey( time );
	if( iter != Keys.end() )
		return &( *iter );
	return nullptr;
}

/**@brief Adds key. If key exists updates this key.

@return Returns false if key existed and have been updated.*/
template< typename KeyType >
inline bool				KeySet< KeyType >::AddKey		( TimeType time, const KeyType& value )
{
	auto iter = FindPlace( time );

	if( iter == Keys.end() )
	{
		Keys.push_back( Key< KeyType >( time, value ) );
		AddDefaultInterpolator( Keys.size() - 1 );

		return true;
	}

	// Key exists. Update it.
	if( abs( iter->Time - time ) < epsilon )
	{
		iter->Value = value;
		return false;
	}

	// Add new key in the middle of vector.
	iter = Keys.insert( iter, Key< KeyType >( time, value ) );
	AddDefaultInterpolator( std::distance( Keys.begin(), iter ) );

	return true;
}

/**@brief Adds key. If key exists updates this key.

@param[in] type Adds one of built in interpolators.

@return Returns false if key existed and have been updated.*/
template< typename KeyType >
inline bool				KeySet< KeyType >::AddKey		( TimeType time, const KeyType& value, InterpolatorType type )
{
	return false;
}

/**@brief Updates key.
@return Returns false if key couldn't be found.*/
template< typename KeyType >
inline bool				KeySet< KeyType >::UpdateKey	( TimeType time, const KeyType& value )
{
	auto iter = FindKey( time );
	if( iter == Keys.end() )
		return false;

	iter->Value = value;

	auto keyIdx = std::distance( Keys.begin(), iter );
	UpdateInterpolator( keyIdx );
	UpdateInterpolator( keyIdx - 1 );

	return true;
}

/**@brief Removes key.
Last key can't be removed.

@return Returns false if key couldn't be found or it was last key..*/
template< typename KeyType >
inline bool				KeySet< KeyType >::RemoveKey	( TimeType time )
{
	if( Keys.size() <= 1 )
		return false;

	auto iter = FindKey( time );
	if( iter == Keys.end() )
		return false;

	iter = Keys.erase( iter );

	// Remove interplator.
	auto keyIdx = std::distance( Keys.begin(), iter );
	auto interIter = Interpolators.erase( Interpolators.begin() + keyIdx );

	Size interpolatorIdx = std::distance( Interpolators.begin(), interIter );
	UpdateInterpolator( interpolatorIdx );
	UpdateInterpolator( interpolatorIdx - 1 );

	return true;
}

// ================================ //
//
template< typename KeyType >
inline KeyType			KeySet< KeyType >::Evaluate		( TimeType time )
{
	if( Keys.size() == 1 )
		return Keys[ 0 ].Value;

	///@todo Check if we can find keywith std::lower_bound or FindPlace. Messure performance in cases with multiple keys
	/// and choose better version.
	auto iter = Keys.begin();
	for( ; iter != Keys.end(); ++iter )
	{
		if( iter->Time > time )
			break;
	}

	Size keyIdx = std::distance( Keys.begin(), iter );

	if( keyIdx >= Keys.size() )
		return Keys.back().Value;
	if( keyIdx == 0 )
		return Keys[ 0 ].Value;

	return Interpolators[ keyIdx - 1 ]->Interpolate( time, Keys[ keyIdx - 1 ], Keys[ keyIdx ] );
}

// ================================ //
//
template< typename KeyType>
inline bool					KeySet< KeyType >::ChangeInterpolator	( Size idx, UPtr< Interpolator >&& interpolator )
{
	if( idx < Interpolators.size() )
	{
		Interpolators[ idx ] = std::move( interpolator );
		UpdateInterpolator( idx );

		return true;
	}

	return false;
}

//====================================================================================//
//			Internal functions	
//====================================================================================//


// ================================ //
//
template< typename KeyType >
inline typename std::vector< Key< KeyType > >::iterator			KeySet< KeyType >::FindKey	( TimeType time )
{
	Key< KeyType > fakeKey;
	fakeKey.Time = time;

	auto iterRange = std::equal_range( Keys.begin(), Keys.end(), fakeKey );

	assert( std::distance( iterRange.first, iterRange.second ) <= 1 );
	if( std::distance( iterRange.first, iterRange.second ) == 1 )
		return iterRange.first;
	else
		return Keys.end();
}

// ================================ //
//
template< typename KeyType >
inline typename std::vector< Key< KeyType > >::iterator			KeySet< KeyType >::FindPlace	( TimeType time )
{
	Key< KeyType > fakeKey;
	fakeKey.Time = time;

	auto iter = std::lower_bound( Keys.begin(), Keys.end(), fakeKey );
	return iter;
}

// ================================ //
//
template< typename KeyType >
inline void				KeySet< KeyType >::AddDefaultInterpolator	( Size keyIndex )
{
	Size leftKeyIdx;
	Size rightKeyIdx;

	if( keyIndex == Keys.size() - 1 )
	{
		leftKeyIdx = keyIndex - 1;
		rightKeyIdx = keyIndex;
	}
	else
	{
		leftKeyIdx = keyIndex;
		rightKeyIdx = keyIndex + 1;
	}


	// We always have at least one key and we already added the second.
	auto& leftKey = Keys[ leftKeyIdx ];
	auto& rightKey = Keys[ rightKeyIdx ];
	
	// Create new interpolator.
	auto& leftInterpolator = keyIndex > 0 && keyIndex < Interpolators.size() ? Interpolators[ keyIndex - 1 ] : MakeUPtr< DummyInterpolator< KeyType > >();
	auto& rightInterpolator = keyIndex < Interpolators.size() ? Interpolators[ keyIndex ] : MakeUPtr< DummyInterpolator< KeyType > >();
	
	auto interpolator = DefaultInterpolators::Create< KeyType >( leftKey, rightKey, CastConst( leftInterpolator ), CastConst( rightInterpolator ) );

	// Add interpolator
	if( keyIndex == Keys.size() - 1 )
		Interpolators.push_back( std::move( interpolator ) );
	else
		Interpolators.insert( Interpolators.begin() + keyIndex, std::move( interpolator ) );


	// Update sourrounding interpolators
	UpdateInterpolator( keyIndex - 1 );
	UpdateInterpolator( keyIndex + 1 );		// Note: We inserted interpolator under keyIndex.
}

// ================================ //
//
template< typename KeyType >
inline void			KeySet< KeyType >::UpdateInterpolator			( Size index )
{
	// Updated interpolator must exist. Note: index is unsigned and is always greater than zero.
	if( index >= Interpolators.size() )
		return;

	// KeySet is in proper state.
	assert( Keys.size() == Interpolators.size() + 1 );

	auto& leftInterpolator = index >= 1 ? CastConst( Interpolators[ index - 1 ] ) : CastConst( MakeUPtr< DummyInterpolator< KeyType > >() );
	auto& rightInterpolator = index + 1 < Interpolators.size() ? CastConst( Interpolators[ index + 1 ] ) : CastConst( MakeUPtr< DummyInterpolator< KeyType > >() );
	
	auto& leftKey = Keys[ index ];
	auto& rightKey = Keys[ index + 1 ];

	auto& interpolator = Interpolators[ index ];

	interpolator->Update( leftKey, rightKey, leftInterpolator, rightInterpolator );
}
