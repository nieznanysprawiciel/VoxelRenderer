#pragma once
/**
@file ActorBase.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/EngineObject.h"


/**@defgroup Actors Actors
@ingroup EngineCore
@brief Information about engine actors.*/


class EngineInterface;
class Engine;
class ControllersEngine;
class IController;
class Event;
class InputAbstractionLayerBase;
class IRenderer;
class Model3DFromFile;



/**@brief Base class for all engine actors.

@ingroup Actors*/
class ActorBase	:	public EngineObject
{
	RTTR_ENABLE( EngineObject )
private:
	static Engine*	engine;		///< Wskaünik na g≥Ûwny obiekt silnika.
protected:

	void event( Event* );

public:
	virtual ~ActorBase() = default;
	virtual void		Init(){};

	/**@brief Zwraca wskaünik na interfejs silnika, ktÛry nadaje siÍ do uøywania przez
	programistÍ gry.
	@attention Nie wolno rzutowaÊ obiektu na Engine.
	*/
	static EngineInterface*			GetEngineInterface()	{ return reinterpret_cast< EngineInterface* >( engine ); }

	/**@brief Funkcja ustawia wskaünik na g≥Ûwny obiekt silnika.
	@attention Wolno uøywaÊ tylko klasie Engine w konstruktorze.
	@param[in] engine_ptr Wskaünik na g≥Ûwny obiekt silnika.
	*/
	static void			SetEngine		( Engine* enginePtr )	{ if( !engine ) engine = enginePtr; }

	static ActorBase*	Create			()						{ return new ActorBase; }

	virtual void		Serialize		( ISerializer* ser ) const override;
	virtual void		Deserialize		( IDeserializer* deser ) override;
};
