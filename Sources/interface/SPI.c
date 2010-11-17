#include "SPI.h"

void spiInitTPH(void)
{
	sbi(PORTB, 1); // set SCK
	sbi(DDRB, 1); // set SCK as output
	cbi(DDRB, 3); // set MISO as input
	sbi(DDRB, 2); // set MOSI as output
	sbi(DDRB, 0); // SS must be output for Master mode to work
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << DORD);
	inb(SPSR);
}

void spiInitZigbee(void)
{
	sbi(PORTB, 1); // set SCK
	sbi(DDRB, 1); // set SCK as output
	cbi(DDRB, 3); // set MISO as input
	sbi(DDRB, 2); // set MOSI as output
	sbi(DDRB, 0); // SS must be output for Master mode to work
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA) | (1 << SPR1)
			| (1 << SPR0);
	inb(SPSR);
}

void spiSendByte(u08 data)
{
	SPDR = data;
	while (!(inb(SPSR) & (1 << SPIF)))
	{
	};
}

u08 spiTransferByte(u08 data)
{
	// send the given data
	outb(SPDR, data);
	// wait for transfer to complete
	while (!(inb(SPSR) & (1 << SPIF)))
	{
	};
	return inb(SPDR);
}

/*int MirrorByte(int s)
 {
 s = ((s >> 1) & 0x55) | ((s << 1) & 0xaa);
 s = ((s >> 2) & 0x33) | ((s << 2) & 0xcc);
 s = ((s >> 4) & 0x0f) | ((s << 4) & 0xf0);
 return s;
 }*/
