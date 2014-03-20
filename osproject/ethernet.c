#include <ethernet/Ethernet.h>
#include <ethernet/IPAddress.h>
#include <ethernet/EthernetClient.h>
#include "ethernet.h"

static uint8_t _mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x4F, 0x9C};
static uint8_t *mac = _mac;

static IPAddress _ip = {._address = {192, 168, 1, 55}};
static IPAddress *ip = &_ip;

static IPAddress _server_ip = {._address = {192, 168, 1, 50}};
static IPAddress *server_ip = &_server_ip;

static IPAddress _subnet_ip = {._address = {255, 255, 255, 0}};
static IPAddress *subnet_ip = &_subnet_ip;

static EthernetClient _ethclient, *ethclient = &_ethclient;

int ethernet_init(void)
{
	int status = 0;
	
	// Prepare IP address records
	construct_IPAddress_4(ip, 192, 168, 1, 55);
	construct_IPAddress_4(server_ip, 192, 168, 1, 50);
	construct_IPAddress_4(subnet_ip, 255, 255, 255, 0);
	
	// Initialize Ethernet shield hardware and classes
	Ethernet_begin_5(&Ethernet, mac, ip, server_ip, server_ip, subnet_ip);
	construct_EthernetClient_0(ethclient);
	status = EthernetClient_connect_ip(ethclient, server_ip, 1234);
	return status;
}

int ethernet_send_byte(char c)
{
	return EthernetClient_write_2(ethclient, &c, 1);
}

int ethernet_send_bytes(char *ptr, int len)
{
	return EthernetClient_write_2(ethclient, ptr, len);
}

int ethernet_send_string(char *str)
{
	int len = strlen(str);
	return EthernetClient_write_2(ethclient, str, len);
}
