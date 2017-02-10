#pragma once

#include "swCommonLib/Common/RTTR.h"
#include "swCommonLib/Serialization/SerializationContext.h"

#include <map>
#include <vector>



struct EngineSerializationContext : public SerializationContext
{
	typedef std::map< rttr::type, std::vector< rttr::property > > TypePropertyMap;
public:

	bool					SaveWholeMap;		///< Wartoœæ true oznacza, ¿e zapisujemy mapê, false, ¿e zapisujemy stan gry.
	bool					EditorLoad;			///< Wczytywanie mapy w trybie edytora.

	TypePropertyMap			TypeProperties;		///< Mapa zawieraj¹ca typy i ich w³aœciwoœci. W³aœciwoœci nieserializowane s¹ odfiltrowane.



public:
	virtual		~EngineSerializationContext() = default;
};

