
#include "voltage_defines.h"
#include "voltmeter_pins_defines.h"
#include <avr/io.h>

#define ABS(x) (((x) < 0) ? -(x) : (x))

void Voltmeter_RangePinsInit(void)
{
	RANGE_CLEAR_DDR(RANGE_200_MV_PIN);
	RANGE_CLEAR_DDR(RANGE_2_V_PIN);
	RANGE_CLEAR_DDR(RANGE_20_V_PIN);

	RANGE_SET_PORT(RANGE_200_MV_PIN);
	RANGE_SET_PORT(RANGE_2_V_PIN);
	RANGE_SET_PORT(RANGE_20_V_PIN);
}

uint8_t Voltmeter_GetRange(void)
{
	if (RANGE_READ_PIN(RANGE_200_MV_PIN) == 0)	return RANGE_200_MV;
	else if (RANGE_READ_PIN(RANGE_2_V_PIN) == 0)	return RANGE_2_V;
	else if (RANGE_READ_PIN(RANGE_20_V_PIN) == 0)	return RANGE_20_V;
}

void convertAdcToVoltage(int16_t adc_value, uint8_t range, uint8_t resolution, int16_t *decimal_part, int16_t *fractional_part)
{
	if (range == RANGE_200_MV) {
		int64_t voltage = (int64_t)adc_value * LSB_RANGE_200_MV;

		*decimal_part = voltage / 10000000;

		if (resolution == RESOLUTION_3_5_DIGITS)		*fractional_part = (voltage / 1000000) % 10;
		else if (resolution == RESOLUTION_4_5_DIGITS)	*fractional_part = (voltage / 10000) % 100;
		else if (resolution == RESOLUTION_5_5_DIGITS)	*fractional_part = (voltage / 1000) % 1000;
	}
	else if (range == RANGE_2_V) {
		int32_t voltage = (int32_t)adc_value * LSB_RANGE_2_V;

		*decimal_part = voltage / 10000000;

		if (resolution == RESOLUTION_3_5_DIGITS)		*fractional_part = (voltage / 10000) % 1000;
		else if (resolution == RESOLUTION_4_5_DIGITS)	*fractional_part = (voltage / 1000) % 10000;
		else if (resolution == RESOLUTION_5_5_DIGITS)	*fractional_part = (voltage / 100) % 100000;
	}
	else if (range == RANGE_20_V) {
		int32_t voltage = (int32_t)adc_value * LSB_RANGE_20_V;

		*decimal_part = voltage / 1000000;
		
		if (resolution == RESOLUTION_3_5_DIGITS)		*fractional_part = (voltage / 10000) % 10;
		else if (resolution == RESOLUTION_4_5_DIGITS)	*fractional_part = (voltage / 1000) % 100;
		else if (resolution == RESOLUTION_5_5_DIGITS)	*fractional_part = (voltage / 100) % 1000;
	}
	*fractional_part = ABS(*fractional_part);
}
