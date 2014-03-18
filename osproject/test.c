/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "Arduino.h"

#include "HardwareSerial.h"
#include "test.h"

static unsigned int count = 0;
static unsigned int toggle = 0;
static unsigned int recv = 0;

static int crashed = 0;

void send_string(char *str)
{
	char *p = str, c;
	while ((c = *p++)) {
		HardwareSerial_write(&Serial, c);
	}
}

void test_run(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(; crashed == 0;) {
		vTaskDelay( mainTEST_CHECK_PERIOD );
		count++;

		toggle = !toggle;
		recv = digitalRead(2);
		//digitalWrite(3, toggle);
		//digitalWrite(3, recv);
		
		send_string("Hej hej. \n");
	}
	for(;;) {} // Trap
}

void test_init(void)
{
	// Test send init
	pinMode(2, INPUT);
	pinMode(3, OUTPUT);

	HardwareSerial_global_init();
	HardwareSerial_begin_1(&Serial, 9600);
}

char test_running(void)
{
	static unsigned int saved_count = 0;

	if (count == saved_count) {
		saved_count = count;
		return 0;
	} else {
		saved_count = count;
		return 1;
	}
}


