
#ifndef ADS1115_H_
#define ADS1115_H_

#include <avr/io.h>
#include <stdbool.h>

#define ADS_START_CONVERSION	0x8000
#define ADS_STOP				0x0000
#define ADS_BUSY_BIT			0x8000

typedef enum {
	ADS_CONVERSION_REGISTER = 0x00,
	ADS_CONFIG_REGISTER		= 0x01,
	ADS_LoThresh_REGISTER	= 0x02,
	ADS_HiThresh_REGISTER	= 0x03
} ADS_registers;

typedef enum {
	ADS_AIN0_AIN1	= 0x0000,
	ADS_AIN0_AIN3	= 0x1000,
	ADS_AIN1_AIN3	= 0x2000,
	ADS_AIN2_AIN3	= 0x3000,
	ADS_AIN0_GND	= 0x4000,
	ADS_AIN1_GND	= 0x5000,
	ADS_AIN2_GND	= 0x6000,
	ADS_AIN3_GND    = 0x7000,
	ADS_MUX_DEFAULT	= ADS_AIN0_AIN1
} ADS_mux_settings;

typedef enum {
	ADS_FSR_6_144_V	= 0x0000,
	ADS_FSR_4_096_V	= 0x0200,
	ADS_FSR_2_048_V	= 0x0400,
	ADS_FSR_1_024_V	= 0x0600,
	ADS_FSR_0_512_V	= 0x0800,
	ADS_FSR_0_256_V	= 0x0A00,
	ADS_FSR_DEFAULT = ADS_FSR_2_048_V
} ADS_pga_settings;

typedef enum {
	ADS_DR_8_SPS	= 0x0000,
	ADS_DR_16_SPS	= 0x0020,
	ADS_DR_32_SPS	= 0x0040,
	ADS_DR_64_SPS	= 0x0060,
	ADS_DR_128_SPS	= 0x0080,
	ADS_DR_250_SPS	= 0x00A0,
	ADS_DR_475_SPS	= 0x00C0,
	ADS_DR_860_SPS	= 0x00E0,
	ADS_DR_DEFAULT	= ADS_DR_128_SPS	
} ADS_dr_settings;

typedef enum {
	ADS_TRADITIONAL_COMP  = 0x0000,
	ADS_WINDOW_COMP		  = 0x0010,
	ADS_COMP_MODE_DEFAULT = ADS_TRADITIONAL_COMP
} ADS_comp_mode_settings;

typedef enum {
	ADS_COMP_ACTIVE_LOW     = 0x0000,
	ADS_COMP_ACTIVE_HIGH    = 0x0008,
	ADS_COMP_POL_DEFAULT = ADS_COMP_ACTIVE_LOW
} ADS_comp_pol_settings;

typedef enum {
	ADS_SINGLE_MODE		  = 0x0100,
	ADS_CONTINUOUS_MODE   = 0x0000,
	ADS_CONV_MODE_DEFAULT = ADS_SINGLE_MODE
} ADS_conv_mode_settings;

typedef enum {
	ADS_NONLATCHING_COMP = 0x0000,
	ADS_LATCHING_COMP    = 0x0004,
	ADS_COMP_LAT_DEFAULT = ADS_NONLATCHING_COMP
} ADS_comp_lat_settings;

typedef enum {
	ADS_COMP_ASSERT_ONE  = 0x0000,
	ADS_COMP_ASSERT_TWO  = 0x0001,
	ADS_COMP_ASSERT_FOUR = 0x0002,
	ADS_COMP_DISABLE     = 0x0003,
	ADS_COMP_QUE_DEFAULT = ADS_COMP_DISABLE
} ADS_comp_que_settings;

typedef enum {
	ADS_ADDRESS_GND	= 0x48,
	ADS_ADDRESS_VDD = 0x49,
	ADS_ADDRESS_SDA = 0x4A,
	ADS_ADDRESS_SCL = 0x4B,
	ADS_ADDRESS_DEFAULT = ADS_ADDRESS_GND
} ADS_address_settings;

typedef struct {
	uint16_t conversion;
	ADS_address_settings address;
	ADS_mux_settings mux;
	ADS_pga_settings fsr;
	ADS_dr_settings dr;
	ADS_conv_mode_settings conv_mode;
	ADS_comp_mode_settings comp_mode;
	ADS_comp_pol_settings comp_pol;
	ADS_comp_lat_settings comp_lat;
	ADS_comp_que_settings comp_que;
} ADS_chip;

typedef enum {
	AT_INTERRUPT_INT0 = 0,
	AT_INTERRUPT_INT1 = 1
} AT8_interrupt_pins;


void ADS_Init(ADS_chip *chip, ADS_address_settings ads_address, ADS_dr_settings ads_dr);

bool ADS_Busy(ADS_chip *chip);
int16_t ADS_GetValue(ADS_chip *chip);

void ADS_Start_SingleConversion(ADS_chip *chip);
void ADS_Start_ContinuousConversion(ADS_chip *chip);
void ADS_Start_It(ADS_chip *chip, AT8_interrupt_pins pin);

void ADS_SET_Address(ADS_chip *chip, ADS_address_settings ads_address);
void ADS_SET_Mux(ADS_chip *chip, ADS_mux_settings ads_mux);
void ADS_SET_Fsr(ADS_chip *chip, ADS_pga_settings ads_fsr);
void ADS_SET_Dr(ADS_chip *chip, ADS_dr_settings ads_dr);

#endif