#include "XGUI.h"


using namespace XGUI;

/**@brief Funkcja rysuj¹ca zewnêtrzna ustawiaj¹ca prostok¹t obcinania i inne parametry rysowania.

Wykonywane s¹ nastêpuj¹ce czynnoœci:
- Sprawdzane jest czy kontrolka jest widoczna. Nie jest renderowana, je¿eli visible jest ustawione na false.
- Obszar obcinania podany w parametrze jest ustawiany w zmiennych DirectX.
- Wywo³ywana jest funkcja onDraw, która powinna byc zaimplementowana w klasie pochodnej.

@param clipping_rect Czworok¹t, wewn¹trz którego ma zostaæ narysowana kontrolka.
*/
void Control::draw_clipped( const Rect& clipping_rect )
{


}

