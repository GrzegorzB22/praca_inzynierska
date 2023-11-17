/*
 * voltage_defines.h
 *
 * Created: 16.11.2023 23:09:45
 *  Author: Admin
 */ 


#ifndef VOLTAGE_DEFINES_H_
#define VOLTAGE_DEFINES_H_

#include <avr/io.h>

typedef enum {
	LSB_RANGE_200_MV = 78125,
	LSB_RANGE_2_V	 = 625,
	LSB_RANGE_20_V	 = 625
} Voltmeter_lsb_values;

typedef enum {
	RANGE_200_MV = 1,
	RANGE_2_V	 = 2,
	RANGE_20_V	 = 3,
} Voltmeter_ranges;

typedef enum {
	RESOLUTION_3_5_DIGITS = 1,
	RESOLUTION_4_5_DIGITS = 2,
	RESOLUTION_5_5_DIGITS = 3
} Voltmeter_resolution;


void Voltmeter_RangePinsInit(void);
Voltmeter_ranges Voltmeter_GetRange(void);
void convertAdcToVoltage(int16_t adc_value, Voltmeter_ranges range, Voltmeter_resolution resolution, int16_t *decimal_part, int16_t *fractional_part);


#endif /* VOLTAGE_DEFINES_H_ */