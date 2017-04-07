/**
@file Texture.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swGraphicAPI/ResourceManager/stdafx.h"

#include "swGraphicAPI/Resources/Texture.h"



RTTR_REGISTRATION
{

	rttr::registration::enumeration< TextureType >( "TextureType" )
	(
		rttr::value( "Buffer", TextureType::TEXTURE_TYPE_BUFFER ),
		rttr::value( "Texture 1D", TextureType::TEXTURE_TYPE_TEXTURE1D ),
		rttr::value( "Texture 1D Array", TextureType::TEXTURE_TYPE_TEXTURE1D_ARRAY ),
		rttr::value( "Texture 2D", TextureType::TEXTURE_TYPE_TEXTURE2D ),
		rttr::value( "Texture 2D Array", TextureType::TEXTURE_TYPE_TEXTURE2D_ARRAY ),
		rttr::value( "Texture 2D Multisample", TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE ),
		rttr::value( "Texture 2D Multisample Array", TextureType::TEXTURE_TYPE_TEXTURE2D_MULTISAMPLE_ARRAY ),
		rttr::value( "Texture 3D", TextureType::TEXTURE_TYPE_TEXTURE3D )
	);

	rttr::registration::enumeration< MipMapFilter >( "MipMapFilter" )
	(
		rttr::value( "Bell", MipMapFilter::Bell ),
		rttr::value( "Blackman", MipMapFilter::Blackman ),
		rttr::value( "Box", MipMapFilter::Box ),
		rttr::value( "bSpline", MipMapFilter::bSpline ),
		rttr::value( "Catmullrom", MipMapFilter::Catmullrom ),
		rttr::value( "Gaussian", MipMapFilter::Gaussian ),
		rttr::value( "Kaiser", MipMapFilter::Kaiser ),
		rttr::value( "Lanczos12", MipMapFilter::Lanczos12 ),
		rttr::value( "Lanczos3", MipMapFilter::Lanczos3 ),
		rttr::value( "Lanczos4", MipMapFilter::Lanczos4 ),
		rttr::value( "Lanczos6", MipMapFilter::Lanczos6 ),
		rttr::value( "Mitchell", MipMapFilter::Mitchell ),
		rttr::value( "QuadraticAproximation", MipMapFilter::QuadraticAproximation ),
		rttr::value( "QuadraticInterpolation", MipMapFilter::QuadraticInterpolation ),
		rttr::value( "QuadraticMix", MipMapFilter::QuadraticMix ),
		rttr::value( "Tent", MipMapFilter::Tent ),
		rttr::value( "No filtering", MipMapFilter::Unknown )
	);

	rttr::registration::class_< TextureInfo >( "TextureInfo" )
		.property_readonly( "Width", &TextureInfo::GetWidth )
		.property_readonly( "Height", &TextureInfo::GetHeight )
		.property_readonly( "ArraySize", &TextureInfo::GetArraySize )
		.property_readonly( "CPUReadable", &TextureInfo::IsCPUReadable )
		.property_readonly( "CPUWritable", &TextureInfo::IsCPUWriteable )
		.property_readonly( "SharedResource", &TextureInfo::IsSharedResource )
		.property_readonly( "CubeMap", &TextureInfo::IsCubeMapTex )
		.property_readonly( "GeneratedMipMaps", &TextureInfo::GenMipMaps )
		.property_readonly( "TextureType", &TextureInfo::TextureType )
		.property_readonly( "Usage", &TextureInfo::Usage )
		.property_readonly( "Format", &TextureInfo::Format )
		.property_readonly( "MipMapFilter", &TextureInfo::MipMapFilter )
		.property_readonly( "MipMapsLevels", &TextureInfo::GetMipLevels )
		.property_readonly( "FilePath", &TextureInfo::GetPath );

	rttr::registration::class_< TextureObject >( "TextureObject" )
		.property_readonly( "Descriptor", &TextureObject::GetDescriptor ) BIND_AS_PTR;
}


//----------------------------------------------------------------------------------------------//
//								TextureObject													//
//----------------------------------------------------------------------------------------------//


/**@brief Porównuje ze soba dwa obiekty tekstur.

Obiekty s¹ takie same, kiedy odwo³uj¹ siê do tego samego pliku.
*/
bool		TextureObject::operator==( TextureObject& object )
{
	if( this->GetFilePath().String() == object.GetFilePath().String() )
		return true;
	return false;
}

/**@brief Porównuje ze soba dwa obiekty tekstur.

Obiekty s¹ takie same, kiedy odwo³uj¹ siê do tego samego pliku.
*/
bool		TextureObject::operator==( const std::wstring& fileName )
{
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	auto convertedFileName = converter.to_bytes( fileName );

	if( this->GetFilePath().String() == convertedFileName )
		return true;
	return false;
}

