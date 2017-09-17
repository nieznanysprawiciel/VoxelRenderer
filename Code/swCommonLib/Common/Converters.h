#pragma once

#include <string>
#include <codecvt>
#include <type_traits>




class Convert
{
private:

	template< typename SrcType >
	static inline std::string			EnumToString		( SrcType defaultVal );

	template< typename SrcType >
	static inline SrcType				StringToEnum		( const std::string& value, SrcType defaultValue );

public:

	template< typename SrcType >
	static inline typename std::enable_if< std::is_enum< SrcType >::value, std::string >::type
										ToString			( const SrcType& val );

	template< typename SrcType >
	static inline typename std::enable_if< std::is_enum< SrcType >::value, SrcType >::type
										FromString			( const std::string& val, const SrcType& defaultValue );

	template< typename SrcType >
	static inline typename std::enable_if< !std::is_enum< SrcType >::value, std::string >::type
										ToString			( const SrcType& val );

	template< typename SrcType >
	static inline typename std::enable_if< !std::is_enum< SrcType >::value, SrcType >::type
										FromString			( const std::string& val, const SrcType& defaultValue );



	template<>
	static inline typename std::enable_if< !std::is_enum< std::wstring >::value, std::string >::type
										ToString< std::wstring >	( const std::wstring& value );

	template<>
	static inline typename std::enable_if< !std::is_enum< std::wstring >::value, std::wstring >::type
										FromString< std::wstring >	( const std::string& value, const std::wstring& defaultValue );

};


template< typename SrcType >
inline typename std::enable_if< !std::is_enum< SrcType >::value, std::string >::type
							Convert::ToString		( const SrcType& val )
{
	return std::to_string( val );
}

template< typename SrcType >
inline typename std::enable_if< !std::is_enum< SrcType >::value, SrcType >::type
							Convert::FromString		( const std::string& val, const SrcType& defaultValue )
{
	static_assert( false, "Specialize template" );
	return SrcType();
}


//====================================================================================//
//			Public template specialization for enum types
//====================================================================================//

/**@brief Konwertuje enuma do stringa.*/
template< typename SrcType >
inline typename std::enable_if< std::is_enum< SrcType >::value, std::string >::type
							Convert::ToString		( const SrcType& val )
{
	return EnumToString( val );
}

/**@brief Konwertuje stringa do enuma.*/
template< typename SrcType >
inline typename std::enable_if< std::is_enum< SrcType >::value, SrcType >::type
							Convert::FromString		( const std::string& val, const SrcType& defaultValue )
{
	return StringToEnum( val, defaultValue );
}

//====================================================================================//
//			Wstring to string	
//====================================================================================//

template<>
inline typename std::enable_if< !std::is_enum< std::wstring >::value, std::string >::type
							Convert::ToString< std::wstring >	( const std::wstring& value )
{
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes( value );
}

template<>
inline typename std::enable_if< !std::is_enum< std::wstring >::value, std::wstring >::type
							Convert::FromString< std::wstring >	( const std::string& value, const std::wstring& defaultValue )
{
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.from_bytes( value );
}

//====================================================================================//
//			Private	enum helpers
//====================================================================================//


template< typename SrcType >
std::string				Convert::EnumToString		( SrcType value )
{
	static_assert( std::is_enum< SrcType >::value, "Type is not enum" );

	auto type = rttr::type::get< SrcType >();
	
	assert( type.is_valid() );		/// Type haven't been registered.
	assert( type.is_enumeration() );
	
	rttr::enumeration enumVal = type.get_enumeration();

	return enumVal.value_to_name( value );
}


template< typename SrcType >
SrcType				Convert::StringToEnum		( const std::string& value, SrcType defaultValue )
{
	static_assert( std::is_enum< SrcType >::value, "Type is not enum" );

	auto type = rttr::type::get< SrcType >();

	assert( type.is_valid() );		/// Type haven't been registered.
	assert( type.is_enumeration() );

	rttr::enumeration enumVal = type.get_enumeration();

	rttr::variant result = enumVal.value_to_name( value );
	if( !result.is_valid() )
		return defaultValue;

	return result.get_value< SrcType >();
}




