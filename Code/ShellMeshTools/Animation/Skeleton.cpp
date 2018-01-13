#include "ShellMeshTools/stdafx.h"
#include "Skeleton.h"



namespace vr
{

// ================================ //
//
Skeleton::Skeleton( std::vector< Joint >&& joints )
	:	m_joints( std::move( joints ) )
{}



}

