
#include "nokia5110.h"
#include "nokia_chars.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "ads1115.h"
#include <stdio.h>
#include "voltage_defines.h"

#define ABS(x) (((x) < 0) ? -(x) : (x))

static void Nokia_Transmit(uint8_t byte)
{
	uint8_t mask = 0x80;
	
	NOKIA_START_TRANSMIT;

	while (mask) {
		if (byte & mask)
			NOKIA_SET_PORT(NOKIA_MOSI_PIN);
		else
			NOKIA_CLEAR_PORT(NOKIA_MOSI_PIN);
		
		NOKIA_SET_PORT(NOKIA_CLK_PIN);
		_delay_us(SPI_PERIOD_US / 2);
		NOKIA_CLEAR_PORT(NOKIA_CLK_PIN);
		_delay_us(SPI_PERIOD_US / 2);
		
		mask >>= 1;
	}
	
	NOKIA_STOP_TRANSMIT;
}

static n5110 nokia_screen = {
	{0}, 0, 0
};

static void Nokia_TransmitCommand(uint8_t command)
{
	NOKIA_CLEAR_PORT(NOKIA_DC_PIN);
	Nokia_Transmit(command);
}

static void Nokia_TransmitData(uint8_t data)
{
	NOKIA_SET_PORT(NOKIA_DC_PIN);
	Nokia_Transmit(data);
}

void NOKIA_Init(void)
{

	NOKIA_SET_DDR(NOKIA_MOSI_PIN);
	NOKIA_SET_DDR(NOKIA_CS_PIN);
	NOKIA_SET_DDR(NOKIA_CLK_PIN);
	NOKIA_SET_DDR(NOKIA_DC_PIN);
	NOKIA_SET_DDR(NOKIA_RST_PIN);
	
	NOKIA_SET_PORT(NOKIA_RST_PIN);
	NOKIA_SET_PORT(NOKIA_CS_PIN);

	_delay_ms(1);
	NOKIA_CLEAR_PORT(NOKIA_RST_PIN);
	_delay_us(10);
	NOKIA_SET_PORT(NOKIA_RST_PIN);

	/* Extended instruction set (H = 1) */
	Nokia_TransmitCommand(0x21); // Enter extended instruction set, horizontal addressing (V = 0), chip in power-down mode (PD = 1)
	Nokia_TransmitCommand(0x80); // Set default contrast (Vop = 48)
	Nokia_TransmitCommand(0x04); // Set default temperature coefficient (Tcx = 0)
	Nokia_TransmitCommand(0x14); // Set default bias system (Bs2 = 1)
	
	/* Basic instruction set (H = 0) */
	Nokia_TransmitCommand(0x20); // Enter basic instruction set, horizontal addressing (V = 0), chip in power-down mode (PD = 1)
	Nokia_TransmitCommand(0x0C); // display normal mode (D = 1, E = 0)
	NOKIA_ClearBuffer();
	NOKIA_Update();
}

void NOKIA_SetContrast(uint8_t contrast)
{
	Nokia_TransmitCommand(0x21);
	Nokia_TransmitCommand(0x80 | contrast);
	Nokia_TransmitCommand(0x20);
}

void NOKIA_SetCursorXY(uint8_t x, uint8_t y)
{
	nokia_screen.x = x;
	nokia_screen.y = y;
}

void NOKIA_ClearBuffer(void)
{
	nokia_screen.x = nokia_screen.y = 0;
	for (int i = 0; i < 504; i++)
	nokia_screen.buffer[i] = 0;
}

void NOKIA_PutPixel(uint8_t x, uint8_t y, uint8_t pixel_value)
{
	uint8_t offset = y % 8;
	nokia_screen.buffer[x + (y/8) * NOKIA_WIDTH] |= (_BV(offset) * pixel_value);
}

void NOKIA_Update(void)
{
	Nokia_TransmitCommand(0x80);
	Nokia_TransmitCommand(0x40);
	
	for (int i = 0; i < 504; i++)
		Nokia_TransmitData(nokia_screen.buffer[i]);
}

void NOKIA_WriteChar(char c)
{
	uint8_t x, y;

	for (x = 0; x < 5; x++) {
		for (y = 0; y < 8; y++) {
			if (CHARS[c - 32][x] & (1 << y))
			NOKIA_PutPixel(nokia_screen.x + x, nokia_screen.y + y, PIXEL_SET);
			else
			NOKIA_PutPixel(nokia_screen.x + x, nokia_screen.y + y, PIXEL_CLEAR);
		}
	}
	
	if ((nokia_screen.x + 5) <= NOKIA_WIDTH)
		nokia_screen.x += 5;
	else {
		nokia_screen.x = 0;
		if ((nokia_screen.y + 7) <= NOKIA_HEIGHT)
			nokia_screen.y += 7;
		else
			nokia_screen.x = nokia_screen.y = 0;
	}
}

void NOKIA_WriteString(char *s)
{
	while (*s)
	NOKIA_WriteChar(*s++);
}

void NOKIA_DisplayImage(uint8_t x0, uint8_t y0, uint8_t *image, uint8_t width, uint8_t height)
{
	uint8_t x, y, byteWidth;
	byteWidth = (width + 7) / 8;
	
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			if (BLUETOOTH_ICON[y * byteWidth + x/8] & (128 >> (x & 7)))
			NOKIA_PutPixel(nokia_screen.x + x + x0, nokia_screen.y + y + y0, PIXEL_SET);
			else
			NOKIA_PutPixel(nokia_screen.x + x + x0, nokia_screen.y + y + y0, PIXEL_CLEAR);
		}
	}
}

void NOKIA_DisplayVoltage(int16_t dec, int16_t fra, Voltmeter_ranges range, Voltmeter_resolution resolution)
{
	if (((range == RANGE_200_MV) && (ABS(dec) >= 200)) || ((range == RANGE_2_V) && (ABS(dec) >= 2)) || ((range == RANGE_20_V) && (ABS(dec) >= 20))) {
		NOKIA_SetCursorXY(NOKIA_WIDTH/2 - 3, NOKIA_HEIGHT/2 - 4);
		NOKIA_WriteChar('1');
		return;
	}

	char voltage_string[10];
	NOKIA_SetCursorXY(NOKIA_WIDTH/2 - 15, NOKIA_HEIGHT/2 - 4);

	switch (range) {
		case RANGE_200_MV:	if (resolution == RESOLUTION_3_5_DIGITS)		sprintf(voltage_string, "%d,%d mV", dec, fra);
							else if (resolution == RESOLUTION_4_5_DIGITS)	sprintf(voltage_string, "%d,%02d mV", dec, fra);
							else if (resolution == RESOLUTION_5_5_DIGITS)	sprintf(voltage_string, "%d,%03d mV", dec, fra);
							break;
		
		case RANGE_2_V:		if (resolution == RESOLUTION_3_5_DIGITS)		sprintf(voltage_string, "%d,%03d V", dec, fra);
							else if (resolution == RESOLUTION_4_5_DIGITS)	sprintf(voltage_string, "%d,%04d V", dec, fra);
							else if (resolution == RESOLUTION_5_5_DIGITS)	sprintf(voltage_string, "%d,%05d V", dec, fra);
							break;
							
		case RANGE_20_V:	if (resolution == RESOLUTION_3_5_DIGITS)		sprintf(voltage_string, "%d,%03d V", dec, fra);
							else if (resolution == RESOLUTION_4_5_DIGITS)	sprintf(voltage_string, "%d,%04d V", dec, fra);
							else if (resolution == RESOLUTION_5_5_DIGITS)	sprintf(voltage_string, "%d,%05d V", dec, fra);
							break;
	}

	NOKIA_WriteString(voltage_string);
}

void NOKIA_DisplayRange(Voltmeter_ranges range)
{
	NOKIA_SetCursorXY(NOKIA_WIDTH/2 - 30, NOKIA_HEIGHT - 7);
	if (range == RANGE_200_MV)		NOKIA_WriteString("Zakres 200 mV");
	else if (range == RANGE_2_V)	NOKIA_WriteString("Zakres 2 V");
	else if (range == RANGE_20_V)	NOKIA_WriteString("Zakres 20 V");
}