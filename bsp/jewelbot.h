#ifndef JEWELOT_BOARD_H_
#define JEWELOT_BOARD_H_

#include "nrf_gpio.h"



#define I2C_SCL 12u
#define I2C_SDA 11u
#define HAPTIC_I2C_ADDR (0x5A)  // 0b1011 010x  // 0x5A << 1 //no need to do this in SDK10
#define PMIC_I2C_ADDR (0x48)    // 0b1001 000x //0x48 << 1   // same
#define PMIC_I2C_ADDRESS PMIC_I2C_ADDR
#define PMIC_INTERRUPT_PIN 27
#define DRV2604_I2C_ADDRESS	HAPTIC_I2C_ADDR
#define DRV2604_ENABLE_PIN		(8)
#define DRV2604_TRIGGER_PIN     (9)


// UART over USB
#define RX_PIN_NUMBER  21
#define TX_PIN_NUMBER  24



#endif // inclusion guard
