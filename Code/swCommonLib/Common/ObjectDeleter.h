#pragma once
/**@file ObjectDeleter.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Klasy s³u¿¹ce do zapewnienia ograniczonej mo¿liwoœci kasowania niektórych obiektów silnika,
które maj¹ prywatne destruktory.
*/

template <class TYPE> class ResourceContainer;




/**
@ingroup Helpers

@brief You can specify classes that can delete objects with private constructors.

Niektóre obiekty s¹ zadeklarowane z prywatnym (chronionym) destruktorem, w celu uniemo¿liwienia
skasowania ich przez niepowo³ane klasy. Jednak¿e musi istnieæ jakiœ dostêp do destruktorów,
aby nie powodowaæ wycieków pamiêci. Do kasowania obiektów s³u¿y klasa ObjectDeleter. Aby stworzyæ
obiekt tej klasy potrzebny jest "klucz", którym jest w³asnie ta klasa.

Klucz ma prywatne konstruktory, w zwi¹zku z czym mo¿e zostaæ stworzony jedynie przez klasy,
które s¹ z nim zaprzyjaŸnione. Domyœlnie klasami zaprzyjaŸnionymi s¹ ResourceManager i ResourceContainer,
aby stworzyæ klucz dla innych typów klas, nale¿y napisaæ specjalizacjê tego szablonu.

Odddelegowanie innej klasy do kasowania obiektów ma za zadanie zlikwidowanie niewygodnych zale¿noœci.
Dziêki temu obiekty, które maj¹ byæ kasowane, nie musz¹ siê przyjaŸniæ bezpoœrednio z klasami, które mog¹ je usuwaæ.
Wystarczy, ¿e zadeklaruj¹ przyjaŸñ z obiektem ObjectDeleter. Taka przyjaŸñ jest niegroŸna, poniewa¿
klasa ta nie s³u¿y do niczego innego ni¿ kasowanie obiektów, wiêc jesteœmy pewni, ¿e zmiany wprowadzone
w klasie deklaruj¹cej przyjaŸñ nie bêd¹ psu³y ca³ego kodu aplikacji.
*/
template<typename class_type> class ObjectDeleterKey
{
	friend class ResourceManager;
	friend class AssetsManager;
	friend class ResourceContainer<class_type>;
	friend class RenderTargetObject;
private:
	ObjectDeleterKey() = default;						///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	ObjectDeleterKey( const ObjectDeleterKey& ) {}		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	ObjectDeleterKey( const ObjectDeleterKey&& ) {}		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
public:
};

/**@brief Klasa s³u¿y do kasowania obiektów, których destruktor jest prywatny.
@ingroup DeletingObjects

Taka sytuacja zachodzi w momencie, gdy nie chcemy, aby ka¿da klasa mog³a skasowaæ
jakiœ obiekt, ale chcemy daæ takie uprawnienia jedynie wybrañcom. Ta klasa implementuje
funkcjonalnoœæ kasowania obieków. Obiekt tej klasy mo¿e zostaæ stworzony jedynie
przez funkcjê, która dostarczy odpowiedni obiekt klucz @ref ObjectDeleterKey.
Dopiero wtedy mo¿na wywo³aæ funkcjê delete_object, która usunie ¿¹dany obiekt z pamiêci.

@see @ref ObjectDeleterKey
*/
template<typename class_type> class ObjectDeleter
{
private:
	ObjectDeleter() = default;							///<Nie mo¿na stworzyæ obiektu nie maj¹c klucza.
	ObjectDeleter( const ObjectDeleter& ) = default;	///<Nie mo¿na stworzyæ obiektu nie maj¹c klucza.
public:
	ObjectDeleter( const ObjectDeleterKey<class_type>& deleter_key ) {};	///<Jedyny dostêpny do wywo³ania konstruktor, tylko dla wybrañców.

	/**@brief Kasuje podany w parametrze obiekt.
	
	@param[in] object Obiekt do skasowania.*/
	inline void delete_object( class_type* object )
	{
		delete object;
	}

	/**@brief Kasuje podany w parametrze obiekt.

	@param[in] object Obiekt do skasowania.
	@param[in] deleter_key Klucz dostêpu do obiektu.*/
	static inline void delete_object( class_type* object, const ObjectDeleterKey<class_type>& deleter_key )
	{
		delete object;
	}
};
