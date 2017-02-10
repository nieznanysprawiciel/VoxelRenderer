#pragma once
/**
@file GUIEvent.h
@brief Plik zawiera Deklaracje struktur eventów oraz enumeracje s³u¿¹c¹ do identyfikowania ich typów.
*/


namespace XGUI
{

	/**@brief Enumeracja identyfikuj¹ca mo¿liwe do wygenerowania eventy.*/
	enum EventType
	{

	};


	/**@brief Struktura przekazywana w funkcjach obs³ugi zdarzeñ.

	Wszystkie struktury opisuj¹ce eventy dziedzicz¹ po tej strukturze.
	*/
	struct GUIEvent
	{
		unsigned int type;		///< Jedna z wartoœci enuma @ref EventType


		virtual ~GUIEvent();
	};

}