/**
@file DynamicActor.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "DynamicActor.h"
//#include "EngineCore/ControllersEngine/BaseClasses/IController.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< DynamicActor >( "DynamicActor" )
		.property( "Speed", &DynamicActor::m_speed )
		(
			rttr::metadata( MetaDataType::Category, "Movement" ),
			rttr::policy::prop::bind_as_ptr
		)
		.property( "RotationSpeed", &DynamicActor::m_rotationSpeed )
		(
			rttr::metadata( MetaDataType::Category, "Movement" ),
			rttr::policy::prop::bind_as_ptr
		)
		.property( "Mass", &DynamicActor::m_mass )
		(
			rttr::metadata( MetaDataType::Category, "Physical properties" )
		);
}


using namespace DirectX;


DynamicActor::DynamicActor()
	: m_controller( nullptr )
{
	m_speed.x = 0.0;
	m_speed.y = 0.0;
	m_speed.z = 0.0;

#ifdef _QUATERNION_SPEED
	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4( &m_rotationSpeed, quaternion );
#else
	m_rotationSpeed.w = 0.0;
	m_rotationSpeed.x = 0.0;
	m_rotationSpeed.y = 0.0;
	m_rotationSpeed.z = 0.0;
#endif

	m_mass = 1.0f;
}

DynamicActor::DynamicActor( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed )
	: m_controller( nullptr )
{
	m_speed = move_speed;
	m_rotationSpeed = rot_speed;

	m_mass = 1.0f;
}

/**@brief Usuwa przypisany kontroler.*/
DynamicActor::~DynamicActor()
{
	//delete m_controller;
}


/**Funkcja wykonywana w ka¿dym obiegu pêtli renderingu przez obiekt MovementEngine.
 Dodajemy aktualny wektor prêdkoœci i prêdkoœci obrotowej do wektorów po³o¿enia i orientacji.
 Poniewa¿ te wektory wyra¿aj¹ przesuniêcie/rotacje na sekundê, to musimy to przemno¿yæ jeszcze przez time_interval.
 
 W momencie wykonania zosta³y juz uwzglêdnione oddzia³ywania fizyczne oraz wp³yw kontrolerów.
 Nale¿y zauwa¿yæ, ¿e o ile najlepiej by by³o, ¿eby kontrolery tak¿e u¿ywa³y tych zmiennych do poruszania
 obiektami na scenie, to nie jest to obowi¹zek, bo maj¹ te¿ bezpoœredni dostêp do po³o¿enia i orientacji.
 
 Sprawdzanie kolizji i ewentualne przesuniêcia z tym zwi¹zane nastêpuj¹ dopiero po zakoñczeniu wywo³ywania tych funkcji.
 
 Do przeliczania u¿ywamy biblioteki DirectXmath, która u¿ywa zmiennych wektorowych i wykonuje obliczenia
 na jednostkach SSE2. Do tego potrzebne jest wyrównanie zmiennych do 16 bitów, czego nie oferuj¹ zmienne 
 XMFloat, dlatego trzeba wykonywaæ operacjê XMLoadFloat4 i XMStoreFloat4. 
 Uwaga zmienne XMVECTOR i XMMATRIX nie mog¹ byc alokowane na stercie, poniewa¿ nie jest tam gwarantowane 
 16bitowe wyrównanie. Po dok³adny opis odsy³am do MSDNu.*/
void DynamicActor::Move(float time_interval)
{
//translacja
	XMVECTOR pos = GetPosition();
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = GetSpeed();
	translate *= time;
	pos = pos + translate;
	SetPosition( pos );

//w docelowej wersji trzeba siê zdecydowaæ, co lepsze i resztê wywaliæ
#ifdef _QUATERNION_SPEED
//orientacja
	XMVECTOR orient = GetOrientation();
	XMVECTOR rot = GetRotationSpeed();

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/

	SetOrientation( orient );
#else
	XMVECTOR orient = GetOrientation();				//pobieramy orientacjê
	XMVECTOR rot = GetRotationSpeed();				//pobieramy oœ obrotu (k¹t te¿, ale on nie ma znaczenia)

	if ( !XMVector3Equal( rot, XMVectorZero() ) )
	{
		float rot_angle = m_rotationSpeed.w * time_interval;	//liczymy k¹t obrotu

		rot = XMQuaternionRotationAxis( rot, rot_angle );		//przerabiamy na kwaternion
		orient = XMQuaternionMultiply( orient, rot );			//liczymy nowy kwaternion orientacji
	}
		
	SetOrientation( orient );
#endif
}

/**Funkcja o zastosowaniu tym samym co Move, z t¹ ró¿nic¹, ¿e wykonywana dla obiektów z³o¿onych. Przesuniêcie
jest z³o¿eniem ruchu rodzica i danegi obiektu.

Funkcja jest wirtualna i w klasie ComplexActor (implementacja poni¿ej) wywo³uje rekurencyjnie tê funkcjê
dla wszystkich swoich dzieci.

UWAGA!! nale¿y pamiêtaæ, ¿e w klasie MovementEngine nie powinno wyst¹piæ ¿adne dziecko klasy ComplexActor.
W przeciwnym razie niektóre przesuniecia i obroty zostan¹ zastosowane wielokrotnie do jednego obiektu.*/
void DynamicActor::MoveComplex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{
	return;
	//DOKOÑCZYC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//translacja
	XMVECTOR pos = GetPosition();
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&m_speed);
	XMVECTOR parent_translate = XMLoadFloat3(&parent_speed);
	
	translate += parent_translate;		//g³ówna ró¿nica: dodajemy przesuniêcie rodzica
	translate *= time;
	pos = pos + translate;
	SetPosition( pos );

	//orientacja
	XMVECTOR orient = GetOrientation();
	XMVECTOR rot = XMLoadFloat4(&m_rotationSpeed);
	XMVECTOR parent_rotn = XMLoadFloat4(&parent_rotation);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/

	SetOrientation( orient );
}


// ================================ //
//
void				DynamicActor::SetController		( IController* ctrl )
{
	m_controller = ctrl;
	//m_controller->Initialize( this );
}

// ================================ //
//
IController*		DynamicActor::GetController		()
{
	return m_controller;
}

