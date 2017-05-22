#pragma once

/**@file CameraController.h
*/

#include "IController.h"

#include "swInputLibrary/InputCore/KeyboardState.h"
#include "swInputLibrary/InputCore/MouseState.h"


#include <DirectXMath.h>


/**@brief */
class SpectatorCameraController	:	public IController
{
	RTTR_ENABLE( IController )
private:

	const sw::input::MouseState&		m_mouse;
	const sw::input::KeyboardState&		m_keyboard;

	float		m_verticalAngle;
	float		m_horizontalAngle;


	float		m_moveSpeed;			///<Prêdkoœæ poruszania posuwistego
	float		m_buttonRotSpeed;		///<Prêdkoœæ ruchu obrotowego (dla przycisku)
	float		m_axisRotSpeed;			///<Prêdkoœæ ruchu obrotowego (dla myszy/jousticka)
	float		m_zoomSpeed;			///<Prêdkoœæ zbli¿ania i oddalania (kó³kiem myszy)

public:
	explicit SpectatorCameraController			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard );
	explicit SpectatorCameraController			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard, float horAngle, float vertAngle );
	virtual ~SpectatorCameraController			();

	virtual void		ControlObjectPre		( DynamicActor* actor, IControllersState* globalState );
	virtual void		ControlObjectPost		( DynamicActor* actor, IControllersState* globalState );

	virtual void		Initialize				( DynamicActor* actor ) override;

public:
	
	DirectX::XMVECTOR	ForwardVector			( DynamicActor* actor );
	DirectX::XMVECTOR	BackwardVector			( DynamicActor* actor );
	DirectX::XMVECTOR	LeftVector				( DynamicActor* actor );
	DirectX::XMVECTOR	RightVector				( DynamicActor* actor );
};

DEFINE_OPTR_TYPE( SpectatorCameraController );

