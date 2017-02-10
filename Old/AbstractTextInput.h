#pragma once
/**@file AbstractTextInput.h
@brief Plik zawiera deklaracjê klasy AbstractTextInput.*/

#include "AbstractText.h"

namespace XGUI
{
	/**@brief Klasa bazowa dla pól tekstowych.

	Umo¿liwia wpisywanie tekstu. Obs³uguje po³o¿enie karetki oraz zaznaczanie tekstu.
	Tekst jest zaznaczony od po³o¿enia karetki do indeksu wskazywanego przez selection.
	Nale¿y pamiêtaæ, ¿e oba indeksy wskazuj¹ na miejsce przed znakiem.
	Je¿eli zmienne s¹ równe, oznacza to, ¿e nic nie jest zaznaczone.*/
	class AbstractTextInput	:	public Control
	{
	private:
		AbstractText*	text;		///<Kontrolka do wyœwietlania tekstu.

		uint16			caret;		///<Po³o¿enie karetki w tekœcie (indeks znaku). Karetka znajduje siê przed wskazywanym znakiem.
		uint16			selection;	///<Indeks ostatniego zaznaczonego elementu. Pierwszym zaznaczonym elementem jest znak wskazywany przez karetkê. caret == selection oznacza brak zaznaczenia.
		uint16			max_length;	///<Maksymalna d³ugoœæ tekstu, jak¹ mo¿e wpisaæ uzytkownik.
	protected:
		int8			enable_onTextChanged : 1;
		int8			enable_onCaretChanged : 1;

	public:
		AbstractTextInput( Control* parent );
		~AbstractTextInput( );

		XGUIEventDelegate			eventTextChanged;
		XGUIEventDelegate			eventCaretChanged;

		virtual void onTextChanged();
		virtual void onCaretChanged();

		inline uint16 getMaxLength() const { return max_length; }
		std::wstring getSelectedTextString() const;
		inline std::wstring getTextString() const  { return text->getTextString(); }
		//const wchar_t* getText() const { return text->getTextStringRef().c_str(); }		///<Nie tworzy nowego stringa.

		void setMaxLength( uint16 new_length );
	};

}