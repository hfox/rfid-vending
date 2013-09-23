#include "w5100.h"
#include "Ethernet.h"

#ifdef HAS_DHCP
#include "Dhcp.h"
#endif /* HAS_DHCP */

// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t Ethernet_state[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };
uint16_t Ethernet_server_port[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };

#ifdef HAS_DHCP
int Ethernet_begin_1(EthernetClass * this, uint8_t *mac_address)
{
  static DhcpClass s_dhcp;
  static IPAddress s_ip;
  this->_dhcp = &s_dhcp;


  // Initialise the basic info
  W5100_init(&W5100);
  W5100_setMACAddress(mac_address);
  construct_IPAddress(&s_ip,0,0,0,0);
  W5100_setIPAddress(IPAddress_raw_address(&s_ip));

  // Now try to get our config info from a DHCP server
  int ret = this->_dhcp->beginWithDHCP(mac_address);
  if(ret == 1)
  {
    // We've successfully found a DHCP server and got our configuration info, so set things
    // accordingly
    W5100_setIPAddress(IPAddress_raw_address(this->_dhcp->getLocalIp()));
    W5100_setGatewayIp(IPAddress_raw_address(this->_dhcp->getGatewayIp()));
    W5100_setSubnetMask(IPAddress_raw_address(this->_dhcp->getSubnetMask()));
    this->_dnsServerAddress = this->_dhcp->getDnsServerIp();
  }

  return ret;
}
#endif /* HAS_DHCP */

#ifdef HAS_UNNECESSARY_ETHERNET_BEGIN
void Ethernet_begin_2(EthernetClass * this, uint8_t *mac_address, IPAddress local_ip)
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  Ethernet_begin_3(this, mac_address, local_ip, dns_server);
}

void Ethernet_begin_3(EthernetClass * this, uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  Ethernet_begin_4(this, mac_address, local_ip, dns_server, gateway);
}

void Ethernet_begin_4(EthernetClass * this, uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
  IPAddress subnet(255, 255, 255, 0);
  Ethernet_begin_5(this, mac_address, local_ip, dns_server, gateway, subnet);
}
#endif /* HAS_UNNECESSARY_ETHERNET_BEGIN */

void Ethernet_begin_5(EthernetClass * this, uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
  W5100_init(&W5100);
  W5100_setMACAddress(mac);
  W5100_setIPAddress(local_ip._address);
  W5100_setGatewayIp(gateway._address);
  W5100_setSubnetMask(subnet._address);
  this->_dnsServerAddress = dns_server;
}

#ifdef HAS_DHCP
int Ethernet_maintain(){
  int rc = DHCP_CHECK_NONE;
  if(this->_dhcp != NULL){
    //we have a pointer to dhcp, use it
    rc = this->_dhcp->checkLease();
    switch ( rc ){
      case DHCP_CHECK_NONE:
        //nothing done
        break;
      case DHCP_CHECK_RENEW_OK:
      case DHCP_CHECK_REBIND_OK:
        //we might have got a new IP.
        W5100_setIPAddress(IPAddress_raw_address(this->_dhcp->getLocalIp()));
        W5100_setGatewayIp(IPAddress_raw_address(this->_dhcp->getGatewayIp()));
        W5100_setSubnetMask(IPAddress_raw_address(this->_dhcp->getSubnetMask()));
        this->_dnsServerAddress = this->_dhcp->getDnsServerIp();
        break;
      default:
        //this is actually a error, it will retry though
        break;
    }
  }
  return rc;
}
#endif /* HAS_DHCP */

IPAddress Ethernet_localIP(EthernetClass * this)
{
  IPAddress ret;
  W5100_getIPAddress(IPAddress_raw_address(&ret));
  return ret;
}

IPAddress Ethernet_subnetMask(EthernetClass * this)
{
  IPAddress ret;
  W5100_getSubnetMask(IPAddress_raw_address(&ret));
  return ret;
}

IPAddress Ethernet_gatewayIP(EthernetClass * this)
{
  IPAddress ret;
  W5100_getGatewayIp(IPAddress_raw_address(&ret));
  return ret;
}

IPAddress Ethernet_dnsServerIP(EthernetClass * this)
{
  return this->_dnsServerAddress;
}

EthernetClass Ethernet;
