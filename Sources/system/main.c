#include "main.h"

#define RPRINTF_FLOAT

void initPorts() {
	DDRA = 0b11111111;
	PORTA = 0b00000010;
	DDRB = 0b11110111;
	PORTB = 0b11101000;
	DDRC = 0b11010111;
	PORTC = 0b00101011;
	DDRD = 0b11110000;
	PORTD = 0b10001111;
	DDRE = 0b11110100;
	PORTE = 0b00001011;
	DDRF = 0b11111110;
	PORTF = 0b00000101;
	DDRG = 0b11111100;
	PORTG = 0b00000011;
}

int main(void) {
	// Инициализация всех процессов
	initPorts();
	i2cInit();
	InitMessages();
	InitTimers();
	InitFsmDS1307();
	InitFsmErrors();
	InitFsmSpeaker();
	InitFsmKeyboard();
	sei();

	//rprintf("START\n");
	/*
	 i2cAddress = I2C_ADDR_DS1307;
	 i2cNBytes = 0;
	 bufferInit(&i2cTransmitBuffer, i2cTempBuffer, 15);
	 bufferAddToEnd(&i2cTransmitBuffer, 0x00);
	 bufferAddToEnd(&i2cTransmitBuffer, 0x00); // sec
	 bufferAddToEnd(&i2cTransmitBuffer, 0x25); // min
	 bufferAddToEnd(&i2cTransmitBuffer, 0x11); // hour
	 bufferAddToEnd(&i2cTransmitBuffer, 0x05); // week day
	 bufferAddToEnd(&i2cTransmitBuffer, 0x10); // month day
	 bufferAddToEnd(&i2cTransmitBuffer, 0x09); // month
	 bufferAddToEnd(&i2cTransmitBuffer, 0x10); // year
	 i2cSendStart();
	 */
	rprintfInit(putcharTIC218);

	// Главный цикл: здесь крутятся выполняемые одновременно процессы, выполненые как конечные автоматы.
	// В процессах нет ожиданий и длительных циклов, процесс просто запоминает текущее состояние ожидания события и
	// пока прекращает работу, тогда выполняется следующий процесс, и так далее по кругу.
	while (1) {
		ProcessKeyboard(); //  Клавиатура: опрос I2C
		ProcessDebounce(); //  Клавиатура: антидребезг нажатия клавиш
		ProcessClock(); //     Часы
		ProcessScreen(); //    Дисплей
		ProcessBatLevel(); //  Опрос АЦП: уровень заряда аккумулятора
		ProcessDrawer(); //    Открытие денежного ящика
		PrecessFeed(); //      Прокрутка бумаги
		ProcessPaper(); //     Опрос датчика конца бумаги
		ProcessStepMotor(); // Работа шагового двигателя
		ProcessTPH(); //       Работа ТПГ
		ProcessErrors(); //    Процесс для отладки и вывода ошибок куда-то
		ProcessSpeaker(); //   Спикер: короткие и длинные гудки

		// Добавятся:
		ProcessEKLZ(); //      Работа с ЭКЛЗ
		ProcessFM(); //        Работа с фискальной памятью
		ProcessZigbee(); //    Работа с радиоканалом

		/******** Обязательный служебный процесс: ***********/
		ProcessMessages();
		/****************************************************/
	}
}
