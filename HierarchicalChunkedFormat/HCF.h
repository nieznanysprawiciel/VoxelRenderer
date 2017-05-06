#pragma once
/**
@file HCF.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/System/Path.h"

#include "swCommonLib/HierarchicalChunkedFormat/Attributes/IAttribute.h"
#include "swCommonLib/HierarchicalChunkedFormat/Internal/ImplHCF.h"

#include "Chunk.h"
#include "Attribute.h"

#include <string>


namespace sw
{

class ImplHCF;


/**@defgroup HierarchicalChunkedFormat Hierarchical Chunked Format
@ingroup CommonLibrary
@brief Skeleton for writing and reading binary data from file in hierarchical maner.*/



/**@brief Main class for loading and writing HCF files.
@ingroup HierarchicalChunkedFormat*/
class HCF
{
	FRIEND_CLASS_TESTER( HCF );
public:

	enum WriteMode : uint8
	{
		DirectToFile,
		Indirect
	};

	const static bool		READ_ONLY				= true;
	const static bool		READ_WRITE				= false;
	const static Size		UserDefinedAttributes	= 0xFFFFFFFFFFFF0000;

private:
	ImplHCF			m_impl;

protected:
public:
	explicit		HCF			();
					~HCF		() = default;

///@name Loading and saving
///@{
	bool			LoadFile		( const filesystem::Path& filePath, bool readOnly );
	bool			Load			( DataPtr data, Size size );

	/**@brief Saves content to file.
	@see HCF::OpenFile*/
	bool			WriteFile		( const filesystem::Path& filePath );

	/**@brief Opens file for writing.
	This functions allows to use WriteMode::DirectToFile. If you use Indirect mode, there's no need to call this function.*/
	bool			OpenFile		( const filesystem::Path& filePath, WriteMode mode );
	///@}

	Chunk			GetRootChunk	();
	Chunk			CreateRootChunk	();

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

};



}	// sw

/**@brief Override this funtion for your attribute atructure.*/
template< typename AttributeStruct >
sw::AttributeType		GetAttributeTypeID		();


#include "HCF.inl"
