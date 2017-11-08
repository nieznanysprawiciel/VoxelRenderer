/**
@file ResourceManager.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swGraphicAPI/ResourceManager/stdafx.h"
#include "ResourceManager.h"

#include "swCommonLib/Common/ObjectDeleter.h"
#include "swCommonLib/Common/Converters.h"
#include "swCommonLib/System/Path.h"

#include "swGraphicAPI/Resources/ResourcesFactory.h"



#include "swCommonLib/Common/MemoryLeaks.h"


using namespace DirectX;

//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

ResourceManager::ResourceManager()
{}



ResourceManager::~ResourceManager( )
{}




/** @brief Znajduje Loader pasuj¹cy do pliku podanego w parametrze.
@param[in] path Œcie¿ka do pliku, dla której szukamy loadera.
@return WskaŸnik na odpowiedni loader lub nullptr, je¿eli nie znaleziono pasuj¹cego.*/
//ILoader* ResourceManager::FindLoader( const std::wstring& path )
//{
//	for ( unsigned int i = 0; i <  m_loader.size( ); ++i )
//	if ( m_loader[i]->can_load( path ) )
//		return m_loader[i];
//	return nullptr;
//}




/**@brief Tworzy nowy render target.

Funkcja dodaje stworzony obiekt do tablicy m_renderTarget. Je¿eli tekstury
bufora colorów, g³êbokoœci i stencilu nie s¹ nullptrami, to i one s¹ dodawane do tablicy m_texture.

Tekstury te maj¹ nazwy jak render target + dodany jest cz³on
- ::color
- ::depth
- ::stencil

@todo Przy dodawaniu tekstur nie jest sprawdzane czy one ju¿ istniej¹. Trzeba albo to sprawdzaæ, albo zapewniæ
np. jak¹œ polityk¹ nazewnictwa, ¿e w ten sposób nie nadpisujemy istniej¹cej tekstury.

@param[in] name Nazwa identyfikuj¹ca render target.
@param[in] renderTargetDescriptor Deskryptor opisuj¹cy parametry render targetu.
@return Zwraca stworzony obiekt lub nullptr w przypadku niepowodzenia. Je¿eli render target ju¿ istnia³, to zwracany jest istniej¹cy obiekt.
*/
RenderTargetObject* ResourceManager::CreateRenderTarget( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	RenderTargetObject* newRenderTarget = m_renderTarget.get( name );
	if( !newRenderTarget )
	{
		newRenderTarget = ResourcesFactory::CreateRenderTarget( name, renderTargetDescriptor );
		if( !newRenderTarget )	return nullptr;

		m_renderTarget.UnsafeAdd( name, newRenderTarget );
		
		auto colorBuff = newRenderTarget->GetColorBuffer();
		if( colorBuff )
			m_texture.UnsafeAdd( Convert::FromString( colorBuff->GetFilePath().String(), std::wstring() ), colorBuff );

		auto depthBuffer = newRenderTarget->GetDepthBuffer();
		if( depthBuffer )
			m_texture.UnsafeAdd( Convert::FromString( depthBuffer->GetFilePath().String(), std::wstring() ), depthBuffer );

		auto stencilBuffer = newRenderTarget->GetStencilBuffer();
		if( stencilBuffer )
			m_texture.UnsafeAdd( Convert::FromString( stencilBuffer->GetFilePath().String(), std::wstring() ), stencilBuffer );
	}

	return newRenderTarget;
}



/**@brief Dodaje renderTarget do ResourceManagera, je¿eli jeszcze nie istnia³.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@note Je¿eli renderTarget ju¿ istnia³ (jego nazwa), to ten podany w parametrze nie zostanie dodany.
Oznacza to, ¿e za jego zwolnienie odpowiada ten, kto go stworzy³. Trzeba zawsze sprawdziæ czy
zwrócona wartoœæ jest tym samym co podaliœmy.

@todo Nie mo¿e tak zostaæ, ¿e ktoœ dodaje renderTarget i musi sprawdziæ czy nie dosta³ innego. Nie mo¿na
te¿ zmuszaæ kogoœ do zwalniania pamiêci po renderTargecie. Wogóle dodawanie renderTargetów musi siê odbywaæ jakoœ inaczej.
Najlepiej, ¿eby by³y one tworzone przez ResourceManager, ale wtedy trzeba wymyœleæ sposób dodawania renderTargetu zwi¹zanego z buforem okna.

@param[in] renderTarget renderTarget, który ma zostaæ dodany.
@param[in] name Nazwa renderTargetu. Do materia³u bêdzie mo¿na siê odwo³aæ podaj¹c ci¹g znaków
@return Zwraca wskaŸnik na dodany renderTarget.*/
RenderTargetObject* ResourceManager::AddRenderTarget( RenderTargetObject* renderTarget, const std::wstring& name )
{
	RenderTargetObject* existingRenderTarget = m_renderTarget.get( name );
	if ( !existingRenderTarget )
		m_renderTarget.UnsafeAdd( name, renderTarget );

	return existingRenderTarget ? existingRenderTarget : renderTarget;
}

/**@brief Adds texture to ResourceManager.
If texture under this name already existed, texture want be added and new texture will be returned in result.

@todo Consider making this function protected.*/
TextureObject*		ResourceManager::AddTexture		( TextureObject* texture, const std::wstring& name )
{
	TextureObject* existingTex = m_texture.get( name );
	if ( !existingTex )
		m_texture.UnsafeAdd( name, texture );

	return existingTex ? existingTex : texture;
}

/**@brief Dodaje vertex shader do ResourceManagera. Je¿eli obiekt ju¿ istnia³, to nie jest tworzony nowy.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Nazwa pliku, w którym znajduje siê vertex shader.
@param[in] shaderEntry Nazwa funkcji od której ma siê zacz¹æ wykonywanie shadera.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je¿eli shadera nie uda³o siê skompilowaæ.*/
VertexShader* ResourceManager::LoadVertexShader( const std::wstring& fileName, const std::string& shaderEntry )
{
	VertexShader* shader = m_vertexShader.get( fileName );
	if ( !shader )
	{
		// Nie by³o shadera, trzeba go stworzyæ i dodaæ
		shader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry );
		if ( !shader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ResourceManagera
			return nullptr;

		m_vertexShader.UnsafeAdd( fileName, shader );	// Dodaliœmy teksturê
	}

	return shader;
}

/**@brief Dodaje vertex shader do ResourceManagera. Je¿eli obiekt ju¿ istnia³, to nie jest tworzony nowy.
Tworzy te¿ layout wierzcho³ka zwi¹zany z tym shaderem i zwraca go w zmiennej layout.

Je¿eli vertex shader wczeœniej istnia³, to stworzenie layoutu wymaga ponownego skompilowania shadera. Shader taki jest potem
kasowany i nie zostaje zdublowany w ResourceManagerze, ale niepotrzebna praca zostaje w³o¿ona. Jest wiêc zadaniem programisty, ¿eby
do takich rzeczy dochodzi³o jak najrzadziej.

@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Nazwa pliku, w którym znajduje siê vertex shader.
@param[in] shaderEntry Nazwa funkcji od której ma siê zacz¹æ wykonywanie shadera.
@param[out] layout W zmiennej umieszczany jest wskaŸnik na layout wierzcho³ka. Nawet je¿eli shader siê nie skompilowa³, to pole mo¿e mieæ wartoœæ inn¹ ni¿ nullptr.
Dzieje siê tak wtedy, gdy layout istnia³ ju¿ wczeœniej.
@attention Je¿eli vertex shader wczeœniej istnia³, to stworzenie layoutu wymaga ponownego skompilowania shadera. Shader taki jest potem 
kasowany i nie zostaje zdublowany w ResourceManagerze, ale niepotrzebna praca zostaje w³o¿ona. Jest wiêc zadaniem programisty, ¿eby
do takich rzeczy dochodzi³o jak najrzadziej.
@param[in] layoutDesc Deskryptor opisujacy tworzony layout.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je¿eli shadera nie uda³o siê skompilowaæ.*/
VertexShader* ResourceManager::LoadVertexShader( const std::wstring& fileName,
													const std::string& shaderEntry,
													ShaderInputLayout** layout,
													InputLayoutDescriptor* layoutDesc )
{
	/// @todo Ten kod to jakiœ totalny shit. Jak komuœ siê bêdzie nudzi³o kiedyœ (ha ha), to mo¿e niech poprawi.
	*layout = nullptr;
	VertexShader* shader = m_vertexShader.get( fileName );
	VertexShader* newShader = nullptr;
	ShaderInputLayout* inputLayout = m_vertexLayout.get( layoutDesc->GetName() );


	// Tworzymy potrzebne obiekty
	if( !inputLayout )
	{
		// Tworzymy shader niezale¿nie czy istnieje. Inaczej nie moglibyœmy stworzyæ layoutu.
		// Shader zostanie potem usuniêty.
		newShader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry, layout, layoutDesc );
		if ( !newShader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ResourceManagera
			return nullptr;		// layout te¿ jest nullptrem, nie trzeba siê martwiæ.
	}
	else if( !shader )
	{
		// Layout istnieje, ale shader nie.
		newShader = ResourcesFactory::CreateVertexShaderFromFile( fileName, shaderEntry );
		*layout = inputLayout;	// Je¿eli layout istnia³, to przepisujemy go na wyjœcie. Je¿eli nie to i tak bêdzie nullptr.
		if ( !newShader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ResourceManagera
			return nullptr;
	}
	else
	{// Wszystkie obiekty istania³y ju¿ wczeœniej.
		*layout = inputLayout;
		return shader;
	}

	// Nowo powsta³e obiekty musz¹ zostaæ dodane do kontenerów.
	if ( !shader )
	{
		// Nie by³o shadera, trzeba go dodaæ
		m_vertexShader.UnsafeAdd( fileName, newShader );	// Dodaliœmy shader
		shader = newShader;
	}
	else
	{	// Shader ju¿ by³, wiêc kasujemy nowy
		// Destruktor jest prywatny, wiêc nie mo¿emy kasowaæ obiektu bezpoœrednio.
		ObjectDeleter< VertexShader>::delete_object( shader, ObjectDeleterKey< VertexShader>() );
	}

	if( !inputLayout )	// Layoutu nie by³o wczeœniej wiêc dodajemy.
		m_vertexLayout.UnsafeAdd( layoutDesc->GetName(), *layout );

	return shader;
}


/**@brief Dodaje pixel shader do ResourceManagera. Je¿eli obiekt ju¿ istnia³, to nie jest tworzony nowy.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Nazwa pliku, w którym znajduje siê pixel shader.
@param[in] shaderEntry Nazwa funkcji od której ma siê zacz¹æ wykonywanie shadera.
@return Zwraca obiekt dodanego shadera. Zwraca nullptr, je¿eli shadera nie uda³o siê skompilowaæ.*/
PixelShader* ResourceManager::LoadPixelShader				( const std::wstring& fileName, const std::string& shaderEntry )
{
	PixelShader* shader = m_pixelShader.get( fileName );
	if ( !shader )
	{
		// Nie by³o shadera, trzeba go stworzyæ i dodaæ
		shader = ResourcesFactory::CreatePixelShaderFromFile( fileName, shaderEntry );
		if ( !shader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ResourceManagera
			return nullptr;

		m_pixelShader.UnsafeAdd( fileName, shader );	// Dodaliœmy teksturê
	}

	return shader;
}

// ================================ //
//
GeometryShader*		ResourceManager::LoadGeometryShader	( const std::wstring& fileName, const std::string& shaderEntry )
{
	GeometryShader* shader = m_geometryShader.get( fileName );
	if ( !shader )
	{
		// Nie by³o shadera, trzeba go stworzyæ i dodaæ
		shader = ResourcesFactory::CreateGeometryShaderFromFile( fileName, shaderEntry );
		if ( !shader )		// shader móg³ mieæ z³y format, a nie chcemy dodawaæ nullptra do ResourceManagera
			return nullptr;

		m_geometryShader.UnsafeAdd( fileName, shader );	// Dodaliœmy teksturê
	}

	return shader;
}

ControlShader*		ResourceManager::LoadControlShader	( const std::wstring& fileName, const std::string& shaderEntry )
{
	assert( !"Implements me" );
	return nullptr;
}

EvaluationShader*	ResourceManager::LoadEvaluationShader	( const std::wstring& fileName, const std::string& shaderEntry )
{
	assert( !"Implements me" );
	return nullptr;
}

/**@brief Dodaje teksturê do ModelManagera, je¿eli jeszcze nie istnia³a.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] fileName Œcie¿ka do tekstury

@return Zwraca wskaŸnik na dodan¹ teksturê lub nullptr, je¿eli nie da³o siê wczytaæ.*/
TextureObject* ResourceManager::LoadTexture( const std::wstring& fileName )
{
	TextureObject* tex = m_texture.get( fileName );
	if ( !tex )
	{
		// Nie by³o tekstury, trzeba j¹ stworzyæ i dodaæ
		TextureInfo texInfo;
		texInfo.FilePath = filesystem::Path( fileName );
		texInfo.GenerateMipMaps = true;
		texInfo.MipMapFilter = MipMapFilter::Lanczos4;

		MemoryChunk texData = LoadTextureImpl( texInfo.FilePath, texInfo );

		tex = ResourcesFactory::CreateTextureFromMemory( texData, std::move( texInfo ) );
		if ( !tex )		// Tekstura mog³a mieæ z³y format, a nie chcemy dodawaæ nullptra do ResourceManagera
			return nullptr;

		m_texture.UnsafeAdd( fileName, tex );	// Dodaliœmy teksturê
	}

	return tex;
}

/**@brief Dodaje do ResourceManagera bufor wierzcho³ków.
Je¿eli pod tak¹ nazw¹ istnieje jakiœ bufor, to zostanie zwrócony wskaŸnik na niego.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] name Nazwa bufora, po której mo¿na siê bêdzie odwo³aæ.
@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze.
@param[in] vertCount Liczba wierzcho³ków/indeksów w buforze.
@return Dodany bufor wierzcho³ków. Zwraca nullptr, je¿eli nie uda³o siê stworzyæ bufora.*/
ResourcePtr< BufferObject > ResourceManager::CreateVertexBuffer( const std::wstring& name, const void* buffer, unsigned int elementSize, unsigned int vertCount )
{
	VertexBufferInitData initData;
	initData.Data = (const uint8*)buffer;
	initData.ElementSize = elementSize;
	initData.NumElements = vertCount;
	
	return CreateVertexBuffer( name, initData );
}

/**@brief Creates vetex buffer.

@return Returns buffer or nullptr if name already exists or buffer creation failed.*/
ResourcePtr<BufferObject>	ResourceManager::CreateVertexBuffer		( const std::wstring& name, const VertexBufferInitData& data )
{
	BufferObject* vertexBuff = m_vertexBuffer.get( name );
	if ( vertexBuff )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr<BufferObject>();
	
	
	vertexBuff = ResourcesFactory::CreateBufferFromMemory( name, data.Data, data.CreateBufferInfo() );
	if ( !vertexBuff )		// Bufor móg³ siê nie stworzyæ, a nie chcemy dodawaæ nullptra do ResourceManagera
		return nullptr;

	m_vertexBuffer.UnsafeAdd( name, vertexBuff );	// Dodaliœmy bufor
	return ResourcePtr<BufferObject>( vertexBuff );
}

/**@brief Dodaje do ResourceManagera bufor indeksów.
Je¿eli pod tak¹ nazw¹ istnieje jakiœ bufor, to zostanie zwrócony wskaŸnik na niego.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] name Nazwa bufora, po której mo¿na siê bêdzie odwo³aæ.
@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] elementSize Rozmiar pojedynczego elementu w buforze.
@param[in] vertCount Liczba wierzcho³ków/indeksów w buforze.
@return Dodany bufor indeksów. Zwraca nullptr, je¿eli nie uda³o siê stworzyæ bufora.*/
ResourcePtr< BufferObject > ResourceManager::CreateIndexBuffer( const std::wstring& name, const void* buffer, unsigned int elementSize, unsigned int vertCount )
{
	IndexBufferInitData initData;
	initData.Data = (const uint8*)buffer;
	initData.ElementSize = elementSize;
	initData.NumElements = vertCount;

	return CreateIndexBuffer( name, initData );
}

/**@brief Vreates index buffer.

@return Returns buffer or nullptr if name already exists or buffer creation failed.*/
ResourcePtr<BufferObject>	ResourceManager::CreateIndexBuffer		( const std::wstring& name, const IndexBufferInitData& data )
{
	BufferObject* indexBuff = m_indexBuffer.get( name );
	if ( indexBuff )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr<BufferObject>();
	
	
	indexBuff = ResourcesFactory::CreateBufferFromMemory( name, data.Data, data.CreateBufferInfo() );
	if ( !indexBuff )		// Bufor móg³ siê nie stworzyæ, a nie chcemy dodawaæ nullptra do ResourceManagera
		return nullptr;

	m_indexBuffer.UnsafeAdd( name, indexBuff );	// Dodaliœmy bufor
	return ResourcePtr<BufferObject>( indexBuff );
}

/**@brief Dodaje do ResourceManagera bufor sta³ch dla shadera.
Je¿eli pod tak¹ nazw¹ istnieje jakiœ bufor, to zostanie zwrócony wskaŸnik na niego.
@note Funkcja nie dodaje odwo³ania do obiektu, bo nie zak³ada, ¿e ktoœ go od razu u¿yje.
W ka¿dym miejscu, gdzie zostanie przypisany zwrócony obiekt, nale¿y pamiêtaæ o dodaniu odwo³ania oraz
skasowaniu go, gdy obiekt przestanie byæ u¿ywany.

@param[in] name Nazwa bufora, po której mo¿na siê bêdzie odwo³aæ.
@param[in] buffer WskaŸnik na bufor z danym, które maj¹ byæ przeniesione do bufora DirectXowego.
@param[in] size Rozmiar bufora.
@return Dodany bufor indeksów. Zwraca nullptr, je¿eli nie uda³o siê stworzyæ bufora.*/
ResourcePtr< BufferObject >	ResourceManager::CreateConstantsBuffer( const std::wstring& name, const void* buffer, unsigned int size )
{
	ConstantBufferInitData initData;
	initData.Data = (const uint8*)buffer;
	initData.ElementSize = size;
	initData.NumElements = 1;

	return CreateConstantsBuffer( name, initData );
}

/**@brief Creates constant buffer.

@return Returns buffer or nullptr if name already exists or buffer creation failed.*/
ResourcePtr<BufferObject>	ResourceManager::CreateConstantsBuffer		( const std::wstring& name, const ConstantBufferInitData& data )
{
	BufferObject* constBuff = m_constantBuffer.get( name );
	if ( constBuff )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr<BufferObject>();
	
	
	constBuff = ResourcesFactory::CreateBufferFromMemory( name, data.Data, data.CreateBufferInfo() );
	if ( !constBuff )		// Bufor móg³ siê nie stworzyæ, a nie chcemy dodawaæ nullptra do ResourceManagera
		return nullptr;

	m_constantBuffer.UnsafeAdd( name, constBuff );	// Dodaliœmy bufor
	return ResourcePtr<BufferObject>( constBuff );
}

// ================================ //
//
ResourcePtr< BufferObject >		ResourceManager::CreateTextureBuffer	( const std::wstring& name, const TextureBufferInitData& data )
{
	BufferObject* constBuff = m_textureBuffers.get( name );
	if ( constBuff )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr<BufferObject>();
	
	
	constBuff = ResourcesFactory::CreateBufferFromMemory( name, data.Data, data.CreateBufferInfo() );
	if ( !constBuff )		// Bufor móg³ siê nie stworzyæ, a nie chcemy dodawaæ nullptra do ResourceManagera
		return nullptr;

	m_textureBuffers.UnsafeAdd( name, constBuff );	// Dodaliœmy bufor
	return ResourcePtr<BufferObject>( constBuff );
}

/**@brief Created BlendingState object.

@return If object named name exist, returns nullptr.*/
ResourcePtr< BlendingState >	ResourceManager::CreateBlendingState	( const std::wstring& name, const BlendingInfo& info )
{
	auto resource = m_blendingState.get( name );
	if ( resource )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr< BlendingState >();

	resource = ResourcesFactory::CreateBlendingState( info );
	m_blendingState.UnsafeAdd( name, resource );

	return ResourcePtr< BlendingState >( resource );
}

/**@brief Created RasterizerState object.

@return If object named name exist, returns nullptr.*/
ResourcePtr< RasterizerState >	ResourceManager::CreateRasterizerState	( const std::wstring& name, const RasterizerStateInfo& info )
{
	auto resource = m_rasterizerState.get( name );
	if ( resource )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr< RasterizerState >();

	resource = ResourcesFactory::CreateRasterizerState( info );
	m_rasterizerState.UnsafeAdd( name, resource );

	return ResourcePtr< RasterizerState >( resource );
}

/**@brief Created DepthStencilState object.

@return If object named name exist, returns nullptr.*/
ResourcePtr< DepthStencilState >	ResourceManager::CreateDepthStencilState	( const std::wstring& name, const DepthStencilInfo& info )
{
	auto resource = m_depthStencilState.get( name );
	if ( resource )	// Je¿eli znaleŸliœmy bufor, to zwracamy nullptr
		return ResourcePtr< DepthStencilState >();

	resource = ResourcesFactory::CreateDepthStencilState( info );
	m_depthStencilState.UnsafeAdd( name, resource );

	return ResourcePtr< DepthStencilState >( resource );
}

//====================================================================================//
//			Listowanie assetów
//====================================================================================//


/**@brief Listowanie buforów wierzcho³ków.*/
std::vector< ResourcePtr< BufferObject > >		ResourceManager::ListVertexBuffers()
{
	return m_vertexBuffer.List();
}

/**@brief Listowanie buforów indeksów.*/
std::vector< ResourcePtr< BufferObject > >		ResourceManager::ListIndexBuffers()
{
	return m_indexBuffer.List();
}

/**@brief Listowanie buforów sta³ych.*/
std::vector< ResourcePtr< BufferObject > >		ResourceManager::ListConstantBuffers()
{
	return m_constantBuffer.List();
}

/**@brief Listowanie layoutów wierzcho³ków.*/
std::vector< ResourcePtr< ShaderInputLayout > > ResourceManager::ListShaderLayouts()
{
	return m_vertexLayout.List();
}

/**@brief Listowanie tekstur.*/
std::vector< ResourcePtr< TextureObject > >		ResourceManager::ListTextures()
{
	return m_texture.List();
}

/**@brief Listowanie vertex shaderów.*/
std::vector< ResourcePtr< VertexShader > >		ResourceManager::ListVertexShaders()
{
	return m_vertexShader.List();
}

/**@brief Listowanie pixel shaderów.*/
std::vector< ResourcePtr< PixelShader > >		ResourceManager::ListPixelShaders()
{
	return m_pixelShader.List();
}

/**@brief Listowanie render targetów.*/
std::vector< ResourcePtr< RenderTargetObject > > ResourceManager::ListRenderTargets()
{
	return m_renderTarget.List();
}

/**@brief Implementation of texture loading.

This is hack function. Resource manager have no texture loading function beacause
it needs separate library for this. Derived classes will implement it, but in future
this must change. ResourceManager must be fully operational class. Otherwise GUI 
won't load textures.*/
MemoryChunk ResourceManager::LoadTextureImpl( const filesystem::Path& filePath, TextureInfo& texInfo )
{
	assert( !"Implement me" );
	return MemoryChunk();
}


