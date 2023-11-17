
#ifndef VOLTMETER_PINS_DEFINES_H_
#define VOLTMETER_PINS_DEFINES_H_

#define CONCATENATE(x, y)	(x ## y)
#define SETx(dp, dp_name, dp_pin_number)			(CONCATENATE(dp, dp_name) |= _BV(dp_pin_number))
#define CLEARx(dp, dp_name, dp_pin_number)			(CONCATENATE(dp, dp_name) &= ~_BV(dp_pin_number))
#define READx(dp, dp_name, dp_pin_number)			(CONCATENATE(dp, dp_name) & _BV(dp_pin_number))

/*--------------------------- NOKIA ---------------------------*/
// Port name, Pin number
#define NOKIA_CLK_PIN	D, 3
#define NOKIA_MOSI_PIN	D, 4
#define NOKIA_CS_PIN	D, 6
#define NOKIA_DC_PIN	D, 5
#define NOKIA_RST_PIN	D, 7

#define NOKIA_SET_DDR(x)		SETx(DDR, x)
#define NOKIA_SET_PORT(x)		SETx(PORT, x)

#define NOKIA_CLEAR_DDR(x)		CLEARx(DDR, x)
#define NOKIA_CLEAR_PORT(x)		CLEARx(PORT, x)
/*-------------------------------------------------------------*/

/*------------------------- VOLTMETER RANGE PINS --------------*/

#define RANGE_200_MV_PIN	B, 4
#define RANGE_2_V_PIN		B, 3
#define RANGE_20_V_PIN		B, 2

#define RANGE_SET_DDR(x)		SETx(DDR, x)
#define RANGE_SET_PORT(x)		SETx(PORT, x)

#define RANGE_CLEAR_DDR(x)		CLEARx(DDR, x)
#define RANGE_CLEAR_PORT(x)		CLEARx(PORT, x)

#define RANGE_READ_PIN(x)		READx(PIN, x)
/*-------------------------------------------------------------*/

/*------------------------ BLUETOOTH CONNECTED PIN -----------------*/
#define BLUETOOTH_CONNECTED_PIN	D, 2

#define BLUETOOTH_SET_DDR(x)	SETx(DDR, x)
#define BLUETOOTH_CLEAR_DDR(x)	CLEARx(DDR, x)

#define BLUETOOTH_SET_PORT(x)	SETx(PORT, x)
#define BLUETOOTH_CLEAR_PORT(x)	CLEARx(PORT, x)

#define BLUETOOTH_READ_PIN(x)	READx(PIN, x)
/*-------------------------------------------------------------------*/

#endif 