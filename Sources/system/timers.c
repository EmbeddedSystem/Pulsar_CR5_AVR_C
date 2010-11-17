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
	TCNT0 = 0x8D; // ����������� �������
	if (SPEAKER_ON) // ���� ����, ������� ����� ������� � ����� 2 ���
	{
		PORTF = PORTF ^ (1 << PF3);
	}
	else
	{
		cbi(PORTF, PF3); // ����� �� ���� �� �����, � ���� ������ � 0
	}
	int i;
	for (i = 0; i <= LAST_TIMER; i++)
	{
		if (TStates[i] == TIMER_RUNNING)
		{
			Timers[i]++; //��������� ��������� ����������� ��������
		}
	}
}

ISR(TIMER1_OVF_vect)
{
}

void InitTimers(void)
{
	timer0_init(); //������������� ����������� �������
	int i;
	for (i = 0; i <= LAST_TIMER; i++)
	{
		TStates[i] = TIMER_STOPPED; // ������������� ����������� ��������
		// �������� �������� �������������, �� ����� ������ ���������� ��� �����
	}
	sbi(DDRF, PF3); // ������������� ���� ������� �� �����
}

void StartTimer(int TimerID) // ������ �������
{
	Timers[TimerID] = 0; // ����� �������� �������
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
