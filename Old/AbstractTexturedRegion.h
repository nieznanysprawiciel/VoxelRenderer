#pragma once
/**@file AbstractTexturedRegion.h
@brief Plik zawiera deklaracjê klasy AbstractTexturedRegion.*/

#include "XGUI.h"

namespace XGUI
{
	class Texture;

	/**@brief Klasa jest interfejsem dla obiektów renderuj¹cych oteksturowany region.*/
	class AbstractTexturedRegion : public Control
	{
	private:
		Texture*			texture;		///<WskaŸnik na obiekt tekstury. Nie ma sk³¹dowych ani funkcji. S³u¿y jedynie jako identyfikator dla renderera.
	protected:
		int8				enable_onChangeTexture : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu

		virtual void onChangeTexture();
	public:
		AbstractTexturedRegion( Control* parent );
		~AbstractTexturedRegion();

		inline Texture* getTexture() const { return texture; }	///<Zwraca aktualnie u¿ywan¹ teksturê.
		void setTexture( Texture* new_texture );
	};

}

