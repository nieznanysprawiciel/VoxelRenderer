#pragma once

#include "DX11Initializer/DX11APIObjects.h"
#include "swGraphicAPI/Resources/MeshResources.h"

#include <vector>


/**@brief Deskryptor layoutu dla DirectX 11.
@ingroup DX11API*/
class DX11InputLayoutDescriptor : public InputLayoutDescriptor
{
private:
	std::vector<D3D11_INPUT_ELEMENT_DESC>		m_inputElement;
public:
	DX11InputLayoutDescriptor( const std::wstring& layoutName ) : InputLayoutDescriptor( layoutName ) {}
	~DX11InputLayoutDescriptor() = default;

	Size							GetNumElements()		{ return m_inputElement.size(); }
	D3D11_INPUT_ELEMENT_DESC*		GetDescriptorPtr()		{ return m_inputElement.data(); }

	virtual void					AddRow( const char* semanticName,
											ResourceFormat format,
											unsigned int inputSlot,
											unsigned int byteOffset,
											bool perInstance,
											unsigned int instanceDataStep );

	unsigned int					CountSemantic( const char* semanticName );
};

