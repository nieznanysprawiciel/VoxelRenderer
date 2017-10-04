#pragma once
/**
@file CameraActor.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/RTTR.h"
#include "swCommonLib/Common/EngineObject.h"

#include "DynamicActor.h"

#include <DirectXMath.h>


/**@brief Struktura do wymieniania informacji o danych kamery.

Generalnie aktorzy nie powinni siê wzajemnie odpytywaæ o swoje dane.
Jest zagro¿enie, ¿e z powodu wykonania wielow¹tkowego jakiœ aktor odczyta niespójne dane.
Celem powstania tej struktury jest przechowywanie stanu kamery zarejestrowanego przed
wywo³aniem kontrolerów.

@todo Prawdopodobnie jakiœ mechanizm odpytywania siê wzajemnie aktorów o swoje dane bêdzie potrzebny.
W momencie jak siê on pojawi trzeba siê pozbyæ tej struktury i zrobiæ to nowym mechanizmem.

@ingroup Actors*/
struct CameraData
{
	DirectX::XMFLOAT3	Position;
	DirectX::XMFLOAT3	Direction;
	DirectX::XMFLOAT3	UpVector;
	DirectX::XMFLOAT3	RightVector;
	DirectX::XMFLOAT4	OrientationQuat;
	float				Fov;
	float				Width;
	float				Height;
	float				ViewportSize;
	float				NearPlane;
	float				FarPlane;
	bool				IsPerspective;

	DirectX::XMVECTOR	GetPosition()		{	return DirectX::XMLoadFloat3( &Position );		}
	DirectX::XMVECTOR	GetDirection()		{	return DirectX::XMLoadFloat3( &Direction );		}
	DirectX::XMVECTOR	GetUpVector()		{	return DirectX::XMLoadFloat3( &UpVector );		}
	DirectX::XMVECTOR	GetRightVector()	{	return DirectX::XMLoadFloat3( &RightVector );	}
	DirectX::XMVECTOR	GetOrientation()	{	return DirectX::XMLoadFloat4( &OrientationQuat );	}
};


/**@brief Klasa bazowa dla wszystkich obiektów kamer w silniku.
*/
class CameraActor : public DynamicActor
{
	friend class DisplayEngine;

	RTTR_ENABLE( DynamicActor )
	RTTR_REGISTRATION_FRIEND
protected:

	float					m_fov;
	float					m_width;
	float					m_height;
	float					m_viewportSize;			///< Size of viewport in camera space. Width and height give us only aspect. We multiply it by this size.
	float					m_nearPlane;
	float					m_farPlane;
	bool					m_isPerspective;

	DirectX::XMFLOAT4X4		m_projectionMatrix;		///<Macierz projekcji. Dla ka¿dej kamery mo¿e byæ inna. @attention Na razie nieu¿ywane. Macierz projekcji jest ustawiana na sta³e w DisplayEngine.

public:
	explicit CameraActor();

	void				SetPerspectiveProjectionMatrix		( float angle, float width, float height, float nearPlane, float farPlane );
	void				SetOrthogonalProjectionMatrix		( float width, float height, float viewportSize, float nearPlane, float farPlane );

	void				SetPerspective		( bool value );
	void				SetWidth			( float width );
	void				SetHeight			( float height );
	void				SetViewportSize		( float size );
	void				SetNearPlane		( float plane );
	void				SetFarPlane			( float plane );
	void				SetFov				( float fov );

	bool				GetIsPerspective	() const					{ return m_isPerspective; }
	float				GetWidth			() const					{ return m_width; }
	float				GetHeight			() const					{ return m_height; }
	float				GetViewportSize		() const					{ return m_viewportSize;  }
	float				GetNearPlane		() const					{ return m_nearPlane; }
	float				GetFarPlane			() const					{ return m_farPlane; }
	float				GetFov				() const					{ return m_fov; }

	DirectX::XMFLOAT4X4		GetProjection	() const					{ return m_projectionMatrix; }
	DirectX::XMFLOAT4X4		GetView			() const;

	CameraData			GetCameraData		() const;

	static ActorBase*	Create				()					{ return new CameraActor; }

private:
	void				UpdateMatrix		();
};

