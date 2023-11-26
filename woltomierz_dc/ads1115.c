
#include "ads1115.h"
#include <avr/io.h>
#include "i2c.h"
#include <stdbool.h>
#include "voltage_defines.h"

static void ads_write16ToRegister(uint8_t I2C_Address, uint8_t reg, uint16_t value)
{
	I2C_StartAndTransmitAddress((I2C_Address << 1) + I2C_WRITE);
	I2C_TransmitByte(reg);
	I2C_TransmitByte((uint8_t)(value >> 8));
	I2C_TransmitByte((uint8_t)(value & 0x00FF));
	I2C_Stop();
}

static uint16_t ads_read16FromRegister(uint8_t I2C_Address, uint8_t reg)
{
	uint16_t register_value;

	I2C_StartAndTransmitAddress((I2C_Address << 1) | I2C_WRITE);
	I2C_TransmitByte(reg);
	I2C_Stop();
	
	I2C_StartAndTransmitAddress((I2C_Address << 1) | I2C_READ);
	register_value = (I2C_ReceiveByte_ACK() << 8);
	register_value |= I2C_ReceiveByte_NACK();
	I2C_Stop();
	
	return register_value;
}

static uint16_t struct_sum(ADS_chip *chip)
{
	uint16_t sum =	chip->comp_lat		|
					chip->comp_mode		|
					chip->comp_pol		|
					chip->comp_que		|
					chip->conv_mode		|
					chip->dr			|
					chip->fsr			|
					chip->conversion	|
					chip->mux			;
					
	return sum;
}

void ADS_Init(ADS_chip *chip, uint8_t ads_address, uint16_t ads_dr)
{
	I2C_Init();	
	Voltmeter_RangePinsInit();

	chip->conversion = ADS_STOP;
	chip->address = ads_address;
	chip->comp_lat = ADS_COMP_LAT_DEFAULT;
	chip->comp_mode = ADS_COMP_MODE_DEFAULT;
	chip->comp_pol = ADS_COMP_POL_DEFAULT;
	chip->comp_que = ADS_COMP_QUE_DEFAULT;
	chip->conv_mode = ADS_CONV_MODE_DEFAULT;
	chip->dr = ads_dr;
	chip->fsr = ADS_FSR_DEFAULT;
	chip->mux = ADS_MUX_DEFAULT;
}

bool ADS_Busy(ADS_chip *chip)
{
	uint16_t reg;
	
	reg = ads_read16FromRegister(chip->address, ADS_CONFIG_REGISTER);
	
	return !((reg & ADS_BUSY_BIT) >> 8);
}

int16_t ADS_GetValue(ADS_chip *chip)
{
	return ads_read16FromRegister(chip->address, ADS_CONVERSION_REGISTER);
}

void ADS_Start_SingleConversion(ADS_chip *chip)
{
	chip->conversion = ADS_START_CONVERSION;
	chip->conv_mode = ADS_SINGLE_MODE;
	ads_write16ToRegister(chip->address, ADS_CONFIG_REGISTER, struct_sum(chip));
}

void ADS_Start_ContinuousConversion(ADS_chip *chip)
{
	chip->conversion = ADS_START_CONVERSION;
	chip->conv_mode = ADS_CONTINUOUS_MODE;
	ads_write16ToRegister(chip->address, ADS_CONFIG_REGISTER, struct_sum(chip));
}

void ADS_Start_It(ADS_chip *chip, uint8_t pin)
{
	chip->comp_pol = ADS_COMP_ACTIVE_HIGH;
	chip->comp_lat = ADS_NONLATCHING_COMP;
	chip->comp_que = ADS_COMP_ASSERT_ONE;
	chip->conv_mode = ADS_CONTINUOUS_MODE;
	chip->conversion = ADS_START_CONVERSION;
	
	if (pin == AT_INTERRUPT_INT0) {
		EICRA = _BV(ISC01);
		EIMSK = _BV(INT0);
	}
	else if (pin == AT_INTERRUPT_INT1) {
		EICRA = _BV(ISC11);
		EIMSK = _BV(INT1);
	}

	ads_write16ToRegister(chip->address, ADS_LoThresh_REGISTER, 0x7FFF);
	ads_write16ToRegister(chip->address, ADS_HiThresh_REGISTER, 0x8000);
	ads_write16ToRegister(chip->address, ADS_CONFIG_REGISTER, struct_sum(chip));
}

void ADS_SET_Address(ADS_chip *chip, uint8_t ads_address)
{
	chip->address = ads_address;
}

void ADS_SET_Mux(ADS_chip *chip, uint16_t ads_mux)
{
	chip->mux = ads_mux;
	ads_write16ToRegister(chip->address, ADS_CONFIG_REGISTER, struct_sum(chip));
}

void ADS_SET_Fsr(ADS_chip *chip, uint16_t ads_fsr)
{
	chip->fsr = ads_fsr;
	ads_write16ToRegister(chip->address, ADS_CONFIG_REGISTER, struct_sum(chip));
}

void ADS_SET_Dr(ADS_chip *chip, uint16_t ads_dr)
{
	chip->dr = ads_dr;
	ads_write16ToRegister(chip->address, ADS_CONFIG_REGISTER, struct_sum(chip));
}
