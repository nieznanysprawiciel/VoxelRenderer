#pragma once

#include <string>


enum class NullableInit
{		Valid	};


template< typename ResultType >
struct Nullable
{
	ResultType		Value;
	bool			IsValid;
	std::string		ErrorString;



	/**@brief Creates invalid object. Value fieled is default contructed.*/
	Nullable()
		:	IsValid( false )
		,	Value( ResultType() )
	{}


	/**@brief Creates valid object. Value fieled is default contructed.*/
	explicit Nullable( NullableInit )
		:	IsValid( true )
		,	Value( ResultType() )
	{}

	/**@brief Creates invalid object and sets error string. Value fieled is default contructed.*/
	Nullable( std::string&& error )
		:	ErrorString( std::move( error ) )
		,	IsValid( false )
		,	Value( ResultType() )
	{}

	/**@brief Creates valid object.*/
	Nullable( ResultType&& result )
		:	Value( std::move( result ) )
		,	IsValid( true )
	{}

	Nullable( const Nullable< ResultType >& other )
		:	ErrorString( other.ErrorString )
		,	IsValid( other.IsValid )
		,	Value( other.Value )
	{}

	Nullable( Nullable< ResultType >&& other )
		:	ErrorString( std::move( other.ErrorString ) )
		,	IsValid( other.IsValid )
		,	Value( std::move( other.Value ) )
	{}

	void operator=( const Nullable< ResultType >& other )
	{
		Value = other.Value;
		IsValid = other.IsValid;
		ErrorString = other.ErrorString;
	}

	void operator=( Nullable< ResultType >&& other )
	{
		Value = std::move( other.Value );
		IsValid = other.IsValid;
		ErrorString = std::move( other.ErrorString );
	}


	bool operator!()
	{
		return !IsValid;
	}

};

#define ReturnIfInvalid( nullable )		if( !nullable.IsValid ) return std::move( nullable.ErrorString );

