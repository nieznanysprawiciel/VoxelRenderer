#pragma once

/**
@file AbstractText.h
@brief Plik zawiera deklaracjê klasy AbstractText.
*/

#include "XGUI.h"
#include <string>

namespace XGUI
{
	class Font;

	/**@brief Klasa bazowa do wyœwietlania tekstu.

	Klasa ma za zadanie wyswietliæ go³y tekst na przezroczystym tle
	ewentualnie mo¿e byæ jednolite wype³nienie. Nie jest to kontrolka pozawalaj¹ca na jakiekolwiek
	modyfikowanie tekstu, ani nie jest to pole tekstowe.
	*/
	class AbstractText :	public Control
	{
	private:
		std::wstring		text;			///<String zawieraj¹cy tekst do wyœwietlenia.
		DirectX::XMFLOAT4	text_color;		///<Kolor tekstu. Mo¿e zawieraæ kana³ alfa, ale nie ma gwarancji, ¿e zostanie on u¿yty jako przezroczystoœæ.
		ALIGNMENT			text_alignment;	///<Wyrównanie tekstu wzglêdem obszaru kontrolki.
		Font*				font;			///<WskaŸnik na obiekt zawieraj¹cy czcionkê.
		unsigned char		font_size;		///<Rozmiar czcionki.

	protected:
		int8	enable_onTextChange : 1;			///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8	enable_onTextSizeChange : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8	enable_onTextColorChange : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8	enable_onFontChange : 1;			///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8	enable_onAlignmentChange : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu

		virtual void onTextChange();
		virtual void onTextSizeChange();
		virtual void onTextColorChange();
		virtual void onFontChange();
		virtual void onAlignmentChange();

		//inline const std::wstring& getTextStringRef() const { return text; }	///<Zwraca referencjê na przechowywanego stringa.
	public:
		AbstractText( Control* parent );
		~AbstractText();

		inline Font* getFont() const { return font; }					///<Zwraca wskaŸnika na obiekt czcionki.
		inline const wchar_t* getText() const { return text.c_str(); }	///<Zwraca wskaŸnik na tablicê znaków zawieraj¹c¹ tekst.
		inline std::wstring getTextString() const { return text; }		///<Zwraca string zawieraj¹cy tekst.
		inline unsigned char getFontSize() const { return font_size; }	///<Zwraca rozmiar czcionki.
		inline ALIGNMENT getTextAlignment() const { return text_alignment; }	///<Zwraca wyrównanie tekstu wzglêdem swojego obszzaru renderowania.
		inline DirectX::XMFLOAT4 getTextColor() const { return text_color; }	///<Zwraca kolor tekstu.

		void setFont( Font* new_font );									///<Ustawia czcionkê.
		void setFontSize( unsigned char new_size );						///<Ustawia wielkoœæ czcionki.
		void setAlignment( ALIGNMENT new_align );						///<Ustawia wyrównanie tekstu wzglêdem obszaru kontrolki.
		void setTextColor( DirectX::XMFLOAT4 new_color );				///<Ustawia kolor tekstu.
		void setText( std::wstring new_text );							///<Ustawia nowy tekst do wyœwietlania.
	};

}
