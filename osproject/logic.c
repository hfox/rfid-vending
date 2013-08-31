/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "logic.h"

void vMainLogic(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainMAIN_LOGIC_CHECK_PERIOD );
	}
}

