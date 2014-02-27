#define mainVENDING_CHECK_PERIOD		( ( portTickType ) 10 / portTICK_RATE_MS  )

#define vendingSTATE_PENDING			1
#define vendingSTATE_SUCCESS			2
#define vendingSTATE_FAIL			3

void vListenToVendingMachine(void *pvParameters );
void vending_init(void);

void vending_set_accept(void);
void vending_set_deny(void);

int vending_has_price(void);
void vending_reset_price(void);
int vending_get_price(void);

int vending_has_state(void);
void vending_reset_state(void);
int vending_get_state(void);

char vending_running(void);
