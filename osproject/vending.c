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

