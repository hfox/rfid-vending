/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "display.h"

void vDisplay(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainDISPLAY_PERIOD );
	}
}

void display_set_error(void)
{
}

void display_set_has_card(void)
{
}

void display_set_accept(void)
{
}

void display_set_deny(void)
{
}


