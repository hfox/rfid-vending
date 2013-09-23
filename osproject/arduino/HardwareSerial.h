/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include "ArduinoConfig.h"

#include <inttypes.h>

//#include "Stream.h"

#define virtual
#define bool char

struct ring_buffer;
typedef struct ring_buffer ring_buffer;

typedef struct HardwareSerial
{
//  private:
    ring_buffer *_rx_buffer;
    ring_buffer *_tx_buffer;
    volatile uint8_t *_ubrrh;
    volatile uint8_t *_ubrrl;
    volatile uint8_t *_ucsra;
    volatile uint8_t *_ucsrb;
    volatile uint8_t *_ucsrc;
    volatile uint8_t *_udr;
    uint8_t _rxen;
    uint8_t _txen;
    uint8_t _rxcie;
    uint8_t _udrie;
    uint8_t _u2x;
    bool transmitting;
} HardwareSerial;

//  public:
    void construct_HardwareSerial(HardwareSerial * this,
      ring_buffer *rx_buffer, ring_buffer *tx_buffer,
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x);
    void HardwareSerial_begin_1(HardwareSerial * this, unsigned long);
    void HardwareSerial_begin_2(HardwareSerial * this, unsigned long, uint8_t);
    void HardwareSerial_end(HardwareSerial * this);
    virtual int HardwareSerial_available(HardwareSerial * this);
    virtual int HardwareSerial_peek(HardwareSerial * this);
    virtual int HardwareSerial_read(HardwareSerial * this);
    virtual void HardwareSerial_flush(HardwareSerial * this);
    virtual size_t HardwareSerial_write(HardwareSerial * this, uint8_t c);
//    inline size_t HardwareSerial_write(HardwareSerial * this, unsigned long n)
//        { return HardwareSerial_write(this, (uint8_t)n); }
//    inline size_t HardwareSerial_write(HardwareSerial * this, long n)
//        { return HardwareSerial_write(this, (uint8_t)n); }
//    inline size_t HardwareSerial_write(HardwareSerial * this, unsigned int n)
//        { return HardwareSerial_write(this, (uint8_t)n); }
//    inline size_t HardwareSerial_write(HardwareSerial * this, int n)
//        { return HardwareSerial_write(this, (uint8_t)n); }
//    operator bool();

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

#if defined(UBRRH) || defined(UBRR0H)
  extern HardwareSerial Serial;
#elif defined(USBCON)
  #include "USBAPI.h"
//  extern HardwareSerial Serial_;  
#endif
#if defined(UBRR1H)
  extern HardwareSerial Serial1;
#endif
#if defined(UBRR2H)
  extern HardwareSerial Serial2;
#endif
#if defined(UBRR3H)
  extern HardwareSerial Serial3;
#endif

extern void serialEventRun(void) __attribute__((weak));

#undef virtual
#undef bool

#endif
