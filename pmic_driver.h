#ifndef _PMIC_DRIVER_H_
#define _PMIC_DRIVER_H_
#include <stdint.h>
#include <stdbool.h>
#include "jewelbot.h"
#include "nrf_drv_twi.h"


#define PMIC_REG_CHGSTATUS		    (0x01)
#define PMIC_REG_DEFDCDC1         (0x07)
#define PMIC_CONTROL1_REG         (0x0A)
#define PMIC_REG_IR0		          (0x10)
#define PMIC_REG_IR1		          (0x11)
#define PMIC_REG_IR2		          (0x12)
#define PMIC_REG_IRMASK0          (0x0D)
#define PMIC_REG_IRMASK2          (0x0F)

#define PMIC_PB_STAT_MASK         (0x02)
#define PMIC_5V_PRESENT_MASK	    (0x04)
#define PMIC_CHARGING_MASK		    (0x08)
#define PMIC_DCDC1_ENABLE_MASK    (0x80)


#define PMIC_CH_PGOOD             (0x04)
#define PMIC_CH_ACTIVE            (0x08)
#define PMIC_PB_STAT              (0x10)

void pmic_init(void);
void pmic_disable(void);
bool pmic_is_charging(void);
bool pmic_5V_present(void);
void pmic_clear_interrupts(void);

#endif
