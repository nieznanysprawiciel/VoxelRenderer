#pragma once
/**
@file AbstractItem.h
@brief Plik zawiera deklaracjê klasy AbstractItem.
*/
#include "XGUI.h"
#include <vector>

namespace XGUI
{
	class AbstractMenu;

	/**@brief Klasa bazowa dla obiektów wybieranych w menu.

	@todo Wszystko jeszcze do wymyœlenia.
	*/
	class AbstractItem : public Control
	{
	private:
		std::vector<AbstractMenu*>		child_items;
		/**@brief Rozmiar obszaru w którym narysuje siê kontrolka.
		
		Ze wzglêdu na to, ¿e w menu dzieci kontrolki nie znajduj¹ siê wewn¹trz obszaru rodzica,
		to istniej¹ dwa wymiary charakteryzuj¹ce kontrolkê. Zmienna dimmension podaje obszar
		obcinania przy renderowaniu (czêsto jest to ca³y ekran). Zmienna visible_dimmension
		opisuje rozmiar faktyczny, jaki kontrolka bêdzie zajmowaæ.*/
		DirectX::XMFLOAT2				visible_dimmension;
	protected:
		int8		selected : 1;		///<Informacja czy podmenu (dziecko naszej klasy) zosta³o otwarte/wybrane.
		int8		pushbutton : 1;		///<Przycisk mo¿e wymagaæ wciœniêcia lub wystarczy najechaæ mysz¹.
	public:
		AbstractItem( Control* parent );
		~AbstractItem();

		inline bool isSelected( ) const { return selected && 1; }
		inline bool isPushButton() const { return pushbutton && 1; }
		inline DirectX::XMFLOAT2 getVisibleDimmension() const { return visible_dimmension; }
	};

}
