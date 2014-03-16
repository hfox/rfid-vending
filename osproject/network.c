/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "Ethernet.h"
#include "EthernetClient.h"
#include "IPAddress.h"
#include "network.h"

static unsigned int count = 0;

static char card[20];
static int price;

static uint8_t _mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x4F, 0x9C};
static uint8_t *mac = _mac;

static IPAddress _ip = {._address = {192, 168, 1, 55}};
static IPAddress *ip = &_ip;

static IPAddress _server_ip = {._address = {192, 168, 1, 50}};
static IPAddress *server_ip = &_server_ip;

static IPAddress _subnet_ip = {._address = {255, 255, 255, 0}};
static IPAddress *subnet_ip = &_subnet_ip;

static EthernetClass _eth, *eth = &_eth;
static EthernetClient _ethclient, *ethclient = &_ethclient;

void network_run (void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;
	static char text[] = "Hej\n";
	static int len = 0;
	if (len == 0)
		len = strlen(text);
	
	for(;;) {
		vTaskDelay( mainDATABASE_CHECK_PERIOD );
		count++;
		
		//EthernetClient_write_2(ethclient, text, len);
		EthernetClient_write_2(ethclient, "abcdefghijklmn", 8);
	}
}

void network_init(void)
{
	// Prepare IP address records
	construct_IPAddress_4(ip, 192, 168, 1, 55);
	construct_IPAddress_4(server_ip, 192, 168, 1, 50);
	construct_IPAddress_4(subnet_ip, 255, 255, 255, 0);
	
	// Initialize Ethernet shield hardware and classes
	Ethernet_begin_5(eth, mac, ip, server_ip, server_ip, subnet_ip);
	construct_EthernetClient_0(ethclient);
	EthernetClient_connect_ip(ethclient, server_ip, 1234);
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


