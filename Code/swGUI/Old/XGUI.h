#pragma once

/**
@file XGUI.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Ten plik nale¿y za³¹czyæ w projekcie u¿ywaj¹cym biblioteki DirectXGUI.
Zawiera podstawowe deklaracje.*/

#include <DirectXMath.h>
#include "FastDelegate/FastDelegate.h"
#include "GUIEvent.h"
#include "Common/TypesDefinitions.h"


namespace XGUI
{
	class Control;
	class Root;
	class Renderer;


	/**@defgroup EngineGUI
	@ingroup ModulesStructure
	@brief Modu³ s³u¿¹cy do renderowania interfejsu graficznego u¿ytkownika.*/


	/**@typedef fastdelegate::FastDelegate1<GUIEvent*> XGUIEventDelegate
	@brief Definicja delegata u¿ywanego przez XGUI.

	Aby zdarzenia generowane wewnatrz GUI mog³y zostaæ obs³u¿one na zewn¹trz,
	u¿ytkownik GUI musi przypisaæ temu delegatowi odpowiedni¹ funkcjê obs³ugi.
	Funkcja ta dostaje w parametrze wskaŸnik na strukturê @ref Event, która zawiera
	wszystkie informacje o danym zdarzeniu. WskaŸnik ten trzeba sobie zrzutowaæ w funkcji
	obs³ugi na wskaŸnik na jeden z obiektów pochodnych odpowiadaj¹cych konkretnemu typowi zdarzenia.

	Ustawienie delegata mo¿na przeprowadziæ przypisuj¹c mu po prostu odpowiedni¹ funkcjê (delegaci s¹ sk³adowymi publicznymi).
	Dla funkcji globalnych i metod statycznych dostêpne s¹ opcje:
	- Delegat.bind(&Funkcja);
	- Delegat.bind(&Klasa::MetodaStatyczna);
	- Delegat = &FunkcjaGlobalna;
	- Delegat = &Klasa::MetodaStatyczna;

	Dla normalnych metod sk³adowych:
	- Delegat.bind(&ObjektKlasy, &Klasa::Metoda);
	- Delegat = fastdelegate::MakeDelegate(&ObjektKlasy, &Klasa::Metoda);

	@attention W przypadku implementowania w³asnych klas GUI, nie nale¿y u¿ywaæ delegatów do
	implementowania wewnêtrzych funkcjonalnoœci jakiejœ kontrolki. Do takiego celu s¹ przygotowane
	specjalne funkcje virtualne, które mo¿na napisaæ.

	Konwencja nazewnictwa jest nastêpuj¹ca:
	- delegaci maj¹ nazwy postaci event[nazwa_zdarzenia]
	- funkcje wewnêtrzne obs³ugi zdarzeñ dla kontrolek maj¹ nazwy on[nazwa_zdarzenia]

	Oba typy funkcji s¹ wywo³ywane w tych samych okolicznoœciach. Najpierw jest wywo³ywana obs³uga funkcji
	wewnêtrznej, a dopiero potem zdarzenie zewnêtrzne (zak³adaj¹c, ¿e oba typy bœ³ugi s¹ przez kontrolkê u¿ywane).

	@attention Je¿eli jakieœ niuanse sprawi¹, ¿e zdarzenia o tej samej nazwie nie bêd¹ wywo³ywane w tych samych
	okolicznoœciach to trzeba nadaæ im inne nazwy.
	*/
	typedef fastdelegate::FastDelegate1<GUIEvent*> XGUIEventDelegate;

	/**@brief Struktura opisuj¹ca czworok¹t.*/
	struct Rect
	{
		DirectX::XMFLOAT2 top_left;		///<Wspó³rzêdne lewego górnego rogu.
		DirectX::XMFLOAT2 bottom_right;	///<Wspó³rzêdne prawego dolnego rogu.
	};

	/**@brief Klasa bazowa dla obiektów reprezentuj¹cych teksturê.
	
	Klasa nie zawiera niczego. S³u¿y jedynie do dalszego dziedziczenia.
	Jest potrzebna obiektom GUI, które musz¹ przechowywaæ jakiœ identyfikator
	tekstury, ale nie mog¹ znaæ jej implementacji (bo zale¿y ona od klasy Renderer,
	której implementacji równie¿ nie znamy z poziomy kontrolki).*/
	class Texture{};
	class PixelShader{};	///<Klasa bazowa dla pixel shaderów. @see Texture
	class VertexShader{};	///<Klasa bazowa dla vertex shaderów. @see Texture

	/**@brief Klasa s³u¿¹ca do przekazywania zawartoœci schowak windows do kontrolek.*/
	class ClipboardData{};

	/**@brief Enumeracja opisuj¹ca mo¿liwe wyrównanie kontrolek wzglêdem rodzica.
	
	Zmienne enumeracjê mo¿na sk³adaæ za pomoca operatora sumy bitowej.
	U¿ytkownik jest odpowiedzialny za sk³adanie zmiennej z sensem.*/
	enum ALIGNMENT
	{
		LEFT				= 1,
		RIGHT				= 2,
		TOP					= 4,
		BOTTOM				= 8,
		VERTICAL_CENTER		= 16,
		HORIZONTAL_CENTER	= 32,
		TOP_LEFT			= LEFT | TOP,
		TOP_RIGHT			= RIGHT | TOP,
		CENTER_LEFT			= VERTICAL_CENTER | LEFT,
		CENTER_RIGHT		= VERTICAL_CENTER | RIGHT,
		BOTTOM_LEFT			= BOTTOM | LEFT,
		BOTTOM_RIGHT		= BOTTOM | RIGHT,
		TOP_CENTER			= TOP | HORIZONTAL_CENTER,
		BOTTOM_CENTER		= BOTTOM | HORIZONTAL_CENTER
	};


	/**@brief Klasa bazowa dla wszystkich kontrolek.*/
	class Control
	{
	private:
		static Root*			root;		///<Korzeñ drzewa kontrolek bêd¹cy jednoczeœnie zarz¹dc¹ ca³ego GUI
		static Renderer*		renderer;	///<Obiekt poœrednicz¹cy w renderowaniu GUI.

		Control*				parent;		///<Rodzic kontrolki.

		// W³aœciwoœci ustawiane przez u¿ytkownika GUI lub implementacjê (decyzja podejmowana przez klasê potomn¹).
		DirectX::XMFLOAT2		dimension;			///<Rozmiar kontrolki (we wspó³rzêdnych ekranu [-1;1])

		// W³aœciwoœci ustawiane przez u¿ytkownika GUI.
		DirectX::XMFLOAT2		relative_position;	///<Pozycja wzglêdem rodzica (we wspó³rzêdnych ekranu [-1;1])
		ALIGNMENT				alignment;			///<Identyfikuje wzglêdem czego s¹ podawane wspó³rzêdne position

		// W³aœciwoœci ustawiane przez mechanizmy GUI.
		int8					mouse_on : 1;				///<Przy ostatnim sprawdzaniu myszka by³a w obszarze kontrolki.

		// W³aœciwoœci ustawiane przez u¿ytkownika.
		int8					visible : 1;				///<Widocznoœæ kontrolki
		int8					disabled : 1;				///<Umo¿liwia wy³¹czenie przycisku (mimo ¿e jest widoczny)
	protected:
		// W³aœciwoœci ustawiane przez implementacjê kontrolki.	
		int8					focus_change_order : 1;		///<W przypadku dostania focusa kontrolka mo¿e zostaæ przeniesiona na pocz¹tek listy, ¿eby byæ renderowan¹ na wierzchu.

		int8					enable_onMouseOn : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onMouseOut : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onLeftClick : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onLeftUnClick : 1;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onRightClick : 1;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onRightUnClick : 1;	///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onFocusSet : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onFocusLost : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
		int8					enable_onDisable : 1;		///<W³¹cza wywo³ywanie funkcji w momencie powstania eventu
	public:
		Control( Control* set_parent ) { parent = set_parent; }

		// Delegaci ustawiani przez u¿ytkownika GUI.
		XGUIEventDelegate		eventMouseOn;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventMouseOut;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventLeftClick;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventLeftUnClick;		///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventRightClick;		///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventRightUnClick;		///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventFocusSet;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.
		XGUIEventDelegate		eventFocusLost;			///<Delegat, który zostanie wywo³any w momencie powstania eventu.


		inline bool isMouseOn() const  { return mouse_on && 1; }				///<Funkcja zwraca wartoœæ zmiennej @ref mouse_on
		inline bool isVisible() const { return visible && 1; }					///<Funkcja zwraca wartoœæ pola @ref visible
		inline bool isChangeOrder() const { return focus_change_order && 1; }	///<Zwraca wartoœæ pola @ref focus_change_order
		inline bool isDisabled() const { return disabled && 1; }				///<Zwraca wartoœæ pola disabled.
		
		void setVisible( bool value );		///<Ustawia widocznoœæ kontrolki (niewidoczne nie s¹ renderowane).
		void setDisabled( bool value );		///<Blokuje lub odblokowuje kontrolkê (kontrolka jest renderowana, ale nie mo¿na jej u¿yæ).
		
		inline Control* getParent() { return parent; }						///<Zwraca rodzica kontrolki
		inline ALIGNMENT getAlignment() { return alignment; }				///<Zwraca enum opisuj¹cy wyrównanie kontrolki wzglêdem rodzica
		inline DirectX::XMFLOAT2 getDimension() { return dimension; }		///<Zwraca rozmiar kontrolki
		inline DirectX::XMFLOAT2 getRelativePosition() { return relative_position; }	///<Zwraca pozycjê kontrolki wzglêdem rodzica
		inline Renderer* getRenderer() const { return renderer; }			///<Zwraca obiekt renderer.

		void setAlignment( ALIGNMENT align );						///<Ustawia wyrównanie kontrolkie wzglêdem rodziaca.
		void setRelativePosition( DirectX::XMFLOAT2 position );		///<Ustawia pozycje kontrolki wzgldem rodzica. O wyrówaniu do krawêdzi decyduje zmienna alignment.

	private:
		/**Ustawia rozmiar kontrolki. Klasa pochodna mo¿e udostêpniæ metodê publiczn¹ do tego,
		je¿eli chce, aby u¿ytkownik móg³ zmieniaæ rozmiar.*/
		void setDimmension( DirectX::XMFLOAT2 dimm );
	public:
		/**@brief Funkcja sprawdza czy mysz znajduje siê wewn¹trz obszaru danej kontrolki.

		Domyœlnie zaimplementowane jest sprawdzanie czy kursor znajduje siê w odpowiednim
		prostok¹cie. Je¿eli kszta³t kontrolki jest inny, mo¿na tê funkcjê przeimplementowaæ
		dla tego kszta³tu.

		Po³o¿enie kontrolki jest obliczane wzgledem podanego prostok¹ta w parametrze clipping_rect.
		Zmienna align mówi o tym, wzglêdem której krawêdzi zdefiniowane jest po³o¿enie
		w wektorze relative_position.

		@attention
		Obiekt nie sprawdza czy mysz znalaz³a siê w jego obszarze, je¿eli nie jest widoczny.

		@note
		Funkcja nie powinna zmieniaæ stanu obiektu. Po sprawdzeniu czy mysz jest w obszarze
		kontrolki nastêpujê najczêœciej wywo³anie funkcji on_mouse_reaction. Tam nale¿y implementowaæ
		reakcjê na mysz.

		@note
		Nie zawsze musimy zwróciæ prawdziw¹ informacjê. Je¿eli chcemy zaimplementowaæ zmianê
		rozmiarów kontrolki, to mysz czêsto bêdzie wyje¿d¿aæ poza jej obszar. Wtedy musimy
		zwróciæ true, mimo ¿e mysz nie znajduje siê w naszym obszarze.

		@param point Punkt w pikselach, w których znajduje siê mysz.
		@param clipping_rect Obszar rodzica wzglêdem którego jest liczone po³o¿enie kontrolki.
		@return Zwraca true, je¿eli mysz znajduje siê w obszarze zajmowanym prze kontrolkê.
		*/
		virtual bool test_mouse( const DirectX::XMFLOAT2& point, const Rect& clipping_rect);

		/**@brief Funkcja implementuje reakcjê kontrolki na to, ¿e mysz znalaz³a siê w jej obszarze.

		Najpierw kontrolka jest sprawdzana funkcj¹ test_mouse. Je¿eli zwróci true, to powinna zostaæ
		wywo³ana funkcja on_mouse_reaction, w której znajduje siê reakcja na najechanie mysz¹.

		@param point Punkt w pikselach, w których znajduje siê mysz.
		@param clipping_rect Obszar rodzica wzglêdem którego jest liczone po³o¿enie kontrolki.
		@param buttons Tablica przycisków. @todo: Trzeba wymyœleæ format tej tablicy
		*/
		virtual void on_mouse_reaction( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );
	protected:
		/**@brief Funkcja zostaje wywo³ana, kiedy mysz najedzie na kontrolkê. Wywo³ywana tylko
		raz za pierwszym razem, kiedy zostanie wykryta mysz nad kontrolk¹.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci,
		wywo³ywanej przez najechanie mysz¹, bez przedefiniowywania funkcji on_mouse_reaction.
		Tak¹ funkcjonalnoœci¹ mo¿e byæ np. zmiana wygl¹du kontrolki po najechaniu i po zjechaniu myszy
		z jej obszaru ( @ref onMouseOut ).

		@attention
		Do renderingu przeznaczona jest tylko i wy³¹cznie funkcja @ref draw_clipped. W tej funkcji mo¿na
		co najwy¿ej ustawiæ odpowiedni stan obiektu, ¿eby póŸniej zosta³ on poprawnie wyrenderowany.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w których znajduje siê mysz.
		@param clipping_rect Obszar rodzica wzglêdem którego jest liczone po³o¿enie kontrolki.
		@param buttons Tablica przycisków. @todo: Trzeba wymyœleæ format tej tablicy
		*/
		virtual void onMouseOn( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );
		
		/**@brief Funkcja wywo³ywana w momencie przekazania kontrolce focusa.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na otrzymanie focusa.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param prev_focus Zwraca wskaŸnik na kontrolkê, która poprzednio mia³a ustawionego focusa.
		*/
		virtual void onFocusSet( Control* prev_focus );

		/**@brief Funkcja wywo³ywana w momencie, gdy kontrolka traci focusa.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na utratê focusa.

		@attention
		Funkcja nie powinna próbowaæ przywracaæ sobie focusa. Istanieje niebezpieczeñstwo zapêtlenia
		lub nieprzewidywalnego zachowania, je¿eli dwie kotrolki bêd¹ próbowa³y wykonaæ tê operacjê.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param next_focus WskaŸnik na kontrolkê, która otrzyma³a focusa.
		*/
		virtual void onFocusLost( Control* next_focus );

		/**@brief Funkcja wywo³ywana w momencie, gdy kontrolka zostanie klikniêta lewym przyciskiem myszy.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na klikniêcie.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w których znajduje siê mysz.
		@param clipping_rect Obszar rodzica wzglêdem którego jest liczone po³o¿enie kontrolki.
		@param buttons Tablica przycisków. @todo: Trzeba wymyœleæ format tej tablicy
		*/
		virtual void onLeftClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Lewy przycisk myszy zosta³ puszczony.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na puszczenie przycisku.

		@attention
		Funkcja nie zostanie wywo³ana, je¿eli puszczenie przycisku nie nast¹pi nad kontrolk¹. W przypadku
		przeniesienia siê znad obszaru jednej kontrolki nad inn¹ z wciœniêtym przyciskiem, pierwsza kontrolka
		dostanie zdarzenie click, a druga un_click.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w których znajduje siê mysz.
		@param clipping_rect Obszar rodzica wzglêdem którego jest liczone po³o¿enie kontrolki.
		@param buttons Tablica przycisków. @todo: Trzeba wymyœleæ format tej tablicy
		*/
		virtual void onLeftUnClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Funkcja wywo³ywana w momencie, gdy kontrolka zostanie klikniêta prawym przyciskiem myszy.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na klikniêcie.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w których znajduje siê mysz.
		@param clipping_rect Obszar rodzica wzglêdem którego jest liczone po³o¿enie kontrolki.
		@param buttons Tablica przycisków. @todo: Trzeba wymyœleæ format tej tablicy
		*/
		virtual void onRightClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Prawy przycisk myszy zosta³ puszczony.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na puszczenie przycisku.

		@attention
		Funkcja nie zostanie wywo³ana, je¿eli puszczenie przycisku nie nast¹pi nad kontrolk¹. W przypadku 
		przeniesienia siê znad obszaru jednej kontrolki nad inn¹ z wciœniêtym przyciskiem, pierwsza kontrolka
		dostanie zdarzenie click, a druga un_click.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w których znajduje siê mysz.
		@param clipping_rect Obszar rodzica wzglêdem którego jest liczone po³o¿enie kontrolki.
		@param buttons Tablica przycisków. @todo: Trzeba wymyœleæ format tej tablicy
		*/
		virtual void onRightUnClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );


		/**@brief Mysz opuœci³a obszar rodzica. Sytuacje, w których jest wywo³ywana funkcja,
		s¹ opisane w opisie funkcji @ref Control::mouse_out.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na puszczenie przycisku.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.
		
		@param point Wspó³rzêdne myszy.
		*/
		virtual void onMouseOut( const DirectX::XMFLOAT2& point );

		/**@brief Kontrolka zosta³a zablokowana (lub odblokowana).

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiejœ typowej dla siebie funkcjonalnoœci
		w reakcji na zablokowanie.

		@note
		Informacja o zajœciu zdarzenia zostaje równie¿ przekazana na zewn¹trz GUI w postaci wywo³ania
		odpowiedniej funkcji obs³ugi lub wys³ania eventu do kolejki komunikatów. Nie nale¿y samodzielnie
		implementowaæ takiego zachowania, poniewa¿ jest ono zaimplementowane w funkcji on_mouse_reaction.

		$param[in] disable Je¿eli wartoœæ jest ustawiona na true, to kontrolka zosta³a zablokowana, w przeciwnym
		razie zosta³a odblokowana.
		*/
		virtual void onDisable( bool disable );

		/**@brief Funkcja rysuj¹ca, która powinna zostaæ zaimplementowana w klasach potomnych.

		Rysowanie kontrolki zaczyna siê od funkcji draw_clipped, która jest wywo³ywana przez rodzica
		kontrolki.
		Wykonywane s¹ nastêpuj¹ce czynnoœci:
		- Sprawdzane jest czy kontrolka jest widoczna. Nie jest renderowana, je¿eli visible jest ustawione na false.
		- Obszar obcinania podany w parametrze jest ustawiany w zmiennych DirectX.
		- Wywo³ywana jest funkcja onDraw, która powinna byc zaimplementowana w klasie pochodnej.

		Funkcja ta powinna wywo³aæ funkcje draw_clipped wszystkich kontrolek, które s¹ jej dzieæmi i powinny
		siê narysowaæ.

		Co prawda kontrolki musz¹ byæ wewn¹trz obszaru rodzica, ale nie ma gwarancji,
		¿e na siebie nie wchodz¹. Je¿eli w funkcji test_mouse schodzono rekurencyjnie od pocz¹tku listy
		dzieci, to rysowaæ wypada w odwrotnej kolejnoœci.

		Wewn¹trz funkcji nie ma koniecznoœci pilnowania prostok¹ta obcinania, poniewa¿ jest to robione
		sprzêtowo przez DirectXa. Mimo to podawany jest w parametrze prostok¹t, wewn¹trz którego odbywa siê
		rysowanie. Mo¿na to wykorzystaæ w celu optymalizacji, aby nie rysowaæ kontrolek, które i tak zostan¹
		obciête.

		@param clipping_rect Czworok¹t, wewn¹trz którego ma zostaæ narysowana kontrolka.
		*/
		virtual void onDraw( const Rect& clipping_rect ) = 0;
	public:
		/**@brief Funkcja u¿ywana do zakomunikowania dzieciom jakiejœ kontrolki, ¿e mysz opuœci³a
		obszar rodzica.

		Funkcja jest wywo³ywana w trzech sytuacjach:
		- Rodzic kontrolki przy poprzednim wywo³aniu test_mouse mia³ nad sob¹ kursor myszy, a w kolejnym
		go utraci³. Wtedy musi on wywo³aæ mouse_out dla wszystkich swoich dzieci, które tak¿e mia³y
		nad sob¹ kursor, sprawdzaj¹c wartoœæ zmiennej mouse_on.
		- Rodzic kontrolki otrzyma³ komunikat mouse_out, wiêc przekazuje je wszystkim potomkom, którzy
		maj¹ ustawion¹ zmienn¹ mouse_on na true.
		- Rodzic odpytuj¹c swoje dzieci znalaz³ ju¿ kontrolkê w której obszarze znajduje siê mysz.
		Musi teraz poinformowaæ rodzeñstwo tamtej kontrolki, ¿e utraci³y mysz je¿eli j¹ mia³y.

		Mysz mo¿e siê znajdowaæ jednoczeœnie w obszarze tylko jednej kontrolki spoœród dzieci.
		Jednak poniewa¿ kotrolki mog¹ siê przys³aniaæ, to funkcja test_mouse mo¿e wielokrotnie
		zwróciæ wartoœæ true. Z tego wzglêdu bierze siê pod uwagê zawsze pierwsz¹ kontrolkê na liœcie,
		która zwróci tê wartoœæ.

		@param point Wspó³rzêdne myszy.
		*/
		void mouse_out( const DirectX::XMFLOAT2& point );
		void draw_clipped( const Rect& clipping_rect );

		/**@brief Funkcja wywo³ywana w momencie, gdy u¿ytkownik chce przenieœæ zawartoœæ kontrolki
		do schowka windows. Typ danych zale¿y od kontrolki i ustala go implementacja klas pochodnych.

		@note Funkcja musi zostaæ zaimplementowana, nawet je¿eli kontrolka nic nie zamierza przekazywaæ.
		Domyœlna implementacja zwraca nullptr, co oznacza, ¿e kotrolka nie chowa niczego do schowka
		i nale¿y j¹ zachowaæ, je¿eli nie zamierza siê korzystaæ z dobrodziejstw schowka. Zwracam
		jednak uwagê, jakie wkurzaj¹ce jest dla u¿ytkownika kiedy nie mo¿e zrobiæ kopiuj-wklej.

		@return data Dane, które kontrolka przekazuje do schowka. Nale¿y zwróciæ nullptr, je¿eli
		nie chce siê przekazaæ ¿adnych danych.*/
		virtual ClipboardData* onGetClipboardData() { return nullptr; }

		/**@brief Funkcja wywo³ywana w momencie, gdy u¿ytkownik chce przenieœæ zawartoœæ schowka windows
		do kontrolki.

		@note Funkcja musi zostaæ zaimplementowana, nawet je¿eli kontrolka nic nie zamierza przekazywaæ.
		Domyœlna implementacja zwraca wartoœæ false i nale¿y j¹ zachowaæ, je¿eli nie zamierza siê 
		korzystaæ z dobrodziejstw schowka. Zwracam jednak uwagê, jakie wkurzaj¹ce jest dla u¿ytkownika
		kiedy nie mo¿e zrobiæ kopiuj-wklej.

		@param[in] data Dane, które kontrolka powinna przyj¹æ.
		@return Informacja o tym czy dane zosta³y u¿yte (true) czy zignorowane (false).
		*/
		virtual bool onSetClipboardData( ClipboardData* data ) { return false; };
	};


	/**@brief Korzeñ drzewa kontrolek i jednoczeœnie klasa zarz¹dzaj¹ca ca³ym GUI.
	*/
	class Root	:	public Control
	{
	private:
		Control*		focus;				///<Wskazuje na kontrolkê, która posiada aktualnie focusa lub nullptr

	protected:

	public:
		void set_focus( Control* control );		///<Ustawia focus dla podanej w parametrze kontrolki.

	};

}