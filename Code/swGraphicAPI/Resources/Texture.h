#pragma once
/**
@file Texture.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swCommonLib/Common/ObjectDeleter.h"
#include "swCommonLib/System/Path.h"
#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/MemoryChunk.h"

#include "swGraphicAPI/Resources/ResourcePtr.h"
#include "swGraphicAPI/Resources/ResourceObject.h"
#include "swGraphicAPI/Rendering/GraphicAPIConstants.h"


//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//

/**@defgroup Textures Textures
@ingroup Resources

@todo Rename TextureObject to Texture. TextureObject is too long name.
*/

/**@brief Textures filtering modes.

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





/** @brief Base class for textures.
@ingroup Textures
@ingroup Resources
@ingroup GraphicAPI

This class is platform independent. Inherit it while implementing chosen graphic API.*/
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

	virtual const filesystem::Path&		GetFilePath		() const = 0;		///< Returns name of file, from which this object was created.

	virtual MemoryChunk					CopyData		() const = 0;		///< Copies texture data from graphic card to memory chunk.
	virtual const TextureInfo&			GetDescriptor	() const = 0;		///< Gets texture descriptor.

	virtual std::string					GetResourceName	() const override { return GetFilePath().String(); }

	inline bool operator==( TextureObject& object );
	inline bool operator==( const std::wstring& file_name );
};

