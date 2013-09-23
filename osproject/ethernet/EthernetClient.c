#include "w5100.h"
#include "socket.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
  #include "string.h"
#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "Arduino.h"

#include "Ethernet.h"
#include "EthernetClient.h"
#include "EthernetUdp.h"
#ifdef HAS_ETHERNET_SERVER
#include "EthernetServer.h"
#endif /* HAS_ETHERNET_SERVER */
#ifdef HAS_DNS
#include "Dns.h"
#endif /* HAS_DNS */

#define bool char

uint16_t EthernetClient_srcport = 1024;

void construct_EthernetClient_0(EthernetClient * this)
{
  this->_sock = MAX_SOCK_NUM;
}

void construct_EthernetClient_1(EthernetClient * this, uint8_t sock)
{
  this->_sock = sock;
}

#ifdef HAS_DNS
int EthernetClient_connect_dns(EthernetClient * this, const char* host, uint16_t port) {
  // Look up the host first
  int ret = 0;
  DNSClient dns;
  IPAddress remote_addr;

  dns.begin(Ethernet.dnsServerIP());
  ret = dns.getHostByName(host, remote_addr);
  if (ret == 1) {
    return connect_ip(&remote_addr, port);
  } else {
    return ret;
  }
}
#endif /* HAS_DNS */

int EthernetClient_connect_ip(EthernetClient * this, IPAddress * ip, uint16_t port) {
  if (this->_sock != MAX_SOCK_NUM)
    return 0;

  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    uint8_t s = W5100_readSnSR(i);
    if (s == SnSR_CLOSED || s == SnSR_FIN_WAIT || s == SnSR_CLOSE_WAIT) {
      this->_sock = i;
      break;
    }
  }

  if (this->_sock == MAX_SOCK_NUM)
    return 0;

  _srcport++;
  if (_srcport == 0) _srcport = 1024;
  socket(this->_sock, SnMR_TCP, _srcport, 0);

  if (!connect(this->_sock, EthernetUDP_rawIPAddress(ip), port)) {
    this->_sock = MAX_SOCK_NUM;
    return 0;
  }

  while (EthernetClient_status(this) != SnSR_ESTABLISHED) {
    delay(1);
    if (EthernetClient_status(this) == SnSR_CLOSED) {
      this->_sock = MAX_SOCK_NUM;
      return 0;
    }
  }

  return 1;
}

size_t EthernetClient_write_1(EthernetClient * this, uint8_t b) {
  return EthernetClient_write_2(this, &b, 1);
}

size_t EthernetClient_write_2(EthernetClient * this, const uint8_t *buf, size_t size) {
  if (this->_sock == MAX_SOCK_NUM) {
#ifdef HAS_STREAM_PRINT
    setWriteError();
#endif /* HAS_STREAM_PRINT */
    return 0;
  }
  if (!send(this->_sock, buf, size)) {
#ifdef HAS_STREAM_PRINT
    setWriteError();
#endif /* HAS_STREAM_PRINT */
    return 0;
  }
  return size;
}

int EthernetClient_available(EthernetClient * this) {
  if (this->_sock != MAX_SOCK_NUM)
    return W5100_getRXReceivedSize(this->_sock);
  return 0;
}

int EthernetClient_read_0(EthernetClient * this) {
  uint8_t b;
  if ( recv(this->_sock, &b, 1) > 0 )
  {
    // recv worked
    return b;
  }
  else
  {
    // No data available
    return -1;
  }
}

int EthernetClient_read_2(EthernetClient * this, uint8_t *buf, size_t size) {
  return recv(this->_sock, buf, size);
}

int EthernetClient_peek(EthernetClient * this) {
  uint8_t b;
  // Unlike recv, peek doesn't check to see if there's any data available, so we must
  if (!EthernetClient_available(this))
    return -1;
  peek(this->_sock, &b);
  return b;
}

void EthernetClient_flush(EthernetClient * this) {
  while (EthernetClient_available(this))
    EthernetClient_read_0(this);
}

void EthernetClient_stop(EthernetClient * this) {
  if (this->_sock == MAX_SOCK_NUM)
    return;

  // attempt to close the connection gracefully (send a FIN to other side)
  disconnect(this->_sock);
  unsigned long start = millis();

  // wait a second for the connection to close
  while (EthernetClient_status(this) != SnSR_CLOSED && millis() - start < 1000)
    delay(1);

  // if it hasn't closed, close it forcefully
  if (EthernetClient_status(this) != SnSR_CLOSED)
    close(this->_sock);

  Ethernet_server_port[this->_sock] = 0;
  this->_sock = MAX_SOCK_NUM;
}

uint8_t EthernetClient_connected(EthernetClient * this) {
  if (this->_sock == MAX_SOCK_NUM) return 0;
  
  uint8_t s = EthernetClient_status(this);
  return !(s == SnSR_LISTEN || s == SnSR_CLOSED || s == SnSR_FIN_WAIT ||
    (s == SnSR_CLOSE_WAIT && !EthernetClient_available(this)));
}

uint8_t EthernetClient_status(EthernetClient * this) {
  if (this->_sock == MAX_SOCK_NUM) return SnSR_CLOSED;
  return W5100_readSnSR(this->_sock);
}

// the next function allows us to use the client returned by
// EthernetServer_available(this) as the condition in an if-statement.

bool EthernetClient_operator_bool(EthernetClient * this) {
  return this->_sock != MAX_SOCK_NUM;
}

#undef bool

