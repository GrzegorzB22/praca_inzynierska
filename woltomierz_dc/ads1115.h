
#ifndef ADS1115_H_
#define ADS1115_H_

#include <avr/io.h>
#include <stdbool.h>

#define ADS_START_CONVERSION	0x8000
#define ADS_STOP				0x0000
#define ADS_BUSY_BIT			0x8000

#define ADS_CONVERSION_REGISTER 0x00
#define ADS_CONFIG_REGISTER     0x01
#define ADS_LoThresh_REGISTER   0x02
#define ADS_HiThresh_REGISTER   0x03

#define ADS_AIN0_AIN1	 0x0000
#define ADS_AIN0_AIN3	 0x1000
#define ADS_AIN1_AIN3	 0x2000
#define ADS_AIN2_AIN3	 0x3000
#define ADS_AIN0_GND	 0x4000
#define ADS_AIN1_GND	 0x5000
#define ADS_AIN2_GND	 0x6000
#define ADS_AIN3_GND     0x7000
#define ADS_MUX_DEFAULT	 ADS_AIN0_AIN1

#define ADS_FSR_6_144_V 0x0000
#define ADS_FSR_4_096_V 0x0200
#define ADS_FSR_2_048_V 0x0400
#define ADS_FSR_1_024_V 0x0600
#define ADS_FSR_0_512_V 0x0800
#define ADS_FSR_0_256_V 0x0A00
#define ADS_FSR_DEFAULT ADS_FSR_2_048_V

#define ADS_DR_8_SPS   0x0000
#define ADS_DR_16_SPS  0x0020
#define ADS_DR_32_SPS  0x0040
#define ADS_DR_64_SPS  0x0060
#define ADS_DR_128_SPS 0x0080
#define ADS_DR_250_SPS 0x00A0
#define ADS_DR_475_SPS 0x00C0
#define ADS_DR_860_SPS 0x00E0
#define ADS_DR_DEFAULT ADS_DR_128_SPS

#define ADS_TRADITIONAL_COMP  0x0000
#define ADS_WINDOW_COMP       0x0010
#define ADS_COMP_MODE_DEFAULT ADS_TRADITIONAL_COMP

#define ADS_COMP_ACTIVE_LOW  0x0000
#define ADS_COMP_ACTIVE_HIGH 0x0008
#define ADS_COMP_POL_DEFAULT ADS_COMP_ACTIVE_LOW

#define ADS_SINGLE_MODE       0x0100
#define ADS_CONTINUOUS_MODE   0x0000
#define ADS_CONV_MODE_DEFAULT ADS_SINGLE_MODE

#define ADS_NONLATCHING_COMP 0x0000
#define ADS_LATCHING_COMP    0x0004
#define ADS_COMP_LAT_DEFAULT ADS_NONLATCHING_COMP

#define ADS_COMP_ASSERT_ONE  0x0000
#define ADS_COMP_ASSERT_TWO  0x0001
#define ADS_COMP_ASSERT_FOUR 0x0002
#define ADS_COMP_DISABLE     0x0003
#define ADS_COMP_QUE_DEFAULT ADS_COMP_DISABLE

#define ADS_ADDRESS_GND     0x48
#define ADS_ADDRESS_VDD     0x49
#define ADS_ADDRESS_SDA     0x4A
#define ADS_ADDRESS_SCL     0x4B
#define ADS_ADDRESS_DEFAULT ADS_ADDRESS_GND

#define AT_INTERRUPT_INT0 0
#define AT_INTERRUPT_INT1 1

typedef struct {
	uint8_t address;
	uint16_t conversion;
	uint16_t mux;
	uint16_t fsr;
	uint16_t dr;
	uint16_t conv_mode;
	uint16_t comp_mode;
	uint16_t comp_pol;
	uint16_t comp_lat;
	uint16_t comp_que;
} ADS_chip;

void ADS_Init(ADS_chip *chip, uint8_t ads_address, uint16_t ads_dr);

bool ADS_Busy(ADS_chip *chip);
int16_t ADS_GetValue(ADS_chip *chip);

void ADS_Start_SingleConversion(ADS_chip *chip);
void ADS_Start_ContinuousConversion(ADS_chip *chip);
void ADS_Start_It(ADS_chip *chip, uint8_t pin);

void ADS_SET_Address(ADS_chip *chip, uint8_t ads_address);
void ADS_SET_Mux(ADS_chip *chip, uint16_t ads_mux);
void ADS_SET_Fsr(ADS_chip *chip, uint16_t ads_fsr);
void ADS_SET_Dr(ADS_chip *chip, uint16_t ads_dr);

#endif