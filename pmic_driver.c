#include "pmic_driver.h"
#include "jewelbot.h"
#include <stdint.h>
#include <stdbool.h>
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "SEGGER_RTT.h"

extern nrf_drv_twi_t app_twi_instance;

static volatile bool m_xfer_done = true;
/* Indicates if setting mode operation has ended. */
static volatile bool m_set_mode_done;

static bool pmic_driver_read_reg(uint8_t reg, uint8_t * data)
{
	uint32_t err_code;
	err_code = nrf_drv_twi_tx(&app_twi_instance, PMIC_I2C_ADDRESS, &reg, 1, true); //was TWI_DONT_ISSUE_STOP
	if (err_code != NRF_SUCCESS) {
    SEGGER_RTT_printf(0, "PMIC nrf_drv_twi_tx failed for read register: %u, Error code: %u\n", reg, err_code);
		return false;
	}
	err_code = nrf_drv_twi_rx(&app_twi_instance, PMIC_I2C_ADDRESS, data, 1, false);
  if (err_code != NRF_SUCCESS) {
    SEGGER_RTT_printf(0, "PMIC nrf_drv_twi_rx failed for read register: %u, Error code: %u\n", reg, err_code);
  }
	return (err_code == NRF_SUCCESS);
}

static bool pmic_driver_write_reg(uint8_t reg, uint8_t data)
{
	uint8_t tmp_data[2] = { reg, data };
	uint32_t err_code = nrf_drv_twi_tx(&app_twi_instance, PMIC_I2C_ADDRESS, tmp_data, 2, false);
  if (err_code != NRF_SUCCESS) {
    SEGGER_RTT_printf(0, "PMIC nrf_drv_twi_tx failed for write register: %u, Error code: %u\n", reg, err_code);
  }
	return err_code == NRF_SUCCESS;
}


void pmic_init()
{
#ifndef REAL_JEWELBOT
return;
#endif
  //pmic_turn_off_charging();
	bool success;
	uint8_t val = 0;
	/*success = pmic_driver_read_reg(PMIC_REG_DEFDCDC1, &val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error reading DCDC1 configuration!\n");
		return;
	} */
#define VSYS_REG_4V4                  (0x40)
#define INPUT_DPPM_DISABLED_500MA     (0x20)
#define CHARGE_ENABLED                (0x01)
#define TERM_EN                       (0x02)
#define DYN_TMR_CLOCK_SPEED_REDUCED   (0x04)
#define TH_LOOP_ENABLED               (0x08)
	// To do - make nice defines for all these
  // Configure CHGCONFIG0 register
	// set: 0x40 = Vsys to 4.4V | 0x20 = 500 mA input current, input DPPM disabled | 0xF = default settings
	val = VSYS_REG_4V4 | INPUT_DPPM_DISABLED_500MA | (CHARGE_ENABLED | TERM_EN | DYN_TMR_CLOCK_SPEED_REDUCED | TH_LOOP_ENABLED) ;
	success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing CHGCONFIG0 configuration!\n");
		return;
	}

#define I_PRE0_PRECHARGE_CURRENT_10PERCENT                      (0x40)
#define ICH_SCL1_SCL0_CHARGE_CURRENT_SCALING_FACTOR_25PERCENT   (0x00)
#define I_TERM0_TERM_CURRENT_SCALING_FACTOR_10PERCENT           (0x04)

	// Configure CHGCONFIG1 register
	// set:  0x40 = default precharge current | 0x00 = reduced current scaling | 0x4 = default current termination
	val = I_PRE0_PRECHARGE_CURRENT_10PERCENT | ICH_SCL1_SCL0_CHARGE_CURRENT_SCALING_FACTOR_25PERCENT | I_TERM0_TERM_CURRENT_SCALING_FACTOR_10PERCENT;
	success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG1, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing CHGCONFIG1 configuration!\n");
		return;
	}
#define SFTY_TMR_CHARGE_VALUE_5H    (0x40)
#define NTC_SENSOR_RESISTENCE_10K   (0x08)
#define V_DPPM_VOLTAGE_43V          (0x04)
	// Configure CHGCONFIG2 register
	// set:  0x40 = default safety timer | 0x08 = default sensor resistance | 0x04 = default - DPPM @ 4.3V
	val = SFTY_TMR_CHARGE_VALUE_5H | NTC_SENSOR_RESISTENCE_10K | V_DPPM_VOLTAGE_43V;
	success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG2, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing CHGCONFIG2 configuration!\n");
		return;
	}
#define CH_VLTG1_SET_42V            (0x40)
#define DEFAULT_CHARGE_VOLTAGE      CH_VLTG1_SET_42V
#define DISABLE_BATTERY_COMPARATOR  (0x01)

	// Configure CHGCONFIG3 register
	// set:  0x40 = default charging voltage | 0x01 = disable battery comp (not used for rechargeable battery)
	val = DEFAULT_CHARGE_VOLTAGE | DISABLE_BATTERY_COMPARATOR;
	success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG3, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing CHGCONFIG3 configuration!\n");
		return;
	}

	/* Configure DEFDCDC1 register*/
	//app_trace_log("DCDC 1 Before Write %02x\n", val);
	val = PMIC_DCDC1_ENABLE_MASK | PMIC_DCDC1_VOLT_MASK;
	success = pmic_driver_write_reg(PMIC_REG_DEFDCDC1, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing DCDC1 configuration!\n");
		return;
	}


	val = ~(PMIC_CH_PGOOD | PMIC_CH_ACTIVE);  // enable interrupts for CH_PGOOD and CH_ACTIVE
	success = pmic_driver_write_reg(PMIC_REG_IRMASK0, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing PMIC_REG_IRMASK0 configuration!\n");
		return;
	}

  //uint8_t chgcfg0_val;
  //pmic_enable_dynamic_ppm();
	// Read and print out all registers, are they what we think?
	uint8_t reg_num;
	for (reg_num = 1; reg_num < 19; reg_num++) {
		uint8_t val = 0;
		success = pmic_driver_read_reg(reg_num, &val);
		if(!success) {
			SEGGER_RTT_printf(0, "PMIC read failed for write register: %02x\n", reg_num);
		}
		else {
			SEGGER_RTT_printf(0, "PMIC read register: %u.  Value was:  %02x\n", reg_num, val);
		}
	}

	pmic_clear_interrupts();
}

void pmic_clear_interrupts(void)
{
	bool success;
	uint8_t val;
	do {
		success = pmic_driver_read_reg(PMIC_REG_IR0, &val);
	} while (success && val != 0);
}

void pmic_disable()
{
	bool success;
	uint8_t val;
	success = pmic_driver_read_reg(PMIC_REG_DEFDCDC1, &val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error reading DCDC1 configuration!\n");
		return;
	}

	/* Enable DCDC1 */
	//app_trace_log("DCDC 1 Before Write %02x\n", val);
	val &= !PMIC_DCDC1_ENABLE_MASK;
	success = pmic_driver_write_reg(PMIC_REG_DEFDCDC1, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing DCDC1 configuration!\n");
		return;
	}
}

bool pmic_is_charging(void)
{
#ifdef SIMULATE_CHARGING
  return true;
#elif SIMULATE_BATTERY
  return false;
#endif
	uint8_t success;
	uint8_t val;
	success = pmic_driver_read_reg(PMIC_REG_CHGSTATUS, &val);
	if (!success) {
		SEGGER_RTT_WriteString(0,"Error reading CHGSTATUS register!\n");
		return false;
	}
	if (val & PMIC_CHARGING_MASK) {
		return true;
	}
	return false;
}

bool pmic_5V_present(void)
{
#ifdef SIMULATE_CHARGING
  return true;
#elif SIMULATE_BATTERY
  return false;
#endif

	uint8_t success;
	uint8_t val;
	success = pmic_driver_read_reg(PMIC_REG_CHGSTATUS, &val);
	if (!success) {
		SEGGER_RTT_WriteString(0,"Error reading CHGSTATUS register!\n");
		return false;
	}
	if (val & PMIC_5V_PRESENT_MASK) {
		return true;
	}
	return false;
}

bool pmic_toggle_charging() {
	uint8_t success;
  uint8_t val = 0;

#define VSYS1_SET_5V    (0x80)

  val = VSYS1_SET_5V | INPUT_DPPM_DISABLED_500MA | (CHARGE_ENABLED | TERM_EN | DYN_TMR_CLOCK_SPEED_REDUCED | TH_LOOP_ENABLED);
	success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error writing CHGCONFIG0 configuration!\n");
		return false;
	}
	return true;

  //success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0,
}
bool pmic_turn_off_charging() {
  uint8_t success;
  uint8_t val = 0;

  SEGGER_RTT_WriteString(0, "Toggle Charging\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after read is: %02x\n", val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to read CHGCONFIG0 register!\n");
    return false;
  }
  val &= ~(1 << 0);

  SEGGER_RTT_printf(0, "CHGCONFIG0 val after change is: %02x\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to write CHGCONFIG0 register!\n");
    return false;
  }
  return true;
}

bool pmic_turn_on_charging() {
  uint8_t success;
  uint8_t val = 0;

  SEGGER_RTT_WriteString(0, "Toggle Charging\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after read is: %02x\n", val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to read CHGCONFIG0 register!\n");
    return false;
  }
  val |= 1 << 0;
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after change is: %02x\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to write CHGCONFIG0 register!\n");
    return false;
  }
  return true;
}

bool pmic_enable_dynamic_ppm(void) {
  // 00100000 //default       0x20
  // 00010000 //enabled       0x10
  uint8_t success;
  uint8_t val = 0;
  SEGGER_RTT_WriteString(0, "Turn on Dynamic PPM\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after read is: %02x\n", val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to read CHGCONFIG0 register!\n");
    return false;
  }
  val &= ~((1 << 4) | (1 << 5));
  val |= PMIC_DPPM_ENABLED;
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after change is: %02x\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to write CHGCONFIG0 register!\n");
    return false;
  }
  return true;

}

bool pmic_disable_dynamic_ppm(void) {
 // 00100000 //default       0x20
  // 00010000 //enabled       0x10
  uint8_t success;
  uint8_t val = 0;
  SEGGER_RTT_WriteString(0, "Turn off Dynamic PPM\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after read is: %02x\n", val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to read CHGCONFIG0 register!\n");
    return false;
  }
  val &= ~((1 << 4) | (1 << 5)); //reset bits; then disable
  val |= PMIC_DPPM_DISABLED;
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after change is: %02x\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to write CHGCONFIG0 register!\n");
    return false;
  }
  return true;

}
