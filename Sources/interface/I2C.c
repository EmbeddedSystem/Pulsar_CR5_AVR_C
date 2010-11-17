#include "i2c.h"

void i2cInit(void)
{
	// Подтягивающие резисторы включены
	sbi(PORTD, 0); // i2c SCL on ATmega128
	sbi(PORTD, 1); // i2c SDA on ATmega128
	// Скорость 100 KHz
	outb(TWBR, 0x42);
	sbi(TWCR, TWEN);
	sbi(TWCR, TWIE);
}

inline void i2cSendStart(void)
{
	i2cBusy = 1;
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWSTA));
}

inline void i2cSendStop(void)
{
	i2cBusy = 0;
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA)|BV(TWSTO));
}

inline void i2cSendByte(u08 data)
{
	outb(TWDR, data);
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
}

inline void i2cReceiveByteAck()
{
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
}

inline void i2cReceiveByteNack()
{
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
}

ISR(TWI_vect)
{
	static u08 i2cStatus;
	i2cStatus = inb(TWSR) & TWSR_STATUS_MASK;

	switch (i2cStatus)
	{
	case TW_START: // 0x08
		if (i2cTransmitBuffer.datalength)
		{
			i2cSendByte(i2cAddress & 0xFE); // R/~W = 0
		}
		else if (i2cNBytes)
		{
			i2cSendByte(i2cAddress | 0x01); // R/~W = 1
		}
		else
		{
			i2cSendStop();
			SendMessage(MSG_B_I2C_OK);
		}
		break;
	case TW_REP_START: // 0x10
		i2cSendByte(i2cAddress | 0x01); // R/~W = 1
		break;
	case TW_MT_SLA_ACK: // 0x18
	case TW_MT_DATA_ACK: // 0x28
		if (i2cTransmitBuffer.datalength)
		{
			i2cSendByte(bufferGetFromFront(&i2cTransmitBuffer));
		}
		else if (i2cNBytes)
		{
			i2cSendStart();
		}
		else
		{
			i2cSendStop();
			SendMessage(MSG_B_I2C_OK);
		}
		break;
	case TW_MR_DATA_NACK: // 0x58
		bufferAddToEnd(&i2cReceiveBuffer, inb(TWDR));
		SendMessage(MSG_B_I2C_OK);
		i2cSendStop();
		break;
	case TW_MR_SLA_NACK: // 0x48
	case TW_MT_SLA_NACK: // 0x20
		SendMessage(MSG_B_I2C_ERROR_NODEV);
		i2cSendStop();
		break;
	case TW_MT_DATA_NACK: // 0x30
		SendMessage(MSG_B_I2C_ERROR_DATA_NACK);
		i2cSendStop();
		break;
	case TW_MT_ARB_LOST: // 0x38
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
		SendMessage(MSG_B_I2C_ERROR_HARD);
		i2cBusy = 0;
		break;
	case TW_MR_DATA_ACK: // 0x50:
		bufferAddToEnd(&i2cReceiveBuffer, inb(TWDR));
		if (--i2cNBytes)
		{
			i2cReceiveByteAck();
		}
		else
		{
			i2cReceiveByteNack();
		}
		break;
	case TW_MR_SLA_ACK: // 0x40
		if (--i2cNBytes)
		{
			i2cReceiveByteAck();
		}
		else
		{
			i2cReceiveByteNack();
		}
		break;
	default:
	case TW_NO_INFO: // 0xF8
		break;
	case 0x00: // 0x00
		i2cBusy = 0;
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWSTO)|BV(TWEA));
		break;
	}
}

