#pragma once
/**
@file MeshResources.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracje formatów wierzcho³ków oraz klas zawieraj¹cych assety shadery itp.
*/


#include "swCommonLib/Common/ObjectDeleter.h"
#include "swCommonLib/System/Path.h"

#include "swGraphicAPI/Resources/ResourceObject.h"
#include "swGraphicAPI/Resources/IShader.h"
#include "swGraphicAPI/Resources/IBuffer.h"
#include "swGraphicAPI/Resources/IRenderTarget.h"
#include "swGraphicAPI/Resources/IShaderInputLayout.h"
#include "swGraphicAPI/Rendering/GraphicAPIConstants.h"
#include "swGraphicAPI/Resources/ResourcePtr.h"
#include "swGraphicAPI/Resources/BufferInitData.h"

#include <DirectXMath.h>


//definicje
/** @def WRONG_ID
B³êdny identyfikator assetu w klasie @ref ResourceObject.*/
#define WRONG_ID						0

/**@defgroup Resources Resources
@ingroup ResourcesManagment
@ingroup GraphicAPI
@brief Niskopoziomowe zasoby zwi¹zane z api graficznym.

Zasoby s¹ silnie zale¿ne od u¿ywanej platformy sprzêtowej. W celu oddzielenia referencji do
API graficznego od @ref EngineCore, wszystkie obiekty silnika u¿ywaj¹ jedynie klas bazowych, które
s¹ implementowane przez poszczególne API graficzne. Aby zobaczyæ konkretne implementacje tych klas
zobacz @ref GraphicAPI.

Zasoby nigdy nie s¹ tworzone bezpoœrednio. Zamiast tego u¿ywa siê klasy @ref ResourcesFactory, któr¹
implementuje konkretne API graficzne.

Poniewa¿ zasoby mog¹ byæ wspó³dzielone przez wiele obiektów w silniku, istnieje mechanizm zliczania
odwo³añ do obiektów implementowany przez klasê @ref ResourceObject.*/

class AssetsManager;
struct ModelPart;
class BufferObject;
class TextureObject;
class VertexShader;
class PixelShader;
struct MeshPartObject;
struct MaterialObject;

/*
// W buforze wierzcho³ków znajduj¹ siê elementy typu VERT_INDEX.
// Definicja typu VERT_INDEX znajduje siê w pliku macros_switches.h i wygl¹da tak:

#if defined(INDEX_BUFFER_UINT16)
typedef UINT16 VERT_INDEX;
#elif defined(INDEX_BUFFER_UINT32)
typedef UINT32 VERT_INDEX;
#else
typedef UINT32 VERT_INDEX;
#endif

*/


static const std::wstring RENDER_TARGET_COLOR_BUFFER_NAME = L"::color";
static const std::wstring RENDER_TARGET_DEPTH_BUFFER_NAME = L"::depth";
static const std::wstring RENDER_TARGET_STENCIL_BUFFER_NAME = L"::stencil";


typedef uint16 Index16;
typedef uint32 Index32;
typedef uint8 Index8;


/**@brief Meaning of texture indicies.

@ingroup Textures

These are values used by default shaders. You don't have to use this convention
when you write your own shaders.*/
enum TextureUse
{
	TEX_DIFFUSE			///<Tekstura dla kana³u diffuse
	, TEX_SPECULAR		///<Tekstura dla kana³u specular
	, TEX_EMISSIVE		///< Texture for emmisive channel
	, TEX_BUMP_MAP		///<Bump mapa
	, TEX_DISPLACEMENT_MAP		///<Tekstura przemieszczeñ wierzcho³ków, w przypadku u¿ywania teselacji wierzcho³ków
	, TEX_OTHER1		///<Tekstura o dowolnym znaczeniu
	, TEX_OTHER2		///<Tekstura o dowolnym znaczeniu
	, TEX_LIGHTMAP		///<Lightmapa
};




//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//

/**@defgroup Textures Textures
@ingroup Resources
*/

/**@brief Tryby filtrowania tekstur.

Poni¿sze tryby filtrowania s¹ u¿ywane przy tworzeniu mipmap.
Istnieje jeszcze drugi etap filtrowania przy próbkowania w pixel shaderze,
do którego odnosi siê inna enumeracja.

@ingroup Textures
*/
enum class MipMapFilter : short
{
	Box = 0,		///<
	Tent,			///< 
	Bell,			///<
	bSpline,		///<
	Mitchell,		///<
	Lanczos3,		///<
	Blackman,		///<
	Lanczos4,		///<
	Lanczos6,		///<
	Lanczos12,		///<
	Kaiser,			///<
	Gaussian,		///<
	Catmullrom,		///<
	QuadraticInterpolation,		///<
	QuadraticAproximation,		///<
	QuadraticMix,				///<

	Unknown						///<
};


/**@brief Deskryptor tekstury.

@ingroup Textures
*/
struct TextureInfo
{
	uint16				TextureWidth;				///< Szerokoœæ tekstury w pikselach.
	uint16				TextureHeight;				///< Wysokoœæ tekstury w pikselach.
	uint16				ArraySize;					///< Liczba elementów tablicy.
	bool				CPURead : 1;				///< Pozwala na odczyt tekstury przez CPU.
	bool				CPUWrite : 1;				///< Pozwala na zapis tekstury przez CPU.
	bool				AllowShareResource : 1;		///< Pozwala na dostêp do zasoby z wielu API graficznych i pomiêdzy kontekstami.
	bool				IsCubeMap : 1;				///< Nale¿y ustawiæ je¿eli tekstura jest cubemap¹.
	bool				GenerateMipMaps : 1;		///< Automatyczne generowanie mipmap.
	TextureType			TextureType;				///< Typ tekstury (liczba wymiarów, multsampling). Na razie tekstura nie mo¿e byæ inna ni¿ dwuwymiarowa (mo¿e byæ tablic¹).
	ResourceUsage		Usage;						///< Sposób u¿ycia render targetu. Wp³ywa na optymalizacje u³o¿enia w pamiêci.
	ResourceFormat		Format;						///< Format tekstury (liczba kana³ów, liczba bitów na kana³ itp)
	MipMapFilter		MipMapFilter;				///< Tryb filtrowania tekstury. U¿ywany tylko je¿eli ustawiono GenerateMipMaps na true.
	uint16				MipMapLevels;				///< Liczba poziomów mipmap. 1 oznacza tylko teksturê oryginaln¹.
	uint16				CutOffMipMaps;				///< Usuwa podan¹ liczbê poziomów mipmap. Przydatne gdy nie potrzebujemy tekstur zbyt wysokiej rozdzielczoœci (np. stosuj¹c dynamiczny LoD).
													///< Ustawienie wartoœci 1 oznacza, ¿e oryginalna tekstura zostanie zast¹piona pierwsz¹ mipmap¹ w kolejnoœci.

	uint32				MemorySize;					///< Pamiêæ zajmowana przez teksturê.
	filesystem::Path	FilePath;					///< Œcie¿ka do pliku z tekstur¹ lub jej nazwa.
	
	TextureInfo()
	{
		ArraySize = 1;
		CPURead = false;
		CPUWrite = false;
		AllowShareResource = false;
		IsCubeMap = false;
		GenerateMipMaps = false;
		Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;
		MipMapFilter = MipMapFilter::Unknown;
		MipMapLevels = 1;
		CutOffMipMaps = 0;
		TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
		MemorySize = 0;
	}

private:
	RTTR_REGISTRATION_FRIEND;
	
	int		GetWidth		()		{ return TextureWidth; }
	int		GetHeight		()		{ return TextureHeight; }
	int		GetArraySize	()		{ return ArraySize; }
	bool	IsCPUReadable	()		{ return CPURead; }
	bool	IsCPUWriteable	()		{ return CPUWrite; }
	bool	IsSharedResource()		{ return AllowShareResource; }
	bool	IsCubeMapTex	()		{ return IsCubeMap; }
	bool	GenMipMaps		()		{ return GenerateMipMaps; }
	int		GetMipLevels	()		{ return MipMapLevels; }

	std::string		GetPath	()		{ return FilePath.String(); }
};

/** @brief Klasa przechowuj¹ca tekstury.
@ingroup Textures
@ingroup Resources
@ingroup GraphicAPI

Klasa bazowa, która bêdzie u¿ywana przez obiekty silnika.
Powinny po niej odziedziczyæ obiekty konkretnego API graficznego,
¿eby zaimplementowaæ najwa¿niejsze funkcjonalnoœci.*/
class TextureObject : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	friend ObjectDeleter<TextureObject>;
private:
protected:
	/// ¯eby unikn¹æ pomy³ki, obiekt mo¿e byœ kasowany tylko przez AssetsManager. Zapewnia to ObjectDeleter.
	virtual ~TextureObject() = default;
public:
	TextureObject() : ResourceObject( 0 ) {}

	virtual const filesystem::Path&		GetFilePath		() const = 0;		///<Zwraca nazwê pliku, który pos³u¿y³ do stworzenia obiektu

	virtual MemoryChunk					CopyData		() const = 0;		///<Kopiuje dane z bufora i umieszcza je w zwracanym MemoryChunku.
	virtual const TextureInfo&			GetDescriptor	() const = 0;		///<Pozwala pobraæ deskrytpro tekstury.

	virtual std::string					GetResourceName	() const override { return GetFilePath().String(); }

	inline bool operator==( TextureObject& object );
	inline bool operator==( const std::wstring& file_name );
};

//----------------------------------------------------------------------------------------------//
//								RenderTargetObject												//
//----------------------------------------------------------------------------------------------//

/**@defgroup RenderTargets RenderTargets
@ingroup Resources
*/

/**@brief Struktura u¿ywana do tworzenia render targetu.
@ingroup RenderTargets*/
struct RenderTargetDescriptor
{
	uint16				TextureWidth;				///<Szerokoœæ tekstury w pikselach.
	uint16				TextureHeight;				///<Wysokoœæ tekstury w pikselach.
	uint16				ArraySize;					///<Liczba elementów tablicy.
	bool				CPURead : 1;				///<Pozwala na odczyt tekstury przez CPU.
	bool				CPUWrite : 1;				///<Pozwala na zapis tekstury przez CPU.
	bool				AllowShareResource : 1;		///<Pozwala na dostêp do zasoby z wielu API graficznych i pomiêdzy kontekstami.
	bool				IsCubeMap : 1;				///<Nale¿y ustawiæ je¿eli tekstura jest cubemap¹.
	uint8				NumSamples;					///<Liczba próbek w przypadku stosowania multisamplingu.
	uint16				SamplesQuality;				///<Jakoœæ próbek przy multisamplingu.
	TextureType			TextureType;				///<Typ tekstury (liczba wymiarów, multsampling). Tekstura nie mo¿e byæ inna ni¿ dwuwymiarowa (mo¿e byæ tablic¹).
	ResourceFormat		ColorBuffFormat;			///<Format bufora kolorów.
	DepthStencilFormat	DepthStencilFormat;			///<Format bufora g³êbokoœci i stencilu.
	ResourceUsage		Usage;						///<Sposób u¿ycia render targetu. Wp³ywa na optymalizacje u³o¿enia w pamiêci.

	/**@brief Ustawia domyœlne wartoœci deskryptora.
	
	Ustawiane s¹ pola CPURead, CPUWrite, AllowShareResource, IsCubeMap, Usage.
	Te zmienne s¹ u¿ywane rzadko i dlatego powinny mieæ takie wartoœci, ¿eby nie trzeba by³o ich jawnie ustawiaæ.
	Pozosta³e wartoœci u¿ytkownik i tak musi zdefiniowaæ samemu, wiêc nie ma co nadk³adaæ pracy.
	
	Pola NumSamples i SamplesQuality s¹ ignorowane, je¿eli TextureType nie zosta³ ustawiony na teksturê z multisamplingiem.
	Pole ArraySize jest ignorowane, je¿eli tekstura nie jest tablic¹.*/
	RenderTargetDescriptor()
	{
		ArraySize = 1;
		CPURead = 0;
		CPUWrite = 0;
		AllowShareResource = 0;
		IsCubeMap = 0;
		Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;
	}

	/**@brief Tworzy strukture TextureInfo wype³nion¹ danymi zgodnymi z deskryptorem RenderTargetu.
	
	@attention Funkcja nie ustawia formatu tekstury. Nie da siê wywnioskowaæ formatu na podstawie deskryptora.*/
	TextureInfo		CreateTextureInfo() const
	{
		TextureInfo texInfo;
		texInfo.TextureWidth = TextureWidth;
		texInfo.TextureHeight = TextureHeight;
		texInfo.ArraySize = ArraySize;
		texInfo.CPURead = CPURead;
		texInfo.CPUWrite = CPUWrite;
		texInfo.AllowShareResource = AllowShareResource;
		texInfo.IsCubeMap = IsCubeMap;
		texInfo.TextureType = TextureType;
		texInfo.Usage = Usage;

		return texInfo;
	}
};

/**@brief Klasa dla render targetów.
@ingroup RenderTargets
@ingroup Resources
@ingroup GraphicAPI

Klasa umo¿liwia pobranie jednej z tekstur sk³adowych i udostêpnienie dla shaderów.
Je¿eli API graficzne nie pozwala na oddzielne trzymanie bufora g³êbokoœci i stencilu,
to mo¿e tu byæ przechowywany ten sam obiekt. Ewentualnie mog¹ byæ to dwa obiekty,
które przechowuj¹ inny widok, ale fizycznie odwo³uj¹ siê do tej samej pamiêci.*/
class RenderTargetObject : public IRenderTarget
{
	RTTR_ENABLE( IRenderTarget );
	RTTR_REGISTRATION_FRIEND;
private:
protected:
	ResourcePtr< TextureObject >	m_colorBuffer;			///<Pozwala na dostêp do bufora kolorów dla innych obiektów. Mo¿e byæ nullptrem.
	ResourcePtr< TextureObject >	m_depthBuffer;			///<Pozwala na dostêp do bufora g³êbokoœci. Mo¿e byæ nullptrem.
	ResourcePtr< TextureObject >	m_stencilBuffer;		///<Pozwala na dostêp do bufora stencil. Mo¿e byæ nulltrem.
public:
	RenderTargetObject( TextureObject* colorBuffer, TextureObject* depthBuffer, TextureObject* stencilBuffer );
	virtual ~RenderTargetObject();

	inline TextureObject*		GetColorBuffer()			{ return m_colorBuffer.Ptr(); }		///<Zwraca obiekt bufora kolorów.
	inline TextureObject*		GetDepthBuffer()			{ return m_depthBuffer.Ptr(); }		///<Zwraca obiekt bufora g³êbokoœci.
	inline TextureObject*		GetStencilBuffer()			{ return m_stencilBuffer.Ptr(); }		///<Zwraca obiekt bufora stencilu.

	virtual std::string			GetResourceName	() const override;	///<@todo RenderTargety powinny mieæ swoje nazwy.
};

//----------------------------------------------------------------------------------------------//
//								ShaderInputLayout												//
//----------------------------------------------------------------------------------------------//

/**@defgroup Shaders Shaders
@ingroup Resources
*/

/**@brief Klasa przechowuje layout wierzcho³ka trafiaj¹cego do
vertex shadera.
@ingroup Shaders
@ingroup Buffers
@ingroup Resources*/
class ShaderInputLayout : public IShaderInputLayout
{
	RTTR_ENABLE( IShaderInputLayout );
	friend ObjectDeleter< ShaderInputLayout >;
private:
protected:
	virtual ~ShaderInputLayout() = default;
public:
	ShaderInputLayout() = default;

	virtual std::string			GetResourceName	() const override { return ""; }
};

/**@brief Klasa przechowuje opis layoutu wierzcho³ka, na podstawie którego
tworzony jest obiekt layoutu.
@ingroup Buffers
@ingroup Shaders
@ingroup Resources*/
class InputLayoutDescriptor
{
private:
	std::wstring				m_inputLayoutName;
protected:
public:
	InputLayoutDescriptor( const std::wstring& layoutName ) : m_inputLayoutName( layoutName ){}
	virtual ~InputLayoutDescriptor() = default;

	virtual void		AddRow		( const char* semanticName, ResourceFormat format, unsigned int inputSlot, unsigned int byteOffset, bool perInstance, unsigned int instanceDataStep ) = 0;

	std::wstring&		GetName		() { return m_inputLayoutName; }
};


/**@brief Typ shadera.
@ingroup Shaders*/
enum class ShaderType : uint8
{
	VertexShader				= 0x01,
	PixelShader					= 0x02,
	GeometryShader				= 0x04,
	TesselationControlShader	= 0x08,
	TesselationEvaluationShader	= 0x10,
	ComputeShader				= 0x20
};

//----------------------------------------------------------------------------------------------//
//								VertexShader													//
//----------------------------------------------------------------------------------------------//

/** @brief Klasa przechowuj¹ca vertex shader.
@ingroup Shaders
@ingroup Resources
@ingroup GraphicAPI*/
class VertexShader : public IShader
{
	RTTR_ENABLE( IShader );
	friend ObjectDeleter< VertexShader >;
private:
protected:
	~VertexShader() = default;
public:
	VertexShader() = default;

	virtual std::string			GetResourceName	() const override { return ""; }
};

//----------------------------------------------------------------------------------------------//
//								PixelShader														//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Shaders
@ingroup Resources
@ingroup GraphicAPI*/
class PixelShader : public IShader
{
	RTTR_ENABLE( IShader );
	friend ObjectDeleter< PixelShader >;
private:
protected:
	~PixelShader() = default;
public:
	PixelShader() = default;

	virtual std::string			GetResourceName	() const override { return ""; }
};

//----------------------------------------------------------------------------------------------//
//								GeometryShader													//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Shaders
@ingroup Resources
@ingroup GraphicAPI*/
class GeometryShader : public IShader
{
	RTTR_ENABLE( IShader );
	friend ObjectDeleter<GeometryShader>;
private:
protected:
	~GeometryShader() = default;
public:
	GeometryShader() = default;

	virtual std::string			GetResourceName	() const override { return ""; }
};

//----------------------------------------------------------------------------------------------//
//								ControlShader													//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Shaders
@ingroup Resources
@ingroup GraphicAPI*/
class ControlShader : public IShader
{
	RTTR_ENABLE( IShader );
	friend ObjectDeleter< ControlShader >;
private:
protected:
	~ControlShader() = default;
public:
	ControlShader() = default;

	virtual std::string			GetResourceName	() const override { return ""; }
};

//----------------------------------------------------------------------------------------------//
//								EvaluationShader												//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca pixel shader.
@ingroup Shaders
@ingroup Resources
@ingroup GraphicAPI*/
class EvaluationShader : public IShader
{
	RTTR_ENABLE( IShader );
	friend ObjectDeleter< EvaluationShader >;
private:
protected:
	~EvaluationShader() = default;
public:
	EvaluationShader() = default;

	virtual std::string			GetResourceName	() const override { return ""; }
};

//----------------------------------------------------------------------------------------------//
//								ComputeShader													//
//----------------------------------------------------------------------------------------------//

/**@brief Klasa przechowuj¹ca compute shader
@ingroup Shaders
@ingroup Resources
@ingroup GraphicAPI*/
class ComputeShader : public IShader
{
	RTTR_ENABLE( IShader );
	friend ObjectDeleter<ComputeShader>;
private:
protected:
	~ComputeShader() = default;
public:
	ComputeShader() = default;

	virtual std::string			GetResourceName	() const override { return ""; }
};

//----------------------------------------------------------------------------------------------//
//								BufferObject													//
//----------------------------------------------------------------------------------------------//

/**@brief Obiekt opakowuj¹cy bufor.
@ingroup Buffers
@ingroup Resources
@ingroup GraphicAPI

Bufor mo¿e byæ zarówno buforem wierzcho³ków, indeksów jak i sta³ych.
*/
class BufferObject : public IBuffer
{
	RTTR_ENABLE( IBuffer )
	friend ObjectDeleter<BufferObject>;
protected:
	unsigned int		m_elementSize;			///<Rozmiar elementu.
	unsigned int		m_elementCount;			///<Liczba elementów.

	~BufferObject() = default;
public:
	BufferObject( unsigned int elementSize, unsigned int elementCount );

	inline unsigned int GetStride()				{ return m_elementSize; }		///<Zwraca rozmiar pojedynczego elementu w buforze.
	inline unsigned int	GetElementSize()		{ return m_elementSize; }		///<Zwraca rozmiar pojedynczego elementu w buforze.
	inline unsigned int GetElementCount()		{ return m_elementCount; }		///<Zwraca liczbê elementów w buforze.

	virtual std::string	GetResourceName	() const override { return GetDescriptor().GetName(); }
};

//----------------------------------------------------------------------------------------------//
//								MaterialObject													//
//----------------------------------------------------------------------------------------------//


/**@brief Struktura przechowuj¹ca materia³.
@ingroup Resources

@deprecated New material structure is coming soon

DirectX 11 nie ma w³asnych obiektów na materia³y, poniewa¿ nie ma ju¿ domyœlnego
potoku przetwarzania na karcie graficznej. Na wszystko trzeba napisaæ shader i dostarcza
mu siê takie dane, jakie siê chce dostarczyæ. Dlatego informacja o materia³ach bêdzie
przekazywana z buforze sta³ych.

Struktura zachowuje siê jak asset w zwi¹zku z czym mo¿e
byæ wspó³dzielona przez wiele obiektów. Z tego wzglêdu nie mo¿na jej u¿yæ bezpoœrednio w ConstantPerMesh,
poniewa¿ nie chcemy przekazywaæ do bufora sta³ych zmiennych odziedziczonych po ResourceObject.
Zamiast tego trzeba tê strukture przepisaæ.

Zwracam uwagê, ¿e tylko kana³ Diffuse jest wektorem 4 wymiarowym, w którym sk³adowa w jest odpowiedzialna
za przezroczystoœæ. Pozosta³e s¹ takie tylko dlatego, ¿e jest to domyœlny format przechowywania danych 
w rejestrach karty graficznej i przyspiesza to operacjê kopiowania.
@see ConstantPerFrame
*/
struct MaterialObject : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
public:
	friend ObjectDeleter<MaterialObject>;

	DirectX::XMFLOAT4		Diffuse;		//Sk³adowa przezroczystoœci odnosi siê do ca³ego materia³u
	DirectX::XMFLOAT4		Ambient;
	DirectX::XMFLOAT4		Specular;
	DirectX::XMFLOAT4		Emissive;
	float					Power;

	MaterialObject( unsigned int id = WRONG_ID ) : ResourceObject( id ){}
	MaterialObject( const MaterialObject* material );

	void SetNullMaterial();

	virtual std::string	GetResourceName	() const override { return ""; }
};


