#include "processEKLZ.h"

void InitFsmEKLZ(void)
{
}

void ProcessEKLZ(void)
{
	if (GetMessage(MSG_B_KEY_PRESSED))
	{
		if (keyCode == 0x05)
		{
			if (!I2C_BUSY)
			{
				i2cAddress = I2C_ADDR_EKLZ;
				bufferInit(&i2cTransmitBuffer, i2cTransmitBuffer_,
						sizeof(i2cTransmitBuffer_));
				bufferInit(&i2cReceiveBuffer, i2cReceiveBuffer_,
						sizeof(i2cReceiveBuffer_));
				bufferAddToEnd(&i2cTransmitBuffer, 0x02);
				bufferAddToEnd(&i2cTransmitBuffer, 0x02);
				bufferAddToEnd(&i2cTransmitBuffer, 0x01);
				bufferAddToEnd(&i2cTransmitBuffer, 0x02);
				bufferAddToEnd(&i2cTransmitBuffer, 0x01);

				i2cNBytes = 22;
				i2cSendStart();
			}
			else
			{
				SendMessage(MSG_B_KEY_PRESSED);
			}
		}
	}
}

