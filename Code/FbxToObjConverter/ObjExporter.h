#pragma once


#include "swCommonLib/System/Path.h"

#include "ShellMeshTools/ShellMesh/TexturedVertex.h"
#include "ShellMeshTools/Loader/FbxHelperStructs.h"


#include <fstream>


namespace vr
{

// ================================ //
//
class ObjExporter
{
private:

	const TexturedMesh&			m_mesh;
	
	std::fstream				m_file;

public:

	explicit		ObjExporter		( const TexturedMesh& mesh );


	bool			Export			( const filesystem::Path& outputFile );


private:

	void			WriteHeader		();

	void			WritePositions	();
	void			WriteNormals	();
	void			WriteUVs		();

	void			WriteFaces		();

private:

	void			WriteVertex		( const DirectX::XMFLOAT3& position );
	void			WriteNormal		( const DirectX::XMFLOAT3& normal );
	void			WriteUV			( const DirectX::XMFLOAT3& uv );

	void			WriteFace		( Index32 idx1, Index32 idx2, Index32 idx3 );

	void			WriteVector		( const DirectX::XMFLOAT3& vector );
	void			WriteIndex		( Index32 idx1 );
};



}


