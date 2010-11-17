#include "processClock.h"

void InitFsmDS1307(void) {
	StartTimer(TIMER_SEC);
}
void ProcessClock(void) {
	if (GetTimer(TIMER_SEC) >= sec) {
		StartTimer(TIMER_SEC);

		//ClearScreen();
		DateTimeToString(dateTime, dateTimeStr);
		OutTextXY(5, 10, dateTimeStr, 19, 0);
		SendMessage(MSG_INIT_SCREEN);
		SendMessage(MSG_REFRESH_SCREEN);

		if (!I2C_BUSY) {
			i2cAddress = I2C_ADDR_DS1307;
			i2cNBytes = 9;
			bufferInit(&i2cTransmitBuffer, i2cTransmitBuffer_, 1);
			bufferAddToEnd(&i2cTransmitBuffer, 0);
			bufferInit(&i2cReceiveBuffer, dateTime, 9);
			i2cSendStart();
		}
	}
}

// Строка из 19 символов вида 01/01/2001 10:30:00
void DateTimeToString(u08* dateTime, char* string) {
	string[0] = (dateTime[4] >> 4) + 0x30;
	string[1] = (dateTime[4] & 0x0F) + 0x30;
	string[2] = '/';
	string[3] = (dateTime[5] >> 4) + 0x30;
	string[4] = (dateTime[5] & 0x0F) + 0x30;
	string[5] = '/';
	string[6] = '2';
	string[7] = '0';
	string[8] = (dateTime[6] >> 4) + 0x30;
	string[9] = (dateTime[6] & 0x0F) + 0x30;

	string[10] = ' ';
	string[11] = (dateTime[2] >> 4) + 0x30;
	string[12] = (dateTime[2] & 0x0F) + 0x30;
	string[13] = ':';
	string[14] = (dateTime[1] >> 4) + 0x30;
	string[15] = (dateTime[1] & 0x0F) + 0x30;
	string[16] = ':';
	string[17] = (dateTime[0] >> 4) + 0x30;
	string[18] = (dateTime[0] & 0x0F) + 0x30;
}
