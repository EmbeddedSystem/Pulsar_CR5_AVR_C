#include "processScreen.h"
u08 init[] =
{ 0b00000000, // control byte
		0b00000001, // на основн стр
		0X12, // PD = 0, V = 1; вертикальная адресация
		0x0E, //
		0X12, // BIAS = 1/9
		0X06, // D = 1, E = 0;
		0X84, // MUX = 1/65
		0b00001010, // MX = 0, MY = 1;
		0x01, // на основн стр
		0X0D, //
		0X04, //
		0X09, // S[1;0] = 01, mul factor = 3
		175, // установка напряжения B4
		0X05, // PRS = 0, Vlcd programing range
		0X0A, // S[1;0] = 10, mul factor = 4
		0X12, //
		0X0B, // S[1;0] = 11, mul factor = 5
		0x01, // на основн стр
		0X0B, //
		0X05, // DM = 1
		0X58, // TRS = 1, BRS = 1;
		0b00100001, 0b00000001, 0b00000000, /* control byte */
		0b00000001, /* на основн стр */
		0b01000000, /* Y = 0 */
		0b10000000 /* X = 0 */}; // на основн стр


struct
{
	u08 command[1];
	u08 XY[133 * 64 / 8];
} lcdData =
{
{ 0b01000000 },
{ } };

void InitFsmTIC218()
{

}

void ProcessScreen()
{
	if (GetMessage(MSG_INIT_SCREEN))
	{
		if (!I2C_BUSY)
		{
			bufferInit(&i2cTransmitBuffer, init, sizeof(init));
			i2cTransmitBuffer.datalength = sizeof(init);
			i2cNBytes = 0;
			i2cAddress = I2C_ADDR_TIC218;
			i2cSendStart();
		}
		else
		{
			SendMessage(MSG_INIT_SCREEN);
		}
	}
	if (GetMessage(MSG_REFRESH_SCREEN))
	{
		if (!I2C_BUSY)
		{
			bufferInit(&i2cTransmitBuffer, &lcdData, sizeof(lcdData));
			i2cTransmitBuffer.datalength = sizeof(lcdData);
			i2cNBytes = 0;
			i2cAddress = I2C_ADDR_TIC218;
			i2cSendStart();
		}
		else
		{
			SendMessage(MSG_REFRESH_SCREEN);
		}
	}
}
void PutPixel(unsigned int xp, unsigned int yp)
{
	if (133 > xp && 64 > yp)
		lcdData.XY[((xp << 3) + (yp >> 3))] |= 128 >> (yp & 7);
}

void ClearPixel(unsigned int xp, unsigned int yp)
{
	if (133 > xp && 64 > yp)
		lcdData.XY[((xp << 3) + (yp >> 3))] &= ~(128 >> (yp & 7));
}

void ClearScreen()
{
	int i;
	for (i = 0; i < sizeof(lcdData.XY) / sizeof(unsigned char); i++)
		lcdData.XY[i] = 0;
}

void OutTextXY(int X, int Y,//координаты
		char* str,//строка вывода
		int n, //количество символов
		unsigned char inv)//инвертирование символов
{
	unsigned char textL, Put;
	int a;

	for (a = 0; (a < n); a++)
	{
		if (str[a] == 0)
			return;
		if (str[a] < 0x90)
			textL = 0x20;
		else
			textL = 0x60;
		//if (X>(133-5)){X=0;Y=Y+8;}//перевод каретки, определить по разрещению ЖКИ
		int i;
		for (i = 0; i < 5; i++)
		{
			Put = pgm_read_byte(&sym[str[a] - textL][i]) ^ inv;

			if ((Put & 1) > 0)
				PutPixel(X, Y);
			else
				ClearPixel(X, Y);
			if ((Put & 2) > 0)
				PutPixel(X, Y + 1);
			else
				ClearPixel(X, Y + 1);

			if ((Put & 4) > 0)
				PutPixel(X, Y + 2);
			else
				ClearPixel(X, Y + 2);

			if ((Put & 8) > 0)
				PutPixel(X, Y + 3);
			else
				ClearPixel(X, Y + 3);

			if ((Put & 16) > 0)
				PutPixel(X, Y + 4);
			else
				ClearPixel(X, Y + 4);

			if ((Put & 32) > 0)
				PutPixel(X, Y + 5);
			else
				ClearPixel(X, Y + 5);

			if ((Put & 64) > 0)
				PutPixel(X, Y + 6);
			else
				ClearPixel(X, Y + 6);

			if ((Put & 128) > 0)
				PutPixel(X, Y + 7);
			else
				ClearPixel(X, Y + 7);

			X++;
		}
		Put = (0 ^ inv);
		if ((Put & 1) > 0)
			PutPixel(X, Y);
		else
			ClearPixel(X, Y);

		if ((Put & 2) > 0)
			PutPixel(X, Y + 1);
		else
			ClearPixel(X, Y + 1);

		if ((Put & 4) > 0)
			PutPixel(X, Y + 2);
		else
			ClearPixel(X, Y + 2);

		if ((Put & 8) > 0)
			PutPixel(X, Y + 3);
		else
			ClearPixel(X, Y + 3);

		if ((Put & 16) > 0)
			PutPixel(X, Y + 4);
		else
			ClearPixel(X, Y + 4);

		if ((Put & 32) > 0)
			PutPixel(X, Y + 5);
		else
			ClearPixel(X, Y + 5);

		if ((Put & 64) > 0)
			PutPixel(X, Y + 6);
		else
			ClearPixel(X, Y + 6);

		if ((Put & 128) > 0)
			PutPixel(X, Y + 7);
		else
			ClearPixel(X, Y + 7);

		X++;
	}
}
;

void putcharTIC218(unsigned char c)
{
	static int p;
	printStr[p++] = c;
	if (p >= 22)
		p = 0;
	OutTextXY(1, 1, printStr, 22, 0);
	SendMessage(MSG_REFRESH_SCREEN);
}
