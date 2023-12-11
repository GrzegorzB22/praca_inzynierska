
#ifndef USART_H_
#define USART_H_

#include <stdio.h>
#include "voltage_defines.h"
#include <stdbool.h>

#define END_OF_TRANSMISSION_RANGE	']'
#define END_OF_TRANSMISSION_VOLTAGE	'['

#define BLUETOOTH_DISCONNECTED 0
#define BLUETOOTH_CONNECTED 1


bool Bluetooth_GetStatus(void);
void USART_Init(void);
void USART_TransmitByte(uint8_t byte);
void USART_TransmitString(char *s);
void USART_DisplayVoltage(int16_t dec, int16_t fra, uint8_t range, uint8_t resolution);
void USART_DisplayRange(uint8_t range);

#endif 
