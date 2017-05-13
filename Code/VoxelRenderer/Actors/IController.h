#pragma once
/**@file IController.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Klasa bazowa dla kontrolerów.*/

#include "swCommonLib/Common/EngineObject.h"




class DynamicActor;
class IControllersState;

/**@brief Klasa bazowa dla wszystkich kontrolerów dla obiektów.*/
class IController : public EngineObject
{
	RTTR_ENABLE( EngineObject )
public:
	virtual ~IController() = default;

	virtual void		ControlObjectPre		( DynamicActor* actor, IControllersState* globalState ) = 0;	///< Funkcja wywo³ywana przed 
	virtual void		ControlObjectPost		( DynamicActor* actor, IControllersState* globalState ) = 0;

	virtual void		Initialize				( DynamicActor* actor ) = 0;
};


/**
@page Controllers Kontrolery

@section Contents Spis Treœci
- @ref mainUse
- @ref PrePostControllers

@subsection mainUse Do czego s³u¿¹ kotrolery

Kontrolery pozwalaj¹ na sterowanie ruchem Aktorów. Ka¿dy aktor dziedzicz¹cy po DynamicActor
mo¿e mieæ przypisany kontroler przy pomocy funkcji DynamicActor::SetController.
Interfejs IController posiada dwie funkcje @ref IController::ControlObjectPre i @ref IController::ControlObjectPost,
które nale¿y zaimplementowaæ. Po dodaniu Aktora do ControllersEngine funkcje te s¹ wywo³ywane w ka¿dej klatce.

Kontrolery mog¹ one dzia³aæ jak sztuczna inteligencja, wtedy trzeba odziedziczyæ po klasie @ref BaseAIController,
albo odbieraæ wejœcie od u¿ytkownika, wtedy trzeba odziedziczyæ po @ref BaseInputController.

@subsection PrePostControllers Kotrolery pre i post
Istniej¹ kotrolery, których ruch uzale¿niony jest od po³o¿enia innych obiektów. Z racji tego nie mog¹ one zostaæ wywo³ane przed policzeniem
przemieszczeñ, bo by³yby zawsze opóŸnione o jedn¹ klatkê. Pozosta³e kotrolery s³u¿¹ do wp³ywania na ruch obiektów, dlatego nie mog¹ byæ
wywo³ane, po obliczeniu po³o¿eñ obiektów. Z tego wzglêdu kotrolery obs³ugiwane s¹ dwukrotnie, jedne w funkcji ControllersEngine::ProceedControllersPre
tu¿ przed wywo³aniem MovementEngine::ProceedMovement, a drugie tu¿ po tym wywo³aniu w funkcji ControllersEngine::ProceedControllersPost.

Do kontrolerów post zaliczamy kotrolery œledz¹ce jakis obiekt, do kotrolerów pre wszystkie kontrolery odpowiedzialne za AI oraz
steruj¹ce ruchem na podstawie klawiatury czy myszy.

*/
