#define mainDISPLAY_PERIOD		( ( portTickType ) 3000 / portTICK_RATE_MS  )

void vDisplay(void *pvParameters );

void display_set_error(void);
void display_set_has_card(void);
void display_set_accept(void);
void display_set_deny(void);
