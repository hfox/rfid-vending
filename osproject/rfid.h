#define mainRFID_CHECK_PERIOD			( ( portTickType ) 10 / portTICK_RATE_MS  )

void rfid_run(void *pvParameters );
void rfid_step(void);
void rfid_init(void);

int rfid_has_card(void);
void rfid_reset_card(void);
void rfid_get_card(char *dest);

int rfid_has_timeout(void);
void rfid_reset_has_timeout(void);

char rfid_running(void);
