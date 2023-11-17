
#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>

static inline void i2c_wait(void)
{
	while (!(TWCR & _BV(TWINT)));
}

void I2C_Init(void)
{
	// F_CPU = 8 MHz, F_I2C = 100 kHz
	TWSR &= ~_BV(TWPS0);
	TWSR &= ~_BV(TWPS1);
	TWBR = 32; // dsa
}

void I2C_Start( void )
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	i2c_wait();
}

void I2C_Stop( void )
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
	while (TWCR & _BV(TWSTO));
}

void I2C_TransmitByte(uint8_t byte)
{
	TWDR = byte;
	TWCR = _BV(TWINT) | _BV(TWEN);
	i2c_wait();
}

void I2C_StartAndTransmitAddress(uint8_t address)
{
	I2C_Start();
	I2C_TransmitByte(address);
}

uint8_t I2C_ReceiveByte_ACK(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	i2c_wait();
	return TWDR;
}

uint8_t I2C_ReceiveByte_NACK(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN);
	i2c_wait();
	return TWDR;
}