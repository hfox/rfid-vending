/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "Ethernet.h"
#include "EthernetClient.h"
#include "IPAddress.h"
#include "network.h"
#include "ethernet.h"
#include "serial.h"
#include "parallel.h"

static unsigned int count = 0;

static char card[20];
static int price;

static int crash = 0;

void network_run (void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;
	static char text[] = "Hej\n";
	static int len = 0;
	static int status = 0;
	
	if (len == 0)
		len = strlen(text);
	
	for(; crash == 0;) {
		vTaskDelay( mainNETWORK_CHECK_PERIOD );
		count++;
		
		status = ETHERNET_SEND_ARRAY("abcdefghijklmn");
		serial_send_string("Network\n");
		if (status == 0)
			serial_send_string("Eth send fail\n");
		else
			serial_send_string("Eth send ok\n");
	}
	for(;;){} // Trap
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


