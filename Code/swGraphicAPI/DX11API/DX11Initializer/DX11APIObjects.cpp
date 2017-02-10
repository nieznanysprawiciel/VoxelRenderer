/**
@file DX11APIObjects.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/DX11API/stdafx.h"


#include "DX11APIObjects.h"
#include <DirectXMath.h>

#include "swCommonLib/Common/MemoryLeaks.h"

//----------------------------------------------------------------------------------------------//
//								Zmienne statyczne klasy											//
//----------------------------------------------------------------------------------------------//

/**@brief WskaŸnik na klasê lub nullptr je¿eli nie zosta³ stworzony jeszcze obiekt.

Ta klasa nie zawiera ¿adnych zmiennych niestatycznych, ale chcemy wiedzieæ czy zosta³ stworzony chocia¿ jeden
obiekt. Dziêki temu mo¿emy zainicjowaæ deskryptory domyœlnymi wartoœciami w wygodny sposób w konstruktorze.*/
DX11APIObjects*						DX11APIObjects::this_ptr = nullptr;

bool								DX11APIObjects::m_useDebugLayer = false;

//Zmienne globalne dla funkcji
D3D11_VIEWPORT						DX11APIObjects::_view_port_desc;			//Domyœlny viewport. Je¿eli uzytkownik poda w³asny to zostanie on nadpisany.
DXGI_SWAP_CHAIN_DESC				DX11APIObjects::_swap_chain_desc;
D3D_FEATURE_LEVEL*					DX11APIObjects::_feature_levels = nullptr;
unsigned int						DX11APIObjects::_num_feature_levels = 0;
D3D_FEATURE_LEVEL					DX11APIObjects::_current_feature_level;
D3D11_TEXTURE2D_DESC				DX11APIObjects::_z_buffer_desc;					///<Deskryptor z i stencil bufora.
D3D11_DEPTH_STENCIL_VIEW_DESC		DX11APIObjects::_z_buffer_view_desc;
std::string							DX11APIObjects::_pixel_shader_model = "ps_4_0";
std::string							DX11APIObjects::_vertex_shader_model = "vs_4_0";
unsigned int						DX11APIObjects::_window_width = 1024;
unsigned int						DX11APIObjects::_window_height = 768;
D3D11_INPUT_ELEMENT_DESC*			DX11APIObjects::_vertex_layout_desc = nullptr;
unsigned int						DX11APIObjects::_layout_elements_count = 0;
D3D11_SAMPLER_DESC					DX11APIObjects::_sampler_desc;
D3D11_RASTERIZER_DESC				DX11APIObjects::_rasterizer_desc;

/*Inicjalizacja zmiennych statycznych.*/
ID3D11Debug*						DX11APIObjects::debug_interface = nullptr;
ID3D11Device*						DX11APIObjects::device = nullptr;
ID3D11DeviceContext*				DX11APIObjects::device_context = nullptr;
IDXGISwapChain*						DX11APIObjects::swap_chain = nullptr;
ID3D11RenderTargetView*				DX11APIObjects::render_target = nullptr;
ID3D11DepthStencilView*				DX11APIObjects::z_buffer_view = nullptr;
ID3D11Texture2D*					DX11APIObjects::back_buffer = nullptr;
ID3D11Texture2D*					DX11APIObjects::z_buffer = nullptr;

ID3D11InputLayout*					DX11APIObjects::default_vertex_layout = nullptr;
ID3D11VertexShader*					DX11APIObjects::default_vertex_shader = nullptr;
ID3D11PixelShader*					DX11APIObjects::default_pixel_shader = nullptr;
ID3D11SamplerState*					DX11APIObjects::default_sampler = nullptr;

ID3D11Buffer*						DX11AuxiliaryObjects::const_per_frame = nullptr;
ID3D11Buffer*						DX11AuxiliaryObjects::const_per_mesh = nullptr;
ID3D11DepthStencilState*			DX11AuxiliaryObjects::depth_enabled = nullptr;		///<Do w³¹czania z-bufora
ID3D11DepthStencilState*			DX11AuxiliaryObjects::depth_disabled = nullptr;		///<D0 wy³¹czania z-bufora

//----------------------------------------------------------------------------------------------//
//								Layouty dla bufora wierzcho³ków									//
//----------------------------------------------------------------------------------------------//
D3D11_INPUT_ELEMENT_DESC vertex_normal_texture[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC vertex_texture[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC vertex_color[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

D3D11_INPUT_ELEMENT_DESC vertex_normal_color[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

D3D11_INPUT_ELEMENT_DESC* layouts[] =
{
	vertex_normal_texture,
	vertex_texture,
	vertex_color,
	vertex_normal_color
};
unsigned int layouts_elements[] = { ARRAYSIZE( vertex_normal_texture ),
ARRAYSIZE( vertex_texture ),
ARRAYSIZE( vertex_color ),
ARRAYSIZE( vertex_normal_color ) };

//----------------------------------------------------------------------------------------------//
//								Inicjalizacja deskryptorów										//
//----------------------------------------------------------------------------------------------//

DX11APIObjects::DX11APIObjects()
{
	// Inicjalizujemy tylko wtedy, kiedy wczeœniej siê to nie sta³o.
	if ( this_ptr == nullptr )
	{
		this_ptr = this;

		_window_width = 1024;
		_window_height = 768;

		_vertex_layout_desc = layouts[0];
		_layout_elements_count = layouts_elements[0];

		// DXGI_SWAP_CHAIN_DESC
		ZeroMemory( &_swap_chain_desc, sizeof( _swap_chain_desc ) );
		_swap_chain_desc.BufferCount = 1;
		_swap_chain_desc.BufferDesc.Width = _window_width;
		_swap_chain_desc.BufferDesc.Height = _window_height;
		_swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		_swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		_swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		_swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_swap_chain_desc.OutputWindow = 0;		//HWND niestety nie wiem co wpisaæ w tym momencie
		_swap_chain_desc.SampleDesc.Count = 1;
		_swap_chain_desc.SampleDesc.Quality = 0;//D3D10_STANDARD_MULTISAMPLE_QUALITY_LEVELS::D3D10_STANDARD_MULTISAMPLE_PATTERN;
		_swap_chain_desc.Windowed = TRUE;
		_swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		_swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// D3D11_VIEWPORT Deklarujemy przekszta³cenie wspó³rzêdnych ekranowych, na wspó³rzedne znormalizowane
		_view_port_desc.Width = (FLOAT)_window_width;
		_view_port_desc.Height = (FLOAT)_window_height;
		_view_port_desc.MinDepth = 0.0f;
		_view_port_desc.MaxDepth = 1.0f;
		_view_port_desc.TopLeftX = 0;
		_view_port_desc.TopLeftY = 0;

		// D3D11_TEXTURE2D_DESC Deskryptor z-stencil_buffer
		ZeroMemory( &_z_buffer_desc, sizeof( _z_buffer_desc ) );
		_z_buffer_desc.Width = _window_width;
		_z_buffer_desc.Height = _window_height;
		_z_buffer_desc.MipLevels = 1;
		_z_buffer_desc.ArraySize = 1;
		_z_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_z_buffer_desc.SampleDesc.Count = 1;
		_z_buffer_desc.SampleDesc.Quality = 0;//D3D10_STANDARD_MULTISAMPLE_QUALITY_LEVELS::D3D10_STANDARD_MULTISAMPLE_PATTERN;
		_z_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		_z_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		_z_buffer_desc.CPUAccessFlags = 0;
		_z_buffer_desc.MiscFlags = 0;

		ZeroMemory( &_z_buffer_view_desc, sizeof( _z_buffer_view_desc ) );
		_z_buffer_view_desc.Format = _z_buffer_desc.Format;
		_z_buffer_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;// D3D11_DSV_DIMENSION_TEXTURE2DMS; //D3D11_DSV_DIMENSION_TEXTURE2D;
		_z_buffer_view_desc.Texture2D.MipSlice = 0;

		// D3D11_SAMPLER_DESC
		ZeroMemory( &_sampler_desc, sizeof( _sampler_desc ) );
		_sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		_sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		_sampler_desc.MinLOD = 0;
		_sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		_sampler_desc.MaxAnisotropy = 1;

		// D3D11_RASTERIZER_DESC
		ZeroMemory( &_rasterizer_desc, sizeof( _rasterizer_desc ) );
		_rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		_rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		_rasterizer_desc.AntialiasedLineEnable = false;
		_rasterizer_desc.DepthBias = 0;
		_rasterizer_desc.DepthBiasClamp = 0.0f;
		_rasterizer_desc.DepthClipEnable = true;
		_rasterizer_desc.FrontCounterClockwise = true;
		_rasterizer_desc.MultisampleEnable = false;
		_rasterizer_desc.ScissorEnable = false;
		_rasterizer_desc.SlopeScaledDepthBias = 0.0f;
	}
}

//----------------------------------------------------------------------------------------------//
//								Zwalnianie obiektów DirectXa									//
//----------------------------------------------------------------------------------------------//

/**@brief Zwalnia wszystkie stworzone obiekty DirectXa.
*/
void DX11APIObjects::release_DirectX()
{
	// Bardzo wa¿ne jest ustawienie zmiennych na nullptr w razie, gdyby jakaœ inna klasa wywo³ywa³a destruktor

	//Zmienne pomocnicze
	if ( default_vertex_layout )
		default_vertex_layout->Release(), default_vertex_layout = nullptr;
	if ( default_vertex_shader )
		default_vertex_shader->Release(), default_vertex_shader = nullptr;
	if ( default_pixel_shader )
		default_pixel_shader->Release(), default_pixel_shader = nullptr;
	if ( default_sampler )
		default_sampler->Release(), default_sampler = nullptr;

	if ( swap_chain )
		//DirectX nie potrafi siê zamkn¹æ w trybie pe³noekranowym, wiêc musimy go zmieniæ
		swap_chain->SetFullscreenState( FALSE, NULL );

	//Zmienne s³u¿¹ce do wyœwietlania
	if ( z_buffer_view )
		z_buffer_view->Release(), z_buffer_view = nullptr;
	if ( swap_chain )
		swap_chain->Release(), swap_chain = nullptr;
	if ( render_target )
		render_target->Release(), render_target = nullptr;
	if( back_buffer )
		back_buffer->Release(), back_buffer = nullptr;
	if( z_buffer )
		z_buffer->Release(), z_buffer = nullptr;
	if ( device_context )
		device_context->Release(), device_context = nullptr;
	if ( device )
		device->Release(), device = nullptr;

	if( IsDebugLayerEnabled() )
		debug_interface->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );

	if( debug_interface )
		debug_interface->Release(), debug_interface = nullptr;
}


//----------------------------------------------------------------------------------------------//
//					Funkcje ustawiaj¹ce parametry inicjalizacji obiektów DX11					//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja ustawia deskryptor dla obiektów Device, DeviceContext i SwapChain DirectXa.

Wartoœci takich pól jak:
- BufferDesc.Width
- BufferDesc.Height
s¹ pobierane z pól klasy i trzeba je modyfikowaæ dedykowan¹ do tego funkcj¹.*/
void DX11APIObjects::set_swapchain_desc( const DXGI_SWAP_CHAIN_DESC& swap_chain_desc )
{
	_swap_chain_desc = swap_chain_desc;

	_swap_chain_desc.BufferDesc.Width = _window_width;
	_swap_chain_desc.BufferDesc.Height = _window_height;
}

/**@brief Funkcja ustawia deskryptor dla Viewportu.
Nie jest on ustawiany jako aktywny w DirectX, trzeba wywo³aæ funkcjê init_viewport.

Wartoœci takich pól jak:
- BufferDesc.Width
- BufferDesc.Height
s¹ pobierane z pól klasy i trzeba je modyfikowaæ dedykowan¹ do tego funkcj¹.*/
void DX11APIObjects::set_viewport_desc( const D3D11_VIEWPORT& view_port_desc )
{
	_view_port_desc = view_port_desc;

	_view_port_desc.Height = static_cast<float>(_window_height);
	_view_port_desc.Width = static_cast<float>(_window_width);
}

/**@brief ustawia podan¹ w parametrze tablicê z levelami.
Tablica nie powinna zostaæ usuniêta, poniewa¿ nie jest kopiowana.

Je¿eli nic nie zostanie ustawione to domyœlne ustawiana jest wartoœæ nullptr, co jest
równowa¿ne podaniu nastêpuj¹cej tablicy:
D3D_FEATURE_LEVEL_11_0,
D3D_FEATURE_LEVEL_10_1,
D3D_FEATURE_LEVEL_10_0,
D3D_FEATURE_LEVEL_9_3,
D3D_FEATURE_LEVEL_9_2,
D3D_FEATURE_LEVEL_9_1,

Zasadniczo lepiej nie modyfikowaæ tego parametru, je¿eli nie ma takiej koniecznoœci.

@param[in] elements Liczba elementów w tablicy.*/
void DX11APIObjects::set_feature_levels( D3D_FEATURE_LEVEL* feature_levels, unsigned int elements )
{
	_feature_levels = feature_levels;
	_num_feature_levels = elements;
}

void DX11APIObjects::set_window_resolution( unsigned int window_width, unsigned int window_height )
{
	_window_width = window_width;
	_window_height = window_height;

	// Uzupe³niamy dane w odpowiednich deskryptorach
	// DXGI_SWAP_CHAIN_DESC
	_swap_chain_desc.BufferDesc.Width = _window_width;
	_swap_chain_desc.BufferDesc.Height = _window_height;

	// D3D11_VIEWPORT
	_view_port_desc.Height = static_cast<float>(_window_height);
	_view_port_desc.Width = static_cast<float>(_window_width);

	// D3D11_TEXTURE2D_DESC Z-Buffer
	_z_buffer_desc.Height = _window_height;
	_z_buffer_desc.Width = _window_width;
}

void DX11APIObjects::set_depth_stencil_format( DXGI_FORMAT depth_stencil_format )
{
	_z_buffer_desc.Format = depth_stencil_format;
	_z_buffer_view_desc.Format = depth_stencil_format;
}

/**@brief Ustawia jeden z domyœlnych deskryptorów layoutów zdefiniowanych w klasie.
Funkcja nie ustawia ¿adnego layoutu w directX.

@attention Funkcja nie zwalnia pamiêci po starym layoucie. Layouty powinny byæ alokowane na stosie
i byæ zmiennymi globalnymi w przeciwnym razie moga byc wycieki pamêci.*/
void DX11APIObjects::set_vertex_layout( DX11_DEFAULT_VERTEX_LAYOUT layout )
{
	_vertex_layout_desc = layouts[static_cast<unsigned int>(layout)];
	_layout_elements_count = layouts_elements[static_cast<int>(layout)];
}

/**@brief Ustawia podany deskryptor layoutu.
Funkcja nie ustawia ¿adnego layoutu w directX.

@attention Funkcja nie zwalnia pamiêci po starym layoucie. Layouty powinny byæ alokowane na stosie
i byæ zmiennymi globalnymi w przeciwnym razie moga byc wycieki pamêci.*/
void DX11APIObjects::set_vertex_layout( D3D11_INPUT_ELEMENT_DESC* layout, unsigned int array_size )
{
	_vertex_layout_desc = layout;
	_layout_elements_count = array_size;
}

/**Ustawia podany w parametrze deskryptor samplera.*/
void DX11APIObjects::set_sampler_desc( D3D11_SAMPLER_DESC sampler_desc )
{
	_sampler_desc = sampler_desc;
}

/**@brief Ustawia podany w parametrze deskrytptor rasteryzatora.*/
void DX11APIObjects::set_rasterizer_desc( const D3D11_RASTERIZER_DESC& rasterizer_desc )
{
	_rasterizer_desc = rasterizer_desc;
}

//----------------------------------------------------------------------------------------------//
//					Jedna du¿a funkcja, która za³atwia jak najwiêcej							//
//----------------------------------------------------------------------------------------------//

/**@brief Funkcja do pe³nej inicjalizacji DirectXa.

Je¿eli chcesz zainicjowaæ wszystko na raz, to wywo³aj tê funkcjê.
Przed wywo³aniem ustaw odpowiednie deskryptory, je¿eli chcesz, ¿eby u¿yte zosta³y
inne wartoœci ni¿ domyœlne.

@param[in] width Szerokoœæ okna.
@param[in] height Wysokoœæ okna.
@param[in] window Uchwyt g³ównego okna aplikacji.
@param[in] fullscreen Ustaw na true, je¿eli aplikacja ma dzia³aæ w trybie pe³noekranowym.
@param[in] pix_shader_file Plik zawieraj¹cy shader.
@param[in] pix_shader_name Nazwa funkcji, od której ma siê zacz¹æ wykonywanie shadera.
@param[in] vert_shader_file Plik zawieraj¹cy shader.
@param[in] vert_shader_name Nazwa funkcji, od której ma siê zacz¹æ wykonywanie shadera.
@param[in] singleThread Ustaw na true, je¿eli nie zamierzasz korzystaæ z ID3D11Device wielow¹tkowo (pozwala to na przyspieszenie dzia³ania
ze wzglêdu na to, ¿e nie jest konieczna synchronizacja. Nie wiem jak du¿y to ma wp³yw.)
@return Zwraca jedn¹ ze sta³ych DX11_INIT_RESULT.*/
DX11_INIT_RESULT DX11APIObjects::init_DX11(
	int width, int height, HWND window, bool fullscreen,
	const std::wstring& pix_shader_file, const std::string& pix_shader_name,
	const std::wstring& vert_shader_file, const std::string& vert_shader_name,
	bool single_thread )
{
	DX11_INIT_RESULT result;

	set_window_resolution( width, height );
	_swap_chain_desc.OutputWindow = window;

	result = InitDevicesAndSwapChain( window, fullscreen, single_thread );	// Funkcja sama sprz¹ta po sobie
	if ( result != DX11_INIT_OK )
		return result;

	result = init_z_buffer_and_render_target();		// Funkcja sama sprz¹ta po sobie
	if ( result != DX11_INIT_OK )
		return result;

	result = init_viewport();		// Funkcja nie sprz¹ta, bo byæ mo¿e nie ma czego, je¿eli siê nie uda³o.
	if ( result != DX11_INIT_OK )
	{
		release_DirectX();	// Sprz¹tamy na wszelki wypadek, ale w gruncie rzeczy najprawdopodobniej nie ma czego.
		return result;
	}

	result = init_vertex_shader( vert_shader_file, vert_shader_name );
	if ( result != DX11_INIT_OK )
	{
		release_DirectX();	// Byæ mo¿e nie by³o pliku, ale i tak nic z tym nie zrobimy wiêc sprz¹tamy.
		return result;
	}

	result = init_pixel_shader( pix_shader_file, pix_shader_name );
	if ( result != DX11_INIT_OK )
	{
		release_DirectX();	// Byæ mo¿e nie by³o pliku, ale i tak nic z tym nie zrobimy wiêc sprz¹tamy.
		return result;
	}

	result = init_sampler();
	if ( result != DX11_INIT_OK )
	{
		release_DirectX();	// Jak tu coœ siê nie uda³o, to znaczy, ¿e deskryptor by³ niepoprawny.
		return result;
	}

	return DX11_INIT_OK;
}

/**@brief Funkcja do pe³nej inicjalizacji DirectXa.

Je¿eli chcesz zainicjowaæ wszystko na raz, to wywo³aj tê funkcjê.
Przed wywo³aniem ustaw odpowiednie deskryptory, je¿eli chcesz, ¿eby u¿yte zosta³y
inne wartoœci ni¿ domyœlne.

Funkcja nie inicjuje shaderów, nie tworzy layoutu wierzcho³ka ani nie ustawia samplera.

@param[in] width Szerokoœæ okna.
@param[in] height Wysokoœæ okna.
@param[in] window Uchwyt g³ównego okna aplikacji.
@param[in] fullscreen Ustaw na true, je¿eli aplikacja ma dzia³aæ w trybie pe³noekranowym.
@param[in] singleThread Ustaw na true, je¿eli nie zamierzasz korzystaæ z ID3D11Device wielow¹tkowo (pozwala to na przyspieszenie dzia³ania
ze wzglêdu na to, ¿e nie jest konieczna synchronizacja. Nie wiem jak du¿y to ma wp³yw.)
@return Zwraca jedn¹ ze sta³ych DX11_INIT_RESULT.*/
DX11_INIT_RESULT DX11APIObjects::init_DX11( int width, int height, HWND window, bool fullscreen, bool singleThread )
{
	DX11_INIT_RESULT result;

	set_window_resolution( width, height );
	_swap_chain_desc.OutputWindow = window;

	result = InitDevicesAndSwapChain( window, fullscreen, singleThread );	// Funkcja sama sprz¹ta po sobie
	if ( result != DX11_INIT_OK )
		return result;

	result = init_z_buffer_and_render_target();		// Funkcja sama sprz¹ta po sobie
	if ( result != DX11_INIT_OK )
		return result;

	result = init_viewport();		// Funkcja nie sprz¹ta, bo byæ mo¿e nie ma czego, je¿eli siê nie uda³o.
	if ( result != DX11_INIT_OK )
	{
		release_DirectX();	// Sprz¹tamy na wszelki wypadek, ale w gruncie rzeczy najprawdopodobniej nie ma czego.
		return result;
	}

	return DX11_INIT_OK;
}

//----------------------------------------------------------------------------------------------//
//								Tworzenie obiektów DirectXa										//
//----------------------------------------------------------------------------------------------//

/**@brief Tworzy obiekty swap_chain, device i device_context.

@param[in] width Szerokoœæ ekranu w pikselach
@param[in] height Wysokoœæ ekranu w pikselach
@param[in] window Uchwyt okna, w którym renderujemy
@param[in] fullscreen Tryb pe³noekranowy lub w oknie.
@param[in] singleThread Ustawiæ na true, je¿eli mamy tylko jeden w¹tek, który tworzy wszystkie obiekty
DirectXa typu bufory i shadery. Domyœlnie false.
@return Zwraca jedn¹ z wartoœci DX11_INIT_RESULT.*/
DX11_INIT_RESULT DX11APIObjects::InitDevicesAndSwapChain( HWND window, bool fullscreen, bool single_thread )
{
	if ( !this_ptr )
		return NO_CLASS_INHERTIS_THIS_INTERFACE;

	HRESULT result = S_OK;
	UINT createDeviceFlags = 0;

	if( m_useDebugLayer )
	{
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	if ( single_thread )
		// Domyœlnie obiekt ID3D11Device jest synchronizowany, ale mo¿na to wy³¹czyæ
		createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;


	result = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
											createDeviceFlags, _feature_levels, _num_feature_levels,
											D3D11_SDK_VERSION, &_swap_chain_desc, &swap_chain,
											&device, &_current_feature_level, &device_context );
	if ( FAILED( result ) )
		return COULD_NOT_INIT_DEVICES_AND_SWAPCHAIN;

	if ( fullscreen )
		swap_chain->SetFullscreenState( TRUE, nullptr );

	if( m_useDebugLayer )
	{
		//debug_interface
		device->QueryInterface( __uuidof( ID3D11Debug ), (void**)&debug_interface );
	}

	return DX11_INIT_OK;
}


/**@brief Ustawia viewport zgodny z aktualnie ustawionym deskryptorem.*/
DX11_INIT_RESULT DX11APIObjects::init_viewport()
{
	if ( !device )
		return DX11_DEVICE_NOT_INITIALIZED;
	device_context->RSSetViewports( 1, &_view_port_desc );	//tworzymy tylko jeden viewport, ale noramlnie mo¿na wiecej, tylko po co
	return DX11_INIT_OK;
}

/**@brief Funkcja tworzy z-bufffer oraz pobiera tylny bufor i tworzy z niego render target.
Nastêpnie widok z bufora i widok tylnego bufora s¹ ustawione jako cel dla funkcji renderuj¹cych renderowania.

@return Zwraca jedn¹ ze sta³ych DX11_INIT_RESULT.
*/
DX11_INIT_RESULT DX11APIObjects::init_z_buffer_and_render_target()
{
	HRESULT result = S_OK;
	if ( !device )
		return DX11_DEVICE_NOT_INITIALIZED;

	// RenderTargetView
	// Tworzymy RenderTargetView. W tym celu pobieramy wskaŸnik na obiekt tylniego bufora
	// i tworzymy z niego widok.
	result = swap_chain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer );
	if ( FAILED( result ) )
	{
		release_DirectX();
		return COULD_NOT_CREATE_BACKBUFFER;
	}

	result = device->CreateRenderTargetView( back_buffer, nullptr, &render_target );
	if ( FAILED( result ) )
	{
		release_DirectX();
		return COULD_NOT_CREATE_RENDERTARGET;
	}
	back_buffer->Release();
	back_buffer = nullptr;

	// z-buffer

	result = device->CreateTexture2D( &_z_buffer_desc, nullptr, &z_buffer );

	if ( FAILED( result ) )
	{
		release_DirectX();
		return COULD_NOT_CREATE_DEPTHSTENCIL;
	}

	// Create the depth stencil view
	result = device->CreateDepthStencilView( z_buffer, &_z_buffer_view_desc, &z_buffer_view );
	z_buffer->Release();
	z_buffer = nullptr;

	if ( FAILED( result ) )
	{
		release_DirectX();
		return COULD_NOT_CREATE_DEPTHSTENCIL_VIEW;
	}

	device_context->OMSetRenderTargets( 1, &render_target, z_buffer_view );

	return DX11_INIT_OK;
}



//----------------------------------------------------------------------------------------------//
//								Tworzenie domyœlnych shaderów									//
//----------------------------------------------------------------------------------------------//

/**@brief Tworzy obiekt VertexShader na podstawie pliku.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z którego zostanie wczytany shader.
@param[in] shader_name Nazwa funkcji, która jest punktem poczatkowym wykonania shadera.
@param[in] shader_model £añcuch znaków opisuj¹cy shader model.
@return Zwraca wskaŸnik na obiekt vertex shadera lub nullptr w przypadku niepowodzenia.*/
ID3D11VertexShader* DX11APIObjects::load_vertex_shader( const std::wstring& file_name, const std::string& shader_name, const char* shader_model = "vs_4_0" )
{
	if ( !device )
		return nullptr;

	HRESULT result;
	ID3DBlob* compiled_shader;
	ID3D11VertexShader* vertex_shader;
	// Troszkê zamieszania, ale w trybie debug warto wiedzieæ co jest nie tak przy kompilacji shadera
#ifdef _DEBUG
	ID3D10Blob* error_blob = nullptr;	// Tu znajdzie siê komunikat o b³êdzie
#endif

	// Kompilujemy shader znaleziony w pliku
	D3DX11CompileFromFile( file_name.c_str(), 0, 0, shader_name.c_str(), shader_model,
						   0, 0, 0, &compiled_shader,
#ifdef _DEBUG
						   &error_blob,	// Funkcja wsadzi informacje o b³êdzie w to miejsce
#else
						   0,	// W trybie Release nie chcemy dostawaæ b³êdów
#endif
						   &result );

	if ( FAILED( result ) )
	{
#ifdef _DEBUG
		if ( error_blob )
		{
			// B³¹d zostanie wypisany na standardowe wyjœcie
			OutputDebugStringA( (char*)error_blob->GetBufferPointer() );
			error_blob->Release();
		}
#endif
		return nullptr;
	}

	// Tworzymy obiekt shadera na podstawie tego co sie skompilowa³o
	result = device->CreateVertexShader( compiled_shader->GetBufferPointer(),
										 compiled_shader->GetBufferSize(),
										 nullptr, &vertex_shader );

	if ( FAILED( result ) )
	{
		compiled_shader->Release();
		return nullptr;
	}

	return vertex_shader;
}

/**@brief Tworzy obiekt vertex shadera na podstawie pliku. Zwraca równie¿ layout dla podanej struktury wierzcho³ka.
Nie nale¿y u¿ywaæ tej funkcji, je¿eli layout nie jest rzeczywiœcie potrzebny. Trzeba pamietaæ o zwolnieniu
go, kiedy przestanie byæ potrzebny.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z którego zostanie wczytany shader
@param[in] shader_name Nazwa funkcji, która jest punktem poczatkowym wykonania shadera
@param[out] layout W zmiennej umieszczany jest wskaŸnik na layout wierzcho³ka. Nale¿y pamiêtaæ o zwolnieniu go kiedy bêdzie niepotrzebny.
@param[in] layout_desc Deskryptor opisujacy tworzony layout.
@param[in] array_size Liczba elementów tablicy layout_desc.
@param[in] shader_model £añcuch znaków opisuj¹cy shader model.
@return Zwraca wskaŸnik na obiekt vertex shadera lub nullptr w przypadku niepowodzenia.*/
ID3D11VertexShader* DX11APIObjects::load_vertex_shader( const std::wstring& file_name, const std::string& shader_name,
																   ID3D11InputLayout** layout, D3D11_INPUT_ELEMENT_DESC* layout_desc,
																   unsigned int array_size, const char* shader_model = "vs_4_0" )
{
	if ( !device )
		return nullptr;

	HRESULT result;
	ID3DBlob* compiled_shader;
	ID3D11VertexShader* vertex_shader;
	// Troszkê zamieszania, ale w trybie debug warto wiedzieæ co jest nie tak przy kompilacji shadera
#ifdef _DEBUG
	ID3D10Blob* error_blob = nullptr;	// Tu znajdzie siê komunikat o b³êdzie
#endif

	// Kompilujemy shader znaleziony w pliku
	D3DX11CompileFromFile( file_name.c_str(), 0, 0, shader_name.c_str(), shader_model,
						   0, 0, 0, &compiled_shader,
#ifdef _DEBUG
						   &error_blob,	// Funkcja wsadzi informacje o b³êdzie w to miejsce
#else
						   0,	// W trybie Release nie chcemy dostawaæ b³êdów
#endif
						   &result );

	if ( FAILED( result ) )
	{
#ifdef _DEBUG
		if ( error_blob )
		{
			// B³¹d zostanie wypisany na standardowe wyjœcie
			OutputDebugStringA( (char*)error_blob->GetBufferPointer() );
			error_blob->Release();
		}
#endif
		layout = nullptr;
		return nullptr;
	}

	// Tworzymy obiekt shadera na podstawie tego co sie skompilowa³o
	result = device->CreateVertexShader( compiled_shader->GetBufferPointer(),
										 compiled_shader->GetBufferSize(),
										 nullptr, &vertex_shader );

	if ( FAILED( result ) )
	{
		compiled_shader->Release();
		layout = nullptr;
		return nullptr;
	}

	// Tworzymy layout
	result = device->CreateInputLayout( layout_desc, array_size, compiled_shader->GetBufferPointer(),
										compiled_shader->GetBufferSize(), layout );
	compiled_shader->Release();
	if ( FAILED( result ) )
	{
		layout = nullptr;
		return nullptr;
	}

	return vertex_shader;
}

/**@bref Tworzy pixel shader na podstawie pliku.

W przypadku b³êdów kompilacji w trybie debug s¹ one przekierowane do okna Output.

Na razie obs³uguje tylko nieskompilowane pliki.
@param[in] file_name Nazwa pliku, z którego zostanie wczytany shader
@param[in] shader_name Nazwa funkcji, która jest punktem poczatkowym wykonania shadera
@param[in] shader_model £añcuch znaków opisuj¹cy shader model.
*/
ID3D11PixelShader* DX11APIObjects::load_pixel_shader( const std::wstring& file_name, const std::string& shader_name, const char* shader_model = "ps_4_0" )
{
	if ( !device )
		return nullptr;

	HRESULT result;
	ID3DBlob* compiled_shader;
	ID3D11PixelShader* pixel_shader;
	// Troszkê zamieszania, ale w trybie debug warto wiedzieæ co jest nie tak przy kompilacji shadera
#ifdef _DEBUG
	ID3D10Blob* error_blob = nullptr;	// Tu znajdzie siê komunikat o b³êdzie
#endif

	// Kompilujemy shader znaleziony w pliku
	D3DX11CompileFromFile( file_name.c_str(), 0, 0, shader_name.c_str(), shader_model,
						   0, 0, 0, &compiled_shader,
#ifdef _DEBUG
						   &error_blob,	// Funkcja wsadzi informacje o b³êdzie w to miejsce
#else
						   0,	// W trybie Release nie chcemy dostawaæ b³êdów
#endif
						   &result );

	if ( FAILED( result ) )
	{
#ifdef _DEBUG
		if ( error_blob )
		{
			// B³¹d zostanie wypisany na standardowe wyjœcie
			OutputDebugStringA( (char*)error_blob->GetBufferPointer() );
			error_blob->Release();
		}
#endif
		return nullptr;
	}

	// Tworzymy obiekt shadera na podstawie tego co sie skompilowa³o
	result = device->CreatePixelShader( compiled_shader->GetBufferPointer(),
										compiled_shader->GetBufferSize(),
										nullptr, &pixel_shader );

	if ( FAILED( result ) )
	{
		compiled_shader->Release();
		return nullptr;
	}

	return pixel_shader;
}


/**@brief Funkcja tworzy shader na podstawie podanego pliku, kompiluje go, a potem obudowuje klas¹
bêd¹c¹ interfejsem shadera. Parametr shader_name oznacza nazwê funkcji, od której zaczyna
siê wykonanie kodu shadera.

Funkcja ustawia shader oraz layout wierzcho³ków w kontekœcie urz¹dzenia jako aktywny.
W trybie debug komunikaty kompilacji shadera s¹ przekazywane do okna output.

@param[in] file_name Nazwa pliku zawieraj¹cego shader.
@param[in] shader_name Nazwa shadera.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.*/
DX11_INIT_RESULT DX11APIObjects::init_vertex_shader( const std::wstring& file_name, const std::string& shader_name )
{
	if ( !device )
		return DX11_DEVICE_NOT_INITIALIZED;
	if ( !device_context )
		return DX11_DEVICECONTEXT_NOT_INITIALIZED;

	default_vertex_shader = load_vertex_shader( file_name, shader_name, &default_vertex_layout, _vertex_layout_desc, _layout_elements_count, _vertex_shader_model.c_str() );
	if ( !default_vertex_shader )
		return COULD_NOT_CREATE_VERTEX_SHADER;

	device_context->VSSetShader( default_vertex_shader, nullptr, 0 );
	device_context->IASetInputLayout( default_vertex_layout );

	return DX11_INIT_OK;
}

/**@brief Funkcja tworzy shader na podstawie podanego pliku, kompiluje go, a potem obudowuje klas¹
bêd¹c¹ interfejsem shadera. Parametr shader_name oznacza nazwê funkcji, od której zaczyna
siê wykonanie kodu shadera.

Poza tworzeniem domyœlnego shadera, funkcja tworzy domyœlny sampler.
W trybie debug komunikaty kompilacji shadera s¹ przekazywane do okna output.

Funkcja ustawia ten shader i sampler w kontekœcie urz¹dzenia jako aktywny.
@param[in] file_name Nazwa pliku zawieraj¹cego shader.
@param[in] shader_name Nazwa shadera.
@return Zwraca jedn¹ ze sta³ych, jak¹ zwraca DirectX.*/
DX11_INIT_RESULT DX11APIObjects::init_pixel_shader( const std::wstring& file_name, const std::string& shader_name )
{
	if ( !device )
		return DX11_DEVICE_NOT_INITIALIZED;
	if ( !device_context )
		return DX11_DEVICECONTEXT_NOT_INITIALIZED;

	default_pixel_shader = load_pixel_shader( file_name, shader_name, _pixel_shader_model.c_str() );

	if ( !default_pixel_shader )
		return COULD_NOT_CREATE_PIXEL_SHADER;

	device_context->PSSetShader( default_pixel_shader, nullptr, 0 );
	return DX11_INIT_OK;
}

/**@brief Tworzy domyœlny smapler.
@return Zwraca jedn¹ ze sta³ych DX11_INIT_RESULT.*/
DX11_INIT_RESULT DX11APIObjects::init_sampler()
{
	HRESULT result = device->CreateSamplerState( &_sampler_desc, &default_sampler );

	if ( FAILED( result ) )
		return COULD_NOT_CREATE_SAMPLER;
	device_context->PSSetSamplers( 0, 1, &default_sampler );

	return DX11_INIT_OK;
}

//----------------------------------------------------------------------------------------------//
//								Funkcje pomocne przy wyœwietlaniu								//
//----------------------------------------------------------------------------------------------//


/**@brief Czyœcimy tylny bufor oraz z-bufor i wywo³ujemy funcjê inicjuj¹c¹ renderowanie.

@attention Funkcja zak³ada, ¿e device_context, render target i depthstencil s¹ utworzone.
Poniewa¿ funkcja jest wywo³ywana w pêtli renderingu, to nie jest to sprawdzane, wiêc
programista jest odpowiedzialny za poprawne wywo³anie.*/
void DX11APIObjects::begin_scene()
{
	//Bufor tylny
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// red, green, blue, alpha
	device_context->ClearRenderTargetView( render_target, ClearColor );

	//Z-bufor
	device_context->ClearDepthStencilView( z_buffer_view, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

// ================================ //
//
void		DX11APIObjects::SetDebugName	( ID3D11DeviceChild* child, const std::string& name )
{
	if( child != nullptr )
	{
		child->SetPrivateData( WKPDID_D3DDebugObjectName, (uint32)name.size(), name.c_str() );
	}
}

// ================================ //
//
bool		DX11APIObjects::IsDebugLayerEnabled()
{
	return m_useDebugLayer;
}


//----------------------------------------------------------------------------------------------//
//								Funkcje dla klasy DX11AuxiliaryObjects				//
//----------------------------------------------------------------------------------------------//

/**@brief Tworzy bufory sta³ych dla shadera.

Bufory musz¹ mieæ rozmiar bêd¹cy wielokrotnoœci¹ 16 bajtów ze wzglêdu na rozmiar rejestrów GPU.
Je¿eli podane wartoœci takie nie s¹, to s¹ rozszerzane do tej wielokrotnoœci, ale taka sytuacja mo¿e to oznaczaæ,
¿e jest jakiœ b³¹d w programie.

@param[in] size_per_frame Rozmiar bufora const_per_frame.
@param[in] size_per_mesh Rozmiar bufora const_per_mesh.*/
void DX11AuxiliaryObjects::init_buffers( unsigned int size_per_frame, unsigned int size_per_mesh )
{
	HRESULT result;

	// Bufory sta³ych musz¹ mieæ rozmiar bêd¹cy wielokrotnoœci¹ 16
	// Dobrze ¿e DirectX wypluwa jakieœ debugowe informacje, bo nie wiem, jakbym na to wpad³
	if ( size_per_frame % 16 )
	{
		size_per_frame = size_per_frame >> 4;		// Dzielimy na 16 (dzielenie ca³kowite)
		size_per_frame = (size_per_frame + 1) << 4;	// Najbli¿sza wielokrotnoœæ 16
	}

	if ( size_per_mesh % 16 )
	{
		size_per_mesh = size_per_mesh >> 4;			// Dzielimy na 16 (dzielenie ca³kowite)
		size_per_mesh = (size_per_mesh + 1) << 4;	// Najbli¿sza wielokrotnoœæ 16
	}

	// Tworzymy bufor sta³ych w ka¿dej ramce
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory( &buffer_desc, sizeof( buffer_desc ) );
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_per_frame;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;

	//Tworzymy bufor sta³ych, który jest niezmienny podczas wyœwietlania ramki
	result = device->CreateBuffer( &buffer_desc, nullptr, &const_per_frame );

	//Drugi bufor ró¿ni siê tylko wielkoœci¹
	buffer_desc.ByteWidth = size_per_mesh;

	//Tworzymy bufor sta³ych, do którego bêdziemy wpisywaæ wartoœci sta³e dla ka¿dego mesha (albo czêœci mesha)
	result = device->CreateBuffer( &buffer_desc, nullptr, &const_per_mesh );
}


void  DX11AuxiliaryObjects::init_depth_states()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	HRESULT result = device->CreateDepthStencilState( &dsDesc, &depth_enabled );
	if( FAILED( result ) )
		throw new std::runtime_error( "Could not create DepthStencilState" );

	device_context->OMSetDepthStencilState( depth_enabled, 1 );

	dsDesc.DepthEnable = false;
	device->CreateDepthStencilState( &dsDesc, &depth_disabled );
}

/**@brief Zwalania obiekty DirectXa. Funkcja wywo³uje tê sam¹ funkcjê z obiektu potomnego, ¿eby
zwolniæ wszystkie obiekty, które istniej¹.*/
void DX11AuxiliaryObjects::release_DirectX()
{
	if ( const_per_frame )
		const_per_frame->Release(), const_per_frame = nullptr;
	if ( const_per_mesh )
		const_per_mesh->Release(), const_per_mesh = nullptr;
	if ( depth_enabled )
		depth_enabled->Release();
	if ( depth_disabled )
		depth_disabled->Release();

	// Zwalniamy te¿ wszystkie obiekty, które zwalnia³a klasa bazowa
	DX11APIObjects::release_DirectX();
}
