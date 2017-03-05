#pragma once
/**
@file DynamicActor.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "StaticActor.h"


class IController;

/**@brief Klasa bazowa dla obiektów dynamicznych.

Klasa zawiera nastêpuj¹ce komponenty obs³uguj¹ce:
- ruch postêpowy i obrotowy
- fizykê
- kontrolery
- podobiekty (@todo napisaæ obs³ugê podobiektów)

Je¿eli jest zdefiniowana sta³a _QUATERNION_SPEED, to prêdkoœci k¹towe s¹ wyra¿one
kwaternionem w przeciwnym razie jest to wektor, w którym sk³adowa w jest k¹tem obrotu.
W docelowej wersji bêdzie najprawdopodobniej wybrana opcja z wetorem a nie kwaternionem.

@note Niezaleznie od tego jak jest wyra¿ona prêdkoœæ, orientacja zawsze jest kwaternionem.
@todo Zastanowiæ siê czy controller nie powinien byæ unique_ptrem.

@ingroup Actors*/
class DynamicActor : public StaticActor
{
	RTTR_REGISTRATION_FRIEND
	RTTR_ENABLE( StaticActor )
protected:

	DirectX::XMFLOAT3		m_speed;				///< Prêdkoœæ postepowa obiektu.
	DirectX::XMFLOAT4		m_rotationSpeed;		///< Prêdkoœæ k¹towa obiektu (wyra¿ona wektorem i k¹tem obrotu w sk³adowej w).
	IController*			m_controller;			///< WskaŸnik na kontroler, poruszaj¹cy obiektem.

	float					m_mass;

public:

	explicit			DynamicActor			();	///< Kontruktor ustawi¹j¹cy zerow¹ prêdkoœæ k¹tow¹ i postêpow¹.
	explicit			DynamicActor			( const DirectX::XMFLOAT3& move_speed, const DirectX::XMFLOAT4& rot_speed );	///< Kontruktor ustawia podan¹ w parametrach prêdkoœæ.
	virtual				~DynamicActor();

	void				SetSpeed				( const DirectX::XMVECTOR& vector );		///<Ustawia prêdkoœæ obiektu @param[in] vector Wektor prêdkoœci.
	void				SetRotationSpeed		( const DirectX::XMVECTOR& quaternion );	///<Ustawia prêdkoœæ obrotow¹ @param[in] quaternion Wektor prêdkoœci.
	void				SetRotationSpeed		( const DirectX::XMFLOAT4 axis_angle );		///<Ustawia prêdkoœæ obrotow¹ @param[in] quaternion Wektor prêdkoœci.
	DirectX::XMVECTOR	GetSpeed				() const;									///< Zwraca prêdkoœæ postêpow¹ obiektu.
	DirectX::XMVECTOR	GetRotationSpeed		() const;									///< Zwraca prêdkoœæ obrotow¹ obiektu.

	void				SetController			( IController* ctrl );						///< Ustawia podany w parametrze kontroler
	IController*		GetController			();

	void				Move					( float time_interval );
	virtual void		MoveComplex				( float time_interval, const DirectX::XMFLOAT3& parent_speed, const DirectX::XMFLOAT4& parent_rotation );

	static ActorBase*	Create()	{ return new DynamicActor; }
};



//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
inline void			DynamicActor::SetSpeed			( const DirectX::XMVECTOR& vector )
{ 
	XMStoreFloat3( &m_speed, vector );
}

// ================================ //
//
inline void			DynamicActor::SetRotationSpeed	( const DirectX::XMVECTOR& quaternion )
{ 
	XMStoreFloat4( &m_rotationSpeed, quaternion );
}

// ================================ //
//
inline void			DynamicActor::SetRotationSpeed	( const DirectX::XMFLOAT4 axisAngle )
{ 
	m_rotationSpeed = axisAngle;
}

// ================================ //
//
inline DirectX::XMVECTOR	DynamicActor::GetSpeed	() const 
{ 
	return XMLoadFloat3( &m_speed ); 
}

// ================================ //
//
inline DirectX::XMVECTOR	DynamicActor::GetRotationSpeed	() const
{
	return XMLoadFloat4( &m_rotationSpeed );
}



