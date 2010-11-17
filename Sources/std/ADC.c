// Функции АЦП для измерения заряда аккумулятора

#include "ADC.h"

int AskAccum(void)
{
	int result;
	sbi(DDRC, PC3);
	sbi(PORTC, PC3);

	result = a2dConvert10bit(0);
	cbi(PORTC, PC3);
	return result;
}
