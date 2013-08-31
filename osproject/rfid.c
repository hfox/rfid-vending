/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "rfid.h"

void vListenToRfidReader(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainRFID_CHECK_PERIOD );
	}
}

