#include "swCommonLib/Serialization/Deserializer.h"

#include "swCommonLib/External/RapidXML/rapidxml.hpp"


#include <fstream>
#include <stack>
#include <stdlib.h>




struct DeserializerImpl
{
	rapidxml::xml_document<>				root;
	std::stack< rapidxml::xml_node<>* >		valuesStack;
	char*									fileContent;
	std::string								errorString;

	DeserializerImpl()
	{	fileContent = nullptr;	}
};



IDeserializer::IDeserializer()
	:	context( nullptr )
{
	impl = new DeserializerImpl;
}

IDeserializer::IDeserializer( std::unique_ptr< SerializationContext > serContext )
	: context( std::move( serContext ) )
{
	impl = new DeserializerImpl;
}

IDeserializer::~IDeserializer()
{	
	delete[] impl->fileContent;
	delete impl;
}

//====================================================================================//
//			Wczytywanie i parsowanie	
//====================================================================================//

/**@brief Wczytuje i parsuje podany plik.

@param[in] fileName Nazwa pliku.
@param[in] mode Tryb parsowania. Parser XMLowy wspiera tylko parsowanie insitu.
@return Zwraca informacjê czy parsowanie powiod³o siê.*/
bool			IDeserializer::LoadFromFile		( const std::string& fileName, ParsingMode mode )
{
	std::ifstream file( fileName, std::ios::binary | std::ios::ate );

	if( file.fail() )
		return false;
	
	// Szukamy koñca pliku, a potem wracamy na pocz¹tek.
	std::streambuf* rawBuffer = file.rdbuf();
	unsigned int fileSize = static_cast< unsigned int >( rawBuffer->pubseekoff( 0, file.end ) );
	file.seekg ( 0, file.beg );


	// Alokujemy bufor odpowiedniej d³ugoœci
	delete[] impl->fileContent;
	impl->fileContent = new char[ fileSize + 1 ];

	// Wczytujemy plik do bufora
	auto result = rawBuffer->sgetn( impl->fileContent, fileSize );
	file.close();

	// Dodajemy znak koñca stringa na koñcu pliku
	impl->fileContent[ result ] = '\0';

	impl->valuesStack.push( &impl->root );

	try
	{
		// Parsujemy w zale¿noœci od wybranego trybu
		if( mode == ParsingMode::ParseInsitu )
			impl->root.parse< rapidxml::parse_default >( impl->fileContent );
		else // ParsingMode::AllocStrings
		{
			// W tym trybie wszystkie stringi s¹ alokowane.
			// Dlatego po wykonaniu tej operacji kasujemy bufor.
			assert( false );		// rapidXML nie obs³uguje niestety tego trybu

			impl->root.parse< rapidxml::parse_default >( impl->fileContent );
		}
	}
	catch( const rapidxml::parse_error& e )
	{
		impl->errorString = "Error: ";
		impl->errorString += e.what();
		impl->errorString += " Offset: ";
		impl->errorString += std::to_string( static_cast<PtrOffset>( e.where<char>() - impl->fileContent ) );

		return false;
	}
	catch( const std::exception& e )
	{
		impl->errorString = "Error: ";
		impl->errorString += e.what();
		return false;
	}
	catch( ... )
	{
		impl->errorString = "Unknown error";
		return false;
	}

	return true;
}

/**@brief Parsuje XMLa z podanego stringa.
@param[in] contentString String do sparsowania.*/
bool			IDeserializer::LoadFromString	( const std::string& contentString )
{
	return false;
}

//====================================================================================//
//			Iterowanie po drzewie	
//====================================================================================//

/**@brief Zwraca nazwê wêz³a, w którym znajduje siê serializator.*/
const char*		IDeserializer::GetName			()
{
	return impl->valuesStack.top()->name();
}


/**@brief Wyszukuje obiekt o podanej nazwie i wchodzi w g³¹b drzewa.

@param[in] name Nazwa obiektu.
@return Zwraca false, je¿eli obiekt o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterObject		( const std::string& name )
{
	return EnterObject( name.c_str() );
}

/**@brief Wyszukuje obiekt o podanej nazwie i wchodzi w g³¹b drzewa.

@param[in] name Nazwa obiektu.
@return Zwraca false, je¿eli obiekt o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterObject		( const char* name )
{
	assert( !impl->valuesStack.empty() );
	auto value = impl->valuesStack.top();

	auto enterNode = value->first_node( name );
	if( enterNode == nullptr )
		return false;

	//assert( enterNode->type() == rapidxml::node_type:: );

	impl->valuesStack.push( enterNode );
	return true;
}

/**@brief Wyszukuje tablicê o podanej nazwie i wchodzi w g³¹b drzewa.

@param[in] name Nazwa tablicy.
@return Zwraca false, je¿eli tablica o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterArray		( const std::string& name )
{
	return EnterArray( name.c_str() );
}

/**@brief Wyszukuje tablicê o podanej nazwie i wchodzi w g³¹b drzewa.

@param[in] name Nazwa tablicy.
@return Zwraca false, je¿eli tablica o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterArray		( const char* name )
{
	assert( !impl->valuesStack.empty() );
	auto value = impl->valuesStack.top();

	auto enterNode = value->first_node( name );
	if( enterNode == nullptr )
		return false;

	//assert( enterNode->type() == rapidxml::node_type:: );

	impl->valuesStack.push( enterNode );
	return true;
}

/**@brief */
void			IDeserializer::Exit			()
{
	auto value = impl->valuesStack.top();
	impl->valuesStack.pop();
}

//=========================================================//
//				
//=========================================================//

/**@brief Wchodzi do pierwszego elementu tablicy lub obiektu.

Je¿eli wêze³, w którym jesteœmy, nie ma ¿adnych dzieci, pozostajemy w nim
i stan serializatora nie zmienia siê.

@return Zwaca false, je¿eli nie ma ¿adnego obiektu w tablicy (lub obiekcie).*/
bool IDeserializer::FirstElement()
{
	auto value = impl->valuesStack.top();
	
	auto firstChild = value->first_node( nullptr );		// Wstawienie nullptra oznacza wybranie pierwszego dziecka w kolejnoœci.
	if( firstChild == nullptr )
		return false;

	impl->valuesStack.push( firstChild );
	return true;
}

/**@brief Przechodzi do nastêpnego elementu w tablicy lub w obiekcie.

@return Zwraca false, je¿eli nie istnieje ju¿ kolejny element, do którego mo¿na przejœæ.*/
bool IDeserializer::NextElement()
{
	auto value = impl->valuesStack.top();
	impl->valuesStack.pop();
	
	auto nextValue = value->next_sibling( nullptr );		// Wstawienie nullptra oznacza wybranie kolejnego node'a w kolejnoœci.
	if( nextValue == nullptr )
		return false;

	impl->valuesStack.push( nextValue );
	return true;
}

/**@brief Przechodzi do poprzedniego elementu w tablicy lub w obiekcie.*/
bool IDeserializer::PrevElement()
{
	auto value = impl->valuesStack.top();
	impl->valuesStack.pop();
	
	auto prevValue = value->previous_sibling( nullptr );		// Wstawienie nullptra oznacza wybranie kolejnego node'a w kolejnoœci.
	if( prevValue == nullptr )
		return false;

	impl->valuesStack.push( prevValue );
	return true;
}

/**@brief Wchodzi do ostatniego elementu tablicy lub obiektu.

Je¿eli wêze³, w którym jesteœmy, nie ma ¿adnych dzieci, pozostajemy w nim
i stan serializatora nie zmienia siê.

@return Zwaca false, je¿eli nie ma ¿adnego obiektu w tablicy (lub obiekcie).*/
bool IDeserializer::LastElement()
{
	auto value = impl->valuesStack.top();
	
	auto lastChild = value->last_node( nullptr );		// Wstawienie nullptra oznacza wybranie pierwszego dziecka w kolejnoœci.
	if( lastChild == nullptr )
		return false;

	impl->valuesStack.push( lastChild );
	return true;
}

//=========================================================//
//			GetAttribute funkcje pomocnicze
//=========================================================//

namespace
{

/**@brief Konwertuje stringa do wartoœci liczbowych. Wykorzystuje funkcjê strol
i zwraca dok³adnie takie same wartoœci. Zobacz dokumentacjê do biblioteki standardowej c++.*/
template< typename ElementType >
inline ElementType Convert							( const char* valueBegin, char** checkEndPtr )
{
	assert( false );
	return false;
}

const char* convertCharNullTerminator = "\0";

template<>
inline const char*	Convert< const char* >			( const char* valueBegin, char** checkEndPtr )
{
	// @fixme To jest straszny hack. Gdyby ktoœ z zewn¹trz tej funkcji chcia³ porównaæ
	// zwracany wskaŸnik checkEndPtr ze wskaŸnikiem, który poda³, to by siê strasznie zdziwi³.
	// Ale jakoœ mimo wszystko nie chcê sprawdzaæ d³ugoœci tego stringa, kiedy to nie jest potrzebne.
	*checkEndPtr = const_cast<char*>( convertCharNullTerminator );
	return valueBegin;
}

template<>
inline std::string	Convert< std::string >			( const char* valueBegin, char** checkEndPtr )
{
	return Convert< const char* >( valueBegin, checkEndPtr );
}


template<>
inline uint32		Convert< uint32 >				( const char* valueBegin, char** checkEndPtr )
{	return strtoul( valueBegin, checkEndPtr, 10 );	}

template<>
inline int32		Convert< int32 >				( const char* valueBegin, char** checkEndPtr )
{	return strtol( valueBegin, checkEndPtr, 10 );	}

template<>
inline uint64		Convert< uint64 >				( const char* valueBegin, char** checkEndPtr )
{	return strtoull( valueBegin, checkEndPtr, 10 );	}

template<>
inline int64		Convert< int64 >				( const char* valueBegin, char** checkEndPtr )
{	return strtoll( valueBegin, checkEndPtr, 10 );	}


const char* trueString = "true";
const char* falseString = "false";

template<>
inline bool			Convert< bool >					( const char* valueBegin, char** checkEndPtr )
{
	*checkEndPtr = const_cast<char*>( valueBegin );
	const char* referenceString;
	bool returnValue;

	if( **checkEndPtr == 't' )
	{
		returnValue = true;
		referenceString = trueString;
	}
	else if( **checkEndPtr == 'f' )
	{
		returnValue = false;
		referenceString = falseString;
	}
	else
		return false;

	int i = 0;
	do
	{
		++i;
		*checkEndPtr += 1;
	} while( **checkEndPtr == *( referenceString + i ) && **checkEndPtr != '\0' );

	return returnValue;
}

template<>
inline double		Convert< double >				( const char* valueBegin, char** checkEndPtr )
{	return strtod( valueBegin, checkEndPtr );	}



template<typename Type>
inline Type		GetAttribTemplate( DeserializerImpl* impl, const char* name, Type& defaultValue )
{
	rapidxml::xml_node<>* currentNode = impl->valuesStack.top();

	rapidxml::xml_attribute<>* attribute = currentNode->first_attribute( name );
	if( attribute == nullptr )
		return defaultValue;

	const char* attribValue = attribute->value();
	char* checkEndPtr = nullptr;	// Po konwersji dostaniemy tutaj wskaŸnik na bajt tu¿ za koñcem parsowanego elementu

	errno = 0;	// Funkcje strto** mog¹ zwróciæ ERANGE w przypadku przekroczenia wartoœci.

	auto value = Convert<Type>( attribValue, &checkEndPtr );
	if( *checkEndPtr != '\0' || errno == ERANGE )	// Je¿eli przy parsowaniu stringa nie doszliœmy do koñca, to znaczy, ¿e pojawi³ siê b³¹d.
		return defaultValue;

	return value;
}

}	// anonymous


//=========================================================//
//			GetAttribute string
//=========================================================//


/**@brief Pobiera parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
std::string IDeserializer::GetAttribute( const std::string& name, std::string& defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Zwraca atrybut o podanej nazwie.

@attention Zwracany string przestanie istnieæ w momencie usuniêcia serializatora lub zmiany zawartoœci.

@param[in] name Nazwa atrybutu
@param[in] defaultValue Je¿eli element o podanej nazwie nie istnieje, zostanie zwrócona wartoœæ domyœlna.*/
const char* IDeserializer::GetAttribute( const std::string& name, const char* defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}


/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint32 IDeserializer::GetAttribute( const std::string& name, uint32 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint64 IDeserializer::GetAttribute( const std::string& name, uint64 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int32 IDeserializer::GetAttribute( const std::string& name, int32 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int64 IDeserializer::GetAttribute( const std::string& name, int64 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
bool IDeserializer::GetAttribute( const std::string& name, bool defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
double IDeserializer::GetAttribute( const std::string& name, double defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}


//=========================================================//
//			GetAttribute const char
//=========================================================//

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
std::string		IDeserializer::GetAttribute		( const char* name, std::string& defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
const char*		IDeserializer::GetAttribute		( const char* name, const char* defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint32			IDeserializer::GetAttribute		( const char* name, uint32 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint64			IDeserializer::GetAttribute		( const char* name, uint64 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int32			IDeserializer::GetAttribute		( const char* name, int32 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int64			IDeserializer::GetAttribute		( const char* name, int64 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
bool			IDeserializer::GetAttribute		( const char* name, bool defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
double			IDeserializer::GetAttribute		( const char* name, double defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

//====================================================================================//
//			Obs³uga b³êdów	
//====================================================================================//

/**@brief Zwraca string zawieraj¹cy b³¹d parsowania, je¿eli by³.

Aby siê dowiedzieæ czy parsowanie powiod³o siê, sprawdŸ wartoœæ zwracan¹ przez
funkcje @ref LoadFromString lub @ref LoadFromFile.*/
std::string IDeserializer::GetError()
{
	return impl->errorString;
}



