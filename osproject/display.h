#define mainDISPLAY_PERIOD		( ( portTickType ) 100 / portTICK_RATE_MS  )

void display_run(void *pvParameters );
void display_step(void);
void display_init(void);

void display_set_error(void);
void display_set_has_card(void);
void display_set_accept(void);
void display_set_deny(void);

char display_running(void);

