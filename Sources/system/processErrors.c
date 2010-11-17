#include "processErrors.h"

u08 buffer[20];

void InitFsmErrors(void)
{

}

void DebugInfo(u08 data)
{
}

void ProcessErrors(void)
{
	if (GetMessage(MSG_B_KEY_PRESSED))
	{
		bufferAddToEnd(&speakerBuffer, CLICK);
		SendMessage(MSG_BEEP);
		rprintfChar(keyPressedCode);
	}

}
