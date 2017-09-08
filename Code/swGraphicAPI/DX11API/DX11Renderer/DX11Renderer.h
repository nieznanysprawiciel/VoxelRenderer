#pragma once
/**
@file DX11Renderer.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/



#include "DX11Initializer/DX11APIObjects.h"
#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/Resources/BlendingState.h"
#include "swGraphicAPI/Resources/RasterizerState.h"
#include "swGraphicAPI/Resources/DepthStencilState.h"
#include "swGraphicAPI/Rendering/IRenderer.h"

class DX11Renderer;
typedef DX11Renderer Renderer;


inline DX11Buffer*				DX11	( BufferObject* res )		{ return static_cast< DX11Buffer* >( res ); }
inline DX11Texture*				DX11	( TextureObject* res )		{ return static_cast< DX11Texture* >( res ); }
inline DX11ComputeShader*		DX11	( ComputeShader* res )		{ return static_cast< DX11ComputeShader* >( res ); }
inline DX11PixelShader*			DX11	( PixelShader* res )		{ return static_cast< DX11PixelShader* >( res ); }
inline DX11VertexShader*		DX11	( VertexShader* res )		{ return static_cast< DX11VertexShader* >( res ); }
inline DX11RenderTarget*		DX11	( RenderTargetObject* res )	{ return static_cast< DX11RenderTarget* >( res ); }

inline DX11BlendingState*		DX11	( BlendingState* res )		{ return static_cast< DX11BlendingState* >( res ); }
inline DX11RasterizerState*		DX11	( RasterizerState* res )	{ return static_cast< DX11RasterizerState* >( res ); }
inline DX11DepthStencilState*	DX11	( DepthStencilState* res )	{ return static_cast< DX11DepthStencilState* >( res ); }
inline DX11InputLayout*			DX11	( ShaderInputLayout* res )	{ return static_cast< DX11InputLayout* >( res ); }


//DX11GeometryShader*	Typed	( GeometryShader* res )		{ return static_cast< DX11GeometryShader* >( res ); }


// @todo There're constants in ConstantsBuffersFormats.h file. This is duplicate, but there's no way to do this :(.
#define CAMERA_BUFFER_BINDING_POINT 0
#define LIGHTS_BUFFER_BINDING_POINT 1
#define TRANSFORM_BUFFER_BINDING_POINT 1
#define BONES_BUFFER_BINDING_POINT 2
#define MATERIAL_BUFFER_BINDING_POINT 2


/**@brief Renderer obs³uguj¹cy DirectX 11.
@ingroup DX11API*/
class DX11Renderer	: public IRenderer,	protected DX11AuxiliaryObjects
{
private:
	/**Kontekst urz¹dzenia s³u¿¹cy do renderowania. W zale¿noœci czy stosujemy opóŸnione renderowanie (deferred rendering) czy nie,
	ta zmienna przechowuje kopiê wskaŸnika z DX11_interfaces_container lub opóŸniony kontekst (deferred context).
	W trakcie renderowania nie musimy wiedzieæ z czym mamy do czynienia.*/
	ID3D11DeviceContext*		m_localDeviceContext;			
	bool						m_valid;					///< Je¿eli klasa nie zosta³a zainicjowana poprawnie, to bêdzie tu wartoœæ false.
	RendererUsage				m_usageType;				///< Informacja jak bêdzie u¿ywany rederer.
public:
	DX11Renderer( RendererUsage usage = USE_AS_IMMEDIATE );
	~DX11Renderer();

	bool SupportsDefferedContextRendering() override	{ return true; }		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
	bool IsValid() override								{ return m_valid; }




	virtual void		Draw					( const DrawCommand& command )						override;
	virtual void		DrawInstanced			( const DrawInstancedCommand& command )				override;

	virtual void		SetRenderTarget			( const SetRenderTargetCommand& command )			override;
	virtual void		SetRenderTarget			( const SetRenderTargetExCommand& command )			override;
	virtual void		ClearRenderTarget		( const ClearRenderTargetCommand& command )			override;

	virtual void		UpdateBuffer			( const UpdateBufferCommand& command )				override;
	virtual void		BindBuffer				( const BindBufferCommand& command )				override;
	virtual void		UpdateAndBindBuffer		( const UpdateBindBuffer& command )					override;

	virtual void		SetDefaultBuffers		( const SetDefaultBuffersCommand& command )			override;
	
	virtual void		SetShaderState			( const SetShaderStateCommand& command )			override;
	virtual void		SetShaderState			( const SetShaderStateExCommand& command )			override;
	virtual void		SetShaderState			( const SetRenderStateCommand& command )			override;
	virtual void		SetShaderState			( const SetRenderStateExCommand& command )			override;

	virtual void		CopyTexture				( const CopyTextureCommand& command )				override;

private:
    
    bool                SetVertexBuffer         ( BufferObject* buffer, unsigned int offset );
	void				SetIndexBuffer			( BufferObject* buffer, unsigned int offset, bool extendedIndex );
	void				SetRenderTarget			( RenderTargetObject* const targets[ MAX_BOUND_RENDER_TARGETS ], RenderTargetObject* depthStencil );
	void				SetTextures				( TextureObject* const texturesArray[ MAX_BOUND_RENDER_TARGETS ], const uint8 shaderTypes[ MAX_BOUND_RENDER_TARGETS ] );

	void				ClearTextures			();
};



