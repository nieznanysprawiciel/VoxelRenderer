#pragma once


#include <DirectXMath.h>




// ================================ //
// This structure should be binary equal to structure in Declarations.hlsl file.
// Note that members of ShaderCameraData could be in different order then in CameraData structure.
struct ShaderCameraData
{
	DirectX::XMFLOAT3	Position;
	float				Fov;
	DirectX::XMFLOAT3	Direction;
	float				Width;
	DirectX::XMFLOAT3	UpVector;
	float				Height;
	DirectX::XMFLOAT3	RightVector;
	float				ViewportSize;
	float				NearPlane;
	float				FarPlane;
	bool				IsPerspective;

	float				Align1;			///< Constant buffers must have size which is multiple of 16 bytes.
};


/**@brief Constant buffer layout with camera data.

Buffer contains not only camera data but also other rarely changing

@ingroup ConstantBuffers*/
struct CameraConstants
{
	DirectX::XMFLOAT4X4		ViewMatrix;				///< View matrix.
	DirectX::XMFLOAT4X4		ProjectionMatrix;		///< Projection matrix.
	DirectX::XMFLOAT3		CameraPosition;			///< Position of camera in world space.


	float					FillTo16Bytes;
};

