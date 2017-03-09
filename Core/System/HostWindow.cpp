#include "HostWindow.h"

#include "swCommonLib/Common/Converters.h"

#include "swGraphicAPI/Rendering/IGraphicAPIInitializer.h"
#include "swGraphicAPI/ResourceManager/ResourceManager.h"

#include "CommonTypes/CommonTypes.h"


// Temporary
#include <iostream>


namespace sw {
namespace gui
{

// One pointer at least, but I don't know how much it needs in reality
#define STD_MAP_OVERHEAD_PER_ELEMENT 8



HostWindow::HostWindow( INativeWindow* nativeWindow, IInput* input, ResourceManager* resourceManager, IGraphicAPIInitializer* graphicApi )
	: m_input( input )
	, m_nativeWindow( nativeWindow )
	, m_resourceManager( resourceManager )
{
	// Create RenderTarget and SwapChain
	SwapChainInitData init;
	init.WindowHandle = m_nativeWindow->GetHandle();
	init.WindowHeight = m_nativeWindow->GetClientHeight();
	init.WindowWidth = m_nativeWindow->GetClientWidth();

	m_swapChain = graphicApi->CreateSwapChain( init );
	assert( m_swapChain.Ptr() );

	m_renderTarget = m_swapChain->GetRenderTarget();
	assert( m_renderTarget.Ptr() );

	resourceManager->AddRenderTarget( m_renderTarget.Ptr(), Convert::FromString< std::wstring >( "::" + m_nativeWindow->GetTitle(), L"" ) );
}

HostWindow::~HostWindow()
{
	m_swapChain->DeleteObjectReference();
	if( m_swapChain->CanDelete() )
	{
		delete m_swapChain.Ptr();
		m_swapChain = nullptr;
	}
}



/**@brief Returns memory consumed by this object and all object owned by this.*/
Size				HostWindow::GetMemorySize		()
{
	Size size = sizeof( HostWindow );

	size += m_mousePath.capacity() * sizeof( IControl* );
	size += m_focusPath.capacity() * sizeof( IControl* );
	size += m_invalidated.capacity() * sizeof( IControl* );
	size += m_controlTree.capacity() * sizeof( TopLevelControl* );

	size += m_controlsNames.size() * ( sizeof( std::pair< IControl*, std::string > ) + STD_MAP_OVERHEAD_PER_ELEMENT );

	//size += m_nativeWindow->MemorySize();

	for( auto control : m_controlTree )
		size += control->MemorySize();

	return size;
}

/**@brief Pozwala ustawiæ i pobraæ DataContext.

DataContext zostanie przepropagowany do wszystkich kontrolek w hierarchii.*/
EngineObject*&		HostWindow::DataContext			()
{
	return m_dataContext;
}

/**@brief Removes control from GUI system.*/
void				HostWindow::RemoveControl		( IControl* control )
{

}

/**@brief Allows control to register it's name.*/
void				HostWindow::RegisterControlName	( IControl* control, const std::string& name )
{
	assert( m_controlsNames.find( control ) == m_controlsNames.end() );
	m_controlsNames[ control ] = name;
}

/**@brief Gets name of registered control.*/
const std::string& HostWindow::GetControlName		( IControl* control )
{
	auto iter = m_controlsNames.find( control );
	if( iter != m_controlsNames.end() )
		return iter->second;
	else
		return EMPTY_STRING;
}

// ================================ //
//
ResourcePtr< RenderTargetObject >	HostWindow::GetRenderTarget()
{
	return m_renderTarget;
}

// ================================ //
//
ResourcePtr< SwapChain >	HostWindow::GetSwapChain()
{
	return m_swapChain;
}

// ================================ //
//
INativeWindow*		HostWindow::GetNativeWindow		()
{
	return m_nativeWindow;
}

//====================================================================================//
//				GUI system interaction
//====================================================================================//

// ================================ //
//
void				HostWindow::LostFocus			()
{
	std::cout << "Window [" + m_nativeWindow->GetTitle() + "] lost focus." << std::endl;
}

// ================================ //
//
void				HostWindow::GotFocus			()
{
	std::cout << "Window [" + m_nativeWindow->GetTitle() + "] got focus." << std::endl;
}

}	// gui
}	// sw
