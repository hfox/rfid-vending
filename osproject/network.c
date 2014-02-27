/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "network.h"

static unsigned int count = 0;

static char card[20];
static int price;

void vListenToDatabaseServer(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainDATABASE_CHECK_PERIOD );
		count++;
	}
}

void network_init(void)
{
}

void network_set_price(int source)
{
	price = source;
}

void network_set_card(char *source)
{
	for (int i = 0; i < 20; ++i) {
		card[i] = source[i];
	}
}

void network_set_send(void)
{
}

int network_has_reply(void)
{
	return 0;
}

int network_get_reply(void)
{
	return 0;
}

void network_reset_reply(void)
{
}

char network_running(void)
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


