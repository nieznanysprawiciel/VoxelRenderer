#pragma once

#include "swCommonLib/Common//TypesDefinitions.h"

#include <vector>

/**@brief Klasa przechowuje pamiêæ o dowolnym przeznaczeniu.*/
class MemoryChunk
{
private:
protected:
	int8*			m_memory;
	uint32			m_size;
public:
	/**@brief Tworzy pusty obszar pamiêci.*/
	MemoryChunk()
		: m_memory( nullptr ),
		m_size( 0 )
	{}

	/**@brief Przejmuje na w³asnoœæ podany obszar pamiêci.*/
	MemoryChunk( int8*&& dataPointer, uint32 dataSize )
		:	m_memory( dataPointer ),
			m_size( dataSize )
	{}
	/**@brief alokuje obszar pamiêciu o podanym rozmiarze, nie inicjuj¹c go.*/
	MemoryChunk( uint32 dataSize )
		:	m_memory( new int8[dataSize] ),
			m_size( dataSize )
	{}

	//template<typename Type>
	//MemoryChunk( std::vector<Type>&& vector )
	//{
	//	//vector.
	//}

	/**Zwalnia pamiêæ po zasobach.*/
	~MemoryChunk()
	{
		delete[] m_memory;
	}

	MemoryChunk& operator=( const MemoryChunk& ) = delete;		///< Przypisanie obiektów nie ma sensu, bo mo¿e prowadziæ do dwukrotnego zwalniania pamiêci.
	MemoryChunk( const MemoryChunk& ) = delete;					///< Na razie nie mo¿na wykonywaæ kopii. Jedyna opcja to konstrukcja obiektu z semantyk¹ move.

	MemoryChunk( MemoryChunk&& chunk )				///< Tworzy obiekt kradn¹c pamiêæ z podanego w parametrze obiektu.
	{
		m_memory = chunk.m_memory;
		m_size = chunk.m_size;
		chunk.m_memory = nullptr;
		chunk.m_size = 0;
	}

	MemoryChunk& operator=( MemoryChunk&& chunk )	///< Operator przypisania z semantyk¹ move.
	{
		if( &chunk != this )
		{
			delete[] m_memory;
			m_memory = chunk.m_memory;
			m_size = chunk.m_size;
			chunk.m_size = 0;
			chunk.m_memory = nullptr;
		}
		return *this;
	}

	/**@brief Zwraca otypowany wskaŸnik na pamiêæ obiektu. @note Nie ma tu ¿adnej kontroli typów.*/
	template<typename Type>
	inline Type*			GetMemory		()		{ return reinterpret_cast<Type*>( m_memory ); }

	/**@brief Zwraca otypowany wskaŸnik na pamiêæ obiektu. @note Nie ma tu ¿adnej kontroli typów.*/
	template<typename Type>
	inline const Type*		GetMemory		() const	{ return reinterpret_cast<Type*>( m_memory ); }

	inline uint32			GetMemorySize	() const	{ return m_size; }			///<Zwraca rozmiar pamiêci przechowywanej w obiekcie.

	/**@brief Zwraca referencjê na obiekt w tablicy pod podanym indeksem.
	
	@attention Funkcja nie sprawdza czy indeks nie wychodzi poza pamiêæ w klasie.
	@param[in] index Indeks w tablicy. U¿ywana jest arytmetyka wskaŸnikowa dla typu Type.*/
	template<typename Type>
	inline Type&			Get				( uint32 index )
	{	return reinterpret_cast<Type*>( m_memory )[ index ];		}

	/**@brief Zwraca liczbê elementów w buforze o typie podanym w parametrze szablonu.
	@attention Funkcja nie sprawdza faktycznego typu bufora, bo go nie zna. U¿ytkownik jest odpowiedzialny za poprawne stosowanie.
	@return Zwraca rozmiar tablicy o typie Type.*/
	template<typename Type>
	inline uint32			Count			()
	{	return GetMemorySize() / sizeof( Type );	}	

	inline bool				IsNull			() const { return !m_memory; }		///< Sprawdza czy MemoryChunk ma zawartoœæ.

	/**@brief Kopiuje podany obszar pamiêci.

	@param[in] dataPointer WskaŸnik na pocz¹tek pamiêci do skopiowania.
	@param[in] dataSize Liczba bajtów pamiêci do skopiowania.
	*/
	inline void				MemoryCopy		( const int8* dataPointer, uint32 dataSize )
	{
		if( m_memory )
			delete[] m_memory;
		
		m_memory = new int8[ dataSize ];
		memcpy( m_memory, dataPointer, dataSize );
		m_size = dataSize;
	}

};


