
#include "CameraController.h"
#include "DynamicActor.h"


#include <iostream>


RTTR_REGISTRATION
{
	rttr::registration::class_< SpectatorCameraController >( "SpectatorCameraController" );
}


using namespace DirectX;
using namespace sw::input;



// ================================ //
//
SpectatorCameraController::SpectatorCameraController( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
	:	m_mouse( mouse )
	,	m_keyboard( keyboard )
	,	m_verticalAngle( 0.0f )
	,	m_horizontalAngle( 0.0f )
{
	InitDefaultSpeed();
}

// ================================ //
//
SpectatorCameraController::SpectatorCameraController( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard, float horAngle, float vertAngle )
	:	m_mouse( mouse )
	,	m_keyboard( keyboard )
	,	m_verticalAngle( vertAngle )
	,	m_horizontalAngle( horAngle )
{
	InitDefaultSpeed();
}

/**@brief */
SpectatorCameraController::~SpectatorCameraController()
{}


// ================================ //
//
void		SpectatorCameraController::InitDefaultSpeed()
{
	m_moveSpeed = 1.0f;
	m_buttonRotSpeed = 0.1f;
	m_axisRotSpeed = 0.1f;
	m_zoomSpeed = 0.1f;
}


/*Funkcja g³ówna odpowiedzialna za sterowanie ruchem obiektu.*/
void		SpectatorCameraController::ControlObjectPre		( DynamicActor* actor, IControllersState* globalState, TimeType time, TimeDiff elapsed )
{
	float elpasedTime = (float)elapsed;

	// Turn left and right
	if( m_keyboard[ Keyboard::PhysicalKeys::KEY_Q ].IsPressed() &&
		!m_keyboard[ Keyboard::PhysicalKeys::KEY_E ].IsPressed() )
	{
		m_horizontalAngle += m_buttonRotSpeed * elpasedTime;
	}
	else if( m_keyboard[ Keyboard::PhysicalKeys::KEY_E ].IsPressed() &&
		!m_keyboard[ Keyboard::PhysicalKeys::KEY_Q ].IsPressed() )
	{
		m_horizontalAngle += -m_buttonRotSpeed * elpasedTime;
	}



	XMVECTOR forward = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR left = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );
	XMVECTOR up = XMVectorSet( 0.0, 0.0, 0.0, 0.0 );

	// Forward and backward movement along seeing direction.
	if( m_keyboard[ Keyboard::PhysicalKeys::KEY_W ] && !m_keyboard[ Keyboard::PhysicalKeys::KEY_S ] )
	{
		forward = ForwardVector( actor );
		forward *= m_moveSpeed * elpasedTime;
	}
	else if( m_keyboard[ Keyboard::PhysicalKeys::KEY_S ] && !m_keyboard[ Keyboard::PhysicalKeys::KEY_W ] )
	{
		forward = BackwardVector( actor );
		forward *= m_moveSpeed * elpasedTime;
	}

	if( m_mouse.GetAxesState()[ Mouse::PhysicalAxes::WHEEL ] != 0.0f )
	{
		forward += ForwardVector( actor ) * m_zoomSpeed * elpasedTime * m_mouse.GetAxesState()[ Mouse::PhysicalAxes::WHEEL ];
	}

	// Left and right movement
	if( m_keyboard[ Keyboard::PhysicalKeys::KEY_A ] && !m_keyboard[ Keyboard::PhysicalKeys::KEY_D ] )
	{
		left = LeftVector( actor );
		left *= m_moveSpeed * elpasedTime;
	}
	else if( m_keyboard[ Keyboard::PhysicalKeys::KEY_D ] && !m_keyboard[ Keyboard::PhysicalKeys::KEY_A ] )
	{
		left = RightVector( actor );
		left *= m_moveSpeed * elpasedTime;
	}

	// Up and Down movement
	if( m_keyboard[ Keyboard::PhysicalKeys::KEY_R ] && !m_keyboard[ Keyboard::PhysicalKeys::KEY_F ] )
		up = XMVectorSet( 0.0, 1.0, 0.0, 0.0 ) * m_moveSpeed * elpasedTime;
	else if( m_keyboard[ Keyboard::PhysicalKeys::KEY_F ] && !m_keyboard[ Keyboard::PhysicalKeys::KEY_R ] )
		up = XMVectorSet( 0.0, -1.0, 0.0, 0.0 ) * m_moveSpeed * elpasedTime;


	if( m_mouse.RightButton()->IsPressed() )
	{
		float yAxis = m_mouse.GetAxesState()[ Mouse::PhysicalAxes::Y_AXIS ];
		if( yAxis != 0.0f )
			m_verticalAngle += -yAxis * m_axisRotSpeed * elpasedTime;

		float xAxis = m_mouse.GetAxesState()[ Mouse::PhysicalAxes::X_AXIS ];
		if( xAxis != 0.0f )
			m_horizontalAngle += -xAxis * m_axisRotSpeed * elpasedTime;
	}

	XMVECTOR verticalRotationQuat = XMQuaternionRotationNormal( XMVectorSet( 1.0, 0.0, 0.0, 0.0 ), m_verticalAngle );
	XMVECTOR horizontalRotationQuat = XMQuaternionRotationNormal( XMVectorSet( 0.0, 1.0, 0.0, 0.0 ), m_horizontalAngle );

	actor->TeleportOrientation( XMQuaternionMultiply( verticalRotationQuat, horizontalRotationQuat ) );

	// Teleport after rotations. RightVector uses actor orientation.
	XMVECTOR translation = forward + left + up;
	actor->Teleport( actor->GetPosition() + translation );

	if( XMVector3Equal( translation, XMVectorSet( 0.0, 0.0, 0.0, 0.0 ) ) )
	{
		XMFLOAT3 pos;
		XMStoreFloat3( &pos, actor->GetPosition() );
		std::cout << "\rCamera position: ( " << pos.x << ", " << pos.y << ", " << pos.z << " )";
	}
}

/**@brief Funkcja nic nie robi.

W trybie debug funkcja zatrzymuje siê na assercie.*/
void SpectatorCameraController::ControlObjectPost( DynamicActor* actor, IControllersState* globalState, TimeType time, TimeDiff elapsed )
{
	//assert( !"This is only pre controlled class." );
}

// ================================ //
//
void	SpectatorCameraController::Initialize		( DynamicActor* actor )
{
	XMVECTOR reference = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
	XMMATRIX rotationMat = XMMatrixRotationQuaternion( actor->GetOrientation() );

	XMVECTOR direction = XMVector3Transform( reference, rotationMat );

	XMVECTOR xzDirection = XMVector3Normalize( XMVectorSetY( direction, 0.0f ) );
	m_horizontalAngle = XMVectorGetX( XMVector3AngleBetweenNormals( xzDirection, reference ) );
	m_verticalAngle = XMVectorGetX( XMVector3AngleBetweenNormals( xzDirection, direction ) );
}

//====================================================================================//
//			Helpers	
//====================================================================================//

// ================================ //
//
DirectX::XMVECTOR			SpectatorCameraController::ForwardVector	( DynamicActor* actor )
{
	XMVECTOR versor = XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
	XMVECTOR orientation = actor->GetOrientation();
	return XMVector3Rotate( versor, orientation );
}

// ================================ //
//
DirectX::XMVECTOR			SpectatorCameraController::BackwardVector	( DynamicActor* actor )
{
	return XMVectorNegate( ForwardVector( actor ) );
}

// ================================ //
//
DirectX::XMVECTOR			SpectatorCameraController::LeftVector		( DynamicActor* actor )
{
	return XMVectorNegate( RightVector( actor ) );
}

// ================================ //
//
DirectX::XMVECTOR			SpectatorCameraController::RightVector		( DynamicActor* actor )
{
	XMVECTOR versor = XMVectorSet( 1.0, 0.0, 0.0, 0.0 );
	XMVECTOR orientation = actor->GetOrientation();
	return XMVector3Rotate( versor, orientation );
}

