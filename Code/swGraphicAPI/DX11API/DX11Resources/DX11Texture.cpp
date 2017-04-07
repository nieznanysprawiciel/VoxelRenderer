/**
@file DX11Texture.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/DX11API/stdafx.h"


#include "DX11Texture.h"
#include "DX11Initializer/DX11ConstantsMapper.h"


#include "swCommonLib/Common/MemoryLeaks.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< DX11Texture >( "DX11Texture" );
}




// ================================ //
//
void DX11Texture::Construct()
{
	if( IsDebugLayerEnabled() )
	{	
		SetDebugName( m_texture.Get(), m_descriptor.FilePath.String() );
		SetDebugName( m_textureView.Get(), m_descriptor.FilePath.String() );
	}
}


/**@brief Remember to release tex and texView (Call com interface Release method)*/
DX11Texture::DX11Texture( TextureInfo&& texInfo, ID3D11Texture2D* tex, ID3D11ShaderResourceView* texView )
	:	m_texture( tex )
	,	m_textureView( texView )
	,	m_descriptor( std::move( texInfo ) )
{
	Construct();
}


// ================================ //
//
DX11Texture::DX11Texture( TextureInfo&& texInfo, ComPtr< ID3D11Texture2D > tex, ComPtr< ID3D11ShaderResourceView > texView )
	:	m_texture( tex )
	,	m_textureView( texView )
	,	m_descriptor( std::move( texInfo ) )
{
	Construct();
}

// ================================ //
//
DX11Texture::~DX11Texture()
{
	m_texture = nullptr;
	m_textureView = nullptr;
}


/**@copydoc TextureObject::GetDescriptor.*/
const TextureInfo&			DX11Texture::GetDescriptor() const
{
	return m_descriptor;
}

/**@copydoc TextureObject::GetFilePath.*/
const filesystem::Path&		DX11Texture::GetFilePath() const
{
	return m_descriptor.FilePath;
}


/**@brief Tworzy teksturê z podanego wskaŸnika.

@return Zawraca stworzony wewn¹trz obiekt DX11Texture z wczytan¹ tekstur¹ lub nullptr w przypadku niepowodzenia.*/
DX11Texture*	DX11Texture::CreateFromMemory( const MemoryChunk& texData, TextureInfo&& texInfo )
{
	if( texData.IsNull() )
		return nullptr;

	/// @todo Trzeba zacz¹æ kiedyœ obœ³ugiwac inne typy tekstur.
	assert( texInfo.TextureType == TextureType::TEXTURE_TYPE_TEXTURE2D );
	if( texInfo.TextureType != TextureType::TEXTURE_TYPE_TEXTURE2D )
		return nullptr;

	ComPtr< ID3D11Texture2D > texture = nullptr;
	ComPtr< ID3D11ShaderResourceView > texView = nullptr;
	D3D11_TEXTURE2D_DESC texDesc = FillDesc( texInfo );

	std::unique_ptr< D3D11_SUBRESOURCE_DATA[] > initData( new D3D11_SUBRESOURCE_DATA[ texInfo.MipMapLevels /** texInfo.ArraySize*/ ] );
	uint16 mipWidth = texInfo.TextureWidth;
	uint16 mipHeight = texInfo.TextureHeight;
	PtrOffset offset = 0;

	for( int level = 0; level < texInfo.MipMapLevels; level++ )
	{
		initData[ level ].pSysMem = texData.GetMemory< int8 >() + offset;
		initData[ level ].SysMemPitch = (uint32)mipWidth * BytesPerPixel( texInfo.Format );

		offset += mipWidth * mipHeight * BytesPerPixel( texInfo.Format );

		mipWidth /= 2;
		mipHeight /= 2;
		if( mipHeight == 0 )	mipHeight = 1;
		if( mipWidth == 0 )		mipWidth = 1;
	}

	HRESULT result = device->CreateTexture2D( &texDesc, initData.get(), &texture );
	if( result == S_OK )
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

		if( texInfo.TextureType == TextureType::TEXTURE_TYPE_TEXTURE2D )
		{
			viewDesc.Format = texDesc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MostDetailedMip = 0;
			viewDesc.Texture2D.MipLevels = texInfo.MipMapLevels;

			result = device->CreateShaderResourceView( texture.Get(), &viewDesc, &texView );
			if( result == S_OK )
				return new DX11Texture( std::move( texInfo ), texture, texView );
			else 
				return nullptr;
		}
		
		assert( !"Other texture types are not suported" );
	}

	return nullptr;
}


/**@brief Zwraca zawartoœæ tekstury.

@attention Funkcja wywo³uje metodê DeviceContextu. Mo¿e byæ problem z synchronizacj¹, je¿eli
wiele w¹tków bêdzie wywo³ywa³o jakieœ funkcje z kontekstu jednoczeœnie.*/
MemoryChunk					DX11Texture::CopyData() const
{
	D3D11_TEXTURE2D_DESC texDesc = FillDesc( m_descriptor );
	texDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_READ;
	texDesc.Usage			= D3D11_USAGE::D3D11_USAGE_STAGING;

	ID3D11Texture2D* newTex;

	HRESULT result = device->CreateTexture2D( &texDesc, nullptr, &newTex );
	if( FAILED( result ) )
		return MemoryChunk();


	// Kopiowanie zawartoœci miêdzy buforami
	device_context->CopyResource( newTex, m_texture.Get() );

	assert( m_descriptor.MemorySize != 0 );

	MemoryChunk memoryChunk( m_descriptor.MemorySize );
	PtrOffset offset = 0;

	for( int level = 0; level < m_descriptor.MipMapLevels; level++ )
	{
		D3D11_MAPPED_SUBRESOURCE data;
		result = device_context->Map( newTex, level, D3D11_MAP::D3D11_MAP_READ, 0, &data );
		if( FAILED( result ) )
			return MemoryChunk();

		memcpy( memoryChunk.GetMemory< uint8 >() + offset, data.pData, data.RowPitch << ( m_descriptor.MipMapLevels - level - 1 ) );

		device_context->Unmap( newTex, level );
	}

	
	newTex->Release();

	return std::move( memoryChunk );
}

/**@brief Tworzy deskryptor tekstury 2D na podstawie deskryptora.*/
D3D11_TEXTURE2D_DESC			DX11Texture::FillDesc	( const TextureInfo& texInfo )
{
	D3D11_TEXTURE2D_DESC texDesc;

	unsigned int ArraySize = 1;
	if( texInfo.TextureType == TextureType::TEXTURE_TYPE_TEXTURE2D_ARRAY || texInfo.TextureType == TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE_ARRAY )
		ArraySize = texInfo.ArraySize;

	texDesc.Width				= texInfo.TextureWidth;
	texDesc.Height				= texInfo.TextureHeight;
	texDesc.MipLevels			= texInfo.MipMapLevels;
	texDesc.Usage				= DX11ConstantsMapper::Get( texInfo.Usage );
	texDesc.ArraySize			= ArraySize;
	
	
	assert( texInfo.TextureType != TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE && texInfo.TextureType != TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE_ARRAY );
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	
	texDesc.MiscFlags = 0;
	if( texInfo.IsCubeMap )
		texDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	if( texInfo.AllowShareResource )
		texDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED ;

	texDesc.CPUAccessFlags = 0;
	if( texInfo.CPURead )
		texDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
	if( texInfo.CPUWrite )
		texDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;


	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.Format = DX11ConstantsMapper::Get( texInfo.Format );

	return texDesc;
}


// ================================ //
//
bool						DX11Texture::UpdateData			( uint8* dataPtr, uint16 mipLevel, uint16 arrayIdx )
{
	// Texture must be updatable.
	if( m_descriptor.Usage == ResourceUsage::RESOURCE_USAGE_STATIC ||
		m_descriptor.Usage == ResourceUsage::RESOURCE_USAGE_STAGING )
		return false;

	if( mipLevel > m_descriptor.MipMapLevels ||
		mipLevel < 1 )
		return false;

	if( m_descriptor.ArraySize > 1 )
	{
		// If texture is textures array, check array index.
		if( m_descriptor.ArraySize < arrayIdx )
			return false;
	}
	else
	{
		// If texture is not an array set array index to first array element.
		arrayIdx = 1;
	}

	// mipLevel == 1 have index 0.
	uint16 subresourceIdx = ( mipLevel - 1 ) + m_descriptor.MipMapLevels * ( arrayIdx - 1 );

	D3D11_MAPPED_SUBRESOURCE updateData;
	HRESULT result = device_context->Map( m_texture.Get(), subresourceIdx, D3D11_MAP_WRITE_DISCARD, 0, &updateData );
	if( SUCCEEDED( result ) )
	{
		memcpy( updateData.pData, dataPtr, MipLevelSize( mipLevel ) );	
		device_context->Unmap( m_texture.Get(), subresourceIdx );

		return true;
	}

	return false;
}

//====================================================================================//
//			Internal helpers	
//====================================================================================//


// ================================ //
//
uint16			DX11Texture::MipWidth		( uint16 mipLevel ) const
{
	if( mipLevel > m_descriptor.MipMapLevels )
		return 0;

	uint32 mipDivider = 0x1 << ( mipLevel - 1 );	// MipLevel = 1 means original texture level.
	return m_descriptor.TextureWidth / mipDivider;
}

// ================================ //
//
uint16			DX11Texture::MipHeight		( uint16 mipLevel ) const
{
	if( mipLevel > m_descriptor.MipMapLevels )
		return 0;

	uint32 mipDivider = 0x1 << ( mipLevel - 1 );	// MipLevel = 1 means original texture level.
	return m_descriptor.TextureWidth / mipDivider;
}

// ================================ //
//
uint32			DX11Texture::MipRowSize		( uint16 mipLevel ) const
{
	uint32 bytesPerPix = (uint32)DX11ConstantsMapper::BytesPerPixel( DX11ConstantsMapper::Get( m_descriptor.Format ) );
	return MipWidth( mipLevel ) * bytesPerPix;
}

// ================================ //
//
uint32			DX11Texture::MipLevelSize	( uint16 mipLevel ) const
{
	return MipRowSize( mipLevel ) * MipHeight( mipLevel );
}


