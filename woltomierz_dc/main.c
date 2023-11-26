
#include <avr/io.h>
#include "nokia5110.h"
#include "ads1115.h"
#include <avr/interrupt.h>
#include "voltage_defines.h"
#include "usart.h"
#include <util/delay.h>

#define FILTER_LENGTH	100
#define VOLTMETER_RESOLUTION	RESOLUTION_3_5_DIGITS
#define INTERRUPT_PIN			AT_INTERRUPT_INT0

static volatile int16_t filter_average;
static ADS_chip ads1115;

ISR(INT0_vect)
{
	static uint8_t filter_index;
	static int32_t filter_sum;
	static int16_t filter_array[FILTER_LENGTH];

	int16_t adc = ADS_GetValue(&ads1115);

	filter_sum = filter_sum - filter_array[filter_index];
	filter_array[filter_index++] = adc;
	filter_sum += adc;
	filter_index %= FILTER_LENGTH;
	filter_average = filter_sum / (int16_t)FILTER_LENGTH;
}

int main(void)
{
	int16_t voltage_dec = 0;
	int16_t voltage_fra = 0;

	uint8_t range;
	uint8_t last_range = 0xFF;

	NOKIA_Init();
	USART_Init();
	ADS_Init(&ads1115, ADS_ADDRESS_GND, ADS_DR_128_SPS);
	ADS_Start_It(&ads1115, INTERRUPT_PIN);
	sei();

    while (1) 
    {
		NOKIA_ClearBuffer();	
		
		range = Voltmeter_GetRange();

		if (range != last_range) {
			switch (range) {
				case RANGE_200_MV:	ADS_SET_Fsr(&ads1115, ADS_FSR_0_256_V);
									ADS_SET_Mux(&ads1115, ADS_AIN0_AIN3);
									break;
				case RANGE_2_V:		ADS_SET_Fsr(&ads1115, ADS_FSR_2_048_V);
									ADS_SET_Mux(&ads1115, ADS_AIN0_AIN3);
									break;
				case RANGE_20_V:	ADS_SET_Fsr(&ads1115, ADS_FSR_2_048_V);
									ADS_SET_Mux(&ads1115, ADS_AIN1_AIN3);
									break;
			}
			last_range = range;
		}

		convertAdcToVoltage(filter_average, range, VOLTMETER_RESOLUTION, &voltage_dec, &voltage_fra);

		if (Bluetooth_GetStatus() == BLUETOOTH_CONNECTED) {
			USART_DisplayRange(range);
			USART_DisplayVoltage(voltage_dec, voltage_fra, range, RESOLUTION_3_5_DIGITS);
		}
		NOKIA_DisplayRange(range);
		NOKIA_DisplayVoltage(voltage_dec, voltage_fra, range, VOLTMETER_RESOLUTION);
		NOKIA_Update();
    }
}
