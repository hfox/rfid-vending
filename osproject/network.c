/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "network.h"

void vListenToDatabaseServer(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainDATABASE_CHECK_PERIOD );
	}
}

