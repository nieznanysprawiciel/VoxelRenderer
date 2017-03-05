/**
@file ActorBase.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "ActorBase.h"

#include "swCommonLib/Serialization/PropertySerialization/Serialization.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< ActorBase >( "ActorBase" );
}


Engine* ActorBase::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe³niana wskaŸnikiem this



/**@deprecated Eventy wysyla sie przez API w EngineInterface. FableEngine nie odpowiada ju¿ za
przetwarzanie eventow.

Funkcja pozwala wys³aæ event, który bêdzie potem przetworzony przez klase FableEngine.
Eventy s¹ metod¹ komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
a modu³em silnika odpowiedzialnym za fabu³ê. Istnieje szereg eventów wbudowanych, wysy³anych przez silnik,
mo¿na równie¿ definiowaæ w³asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo¿e byæ wys³any przez dowolny
objekt poprzez wywo³anie funkcji ActorBase::event. Aby wys³aæ w³asny event trzeba przeci¹¿yæ jedn¹ z funkcji klas wbudowanych,
która jest potem wywo³ywana przez silnik i wywo³aæ tê funkjê.

Za zwolnienie pamiêci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo³aniu funkcji obs³ugi,
u¿ytkownik nie musi siê tym przejmowac).*/
void inline ActorBase::event(Event* new_event)
{
	/*engine->SendEvent(new_event);*/
}



/**@brief Domyœlna implementacja serializacji.

Serializuje wszystkie propertiesy oznaczone w metadanych flag¹ Serialize lub AllowInSaveFile.
Serialize oznacza w³aœciwoœci, które zostan¹ zapisane do pliku z map¹. AllowInSaveFile to w³aœciwoœci,
które zostan¹ zapisane w przypadku zapisywania stanu gry.*/
void ActorBase::Serialize( ISerializer* ser ) const
{
	Serialization::DefaultSerialize( ser, this );
}

/**@brief Domyœlna implementacja deserializacji.*/
void ActorBase::Deserialize( IDeserializer* deser )
{
	Serialization::DefaultDeserialize( deser, this );
//	GetEngineInterface()->Level.Serialization.DefaultDeserialize( deser, this );
}

