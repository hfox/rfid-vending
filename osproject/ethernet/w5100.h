/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef	W5100_H_INCLUDED
#define	W5100_H_INCLUDED

#include <avr/pgmspace.h>
#include <SPI.h>

#define MAX_SOCK_NUM 4

typedef uint8_t SOCKET;

#define IDM_OR  0x8000
#define IDM_AR0 0x8001
#define IDM_AR1 0x8002
#define IDM_DR  0x8003
/*
class MR {
public:
  static const uint8_t RST   = 0x80;
  static const uint8_t PB    = 0x10;
  static const uint8_t PPPOE = 0x08;
  static const uint8_t LB    = 0x04;
  static const uint8_t AI    = 0x02;
  static const uint8_t IND   = 0x01;
};
*/
/*
class IR {
public:
  static const uint8_t CONFLICT = 0x80;
  static const uint8_t UNREACH  = 0x40;
  static const uint8_t PPPoE    = 0x20;
  static const uint8_t SOCK0    = 0x01;
  static const uint8_t SOCK1    = 0x02;
  static const uint8_t SOCK2    = 0x04;
  static const uint8_t SOCK3    = 0x08;
  static inline uint8_t SOCK(SOCKET ch) { return (0x01 << ch); };
};
*/

typedef enum SnMR {
  SnMR_CLOSE  = 0x00,
  SnMR_TCP    = 0x01,
  SnMR_UDP    = 0x02,
  SnMR_IPRAW  = 0x03,
  SnMR_MACRAW = 0x04,
  SnMR_PPPOE  = 0x05,
  SnMR_ND     = 0x20,
  SnMR_MULTI  = 0x80,
} SnMR;

typedef enum SockCMD {
  Sock_OPEN      = 0x01,
  Sock_LISTEN    = 0x02,
  Sock_CONNECT   = 0x04,
  Sock_DISCON    = 0x08,
  Sock_CLOSE     = 0x10,
  Sock_SEND      = 0x20,
  Sock_SEND_MAC  = 0x21,
  Sock_SEND_KEEP = 0x22,
  Sock_RECV      = 0x40
} SockCMD;

/*class SnCmd {
public:
  static const uint8_t OPEN      = 0x01;
  static const uint8_t LISTEN    = 0x02;
  static const uint8_t CONNECT   = 0x04;
  static const uint8_t DISCON    = 0x08;
  static const uint8_t CLOSE     = 0x10;
  static const uint8_t SEND      = 0x20;
  static const uint8_t SEND_MAC  = 0x21;
  static const uint8_t SEND_KEEP = 0x22;
  static const uint8_t RECV      = 0x40;
};
*/

typedef enum SnIR {
//public:
  SnIR_SEND_OK = 0x10,
  SnIR_TIMEOUT = 0x08,
  SnIR_RECV    = 0x04,
  SnIR_DISCON  = 0x02,
  SnIR_CON     = 0x01,
} SnIR;

typedef enum SnSR {
//public:
  SnSR_CLOSED      = 0x00,
  SnSR_INIT        = 0x13,
  SnSR_LISTEN      = 0x14,
  SnSR_SYNSENT     = 0x15,
  SnSR_SYNRECV     = 0x16,
  SnSR_ESTABLISHED = 0x17,
  SnSR_FIN_WAIT    = 0x18,
  SnSR_CLOSING     = 0x1A,
  SnSR_TIME_WAIT   = 0x1B,
  SnSR_CLOSE_WAIT  = 0x1C,
  SnSR_LAST_ACK    = 0x1D,
  SnSR_UDP         = 0x22,
  SnSR_IPRAW       = 0x32,
  SnSR_MACRAW      = 0x42,
  SnSR_PPPOE       = 0x5F,
} SnSR;

typedef enum IPPROTO {
//public:
  IPPROTO_IP   = 0,
  IPPROTO_ICMP = 1,
  IPPROTO_IGMP = 2,
  IPPROTO_GGP  = 3,
  IPPROTO_TCP  = 6,
  IPPROTO_PUP  = 12,
  IPPROTO_UDP  = 17,
  IPPROTO_IDP  = 22,
  IPPROTO_ND   = 77,
  IPPROTO_RAW  = 255,
} IPPROTO;

typedef enum W5100Enum {

//public:

//private:
  // W5100 Socket registers
  // ----------------------
  W5100_CH_BASE = 0x0400,
  W5100_CH_SIZE = 0x0100,

//private:
  W5100_RST = 7, // Reset BIT

  W5100_SOCKETS = 4,
  W5100_SMASK = 0x07FF, // Tx buffer MASK
  W5100_RMASK = 0x07FF, // Rx buffer MASK
//public:
  W5100_SSIZE = 2048, // Max Tx buffer size
//private:
  W5100_RSIZE = 2048, // Max Rx buffer size


} W5100Enum;

typedef struct W5100Class {

//public:

//private:
  uint16_t SBASE[W5100_SOCKETS]; // Tx buffer base address
  uint16_t RBASE[W5100_SOCKETS]; // Rx buffer base address

} W5100Class;

//public:
  void W5100_init(W5100Class * this);

  /**
   * @brief	This function is being used for copy the data form Receive buffer of the chip to application buffer.
   * 
   * It calculate the actual physical address where one has to read
   * the data from Receive buffer. Here also take care of the condition while it exceed
   * the Rx memory uper-bound of socket.
   */
  void W5100_read_data(W5100Class * this, SOCKET s, volatile uint8_t * src, volatile uint8_t * dst, uint16_t len);
  
  /**
   * @brief	 This function is being called by send() and sendto() function also. 
   * 
   * This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
   * register. User should read upper byte first and lower byte later to get proper value.
   */
  void W5100_send_data_processing(W5100Class * this, SOCKET s, const uint8_t *data, uint16_t len);
  /**
   * @brief A copy of send_data_processing that uses the provided ptr for the
   *        write offset.  Only needed for the "streaming" UDP API, where
   *        a single UDP packet is built up over a number of calls to
   *        send_data_processing_ptr, because TX_WR doesn't seem to get updated
   *        correctly in those scenarios
   * @param ptr value to use in place of TX_WR.  If 0, then the value is read
   *        in from TX_WR
   * @return New value for ptr, to be used in the next call
   */
// FIXME Update documentation
  void W5100_send_data_processing_offset(W5100Class * this, SOCKET s, uint16_t data_offset, const uint8_t *data, uint16_t len);

  /**
   * @brief	This function is being called by recv() also.
   * 
   * This function read the Rx read pointer register
   * and after copy the data from receive buffer update the Rx write pointer register.
   * User should read upper byte first and lower byte later to get proper value.
   */
  void W5100_recv_data_processing_3(W5100Class * this, SOCKET s, uint8_t *data, uint16_t len);
  void W5100_recv_data_processing_4(W5100Class * this, SOCKET s, uint8_t *data, uint16_t len, uint8_t peek /* = 0 */ );

  inline void W5100_setGatewayIp(uint8_t *_addr);
  inline void W5100_getGatewayIp(uint8_t *_addr);

  inline void W5100_setSubnetMask(uint8_t *_addr);
  inline void W5100_getSubnetMask(uint8_t *_addr);

  inline void W5100_setMACAddress(uint8_t * addr);
  inline void W5100_getMACAddress(uint8_t * addr);

  inline void W5100_setIPAddress(uint8_t * addr);
  inline void W5100_getIPAddress(uint8_t * addr);

  inline void W5100_setRetransmissionTime(uint16_t timeout);
  inline void W5100_setRetransmissionCount(uint8_t _retry);

  void W5100_execCmdSn(SOCKET s, SockCMD _cmd);
  
  uint16_t W5100_getTXFreeSize(SOCKET s);
  uint16_t W5100_getRXReceivedSize(SOCKET s);
  

  // W5100 Registers
  // ---------------
//private:
  extern uint8_t W5100_write_2(uint16_t _addr, uint8_t _data);
  extern uint16_t W5100_write_3(uint16_t addr, const uint8_t *buf, uint16_t len);
  extern uint8_t W5100_read_1(uint16_t addr);
  extern uint16_t W5100_read_3(uint16_t addr, uint8_t *buf, uint16_t len);
  
#define __GP_REGISTER8(name, address)             \
  inline void W5100_write##name(uint8_t _data); \
  inline void W5100_write##name(uint8_t _data) { \
    W5100_write_2(address, _data);                        \
  }                                               \
  inline uint8_t W5100_read##name(void);          \
  inline uint8_t W5100_read##name(void) {            \
    return W5100_read_1(address);                         \
  }
#define __GP_REGISTER16(name, address)            \
  void W5100_write##name(uint16_t _data);       \
  uint16_t W5100_read##name(void);
#define __GP_REGISTER_N(name, address, size)      \
  uint16_t W5100_write##name(uint8_t *_buff);   \
  uint16_t W5100_read##name(uint8_t *_buff);

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
//private:
  inline uint8_t W5100_readSn_2(SOCKET _s, uint16_t _addr);
  inline uint8_t W5100_writeSn_3(SOCKET _s, uint16_t _addr, uint8_t _data);
  inline uint16_t W5100_readSn_4(SOCKET _s, uint16_t _addr, uint8_t *_buf, uint16_t len);
  inline uint16_t W5100_writeSn_4(SOCKET _s, uint16_t _addr, uint8_t *_buf, uint16_t len);

#define __SOCKET_REGISTER8(name, address)                    \
  inline void W5100_write##name(SOCKET _s, uint8_t _data); \
  inline void W5100_write##name(SOCKET _s, uint8_t _data) { \
    W5100_writeSn_3(_s, address, _data);                             \
  }                                                          \
  inline uint8_t W5100_read##name(SOCKET _s);               \
  inline uint8_t W5100_read##name(SOCKET _s) {              \
    return W5100_readSn_2(_s, address);                              \
  }
#define __SOCKET_REGISTER16(name, address)                   \
  void W5100_write##name(SOCKET _s, uint16_t _data);       \
  uint16_t W5100_read##name(SOCKET _s);
#define __SOCKET_REGISTER_N(name, address, size)             \
  uint16_t W5100_write##name(SOCKET _s, uint8_t *_buff);   \
  uint16_t W5100_read##name(SOCKET _s, uint8_t *_buff);
  
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

inline void W5100_initSS(void);
inline void W5100_setSS(void);
inline void W5100_resetSS(void);

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  inline void W5100_initSS(void)    { DDRB  |=  _BV(4); };
  inline void W5100_setSS(void)     { PORTB &= ~_BV(4); };
  inline void W5100_resetSS(void)   { PORTB |=  _BV(4); };
#elif defined(__AVR_ATmega32U4__)
  inline void W5100_initSS(void)    { DDRB  |=  _BV(6); };
  inline void W5100_setSS(void)     { PORTB &= ~_BV(6); };
  inline void W5100_resetSS(void)   { PORTB |=  _BV(6); }; 
#elif defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB162__)
  inline void W5100_initSS(void)    { DDRB  |=  _BV(0); };
  inline void W5100_setSS(void)     { PORTB &= ~_BV(0); };
  inline void W5100_resetSS(void)   { PORTB |=  _BV(0); }; 
#else
  inline void W5100_initSS(void)    { DDRB  |=  _BV(2); };
  inline void W5100_setSS(void)     { PORTB &= ~_BV(2); };
  inline void W5100_resetSS(void)   { PORTB |=  _BV(2); };
#endif

extern W5100Class W5100;

inline uint8_t W5100_readSn_2(SOCKET _s, uint16_t _addr) {
  return W5100_read_1(W5100_CH_BASE + _s * W5100_CH_SIZE + _addr);
}

inline uint8_t W5100_writeSn_3(SOCKET _s, uint16_t _addr, uint8_t _data) {
  return W5100_write_2(W5100_CH_BASE + _s * W5100_CH_SIZE + _addr, _data);
}

inline uint16_t W5100_readSn_4(SOCKET _s, uint16_t _addr, uint8_t *_buf, uint16_t _len) {
  return W5100_read_3(W5100_CH_BASE + _s * W5100_CH_SIZE + _addr, _buf, _len);
}

inline uint16_t W5100_writeSn_4(SOCKET _s, uint16_t _addr, uint8_t *_buf, uint16_t _len) {
  return W5100_write_3(W5100_CH_BASE + _s * W5100_CH_SIZE + _addr, _buf, _len);
}

inline void W5100_getGatewayIp(uint8_t *_addr) {
  W5100_readGAR(_addr);
}

inline void W5100_setGatewayIp(uint8_t *_addr) {
  W5100_writeGAR(_addr);
}

inline void W5100_getSubnetMask(uint8_t *_addr) {
  W5100_readSUBR(_addr);
}

inline void W5100_setSubnetMask(uint8_t *_addr) {
  W5100_writeSUBR(_addr);
}

inline void W5100_getMACAddress(uint8_t *_addr) {
  W5100_readSHAR(_addr);
}

inline void W5100_setMACAddress(uint8_t *_addr) {
  W5100_writeSHAR(_addr);
}

inline void W5100_getIPAddress(uint8_t *_addr) {
  W5100_readSIPR(_addr);
}

inline void W5100_setIPAddress(uint8_t *_addr) {
  W5100_writeSIPR(_addr);
}

inline void W5100_setRetransmissionTime(uint16_t _timeout) {
  W5100_writeRTR(_timeout);
}

inline void W5100_setRetransmissionCount(uint8_t _retry) {
  W5100_writeRCR(_retry);
}

#endif
