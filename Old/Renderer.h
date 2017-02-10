#pragma once
/**
@file Renderer.h
@brief Plik zawiera deklaracjê interfejsu Renderer s³u¿¹cego do renderowania obiektów GUI.
*/

#include "XGUI.h"
#include <string>

namespace XGUI
{
	enum class VertexLayout
	{
		POSITION_ONLY,
		POSITION_COLOR,
		POSITION_NORMAL,
		POSITION_NORMAL_COLOR,
		POSITION_UV,
		POSITION_NORMAL_UV
	};


	/**@brief Klasa stanowi interfejs klas renderuj¹cych obiekty GUI.*/
	class Renderer
	{
		friend void Control::draw_clipped( const Rect& );	///<Ma prawo ustawiæ prostok¹t obcinaj¹cy.
	private:
	protected:
		virtual void setClippingRect( const Rect& );
	public:

		Renderer() = default;
		virtual ~Renderer();

		virtual VertexShader* addVertexShader( const std::wstring& file_name, const std::string& shader_entry ) = 0;
		virtual PixelShader* addPixelShader( const std::wstring& file_name, const std::string& shader_entry ) = 0;
		virtual Texture* addTexture( const std::wstring file_name ) = 0;

		virtual void bindVertexShader( VertexShader* shader ) = 0;
		virtual void bindPixelShader( PixelShader* shader ) = 0;
		virtual void bindTexture( Texture* texture, uint8 slot ) = 0;
		virtual void bindTextures( Texture** texture, uint8 slot, uint8 num_of_slots ) = 0;
		virtual void bindVertexBuffer();	///>@todo
		virtual void bindConstantBuffer();	///>@todo

		virtual VertexLayout getVertexLayout() = 0;
		virtual void setVertexLayout( VertexLayout layout ) = 0;

		virtual void beginGUI();	///<Funkcja wywo³ywana na poczatku renderowania GUI. Mo¿na ustawiæ porzadany stan karty graficznej.
		virtual void endGUI();		///<Funkcja wywo³ywana na koniec renderowania GUI. Mo¿na przywróciæ stan sprzed renderowania GUI.	
	};

}