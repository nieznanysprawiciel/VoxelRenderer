#pragma once
/**
@file DX11InputLayout.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include "swGraphicAPI/Resources/MeshResources.h"
#include "DX11Initializer/DX11APIObjects.h"

/**@brief Implementacja layout w DirectX 11.
@ingroup DX11API*/
class DX11InputLayout : public ShaderInputLayout, protected DX11APIObjects
{
	RTTR_ENABLE( ShaderInputLayout );
private:
	ID3D11InputLayout*				m_vertexLayout;
protected:
	~DX11InputLayout();
public:
	DX11InputLayout( ID3D11InputLayout* layout );

	inline ID3D11InputLayout*		Get() { return m_vertexLayout; }
};

