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
/*ret_code_t nrf_drv_twi_tx(nrf_drv_twi_t const * const p_instance,
                          uint8_t                     address,
                          uint8_t const *             p_data,
                          uint32_t                    length,
                          bool                        xfer_pending);
													*/
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
  //pmic_turn_off_charging();
	bool success;
	uint8_t val = 0;
	success = pmic_driver_read_reg(PMIC_REG_DEFDCDC1, &val);
	if(!success) {
		SEGGER_RTT_WriteString(0,"Error reading DCDC1 configuration!\n");
		return;
	}

	/* Enable DCDC1 */
	//app_trace_log("DCDC 1 Before Write %02x\n", val);
	val |= PMIC_DCDC1_ENABLE_MASK;
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
  
  SEGGER_RTT_WriteString(0, "Toggle Charging\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after read is: %02x\n", val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to read CHGCONFIG0 register!\n");
    return false;
  }
  val = val ^ (1<<0);
  SEGGER_RTT_printf(0, "CHGCONFIG0 val after change is: %02x\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    SEGGER_RTT_WriteString(0, "failed to write CHGCONFIG0 register!\n");
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
