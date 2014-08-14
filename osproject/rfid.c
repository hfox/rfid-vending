/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "rfid.h"
#include "serial.h"

static unsigned int count = 0;

static char card[20];

void rfid_run(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainRFID_CHECK_PERIOD );
		count++;
		serial_send_string("RFID\n");
	}
}

void rfid_step(void)
{
	serial_send_string("RFID\n");
}

void rfid_init(void)
{
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

char rfid_running(void)
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


