/*
    FreeRTOS V7.5.2 - Copyright (C) 2013 Real Time Engineers Ltd.

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>! NOTE: The modification to the GPL is included to allow you to distribute
    >>! a combined work that includes FreeRTOS without being obliged to provide
    >>! the source code for proprietary components outside of the FreeRTOS
    >>! kernel.

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!
*/

#include <stdlib.h>
#include <string.h>
#include <avr/wdt.h>

#ifdef GCC_MEGA_AVR
	/* EEPROM routines used only with the WinAVR compiler. */
	#include <avr/eeprom.h> 
#endif

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

/* Project components */
#include "vending.h"
#include "rfid.h"
#include "network.h"
#include "logic.h"
#include "display.h"

/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainDISPLAY_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainMAIN_LOGIC_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainRFID_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define mainDATABASE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define mainVENDING_MACHINE_TASK_PRIORITY	( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )

/* The period between executions of the check task. */
#define mainCHECK_PERIOD				( ( portTickType ) 1000 / portTICK_RATE_MS  )
#define mainERROR_PERIOD				( ( portTickType ) 250 / portTICK_RATE_MS )

/* An address in the EEPROM used to count resets.  This is used to check that
the demo application is not unexpectedly resetting. */
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )

/*
 * The task function for the "Check" task.
 */
static void vErrorChecks( void *pvParameters );

/*
 * Checks the unique counts of other tasks to ensure they are still operational.
 * Flashes an LED if everything is okay. 
 */
static void prvCheckOtherTasksAreStillRunning( void );

/*
 * Called on boot to increment a count stored in the EEPROM.  This is used to 
 * ensure the CPU does not reset unexpectedly.
 */
static void prvIncrementResetCount( void );

/*
 * The idle hook is used to scheduler co-routines.
 */
void vApplicationIdleHook( void );

static void initWatchDog(void);
static void led_init(void);
static void led_toggle(void);

static portBASE_TYPE xErrorHasOccurred = pdFALSE;

/*-----------------------------------------------------------*/

short main( void )
{
	prvIncrementResetCount();

	initWatchDog();

	/* Run the init function in each module */
	vending_init();
	rfid_init();
	network_init();
	logic_init();
	display_init();

	/* Create the tasks defined within this file. */
	xTaskCreate( vErrorChecks, ( signed char * ) "Check",
		configMINIMAL_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
	xTaskCreate( vListenToVendingMachine, ( signed char * ) "Listen to vending machine",
		configMINIMAL_STACK_SIZE, NULL, mainVENDING_MACHINE_TASK_PRIORITY, NULL );
	xTaskCreate( vListenToRfidReader, ( signed char * ) "Listen to RFID reader",
		configMINIMAL_STACK_SIZE, NULL, mainRFID_TASK_PRIORITY, NULL );
	xTaskCreate( vListenToDatabaseServer, ( signed char * ) "Listen to database server",
		configMINIMAL_STACK_SIZE, NULL, mainDATABASE_TASK_PRIORITY, NULL );
	xTaskCreate( vMainLogic, ( signed char * ) "Main logic",
		configMINIMAL_STACK_SIZE, NULL, mainMAIN_LOGIC_TASK_PRIORITY, NULL );
	xTaskCreate( vDisplay, ( signed char * ) "Display",
		configMINIMAL_STACK_SIZE, NULL, mainDISPLAY_TASK_PRIORITY, NULL );

	/* In this port, to use preemptive scheduler define configUSE_PREEMPTION 
	as 1 in portmacro.h.  To use the cooperative scheduler define 
	configUSE_PREEMPTION as 0. */
	vTaskStartScheduler();

	return 0;
}
/*-----------------------------------------------------------*/

static void vErrorChecks( void *pvParameters )
{
	static volatile unsigned long ulDummyVariable = 3UL;

	/* The parameters are not used. */
	( void ) pvParameters;

	// Initialize LED
	led_init();

	/* Cycle for ever, delaying then checking all the other tasks are still
	operating without error. */
	while (xErrorHasOccurred == pdFALSE)
	{
		vTaskDelay( mainCHECK_PERIOD );

		/* Perform a bit of 32bit maths to ensure the registers used by the 
		integer tasks get some exercise. The result here is not important - 
		see the demo application documentation for more info. */
		ulDummyVariable *= 3;
		
		prvCheckOtherTasksAreStillRunning();
	}

	// Show that an error has occured by toggling the LED quickly
	for ( ;; )
	{
		vTaskDelay(mainERROR_PERIOD);
		led_toggle();

#if configHANG_ON_ERROR != 0
		wdt_reset();
#endif
	}
}

/*-----------------------------------------------------------*/

static void prvCheckOtherTasksAreStillRunning( void )
{
	if (! display_running())
		xErrorHasOccurred = pdTRUE;
	if (! logic_running())
		xErrorHasOccurred = pdTRUE;
	if (! network_running())
		xErrorHasOccurred = pdTRUE;
	if (! rfid_running())
		xErrorHasOccurred = pdTRUE;
	if (! vending_running())
		xErrorHasOccurred = pdTRUE;

	if( xErrorHasOccurred == pdFALSE )
	{
		// Toggle the LED if everything is okay so we know if an error occurs
		led_toggle();

		// Reset the watchdog timer
		wdt_reset();
	}
}
/*-----------------------------------------------------------*/

static void prvIncrementResetCount( void )
{
	unsigned char ucCount;

	eeprom_read_block( &ucCount, mainRESET_COUNT_ADDRESS, sizeof( ucCount ) );
	ucCount++;
	eeprom_write_byte( mainRESET_COUNT_ADDRESS, ucCount );
}

static void initWatchDog(void)
{
	// Watchdog setup is time critical. Block interrupts.
	taskENTER_CRITICAL();

	// Reset the counter
	wdt_reset();

	// Initialize the change
	WDTCSR |= (1<<WDCE) | (1<<WDE);

	// Enable system-reset mode
	// Enable 512K cycles prescaling
	// Typical time-out is 4.0 seconds
	WDTCSR = (1<<WDE) | (1<<WDP3);

	// Allow interrupts
	taskEXIT_CRITICAL();
}

/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
}

/*-----------------------------------------------------------*/

void led_init(void)
{
	DDRB |= 1 << PB5;
}

void led_toggle(void)
{
	PORTB ^= 1 << PB5;
}

