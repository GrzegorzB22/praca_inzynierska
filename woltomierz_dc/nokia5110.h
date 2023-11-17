
#ifndef NOKIA5110_H_
#define NOKIA5110_H_


#include <avr/io.h>
#include <stdio.h>
#include "ads1115.h"
#include "voltmeter_pins_defines.h"
#include "voltage_defines.h"

#define SPI_PERIOD_US	100

#define PIXEL_SET	1
#define PIXEL_CLEAR	0

#define NOKIA_WIDTH		84
#define NOKIA_HEIGHT	48

#define NOKIA_START_TRANSMIT	NOKIA_CLEAR_PORT(NOKIA_CS_PIN)
#define NOKIA_STOP_TRANSMIT		NOKIA_SET_PORT(NOKIA_CS_PIN)

typedef struct {
	uint8_t buffer[504];
	uint8_t x;
	uint8_t y;
} n5110;


void NOKIA_Init(void);
void NOKIA_Update(void);
void NOKIA_SetContrast(uint8_t contrast);
void NOKIA_SetCursorXY(uint8_t x, uint8_t y);
void NOKIA_ClearBuffer(void);
void NOKIA_PutPixel(uint8_t x, uint8_t y, uint8_t pixel_value);
void NOKIA_WriteChar(char c);
void NOKIA_WriteString(char *s);
void NOKIA_DisplayImage(uint8_t x0, uint8_t y0, uint8_t *image, uint8_t width, uint8_t height);
void NOKIA_DisplayVoltage(int16_t dec, int16_t fra, Voltmeter_ranges range, Voltmeter_resolution resolution);
void NOKIA_DisplayRange(Voltmeter_ranges range);


#endif