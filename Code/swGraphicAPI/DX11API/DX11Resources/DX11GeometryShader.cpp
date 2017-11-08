/**
@file DX11PixelShader.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/DX11API/stdafx.h"

#include "DX11GeometryShader.h"

#include "swCommonLib/Common/MemoryLeaks.h"


//====================================================================================//
//			RTTR registration	
//====================================================================================//

RTTR_REGISTRATION
{
	rttr::registration::class_< DX11GeometryShader >( "DX11GeometryShader" );
}


//====================================================================================//
//			DX11PixelShader	
//====================================================================================//

/**@brief */
DX11GeometryShader::DX11GeometryShader( ID3D11GeometryShader* shader )
	:	m_geometryShader( shader )
{}

/**@brief */
DX11GeometryShader::~DX11GeometryShader()
{
	if( m_geometryShader )
		m_geometryShader->Release();
	m_geometryShader = nullptr;
}

/**@brief */
bool DX11GeometryShader::ReloadFromFile()
{

	return false;
}

/**@brief */
bool DX11GeometryShader::ReloadFromBinFile()
{

	return false;
}

/**@brief */
void DX11GeometryShader::SaveShaderBinFile( const std::wstring& fileName )
{
	assert( false );

}

/**@bref Tworzy obiekt DX11GeometryShader na podstawie pliku.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] fileName Nazwa pliku, z którego zostanie wczytany shader.
@param[in] shaderName Nazwa funkcji, która jest punktem poczatkowym wykonania shadera.
@param[in] shaderModel £añcuch znaków opisuj¹cy shader model.
@return Zwaraca wskaŸnik na DX11VertexShader lub nullptr w przypadku b³êdów wczytywania b¹dŸ kompilacji.
*/
DX11GeometryShader* DX11GeometryShader::CreateFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	HRESULT result;
	ID3DBlob* compiledShader;
	ID3D11GeometryShader* geometryShader;
	// Troszkê zamieszania, ale w trybie debug warto wiedzieæ co jest nie tak przy kompilacji shadera
#ifdef _DEBUG
	ID3D10Blob* error_blob = nullptr;	// Tu znajdzie siê komunikat o b³êdzie
#endif

	// Kompilujemy shader znaleziony w pliku
	D3DX11CompileFromFile( fileName.c_str(), 0, 0, shaderName.c_str(), shaderModel,
#ifdef _DEBUG						   
						   D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION,
#else
						   0,
#endif
						   0, 0, &compiledShader,
#ifdef _DEBUG
						   &error_blob,	// Funkcja wsadzi informacje o b³êdzie w to miejsce
#else
						   0,	// W trybie Release nie chcemy dostawaæ b³êdów
#endif
						   &result );

	if( FAILED( result ) )
	{
#ifdef _DEBUG
		if( error_blob )
		{
			// B³¹d zostanie wypisany na standardowe wyjœcie
			OutputDebugStringA( (char*)error_blob->GetBufferPointer() );
			error_blob->Release();
		}
#endif
		return nullptr;
	}

	// Tworzymy obiekt shadera na podstawie tego co sie skompilowa³o
	result = device->CreateGeometryShader( compiledShader->GetBufferPointer(),
										compiledShader->GetBufferSize(),
										nullptr, &geometryShader );

	if( FAILED( result ) )
	{
		compiledShader->Release();
		return nullptr;
	}

	// Tworzymy obiekt nadaj¹cy siê do u¿ycia w silniku i zwracamy wskaŸnik na niego
	DX11GeometryShader* newShader = new DX11GeometryShader( geometryShader );
	return newShader;
}

// ================================ //
//
DX11GeometryShader* DX11GeometryShader::CreateFromBinFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel)
{
	return nullptr;
}

