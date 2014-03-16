/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "Arduino.h"

#include "HardwareSerial.h"
#include "test.h"

static unsigned int count = 0;
static unsigned int toggle = 0;
static unsigned int recv = 0;

// static char rxbuf[100];
// static char txbuf[100];

static ring_buffer _rxbuf, *rxbuf = &_rxbuf;
static ring_buffer _txbuf, *txbuf = &_txbuf;

static HardwareSerial _hwserial;
static HardwareSerial *hwserial = &_hwserial;

#define regvar(name,val) static uint8_t _ ## name = val, *name = & _ ## name ;

regvar(ubrrh,0)
regvar(ubrrl,0)
regvar(ucsra,0)
regvar(ucsrb,0)
regvar(ucsrc,0)
regvar(udr,0)

void test_run(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainTEST_CHECK_PERIOD );
		count++;

		toggle = !toggle;
		recv = digitalRead(2);
		//digitalWrite(3, toggle);
		//digitalWrite(3, recv);
	}
}

void test_init(void)
{
	// Test send init
	pinMode(2, INPUT);
	pinMode(3, OUTPUT);

	
	construct_HardwareSerial(hwserial, rxbuf, txbuf,
		ubrrh, ubrrl, ucsra, ucsrb, ucsrc, udr,
		0, 0, 0, 0, 0);
		
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


