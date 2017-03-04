#pragma once

/**@file TypesDefinitions.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje podstawowych typów zmiennych.

Zmienne s¹ maj¹ sta³y rozmiar, powinny byæ mniej wiêcej niezale¿ne od platformy bo
opieraj¹ siê na bibliotece standardowej. Mimo wszystko moze siê zdarzyæ, ¿e na jakichœ
platformach typy nie istniej¹ i wtedy jest problem.

Definicje s¹ stworzone w celu ³atwiejszego u¿ytku. Biblioteka standardowa definiuje np.
typ int8_t, który jest zamieniany na int8.
*/

/**@defgroup CommonLibrary Common Library
@ingroup ModulesStructure
@brief Czêsto u¿ywane deklaracje i inne funkcjonalnoœci.*/

#include <cstdint>
#include <memory>



typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef size_t Size;

typedef ptrdiff_t PtrOffset;

typedef double TimeType;


namespace rttr
{
	class type;
}


typedef rttr::type ActorType;

/// @todo Use std::unique_ptr instead. Unique_ptr didn't want to compile in current visual compiler.
template< typename PtrType >
using UPtr = std::shared_ptr< PtrType >;

template< typename PtrType >
using Ptr = std::shared_ptr< PtrType >;

/// Declares that class holding this pointer is it's owner. Note: there's only semantical difference between OPtr
/// and UPtr. UPtr should be unique in whole system. OPtr says that, there can exist some raw pointers, but class
/// holding this pointer is responsible for releasing memory.
template< typename PtrType >
using OPtr = std::unique_ptr< PtrType >;

template< typename PtrType, typename... Args >
Ptr< PtrType >		MakePtr		( Args&&... args )
{
	return std::make_shared< PtrType >( args... );
}

template< typename PtrType, typename... Args >
UPtr< PtrType >		MakeUPtr		( Args... args )
{
	return std::make_unique< PtrType >( args... );
}


template< typename PtrType, typename... Args >
UPtr< PtrType >		MakeOPtr		( Args... args )
{
	return std::make_unique< PtrType >( args... );
}


#define DEFINE_PTR_TYPE( type )			typedef Ptr< type > type ## Ptr;
#define DEFINE_UPTR_TYPE( type )		typedef UPtr< type > type ## UPtr;
#define DEFINE_WPTR_TYPE( type )		typedef std::weak_ptr< type > type ## WPtr;
#define DEFINE_OPTR_TYPE( type )		typedef OPtr< type > type ## OPtr;

