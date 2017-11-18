 #ifndef _MATRIX_INVERSE_HLSL
#define _MATRIX_INVERSE_HLSL


// ================================ //
// http://answers.unity3d.com/questions/218333/shader-inversefloat4x4-function.html
// Under WTFPL license.
float4x4	Inverse4x4			( float4x4 input )
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


// ================================ //
//
float3x3	Inverse3x3			( float3x3 matrixA )
{
	float detA = determinant( matrixA );
	
	float3 col1 = matrixA._11_21_31;
	float3 col2 = matrixA._12_22_32;
	float3 col3 = matrixA._13_23_33;

	float3 row1 = cross( col2, col3 );
	float3 row2 = cross( col3, col1 );
	float3 row3 = cross( col1, col2 );

	float3x3 inverseMat = float3x3( row1, row2, row3 );

	return inverseMat / detA;
}


