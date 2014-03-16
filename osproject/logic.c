/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "logic.h"

#include "vending.h"
#include "rfid.h"
#include "network.h"
#include "display.h"

static unsigned int count = 0;

static char card[20];
static int price;
static int reply;
static int vending_state;

static int state = logicSTATE_IDLE;
static int next = logicSTATE_IDLE;

void logic_run(void *pvParameters)
{
	/* The parameters are not used. */
	(void) pvParameters;

	for(;;) {
		vTaskDelay( mainMAIN_LOGIC_CHECK_PERIOD );
		count++;

		switch (state) {
			case logicSTATE_IDLE:
				if (vending_has_price()) {
					vending_reset_price();
					vending_set_deny();
					display_set_error();
				} else if (rfid_has_card()) {
					rfid_get_card(card);
					rfid_reset_card();
					display_set_has_card();
					next = logicSTATE_HAS_CARD;
				}
				break;
			case logicSTATE_HAS_CARD:
				if (vending_has_price()) {
					price = vending_get_price();
					vending_reset_price();
					network_set_card(card);
					network_set_price(price);
					network_set_send();
					next = logicSTATE_CHECKING;
				} else if (rfid_has_timeout()) {
					rfid_reset_has_timeout();
					display_set_deny();
					next = logicSTATE_IDLE;
				}
				break;
			case logicSTATE_CHECKING:
				if (network_has_reply()) {
					reply = network_get_reply();
					network_reset_reply();
					switch (reply) {
						case networkACCEPT:
							vending_set_accept();
							display_set_accept();
							next = logicSTATE_BUYING;
							break;
						case networkDENY:
							vending_set_deny();
							display_set_deny();
							next = logicSTATE_BUYING;
							break;
						default:
							vending_set_deny();
							display_set_error();
							next = logicSTATE_IDLE;
							break;
					}
				}
				break;
			case logicSTATE_BUYING:
				if (vending_has_state()) {
					vending_state = vending_get_state();
					vending_reset_state();
					switch (vending_state) {
						case vendingSTATE_PENDING:
							break;
						case vendingSTATE_SUCCESS:
							next = logicSTATE_IDLE;
							break;
						case vendingSTATE_FAIL:
							display_set_error();
							next = logicSTATE_IDLE;
							break;
					}
				}
				break;
		}
		state = next;
	}
}

void logic_init(void)
{
}

char logic_running(void)
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

