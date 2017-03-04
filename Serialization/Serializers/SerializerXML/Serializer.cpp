#include "swCommonLib/Serialization/Serializer.h"

#include "swCommonLib/External/RapidXML/rapidxml.hpp"
#include "swCommonLib/External/RapidXML/rapidxml_print.hpp"

#include <fstream>
#include <stack>
#include <stdio.h>
#include <float.h>


/**@defgroup SerializerXML XML Serializer
@ingroup Serialization
@brief Implementation of XML serialization.

Project SerializerXML contains implementation of XML serialization based on RapidXML library.
http://rapidxml.sourceforge.net/*/




struct SerializerImpl
{
	rapidxml::xml_document<>			root;
	std::stack< rapidxml::xml_node<>* >	valuesStack;
};

namespace
{

/// Helper
inline void SetAttributeHelper( SerializerImpl* impl, const char* name, Size nameSize, const char* value, Size valueSize )
{
	char* attribName = impl->root.allocate_string( name, nameSize );
	char* attribValue = impl->root.allocate_string( value, valueSize );

	rapidxml::xml_attribute<>* attribute = impl->root.allocate_attribute( attribName, attribValue, nameSize, valueSize );
	auto currentNode = impl->valuesStack.top();
	currentNode->append_attribute( attribute );
}

} //anonymous

/**@brief Konstruktor*/
ISerializer::ISerializer( std::unique_ptr< SerializationContext > serContext )
	: context( std::move( serContext ) )
{
	impl = new SerializerImpl;
	impl->valuesStack.push( &impl->root );
}

/**@brief Destruktor*/
ISerializer::~ISerializer()
{ delete impl; }



/**@brief Zwraca stringa zawieraj¹cego zserializowanego Jsona.

@attention Po wykonaniu funkcji serializator wraca na zerowy poziom
zagnie¿d¿enia node'ów.

@param[in] mode Formatowanie stringa.*/
std::string	ISerializer::SaveString( WritingMode mode )
{
	int printingFlag = 0;
	if( mode == WritingMode::Sparing )
		printingFlag = rapidxml::print_no_indenting;

	while( impl->valuesStack.size() > 1 )
		this->Exit();

	std::string xmlString;
	rapidxml::print( std::back_inserter( xmlString ), impl->root, printingFlag );
	return std::move( xmlString );
}


/**@brief Zapisuje zserializowane dane do pliku.

@attention Po wykonaniu funkcji serializator wraca na zerowy poziom
zagnie¿d¿enia node'ów.

@param[in] fileName Nazwa pliku docelowego.
@return Zwraca true, je¿eli zapisywanie powiedzie siê.*/
bool ISerializer::SaveFile( const std::string& fileName, WritingMode mode )
{
	std::ofstream file;
	file.open( fileName, std::ios::out | std::ios::trunc );
	if( !file.fail() )
	{
		file << SaveString( mode );

		file.close();
		return true;
	}
	return false;
}

/**@brief Tworzy obiekt o podanej nazwie.

@param[in] name Nazwa obiektu.*/
void ISerializer::EnterObject( const std::string& name )
{
	char* nodeName = impl->root.allocate_string( name.c_str() );
	rapidxml::xml_node<>* node = impl->root.allocate_node( rapidxml::node_type::node_element, nodeName );
	impl->valuesStack.push( node );
}

/**@brief Koniec tworzenia obiektu.
EnterObject i ExitObject powinny byæ stosowane parami.*/
void ISerializer::Exit()
{
	rapidxml::xml_node<>* attachNode = impl->valuesStack.top();		// Obiekt, którego konstruowanie skonczyliœmy.
	impl->valuesStack.pop();
	rapidxml::xml_node<>* attachTo = impl->valuesStack.top();		// Nazwa obiektu.

	attachTo->append_node( attachNode );
}

/**@brief Tworzy tablicê o podanej nazwie.

@param[in] name Nazwa tablicy.*/
void ISerializer::EnterArray( const std::string& name )
{
	char* nodeName = impl->root.allocate_string( name.c_str() );
	rapidxml::xml_node<>* node = impl->root.allocate_node( rapidxml::node_type::node_element, nodeName );
	impl->valuesStack.push( node );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, const std::string& value )
{
	SetAttributeHelper( impl, name.c_str(), name.length(), value.c_str(), value.length() );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, const char* value )
{
	SetAttributeHelper( impl, name.c_str(), name.length(), value, strlen( value ) );
}


/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, uint32 value )
{
#define MAX_UINT32_SIGNS 11
	char numericString[ MAX_UINT32_SIGNS ];
	Size valueLength = sprintf_s( numericString, "%u", value );

	SetAttributeHelper( impl, name.c_str(), name.length(), numericString, valueLength );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, uint64 value )
{
#define MAX_UINT64_SIGNS 21
	char numericString[ MAX_UINT64_SIGNS ];
	Size valueLength = sprintf_s( numericString, "%llu", value );

	SetAttributeHelper( impl, name.c_str(), name.length(), numericString, valueLength );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, int32 value )
{
#define MAX_INT32_SIGNS 12
	char numericString[ MAX_INT32_SIGNS ];
	Size valueLength = sprintf_s( numericString, "%i", value );

	SetAttributeHelper( impl, name.c_str(), name.length(), numericString, valueLength );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, int64 value )
{
#define MAX_INT64_SIGNS 22
	char numericString[ MAX_INT64_SIGNS ];
	Size valueLength = sprintf_s( numericString, "%lli", value );

	SetAttributeHelper( impl, name.c_str(), name.length(), numericString, valueLength );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, bool value )
{
	if( value )
		SetAttributeHelper( impl, name.c_str(), name.length(), "true", 4 );
	else
		SetAttributeHelper( impl, name.c_str(), name.length(), "false", 5 );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, double value )
{
#define MAX_DOUBLE_SIGNS ( 3 + DBL_MANT_DIG - DBL_MIN_EXP )
	char numericString[ MAX_DOUBLE_SIGNS ];
	Size valueLength = sprintf_s( numericString, "%lf", value );

	SetAttributeHelper( impl, name.c_str(), name.length(), numericString, valueLength );
}

