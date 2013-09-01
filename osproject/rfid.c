/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "rfid.h"

static char card[20];

void vListenToRfidReader(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainRFID_CHECK_PERIOD );
	}
}

int rfid_has_card(void)
{
	return 0;
}

void rfid_reset_card(void)
{
	for (int i = 0; i < 20; ++i) {
		card[i] = 0;
	}
}

void rfid_get_card(char *dest)
{
	for (int i = 0; i < 20; ++i) {
		dest[i] = card[i];
	}
}

int rfid_has_timeout(void)
{
	return 0;
}

void rfid_reset_has_timeout(void)
{
}


