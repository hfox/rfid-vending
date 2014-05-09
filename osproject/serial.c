#include <arduino/Arduino.h>
#include <arduino/HardwareSerial.h>

#include "FreeRTOS.h"
#include "task.h"
#include "serial.h"

#define SERIAL_SEND_ARRAY(array) serial_send_bytes(array, sizeof(array));

void serial_send_byte(int c)
{
	taskENTER_CRITICAL();
	HardwareSerial_buffer(&Serial, c);
	taskEXIT_CRITICAL();
}

void serial_send_bytes(char *ptr, int len)
{
	char *c = ptr;
	taskENTER_CRITICAL();
	for (int i = 0; i < len; ++i) {
		HardwareSerial_buffer(&Serial, *c);
		++c;
	}
	taskEXIT_CRITICAL();
}

void serial_send_string(char *str)
{
	char *p = str, c;
	taskENTER_CRITICAL();
	while ((c = *p++)) {
		HardwareSerial_buffer(&Serial, c);
	}
	taskEXIT_CRITICAL();
}

void serial_send_buffer(void)
{
	taskENTER_CRITICAL();
	HardwareSerial_write_buffer(&Serial);
	taskEXIT_CRITICAL();
}

int serial_get_tx_buffer_usage(void)
{
	return HardwareSerial_get_tx_buffer_usage(&Serial);
}

int serial_get_tx_buffer_free(void)
{
	return HardwareSerial_get_tx_buffer_free(&Serial);
}

int serial_get_rx_buffer_usage(void)
{
	return HardwareSerial_get_rx_buffer_usage(&Serial);
}

int serial_get_rx_buffer_free(void)
{
	return HardwareSerial_get_rx_buffer_free(&Serial);
}

int serial_get_tx_head(void)
{
	return HardwareSerial_get_tx_head(&Serial);
}

int serial_get_tx_tail(void)
{
	return HardwareSerial_get_tx_tail(&Serial);
}

int serial_get_rx_head(void)
{
	return HardwareSerial_get_rx_head(&Serial);
}

int serial_get_rx_tail(void)
{
	return HardwareSerial_get_rx_tail(&Serial);
}

void serial_init(void)
{
	HardwareSerial_global_init();
	HardwareSerial_begin_1(&Serial, 9600);
}
