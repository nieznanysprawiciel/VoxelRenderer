#include "swCommonLib/Serialization/Serializer.h"


#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <fstream>
#include <stack>


/**@defgroup SerializerJSON JSON Serializer
@ingroup Serialization
@brief Implementation of JSON serialization.

Project SerializerJSON contains implementation of JSON serialization based on RapidJSON library.
http://rapidjson.org/md_doc_tutorial.html*/




struct SerializerImpl
{
	rapidjson::Document				root;
	std::stack<rapidjson::Value>	valuesStack;
};

namespace
{

/// Helper
inline void SetAttributeHelper( SerializerImpl* impl, const std::string& name, rapidjson::Value& value )
{
	rapidjson::Value& currentObject = impl->valuesStack.top();	// Obiekt, do którego przyczepiamy atrybut.
	rapidjson::Value valueName;
	valueName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );

	if( currentObject.IsObject() )
		currentObject.AddMember( std::move( valueName ), std::move( value ), impl->root.GetAllocator() );
	else
		currentObject.PushBack( std::move( value ), impl->root.GetAllocator() );
}

inline void WriteToStreamBuffer( rapidjson::StringBuffer& buffer, rapidjson::Document& document, WritingMode mode )
{
	if( mode == WritingMode::Sparing )
	{
		rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
		document.Accept( writer );
	}
	else
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( buffer );
		document.Accept( writer );
	}
}

} // annonymous


/**@brief Konstruktor*/
ISerializer::ISerializer( std::unique_ptr< SerializationContext > serContext )
	: context( std::move( serContext ) )
{
	impl = new SerializerImpl;
	rapidjson::Value newObject( rapidjson::kObjectType );
	impl->valuesStack.push( std::move( newObject ) );
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
	while( impl->valuesStack.size() > 1 )
		this->Exit();

	rapidjson::Value& topValue = impl->valuesStack.top();
	rapidjson::Value& documentObject = impl->root.SetObject();
	documentObject = std::move( topValue );

	rapidjson::StringBuffer stringBuffer;
	WriteToStreamBuffer( stringBuffer, impl->root, mode );

	return stringBuffer.GetString();
}


/**@brief Zapisuje zserializowane dane do pliku.

@attention Po wykonaniu funkcji serializator wraca na zerowy poziom
zagnie¿d¿enia node'ów, je¿eli zapis do pliku zosta³ wykonany poprawnie.
Je¿eli funkcja zwróci³a false, to serializator nadal jest na tym samym poziomie.

@param[in] fileName Nazwa pliku docelowego.
@return Zwraca true, je¿eli zapisywanie powiedzie siê.*/
bool ISerializer::SaveFile( const std::string& fileName, WritingMode mode )
{
	std::ofstream file;
	file.open( fileName );
	if( !file.fail() )
	{
		while( impl->valuesStack.size() > 1 )
			this->Exit();

		rapidjson::Value& topValue = impl->valuesStack.top();
		rapidjson::Value& documentObject = impl->root.SetObject();
		documentObject = topValue.Move();

		rapidjson::StringBuffer stringBuffer;
		WriteToStreamBuffer( stringBuffer, impl->root, mode );

		file << stringBuffer.GetString();

		file.close();
		return true;
	}
	return false;
}

/**@brief Tworzy obiekt o podanej nazwie.

@param[in] name Nazwa obiektu.*/
void ISerializer::EnterObject( const std::string& name )
{
	rapidjson::Value newObject( rapidjson::kObjectType );

	// Nazwa jest potrzebna tylko, je¿eli przyczepiamy siê do obiektu.
	// W przypadku tablic wszystkie obiekty s¹ bez nazwy, wiêc unikamy alokacji pamiêci i wrzucamy pustego stringa.
	rapidjson::Value newName( rapidjson::kStringType );
	if( impl->valuesStack.top().IsObject() )
		newName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );

	// Wrzucamy nazwê jako pierwsz¹, a potem sam obiekt.
	impl->valuesStack.push( std::move( newName ) );
	impl->valuesStack.push( std::move( newObject ) );
}

/**@brief Koniec tworzenia obiektu.
EnterObject i ExitObject powinny byæ stosowane parami.*/
void ISerializer::Exit()
{
	rapidjson::Value& objectValue = impl->valuesStack.top();		// Obiekt, którego konstruowanie skonczyliœmy.
	impl->valuesStack.pop();
	rapidjson::Value& objectName = impl->valuesStack.top();			// Nazwa obiektu.
	impl->valuesStack.pop();

	rapidjson::Value& newCurrentObject = impl->valuesStack.top();
	if( newCurrentObject.IsObject() )
		newCurrentObject.AddMember( std::move( objectName ), std::move( objectValue ), impl->root.GetAllocator() );
	else if( newCurrentObject.IsArray() )
		newCurrentObject.PushBack( std::move( objectValue ), impl->root.GetAllocator() );
}

/**@brief Tworzy tablicê o podanej nazwie.

@param[in] name Nazwa tablicy.*/
void ISerializer::EnterArray( const std::string& name )
{
	rapidjson::Value newArray( rapidjson::kArrayType );

	// Nazwa jest potrzebna tylko, je¿eli przyczepiamy siê do obiektu.
	// W przypadku tablic wszystkie obiekty s¹ bez nazwy, wiêc unikamy alokacji pamiêci i wrzucamy pustego stringa.
	rapidjson::Value newName( rapidjson::kStringType );
	if( impl->valuesStack.top().IsObject() )
		newName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );

	// Wrzucamy nazwê jako pierwsz¹, a potem sam obiekt.
	impl->valuesStack.push( std::move( newName ) );
	impl->valuesStack.push( std::move( newArray ) );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, const std::string& value )
{
	rapidjson::Value& currentObject = impl->valuesStack.top();	// Obiekt, do którego przyczepiamy atrybut.
	rapidjson::Value newObject;
	rapidjson::Value valueName;
	valueName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );
	newObject.SetString( value.c_str(), (rapidjson::SizeType)value.length(), impl->root.GetAllocator() );

	if( currentObject.IsObject() )
		currentObject.AddMember( std::move( valueName ), std::move( newObject ), impl->root.GetAllocator() );
	else
		currentObject.PushBack( std::move( newObject ), impl->root.GetAllocator() );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, const char* value )
{
	rapidjson::Value& currentObject = impl->valuesStack.top();	// Obiekt, do którego przyczepiamy atrybut.
	rapidjson::Value newObject;
	rapidjson::Value valueName;
	valueName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );
	newObject.SetString( value, (rapidjson::SizeType)strlen( value ), impl->root.GetAllocator() );

	if( currentObject.IsObject() )
		currentObject.AddMember( std::move( valueName ), std::move( newObject ), impl->root.GetAllocator() );
	else
		currentObject.PushBack( std::move( newObject ), impl->root.GetAllocator() );
}


/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, uint32 value )
{
	rapidjson::Value newObject;
	newObject.SetUint( value );
	SetAttributeHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, uint64 value )
{
	rapidjson::Value newObject;
	newObject.SetUint64( value );
	SetAttributeHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, int32 value )
{
	rapidjson::Value newObject;
	newObject.SetInt( value );
	SetAttributeHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, int64 value )
{
	rapidjson::Value newObject;
	newObject.SetInt64( value );
	SetAttributeHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, bool value )
{
	rapidjson::Value newObject;
	newObject.SetBool( value );
	SetAttributeHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetAttribute( const std::string& name, double value )
{
	rapidjson::Value newObject;
	newObject.SetDouble( value );
	SetAttributeHelper( impl, name, newObject );
}


