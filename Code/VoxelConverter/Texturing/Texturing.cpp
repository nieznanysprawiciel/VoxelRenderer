#include "Texturing.h"





Texturing::Texturing	( vr::OctreePtr octree )
	:	m_octree( octree )
	,	m_wrappingModeX( WrappingMode::Mirror )
	,	m_wrappingModeY( WrappingMode::Mirror )
{}
