/**
@file DX11SwapChain.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/DX11API/stdafx.h"


#include "DX11SwapChain.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< DX11SwapChain >( "DX11SwapChain" );
}



DX11SwapChain::DX11SwapChain( IDXGISwapChain* chain, DX11RenderTarget* windowRT )
	:	SwapChain( windowRT )
	,	m_swapChain( chain )
{ }


DX11SwapChain::~DX11SwapChain()
{
	if( m_swapChain )
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}

/**@copydoc SwapChain::Present.*/
void	DX11SwapChain::Present( int syncInterval )
{
	HRESULT result = m_swapChain->Present( syncInterval, 0 );
	if( FAILED( result ) )
	{
		// Here we should handle device removed event
		HRESULT reason = device->GetDeviceRemovedReason();

		OutputDebugStringA( std::string( "Device removed" ).c_str() );
	}
}

/**@copydoc SwapChain::Resize.*/
void	DX11SwapChain::Resize( uint16 newWidth, uint16 newHeight )
{
	assert( !"Implement me" );
}

/**@brief Tworzy object DX11SwapChain z g³ównego render targetu.

@todo Wersja troszkê niew³aœciwa. SwapChain jest ju¿ stworzony wczeœniej przy zwyk³ej inicjalizacji DX11APIObjects.
Tutaj jest jedynie tworzony obiekt silnikowy, który potrafi to obs³u¿yæ. Trzeba to zmieniæ, ¿eby ca³e tworzenie
render targetu odbywa³o siê tutaj.*/
DX11SwapChain* DX11SwapChain::CreateScreenSwapChain( RenderTargetObject* screenRT )
{
	DX11SwapChain* swapChainObject = new DX11SwapChain( swap_chain, static_cast< DX11RenderTarget* >( screenRT ) );

	swap_chain = nullptr;
	return swapChainObject;
}
