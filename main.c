/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup tw_sensor_example main.c
 * @{
 * @ingroup nrf_twi_example
 * @brief TWI Sensor Example main file.
 *
 * This file contains the source code for a sample application using TWI.
 *
 */

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_uart.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

#include "pmic_driver.h"
#include "haptics_driver.h"
#include "SEGGER_RTT.h"

/*UART buffer size. */
#define UART_TX_BUF_SIZE 256
#define UART_RX_BUF_SIZE 1

/*Common addresses definition for accelereomter. */

/* Define version of GCC. */
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)


#ifdef __GNUC_PATCHLEVEL__
#if GCC_VERSION < 50505
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"           // Hack to GCC 4.9.3 bug. Can be deleted after switch on using GCC 5.0.0
#endif
#endif

#ifdef __GNUC_PATCHLEVEL__
#if GCC_VERSION < 50505
#pragma GCC diagnostic pop
#endif
#endif
/* Indicates if reading operation from accelerometer has ended. */
static volatile bool m_xfer_done = true;
/* Indicates if setting mode operation has ended. */
static volatile bool m_set_mode_done;
/* TWI instance. */
nrf_drv_twi_t app_twi_instance = NRF_DRV_TWI_INSTANCE(0);
static bool use_event_handler = false;


/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{   
    
    switch(p_event->type)
    {
        case NRF_DRV_TWI_RX_DONE:
            m_xfer_done = true;
            break;
        case NRF_DRV_TWI_TX_DONE:
            if(m_set_mode_done != true)
            {
                m_set_mode_done  = true;
                return;
            }
            m_xfer_done = false;
            /* Read 4 bytes from the specified address. */
            //err_code = nrf_drv_twi_rx(&m_twi_mma_7660, MMA7660_ADDR, (uint8_t*)&m_sample, sizeof(m_sample), false);
            //APP_ERROR_CHECK(err_code);
            break;
        default:
            break;        
    }   
}

void twi_init_with_handler (void)
{
    ret_code_t err_code;
    
    const nrf_drv_twi_config_t twi_config = {
       .scl                = I2C_SCL,
       .sda                = I2C_SDA,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH
    };
    
    err_code = nrf_drv_twi_init(&app_twi_instance, &twi_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);
    SEGGER_RTT_printf(0, "nrf_drv_twi_init with handler: err_code: %u\n", err_code);
    
    nrf_drv_twi_enable(&app_twi_instance);
}

static void twi_init()
{
	uint32_t err_code;
	const nrf_drv_twi_config_t twi_config = {
		.scl                = I2C_SCL,
		.sda                = I2C_SDA,
		.frequency          = NRF_TWI_FREQ_400K, //was 100K
		.interrupt_priority = APP_IRQ_PRIORITY_HIGH
	};

	err_code = nrf_drv_twi_init(&app_twi_instance, &twi_config, NULL, NULL); //todo add event handler
	APP_ERROR_CHECK(err_code);
  SEGGER_RTT_printf(0, "nrf_drv_twi_init: err_code: %u\n", err_code);
	nrf_drv_twi_enable(&app_twi_instance);
}



/**
 * @brief Function for main application entry.
 */
int main(void)
{
   // int a = __GNUC__, c = __GNUC_PATCHLEVEL__;
    if (use_event_handler) {
      twi_init_with_handler();
      SEGGER_RTT_WriteString(0, "TWI INIT WITH HANDLER\n");
    } else {
      twi_init();
      SEGGER_RTT_WriteString(0, "TWI INIT\n");
    }
    pmic_init();
    SEGGER_RTT_WriteString(0, "PMIC INIT\n");
    haptics_init();
    SEGGER_RTT_WriteString(0, "HAPTICS INIT\n");
    pmic_turn_on_charging();
		nrf_delay_ms(2000);  //delay to feel difference between haptics init and first test run
     if (pmic_is_charging()) {
      SEGGER_RTT_WriteString(0, "haptics_test_run2\n");
			SEGGER_RTT_WriteString(0, "\n");
      haptics_test_run2();
      nrf_delay_ms(10000);
    }
    else {
      haptics_test_run3();
      SEGGER_RTT_WriteString(0, "haptics_test_run3\n");
      SEGGER_RTT_WriteString(0, "\n");
      nrf_delay_ms(10000);
    }
    if (pmic_5V_present()) {
      haptics_test_run1();
      SEGGER_RTT_WriteString(0, "haptics_test_run1\n");
      SEGGER_RTT_WriteString(0, "\n");
      nrf_delay_ms(10000);
    }
    else { 
      haptics_test_run4();
      SEGGER_RTT_WriteString(0, "haptics_test_run4\n");
      SEGGER_RTT_WriteString(0, "\n");
      nrf_delay_ms(10000);
    }

    while(true)
    {
        nrf_delay_ms(100);
        /* Start transaction with a slave with the specified address. */
        if (use_event_handler) {
          do
          {
              __WFE();
          }while(m_xfer_done == false);
          //err_code = nrf_drv_twi_tx(&m_twi_mma_7660, MMA7660_ADDR, &reg, sizeof(reg), true);
          //APP_ERROR_CHECK(err_code);
          m_xfer_done = false;
        }
        else {
         
        }
    }
}

/** @} */
