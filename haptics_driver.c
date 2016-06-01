#include "haptics_driver.h"
#include "Haptics.h"
#include "Actuator_Waveforms.h"
#include "pmic_driver.h"
#include "nrf_delay.h"

void haptics_init(void)
{
	Haptics_Init();
	Haptics_SetActuator(ACTUATOR_LRA);
	Haptics_RunAutoCal_LRA();
}

unsigned char haptics_test_cal_diags(void)
{
	unsigned char ret;
	Haptics_RunAutoCal_LRA();

	ret = Haptics_Diagnostics(ACTUATOR_LRA);
	ret = (ret & 0x8) >> 3;
	return ret;
}

unsigned char haptics_test_run1(void)
{
#ifdef PMIC
	bool battery_is_charging = pmic_is_charging();
	if (battery_is_charging) {
		return 0;  //  Don't activate the motor if the battery  charging
	}
#endif
	Haptics_SendWaveform(Tick, ACTUATOR_LRA, TRIGGER_INTERNAL);
	return 0; //ret;
}

unsigned char haptics_test_run2(void)
{
#ifdef PMIC
	bool battery_is_charging = pmic_is_charging();
	if (battery_is_charging) {

		return 0;  //  Don't activate the motor if the battery charging
	}
#endif
	Haptics_SendWaveform(BuzzAlert, ACTUATOR_LRA, TRIGGER_INTERNAL);
	return 0; //ret;
}

unsigned char haptics_test_run3(void)
{
	//unsigned char ret;
	//uint32_t err;
#ifdef PMIC
	bool battery_is_charging = pmic_is_charging();
	if (battery_is_charging) {
		return 0;  //  Don't activate the motor if the battery charging
	}
#endif
	Haptics_SendWaveform(DoubleStrongClick, ACTUATOR_LRA, TRIGGER_INTERNAL);
	return 0; //ret;
}

unsigned char haptics_test_run4(void)
{
#ifdef PMIC
	//unsigned char ret;
	//uint32_t err;
	bool battery_is_charging = pmic_is_charging();
	if (battery_is_charging) {
		return 0;  //  Don't activate the motor if the battery charging
	}
#endif
	Haptics_SendWaveform(PulsingAlert, ACTUATOR_LRA, TRIGGER_INTERNAL);
	return 0;
}

