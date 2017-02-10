/**
@file DX11InputLayout.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/DX11API/stdafx.h"

#include "DX11InputLayout.h"

#include "swCommonLib/Common/MemoryLeaks.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< DX11InputLayout >( "DX11InputLayout" );
}


DX11InputLayout::DX11InputLayout( ID3D11InputLayout* layout )
{
	m_vertexLayout = layout;
}


DX11InputLayout::~DX11InputLayout()
{
	if( m_vertexLayout )
		m_vertexLayout->Release();
	m_vertexLayout = nullptr;
}
