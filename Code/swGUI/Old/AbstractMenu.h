#pragma once
/**
@file AbstractMenu.h
@brief Plik zawiera deklaracjê klasy AbstractMenu.
*/

#include "XGUI.h"
#include <vector>

namespace XGUI
{
	class AbstractItem;

	/**@brief Klasa bazowa dla klas wyœwietlaj¹cych menu.
	
	Klasa mo¿e wyœwietlaæ zarówno menu takie jak jest na belce okna, jak równie¿
	menu kontekstowe i inne temu podobne.
	
	@todo Wszystko jeszcze do wymyœlenia.*/
	class AbstractMenu : public Control
	{
	private:
		std::vector<AbstractItem*>		child_items;
		/**@brief Rozmiar obszaru w którym narysuje siê kontrolka.

		Ze wzglêdu na to, ¿e w menu dzieci kontrolki nie znajduj¹ siê wewn¹trz obszaru rodzica,
		to istniej¹ dwa wymiary charakteryzuj¹ce kontrolkê. Zmienna dimmension podaje obszar
		obcinania przy renderowaniu (czêsto jest to ca³y ekran). Zmienna visible_dimmension
		opisuje rozmiar faktyczny, jaki kontrolka bêdzie zajmowaæ.*/
		DirectX::XMFLOAT2				visible_dimmension;
	protected:
		int8		selected : 1;		///<Informacja czy podmenu (dziecko naszej klasy) zosta³o otwarte/wybrane.
	public:
		AbstractMenu( Control* parent );
		~AbstractMenu();

		inline bool isSelected() const { return selected && 1; }
		inline DirectX::XMFLOAT2 getVisibleDimmension( ) const { return visible_dimmension; }
	};

}
