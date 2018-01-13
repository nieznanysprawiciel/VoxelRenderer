#include "ObjExporter.h"

#include "swCommonLib/Common/Converters.h"


#include <fstream>



namespace vr
{

// ================================ //
//
ObjExporter::ObjExporter( const TexturedMesh& mesh )
	:	m_mesh( mesh )
{}

// ================================ //
//
bool			ObjExporter::Export			( const filesystem::Path& outputFile )
{
	m_file.open( outputFile.String(), std::fstream::out );

	if( m_file.is_open() )
	{
		WriteHeader();

		WritePositions();
		WriteNormals();
		WriteUVs();

		WriteFaces();

		m_file.close();

		return true;
	}

	return false;
}

// ================================ //
//
void			ObjExporter::WriteHeader	()
{
	m_file << "# File created by FbxToObjConverter from VoxelRenderer." << std::endl;
	m_file << "# Author: nieznany.sprawiciel" << std::endl << std::endl;
}

// ================================ //
//
void			ObjExporter::WritePositions	()
{
	for( auto & vertex : m_mesh.Verticies )
	{
		WriteVertex( vertex.Position );
	}

	m_file << std::endl << std::endl;
}

// ================================ //
//
void			ObjExporter::WriteNormals	()
{
	for( auto & vertex : m_mesh.Verticies )
	{
		WriteNormal( vertex.Normal );
	}

	m_file << std::endl << std::endl;
}

// ================================ //
//
void			ObjExporter::WriteUVs		()
{
	for( auto & vertex : m_mesh.Verticies )
	{
		DirectX::XMFLOAT3 uv;
		uv.x = vertex.UV.x;
		uv.y = vertex.UV.y;
		uv.z = static_cast< float >( vertex.MaterialID );

		WriteUV( uv );
	}

	m_file << std::endl << std::endl;
}

// ================================ //
//
void			ObjExporter::WriteFaces		()
{
	for( auto& indexBuffer : m_mesh.Indicies )
	{
		for( Size i = 0; i < indexBuffer.size(); i += 3 )
		{
			WriteFace( indexBuffer[ i ], indexBuffer[ i + 1 ], indexBuffer[ i + 2 ] );
		}
	}
}

// ================================ //
//
void			ObjExporter::WriteVertex	( const DirectX::XMFLOAT3& position )
{
	m_file << "v ";
	WriteVector( position );
	m_file << std::endl;
}

// ================================ //
//
void			ObjExporter::WriteNormal	( const DirectX::XMFLOAT3& normal )
{
	m_file << "vn ";
	WriteVector( normal );
	m_file << std::endl;
}

// ================================ //
//
void			ObjExporter::WriteUV		( const DirectX::XMFLOAT3& uv )
{
	m_file << "vt ";
	WriteVector( uv );
	m_file << std::endl;
}

// ================================ //
//
void			ObjExporter::WriteVector	( const DirectX::XMFLOAT3& vector )
{
	m_file << Convert::ToString( vector.x ) << " " << Convert::ToString( vector.y ) << " " << Convert::ToString( vector.z );
}

// ================================ //
//
void			ObjExporter::WriteIndex		( Index32 idxToWrite )
{
	// Indicies start from 1.
	auto idx = idxToWrite + 1;

	// Index for vertex, normal and texture is the same.
	m_file << Convert::ToString( idx ) << "/" << Convert::ToString( idx ) << "/" << Convert::ToString( idx ) << " ";
}

// ================================ //
//
void			ObjExporter::WriteFace		( Index32 idx1, Index32 idx2, Index32 idx3 )
{
	m_file << "f ";

	WriteIndex( idx1 );
	WriteIndex( idx2 );
	WriteIndex( idx3 );

	m_file << std::endl;
}

}