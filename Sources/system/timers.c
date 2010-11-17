#include "timers.h"

void timer0_init(void)
{
	sbi(TCCR0, CS02); // fosc/64
	sbi(TIFR, TOV0);
	TCNT0 = 0x8D; // 2kHz, 500uS
	sbi(TIMSK, TOIE0);
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 0x8D; // Перезарядка таймера
	if (SPEAKER_ON) // Если надо, дергаем ногой спикера и пищим 2 КГц
	{
		PORTF = PORTF ^ (1 << PF3);
	}
	else
	{
		cbi(PORTF, PF3); // Когда не надо не пищим, а нога всегда в 0
	}
	int i;
	for (i = 0; i <= LAST_TIMER; i++)
	{
		if (TStates[i] == TIMER_RUNNING)
		{
			Timers[i]++; //Инкремент счетчиков программных таймеров
		}
	}
}

ISR(TIMER1_OVF_vect)
{
}

void InitTimers(void)
{
	timer0_init(); //Инициализация аппаратного таймера
	int i;
	for (i = 0; i <= LAST_TIMER; i++)
	{
		TStates[i] = TIMER_STOPPED; // Инициализация программных таймеров
		// счетчики обнулять необязательно, во время старта обнуляются все равно
	}
	sbi(DDRF, PF3); // Инициализация ноги пищалки на выход
}

void StartTimer(int TimerID) // Запуск таймера
{
	Timers[TimerID] = 0; // Сброс счетчика таймера
	TStates[TimerID] = TIMER_RUNNING;
}

void PauseTimer(int TimerID)
{
	if (TStates[TimerID] == TIMER_RUNNING)
	{
		TStates[TimerID] = TIMER_PAUSED;
	}
}

void ReleaseTimer(int TimerID)
{
	if (TStates[TimerID] == TIMER_PAUSED)
	{
		TStates[TimerID] = TIMER_RUNNING;
	}
}

u32 GetTimer(int TimerID)
{
	return Timers[TimerID];
}
