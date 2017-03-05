/**
@file StaticActor.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "StaticActor.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< StaticActor >( "StaticActor" )
		.property( "Position", &StaticActor::position )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" ),
			rttr::policy::prop::bind_as_ptr
		)
		.property( "Orientation", &StaticActor::orientation )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" ),
			rttr::policy::prop::bind_as_ptr
		)
#ifdef _SCALEABLE_OBJECTS
		.property( "Scale", &StaticActor::scale )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" )
		)
#endif
/*		.property( "VisibleComponent", &StaticActor::m_visibleComponent )
		(
			rttr::metadata( MetaDataType::Category, "VisibleComponent" ),
			rttr::policy::prop::bind_as_ptr
		)*/;
}

using namespace DirectX;

// ================================ //
//
StaticActor::StaticActor()
{//inicjujemy obiekt w punkcie ( 0.0, 0.0, 0.0 ) i zorientowany tak jak jego mesh
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	position_back = position;

	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4(&orientation,quaternion);

	orientation_back = orientation;

	swap_data = false;

#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

// ================================ //
//
StaticActor::StaticActor( const XMFLOAT3& pos, const XMFLOAT4& orient )
{
	position = pos;
	position_back = pos;
	orientation = orient;
	orientation_back = orient;

	swap_data = false;

#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

// ================================ //
//
StaticActor::StaticActor	( BufferObject* vertexBuffer, BufferObject* indexBuffer )
{
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	position_back = position;

	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4(&orientation,quaternion);

	orientation_back = orientation;

	swap_data = false;

#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

// ================================ //
//
StaticActor::~StaticActor()
{}


/**@brief Zwraca interpolowan¹ pozycjê obiektu miêdzy przechowywanymi po³o¿eniami.

@param[in] time_lag Procent czasu jaki up³yn¹³ od ostaniej klatki do nastêpnej
Zakres [0,1].*/
XMVECTOR StaticActor::GetInterpolatedPosition( float time_lag ) const
{
	XMVECTOR pos2 = XMLoadFloat3( &position );
	XMVECTOR pos1 = XMLoadFloat3( &position_back );
	if ( XMVector3Equal( pos1, pos2 ) )
		return pos1;

	if ( swap_data )
		pos1 = XMVectorLerp( pos2, pos1, time_lag );
	else
		pos1 = XMVectorLerp( pos1, pos2, time_lag );

	pos1 = XMVectorSetW( pos1, 1 );
	return pos1;
}

/**@brief Zwraca interpolowan¹ orientacjê obiektu miêdzy przechowywanymi po³o¿eniami.

@param[in] time_lag Procent czasu jaki up³yn¹³ od ostaniej klatki do nastêpnej
Zakres [0,1].*/
XMVECTOR StaticActor::GetInterpolatedOrientation( float time_lag ) const
{
	XMVECTOR orient2 = XMLoadFloat4( &orientation );
	XMVECTOR orient1 = XMLoadFloat4( &orientation_back );
	if ( XMVector3Equal( orient1, orient2 ) )	// Wa¿ne! Oblcizenia na floatach s¹ niedok³adne i troszkê wszystko lata.
		return orient1;

	if ( swap_data )
		return XMQuaternionSlerp( orient2, orient1, time_lag );
	else
		return XMQuaternionSlerp( orient1, orient2, time_lag );
}


//====================================================================================//
//			Visible component	
//====================================================================================//
//
//bool		StaticActor::SetModel		( const ResourcePtr< MeshAsset >& model )
//{
//	if( model == nullptr )
//		return false;
//
//	m_visibleComponent.SetAsset( model );
//	return true;
//}

