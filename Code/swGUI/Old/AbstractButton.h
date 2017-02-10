#pragma once

/**
@file AbstractButton.h
@brief Plik zawiera deklaracjê klasy AbstractButton.
*/

#include "XGUI.h"

namespace XGUI
{


	class AbstractButton : public Control
	{
	private:
		int8			pushed : 1;				///<Identyfikuje czy przycisk jest aktualnie wciœniêty
	protected:
		int8			work_as_switch : 1;		///<Tryb pracy przycisku. Zmienn¹ trzeba ustawiæ w konstruktorze klasy pochodnej.

		int8			enable_onPush : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8			enable_onPull : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8			enable_onChange : 1;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu

		virtual void onPush();
		virtual void onPull();
		virtual void onChangeState();

		void pushButton();		///<Programowe wciœniêcie przycisku. Klasa pochodna mo¿e zdecydowaæ, ¿e trzeba wcisn¹æ przycisk mimo, ¿e nie kliknieto go.
	public:
		AbstractButton( Control* parent );
		~AbstractButton();

		inline bool isUp() const { return !pushed && 1; }		///<Zwraca informacjê czy przycisk jest niewciœniêty
		inline bool isDown() const { return pushed && 1; }		///<Zwraca informacjê czy przycisk jest wciœniêty
		inline bool isChecked() const { return pushed && 1; }	///<Zwraca informacjê czy przycisk jest wciœniêty
		inline bool isSwitch() const { return work_as_switch && 1; }	///<Zwraca informacje czy przycisk jest switchem czy przyciskiem wciskanym


		XGUIEventDelegate		eventPush;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventPull;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventChangeState;	///<Delegat, który zostanie wywo³any w momencie powstania eventu.
	};







}