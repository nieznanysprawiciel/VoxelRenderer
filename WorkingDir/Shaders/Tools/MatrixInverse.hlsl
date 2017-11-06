 #ifndef _MATRIX_INVERSE_HLSL
#define _MATRIX_INVERSE_HLSL


// ================================ //
// http://answers.unity3d.com/questions/218333/shader-inversefloat4x4-function.html
// Under WTFPL license.
float4x4	Inverse		( float4x4 input )
{
#define minor(a,b,c) determinant( float3x3( input.a, input.b, input.c ) )
//determinant(float3x3(input._22_23_23, input._32_33_34, input._42_43_44))

	float4x4 cofactors = float4x4(
		minor( _22_23_24, _32_33_34, _42_43_44 ),
		-minor( _21_23_24, _31_33_34, _41_43_44 ),
		minor( _21_22_24, _31_32_34, _41_42_44 ),
		-minor( _21_22_23, _31_32_33, _41_42_43 ),

		-minor( _12_13_14, _32_33_34, _42_43_44 ),
		minor( _11_13_14, _31_33_34, _41_43_44 ),
		-minor( _11_12_14, _31_32_34, _41_42_44 ),
		minor( _11_12_13, _31_32_33, _41_42_43 ),

		minor( _12_13_14, _22_23_24, _42_43_44 ),
		-minor( _11_13_14, _21_23_24, _41_43_44 ),
		minor( _11_12_14, _21_22_24, _41_42_44 ),
		-minor( _11_12_13, _21_22_23, _41_42_43 ),

		-minor( _12_13_14, _22_23_24, _32_33_34 ),
		minor( _11_13_14, _21_23_24, _31_33_34 ),
		-minor( _11_12_14, _21_22_24, _31_32_34 ),
		minor( _11_12_13, _21_22_23, _31_32_33 )
		);

	return transpose( cofactors ) / determinant( input );

	#undef minor
}

#endif
