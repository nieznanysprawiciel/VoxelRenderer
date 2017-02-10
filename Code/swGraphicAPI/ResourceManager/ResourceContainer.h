#pragma once

/**@file ResourceContainer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.


@brief Zawiera deklaracjê szblonu klasy kontenera dla assetów.
*/

///@todo Dodaæ IDki obok œcie¿ek.
///@todo Kontrner powinien obs³ugiwaæ œcie¿ki, jako obiekty typu Path zamiast wstringów.
//#include "swCommonLib/Common/System/Path.h"

#include <map>
#include <vector>
#include <type_traits>

#include "swGraphicAPI/Resources/ResourcePtr.h"


/**
@brief Szablon klasy do przechowywania assetów.

Wszystkie assety s¹ identyfikowane po nazwie, która najczêœciej jest nazw¹ pliku, z którego asset
pochodzi. Mapa zapewnia logarytmiczny czas dostêpu po nazwie. Istnieje te¿ mo¿liwoœæ odwo³ania siê
po identyfikatorze, wtedy czas dostêpu jest liniowy (chyba ¿e iterowanie po kolejnych elementacj mapy
nie odbywa siê liniowo.*/
template < class TYPE >
class ResourceContainer
{
	friend class AssetsManager;
	friend class ResourceManager;
private:
	unsigned int count;		///<Indentyfikator jaki zostanie przydzielony kolejnemy elementowi

protected:
	std::map<std::wstring, TYPE*> container;	///<Kontener zawieraj¹cy assety powiazane z ich nazw¹

	// Kasowanie obiektów
	int		ForceRemove			( const std::wstring& name );
	int		ForceRemove			( unsigned int id );
	void	ForceRemoveAll		();
	void	ReleaseMemory		( TYPE* );
public:
	ResourceContainer();
	~ResourceContainer();

	// Kasowanie obiektów
	int		Remove				( const std::wstring& name );
	int		Remove				( unsigned int id );
	int		RemoveUnused		();

	// Dodawanie obiektów
	void	UnsafeAdd			( const std::wstring& name, TYPE* resource );

	// Dostêp do obiektów
	TYPE*	get					( unsigned int id );
	inline unsigned int			GetNextId() { return count; }	///<Zwraca identyfikator, który zostanie przydzielony kolejnemu elementowi

	/**@brief Zwraca element na podstawie jego nazwy
	@param[in] name Nazwa elementu, który chcemy dostaæ
	@return WskaŸnik na obiekt assetu*/
	inline TYPE* get( const std::wstring& name )
	{
		auto iter = container.find( name );
		if ( iter != container.end() )
			return iter->second;
		return nullptr;
	}

	/**@brief Finds resource matching given descriptor.
	
	Resource must implement GetDescriptor function.
	Descriptor must implement operator==.
	
	Function finds resource in linear time. Use only for small containers.*/
	template< typename DescType >
	TYPE*	Find				( const DescType& desc );

	// Listowanie obiektów.
	std::vector< ResourcePtr< TYPE > >		List();
};

template <class TYPE>
ResourceContainer<TYPE>::ResourceContainer()
{
	count = 1;
}

/**@brief Destruktor zwalnia wszystkie elementy w mapie (tak¿e pamiêæ po nich)*/
template <class TYPE>
ResourceContainer<TYPE>::~ResourceContainer( )
{
	for ( auto iter = container.begin( ); iter != container.end( ); iter++ )
	{
		ReleaseMemory( iter->second );
	}
	container.clear();
}

/**@brief Zwraca element na podstawie identyfikatora.

Wyszukiwanie po identyfikatorze jest liniowe, a po nazwie logarytmiczne.
Jednak¿e porównania stringów mog¹ siê okazaæ bardziej kosztowne.
@param[in] id Identyfikator elementu.
@return WskaŸnik na poszukiwany element.*/
template <class TYPE>
TYPE*			ResourceContainer<TYPE>::get	( unsigned int id )
{
	for ( auto iter = container.begin(); iter != container.end(); iter++ )
	{
		if ( iter->second->GetID() == id )
			return iter->second;
	}
	return nullptr;
}

// ================================ //
//
template< class TYPE >
template< typename DescType >
inline TYPE*	ResourceContainer< TYPE >::Find	( const DescType& desc )
{
	static_assert( std::is_member_function_pointer< decltype( &TYPE::GetDescriptor ) >::value, "TYPE must implement GetDescriptor function." );

	for( auto& resource : container )
	{
		if( resource.second->GetDescriptor() == desc )
			return resource.second;
	}
	return nullptr;
}


//-------------------------------------------------------------------------------//
//							dodawanie obiektów
//-------------------------------------------------------------------------------//

/*Dodaje element do kontanera + nadaje mu unikalny identyfikator.
Je¿eli element ju¿ istnia³, to po prostu nie zostanie wstawiony, dlatego
przed uzyciem warto pobraæ element o danej nazwie, ¿eby sprawdziæ
czy dodawanie jest konieczne.*/
// Mo¿e kiedyœ zrobie wstawianie ze sprawdzaniem, na razie nie wydaje siê potrzebne


/**@brief Dodaje element do kontanera + nadaje mu unikalny identyfikator.

Je¿eli element ju¿ istnia³, to zostanie nadpisany nowym, dlatego nale¿y
zawsze przed u¿yciem sprawdziæ czy pod tak¹ nazw¹, coœ ju¿ siê nie 
znajduje.
@param[in] name Nazwa elementu, pod jak¹ zostanie dodany.
@param[in] resource Element dodania.*/
template <class TYPE>
void ResourceContainer<TYPE>::UnsafeAdd( const std::wstring& name, TYPE* resource )
{
	if ( !resource )
		return;	//Nie mo¿emy potem ustawiæ id

	container[name] = resource;

	resource->SetID( count );
	++count;			// Inkrementujemy licznik
}

//-------------------------------------------------------------------------------//
//							kasowanie obiektów
//-------------------------------------------------------------------------------//
/**@brief Zwalnia obiekt podany w parametrze.

Kasowanie pamiêci nie jest mo¿liwe przy pomocy operatora delete,
poniewa¿ destruktory w tych klasach s¹ prywatne. Dlatego trzeba zrobiæ to 
za poœrednictwem obiektu, który ma uprawnienia do tego.

@param[in] object Objekt do skasowania.
*/
template <class TYPE>
void ResourceContainer<TYPE>::ReleaseMemory( TYPE* object )
{
	// Destruktor jest prywatny, wiêc nie mo¿emy kasowaæ obiektu bezpoœrednio.
	ObjectDeleterKey<TYPE> key;							// Tworzymy klucz.
	ObjectDeleter<TYPE> model_deleter( key );			// Tworzymy obiekt kasuj¹cy i podajemy mu nasz klucz.
	model_deleter.delete_object( object );				// Kasujemy obiekt za poœrednictwem klucza.
}

/**@brief Usuwa element o podanej nazwie, je¿eli nie ma do niego odwo³añ.

@param[in] name nazwa elementu do usuniêcia.
@return Zwracana wartoœæ:
- 0	-	w przypadku powodzenia,
- -1	-	nie znaleziono elementu,
- 1	-	nie da siê usun¹æ, bo jest w u¿yciu*/
template <class TYPE>
int ResourceContainer<TYPE>::Remove( const std::wstring& name )
{
	auto iter = container.find( name );
	if ( iter != container.end() )
		return -1;		// Nie znaleŸliœmy elementu

	if ( !iter->second->CanDelete() )
		return 1;		// Nie mo¿emy skasowaæ, bo s¹ odwo³ania

	ReleaseMemory( iter->second );		// Zwalniamy pamiêæ spod wskaŸnika
	container.erase( iter );	// Kasujemy element z mapy

	return 0;			// Wychodzimy z powodzeniem
}


/**@brief Usuwa element o podanym indeksie, je¿eli nie ma do niego odwo³añ.

@param[in] id Identyfikator elementu
@return Zwracana wartoœæ:
- 0	-	w przypadku powodzenia,
- -1	-	nie znaleziono elementu,
- 1	-	nie da siê usun¹æ, bo jest w u¿yciu*/
template <class TYPE>
int ResourceContainer<TYPE>::Remove( unsigned int id )
{
	for ( auto iter = container.begin( ); iter != container.end( ); iter++ )
	{
		if ( iter->second->GetID() == id )
		{
			// Sprawdzamy czy nie ma jakichœ odwo³añ do obiektu
			if ( !iter->second->CanDelete() )
				return 1;				// S¹ odwo³ania, wiêc nie kasujemy

			ReleaseMemory( iter->second );		// Zwalniamy pamiêæ spod wskaŸnika
			container.erase( iter );	// Kasujemy element z mapy

			return 0;					// Zwracamy 0 jako powodzenie operacji
		}
	}
	return -1;		// Nie znaleŸliœmy elementu
}

/**@brief Kasuje wszystkie elementy w kontenerze, które nie s¹ u¿ywane przez
¿aden obiekt. Kasowanie jest w pe³ni bezpieczne.

@return Zwraca liczbê usuniêtych elementów.*/
template <class TYPE>
int ResourceContainer<TYPE>::RemoveUnused()
{
	int count = 0;
	for ( auto iter = container.begin(); iter != container.end(); iter++ )
	{// Iterujemy po ca³ej mapie
		if ( iter->second->CanDelete() )
		{
			// Mo¿emy skasowaæ obiekt, bo nikt go nie u¿ywa
			ReleaseMemory( iter->second );		// Zwalniamy pamiêæ spod wskaŸnika
			container.erase( iter );	// Kasujemy element z mapy

			++count;
		}
	}

	return count;
}


/**@brief Wymusza skasowanie podanego elementu, nawet je¿eli jest u¿ywany

@param[in] name Nazwa elementu do usuniêcia.
@return Zwracana wartoœæ:
- 0	-	w przypadku powodzenia,
- -1	-	nie znaleziono elementu*/
template <class TYPE>
int ResourceContainer<TYPE>::ForceRemove( const std::wstring& name )
{
	auto iter = container.find( name );
	if ( iter != container.end( ) )
		return -1;		// Nie znaleŸliœmy elementu

	delete iter->second;		// Zwalniamy pamiêæ spod wskaŸnika
	return 0;
}

/**@brief Wymusza skasowanie podanego elementu

@param[in] id Identyfkator elementu do usuniêcia
@return Zwracana wartoœæ:
- 0	-	w przypadku powodzenia,
- -1	-	nie znaleziono elementu*/
template <class TYPE>
int ResourceContainer<TYPE>::ForceRemove( unsigned int id )
{
	for ( auto iter = container.begin( ); iter != container.end( ); iter++ )
	{
		if ( iter->second->GetID( ) == id )
		{
			delete iter->second;		// Zwalniamy pamiêæ spod wskaŸnika
			return 0;
		}
	}
	return -1;		// Nie znaleziono elementu
}

/**@brief Kasuje wszystkie elementy niezale¿nie od tego czy by³y u¿ywane,
a nastêpnie czyœci mapê.*/
template <class TYPE>
void ResourceContainer<TYPE>::ForceRemoveAll( )
{
	for ( auto iter = container.begin(); iter != container.end(); iter++ )
	{// Iterujemy po ca³ej mapie
		delete iter->second;		// Zwalniamy pamiêæ spod wskaŸnika
	}
	container.clear();
}

/**@brief Listuje wszystkie assety danego typu.*/
template< class TYPE >
inline std::vector< ResourcePtr< TYPE > > ResourceContainer< TYPE >::List()
{
	std::vector< ResourcePtr< TYPE > > resourcesList;
	resourcesList.reserve( container.size() );

	for( auto iter = container.begin(); iter != container.end(); iter++ )
	{
		resourcesList.push_back( ResourcePtr< TYPE >( iter->second ) );
	}

	return resourcesList;
}

