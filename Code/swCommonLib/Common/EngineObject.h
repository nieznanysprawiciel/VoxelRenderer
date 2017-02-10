#pragma once
/**@file ActorBase.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief */

#include "swCommonLib/Common/RTTR.h"
#include "swCommonLib/Common/TypesDefinitions.h"

class ISerializer;
class IDeserializer;


/**@brief Klasa bazowa dla obiektów, które maj¹ byæ dostêpne w edytorze
poprzez rttr. Nale¿y po niej odziedziczyæ i dodaæ odpowiednie deklaracje.

@todo Dobrze by by³o, gdyby EngineObject implementowa³ domyœln¹ serializacjê, ale wtedy jest problem,
¿e nie wiadomo, która z bibliotek powinna tê implementacjê kompilowaæ.

@ingroup Reflection*/
class EngineObject
{
	RTTR_ENABLE()

public:
	inline TypeID		GetType			() const		{ return this->get_type(); }
	inline std::string	GetTypeName		() const		{ return std::string( this->get_type().get_name() ); }


	/**@brief Implements default serialization.
	@detail Serializes only declared properties.*/
	virtual void		Serialize		( ISerializer* ser ) const;

	/**@brief Implements default deserialization.*/
	virtual void		Deserialize		( IDeserializer* deser );

	/// Zwraca iloœæ pamiêci zajmowanej przez obiekt. Implementacje w klasach potomnych powinny te¿
	/// doliczaæ rekurencyjnie pamiêæ zajmowan¹ przez obiekty, których w³aœcicielem jest ten obiekt.
	virtual Size		MemorySize		()							{ return this->get_type().get_sizeof(); }

	template< typename TargetType >
	TargetType*			DynamicCast		()							{ return rttr::rttr_cast< TargetType* >( this ); }
};

