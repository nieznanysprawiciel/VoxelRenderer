/**@file memory_leaks.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik s³u¿y do wykrywania wycieków pamiêci.
*/

/**@brief Plik umo¿liwiaj¹cy œledzenie wycieków pamiêci.

Aby uruchomiæ sprawdzanie wycieków pamiêci nale¿y zaincludowaæ ten plik w testowanym pliku .cpp.
Aby wszystko kompilowa³o siê poprawnie, musi byæ on ostatnim do³¹czanym plikiem ze wszystkich.
W innym razie definicje bêd¹ oddzia³ywa³y tak¿e na wszystki zaincludowane pliki poni¿ej tego, co powoduje
nieprzewidywalne dzia³anie.

Informacje o wyciekach s¹ wypisywane na standardowe wyjœcie, na które s¹ wysy³ane komunikaty kompilacji.

@note Aby wszystkie definicje siê za³¹czy³y trzeba zadbaæ, aby makro @ref SHOW_MEMORY_LEAKS w pliku
@ref macros_switches.h by³o zdefiniowane.
Je¿eli @ref macros_switches.h nie jest includowany, to trzeba gdzieœ samemu to makro zdefiniowaæ.
*/

#ifdef SHOW_MEMORY_LEAKS
	#define _CRTDBG_MAP_ALLOC
	#include <cstdlib>
	#include <crtdbg.h>

	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW

#endif