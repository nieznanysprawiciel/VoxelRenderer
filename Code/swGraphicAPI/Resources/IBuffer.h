#pragma once
/**
@file IBuffer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "swGraphicAPI/Resources/ResourceObject.h"
#include "swGraphicAPI/Rendering/GraphicAPIConstants.h"
#include "ResourcePtr.h"

#include "swCommonLib/Common/MemoryChunk.h"
#include "swCommonLib/System/Path.h"


/**@defgroup Buffers
@ingroup Resources
*/

class ShaderInputLayout;
class TextureObject;


/**@brief Type of buffer.
@ingroup Buffers*/
enum class BufferType : uint8
{
	VertexBuffer,		///< Vertex buffer
	IndexBuffer,		///< Index buffer
	ConstantBuffer,		///< Constant buffer
	TextureBuffer		///< Buffer for other purposes which can be bound to shader as texture (for example unordered access).
};


/**@brief Descriptor of buffer.
@ingroup Buffers*/
struct BufferInfo
{
	uint32				NumElements;	///< Number of elements of type BufferDescriptor::DataType.
	uint32				ElementSize;	///< Size of single element in buffer.
	rttr::type			DataType;		///< [Optional] Type of single element in buffer.
	ResourceUsage		Usage;			///< Usage of resource by graphic card.
	BufferType			BufferType;		///< Vertex, index or constant buffer.
	bool				AllowRaw : 1;	///< Allows bind buffer as raw buffer to pipeline.
	filesystem::Path	Name;			///< Buffer name or file path.

	///@name Only for vertex or index buffer.
	///@{
	PrimitiveTopology					Topology;			///< [Optional] Topology of verticies. @note Vertex buffer not always have topology. If you use index buffer, vertex buffer topology has no meaning. In this case it is set to PointList.
	///@}

	///@name Vertex buffer only
	///@brief For other buffer types VertexLayout should be set to nullptr.
	///@{
	ResourcePtr< ShaderInputLayout >	VertexLayout;		///< [Optional] Layout of single vertex in buffer. You can add this layout to enable additional information in editor. Otherwise set to nullptr.
	///@}

	///@name Index buffer only
	///@{
	bool								Use4BytesIndex;		///< Index buffer consists of 4 bytes instead of 2 bytes indicies.
	///@}

	BufferInfo()
		: DataType( rttr::type::get_by_name( "" ) )	// Set invalid type.
		, AllowRaw( false )
	{}

	std::string		GetName	() const		{ return Name.String(); }
};


/**@brief Base class for buffers.
@ingroup Buffers
@ingroup Resources*/
class IBuffer	:	public ResourceObject
{
	RTTR_ENABLE( ResourceObject )
private:
protected:

	IBuffer() : ResourceObject( 0 ) {}
	virtual ~IBuffer() = default;

public:

	virtual MemoryChunk			CopyData			() = 0;				///< Kopiuje dane z bufora i umieszcza je w zwracanym MemoryChunku.
	virtual const BufferInfo&	GetDescriptor		() const = 0;		///< Returns buffer descriptor.

protected:

	///< Returns Texture object which can be bound to pipeline as raw buffer shader resource.
	///< Enabled only if BufferInfo::AllowRaw was specified.
	virtual TextureObject*		CreateRawShaderViewImpl	() const = 0;
																			
};

