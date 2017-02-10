#include "../../ResourcesFactory.h"

//#if ENABLE_RENDERER == DX11
	#include "DX11VertexShader.h"
	#include "DX11PixelShader.h"
	#include "DX11Buffer.h"
	#include "DX11Texture.h"

	typedef DX11Texture				Texture;
	typedef DX11VertexShader		VertexShader;
	typedef DX11PixelShader			PixelShader;
	typedef DX11Buffer				Buffer;
//#else
//	#error Nie zdefiniowano API graficznego. Zdefiniuj makro ENABLE_RENDERER w pliku macros_switches.h.
//#endif



TextureObject*			ResourcesFactory::CreateTextureFromFile( const std::wstring& fileName )
{
	Texture* newTexture = new Texture();
	bool result = newTexture->CreateFromFile( fileName );
	if( result )
		return newTexture;
	
	delete newTexture;		// Strata czasu, ¿e alokowaliœmy.
	return nullptr;
}

VertexShaderObject*		ResourcesFactory::CreateVertexShaderFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	VertexShader* newShader = new VertexShader();
	bool result = newShader->CreateFromFile( fileName, shaderName, shaderModel );
	if( result )
		return newShader;

	delete newShader;		// Strata czasu, ¿e alokowaliœmy.
	return nullptr;
}

PixelShaderObject*		ResourcesFactory::CreatePixelShaderFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	PixelShader* newShader = new PixelShader();
	bool result = newShader->CreateFromFile( fileName, shaderName, shaderModel );
	if( result )
		return newShader;

	delete newShader;		// Strata czasu, ¿e alokowaliœmy.
	return nullptr;
}

BufferObject*			ResourcesFactory::CreateBufferFormMemory( const void* buffer,
																  unsigned int elementSize,
																  unsigned int vertCount,
																  unsigned int bindFlag,
																  D3D11_USAGE usage = D3D11_USAGE_DEFAULT )
{
	Buffer* newBuffer = new Buffer();
	bool result = newBuffer->CreateFromFile( buffer, elementSize, vertCount, bindFlag, usage );
	if( result )
		return newBuffer;

	delete newBuffer;		// Strata czasu, ¿e alokowaliœmy.
	return nullptr;
}

