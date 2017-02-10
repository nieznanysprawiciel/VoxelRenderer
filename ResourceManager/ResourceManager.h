#pragma once
/**
@file ResourceManager.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/Resources/BlendingState.h"
#include "swGraphicAPI/Resources/RasterizerState.h"
#include "swGraphicAPI/Resources/DepthStencilState.h"
#include "swGraphicAPI/ResourceManager/ResourceContainer.h"


class ILoader;



/**@brief Manager for low level resources.

@todo This class should implement generic high level asssets managment.

@ingroup ResourcesManagment*/
class ResourceManager
{
private:
protected:
	ResourceContainer< BlendingState >			m_blendingState;	///< BlendingState objects.
	ResourceContainer< RasterizerState >		m_rasterizerState;	///< RasterizerState objects.
	ResourceContainer< DepthStencilState >		m_depthStencilState;///< DepthStencilState objects.

	ResourceContainer< VertexShader >			m_vertexShader;		///< Vertex shaders.
	ResourceContainer< PixelShader >			m_pixelShader;		///< Pixel shaders.
	ResourceContainer< GeometryShader >			m_geometryShader;	///< Geometry shaders.
	ResourceContainer< EvaluationShader >		m_evaluationShader;	///< Tesselation evaluation shaders.
	ResourceContainer< ControlShader >			m_controlShaders;	///< Tesselation control shaders.

	ResourceContainer< TextureObject >			m_texture;			///< Textures.
	ResourceContainer< BufferObject >			m_vertexBuffer;		///< Vertex buffers.
	ResourceContainer< BufferObject >			m_indexBuffer;		///< Index buffers.
	ResourceContainer< BufferObject >			m_constantBuffer;	///< Shader constant buffers.
	ResourceContainer< ShaderInputLayout >		m_vertexLayout;		///< Vertex layouts.
	// UWAGA! m_fileModel musi byæ na koñcu. Jego destruktor kasuje odwo³ania do obiektów powy¿ej. Podobnie RenderTargetObject odwo³uje siê do tekstur.
	// Musz¹ one w tym czasie istnieæ, a destruktory s¹ wywo³ywane w kolejnoœci odwrotnej do zadeklarowanej w klasie.
	ResourceContainer< RenderTargetObject >		m_renderTarget;		///<Obiekty mog¹ce s³u¿yæ za render target.

	/*loadery dla ró¿nych formatów plików z modelami*/
	//std::vector<ILoader*>						m_loader;				///<Loadery do plików z modelami 3D

public:
	explicit						ResourceManager();
									~ResourceManager();

	// Funkcje do zarz¹dzania assetami
	RenderTargetObject*				CreateRenderTarget			( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor );

	///@name Geting existing resource
	///@detail Gets resource if exist otherwise returns nullptr.
	///@{

	inline BlendingState*			GetBlendingState			( const std::wstring& name )		{ return m_blendingState.get( name ); }
	inline RasterizerState*			GetRasterizerState			( const std::wstring& name )		{ return m_rasterizerState.get( name ); }
	inline DepthStencilState*		GetDepthStencilState		( const std::wstring& name )		{ return m_depthStencilState.get( name ); }

	inline BlendingState*			GetBlendingState			( const BlendingInfo& info )		{ return m_blendingState.Find( info ); }
	inline RasterizerState*			GetRasterizerState			( const RasterizerStateInfo& info ) { return m_rasterizerState.Find( info ); }
	inline DepthStencilState*		GetDepthStencilState		( const DepthStencilInfo& info )	{ return m_depthStencilState.Find( info ); }

	inline RenderTargetObject*		GetRenderTarget				( const std::wstring& name ) { return m_renderTarget.get( name ); }	///<Zwraca RenderTarget o podanej nazwie, je¿eli jest wczytany.
	inline VertexShader*			GetVertexShader				( const std::wstring& name ) { return m_vertexShader.get( name ); } ///<Zwraca vertex shader o podanej nazwie, je¿eli jest wczytany.
	inline PixelShader*				GetPixelShader				( const std::wstring& name ) { return m_pixelShader.get( name ); }	///<Zwraca pixel shader o podanej nazwie, je¿eli jest wczytany.
	inline TextureObject*			GetTexture					( const std::wstring& name ) { return m_texture.get( name ); }		///<Zwraca teksturê o podanej nazwie, je¿eli jest wczytany.
	inline BufferObject*			GetVertexBuffer				( const std::wstring& name ) { return m_vertexBuffer.get( name ); }	///<Zwraca bufor wierzcho³ków o podanej nazwie, je¿eli jest wczytany.
	inline BufferObject*			GetConstantBuffer			( const std::wstring& name ) { return m_constantBuffer.get( name ); }	///<Zwraca bufor sta³ych o podanej nazwie, je¿eli jest wczytany.
	inline BufferObject*			GetIndexBuffer				( const std::wstring& name ) { return m_indexBuffer.get( name ); }	///<Zwraca bufor indeksów o podanej nazwie, je¿eli jest wczytany.
	inline ShaderInputLayout*		GetLayout					( const std::wstring& name ) { return m_vertexLayout.get( name ); }	///<Zwraca layout o podanej nazwie.	
	///@}


	///@name Resource loading
	///@detail Load assets from specified file. Functions protect from loading assets multiple times.
	///@{
	TextureObject*					LoadTexture					( const std::wstring& fileName );
	VertexShader*					LoadVertexShader			( const std::wstring& fileName, const std::string& shaderEntry );
	VertexShader*					LoadVertexShader			( const std::wstring& fileName, const std::string& shaderEntry, ShaderInputLayout** layout, InputLayoutDescriptor* layout_desc );
	PixelShader*					LoadPixelShader				( const std::wstring& fileName, const std::string& shaderEntry );
	GeometryShader*					LoadGeometryShader			( const std::wstring& fileName, const std::string& shaderEntry );
	ControlShader*					LoadControlShader			( const std::wstring& fileName, const std::string& shaderEntry );
	EvaluationShader*				LoadEvaluationShader		( const std::wstring& fileName, const std::string& shaderEntry );
	///@}

	///@name Resource creation
	///@detail You can create assets in code using these functions. Remember to give unique names for your assets.
	///Engine uses convention, that all generated resources have :: before name, to distinguish them from assets loaded from files.
	///@{
	ResourcePtr< BufferObject >		CreateVertexBuffer			( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	ResourcePtr< BufferObject >		CreateVertexBuffer			( const std::wstring& name, const VertexBufferInitData& data );
	ResourcePtr< BufferObject >		CreateIndexBuffer			( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	ResourcePtr< BufferObject >		CreateIndexBuffer			( const std::wstring& name, const IndexBufferInitData& data );
	ResourcePtr< BufferObject >		CreateConstantsBuffer		( const std::wstring& name, const void* buffer, unsigned int size );
	ResourcePtr< BufferObject >		CreateConstantsBuffer		( const std::wstring& name, const ConstantBufferInitData& data );

	ResourcePtr< BlendingState >	CreateBlendingState			( const std::wstring& name, const BlendingInfo& info );
	ResourcePtr< RasterizerState >	CreateRasterizerState		( const std::wstring& name, const RasterizerStateInfo& info );
	ResourcePtr< DepthStencilState >CreateDepthStencilState		( const std::wstring& name, const DepthStencilInfo& info );

	///@}

	RenderTargetObject*				AddRenderTarget				( RenderTargetObject* renderTarget, const std::wstring& name );


	///@name Listing resources
	///@{
	std::vector< ResourcePtr< BufferObject > >			ListVertexBuffers	();
	std::vector< ResourcePtr< BufferObject > >			ListIndexBuffers	();
	std::vector< ResourcePtr< BufferObject > >			ListConstantBuffers	();
	std::vector< ResourcePtr< ShaderInputLayout > >		ListShaderLayouts	();

	std::vector< ResourcePtr< TextureObject > >			ListTextures		();
	std::vector< ResourcePtr< VertexShader > >			ListVertexShaders	();
	std::vector< ResourcePtr< PixelShader > >			ListPixelShaders	();
	
	std::vector< ResourcePtr< RenderTargetObject > >	ListRenderTargets	();
	///@}

private:
	//ILoader*						FindLoader					( const std::wstring& path );
	
	virtual MemoryChunk				LoadTextureImpl				( const filesystem::Path& filePath, TextureInfo& texInfo );
};

