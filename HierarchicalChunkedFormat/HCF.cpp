/**
@file HCF.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swCommonLib/HierarchicalChunkedFormat/stdafx.h"

#include "HCF.h"




namespace sw
{

//====================================================================================//
//			Loading and saving	
//====================================================================================//

// ================================ //
//
			HCF::HCF			()
{}

// ================================ //
//
bool		HCF::LoadFile		( const filesystem::Path& filePath, bool readOnly )
{
	return m_impl.LoadFile( filePath, readOnly );
}

// ================================ //
//
bool		HCF::Load			( DataPtr data, Size size )
{
	return false;
}

// ================================ //
//
bool		HCF::WriteFile		( const filesystem::Path& filePath )
{
	return m_impl.WriteFile( filePath );
}

// ================================ //
//
bool		HCF::OpenFile		( const filesystem::Path& filePath, WriteMode mode )
{
	return m_impl.OpenFile( filePath, WriteMode::DirectToFile == mode ? true : false );
}

// ================================ //
//
Chunk		HCF::GetRootChunk	()
{
	return m_impl.GetRootChunk();
}

// ================================ //
//
Chunk		HCF::CreateRootChunk()
{
	return m_impl.CreateRootChunk();
}

// ================================ //
//
Attribute	HCF::AddAttribute		( AttributeType type, DataPtr data, Size dataSize )
{
	return m_impl.AddGlobalAttribute( type, data, dataSize );
}


}	// sw
