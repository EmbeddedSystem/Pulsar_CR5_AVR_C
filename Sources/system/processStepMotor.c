#include "processStepMotor.h"

unsigned char printbuf[192]; //192 байта буфер печати 192*8 точек

void TPU_init(void)
{
	// Ur_EN = 0
	cbi(PORTA, PA3);
	sbi(DDRA, PA3);

	// LATCH = 1
	sbi(PORTF, PF2);
	sbi(DDRF, PF2);

	// ST = 1
	sbi(PORTA, PA1);
	sbi(DDRA, PA1);

	sbi (DDRA, PA7); // A
	sbi (DDRC, PC7); // NA
	sbi (DDRA, PA4); // B
	sbi (DDRA, PA6); // NB
}

void ClearTPH(void)
{
	int i;
	for (i = 0; i < 192; i++)
	{
		printbuf[i] = 0;
	}
}

void PutPixelTPH(unsigned int xp, unsigned int yp)
{
	printbuf[((xp / 8) + (yp * 24))] |= 128 >> (xp & 7);
}

void OutTextTPH(int X,//координаты
		char* str,//строка вывода
		int n)//, //количество символов
//unsigned char inv)//инвертирование символов
{
	ClearTPH();

	unsigned char textL, Put;
	int a, Y = 0;

	for (a = 0; (a < n); a++)
	{
		if (str[a] < 0x90)
			textL = 0x20;
		else
			textL = 0x60;
		//if (X>(133-5)){X=0;Y=Y+8;}//перевод каретки, определить по разрещению ЖКИ
		int i;
		for (i = 0; i < 5; i++)//
		{
			Put = pgm_read_byte(&sym[str[a]-textL][i]);//^inv;

			if ((Put & 1) > 0)
				PutPixelTPH(X, Y);
			if ((Put & 2) > 0)
				PutPixelTPH(X, Y + 1);
			if ((Put & 4) > 0)
				PutPixelTPH(X, Y + 2);
			if ((Put & 8) > 0)
				PutPixelTPH(X, Y + 3);
			if ((Put & 16) > 0)
				PutPixelTPH(X, Y + 4);
			if ((Put & 32) > 0)
				PutPixelTPH(X, Y + 5);
			if ((Put & 64) > 0)
				PutPixelTPH(X, Y + 6);
			if ((Put & 128) > 0)
				PutPixelTPH(X, Y + 7);
			X++;
		}
		Put = 0;//(0^inv);
		if ((Put & 1) > 0)
			PutPixelTPH(X, Y);
		if ((Put & 2) > 0)
			PutPixelTPH(X, Y + 1);
		if ((Put & 4) > 0)
			PutPixelTPH(X, Y + 2);
		if ((Put & 8) > 0)
			PutPixelTPH(X, Y + 3);
		if ((Put & 16) > 0)
			PutPixelTPH(X, Y + 4);
		if ((Put & 32) > 0)
			PutPixelTPH(X, Y + 5);
		if ((Put & 64) > 0)
			PutPixelTPH(X, Y + 6);
		if ((Put & 128) > 0)
			PutPixelTPH(X, Y + 7);
		X++;
	}
}
;

char step = 1;

u08 n_steps = 4;

//void MotorOneSemiStep()
void MotorOneSemiStep()
{
	switch (step)
	{
	case 1:
		SET_B;
		CLR_NB;
		CLR_A;
		CLR_NA;
		step = 2;
		break;
	case 2:
		SET_B;
		CLR_NB;
		SET_A;
		CLR_NA;
		step = 3;
		break;
	case 3:
		CLR_B;
		CLR_NB;
		SET_A;
		CLR_NA;
		step = 4;
		break;
	case 4:
		CLR_B;
		SET_NB;
		SET_A;
		CLR_NA;
		step = 5;
		break;
	case 5:
		CLR_B;
		SET_NB;
		CLR_A;
		CLR_NA;
		step = 6;
		break;
	case 6:
		CLR_B;
		SET_NB;
		CLR_A;
		SET_NA;
		step = 7;
		break;
	case 7:
		CLR_B;
		CLR_NB;
		CLR_A;
		SET_NA;
		step = 8;
		break;
	case 8:
		SET_B;
		CLR_NB;
		SET_A;
		CLR_NA;
		step = 1;
		break;
	}
}

void MotorOneFullStep(void)
{
	switch (step)
	{
	case 1:
		SET_B; // B 1
		CLR_NB; // /B 0
		SET_A; //   A 1
		CLR_NA; // /A 0
		step = 2;
		break;
	case 2:
		CLR_B; //  B 0
		SET_NB; // /B 1
		SET_A; //  A 1
		CLR_NA; // /A 0
		step = 3;
		break;
	case 3:
		CLR_B; //  B 0
		SET_NB; // /B 1
		CLR_A; //  A 0
		SET_NA; // /A 1
		step = 4;
		break;
	case 4:
		SET_B; //  B 1
		CLR_NB; // /B 0
		CLR_A; //  A 0
		SET_NA; // /A 1
		step = 1;
		break;
	}
}

void Roll(stepType type, u08 steps, float Tprint, float Tmotor)
{
	// LATCH...
	cbi(PORTF, PF2);
	sbi(PORTF, PF2);

	// ST...
	cbi(PORTA, PA1);
	_delay_ms(Tprint);
	sbi(PORTA, PA1);
	//			_delay_ms(1.25);

	int i;
	for (i = 0; i < steps; i++)
	{
		if (type == semiStep)
		{
			MotorOneSemiStep();
		}
		else
		{
			MotorOneFullStep();
		}
		_delay_ms(Tmotor);
	}
}

void PrintTPH(stepType param_type, u08 steps, u08 lines, float Tprint,
		float Tmotor)
{
	spiInitTPH();

	int i, j;

	for (i = 0; i < 8; i++)
	{
		for (j = 23; j >= 0; --j)
		{
			spiSendByte(printbuf[j + (24 * i)]);
		}

		UR_ON;
		Roll(param_type, steps, Tprint, Tmotor);
		UR_OFF;
	}
}

void ProcessStepMotor()
{
}
