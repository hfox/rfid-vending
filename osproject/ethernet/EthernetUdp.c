/*
 *  Udp.cpp: Library to send/receive UDP packets with the Arduino ethernet shield.
 *  This version only offers minimal wrapping of socket.c/socket.h
 *  Drop Udp.h/.cpp into the Ethernet library directory at hardware/libraries/Ethernet/ 
 *
 * MIT License:
 * Copyright (c) 2008 Bjoern Hartmann
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * bjoern@cs.stanford.edu 12/30/2008
 */

#include "w5100.h"
#include "socket.h"
#include "Ethernet.h"
//#include "Udp.h"

#ifdef HAS_DNS
#include "Dns.h"
#endif /* HAS_DNS */

#include "EthernetUdp.h"

#define virtual

  // Read up to len characters from the current packet and place them into buffer
  // Returns the number of characters read, or 0 if none are available
  virtual int EthernetUDP_read_2_charptr(EthernetUDP * this, char* buffer, size_t len)
     { return EthernetUDP_read_2(this, (unsigned char*)buffer, len); };

  // Return the IP address of the host who sent the current incoming packet
  virtual IPAddress EthernetUDP_remoteIP(EthernetUDP * this)
     { return this->_remoteIP; };
  // Return the port of the host who sent the current incoming packet
  virtual uint16_t EthernetUDP_remotePort(EthernetUDP * this)
     { return this->_remotePort; };

  uint8_t* EthernetUDP_rawIPAddress(IPAddress * addr)
     { return IPAddress_raw_address(addr); };

/* Constructor */
void construct_EthernetUDP(EthernetUDP * this)
{
  this->_sock = MAX_SOCK_NUM;
}

/* Start EthernetUDP socket, listening at local port PORT */
uint8_t EthernetUDP_begin(EthernetUDP * this, uint16_t port) {
  if (this->_sock != MAX_SOCK_NUM)
    return 0;

  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    uint8_t s = W5100_readSnSR(i);
    if (s == SnSR_CLOSED || s == SnSR_FIN_WAIT) {
      this->_sock = i;
      break;
    }
  }

  if (this->_sock == MAX_SOCK_NUM)
    return 0;

  this->_port = port;
  this->_remaining = 0;
  socket(this->_sock, SnMR_UDP, this->_port, 0);

  return 1;
}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int EthernetUDP_available(EthernetUDP * this) {
  return this->_remaining;
}

/* Release any resources being used by this EthernetUDP instance */
void EthernetUDP_stop(EthernetUDP * this)
{
  if (this->_sock == MAX_SOCK_NUM)
    return;

  close(this->_sock);

  Ethernet_server_port[this->_sock] = 0;
  this->_sock = MAX_SOCK_NUM;
}

#ifdef HAS_DNS
int EthernetUDP_beginPacket_dns(EthernetUDP * this, const char *host, uint16_t port)
{
  // Look up the host first
  int ret = 0;
  DNSClient dns;
  IPAddress remote_addr;

  dns.begin(Ethernet.dnsServerIP());
  ret = dns.getHostByName(host, remote_addr);
  if (ret == 1) {
    return EthernetUDP_beginPacket_ip(this, &remote_addr, port);
  } else {
    return ret;
  }
}
#endif /* HAS_DNS */

int EthernetUDP_beginPacket_ip(EthernetUDP * this, IPAddress * ip, uint16_t port)
{
  this->_offset = 0;
  return startUDP(this->_sock, EthernetUDP_rawIPAddress(ip), port);
}

int EthernetUDP_endPacket(EthernetUDP * this)
{
  return sendUDP(this->_sock);
}

size_t EthernetUDP_write_1(EthernetUDP * this, uint8_t byte)
{
  return EthernetUDP_write_2(this, &byte, 1);
}

size_t EthernetUDP_write_2(EthernetUDP * this, const uint8_t *buffer, size_t size)
{
  uint16_t bytes_written = bufferData(this->_sock, this->_offset, buffer, size);
  this->_offset += bytes_written;
  return bytes_written;
}

int EthernetUDP_parsePacket(EthernetUDP * this)
{
  // discard any remaining bytes in the last packet
  EthernetUDP_flush(this);

  if (W5100_getRXReceivedSize(this->_sock) > 0)
  {
    //HACK - hand-parse the UDP packet using TCP recv method
    uint8_t tmpBuf[8];
    int ret =0; 
    //read 8 header bytes and get IP and port from it
    ret = recv(this->_sock,tmpBuf,8);
    if (ret > 0)
    {
      operator_assign_IPAddress_intptr(&(this->_remoteIP), tmpBuf);
      this->_remotePort = tmpBuf[4];
      this->_remotePort = (this->_remotePort << 8) + tmpBuf[5];
      this->_remaining = tmpBuf[6];
      this->_remaining = (this->_remaining << 8) + tmpBuf[7];

      // When we get here, any remaining bytes are the data
      ret = this->_remaining;
    }
    return ret;
  }
  // There aren't any packets available
  return 0;
}

int EthernetUDP_read_0(EthernetUDP * this)
{
  uint8_t byte;

  if ((this->_remaining > 0) && (recv(this->_sock, &byte, 1) > 0))
  {
    // We read things without any problems
    this->_remaining--;
    return byte;
  }

  // If we get here, there's no data available
  return -1;
}

int EthernetUDP_read_2(EthernetUDP * this, unsigned char* buffer, size_t len)
{

  if (this->_remaining > 0)
  {

    int got;

    if (this->_remaining <= len)
    {
      // data should fit in the buffer
      got = recv(this->_sock, buffer, this->_remaining);
    }
    else
    {
      // too much data for the buffer, 
      // grab as much as will fit
      got = recv(this->_sock, buffer, len);
    }

    if (got > 0)
    {
      this->_remaining -= got;
      return got;
    }

  }

  // If we get here, there's no data available or recv failed
  return -1;

}

int EthernetUDP_peek(EthernetUDP * this)
{
  uint8_t b;
  // Unlike recv, peek doesn't check to see if there's any data available, so we must.
  // If the user hasn't called parsePacket yet then return nothing otherwise they
  // may get the UDP header
  if (!this->_remaining)
    return -1;
  peek(this->_sock, &b);
  return b;
}

void EthernetUDP_flush(EthernetUDP * this)
{
  // could this fail (loop endlessly) if this->_remaining > 0 and recv in read fails?
  // should only occur if recv fails after telling us the data is there, lets
  // hope the w5100 always behaves :)

  while (this->_remaining)
  {
    EthernetUDP_read_0(this);
  }
}

#undef virtual

