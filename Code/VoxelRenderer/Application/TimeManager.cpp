/**
@file TimeManager.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "TimeManager.h"

#include <Windows.h>


static const unsigned int FRAMES_PER_SEC_UPDATE = 10;



/**@brief Kontruktor inicjuje wartoœci m_frames i m_framesPerSec na 0.*/
TimeManager::TimeManager()
{
	m_frames = 0;
	m_framesPerSec = 0;
	m_lag = 0.0;
	m_timerFrequency = 1;	// ¯eby unikn¹æ dzielenia przez zero, je¿eli ktoœ nie zainicjuje timera.
}

/**@brief Funkcja wywo³ywana po zainicjowaniu silnika i przed wejœciem
w pêtlê renderingu. Ustawia odpowiednie dane dla zegara, w tym czêstotliwoœæ oraz czas pocz¹tkowy.*/
void TimeManager::InitTimer()
{
	LARGE_INTEGER timeTemp;

	//Sprawdzamy czêstotliwoœæ timera systemowego, a potem inicjujemy licznik czasu
	QueryPerformanceFrequency( &timeTemp );
	m_timerFrequency = timeTemp.QuadPart;
	QueryPerformanceCounter( &timeTemp );		//inicjujemy licznik czasu
	m_timePrevious = timeTemp.QuadPart;

	//FPS counter
	m_elapsedTime = 0;
	m_frames = 0;

	m_beginTime = m_timePrevious;			// Poniewa¿ w³aœnie zainicjowaliœmy timer, to zapisujemy czas inicjacji.
}

/**@brief Funkcja oblicza interwa³ czasowy jaki up³yn¹³ od ostatniej ramki.

Poza tym s¹ tu generowane eventy dotycz¹ce czasu, opóŸnieñ itp.
@return Zwraca interwa³ jaki up³yn¹³ od ostatniego wywo³ania funkcji.*/
float TimeManager::onStartRenderFrame()
{
	int64 timeCurrent = QueryCurrentTime();

	int64 timeDiff;
	timeDiff = timeCurrent - m_timePrevious;
	float timeInterval = (float)timeDiff / m_timerFrequency;

	m_lag += timeInterval;

	//zliczanie FPSów
	m_elapsedTime += timeDiff;
	if ( m_elapsedTime >= FRAMES_PER_SEC_UPDATE * m_timerFrequency )	//aktualizujemy co 10 sekund
	{
		m_framesPerSec = (float)m_frames / FRAMES_PER_SEC_UPDATE;	//FRAMES_PER_SEC_UPDATE w sekundach
		m_elapsedTime = m_elapsedTime % ( FRAMES_PER_SEC_UPDATE * m_timerFrequency );
		m_frames = 0;		//zerujemy liczbê klatek
	}

	//todo:	generujemy eventy czasowe

	//zapisujemy obecny czas i wychodzimy z funkcji
	m_timePrevious = timeCurrent;
	++m_frames;		//inkrementujemy licznik klatek
	return timeInterval;
}

/**@brief Wywo³uje QueryPerformanceCounter, ¿eby pobraæ aktualny czas.

Nale¿y pamiêtaæ, ¿e najczêœciej interesuje nas czas aktualnej klatki albo lag.
Mo¿na je pobraæ funkcjami TimeManager::GetTime i TimeManager::GetFrameLag.

Ta funkcja pobiera aktualny czas nie¿ny od wyœwietlania klatek.
*/
int64 TimeManager::QueryCurrentTime()
{
	LARGE_INTEGER time_temp;
	QueryPerformanceCounter( &time_temp );
	return time_temp.QuadPart;
}

/**@brief Zwraca czas w sekundach od w³¹czenia silnika.

Podawany jest aktualny czas, a nie czas zapisany na pocz¹tku klatki.
*/
double TimeManager::QueryTimeFromBegin()
{
	int64 currentTime = QueryCurrentTime();
	int64 timeDiff = currentTime - m_beginTime;
	return (double)timeDiff / m_timerFrequency;
}

float TimeManager::GetCurrentLag()
{
	int64 timeCurrent = QueryCurrentTime();
	
	int64 timeDiff;
	timeDiff = timeCurrent - m_timePrevious;
	float timeInterval = (float)timeDiff / m_timerFrequency;

	return m_lag + timeInterval;
}
