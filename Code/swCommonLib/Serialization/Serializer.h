#pragma once
/**@file Serializer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy serializatora.*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Serialization/SerializationContext.h"

#include <string>
#include <assert.h>
#include <memory>


/**@defgroup Serialization Serializacja
@ingroup Tools
@brief Biblioteki do serializacji.*/



struct SerializerImpl;

/**@brief Tryb Zapisywania JSONa.
@ingroup Serialization*/
enum class WritingMode
{
	Sparing,				///< Tryb oszczêdny bez spacji i tabulacji.
	Readable				///< Tryb przeznaczony do czytania przez ludzi.
};

/**@brief Interfejs dla serializatorów.

@ingroup Serialization*/
class ISerializer
{
private:
	SerializerImpl*								impl;
	std::unique_ptr< SerializationContext >		context;
protected:
public:
	explicit	ISerializer		( std::unique_ptr< SerializationContext > serContext );
	virtual		~ISerializer	();

	void		EnterObject		( const std::string& name );
	void		EnterArray		( const std::string& name );

	void		Exit			();

	void		SetAttribute		( const std::string& name, const std::string& value );
	void		SetAttribute		( const std::string& name, const char* value );
	void		SetAttribute		( const std::string& name, uint32 value );
	void		SetAttribute		( const std::string& name, uint64 value );
	void		SetAttribute		( const std::string& name, int32 value );
	void		SetAttribute		( const std::string& name, int64 value );
	void		SetAttribute		( const std::string& name, bool value );
	void		SetAttribute		( const std::string& name, double value );


	bool		SaveFile			( const std::string& fileName, WritingMode mode = WritingMode::Sparing );
	std::string	SaveString			( WritingMode mode = WritingMode::Sparing );


public:

	/**@brief Zwraca kontekst serializacji.
	
	Funkcja sprawdza typ kontekstu jedynie w trybie debug (assert).*/
	template< typename ContextType >
	inline ContextType*			GetContext	()
	{
		assert( context != nullptr );
		
		// Sprawdzanie dynamicznego typu tylko, je¿eli w³¹czone jest RTTI.
#ifdef _CPPRTTI
		assert( typeid( *context ) == typeid( ContextType ) );
#endif

		return static_cast< ContextType* >( context.get() );
	}
};


