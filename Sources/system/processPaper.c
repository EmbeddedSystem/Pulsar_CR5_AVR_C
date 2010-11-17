#include "processPaper.h"

static u08 AskPaper(void)
{
	sbi(DDRF, 5);
	sbi(PORTF, 5); // �������� ���������

	cbi(DDRE,3); // ���� ������� ����� ������
	sbi(PORTE,3); // �������� ����� � +5

	if ((PINE & (1 << 3)) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	cbi(PORTF, 5);// ��������� ���������
}

void ProcessPaper()
{
	if (AskPaper())
	{
		SendMessage(MSG_B_PAPER_END);
	}
}
