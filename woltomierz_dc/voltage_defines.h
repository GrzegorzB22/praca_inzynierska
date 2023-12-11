/*
 * voltage_defines.h
 *
 * Created: 16.11.2023 23:09:45
 *  Author: Admin
 */ 


#ifndef VOLTAGE_DEFINES_H_
#define VOLTAGE_DEFINES_H_

#include <avr/io.h>

#define LSB_RANGE_200_MV 78125
#define LSB_RANGE_2_V    625
#define LSB_RANGE_20_V   625

#define RANGE_200_MV 1
#define RANGE_2_V    2
#define RANGE_20_V   3

#define RESOLUTION_3_5_DIGITS 1
#define RESOLUTION_4_5_DIGITS 2
#define RESOLUTION_5_5_DIGITS 3

void Voltmeter_RangePinsInit(void);
uint8_t Voltmeter_GetRange(void);
void convertAdcToVoltage(int16_t adc_value, uint8_t range, uint8_t resolution, int16_t *decimal_part, int16_t *fractional_part);


#endif /* VOLTAGE_DEFINES_H_ */
