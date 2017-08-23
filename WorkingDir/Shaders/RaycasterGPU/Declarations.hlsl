#ifndef _DECLARATIONS_HLSL
#define _DECLARATIONS_HLSL



#define CAST_STACK_DEPTH        23
#define ROOT_OFFSET				514


struct StackElement
{
	uint		Node;
	float		tMax;
};


struct RaycasterContext
{
	//const OctreeNode*				ChildDescriptor;

	float3							RayDirection;
	float3							RayStartPosition;

	float3							tCoeff;
	float3							tBias;
	float3							Position;
	float							tMax;
	float							tMin;

	uint							Current;		///< Current node, we are in.
	uint							OctantMask;		///< Child bit flipping.
	uint							ChildIdx;		///< Child in children mask.
	
	int								Scale;
	float							ScaleExp;

	StackElement					NodesStack[ CAST_STACK_DEPTH ];		///< Absolut offsets from beginning of array.
};



struct CameraData
{
	float3				Position;
	float3				Direction;
	float3				UpVector;
	float3				RightVector;
	float				Fov;
	float				Width;
	float				Height;
	float				ViewportSize;
	float				NearPlane;
	float				FarPlane;
	bool				IsPerspective;
};


struct Input
{
	float3			RayDirection;
	float3			RayPosition;

};


#endif
