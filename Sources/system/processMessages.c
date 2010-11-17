#include "processMessages.h"

void InitMessages(void)
{
	u08 i;
	for (i = 0; i <= MSG_LAST; i++)
		Messages[i] = 0;
}

void SendMessage(u08 Msg)
{
	if (Messages[Msg] == 0)
		Messages[Msg] = 1;
}

u08 GetMessage(u08 Msg)
{
	if (Messages[Msg] == 2)
	{
		if (Msg <= MSG_LAST) // Если сообщение обычное а не широковещательное, сбрасываем его
			Messages[Msg] = 0;
		return 1;
	}
	return 0;
}

void ProcessMessages(void)
{
	u08 i;
	for (i = 0; i <= MSG_B_LAST; i++)
	{
		if (Messages[i] == 2)
			Messages[i] = 0;
		else if (Messages[i] == 1)
			Messages[i] = 2;
	}
}
