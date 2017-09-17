#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#ifdef _M_AMD64
#include <mutex>

typedef std::mutex SpinLock;

#else


/**
@brief Klasa s³u¿¹ca jako mutex z aktywnym oczekiwaniem.

Klasa SpinLock u¿ywa aktywnego oczekiwania w celu unikniêcia koniecznoœci
odwo³ywania siê do systemu operacyjnego, gdy wejœcie do sekcji krytycznej nie jest mo¿liwe.
Implementacj¹ opiera siê o instrukcjê TSL.

Mo¿liwe jest u¿ywanie klasy we wspó³pracy z mechanizmami takimi jak std::lock_guard.

Ze wzglêdu na to, ¿e Visual Studio nie potrafi kompilowaæ asemblera pod architekturê ARM,
na razie klasa nadaje siê do u¿ytku jedynie pod procesorami x86.

@ingroup Helpers
*/
class SpinLock
{
private:
	volatile int32		m_locked;
protected:
public:
	SpinLock()
	{
		m_locked = 0;
	}
	~SpinLock() = default;

	inline void lock();
	inline void unlock();
	inline bool try_lock();
};


/**@brief Wejœcie do sekcji krytycznej.
*/
inline void SpinLock::lock()
{
	_asm
	{
		mov			ecx,			[this]
		mov			EAX,			0xF
		TRY_LOCK_LABEL:
		xchg		EAX,			[ecx].m_locked
		and			EAX,			0xFF
		jnz			TRY_LOCK_LABEL
	}
}


/**@brief Opuszczenie sekcji krytycznej.
*/
inline void SpinLock::unlock()
{
	_asm mov		ecx,				[this]
	_asm mov		[ecx.m_locked],		0x0
	//_asm mov		EAX,		0x0
	//_asm xchg		EAX,		[ecx].m_locked
}


/**@brief Próbuje zaj¹æ sekcjê krytyczn¹. Je¿eli jest zajêta, wychodzi natychmiast.

@return Zwraca true, je¿eli uda³o siê wejœæ do sekcji krytycznej.
*/
inline bool SpinLock::try_lock()
{
	bool success = false;
	_asm
	{
		mov			ecx,			[this]
		mov			EAX,			0xF
		xchg		EAX,			[ecx].m_locked
		cmp			EAX,			0x0
		jne			LOCK_FAILED_LABEL
	}
	return true;
	LOCK_FAILED_LABEL:
	return false;
}

#endif
