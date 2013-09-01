/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "vending.h"

void vListenToVendingMachine(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainVENDING_CHECK_PERIOD );
	}
}

void vending_set_accept(void)
{
}

void vending_set_deny(void)
{
}

int vending_has_price(void)
{
	return 0;
}

void vending_reset_price(void)
{
}

int vending_get_price(void)
{
	return 10;
}

int vending_has_state(void)
{
	return 0;
}

void vending_reset_state(void)
{
}

int vending_get_state(void)
{
	return vendingSTATE_PENDING;
}

