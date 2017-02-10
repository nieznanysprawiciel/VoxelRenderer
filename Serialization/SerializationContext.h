#pragma once
/**@file SerializationContext
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Interface kontekstu serializacji i deserializacji.*/




/**@brief Interface kontekstu serializacji i deserializacji.
@ingroup Serialization

W klasach pochodnych nale¿y umieœciæ dowolne dane, które s¹ potrzebne w trakcie serializacji.*/
struct SerializationContext
{
public:

				SerializationContext() = default;	///
	virtual		~SerializationContext() = default;	///

};


