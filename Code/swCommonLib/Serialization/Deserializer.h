#pragma once
/**@file Deserializer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy deserializatora.*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Serialization/SerializationContext.h"

#include <string>
#include <memory>

struct DeserializerImpl;

/**@brief Tryby parsowania.
@ingruop Serialization

Parsowanie insitu powoduje zapisanie ca³ego parsowanego dokumentu w pamiêci deserializatora.
Nowe stringi nie s¹ alokowane, a zapisywane s¹ wska¿niki do tego dokumentu.
Metoda prawdopodobnie zajmie wiêcej pamiêci ni¿ w przypadku alokowania, poniewa¿
przechowywane s¹ równie¿ nadmiarowe bia³e znaki i wszystkie niepotrzebne elementy.
Zalet¹ jest za to unikanie alokacji, co powinno przypsieszyæ dzia³anie.

AllocString usuwa ca³y dokument z pamiêci po sparsowaniu.
Metoda dzia³a wolniej, ale za to przechowuje tylko to co potrzebne.
Wskazaniem do u¿ycia s¹ przypadki, gdy deserializator s³u¿y nie tylko
do jednokrotnego parsowania, ale równie¿ przechowywania danych.*/
enum class ParsingMode
{
	ParseInsitu,		///< Nie alokuje stringów, ale zapisuje sobie wskaŸniki na miejsca w parsowanym tekœcie.
	AllocStrings		///< Przy parsowaniu alokuje nowe stringi.
};

/**@brief Interfejs deserializatorów.

@ingroup Serialization*/
class IDeserializer
{
private:
	DeserializerImpl*							impl;
	std::unique_ptr< SerializationContext >		context;
protected:
public:
	IDeserializer();
	IDeserializer( std::unique_ptr< SerializationContext > serContext );
	~IDeserializer();

	bool			LoadFromFile	( const std::string& fileName, ParsingMode mode );
	bool			LoadFromString	( const std::string& contentString );

	const char*		GetName			();

	bool			EnterObject		( const std::string& name );
	bool			EnterArray		( const std::string& name );
	bool			EnterObject		( const char* name );
	bool			EnterArray		( const char* name );
	void			Exit			();

	bool			FirstElement	();
	bool			NextElement		();
	bool			PrevElement		();
	bool			LastElement		();

	std::string		GetAttribute		( const std::string& name, std::string& defaultValue );
	const char*		GetAttribute		( const std::string& name, const char* defaultValue );
	uint32			GetAttribute		( const std::string& name, uint32 defaultValue );
	uint64			GetAttribute		( const std::string& name, uint64 defaultValue );
	int32			GetAttribute		( const std::string& name, int32 defaultValue );
	int64			GetAttribute		( const std::string& name, int64 defaultValue );
	bool			GetAttribute		( const std::string& name, bool defaultValue );
	double			GetAttribute		( const std::string& name, double defaultValue );

	std::string		GetAttribute		( const char* name, std::string& defaultValue );
	const char*		GetAttribute		( const char* name, const char* defaultValue );
	uint32			GetAttribute		( const char* name, uint32 defaultValue );
	uint64			GetAttribute		( const char* name, uint64 defaultValue );
	int32			GetAttribute		( const char* name, int32 defaultValue );
	int64			GetAttribute		( const char* name, int64 defaultValue );
	bool			GetAttribute		( const char* name, bool defaultValue );
	double			GetAttribute		( const char* name, double defaultValue );


	std::string		GetError			();

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


