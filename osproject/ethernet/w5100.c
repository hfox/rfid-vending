/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "w5100.h"
#include "SPI.h"

// W5100 controller instance
W5100Class W5100;

#define TX_RX_MAX_BUF_SIZE 2048
#define TX_BUF 0x1100
#define RX_BUF (TX_BUF + TX_RX_MAX_BUF_SIZE)

#define TXBUF_BASE 0x4000
#define RXBUF_BASE 0x6000

  // W5100 Registers
  // ---------------

#define __GP_REGISTER8(name, address)
#define __GP_REGISTER16(name, address)            \
  void W5100_write##name(uint16_t _data) {       \
    W5100_write_2(address,   _data >> 8);                 \
    W5100_write_2(address+1, _data & 0xFF);               \
  }                                               \
  uint16_t W5100_read##name(void) {                  \
    uint16_t res = W5100_read_1(address);                 \
    res = (res << 8) + W5100_read_1(address + 1);         \
    return res;                                   \
  }
#define __GP_REGISTER_N(name, address, size)      \
  uint16_t W5100_write##name(uint8_t *_buff) {   \
    return W5100_write_3(address, _buff, size);           \
  }                                               \
  uint16_t W5100_read##name(uint8_t *_buff) {    \
    return W5100_read_3(address, _buff, size);            \
  }

//public:
  __GP_REGISTER8 (MR,     0x0000);    // Mode
  __GP_REGISTER_N(GAR,    0x0001, 4); // Gateway IP address
  __GP_REGISTER_N(SUBR,   0x0005, 4); // Subnet mask address
  __GP_REGISTER_N(SHAR,   0x0009, 6); // Source MAC address
  __GP_REGISTER_N(SIPR,   0x000F, 4); // Source IP address
  __GP_REGISTER8 (IR,     0x0015);    // Interrupt
  __GP_REGISTER8 (IMR,    0x0016);    // Interrupt Mask
  __GP_REGISTER16(RTR,    0x0017);    // Timeout address
  __GP_REGISTER8 (RCR,    0x0019);    // Retry count
  __GP_REGISTER8 (RMSR,   0x001A);    // Receive memory size
  __GP_REGISTER8 (TMSR,   0x001B);    // Transmit memory size
  __GP_REGISTER8 (PATR,   0x001C);    // Authentication type address in PPPoE mode
  __GP_REGISTER8 (PTIMER, 0x0028);    // PPP LCP Request Timer
  __GP_REGISTER8 (PMAGIC, 0x0029);    // PPP LCP Magic Number
  __GP_REGISTER_N(UIPR,   0x002A, 4); // Unreachable IP address in UDP mode
  __GP_REGISTER16(UPORT,  0x002E);    // Unreachable Port address in UDP mode
  
#undef __GP_REGISTER8
#undef __GP_REGISTER16
#undef __GP_REGISTER_N

  // W5100 Socket registers
  // ----------------------

#define __SOCKET_REGISTER8(name, address)
#define __SOCKET_REGISTER16(name, address)                   \
  void W5100_write##name(SOCKET _s, uint16_t _data) {       \
    W5100_writeSn_3(_s, address,   _data >> 8);                      \
    W5100_writeSn_3(_s, address+1, _data & 0xFF);                    \
  }                                                          \
  uint16_t W5100_read##name(SOCKET _s) {                    \
    uint16_t res = W5100_readSn_2(_s, address);                      \
    uint16_t res2 = W5100_readSn_2(_s,address + 1);                  \
    res = res << 8;                                          \
    res2 = res2 & 0xFF;                                      \
    res = res | res2;                                        \
    return res;                                              \
  }
#define __SOCKET_REGISTER_N(name, address, size)             \
  uint16_t W5100_write##name(SOCKET _s, uint8_t *_buff) {   \
    return W5100_writeSn_4(_s, address, _buff, size);                \
  }                                                          \
  uint16_t W5100_read##name(SOCKET _s, uint8_t *_buff) {    \
    return W5100_readSn_4(_s, address, _buff, size);                 \
  }
  
//public:
  __SOCKET_REGISTER8(SnMR,        0x0000)        // Mode
  __SOCKET_REGISTER8(SnCR,        0x0001)        // Command
  __SOCKET_REGISTER8(SnIR,        0x0002)        // Interrupt
  __SOCKET_REGISTER8(SnSR,        0x0003)        // Status
  __SOCKET_REGISTER16(SnPORT,     0x0004)        // Source Port
  __SOCKET_REGISTER_N(SnDHAR,     0x0006, 6)     // Destination Hardw Addr
  __SOCKET_REGISTER_N(SnDIPR,     0x000C, 4)     // Destination IP Addr
  __SOCKET_REGISTER16(SnDPORT,    0x0010)        // Destination Port
  __SOCKET_REGISTER16(SnMSSR,     0x0012)        // Max Segment Size
  __SOCKET_REGISTER8(SnPROTO,     0x0014)        // Protocol in IP RAW Mode
  __SOCKET_REGISTER8(SnTOS,       0x0015)        // IP TOS
  __SOCKET_REGISTER8(SnTTL,       0x0016)        // IP TTL
  __SOCKET_REGISTER16(SnTX_FSR,   0x0020)        // TX Free Size
  __SOCKET_REGISTER16(SnTX_RD,    0x0022)        // TX Read Pointer
  __SOCKET_REGISTER16(SnTX_WR,    0x0024)        // TX Write Pointer
  __SOCKET_REGISTER16(SnRX_RSR,   0x0026)        // RX Free Size
  __SOCKET_REGISTER16(SnRX_RD,    0x0028)        // RX Read Pointer
  __SOCKET_REGISTER16(SnRX_WR,    0x002A)        // RX Write Pointer (supported?)
  
#undef __SOCKET_REGISTER8
#undef __SOCKET_REGISTER16
#undef __SOCKET_REGISTER_N

/* ------------------------------------------------------------------ */

void W5100_init(W5100Class * this)
{
  delay(300);

  SPI_begin();
  W5100_initSS();
  
  W5100_writeMR(1<<W5100_RST);
  W5100_writeTMSR(0x55);
  W5100_writeRMSR(0x55);

  for (int i=0; i<MAX_SOCK_NUM; i++) {
    this->SBASE[i] = TXBUF_BASE + W5100_SSIZE * i;
    this->RBASE[i] = RXBUF_BASE + W5100_RSIZE * i;
  }
}

uint16_t W5100_getTXFreeSize(SOCKET s)
{
  uint16_t val=0, val1=0;
  do {
    val1 = W5100_readSnTX_FSR(s);
    if (val1 != 0)
      val = W5100_readSnTX_FSR(s);
  } 
  while (val != val1);
  return val;
}

uint16_t W5100_getRXReceivedSize(SOCKET s)
{
  uint16_t val=0,val1=0;
  do {
    val1 = W5100_readSnRX_RSR(s);
    if (val1 != 0)
      val = W5100_readSnRX_RSR(s);
  } 
  while (val != val1);
  return val;
}


void W5100_send_data_processing(W5100Class * this, SOCKET s, const uint8_t *data, uint16_t len)
{
  // This is same as having no offset in a call to send_data_processing_offset
  W5100_send_data_processing_offset(this, s, 0, data, len);
}

void W5100_send_data_processing_offset(W5100Class * this, SOCKET s, uint16_t data_offset, const uint8_t *data, uint16_t len)
{
  uint16_t ptr = W5100_readSnTX_WR(s);
  ptr += data_offset;
  uint16_t offset = ptr & W5100_SMASK;
  uint16_t dstAddr = offset + this->SBASE[s];

  if (offset + len > W5100_SSIZE) 
  {
    // Wrap around circular buffer
    uint16_t size = W5100_SSIZE - offset;
    W5100_write_3(dstAddr, data, size);
    W5100_write_3(this->SBASE[s], data + size, len - size);
  } 
  else {
    W5100_write_3(dstAddr, data, len);
  }

  ptr += len;
  W5100_writeSnTX_WR(s, ptr);
}

void W5100_recv_data_processing_3(W5100Class * this, SOCKET s, uint8_t *data, uint16_t len)
{
  W5100_recv_data_processing_4(this, s, data, len, 0);
}

void W5100_recv_data_processing_4(W5100Class * this, SOCKET s, uint8_t *data, uint16_t len, uint8_t peek)
{
  uint16_t ptr;
  ptr = W5100_readSnRX_RD(s);
  W5100_read_data(this, s, (uint8_t *)ptr, data, len);
  if (!peek)
  {
    ptr += len;
    W5100_writeSnRX_RD(s, ptr);
  }
}

void W5100_read_data(W5100Class * this, SOCKET s, volatile uint8_t *src, volatile uint8_t *dst, uint16_t len)
{
  uint16_t size;
  uint16_t src_mask;
  uint16_t src_ptr;

  src_mask = (uint16_t)src & W5100_RMASK;
  src_ptr = this->RBASE[s] + src_mask;

  if( (src_mask + len) > W5100_RSIZE ) 
  {
    size = W5100_RSIZE - src_mask;
    W5100_read_3(src_ptr, (uint8_t *)dst, size);
    dst += size;
    W5100_read_3(this->RBASE[s], (uint8_t *) dst, len - size);
  } 
  else
    W5100_read_3(src_ptr, (uint8_t *) dst, len);
}


uint8_t W5100_write_2(uint16_t _addr, uint8_t _data)
{
  W5100_setSS();  
  SPI_transfer(0xF0);
  SPI_transfer(_addr >> 8);
  SPI_transfer(_addr & 0xFF);
  SPI_transfer(_data);
  W5100_resetSS();
  return 1;
}

uint16_t W5100_write_3(uint16_t _addr, const uint8_t *_buf, uint16_t _len)
{
  for (uint16_t i=0; i<_len; i++)
  {
    W5100_setSS();    
    SPI_transfer(0xF0);
    SPI_transfer(_addr >> 8);
    SPI_transfer(_addr & 0xFF);
    _addr++;
    SPI_transfer(_buf[i]);
    W5100_resetSS();
  }
  return _len;
}

uint8_t W5100_read_1(uint16_t _addr)
{
  W5100_setSS();  
  SPI_transfer(0x0F);
  SPI_transfer(_addr >> 8);
  SPI_transfer(_addr & 0xFF);
  uint8_t _data = SPI_transfer(0);
  W5100_resetSS();
  return _data;
}

uint16_t W5100_read_3(uint16_t _addr, uint8_t *_buf, uint16_t _len)
{
  for (uint16_t i=0; i<_len; i++)
  {
    W5100_setSS();
    SPI_transfer(0x0F);
    SPI_transfer(_addr >> 8);
    SPI_transfer(_addr & 0xFF);
    _addr++;
    _buf[i] = SPI_transfer(0);
    W5100_resetSS();
  }
  return _len;
}

void W5100_execCmdSn(SOCKET s, SockCMD _cmd) {
  // Send command to socket
  W5100_writeSnCR(s, _cmd);
  // Wait for command to complete
  while (W5100_readSnCR(s))
    ;
}
