/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "display.h"
#include "serial.h"

static unsigned int count = 0;

void display_run(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainDISPLAY_PERIOD );
		count++;
		serial_send_string("Display\n");
	}
}

void display_init(void)
{
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

char display_running(void)
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

