#ifndef ethernetclient_h
#define ethernetclient_h
#include "Arduino.h"	

#ifdef HAS_STREAM_PRINT
#include "Print.h"
#endif /* HAS_STREAM_PRINT */

//#include "Client.h"
#include "IPAddress.h"

#define virtual
#define bool char

typedef struct EthernetClient {
//private:
  uint8_t _sock;
} EthernetClient;

  static uint16_t _srcport;

  void construct_EthernetClient_0(EthernetClient * this);
  void construct_EthernetClient_1(EthernetClient * this, uint8_t sock);

  uint8_t EthernetClient_status(EthernetClient * this);
  virtual int EthernetClient_connect_ip(EthernetClient * this, IPAddress * ip, uint16_t port);
#ifdef HAS_DNS
  virtual int EthernetClient_connect_dns(EthernetClient * this, const char *host, uint16_t port);
#endif /* HAS_DNS */
  virtual size_t EthernetClient_write_1(EthernetClient * this, uint8_t c);
  virtual size_t EthernetClient_write_2(EthernetClient * this, const uint8_t *buf, size_t size);
  virtual int EthernetClient_available(EthernetClient * this);
  virtual int EthernetClient_read_0(EthernetClient * this);
  virtual int EthernetClient_read_2(EthernetClient * this, uint8_t *buf, size_t size);
  virtual int EthernetClient_peek(EthernetClient * this);
  virtual void EthernetClient_flush(EthernetClient * this);
  virtual void EthernetClient_stop(EthernetClient * this);
  virtual uint8_t EthernetClient_connected(EthernetClient * this);
  virtual bool EthernetClient_operator_bool(EthernetClient * this);

#undef virtual
#undef bool

#endif
