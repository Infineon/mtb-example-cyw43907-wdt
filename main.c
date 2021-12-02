/******************************************************************************
* File Name: main.c
*
* Description: This is the source code for the CYW43907 MCU Watchdog Timer
*              Example for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2021 Cypress Semiconductor $
*******************************************************************************/

#include "cyhal.h"
#include "cybsp.h"
#include "cy_utils.h"

/*******************************************************************************
* Macros
********************************************************************************/

/* WDT time out for reset mode, in milliseconds. Max limit is given by CYHAL_WDT_MAX_TIMEOUT_MS */
#define WDT_TIME_OUT_MS                     4000
#define ENABLE_BLOCKING_FUNCTION            0

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void initialize_wdt(void);

/*******************************************************************************
* Global Variables
********************************************************************************/

/* WDT object */
cyhal_wdt_t wdt_obj;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It demonstrates the WDT reset
* 
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    
    /* Board init failed. Stop program execution */
    if(result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize the User LED */
    cyhal_gpio_init(CYBSP_LED2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    
    /* Check the reason for device restart */
    if (CYHAL_SYSTEM_RESET_WDT == cyhal_system_get_reset_reason())
    {
        /* It's WDT reset event - blink LED twice */
        cyhal_gpio_write(CYBSP_LED2, CYBSP_LED_STATE_ON);
        cyhal_system_delay_ms(100);
        cyhal_gpio_write(CYBSP_LED2, CYBSP_LED_STATE_OFF);
        cyhal_system_delay_ms(200);
        cyhal_gpio_write(CYBSP_LED2, CYBSP_LED_STATE_ON);
        cyhal_system_delay_ms(100);
        cyhal_gpio_write(CYBSP_LED2, CYBSP_LED_STATE_OFF);
    }
    else
    {
        /* It's Power-On reset or XRES event - blink LED once */
        cyhal_gpio_write(CYBSP_LED2, CYBSP_LED_STATE_ON);
        cyhal_system_delay_ms(100);
        cyhal_gpio_write(CYBSP_LED2, CYBSP_LED_STATE_OFF);;
        cyhal_system_delay_ms(100);
    }

    /* Initialize WDT */
    initialize_wdt();

    /* Enable global interrupt */
    __enable_irq();

    for (;;)
    {
        #if (ENABLE_BLOCKING_FUNCTION)
            while(1);            
        #else
            /* Reset WDT */
            cyhal_wdt_kick(&wdt_obj);

            /* Constant delay of 1000ms */
            cyhal_system_delay_ms(1000);

            /* Invert the state of LED */
            cyhal_gpio_toggle(CYBSP_LED2);
        #endif
      
    }
}

/*******************************************************************************
* Function Name: InitializeWDT
********************************************************************************
* Summary:
* This function initializes the WDT block
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void initialize_wdt()
{
    cy_rslt_t result;

    /* Initialize the WDT */
    result = cyhal_wdt_init(&wdt_obj, WDT_TIME_OUT_MS);

    /* WDT initialization failed. Stop program execution */
    if(result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

/* [] END OF FILE */

