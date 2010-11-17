#include "processKeyboard.h"

void InitFsmKeyboard()
{
	StartTimer(TIMER_3);
}

void ProcessKeyboard()
{
	static u08 state;
	switch (state)
	{
	// Õ≈ Õ¿∆¿“¿: ≈—À» I2C «¿Õﬂ“ - ¬€’Œƒ, ≈—À» —¬Œ¡Œƒ≈Õ - Õ¿◊¿“‹ —◊»“€¬¿“‹  Œƒ  ÕŒœ »
	case 0:
		if (GetTimer(TIMER_3) >= 250 * msec)
		{
			StartTimer(TIMER_3);

			if (!I2C_BUSY)
			{
				i2cAddress = I2C_ADDR_KEYBRD;
				bufferInit(&i2cTransmitBuffer, i2cTransmitBuffer_,
						sizeof(i2cTransmitBuffer_));
				bufferInit(&i2cReceiveBuffer, i2cReceiveBuffer_,
						sizeof(i2cReceiveBuffer_));
				bufferAddToEnd(&i2cTransmitBuffer, 'I');
				bufferAddToEnd(&i2cTransmitBuffer, 0x0D);
				i2cNBytes = 11;
				i2cSendStart();
				state = 1;
			}
		}
		break;
		// Œ∆»ƒ¿Õ»≈  Œƒ¿  ÕŒœ » »« I2C
	case 1:
		if (GetMessage(MSG_B_I2C_OK))
		{
			keyCode = bufferGetAtIndex(&i2cReceiveBuffer, 8);
			SendMessage(MSG_DEBOUNCE);
			break;
		}
		state = 0;
		break;
	}
	if (GetMessage(MSG_B_I2C_ERROR_NODEV))
	{
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		sbi(PORTB, PB7); //HACK: RESET PICF16818
		state = 0;
		StartTimer(TIMER_3);
	}
	if (GetMessage(MSG_B_I2C_ERROR_DATA_NACK))
	{
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		cbi(PORTB, PB7); //HACK: RESET PICF16818
		sbi(PORTB, PB7); //HACK: RESET PICF16818
		state = 0;
		StartTimer(TIMER_3);
	}
}
