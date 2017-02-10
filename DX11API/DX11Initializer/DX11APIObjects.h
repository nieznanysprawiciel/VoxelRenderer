#pragma once

/**@file DX11APIObjects.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Zawiera deklaracjê klasy DX11APIObjects i DX11AuxiliaryObjects s³u¿¹cych do
inicjowania i przechowywania obiektów DirectXa.
*/

#include <string>

#pragma warning( disable : 4005 )
#include "d3dx11.h"
#include "d3d11.h"
#pragma warning( default : 4005 )


typedef ID3D11DeviceContext DeviceContext;
typedef ID3D11Device Device;


/**Kody b³êdów zwracane przez funkcje klasy DX11APIObjects.*/
enum DX11_INIT_RESULT
{
	DX11_INIT_OK,								///< Funkcja wykonana poprawnie
	NO_CLASS_INHERTIS_THIS_INTERFACE,			///< ¯adna klasa nie odziedziczy³a po DX11APIObjects. Zmienne s¹ niezainicjalizowane.
	COULD_NOT_INIT_DEVICES_AND_SWAPCHAIN,		///< Nie uda³o siê utworzyæ obieków device, device_context i swap_chain.
	COULD_NOT_CREATE_BACKBUFFER,				///< Nie uda³o siê utworzyæ bufora tylnego.
	COULD_NOT_CREATE_RENDERTARGET,				///< Nie uda³o siê zainicjowaæ widoku na bufor tylni.
	COULD_NOT_CREATE_DEPTHSTENCIL,				///< Nie usta³o siê stworzyæ bufora g³êbokoœci.
	COULD_NOT_CREATE_DEPTHSTENCIL_VIEW,			///< Nie uda³o siê stworzyæ widoku na bufor g³êbokoœci.
	COULD_NOT_CREATE_PIXEL_SHADER,				///< Nie uda³o siê skompilowaæ pixel shadera.
	COULD_NOT_CREATE_VERTEX_SHADER,				///< Nie uda³o siê skompilowaæ vertex shadera.
	COULD_NOT_CREATE_SAMPLER,					///< Nie uda³o siê utworzyæ obiektu samplera.
	DX11_DEVICE_NOT_INITIALIZED,				///< DirectX nie zosta³ zainicjowany (obiekt device wskazuje na nullptr).
	DX11_DEVICECONTEXT_NOT_INITIALIZED			///< DirectX nie zosta³ zainicjowany (obiekt device_context wskazuje na nullptr).
};

/**Enumeruje wbudowane typy layoutów.*/
enum DX11_DEFAULT_VERTEX_LAYOUT
{
	VERTEX_NORMAL_TEXTURE = 0,			///< Pozycja, normalne wspó³rzêdne UV
	VERTEX_TEXTURE = 1,					///< Pozycja, wspó³rzêdne UV
	VERTEX_COLOR = 2,					///< Pozycja, kolor
	VERTEX_NORMAL_COLOR = 3				///< Pozycja, normalne, kolor
};

/**@brief Klasa przechowuje w zmiennych statycznych najwa¿niejsze interfejsy
DirectX11.

Obiekty, które ich potrzebuj¹, powinny odziedziczyæ po tej klasie,
dziêki czemu maj¹ bezpoœredni dostêp.

Aby naj³atwiej zainicjowaæ DirectX, najlepiej jest wywo³aæ funkcjê init_DX11.
Z wyj¹tkiem rzeczy podawanych w parametrach, wszystko zostanie zainicjowane zgodnie z deskryptorami
domyœlnymi. Je¿eli chcesz coœ zmieniæ, to musisz to zrobiæ przed wywo³aniem tej funkcji.

Mo¿na te¿ inicjowaæ ka¿d¹ rzecz z osobna, ale w zasadzie nie ma to wiêkszego sensu,
skoro wiszystkie deskryptory mo¿na zmodyfikowaæ.

Klasa daje te¿ mo¿liwoœæ kompilowania shaderów.*/

class DX11APIObjects
{
protected:
	static DX11APIObjects*			this_ptr;		///<Wa¿ne, ¿eby nie zainicjowac obiektu wielokrotnie.

	static bool						m_useDebugLayer;

	static unsigned int				_window_width;
	static unsigned int				_window_height;
	static std::string				_pixel_shader_model;
	static std::string				_vertex_shader_model;

	static DXGI_SWAP_CHAIN_DESC		_swap_chain_desc;	///<Domyœlny deskryptor g³ównych obiektów DirectX11
	static D3D11_VIEWPORT			_view_port_desc;	///<Domyœlny deskryptor viewportu. Je¿eli u¿ytkownik poda w³asny to zostanie on nadpisany.
	static D3D_FEATURE_LEVEL*		_feature_levels;	///<Tablica z preferowanymi poziomami DirectX. Domyœlnie nullptr, uzytkownik mo¿e zdefiniowaæ.
	static unsigned int				_num_feature_levels;///<Liczba elementów powy¿szej tablicy.
	static D3D_FEATURE_LEVEL		_current_feature_level;		///<Przechowuje poziom DirectXa.
	static D3D11_TEXTURE2D_DESC		_z_buffer_desc;				///<Deskryptor z i stencil bufora.
	static D3D11_DEPTH_STENCIL_VIEW_DESC	_z_buffer_view_desc;	///<Deskryptor widoku z i stencil bufora.
	static D3D11_INPUT_ELEMENT_DESC*		_vertex_layout_desc;	///<Aktualny deskryptor layoutu wierzcho³ka.
	static unsigned int				_layout_elements_count;			///<Liczba elementów w talicy deskryptora layoutu.
	static D3D11_SAMPLER_DESC		_sampler_desc;			///<Deskryptor samplera.
	static D3D11_RASTERIZER_DESC	_rasterizer_desc;		///<Deskryptor rasteryzatora.
protected:
	static ID3D11Debug*				debug_interface;		///<Pozwala na pobranie bardziej szczegó³owych informacji debugowych.
	static ID3D11Device*			device;					///<Zmienna s³u¿y do tworzenia obiektów, buforów tekstur i zasobów
	static ID3D11DeviceContext*		device_context;			///<Zmienna u¿ywana do renderowania sceny, ustawiania buforów, shaderów itp.
	static IDXGISwapChain*			swap_chain;				///<S³u¿y do prezentowania ramki na ekranie
	static ID3D11RenderTargetView*	render_target;			///<Widok bufora docelowego renderowania, czyli bufora tylnego
	static ID3D11DepthStencilView*	z_buffer_view;			///<Widok z-buffora
	static ID3D11Texture2D*			back_buffer;			///<Tekstura bufora tylnego.
	static ID3D11Texture2D*			z_buffer;				///<Tekstura z-bufora.

	static ID3D11SamplerState*		default_sampler;		///<Obiekt domyœlnego samplera
	static ID3D11InputLayout*		default_vertex_layout;	///<Layout formatu wierzcho³ka u¿ywanego dla meshy
	static ID3D11VertexShader*		default_vertex_shader;	///<Obiekt domyœlnego vertex shadera
	static ID3D11PixelShader*		default_pixel_shader;	///<Obiekt domyœlnego piksel shadera
protected:	//public:	Inicjalizacje powinien zrobiæ obiekt, który dzidziczy po tej klasie, dlatego zmieni³em.
	// Funkcje do ustawiania deskryptorów i innych parametrów
	void set_swapchain_desc				( const DXGI_SWAP_CHAIN_DESC& swap_chain_desc );
	void set_viewport_desc				( const D3D11_VIEWPORT& view_port_desc );
	void set_feature_levels				( D3D_FEATURE_LEVEL* feature_levels, unsigned int elements );
	void set_window_resolution			( unsigned int window_width, unsigned int window_height );
	void set_depth_stencil_format		( DXGI_FORMAT depth_stencil_format );
	void set_vertex_layout				( DX11_DEFAULT_VERTEX_LAYOUT layout );
	void set_vertex_layout				( D3D11_INPUT_ELEMENT_DESC* layout, unsigned int array_size );
	void set_sampler_desc				( D3D11_SAMPLER_DESC sampler_desc );
	void set_rasterizer_desc			( const D3D11_RASTERIZER_DESC& rasterizer_desc );

	// Pobieranie deskryptorów
	static DXGI_SWAP_CHAIN_DESC					get_swap_chain_desc()		{ return _swap_chain_desc; }
	static D3D11_VIEWPORT						get_viewport_desc()			{ return _view_port_desc; }
	static D3D_FEATURE_LEVEL					get_current_feature_level() { return _current_feature_level; }
	static D3D11_TEXTURE2D_DESC					get_z_buffer_desc()			{ return _z_buffer_desc; }
	static D3D11_DEPTH_STENCIL_VIEW_DESC		get_z_buffer_view_desc()	{ return _z_buffer_view_desc; }
	static D3D11_SAMPLER_DESC					get_sampler_desc()			{ return _sampler_desc; }
	static D3D11_RASTERIZER_DESC				get_rasterizer_desc()		{ return _rasterizer_desc; }

	ID3D11VertexShader*		load_vertex_shader( const std::wstring& file_name, const std::string& shader_name, const char* shader_model );
	ID3D11VertexShader*		load_vertex_shader( const std::wstring& file_name, const std::string& shader_name,
											ID3D11InputLayout** layout, D3D11_INPUT_ELEMENT_DESC* layout_desc,
											unsigned int array_size, const char* shader_model );
	ID3D11PixelShader*		load_pixel_shader( const std::wstring& file_name, const std::string& shader_name, const char* shader_model );


	// Funkcje inicjuj¹ce
	DX11_INIT_RESULT InitDevicesAndSwapChain	( HWND window, bool fullscreen, bool single_thread = true );
	DX11_INIT_RESULT init_viewport				();
	DX11_INIT_RESULT init_z_buffer_and_render_target();
	DX11_INIT_RESULT init_vertex_shader( const std::wstring& file_name, const std::string& shader_name );
	DX11_INIT_RESULT init_pixel_shader( const std::wstring& file_name, const std::string& shader_name );
	DX11_INIT_RESULT init_sampler();

protected:
	DX11APIObjects();
	~DX11APIObjects() = default;

	DX11_INIT_RESULT init_DX11( int width, int height, HWND window, bool fullscreen,
								const std::wstring& pix_shader_file, const std::string& pix_shader_name,
								const std::wstring& vert_shader_file, const std::string& vert_shader_name,
								bool single_thread = true );
	DX11_INIT_RESULT init_DX11( int width, int height, HWND window, bool fullscreen, bool single_thread = true );


	virtual void release_DirectX();

	void begin_scene();
	inline void end_scene_and_present() { swap_chain->Present( 0, 0 ); }	///<Wywo³uje funkcjê swap_chain->Present w celu wyœwietlenia narysowanej sceny na monitorze

protected:

	void			SetDebugName		( ID3D11DeviceChild* child, const std::string& name );
	bool			IsDebugLayerEnabled	();
};



/**@brief Klasa zawiera wskaŸniki na bufory sta³ych zawieraj¹ce podstawowe zestawy
elementów przekazywanych do shaderów. S¹ to miedzy innymi macierze transformacji.

@note Je¿eli w jakimkolwiek miejscu s¹ u¿ywane obiekty z tej klasy (DX11AuxiliaryObjects), a nie tylko z klasy bazowej,
to przy zwalnianiu nale¿y koniecznie wywo³aæ funkcjê release_DirectX tego obiektu, a nie klasy bazowej. Czyli innymi s³owy klasa
odpowiedzialna za zwalnianie musi odziedziczyæ po tym obiekcie nawet, je¿eli nie u¿ywa jego zmiennych.
W przeciwnym razie zostanie wywo³any funkcja wirtualna dla klasy bazowej.*/
class DX11AuxiliaryObjects : public DX11APIObjects
{
protected:
	static ID3D11Buffer*				const_per_frame;	///<Bufor sta³ych zmieniaj¹cych siê nie czêœciej ni¿ co ramkê
	static ID3D11Buffer*				const_per_mesh;		///<Bufor sta³ych zmieniaj¹cy siê dla ka¿dej czêœci mesha
	static ID3D11DepthStencilState*		depth_enabled;		///<Do w³¹czania z-bufora
	static ID3D11DepthStencilState*		depth_disabled;		///<Do wy³¹czania z-bufora

	void init_buffers( unsigned int size_per_frame, unsigned int size_per_mesh );
	void init_depth_states();

	void release_DirectX() override;
};


