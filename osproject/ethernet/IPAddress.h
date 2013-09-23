/*
 *
 * MIT License:
 * Copyright (c) 2011 Adrian McEwen
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
 * adrianm@mcqn.com 1/1/2011
 */

#ifndef IPAddress_h
#define IPAddress_h

#ifdef HAS_ETHERNET_PRINT
#include <Printable.h>
#endif /* HAS_ETHERNET_PRINT */

// A class to make it easier to handle and pass around IP addresses

#define virtual
#define bool char

typedef struct IPAddress {
//private:
    uint8_t _address[4];  // IPv4 address
} IPAddress;

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* IPAddress_raw_address(IPAddress * this);

//public:
    // Constructors
    void construct_IPAddress_0(IPAddress * this);
    void construct_IPAddress_4(IPAddress * this,
        uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void construct_IPAddress_int(IPAddress * this, uint32_t address);
    void construct_IPAddress_intptr(IPAddress * this, const uint8_t *address);

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    uint32_t operator_uint32_t_IPAddress(IPAddress * this);
    bool operator_equals_IPAddress_ptr(IPAddress * this, const IPAddress * addr);
    bool operator_equals_IPAddress_intptr(IPAddress * this, const uint8_t* addr);

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator_index_IPAddress_val(IPAddress * this, int index);
    uint8_t * operator_index_IPAddress_ptr(IPAddress * this, int index);

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    IPAddress * operator_assign_IPAddress_intptr(IPAddress * this, const uint8_t *address);
    IPAddress * operator_assign_IPAddress_int(IPAddress * this, uint32_t address);

#ifdef HAS_ETHERNET_PRINT
    virtual size_t printTo(Print& p) const;
#endif /* HAS_ETHERNET_PRINT */

extern IPAddress INADDR_NONE;
void IPAddress_global_init(void);

#undef virtual
#undef bool

#endif
