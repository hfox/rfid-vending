#include <arduino/Arduino.h>
#include <arduino/HardwareSerial.h>

#include "serial.h"

#define SERIAL_SEND_ARRAY(array) serial_send_bytes(array, sizeof(array));

void serial_send_byte(int c)
{
	HardwareSerial_write(&Serial, c);
}

void serial_send_bytes(char *ptr, int len)
{
	char *c = ptr;
	for (int i = 0; i < len; ++i) {
		HardwareSerial_write(&Serial, *c);
		++c;
	}
}

void serial_send_string(char *str)
{
	char *p = str, c;
	while ((c = *p++)) {
		HardwareSerial_write(&Serial, c);
	}
}

void serial_init(void)
{
	HardwareSerial_global_init();
	HardwareSerial_begin_1(&Serial, 9600);
}
