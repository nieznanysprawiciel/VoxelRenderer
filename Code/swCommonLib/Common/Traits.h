#pragma once
/**
@file Traits.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.

Addtional traits extending STL library.
*/


#include <type_traits>


#include <string>

//====================================================================================//
//			is_string	
//====================================================================================//

//template< typename KeyType, typename std::enable_if< !std::is_same< KeyType, std::basic_string< typename KeyType::value_type > >::value, KeyType >::type >
//struct is_string
//{
//	static const bool value = false;
//};
//
//template< typename KeyType, typename std::enable_if< std::is_same< KeyType, std::basic_string< typename KeyType::value_type > >::value, typename KeyType::value_type >::type >
//struct is_string
//{
//	static const bool value = true;
//};
//
//
