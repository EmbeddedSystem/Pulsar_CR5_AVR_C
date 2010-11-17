#include "processPaper.h"

static u08 AskPaper(void)
{
	sbi(DDRF, 5);
	sbi(PORTF, 5); // включили светодиод

	cbi(DDRE,3); // вход датчика конца бумаги
	sbi(PORTE,3); // подтяжка входа к +5

	if ((PINE & (1 << 3)) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	cbi(PORTF, 5);// выключили светодиод
}

void ProcessPaper()
{
	if (AskPaper())
	{
		SendMessage(MSG_B_PAPER_END);
	}
}
