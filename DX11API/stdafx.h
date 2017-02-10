#pragma once
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//


/// @note You can disable all headers in precompiled header with one macro.
/// All cpp files should be precompiled header independet and explicitly include needed headers.
#ifndef DISABLE_PRECOMPILED_HEADER


#include <cassert>
#include <string>

#include <DirectXMath.h>


#pragma warning( disable : 4005 )
#include "d3dx11.h"
#include "d3d11.h"
#pragma warning( default : 4005 )


#include "swCommonLib/Common/RTTR.h"

#include "swGraphicAPI/Resources/MeshResources.h"
#include "DX11Resources/DX11PipelineState.h"
#include "DX11Resources/DX11Buffer.h"
#include "DX11Resources/DX11ComputeShader.h"
#include "DX11Resources/DX11InputLayout.h"
#include "DX11Resources/DX11InputLayoutDescriptor.h"
#include "DX11Resources/DX11PixelShader.h"
#include "DX11Resources/DX11RenderTarget.h"
#include "DX11Resources/DX11SwapChain.h"
#include "DX11Resources/DX11Texture.h"
#include "DX11Resources/DX11VertexShader.h"

#include "DX11Initializer/DX11APIObjects.h"
#include "DX11Initializer/DX11ConstantsMapper.h"
#include "DX11Initializer/DX11Initializer.h"
#include "DX11Renderer/DX11Renderer.h"


#undef min
#undef max
#undef RegisterClass


#endif // !DISABLE_PRECOMPILED_HEADER


