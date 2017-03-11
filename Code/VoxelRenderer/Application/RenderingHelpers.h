#pragma once
/**
@file RenderingHelpers.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/Rendering/RenderCommands.h"

#include <DirectXMath.h>

class IRenderer;
struct RenderContext;


/**@brief Rendering helper functions for @ref DisplayEngine and render passes.*/
struct RenderingHelper
{
private:
	IRenderer*			m_renderer;


public:

	/**@brief Instanciate this class to call helper functions without passing renderer.*/
	explicit	RenderingHelper	( IRenderer* renderer )
		:	m_renderer( renderer )
	{}

	/**@brief Default clear command.
	This command doesn't clear stencil buffer.*/
	void					ClearRenderTargetAndDepth	( RenderTargetObject* target, const DirectX::XMFLOAT4& color, float depth );

	/**@brief Set render target.
	Ignores buffers and sets them to nullptr.*/
	void					SetRenderTarget				( RenderTargetObject* target,	RasterizerState* rasterizerState, BlendingState* blendingState, DepthStencilState* depthStencilState );

	/**@brief Updates buffer with data.*/
	template< typename BufferDataType >
	inline void				UpdateBuffer				( BufferObject* buffer, BufferDataType& data );

	/**@brief Draw without using buffer.*/
	void					DrawBufferLess				( uint32 numVerticies, PrimitiveTopology topology );

public:
	// ================================ //
	// Static part

	/**@brief Default clear command.
	This command doesn't clear stencil buffer.*/
	static void					ClearRenderTargetAndDepth	( IRenderer* renderer, RenderTargetObject* target, const DirectX::XMFLOAT4& color, float depth );

	/**@brief Set render target.
	Ignores buffers and sets them to nullptr.*/
	static void					SetRenderTarget				( IRenderer* renderer, RenderTargetObject* target,	RasterizerState* rasterizerState, BlendingState* blendingState, DepthStencilState* depthStencilState );

	/**@brief Updates buffer with data.*/
	template< typename BufferDataType >
	static inline void			UpdateBuffer				( IRenderer* renderer, BufferObject* buffer, BufferDataType& data );

	/**@brief Draw without using buffer.*/
	static void					DrawBufferLess				( IRenderer* renderer, uint32 numVerticies, PrimitiveTopology topology );

	/**@brief Set texture in command.
	@param[in] bindShader Flags @ref ShaderType can be combined with operator|.*/
	static void					SetTexture					( SetShaderStateCommand& command, TextureObject* texture, uint8 bindingPoint, uint8 bindShader );

	/**@brief Set all textures to nullptr.*/
	static void					ClearTextureState			( SetShaderStateCommand& command );
};




// ================================ //
//
template< typename BufferDataType >
inline void			RenderingHelper::UpdateBuffer			( BufferObject* buffer, BufferDataType& data )
{
	UpdateBuffer( m_renderer, buffer, data );
}

// ================================ //
//
template< typename BufferDataType >
inline void			RenderingHelper::UpdateBuffer			( IRenderer* renderer, BufferObject* buffer, BufferDataType& data )
{
	UpdateBufferCommand updateCommand;
	updateCommand.Buffer = buffer;
	updateCommand.FillData = (uint8*)&data;
	updateCommand.Size = sizeof( BufferDataType );

	renderer->UpdateBuffer( updateCommand );
}
