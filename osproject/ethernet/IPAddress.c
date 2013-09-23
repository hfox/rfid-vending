#include "EthernetConfig.h"

#include <Arduino.h>
#include <IPAddress.h>

#define bool char

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* IPAddress_raw_address(IPAddress * this)
          { return this->_address; };

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    uint32_t operator_uint32_t_IPAddress(IPAddress * this)
          { return *((uint32_t*)(this->_address)); };
    bool operator_equals_IPAddress_ptr(IPAddress * this, const IPAddress * addr)
          { return (*((uint32_t*)(this->_address))) == (*((uint32_t*)(addr->_address))); };

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator_index_IPAddress_val(IPAddress * this, int index)
          { return this->_address[index]; };
    uint8_t * operator_index_IPAddress_ptr(IPAddress * this, int index)
          { return &(this->_address[index]); };

IPAddress INADDR_NONE;
void IPAddress_global_init(void)
{
  construct_IPAddress_4(&INADDR_NONE,0,0,0,0);
}


void construct_IPAddress_0(IPAddress * this)
{
    memset(this->_address, 0, sizeof(this->_address));
}

void construct_IPAddress_4(IPAddress * this, uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->_address[0] = first_octet;
    this->_address[1] = second_octet;
    this->_address[2] = third_octet;
    this->_address[3] = fourth_octet;
}

void construct_IPAddress_int(IPAddress * this, uint32_t address)
{
    memcpy(this->_address, &address, sizeof(this->_address));
}

void construct_IPAddress_intptr(IPAddress * this, const uint8_t *address)
{
    memcpy(this->_address, address, sizeof(this->_address));
}

IPAddress * operator_assign_IPAddress_intptr(IPAddress * this, const uint8_t *address)
{
    memcpy(this->_address, address, sizeof(this->_address));
    return this;
}

IPAddress * operator_assign_IPAddress_int(IPAddress * this, uint32_t address)
{
    memcpy(this->_address, (const uint8_t *)&address, sizeof(this->_address));
    return this;
}

bool operator_equals_IPAddress_intptr(IPAddress * this, const uint8_t* addr)
{
    return memcmp(addr, this->_address, sizeof(this->_address)) == 0;
}

#ifdef HAS_ETHERNET_PRINT
size_t IPAddress_printTo(IPAddress * this, Print& p) const
{
    size_t n = 0;
    for (int i =0; i < 3; i++)
    {
        n += p.print(this->_address[i], DEC);
        n += p.print('.');
    }
    n += p.print(this->_address[3], DEC);
    return n;
}
#endif /* HAS_ETHERNET_PRINT */

#undef bool

