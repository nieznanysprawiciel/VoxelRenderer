#pragma once

#include "DX11Initializer/DX11APIObjects.h"
#include "swGraphicAPI/Resources/MeshResources.h"


/**@brief Implementacja tekstury w DirectX 11.
@ingroup DX11API*/
class DX11Texture	:	public TextureObject, protected DX11APIObjects
{
	RTTR_ENABLE( TextureObject )
private:
	ComPtr< ID3D11ShaderResourceView >		m_textureView;
	ComPtr< ID3D11Resource >				m_texture;

	TextureInfo								m_descriptor;
protected:
	~DX11Texture();
public:

	explicit								DX11Texture				( TextureInfo&& texInfo, ComPtr< ID3D11Resource > tex, ComPtr< ID3D11ShaderResourceView > texView );

	virtual bool							UpdateData				( uint8* dataPtr, uint16 mipLevel, uint16 arrayIdx ) override;
	virtual MemoryChunk						CopyData				() const override;
	virtual const TextureInfo&				GetDescriptor			() const override;

	virtual const filesystem::Path&			GetFilePath				() const override;

	static DX11Texture*						CreateFromMemory		( const MemoryChunk& texData, TextureInfo&& texInfo );

	inline ID3D11ShaderResourceView*		Get			()			{ return m_textureView.Get(); }
	inline ID3D11Texture2D*					GetTex		();
	static D3D11_TEXTURE2D_DESC				FillDesc	( const TextureInfo& texInfo );

private:

	uint16					MipWidth				( uint16 mipLevel ) const;
	uint16					MipHeight				( uint16 mipLevel ) const;
	uint32					MipRowSize				( uint16 mipLevel ) const;
	uint32					MipLevelSize			( uint16 mipLevel ) const;


private:
	void		Construct	();
};


//====================================================================================//
//			Inline Implementation	
//====================================================================================//

// ================================ //
//
inline ID3D11Texture2D*			DX11Texture::GetTex()
{
	if( m_descriptor.TextureType != TextureType::TEXTURE_TYPE_TEXTURE2D )
	{
		assert( false );
		return nullptr;
	}
	return static_cast< ID3D11Texture2D* >( m_texture.Get() );
}
