#include <arduino/Arduino.h>

#include "FreeRTOS.h"
#include "task.h"
#include "parallel.h"

#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define A6 20
#define A7 21

static const uint8_t arduino_pins[] = {A0, A1, A2, A3, A4, A5, A6, A7};

void parallel_send_array(char *c)
{
	taskENTER_CRITICAL();

	digitalWrite(A0, c[0]);
	digitalWrite(A1, c[1]);
	digitalWrite(A2, c[2]);
	digitalWrite(A3, c[3]);
	digitalWrite(A4, c[4]);
	digitalWrite(A5, c[5]);
// 	digitalWrite(A6, c[6]);
// 	digitalWrite(A7, c[7]);

	taskEXIT_CRITICAL();
}

void parallel_send_byte(int c)
{
	taskENTER_CRITICAL();

	digitalWrite(A0, c & 1);
	digitalWrite(A1, c & 2);
	digitalWrite(A2, c & 4);
	digitalWrite(A3, c & 8);
	digitalWrite(A4, c & 16);
	digitalWrite(A5, c & 32);
// 	digitalWrite(A6, c & 64);
// 	digitalWrite(A7, c & 128);

	taskEXIT_CRITICAL();
}

void parallel_send_bit(int index, int val)
{
	if (index > 5)
		return;

	int pin = arduino_pins[index];
	digitalWrite(pin, val);
}

void parallel_init(void)
{
	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	pinMode(A4, OUTPUT);
	pinMode(A5, OUTPUT);
	pinMode(A6, OUTPUT);
	pinMode(A7, OUTPUT);
}
