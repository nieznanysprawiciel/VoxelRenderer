#pragma once


#include "swCommonLib/ParameterAnimation/Animation/Animation.h"

#include "swCommonLib/ParameterAnimation/Parameters/DirectPropertyh.h"
#include "swCommonLib/ParameterAnimation/Parameters/PropertyPath.h"
#include "swCommonLib/ParameterAnimation/Parameters/StringPropertyPath.h"


#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <string>



struct Nested2
{
	float			SomeValue;
	int				IntValue;
	std::string		Name;
};

struct Nested
{
	float			SomeValue;
	int				IntValue;
	std::string		Name;
	Nested2			Additional;
};



enum Methods
{
	Discrete,
	Linear,
	Integral
};


/**@brief */
class TestAnim : public EngineObject
{
	RTTR_ENABLE( EngineObject );
	RTTR_REGISTRATION_FRIEND;
public:
	float			m_energy;
	float			m_mass;
	float			m_power;
	float			m_shit;

	Nested			m_otherData;

protected:
public:
	Ptr< FloatAnimation< StringPropertyPath > >		FloatAnimString;
	Ptr< FloatAnimation< PropertyPath > >			FloatAnimProperty;
	Ptr< FloatAnimation< DirectProperty > >			FloatAnimDirect;

public:
	explicit		TestAnim();
	~TestAnim() { };

};


class TestInterpolators : public EngineObject
{
	RTTR_ENABLE( EngineObject );
	RTTR_REGISTRATION_FRIEND;
public:

	double		DoubleField;
	float		FloatField;

	uint64		UIntField64;
	int64		IntField64;
	uint32		UIntField32;
	int32		IntField32;
	uint16		UIntField16;
	int16		IntField16;
	uint8		UIntField8;
	int8		IntField8;

	char		CharField;
	bool		BoolField;

	std::string		StringField;
	std::wstring	WStringField;

	Methods			EnumField;

	DirectX::XMFLOAT4					Color4F;
	DirectX::XMFLOAT3					Position3F;
	DirectX::PackedVector::XMCOLOR		Color;

public:
	explicit		TestInterpolators();
	~TestInterpolators() { };

	template< typename FieldType >
	FieldType&		GetField();

#define DEFINE_GET_FIELD( type, name )			\
template<>										\
type&		GetField< type >() { return name;  }
	
	DEFINE_GET_FIELD( uint64, UIntField64 );
	DEFINE_GET_FIELD( int64, IntField64 );
	DEFINE_GET_FIELD( uint32, UIntField32 );
	DEFINE_GET_FIELD( int32, IntField32 );
	DEFINE_GET_FIELD( uint16, UIntField16 );
	DEFINE_GET_FIELD( int16, IntField16 );
	DEFINE_GET_FIELD( uint8, UIntField8 );
	DEFINE_GET_FIELD( int8, IntField8 );

	DEFINE_GET_FIELD( char, CharField );
	DEFINE_GET_FIELD( bool, BoolField );
	DEFINE_GET_FIELD( std::string, StringField );
	DEFINE_GET_FIELD( std::wstring, WStringField );

	DEFINE_GET_FIELD( double, DoubleField );
	DEFINE_GET_FIELD( float, FloatField );
	DEFINE_GET_FIELD( Methods, EnumField );

	DEFINE_GET_FIELD( DirectX::XMFLOAT4, Color4F );
	DEFINE_GET_FIELD( DirectX::XMFLOAT3, Position3F );
	DEFINE_GET_FIELD( DirectX::PackedVector::XMCOLOR, Color );

#undef DEFINE_GET_FIELD
};






//====================================================================================//
//			Impl	
//====================================================================================//

inline TestAnim::TestAnim()
{
	m_energy = 1.0f;
	m_mass = 2.0f;
	m_power = 3.0f;
	m_shit = 4.0f;
	m_otherData.SomeValue = 5.0f;
	m_otherData.IntValue = 6;
	m_otherData.Name = "Nested other data";
	m_otherData.Additional.SomeValue = 7.0f;
	m_otherData.Additional.Name = "Nested2 additional data";
	m_otherData.Additional.IntValue = 8;
}



inline TestInterpolators::TestInterpolators()
{
	DoubleField = 1.0;
	FloatField = 2.0;

	UIntField64 = 4;
	IntField64 = 4;
	UIntField32 = 4;
	IntField32 = 4;
	UIntField16 = 4;
	IntField16 = 4;
	UIntField8 = 4;
	IntField8 = 4;

	CharField = 4;
	BoolField = false;

	StringField = "To jest string";
	WStringField = L"To jest wstring";

	EnumField = Methods::Integral;
}


