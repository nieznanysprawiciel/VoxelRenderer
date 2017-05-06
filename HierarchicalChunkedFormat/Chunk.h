#pragma once
/**
@file Chunk.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/HierarchicalChunkedFormat/Attribute.h"
#include "swCommonLib/HierarchicalChunkedFormat/Attributes/AttributeTypes.h"

namespace sw
{

class ChunkRepr;
DEFINE_PTR_TYPE( ChunkRepr );


/**@brief Represents memory chunk in file.

Chunk can consist of nested chunks or it can contain data.

@ingroup HierarchicalChunkedFormat*/
class Chunk
{
	FRIEND_CLASS_TESTER( HCF );
private:

	ChunkReprPtr		m_chunkPtr;

protected:
public:
	explicit		Chunk	() = default;
	explicit		Chunk	( ChunkReprPtr chunkRepr );
	~Chunk() = default;

	///@name Chunks manipulation
	///@{
	/**@brief Create child chunk.*/
	Chunk		CreateChunk		();

	/**@brief Gets next chunk on the same nesting level.
	@return Returns invalid chunk if there's no next chunk.*/
	Chunk		NextChunk		();

	/**@brief Gets first child chunk of this chunk.
	@return Returns invalid chunk if there's no children.*/
	Chunk		FirstChild		();

	/**@brief Check if you should call FirstChild or AccessData.*/
	bool		HasChildren		();

	/**@brief Get Chunks parent.
	@return Returned chunk can be invalid if this chunk is invalid or there's no parent.*/
	Chunk		ParentChunk		();

	///@}

	///@name Data access
	///@{

	/**@brief Fills chunk with data.
	You can fill only chunks without children. One filled chunk, can't add children anymore.*/
	bool		Fill			( const DataPtr data, Size dataSize );

	/**@brief Returns chunk's data and transfers ownership to caller.*/
	DataUPack	StealData		();

	/**@brief Returns chunk's data.
	@return Returns memory pointer and it's size but you are not owner of this pointer.
	You can use @ref Chunk::StealMamory function instead.*/
	DataPack	AccessData		();

	///@}

	///@name Attributes manipulation
	///@{
	/**@brief Adds attribute and fills it with data.*/
	Attribute		AddAttribute	( AttributeType type, const DataPtr data, Size dataSize );

	/**@brief Adds attribute and creates content from POD structure.
	@param[in] type You must provide type of attribute by yourself. Use second spetialization if struct
	defines attribute type getter.*/
	template< typename AttributeStruct >
	Attribute		AddAttribute	( AttributeType type, const AttributeStruct& content );

	/**@brief Adds attribute and creates content from POD structure.
	Attribute type will be taken from struct. Specialize GetAttributeTypeID template.*/
	template< typename AttributeStruct >
	Attribute		AddAttribute	( const AttributeStruct& content );
	///@}

	/**@brief Checks if you can use this attribute properly.*/
	bool		IsValid			() const;

	bool		operator==		( Chunk other ) const;
};

//====================================================================================//
//			Implementation	
//====================================================================================//


// ================================ //
//
template< typename AttributeStruct >
inline Attribute			Chunk::AddAttribute	( AttributeType type, const AttributeStruct& content )
{
	if( IsValid() )
		return m_chunkPtr->AddAttribute( type, (const DataPtr)&content, sizeof( AttributeStruct ) );
	return Attribute( nullptr );
}

// ================================ //
//
template< typename AttributeStruct >
inline Attribute			Chunk::AddAttribute	( const AttributeStruct& content )
{
	if( IsValid() )
		return AddAttribute( GetAttributeTypeID< AttributeStruct >(), content );
	return Attribute( nullptr );
}


}	// sw
