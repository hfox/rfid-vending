#define mainDATABASE_CHECK_PERIOD		( ( portTickType ) 3000 / portTICK_RATE_MS  )

#define networkACCEPT		1
#define networkDENY		2

void vListenToDatabaseServer(void *pvParameters );

void network_set_price(int price);
void network_set_card(char *source);
void network_set_send(void);

int network_has_reply(void);
int network_get_reply(void);
void network_reset_reply(void);

