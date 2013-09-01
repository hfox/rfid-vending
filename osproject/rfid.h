#define mainRFID_CHECK_PERIOD			( ( portTickType ) 3000 / portTICK_RATE_MS  )

void vListenToRfidReader(void *pvParameters );

int rfid_has_card(void);
void rfid_reset_card(void);
void rfid_get_card(char *dest);

int rfid_has_timeout(void);
void rfid_reset_has_timeout(void);

