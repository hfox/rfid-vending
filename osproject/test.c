/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "Arduino.h"

#include "HardwareSerial.h"
#include "test.h"
#include "serial.h"

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

// Used for test code in this file, not for USART
#define RXPIN 0
#define TXPIN 1

static unsigned int count = 0;
static unsigned int toggle = 0;
static unsigned int recv = 0;

static int crashed = 0;

void test_run(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(; crashed == 0;) {
		vTaskDelay( mainTEST_CHECK_PERIOD );
		count++;

		toggle = !toggle;
	//	recv = digitalRead(RXPIN);
	//	digitalWrite(TXPIN, toggle);
	//	digitalWrite(TXPIN, recv);
		
		serial_send_string("Test\n");
		serial_send_buffer();
	}
	for(;;) {} // Trap
}

void test_step(void)
{
	serial_send_string("Test\n");
}

void test_init(void)
{
	// Test send init
	pinMode(RXPIN, INPUT);
	pinMode(TXPIN, OUTPUT);

	serial_init();

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


