#ifndef ethernet_h
#define ethernet_h

#include "EthernetConfig.h"

#include <inttypes.h>
//#include "w5100.h"
#include "IPAddress.h"

#ifdef HAS_ETHERNET_CLIENT
#include "EthernetClient.h"
#endif /* HAS_ETHERNET_CLIENT */
#ifdef HAS_ETHERNET_SERVER
#include "EthernetServer.h"
#endif /* HAS_ETHERNET_SERVER */

#ifdef HAS_DHCP
#include "Dhcp.h"
#endif /* HAS_DHCP */

#define MAX_SOCK_NUM 4

typedef struct EthernetClass {
// Private:
  IPAddress _dnsServerAddress;
#ifdef HAS_DHCP
  DhcpClass* _dhcp;
#endif /* HAS_DHCP */
} EthernetClass;

// Public:
  extern uint8_t Ethernet_state[MAX_SOCK_NUM];
  extern uint16_t Ethernet_server_port[MAX_SOCK_NUM];

  // Initialise the Ethernet shield to use the provided MAC address and gain the rest of the
  // configuration through DHCP.
  // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
#ifdef HAS_DHCP
  int Ethernet_begin_1(EthernetClass * this, uint8_t *mac_address);
#endif /* HAS_DHCP */

#ifdef HAS_UNNECESSARY_ETHERNET_BEGIN
  void Ethernet_begin_2(EthernetClass * this, uint8_t *mac_address, IPAddress *local_ip);
  void Ethernet_begin_3(EthernetClass * this, uint8_t *mac_address, IPAddress *local_ip, IPAddress *dns_server);
  void Ethernet_begin_4(EthernetClass * this, uint8_t *mac_address, IPAddress *local_ip, IPAddress *dns_server, IPAddress *gateway);
#endif /* HAS_UNNECESSARY_ETHERNET_BEGIN */

  void Ethernet_begin_5(EthernetClass * this, uint8_t *mac_address, IPAddress *local_ip, IPAddress *dns_server, IPAddress *gateway, IPAddress *subnet);

#ifdef HAS_DHCP
  int Ethernet_maintain(EthernetClass * this);
#endif /* HAS_DHCP */

  IPAddress Ethernet_localIP(EthernetClass * this);
  IPAddress Ethernet_subnetMask(EthernetClass * this);
  IPAddress Ethernet_gatewayIP(EthernetClass * this);
  IPAddress Ethernet_dnsServerIP(EthernetClass * this);

extern EthernetClass Ethernet;

#endif
