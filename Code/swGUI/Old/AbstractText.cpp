#include "AbstractText.h"

using namespace XGUI;

AbstractText::AbstractText(Control* parent)
: Control( parent )
{
	enable_onTextChange = false;
	enable_onAlignmentChange = false;
	enable_onFontChange = false;
	enable_onTextColorChange = false;
	enable_onTextSizeChange = false;
}


AbstractText::~AbstractText()
{
}


//========================================================================================//
//		Funkcje wywo³ywane w reakcji na zdarzenie i implementowane przez programistê
//========================================================================================//

/**@brief Funkcja wywo³ywana, gdy zmieni siê tekst do wyœwietlania.

Tekst mo¿na pobraæ za pomoca funkcji getText lub getTextString. W momencie wywo³ania nowy
tekst jest ju¿ ustawiony.

Funkcja domyœlnie jest pusta. Programista kontrolek z klas pochodnych mo¿e zaimplementowaæ
w tej funkcji dowoln¹ funkcjonalnoœæ.

@note Aby funkcja zosta³a wywo³ana, zmienna @ref enable_onTextChange musi byæ ustawiona na true.
*/
void AbstractText::onTextChange()
{}

/**@brief Funkcja wywo³ywana, gdy zmieni siê rozmiar tekst do wyœwietlania.

Rozmiar znajduje siê w zmiennych klasy, w momencie wywo³ania ustawiony jest ju¿ nowy.

Funkcja domyœlnie jest pusta. Programista kontrolek z klas pochodnych mo¿e zaimplementowaæ
w tej funkcji dowoln¹ funkcjonalnoœæ.

@note Aby funkcja zosta³a wywo³ana, zmienna @ref enable_onTextSizeChange musi byæ ustawiona na true.
*/
void AbstractText::onTextSizeChange()
{}

/**@brief Funkcja wywo³ywana, gdy zmieni siê kolor tekstu do wyœwietlania.

Kolor znajduje siê w zmiennych klasy, w momencie wywo³ania ustawiony jest ju¿ nowy.

Funkcja domyœlnie jest pusta. Programista kontrolek z klas pochodnych mo¿e zaimplementowaæ
w tej funkcji dowoln¹ funkcjonalnoœæ.

@note Aby funkcja zosta³a wywo³ana, zmienna @ref enable_onTextColorChange musi byæ ustawiona na true.
*/
void AbstractText::onTextColorChange()
{}

/**@brief Funkcja wywo³ywana, gdy zmieni siê czcionka tekstu do wyœwietlania.

Czcionka znajduje siê w zmiennych klasy, w momencie wywo³ania ustawiona jest ju¿ nowa.

Funkcja domyœlnie jest pusta. Programista kontrolek z klas pochodnych mo¿e zaimplementowaæ
w tej funkcji dowoln¹ funkcjonalnoœæ.


@note Aby funkcja zosta³a wywo³ana, zmienna @ref enable_onFontChange musi byæ ustawiona na true.
*/
void AbstractText::onFontChange()
{}

/**@brief Funkcja wywo³ywana, gdy zmieni siê wyrównanie tekstu w zglêdem krawêdzi kotrolki.

Funkcja domyœlnie jest pusta. Programista kontrolek z klas pochodnych mo¿e zaimplementowaæ
w tej funkcji dowoln¹ funkcjonalnoœæ.

@note Aby funkcja zosta³a wywo³ana, zmienna @ref enable_onAlignmentChange musi byæ ustawiona na true.
*/
void AbstractText::onAlignmentChange()
{}

//========================================================================================//
//					Funkcje s³u¿¹ce do ustawiania ró¿nych parametrów klasy
//========================================================================================//


void AbstractText::setFont( Font* new_font )
{
	if ( !new_font )
		return;

	font = new_font;
	if ( enable_onFontChange )
		onFontChange();
}

void AbstractText::setFontSize( unsigned char new_size )
{
	if ( new_size == 0 )
		return;
	font_size = new_size;

	if ( enable_onTextSizeChange )
		onTextSizeChange();
}

void AbstractText::setAlignment( ALIGNMENT new_align )
{
	text_alignment = new_align;
	if ( enable_onAlignmentChange )
		onAlignmentChange();
}

void AbstractText::setTextColor( DirectX::XMFLOAT4 new_color )
{
	text_color = new_color;
	if ( enable_onTextColorChange )
		onTextColorChange();
}

void AbstractText::setText( std::wstring new_text )
{
	text = new_text;
	if ( enable_onTextChange )
		onTextChange();
}