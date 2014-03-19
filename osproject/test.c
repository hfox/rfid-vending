/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "Arduino.h"

#include "HardwareSerial.h"
#include "test.h"

#if defined(UBRRH)
 #warning "HwSerial: Serial using USART"
#endif
#if defined(UBRR0H)
 #warning "HwSerial: Serial using USART0"
#endif
#if defined(UBRR1H)
 #warning "HwSerial: Serial1 using USART1"
#endif
#if defined(UBRR1H)
 #warning "HwSerial: Serial2 using USART2"
#endif
#if defined(UBRR1H)
 #warning "HwSerial: Serial3 using USART3"
#endif

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


