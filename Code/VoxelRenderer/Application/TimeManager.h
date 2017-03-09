#pragma once
/**@file TimeManager.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy TimeManager do obs³ugi rzeczy zwi¹zanych z czasem.*/

#include "swCommonLib/Common/TypesDefinitions.h"

/**@brief Klasa obs³uguje wszystko co zwi¹zane z czasem.

Do zadañ klasy nale¿y:
- pobieranie i przeliczanie czasu
- obs³uga eventów czasowych
- Robienie statystyk wykonania poszczególnych fragmentów kodu

Funkcje zaczynaj¹ce siê od query pobieraj¹ czas z systemu.
Funkcje get zwracaj¹ wartoœci obliczone wczeœniej i przechowywane w klasie.
Tak naprawdê powinno siê raczej korzystaæ z funkcji get, poniewa¿ w silniku nie liczy siê
czas aktualny, ale czas jaki jest w danej klatce.*/
class TimeManager
{
private:
	//time control
	int64						m_beginTime;		///<Zawiera czas rozpoczêcia dzia³ania aplikacji. (Tak dok³adniej czas wejœcia w pêtlê renderingu. Wczeœnie odbywa siê wczytywanie Levelu, co mo¿e trwaæ.)

	int64						m_timerFrequency;	///<Czêstotliwoœæ timera
	int64						m_timePrevious;		///<Bêdziemy mierzyæ ró¿nicê miedzy czasami w kolejnych klatkach.
	int64						m_elapsedTime;		///<Do licznika FPSów, czas od ostatniej aktualizacji m_framesPerSec
	float						m_lag;				///<OpóŸnienie wzglêdem normalego tempa updatowania sceny
	unsigned int				m_frames;			///<Zlicza ramki od ostatniego wyzerowania
	float						m_framesPerSec;		///<Najwa¿niejszy licznik w grze
public:
	TimeManager();
	~TimeManager() = default;

	void			InitTimer();
	float			onStartRenderFrame();
	int64			QueryCurrentTime();
	double			QueryTimeFromBegin();
	float			GetCurrentLag();
	inline int64	GetBeginTime()					{ return m_beginTime; }				///<Zwraca czas jaki zosta³ zarejestrowany przy inicjacji silnika.
	inline int64	GetTimerFrequency()				{ return m_timerFrequency; }		///<Zwraca czêstotliwoœæ taktowania uzywanego timera.
	inline float	GetFrameLag()					{ return m_lag; }					///<Zwraca zmienn¹ m_lag.
	inline int64	GetTime()						{ return m_timePrevious; }			///<Zwraca czas, jaki zosta³ ostatnio zarejestrowany przy wywo³aniu onStartRenderFrame.
	inline void		UpdateTimeLag( float timeLag )	{ m_lag = timeLag; }				///<Aktualizuje zmienn¹ m_lag.
};

