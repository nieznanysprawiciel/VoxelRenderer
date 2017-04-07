/**
@file MeshResources.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/ResourceManager/stdafx.h"


#include "swGraphicAPI/Resources/ResourcesFactory.h"
#include "RasterizerState.h"
#include "BlendingState.h"
#include "DepthStencilState.h"

#include "swCommonLib/Common/MemoryLeaks.h"



RTTR_REGISTRATION
{
	RTTR_REGISTRATION_STANDARD_TYPE_VARIANTS( std::wstring );

//====================================================================================//
//			Enumarations	
//====================================================================================//
	

	rttr::registration::enumeration< ResourceUsage >( "ResourceUsage" )
	(
		rttr::value( "Default", ResourceUsage::RESOURCE_USAGE_DEFAULT ),
		rttr::value( "Dynamic", ResourceUsage::RESOURCE_USAGE_DYNAMIC ),
		rttr::value( "Staging", ResourceUsage::RESOURCE_USAGE_STAGING ),
		rttr::value( "Static", ResourceUsage::RESOURCE_USAGE_STATIC )
	);

	rttr::registration::enumeration< BlendOperation >( "BlendOperation" )
	(
		rttr::value( "Add", BlendOperation::Add ),
		rttr::value( "Subtract", BlendOperation::Subtract ),
		rttr::value( "ReverseSubtract", BlendOperation::ReverseSubtract ),
		rttr::value( "Min", BlendOperation::Min ),
		rttr::value( "Max", BlendOperation::Max )
	);


	rttr::registration::enumeration< BlendFactor >( "BlendFactor" )
	(
		rttr::value( "Zero", BlendFactor::Zero ),
		rttr::value( "One", BlendFactor::One ),
		rttr::value( "SrcColor", BlendFactor::SrcColor ),
		rttr::value( "DstColor", BlendFactor::DstColor ),
		rttr::value( "SrcAlpha", BlendFactor::SrcAlpha ),
		rttr::value( "DstAlpha", BlendFactor::DstAlpha ),
		rttr::value( "BlendFactor", BlendFactor::BlendFactor ),
		rttr::value( "InverseSrcColor", BlendFactor::InverseSrcColor ),
		rttr::value( "InverseDstColor", BlendFactor::InverseDstColor ),
		rttr::value( "InverseSrcAlpha", BlendFactor::InverseSrcAlpha ),
		rttr::value( "InverseDstAlpha", BlendFactor::InverseDstAlpha ),
		rttr::value( "InverseBlendFactor", BlendFactor::InverseBlendFactor )
	);

	rttr::registration::enumeration< CullMode >( "CullMode" )
	(
		rttr::value( "Front", CullMode::Front ),
		rttr::value( "Back", CullMode::Back ),
		rttr::value( "None", CullMode::None )
	);

	rttr::registration::enumeration< FillMode >( "FillMode" )
	(
		rttr::value( "Solid", FillMode::Solid ),
		rttr::value( "Wireframe", FillMode::Wireframe )
	);

	rttr::registration::enumeration< ResourceFormat >( "ResourceFormat" )
	(
		rttr::value( "Unknown", ResourceFormat::RESOURCE_FORMAT_UNKNOWN ),
		rttr::value( "R32G32B32A32_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R32G32B32A32_TYPELESS ),
		rttr::value( "R32G32B32A32_UINT", ResourceFormat::RESOURCE_FORMAT_R32G32B32A32_FLOAT ),
		rttr::value( "R32G32B32A32_UINT", ResourceFormat::RESOURCE_FORMAT_R32G32B32A32_UINT ),
		rttr::value( "R32G32B32A32_SINT", ResourceFormat::RESOURCE_FORMAT_R32G32B32A32_SINT ),
		rttr::value( "R32G32B32_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R32G32B32_TYPELESS ),
		rttr::value( "R32G32B32_FLOAT", ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT ),
		rttr::value( "R32G32B32_UINT", ResourceFormat::RESOURCE_FORMAT_R32G32B32_UINT ),
		rttr::value( "R32G32B32_SINT", ResourceFormat::RESOURCE_FORMAT_R32G32B32_SINT ),
		rttr::value( "R16G16B16A16_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R16G16B16A16_TYPELESS ),
		rttr::value( "R16G16B16A16_FLOAT", ResourceFormat::RESOURCE_FORMAT_R16G16B16A16_FLOAT ),
		rttr::value( "R16G16B16A16_UNORM", ResourceFormat::RESOURCE_FORMAT_R16G16B16A16_UNORM ),
		rttr::value( "R16G16B16A16_UINT", ResourceFormat::RESOURCE_FORMAT_R16G16B16A16_UINT ),
		rttr::value( "R16G16B16A16_SNORM", ResourceFormat::RESOURCE_FORMAT_R16G16B16A16_SNORM ),
		rttr::value( "R16G16B16A16_SINT", ResourceFormat::RESOURCE_FORMAT_R16G16B16A16_SINT ),
		rttr::value( "R32G32_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R32G32_TYPELESS ),
		rttr::value( "R32G32_FLOAT", ResourceFormat::RESOURCE_FORMAT_R32G32_FLOAT ),
		rttr::value( "R32G32_UINT", ResourceFormat::RESOURCE_FORMAT_R32G32_UINT ),
		rttr::value( "R32G32_SINT", ResourceFormat::RESOURCE_FORMAT_R32G32_SINT ),
		rttr::value( "R32G8X24_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R32G8X24_TYPELESS ),
		rttr::value( "D32_FLOAT_S8X24_UINT", ResourceFormat::RESOURCE_FORMAT_D32_FLOAT_S8X24_UINT ),
		rttr::value( "R32_FLOAT_X8X24_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R32_FLOAT_X8X24_TYPELESS ),
		rttr::value( "X32_TYPELESS_G8X24_UINT", ResourceFormat::RESOURCE_FORMAT_X32_TYPELESS_G8X24_UINT ),
		rttr::value( "R10G10B10A2_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R10G10B10A2_TYPELESS ),
		rttr::value( "R10G10B10A2_UNORM", ResourceFormat::RESOURCE_FORMAT_R10G10B10A2_UNORM ),
		rttr::value( "R10G10B10A2_UINT", ResourceFormat::RESOURCE_FORMAT_R10G10B10A2_UINT ),
		rttr::value( "R11G11B10_FLOAT", ResourceFormat::RESOURCE_FORMAT_R11G11B10_FLOAT ),
		rttr::value( "R8G8B8A8_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_TYPELESS ),
		rttr::value( "R8G8B8A8_UNORM", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM ),
		rttr::value( "R8G8B8A8_UNORM_SRGB", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM_SRGB ),
		rttr::value( "R8G8B8A8_UINT", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UINT ),
		rttr::value( "R8G8B8A8_SNORM", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_SNORM ),
		rttr::value( "R8G8B8A8_SINT", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_SINT ),
		rttr::value( "R16G16_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R16G16_TYPELESS ),
		rttr::value( "R16G16_FLOAT", ResourceFormat::RESOURCE_FORMAT_R16G16_FLOAT ),
		rttr::value( "R16G16_UNORM", ResourceFormat::RESOURCE_FORMAT_R16G16_UNORM ),
		rttr::value( "R16G16_UINT", ResourceFormat::RESOURCE_FORMAT_R16G16_UINT ),
		rttr::value( "R16G16_SNORM", ResourceFormat::RESOURCE_FORMAT_R16G16_SNORM ),
		rttr::value( "R16G16_SINT", ResourceFormat::RESOURCE_FORMAT_R16G16_SINT ),
		rttr::value( "R32_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R32_TYPELESS ),
		rttr::value( "D32_FLOAT", ResourceFormat::RESOURCE_FORMAT_D32_FLOAT ),
		rttr::value( "R32_FLOAT", ResourceFormat::RESOURCE_FORMAT_R32_FLOAT ),
		rttr::value( "R32_UINT", ResourceFormat::RESOURCE_FORMAT_R32_UINT ),
		rttr::value( "R32_SINT", ResourceFormat::RESOURCE_FORMAT_R32_SINT ),
		rttr::value( "R24G8_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R24G8_TYPELESS ),
		rttr::value( "D24_UNORM_S8_UINT", ResourceFormat::RESOURCE_FORMAT_D24_UNORM_S8_UINT ),
		rttr::value( "R24_UNORM_X8_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R24_UNORM_X8_TYPELESS ),
		rttr::value( "X24_TYPELESS_G8_UINT", ResourceFormat::RESOURCE_FORMAT_X24_TYPELESS_G8_UINT ),
		rttr::value( "R8G8_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R8G8_TYPELESS ),
		rttr::value( "R8G8_UNORM", ResourceFormat::RESOURCE_FORMAT_R8G8_UNORM ),
		rttr::value( "R8G8_UINT", ResourceFormat::RESOURCE_FORMAT_R8G8_UINT ),
		rttr::value( "R8G8_SNORM", ResourceFormat::RESOURCE_FORMAT_R8G8_SNORM ),
		rttr::value( "R8G8_SINT", ResourceFormat::RESOURCE_FORMAT_R8G8_SINT ),
		rttr::value( "R16_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R16_TYPELESS ),
		rttr::value( "R16_FLOAT", ResourceFormat::RESOURCE_FORMAT_R16_FLOAT ),
		rttr::value( "D16_UNORM", ResourceFormat::RESOURCE_FORMAT_D16_UNORM ),
		rttr::value( "R16_UNORM", ResourceFormat::RESOURCE_FORMAT_R16_UNORM ),
		rttr::value( "R16_UINT", ResourceFormat::RESOURCE_FORMAT_R16_UINT ),
		rttr::value( "R16_SNORM", ResourceFormat::RESOURCE_FORMAT_R16_SNORM ),
		rttr::value( "R16_SINT", ResourceFormat::RESOURCE_FORMAT_R16_SINT ),
		rttr::value( "R8_TYPELESS", ResourceFormat::RESOURCE_FORMAT_R8_TYPELESS ),
		rttr::value( "R8_UNORM", ResourceFormat::RESOURCE_FORMAT_R8_UNORM ),
		rttr::value( "R8_UINT", ResourceFormat::RESOURCE_FORMAT_R8_UINT ),
		rttr::value( "R8_SNORM", ResourceFormat::RESOURCE_FORMAT_R8_SNORM ),
		rttr::value( "R8_SINT", ResourceFormat::RESOURCE_FORMAT_R8_SINT ),
		rttr::value( "A8_UNORM", ResourceFormat::RESOURCE_FORMAT_A8_UNORM ),
		rttr::value( "R1_UNORM", ResourceFormat::RESOURCE_FORMAT_R1_UNORM ),
		rttr::value( "R9G9B9E5_SHAREDEXP", ResourceFormat::RESOURCE_FORMAT_R9G9B9E5_SHAREDEXP ),
		rttr::value( "R8G8_B8G8_UNORM", ResourceFormat::RESOURCE_FORMAT_R8G8_B8G8_UNORM ),
		rttr::value( "G8R8_G8B8_UNORM", ResourceFormat::RESOURCE_FORMAT_G8R8_G8B8_UNORM ),
		rttr::value( "BC1_TYPELESS", ResourceFormat::RESOURCE_FORMAT_BC1_TYPELESS ),
		rttr::value( "BC1_UNORM", ResourceFormat::RESOURCE_FORMAT_BC1_UNORM ),
		rttr::value( "BC1_UNORM_SRGB", ResourceFormat::RESOURCE_FORMAT_BC1_UNORM_SRGB ),
		rttr::value( "BC2_TYPELESS", ResourceFormat::RESOURCE_FORMAT_BC2_TYPELESS ),
		rttr::value( "BC2_UNORM", ResourceFormat::RESOURCE_FORMAT_BC2_UNORM ),
		rttr::value( "BC2_UNORM_SRGB", ResourceFormat::RESOURCE_FORMAT_BC2_UNORM_SRGB ),
		rttr::value( "BC3_TYPELESS", ResourceFormat::RESOURCE_FORMAT_BC3_TYPELESS ),
		rttr::value( "BC3_UNORM", ResourceFormat::RESOURCE_FORMAT_BC3_UNORM ),
		rttr::value( "BC3_UNORM_SRGB", ResourceFormat::RESOURCE_FORMAT_BC3_UNORM_SRGB ),
		rttr::value( "BC4_TYPELESS", ResourceFormat::RESOURCE_FORMAT_BC4_TYPELESS ),
		rttr::value( "FORMAT_BC4_UNORM", ResourceFormat::RESOURCE_FORMAT_BC4_UNORM ),
		rttr::value( "BC4_SNORM", ResourceFormat::RESOURCE_FORMAT_BC4_SNORM ),
		rttr::value( "BC5_TYPELESS", ResourceFormat::RESOURCE_FORMAT_BC5_TYPELESS ),
		rttr::value( "BC5_UNORM", ResourceFormat::RESOURCE_FORMAT_BC5_UNORM ),
		rttr::value( "BC5_SNORM", ResourceFormat::RESOURCE_FORMAT_BC5_SNORM ),
		rttr::value( "B5G6R5_UNORM", ResourceFormat::RESOURCE_FORMAT_B5G6R5_UNORM ),
		rttr::value( "B5G5R5A1_UNORM", ResourceFormat::RESOURCE_FORMAT_B5G5R5A1_UNORM ),
		rttr::value( "B8G8R8A8_UNORM", ResourceFormat::RESOURCE_FORMAT_B8G8R8A8_UNORM ),
		rttr::value( "B8G8R8X8_UNORM", ResourceFormat::RESOURCE_FORMAT_B8G8R8X8_UNORM ),
		rttr::value( "R10G10B10_XR_BIAS_A2_UNORM", ResourceFormat::RESOURCE_FORMAT_R10G10B10_XR_BIAS_A2_UNORM ),
		rttr::value( "B8G8R8A8_TYPELESS", ResourceFormat::RESOURCE_FORMAT_B8G8R8A8_TYPELESS ),
		rttr::value( "B8G8R8A8_UNORM_SRGB", ResourceFormat::RESOURCE_FORMAT_B8G8R8A8_UNORM_SRGB ),
		rttr::value( "B8G8R8X8_TYPELESS", ResourceFormat::RESOURCE_FORMAT_B8G8R8X8_TYPELESS ),
		rttr::value( "B8G8R8X8_UNORM_SRGB", ResourceFormat::RESOURCE_FORMAT_B8G8R8X8_UNORM_SRGB ),
		rttr::value( "BC6H_TYPELESS", ResourceFormat::RESOURCE_FORMAT_BC6H_TYPELESS ),
		rttr::value( "BC6H_UF16", ResourceFormat::RESOURCE_FORMAT_BC6H_UF16 ),
		rttr::value( "BC6H_SF16", ResourceFormat::RESOURCE_FORMAT_BC6H_SF16 ),
		rttr::value( "BC7_TYPELESS", ResourceFormat::RESOURCE_FORMAT_BC7_TYPELESS ),
		rttr::value( "BC7_UNORM", ResourceFormat::RESOURCE_FORMAT_BC7_UNORM ),
		rttr::value( "BC7_UNORM_SRGB", ResourceFormat::RESOURCE_FORMAT_BC7_UNORM_SRGB ),
		rttr::value( "AYUV", ResourceFormat::RESOURCE_FORMAT_AYUV ),
		rttr::value( "Y410", ResourceFormat::RESOURCE_FORMAT_Y410 ),
		rttr::value( "Y416", ResourceFormat::RESOURCE_FORMAT_Y416 ),
		rttr::value( "NV12", ResourceFormat::RESOURCE_FORMAT_NV12 ),
		rttr::value( "P010", ResourceFormat::RESOURCE_FORMAT_P010 ),
		rttr::value( "P016", ResourceFormat::RESOURCE_FORMAT_P016 ),
		rttr::value( "420_OPAQUE", ResourceFormat::RESOURCE_FORMAT_420_OPAQUE ),
		rttr::value( "YUY2", ResourceFormat::RESOURCE_FORMAT_YUY2 ),
		rttr::value( "Y210", ResourceFormat::RESOURCE_FORMAT_Y210 ),
		rttr::value( "Y216", ResourceFormat::RESOURCE_FORMAT_Y216 ),
		rttr::value( "NV11", ResourceFormat::RESOURCE_FORMAT_NV11 ),
		rttr::value( "AI44", ResourceFormat::RESOURCE_FORMAT_AI44 ),
		rttr::value( "IA44", ResourceFormat::RESOURCE_FORMAT_IA44 ),
		rttr::value( "P8", ResourceFormat::RESOURCE_FORMAT_P8 ),
		rttr::value( "A8P8", ResourceFormat::RESOURCE_FORMAT_A8P8 ),
		rttr::value( "B4G4R4A4_UNORM", ResourceFormat::RESOURCE_FORMAT_B4G4R4A4_UNORM ),
		rttr::value( "P208", ResourceFormat::RESOURCE_FORMAT_P208 ),
		rttr::value( "V208", ResourceFormat::RESOURCE_FORMAT_V208 ),
		rttr::value( "V408", ResourceFormat::RESOURCE_FORMAT_V408 )
	);

	rttr::registration::enumeration< ShaderType >( "ShaderType" )
	(
		rttr::value( "VertexShader", ShaderType::VertexShader ),
		rttr::value( "PixelShader", ShaderType::PixelShader ),
		rttr::value( "GeometryShader", ShaderType::GeometryShader ),
		rttr::value( "TesselationControlShader", ShaderType::TesselationControlShader ),
		rttr::value( "TesselationEvaluationShader", ShaderType::TesselationEvaluationShader ),
		rttr::value( "ComputeShader", ShaderType::ComputeShader )
	);

	
	rttr::registration::enumeration< BufferType >( "BufferType" )
	(
		rttr::value( "VertexBuffer", BufferType::VertexBuffer ),
		rttr::value( "IndexBuffer", BufferType::IndexBuffer ),
		rttr::value( "ConstantBuffer", BufferType::ConstantBuffer )
	);

	
	rttr::registration::enumeration< PrimitiveTopology >( "PrimitiveTopology" )
	(
		rttr::value( "Points", PrimitiveTopology::PRIMITIVE_TOPOLOGY_POINTLIST ),
		rttr::value( "Lines", PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINELIST ),
		rttr::value( "LinesAdjacency", PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINELIST_ADJ ),
		rttr::value( "LineStrip", PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINESTRIP ),
		rttr::value( "LineStripAdjacency", PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ ),
		rttr::value( "Triangles", PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST ),
		rttr::value( "TrianglesAdjacency", PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ ),
		rttr::value( "TriangleStrip", PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ),
		rttr::value( "TriangleStripAdjacency", PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ )
	);

//====================================================================================//
//			Resources	
//====================================================================================//

	rttr::registration::class_< ResourceObject >( "ResourceObject" )
		.property_readonly( "ID", &ResourceObject::m_uniqueId )
		( 
			rttr::metadata( MetaDataType::AllowInSaveFile, false),
			rttr::metadata( MetaDataType::Serialize, false )
		)
		.property_readonly( "References", &ResourceObject::m_objectReferences )
		( 
			rttr::metadata( MetaDataType::AllowInSaveFile, false),
			rttr::metadata( MetaDataType::Serialize, false )
		);

	rttr::registration::class_< SwapChain >( "SwapChain" );


	// Blending
	rttr::registration::class_< BlendingInfo >( "BlendingInfo" )
		.property_readonly( "CustomBlendFactor", &BlendingInfo::CustomBlendFactor ) BIND_AS_PTR
		.property_readonly( "EnableBlending", &BlendingInfo::EnableBlending )
		.property_readonly( "ColorOperation", &BlendingInfo::ColorOperation )
		.property_readonly( "AlphaOperation", &BlendingInfo::AlphaOperation )
		.property_readonly( "SrcColorBlend", &BlendingInfo::SrcColorBlend )
		.property_readonly( "DstColorBlend", &BlendingInfo::DstColorBlend )
		.property_readonly( "SrcAlphaBlend", &BlendingInfo::SrcAlphaBlend )
		.property_readonly( "DstAlphaBlend", &BlendingInfo::DstAlphaBlend );

	rttr::registration::class_< BlendingState >( "BlendingState" )
		.property_readonly( "Descriptor", &BlendingState::GetDescriptor ) BIND_AS_PTR;


	// Rasterizer
	rttr::registration::class_< RasterizerStateInfo >( "RasterizerStateInfo" )
		.property_readonly( "CullMode", &RasterizerStateInfo::CullMode )
		.property_readonly( "FillMode", &RasterizerStateInfo::FillMode )
		.property_readonly( "IsClockwise", &RasterizerStateInfo::IsClockwise )
		.property_readonly( "EnableScissor", &RasterizerStateInfo::EnableScissor )
		.property_readonly( "EnableZClipping", &RasterizerStateInfo::EnableZClipping )
		.property_readonly( "ConservativeRaserizer", &RasterizerStateInfo::ConservativeRasterizer )
		.property_readonly( "DepthBias", &RasterizerStateInfo::DepthBias );


	rttr::registration::class_< RasterizerState >( "RasterizerState" )
		.property_readonly( "Descriptor", &RasterizerState::GetDescriptor ) BIND_AS_PTR;


	// Depth/Stencil
	rttr::registration::class_< DepthStencilInfo >( "DepthStencilInfo" )
		.property_readonly( "EnableDepthTest", &DepthStencilInfo::EnableDepthTest )
		.property_readonly( "EnableStencilTest", &DepthStencilInfo::EnableStencilTest );

	rttr::registration::class_< DepthStencilState >( "DepthStencilState" )
		.property_readonly( "Descriptor", &RasterizerState::GetDescriptor ) BIND_AS_PTR;


	// Buffer
	rttr::registration::class_< BufferObject >( "BufferObject" )
		.property_readonly( "Descriptor", &BufferObject::GetDescriptor ) BIND_AS_PTR;

	rttr::registration::class_< BufferInfo >( "BufferInfo" )
		.property_readonly( "BufferName", &BufferInfo::GetName )
		.property_readonly( "NumberElements", &BufferInfo::NumElements )
		.property_readonly( "ElementSize", &BufferInfo::ElementSize )
		.property_readonly( "Usage", &BufferInfo::Usage )
		.property_readonly( "BufferType", &BufferInfo::BufferType )
		.property_readonly( "Topology", &BufferInfo::Topology )
		.property_readonly( "4 Bytes Index Size", &BufferInfo::Use4BytesIndex )
		.property_readonly( "VertexLayout", &BufferInfo::VertexLayout );


	rttr::registration::class_< ShaderInputLayout >( "ShaderInputLayout" );
	rttr::registration::class_< IShader >( "IShader" )
		.property_readonly( "FilePath", &IShader::GetShaderFile )
		.property_readonly( "MainFunction", &IShader::GetShaderEntry );

	rttr::registration::class_< PixelShader >( "PixelShader" );
	rttr::registration::class_< VertexShader >( "VertexShader" );

	rttr::registration::class_< RenderTargetObject >( "RenderTargetObject" )
		.property( "ColorBuffer", &RenderTargetObject::m_colorBuffer )
		.property( "DepthBuffer", &RenderTargetObject::m_depthBuffer )
		.property( "StencilBuffer", &RenderTargetObject::m_stencilBuffer );

	rttr::registration::class_< MaterialObject >( "MaterialObject" )
		.property( "Diffuse", &MaterialObject::Diffuse ) BIND_AS_PTR
		.property( "Specular", &MaterialObject::Specular ) BIND_AS_PTR
		.property( "Ambient", &MaterialObject::Ambient ) BIND_AS_PTR
		.property( "Emissive", &MaterialObject::Emissive ) BIND_AS_PTR
		.property( "SpecularPower", &MaterialObject::Power );

}


//----------------------------------------------------------------------------------------------//
//								PixelShader														//
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
//								VertexShader													//
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
//								RenderTargetObject												//
//----------------------------------------------------------------------------------------------//


RenderTargetObject::RenderTargetObject( TextureObject* colorBuffer, TextureObject* depthBuffer, TextureObject* stencilBuffer )
	:	m_colorBuffer( colorBuffer )
	,	m_depthBuffer( depthBuffer )
	,	m_stencilBuffer( stencilBuffer )
{
	//if( m_colorBuffer )
	//	m_colorBuffer->AddAssetReference();
	//if( m_depthBuffer )
	//	m_depthBuffer->AddAssetReference();
	//if( m_stencilBuffer )
	//	m_stencilBuffer->AddAssetReference();
}

/**@brief Destruktor kasuje obiekty tekstury g³êbokoœci i bufora koloru, je¿eli nie s¹ u¿ywane.
Je¿eli istniej¹ odwo³ania do tych obiektów, to nie s¹ one kasowane, a jedynie usuwa siê odwo³ania
pochodz¹ce od RenderTargetObject.*/
RenderTargetObject::~RenderTargetObject()
{
	//if( m_colorBuffer )
	//{
	//	m_colorBuffer->DeleteAssetReference();
	//	//if( m_colorBuffer->CanDelete() )
	//	//	ObjectDeleter<TextureObject>::delete_object( m_colorBuffer, ObjectDeleterKey<TextureObject>() );
	//}
	//if( m_depthBuffer )
	//{
	//	m_depthBuffer->DeleteAssetReference();
	//	//if( m_depthBuffer->CanDelete() )
	//	//	ObjectDeleter<TextureObject>::delete_object( m_depthBuffer, ObjectDeleterKey<TextureObject>() );
	//}
	//if( m_stencilBuffer )
	//{
	//	m_stencilBuffer->DeleteAssetReference();
	//	//if( m_stencilBuffer->CanDelete() )
	//	//	ObjectDeleter<TextureObject>::delete_object( m_stencilBuffer, ObjectDeleterKey<TextureObject>() );
	//}
}

std::string		RenderTargetObject::GetResourceName() const
{
	if( m_colorBuffer )
		return m_colorBuffer->GetFilePath().String();
	if( m_depthBuffer )
		return m_depthBuffer->GetFilePath().String();
	if( m_stencilBuffer )
		return m_stencilBuffer->GetFilePath().String();
	return "";
}

//----------------------------------------------------------------------------------------------//
//								BufferObject													//
//----------------------------------------------------------------------------------------------//

BufferObject::BufferObject( unsigned int elementSize, unsigned int elementCount )
	:	m_elementSize( elementSize ),
		m_elementCount( elementCount )
{
}


//----------------------------------------------------------------------------------------------//
//								MaterialObject													//
//----------------------------------------------------------------------------------------------//

MaterialObject::MaterialObject( const MaterialObject* material )
: ResourceObject(WRONG_ID)
{
	memcpy( this, material, sizeof(MaterialObject) );
}

/**@brief Ustawia materia³ na wartoœci domyœlne dla silnika.

Te wartoœci s¹ najbardziej neutralne, w przypadku gdy nie ma materia³u, a jest ustawiona tekstura.
Wtedy shadery wymana¿aj¹ jasnoœæ piksela przez 1.0 i nic sie nie zmienia.*/
void MaterialObject::SetNullMaterial( )
{
	Diffuse.x = 1.0f;
	Diffuse.y = 1.0f;
	Diffuse.z = 1.0f;
	Diffuse.w = 1.0f;

	Ambient.x = 1.0f;
	Ambient.y = 1.0f;
	Ambient.z = 1.0f;
	Ambient.w = 1.0f;

	Specular.x = 1.0f;
	Specular.y = 1.0f;
	Specular.z = 1.0f;
	Specular.w = 1.0f;

	Emissive.x = 0.0f;
	Emissive.y = 0.0f;
	Emissive.z = 0.0f;
	Emissive.w = 0.0f;

	Power = 1.0f;
}


