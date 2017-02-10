#pragma once
/**
@file AbstractWindow.h
@brief Plik zawiera deklaracjê klasy AbstractWindow.
*/

#include "XGUI.h"
#include "AbstractMenu.h"
#include "AbstractButton.h"


namespace XGUI
{
	/**@brief Klasa bazowa dla okien.
	
	Klasa mo¿e stanowiæ podstawê dla innych typów kontrolek, mniej przypominaj¹cych
	okna, np. kontenerów dla innych kontrolek oraz kontrolek u¿ytkownika realizuj¹cych
	funkcjonalnoœci zupe³nie niepodobne do innych klas bazowych.*/
	class AbstractWindow : public Control
	{
	private:
		int8		resizeable : 1;			///<Czy okno mo¿e zmieniaæ rozmiar.
		int8		moveable : 1;			///<Czy okno mo¿e byæ przesuwane przez u¿ytkownika.
		int8		close_button : 1;		///<Czy okno ma przycisk zamykaj¹cy.
		int8		minimize_button : 1;	///<Czy okno ma przycisk minimalizuj¹cy. @todo Wymyœleæ ca³y mechanizm.
		int8		maximize_button : 1;	///<Czy okno ma przycisk maksymalizuj¹cy. @todo Wymyœleæ ca³y mechanizm.
		int8		window_menu : 1;		///<Czy okno ma w³asne menu.

		int8		is_moving : 1;			///<Zmienna ustawiana, je¿eli okno jest w trakcie przesuwania. @note Musi byæ ustawiona zmienna moveable.
		int8		is_resizing : 1;		///<Zmienna ustawiana, je¿eli okno jest w trakcie zmieniania rozmiaru. @note Musi byæ ustawiona zmienna resizeable.
	protected:
		int8		enable_onResizeWindow : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu.
		int8		enable_onMinimizeWindow : 1;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu.
		int8		enable_onMaximizeWindow : 1;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu.
		int8		enable_onCloseWindow : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu.

		AbstractMenu*		menu;						///<WskaŸnik na obiekt menu. @note Musi byæ ustawiona zmienna window_menu.
		AbstractButton*		close_window_button;		///<WskaŸnik na przycisk zamykaj¹cy okno. @note Musi byæ ustawiona zmienna close_button.
		AbstractButton*		minimize_window_buttn;		///<WskaŸnik na przycisk minimalizuj¹cy okno. @note Musi byæ ustawiona zmienna minimize_button.
		AbstractButton*		maximize_window_button;		///<WskaŸnik na przycisk maksymalizuj¹cy okno. @note Musi byæ ustawiona zmienna maximize_button.

		virtual void onResizeWindow();
		virtual void onMinimizeWindow();
		virtual void onMaximizeWindow();
		virtual void onCloseWindow();
	public:
		AbstractWindow( Control* parent );
		~AbstractWindow();

		XGUIEventDelegate		eventResizeWindow;		///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventMinimizeWindow;	///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventMaximizeWindow;	///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventCloseWindow;		///<Delegat, który zostanie wywo³any w momencie powstania eventu.

		inline bool isResizeable() const { return resizeable && 1; }
		inline bool isResizing() const { return is_resizing && 1; }
		inline bool isMoveable() const { return moveable && 1; }
		inline bool isMoving() const { return is_moving && 1; }
		inline bool hasCloseButton() const { return close_button && 1; }
		inline bool hasMenu() const { return window_menu && 1; }
		inline bool hasMinimizeButton() const { return minimize_button && 1; }
		inline bool hasMaximizeButton() const { return maximize_button && 1; }

		inline AbstractMenu* getMenu() const { return menu; }
	};

}
