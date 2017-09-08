/**
@file DX11Renderer.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/DX11API/stdafx.h"


#include "DX11Renderer.h"

#include "DX11Initializer/DX11ConstantsMapper.h"
#include "DX11Resources/DX11InputLayout.h"
#include "DX11Resources/DX11Buffer.h"
#include "DX11Resources/DX11VertexShader.h"
#include "DX11Resources/DX11PixelShader.h"
#include "DX11Resources/DX11Texture.h"
#include "DX11Resources/DX11RenderTarget.h"

#include "swCommonLib/Common/MemoryLeaks.h"



#define ThrowIfNull( ptr, message )				\
if( !ptr )										\
{												\
	assert( false );							\
	return;										\
}
//throw new std::runtime_error( message );


/**@brief Klasa bêdzie renderowaæ w trybie immediate albo deferred.

Je¿eli ustawiono flagê D3D11_CREATE_DEVICE_SINGLETHREADED, to nie wolno podaæ w parametrze
RendererUsage::USE_AS_DEFERRED.

Mo¿na sprawdziæ czy klasa zosta³a zainicjowana poprawnie wywo³uj¹c funkcjê isValid.*/
DX11Renderer::DX11Renderer( RendererUsage usage )
{
	m_usageType = usage;
	if ( usage == RendererUsage::USE_AS_IMMEDIATE )
	{
		m_localDeviceContext = device_context;
		m_valid = true;
	}
	else if ( usage == RendererUsage::USE_AS_DEFERRED )		// Will not work, if the device was created with the D3D11_CREATE_DEVICE_SINGLETHREADED value.
	{
		device->CreateDeferredContext( 0, &m_localDeviceContext );
		m_valid = true;
	}
	else
	{
		m_valid = false;
		assert( false );
	}
}

// ================================ //
//
DX11Renderer::~DX11Renderer()
{
	if ( m_usageType == RendererUsage::USE_AS_DEFERRED )	/// Tylko w takim wypadku alokowaliœmy nowy kontekst. Inaczej zwalanianie nale¿y do klasy DX11_interfaces_container.
		if ( m_localDeviceContext )
			m_localDeviceContext->Release();
}



//====================================================================================//
//			Drawing commands
//====================================================================================//

// ================================ //
//
void	DX11Renderer::Draw				( const DrawCommand& command )
{
	ID3D11Buffer* indexBuffer = nullptr;
	if( command.IndexBufer )
		indexBuffer = DX11( command.IndexBufer )->Get();

	if( command.Layout )
		m_localDeviceContext->IASetInputLayout( DX11( command.Layout )->Get() );
	else
		m_localDeviceContext->IASetInputLayout( nullptr );

	m_localDeviceContext->IASetPrimitiveTopology( DX11ConstantsMapper::Get( command.Topology ) );
	

	DX11Renderer::SetVertexBuffer( command.VertexBuffer, 0 );
	SetIndexBuffer( command.IndexBufer, 0, command.ExtendedIndex );

	if( indexBuffer )
		m_localDeviceContext->DrawIndexed( command.NumVertices, command.BufferOffset, command.BaseVertex );
	else
		m_localDeviceContext->Draw( command.NumVertices, command.BufferOffset );
}

// ================================ //
//
void	DX11Renderer::DrawInstanced		( const DrawInstancedCommand& command )
{
	auto indexBuffer = DX11( command.IndexBufer )->Get();
	auto layout = DX11( command.Layout )->Get();

	m_localDeviceContext->IASetPrimitiveTopology( DX11ConstantsMapper::Get( command.Topology ) );
	m_localDeviceContext->IASetInputLayout( layout );

	SetIndexBuffer( command.IndexBufer, 0, command.ExtendedIndex );

	ID3D11Buffer* inputBuffer[ 2 ];
	inputBuffer[ 0 ] = DX11( command.VertexBuffer )->Get();
	inputBuffer[ 1 ] = DX11( command.PerInstanceBuffer )->Get();

	uint32 strides[ 2 ] = { command.VertexBuffer->GetStride(), command.PerInstanceBuffer->GetStride() };
	uint32 offsets[ 2 ] = { 0, 0 };

	m_localDeviceContext->IASetVertexBuffers( 0, 2, inputBuffer, strides, offsets );

	if( indexBuffer )
		m_localDeviceContext->DrawIndexedInstanced( command.NumVertices, command.NumInstances, command.BufferOffset, command.BaseVertex, 0 );
	else
		m_localDeviceContext->DrawInstanced( command.NumVertices, command.NumInstances, command.BufferOffset, 0 );
}

//====================================================================================//
//			Render target commands	
//====================================================================================//

// ================================ //
//
void	DX11Renderer::SetRenderTarget	( const SetRenderTargetCommand& command )
{
	// Prevent from binding texture to both pipeline and render target.
	ClearTextures();

	SetRenderTarget( command.RenderTargets, command.DepthStencil );

	assert( command.RenderTargets[ 0 ] );

	DX11RenderTarget* renderTarget = DX11( command.RenderTargets[ 0 ] );

	D3D11_VIEWPORT viewport;
	viewport.Width = renderTarget->GetWidth();
	viewport.Height = renderTarget->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	device_context->RSSetViewports( 1, &viewport );
	device_context->RSSetScissorRects( 0, nullptr );

	float blendFactor[ 4 ] = { 0, 0, 0, 0 };

	device_context->RSSetState( DX11( command.RasterizerState )->Get() );
	device_context->OMSetBlendState( DX11( command.BlendingState )->Get(), blendFactor, 0xffffffff );
	device_context->OMSetDepthStencilState( DX11( command.DepthStencilState )->Get(), 0 );

	auto cameraBuffer = DX11( command.CameraBuffer );
	auto lightBuffer = DX11( command.LightBuffer );

	if( cameraBuffer )
	{
		ID3D11Buffer* directXBuffer = cameraBuffer->Get();
		m_localDeviceContext->VSSetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
		m_localDeviceContext->PSSetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}

	if( lightBuffer )
	{
		ID3D11Buffer* directXBuffer = lightBuffer->Get();
		m_localDeviceContext->PSSetConstantBuffers( LIGHTS_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}
}

// ================================ //
//
void	DX11Renderer::SetRenderTarget	( const SetRenderTargetExCommand& command )
{
	// Prevent from binding texture to both pipeline and render target.
	ClearTextures();

	SetRenderTarget( command.RenderTargets, command.DepthStencil );

	if( command.NumViews > MAX_BOUND_RENDER_TARGETS )
	{
		assert( false );
		//throw new std::runtime_error( "Trying bind to many viewports or scissor rectangles." );
		return;
	}

	D3D11_VIEWPORT viewport[ MAX_BOUND_RENDER_TARGETS ];
	D3D11_RECT scissors[ MAX_BOUND_RENDER_TARGETS ];

	for( int i = 0; i < command.NumViews; ++i )
	{
		viewport[ i ].Width = command.Viewports[ i ].Width;
		viewport[ i ].Height = command.Viewports[ i ].Height;
		viewport[ i ].MinDepth = command.Viewports[ i ].MinDepth;
		viewport[ i ].MaxDepth = command.Viewports[ i ].MaxDepth;
		viewport[ i ].TopLeftX = command.Viewports[ i ].TopLeftX;
		viewport[ i ].TopLeftY = command.Viewports[ i ].TopLeftY;

		scissors[ i ].top = command.Scissors[ i ].Top;
		scissors[ i ].bottom = command.Scissors[ i ].Bottom;
		scissors[ i ].left = command.Scissors[ i ].Left;
		scissors[ i ].right = command.Scissors[ i ].Right;
	}

	ThrowIfNull( command.RasterizerState, "RasterizerState set to nullptr." );
	ThrowIfNull( command.BlendingState, "BlendingState set to nullptr." );
	ThrowIfNull( command.DepthStencilState, "DepthStencilState set to nullptr." );

	device_context->RSSetViewports( command.NumViews, viewport );
	device_context->RSSetScissorRects( command.NumViews, scissors );

	float blendFactor[ 4 ] = { command.BlendFactor.x, command.BlendFactor.y, command.BlendFactor.z, command.BlendFactor.w };

	device_context->RSSetState( DX11( command.RasterizerState )->Get() );
	device_context->OMSetBlendState( DX11( command.BlendingState )->Get(), blendFactor, command.StencilMask );
	device_context->OMSetDepthStencilState( DX11( command.DepthStencilState )->Get(), 0 );

	auto cameraBuffer = DX11( command.CameraBuffer );
	auto lightBuffer = DX11( command.LightBuffer );

	if( cameraBuffer )
	{
		ID3D11Buffer* directXBuffer = cameraBuffer->Get();
		m_localDeviceContext->VSSetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
		m_localDeviceContext->PSGetConstantBuffers( CAMERA_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}

	if( lightBuffer )
	{
		ID3D11Buffer* directXBuffer = lightBuffer->Get();
		m_localDeviceContext->PSGetConstantBuffers( LIGHTS_BUFFER_BINDING_POINT, 1, &directXBuffer );
	}
}

// ================================ //
//
void	DX11Renderer::ClearRenderTarget	( const ClearRenderTargetCommand& command )
{
	auto renderTarget = DX11( command.RenderTarget );

	if( command.ClearRenderTarget )
	{
		float clearColor[ 4 ] = { command.ClearColor.x, command.ClearColor.y, command.ClearColor.z, command.ClearColor.w };
		device_context->ClearRenderTargetView( renderTarget->GetRenderTarget(), clearColor );
	}
	
	uint32 clearFlags = 0;
	if( command.ClearDepth )
		clearFlags = clearFlags | D3D11_CLEAR_DEPTH;
	if( command.ClearStencil )
		clearFlags = clearFlags | D3D11_CLEAR_STENCIL;

	if( clearFlags )
	{
		device_context->ClearDepthStencilView( renderTarget->GetDepthStencil(), clearFlags, command.DepthValue, command.StencilValue );
	}
}

// ================================ //
//
void	DX11Renderer::UpdateBuffer		( const UpdateBufferCommand& command )
{
	auto buffer = DX11( command.Buffer );
	ThrowIfNull( buffer, "[UpdateBufferCommand] Binding nullptr buffer" );

	m_localDeviceContext->UpdateSubresource( buffer->Get(), 0, nullptr, command.FillData, 0, 0 );
}

// ================================ //
//
void	DX11Renderer::BindBuffer		( const BindBufferCommand& command )
{
	auto buffer = DX11( command.Buffer );
	ThrowIfNull( buffer, "[BindBufferCommand] Binding nullptr buffer" );

	auto bufferDX = buffer->Get();

	if( command.BindToShader & (uint8)ShaderType::VertexShader )
		m_localDeviceContext->VSSetConstantBuffers( command.BufferSlot, 1, &bufferDX );
	if( command.BindToShader & (uint8)ShaderType::PixelShader )
		m_localDeviceContext->PSSetConstantBuffers( command.BufferSlot, 1, &bufferDX );
	if( command.BindToShader & (uint8)ShaderType::GeometryShader )
		m_localDeviceContext->GSSetConstantBuffers( command.BufferSlot, 1, &bufferDX );
	if( command.BindToShader & (uint8)ShaderType::TesselationControlShader )
		m_localDeviceContext->HSSetConstantBuffers( command.BufferSlot, 1, &bufferDX );
	if( command.BindToShader & (uint8)ShaderType::TesselationEvaluationShader )
		m_localDeviceContext->DSGetConstantBuffers( command.BufferSlot, 1, &bufferDX );
}

// ================================ //
//
void	DX11Renderer::UpdateAndBindBuffer	( const UpdateBindBuffer& command )
{
	auto buffer = DX11( command.Buffer );
	ThrowIfNull( buffer, "[UpdateBindBuffer] Updating nullptr buffer" );

	m_localDeviceContext->UpdateSubresource( buffer->Get(), 0, nullptr, command.FillData, 0, 0 );
	DX11Renderer::BindBuffer( command );
}

// ================================ //
//
void	DX11Renderer::SetDefaultBuffers	( const SetDefaultBuffersCommand& command )
{
	auto transform = DX11( command.TransformBuffer );
	auto material = DX11( command.MaterialBuffer );
	auto bones = DX11( command.BonesTransforms );

	if( transform )
	{
		auto dxBuffer = transform->Get();
		m_localDeviceContext->VSSetConstantBuffers( TRANSFORM_BUFFER_BINDING_POINT, 1, &dxBuffer );
	}
	else
	{
		// If buffer doesn't exist, set to nullptr.
		ID3D11Buffer* dxBuffer = nullptr;
		m_localDeviceContext->VSSetConstantBuffers( TRANSFORM_BUFFER_BINDING_POINT, 1, &dxBuffer );
	}

	if( material )
	{
		auto dxBuffer = material->Get();
		m_localDeviceContext->PSSetConstantBuffers( MATERIAL_BUFFER_BINDING_POINT, 1, &dxBuffer );
	}
	else
	{
		// If buffer doesn't exist, set to nullptr.
		ID3D11Buffer* dxBuffer = nullptr;
		m_localDeviceContext->VSSetConstantBuffers( MATERIAL_BUFFER_BINDING_POINT, 1, &dxBuffer );
	}

	if( bones )
	{
		auto dxBuffer = bones->Get();
		m_localDeviceContext->VSSetConstantBuffers( BONES_BUFFER_BINDING_POINT, 1, &dxBuffer );
	}
	else
	{
		// If buffer doesn't exist, set to nullptr.
		ID3D11Buffer* dxBuffer = nullptr;
		m_localDeviceContext->VSSetConstantBuffers( BONES_BUFFER_BINDING_POINT, 1, &dxBuffer );
	}
}

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetShaderStateCommand& command )
{
	auto vertexShader = DX11( command.VertexShader );
	auto pixelShader = DX11( command.PixelShader );

	ThrowIfNull( vertexShader, "[SetShaderStateCommand] Vertex shader is nullptr" );
	ThrowIfNull( pixelShader, "[SetShaderStateCommand] Pixel shader is nullptr" );

	device_context->VSSetShader( vertexShader->Get(), nullptr, 0 );
	device_context->PSSetShader( pixelShader->Get(), nullptr, 0 );
	device_context->GSSetShader( nullptr, nullptr, 0 );
	device_context->HSSetShader( nullptr, nullptr, 0 );
	device_context->DSSetShader( nullptr, nullptr, 0 );

	SetTextures( command.Textures, command.BindToShader );
}

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetShaderStateExCommand& command )
{
	assert( false );
	// Implement geometry, hull and domain shader first.

	auto vertexShader = DX11( command.VertexShader );
	auto pixelShader = DX11( command.PixelShader );
	//auto geometryShader = DX11( command.GeometryShader );
	//auto 

	ThrowIfNull( vertexShader, "[SetShaderStateExCommand] Vertex shader is nullptr" );
	ThrowIfNull( pixelShader, "[SetShaderStateExCommand] Pixel shader is nullptr" );

	device_context->VSSetShader( vertexShader->Get(), nullptr, 0 );
	device_context->PSSetShader( pixelShader->Get(), nullptr, 0 );
	device_context->GSSetShader( nullptr, nullptr, 0 );
	device_context->HSSetShader( nullptr, nullptr, 0 );
	device_context->DSSetShader( nullptr, nullptr, 0 );

	SetTextures( command.Textures, command.BindToShader );
}

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetRenderStateCommand& command )
{
	DX11Renderer::SetShaderState( static_cast< const SetShaderStateCommand& >( command ) );
	DX11Renderer::SetDefaultBuffers( static_cast< const SetDefaultBuffersCommand& >( command ) );
}

// ================================ //
//
void	DX11Renderer::SetShaderState	( const SetRenderStateExCommand& command )
{
	DX11Renderer::SetShaderState( static_cast< const SetShaderStateExCommand& >( command ) );
	DX11Renderer::SetDefaultBuffers( static_cast< const SetDefaultBuffersCommand& >( command ) );
}

// ================================ //
//
void	DX11Renderer::CopyTexture		( const CopyTextureCommand& command )
{
	auto source = DX11( command.SourceTexture );
	auto destination = DX11( command.DestinationTexture );

	ThrowIfNull( source, "[CopyTextureCommand] Vertex shader is nullptr" );
	ThrowIfNull( source, "[CopyTextureCommand] Pixel shader is nullptr" );

	device_context->CopyResource( destination->GetTex(), source->GetTex() );
}


//====================================================================================//
//			Internal Helpers	
//====================================================================================//


// ================================ //
//
bool    DX11Renderer::SetVertexBuffer   ( BufferObject* buffer, unsigned int offset )
{
    ID3D11Buffer* vertexBuffer = nullptr;
    if( buffer )
    {
        vertexBuffer = DX11( buffer )->Get();
        unsigned int stride = buffer->GetStride();
        m_localDeviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

        return false;
    }
    else
    {
        //throw new std::runtime_error( "Vertex buffer is nullptr" );
        //assert( !"Vertex buffer is nullptr" );
		unsigned int stride = 0;
		m_localDeviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
    }
    return true;
}


// ================================ //
// Buffer can be nullptr.
void	DX11Renderer::SetIndexBuffer	( BufferObject* buffer, unsigned int offset, bool extendedIndex )
{
	if ( buffer )
	{
		auto indexBuffer = DX11( buffer )->Get();
		assert( indexBuffer );

		unsigned int offset = 0;
		auto indexFormat = extendedIndex ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;

		m_localDeviceContext->IASetIndexBuffer( indexBuffer, indexFormat, offset );
	}
}

// ================================ //
//
void	DX11Renderer::SetRenderTarget	( RenderTargetObject* const targets[ MAX_BOUND_RENDER_TARGETS ], RenderTargetObject* depthStencil )
{
	ID3D11RenderTargetView* DX11Targets[ MAX_BOUND_RENDER_TARGETS ];
	for( int i = 0; i < MAX_BOUND_RENDER_TARGETS; ++i )
	{
		auto renderTargetObj = DX11( targets[ i ] );
		if( renderTargetObj )
			DX11Targets[ i ] = DX11( targets[ i ] )->GetRenderTarget();
		else
			DX11Targets[ i ] = nullptr;
	}

	auto depthStencilBuffer = DX11( depthStencil );
	ID3D11DepthStencilView* depthStencilView = nullptr;
	if( depthStencilBuffer )
		depthStencilView = depthStencilBuffer->GetDepthStencil();

	device_context->OMSetRenderTargets( MAX_BOUND_RENDER_TARGETS, DX11Targets, depthStencilView );
}

// ================================ //
//
void	DX11Renderer::SetTextures		( TextureObject* const texturesArray[ MAX_BOUND_RENDER_TARGETS ], const uint8 shaderTypes[ MAX_BOUND_RENDER_TARGETS ] )
{
	ID3D11ShaderResourceView* texturesVert[ ENGINE_MAX_TEXTURES ];
	ID3D11ShaderResourceView* texturesPix[ ENGINE_MAX_TEXTURES ];
	ID3D11ShaderResourceView* texturesGeom[ ENGINE_MAX_TEXTURES ];
	ID3D11ShaderResourceView* texturesEval[ ENGINE_MAX_TEXTURES ];
	ID3D11ShaderResourceView* texturesDomain[ ENGINE_MAX_TEXTURES ];

	for( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
	{
		if( texturesArray[ i ] )
		{
			auto texView = DX11( texturesArray[ i ] )->Get();
			texturesVert[ i ] = shaderTypes[ i ] & (uint8)ShaderType::VertexShader ? texView : nullptr;
			texturesPix[ i ] = shaderTypes[ i ] & (uint8)ShaderType::PixelShader ? texView : nullptr;
			texturesGeom[ i ] = shaderTypes[ i ] & (uint8)ShaderType::GeometryShader ? texView : nullptr;
			texturesEval[ i ] = shaderTypes[ i ] & (uint8)ShaderType::TesselationControlShader ? texView : nullptr;
			texturesDomain[ i ] = shaderTypes[ i ] & (uint8)ShaderType::TesselationEvaluationShader ? texView : nullptr;
		}
		else
		{
			texturesVert[ i ] = nullptr;
			texturesPix[ i ] = nullptr;
			texturesGeom[ i ] = nullptr;
			texturesEval[ i ] = nullptr;
			texturesDomain[ i ] = nullptr;
		}
	}

	device_context->VSSetShaderResources( 0, ENGINE_MAX_TEXTURES, texturesVert );
	device_context->PSSetShaderResources( 0, ENGINE_MAX_TEXTURES, texturesPix );
	device_context->GSSetShaderResources( 0, ENGINE_MAX_TEXTURES, texturesGeom );
	device_context->HSSetShaderResources( 0, ENGINE_MAX_TEXTURES, texturesEval );
	device_context->DSSetShaderResources( 0, ENGINE_MAX_TEXTURES, texturesDomain );
}

// ================================ //
//
void	DX11Renderer::ClearTextures()
{
	TextureObject* const texturesArray[ MAX_BOUND_RENDER_TARGETS ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	uint8 shaderTypes[ MAX_BOUND_RENDER_TARGETS ];

	for( int i = 0; i < MAX_BOUND_RENDER_TARGETS; ++i )
	{
		shaderTypes[ i ] = 0x1F;
	}

	SetTextures( texturesArray, shaderTypes );
}

