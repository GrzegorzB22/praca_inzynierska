
#include "usart.h"
#include <avr/io.h>
#include "voltage_defines.h"
#include "voltmeter_pins_defines.h"
#include <stdbool.h>

#define ABS(x)	((x < 0) ? -(x) : (x))

void USART_Init(void)
{

	/* ustawienie czestotliwosci usart baud rate 9600 */
	#define BAUD	9600
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
		UCSR0A |= _BV(U2X0):
	#else
		UCSR0A &= ~_BV(U2X0);
	#endif
	/*--------------------*/

	UCSR0B |= _BV(TXEN0);						// wlaczenie transmitera
	UCSR0C &= ~(_BV(UMSEL01) | _BV(UMSEL00));	// tryb asynchroniczny
	UCSR0C &= ~(_BV(UPM01) | _BV(UPM00));		// wylaczony bit parzystosci
	UCSR0C &= ~_BV(USBS0);						// 1 bit stopu
	UCSR0C &= ~(_BV(UCSZ01) | _BV(UCSZ00));		// interfejs 8-bit

	BLUETOOTH_CLEAR_DDR(BLUETOOTH_STATUS_PIN);
}

bool Bluetooth_GetStatus(void)
{
	return (BLUETOOTH_READ_PIN(BLUETOOTH_CONNECTED_PIN));
}

void USART_TransmitByte(uint8_t byte)
{
	while( !(UCSR0A & _BV(UDRE0)) );
	UDR0 = byte;
}

void USART_TransmitString(char *s)
{
	while (*s)
		USART_TransmitByte(*s++);
}

void USART_DisplayRange(uint8_t range)
{
	if (range == RANGE_200_MV)	USART_TransmitString("Zakres 200 mV");
	else if (range == RANGE_2_V)	USART_TransmitString("Zakres 2 V");
	else if (range == RANGE_20_V)	USART_TransmitString("Zakres 20 V");
	
	USART_TransmitByte(END_OF_TRANSMISSION_RANGE);
}

void USART_DisplayVoltage(int16_t dec, int16_t fra, uint8_t range, uint8_t resolution)
{
	if (((range == RANGE_200_MV) && (ABS(dec) >= 200)) || ((range == RANGE_2_V) && (ABS(dec) >= 2)) || ((range == RANGE_20_V) && (ABS(dec) >= 20))) {
		USART_TransmitByte('1');
		USART_TransmitByte(END_OF_TRANSMISSION_VOLTAGE);
		return;
	}

	char voltage_string[10];

	switch (range) {
		case RANGE_200_MV:	if (resolution == RESOLUTION_3_5_DIGITS)		sprintf(voltage_string, "%d,%d mV", dec, fra);
							else if (resolution == RESOLUTION_4_5_DIGITS)	sprintf(voltage_string, "%d,%02d mV", dec, fra);
							else if (resolution == RESOLUTION_5_5_DIGITS)	sprintf(voltage_string, "%d,%03d mV", dec, fra);
							break;
							
		case RANGE_2_V:		if (resolution == RESOLUTION_3_5_DIGITS)		sprintf(voltage_string, "%d,%03d V", dec, fra);
							else if (resolution == RESOLUTION_4_5_DIGITS)	sprintf(voltage_string, "%d,%04d V", dec, fra);
							else if (resolution == RESOLUTION_5_5_DIGITS)	sprintf(voltage_string, "%d,%05d V", dec, fra);
							break;
							
		case RANGE_20_V:	if (resolution == RESOLUTION_3_5_DIGITS)		sprintf(voltage_string, "%d,%02d V", dec, fra);
							else if (resolution == RESOLUTION_4_5_DIGITS)	sprintf(voltage_string, "%d,%03d V", dec, fra);
							else if (resolution == RESOLUTION_5_5_DIGITS)	sprintf(voltage_string, "%d,%04d V", dec, fra);
							break;
	}

	USART_TransmitString(voltage_string);
	USART_TransmitByte(END_OF_TRANSMISSION_VOLTAGE);
}
