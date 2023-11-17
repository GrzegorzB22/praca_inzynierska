
#ifndef USART_H_
#define USART_H_

#include <stdio.h>
#include "voltage_defines.h"

#define END_OF_TRANSMISSION_RANGE	']'
#define END_OF_TRANSMISSION_VOLTAGE	'['

typedef enum {
	BLUETOOTH_DISCONNECTED = 0,
	BLUETOOTH_CONNECTED =	 1
} Bluetooth_status;

Bluetooth_status Bluetooth_GetStatus(void);
void USART_Init(void);
void USART_TransmitByte(uint8_t byte);
void USART_TransmitString(char *s);
void USART_DisplayVoltage(int16_t dec, int16_t fra, Voltmeter_ranges range, Voltmeter_resolution resolution);
void USART_DisplayRange(Voltmeter_ranges range);

#endif 