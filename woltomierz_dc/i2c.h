
#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

#define I2C_WRITE	0
#define I2C_READ	1

void I2C_Start(void);
void I2C_Init(void);
void I2C_Stop(void);
void I2C_TransmitByte(uint8_t byte);
uint8_t I2C_ReceiveByte_ACK(void);
uint8_t I2C_ReceiveByte_NACK(void);
void I2C_StartAndTransmitAddress(uint8_t address);



#endif 