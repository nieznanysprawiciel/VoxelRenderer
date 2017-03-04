#pragma once
/**
@file AttributeTypes.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


namespace sw
{

typedef uint64										AttributeType;
typedef uint8										DataUnderlayingType;
typedef DataUnderlayingType*						DataPtr;
typedef std::unique_ptr< DataUnderlayingType[] >	DataUPtr;


/**@brief Data pointer and it's size.*/
struct DataPack
{
	DataPtr			Data;
	Size			DataSize;

// ================================ //
//
	DataPack()
		: DataSize( 0 )
	{}
};


/**@brief Data pointer and it's size.
This struct is used to transfer ownership of pointer.*/
struct DataUPack
{
	DataUPtr		Data;
	Size			DataSize;

// ================================ //
//
	DataUPack()
		: DataSize( 0 )
	{}
};


enum AttributeTypeBuiltIn
{
	InvalidAttribute,
	ChunkName,
	Compression
};


}	// sw

