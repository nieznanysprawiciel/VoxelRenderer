/**
@file CameraActor.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "CameraActor.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< CameraActor >( "CameraActor" )
		.property( "IsPerspective", &CameraActor::GetIsPerspective, &CameraActor::SetPerspective )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "Width", &CameraActor::GetWidth, &CameraActor::SetWidth )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "Height", &CameraActor::GetHeight, &CameraActor::SetHeight )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "ViewportSize", &CameraActor::GetViewportSize, &CameraActor::SetViewportSize )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "NearPlane", &CameraActor::GetNearPlane, &CameraActor::SetNearPlane )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "FarPlane", &CameraActor::GetFarPlane, &CameraActor::SetFarPlane )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "FOV", &CameraActor::GetFov, &CameraActor::SetFov )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		);
}


using namespace DirectX;



//====================================================================================//
//			Helpers	
//====================================================================================//

/**@brief Funkcja odwraca wektor pozycji, aby by³ zwrócony w przeciwnym kierunku.
Potrzebne w momencie tworzenia macierzy widoku na podstawie po³o¿enia kamery.

@param[inout] result_vector Wektor pozycji. Nale¿y podaæ tutaj wektor do odwrócenia. Wynik zostanie zwrócony w tej zmiennej.
*/
inline void inverse_camera_position( DirectX::XMVECTOR& result_vector )
{
	result_vector = DirectX::XMVectorNegate( result_vector );
	//result_vector = XMVectorSetW( result_vector, 0.0 );		// sk³adowa W powinna byæ 0, ale funkcja XMMatrixTranslationFromVector na ni¹ wogóle nie patrzy
}

/**@brief Funkcja odwraca kwaternion orientacji, aby by³ zwrócony w przeciwnym kierunku.
Potrzebne w momencie tworzenia macierzy widoku na podstawie po³o¿enia kamery.

@param[inout] result_vector Kwaternion orientacji. Nale¿y podaæ tutaj wektor do odwrócenia. Wynik zostanie zwrócony w tej zmiennej.
*/
inline void inverse_camera_orientation( DirectX::XMVECTOR& result_vector )
{
	result_vector = DirectX::XMVectorNegate( result_vector );
	result_vector = DirectX::XMVectorSetW( result_vector, -DirectX::XMVectorGetW( result_vector ) );
}


//====================================================================================//
//			Camera implementation	
//====================================================================================//


/**@brief Tworzy domyœln¹ kamerê perspektywiczn¹ o parametrach:
m_width:		1920
m_height:		1080
m_nearPlane		1.0
m_farPlane		100000.0
m_fov			45.0*/
CameraActor::CameraActor()
{
	m_isPerspective = true;
	m_height = 1080;
	m_width = 1920;
	m_nearPlane = 1.0f;
	m_farPlane = 100000.0f;
	m_fov = 45.0f;
	m_viewportSize = 1.0f;

	SetPerspectiveProjectionMatrix( m_fov, m_width, m_height, m_nearPlane, m_farPlane );
}


/**@brief Tworzy macierz projekcji perspektywicznej dla danej kamery i umieszcza w polu m_projectionMatrix.

@param[in] angle K¹t widzenia w pionie wyra¿ony w stopniach
@param[in] aspect Stosunek Szerokoœci do wysokoœci ekranu
@param[in] nearPlane Bli¿sza p³aszczyzna obcinania
@param[in] farPlane Dalsza p³aszczyzna obcinania*/
void CameraActor::SetPerspectiveProjectionMatrix		( float angle, float width, float height, float nearPlane, float farPlane )
{
	m_isPerspective = true;
	m_height = height;
	m_width = width;
	m_fov = angle;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	float aspect = width / height;

	XMMATRIX projMatrix = XMMatrixPerspectiveFovRH( DirectX::XMConvertToRadians( m_fov ), aspect, nearPlane, farPlane );
	XMStoreFloat4x4( &m_projectionMatrix, projMatrix );
}

/**@brief Tworzy macierz projekcji ortogonalnej dla danej kamery i umieszcza w polu m_projectionMatrix.

@param[in] angle K¹t widzenia w pionie
@param[in] aspect Stosunek Szerokoœci do wysokoœci ekranu
@param[in] nearPlane Bli¿sza p³aszczyzna obcinania
@param[in] farPlane Dalsza p³aszczyzna obcinania*/
void CameraActor::SetOrthogonalProjectionMatrix			( float width, float height, float viewportSize, float nearPlane, float farPlane )
{
	m_isPerspective = false;
	m_height = height;
	m_width = width;
	m_viewportSize = viewportSize;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	float aspect = width / height;

	XMMATRIX projMatrix = XMMatrixOrthographicRH( aspect * m_viewportSize, m_viewportSize, nearPlane, farPlane );
	XMStoreFloat4x4( &m_projectionMatrix, projMatrix );
}

/**@brief Ustawia macierz perspektywiczn¹ lub ortogonaln¹.*/
void				CameraActor::SetPerspective		( bool value )
{
	m_isPerspective = value;
	UpdateMatrix();
}

/**@brief Ustawia szerokoœæ.*/
void				CameraActor::SetWidth			( float width )
{
	m_width = width;
	UpdateMatrix();
}

/**@brief Ustawia wysokoœæ.*/
void				CameraActor::SetHeight			( float height )
{
	m_height = height;
	UpdateMatrix();
}

/**@brief Sets viewport size.*/
void				CameraActor::SetViewportSize	( float size )
{
	m_viewportSize = size;
	UpdateMatrix();
}

/**@brief Ustawia bli¿sz¹ p³aszczyznê obcianania.*/
void				CameraActor::SetNearPlane		( float plane )
{
	m_nearPlane = plane;
	UpdateMatrix();
}

/**@brief Ustawia dalsz¹ p³aszczyznê obcinania.*/
void				CameraActor::SetFarPlane		( float plane )
{
	m_farPlane = plane;
	UpdateMatrix();
}

/**@brief Ustawia k¹t widzenia.*/
void				CameraActor::SetFov				( float fov )
{
	m_fov = fov;
	UpdateMatrix();
}

/**@brief */
DirectX::XMFLOAT4X4 CameraActor::GetView			() const
{
	XMMATRIX viewMatrix;
	XMVECTOR position = GetPosition();
	XMVECTOR orientation = GetOrientation();

	inverse_camera_position( position );
	inverse_camera_orientation( orientation );

	viewMatrix = XMMatrixTranslationFromVector( position );
	XMMATRIX rotation_matrix = XMMatrixRotationQuaternion( orientation );
	viewMatrix = viewMatrix * rotation_matrix;

	XMFLOAT4X4 viewMat;
	XMStoreFloat4x4( &viewMat, viewMatrix );

	return viewMat;
}

/**@copydoc CameraData*/
CameraData			CameraActor::GetCameraData() const
{
	CameraData data;
	data.IsPerspective = m_isPerspective;
	data.FarPlane = m_farPlane;
	data.Fov = m_fov;
	data.Height = m_height;
	data.NearPlane = m_nearPlane;
	data.Width = m_width;
	data.ViewportSize = m_viewportSize;
	XMStoreFloat4( &data.OrientationQuat, GetOrientation() );
	XMStoreFloat3( &data.Position, GetPosition() );

	XMMATRIX rotationMat = XMMatrixRotationQuaternion( GetOrientation() );
	XMVECTOR direction = XMVectorSet( 0.0f, 0.0f, -1.0f, 0.0f );
	XMVECTOR upVector = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR rightVector = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f );

	direction = XMVector3Transform( direction, rotationMat );
	upVector = XMVector3Transform( upVector, rotationMat );
	rightVector = XMVector3Transform( rightVector, rotationMat );

	XMStoreFloat3( &data.Direction, direction );
	XMStoreFloat3( &data.UpVector, upVector );
	XMStoreFloat3( &data.RightVector, rightVector );

	return data;
}

/**@brief Aktualizuje przechowywan¹ macierz na podstawie wartoœci pól klasy.*/
void				CameraActor::UpdateMatrix		()
{
	if( m_isPerspective )
		SetPerspectiveProjectionMatrix( m_fov, m_width, m_height, m_nearPlane, m_farPlane );
	else
		SetOrthogonalProjectionMatrix( m_width, m_height, m_viewportSize, m_nearPlane, m_farPlane );
}