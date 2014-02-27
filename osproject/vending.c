/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "vending.h"

static unsigned int count = 0;

void vListenToVendingMachine(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainVENDING_CHECK_PERIOD );
		count++;
	}
}

void vending_init(void)
{
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

char vending_running(void)
{
	static unsigned int saved_count = 0;

	if (count == saved_count) {
		saved_count = count;
		return 0;
	} else {
		saved_count = count;
		return 1;
	}
}

