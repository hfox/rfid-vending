void serial_send_byte(int c);
void serial_send_bytes(char *str, int len);
void serial_send_string(char *str);
void serial_send_buffer();

int serial_get_tx_buffer_usage(void);
int serial_get_tx_buffer_free(void);
int serial_get_rx_buffer_usage(void);
int serial_get_rx_buffer_free(void);
int serial_get_tx_head(void);
int serial_get_tx_tail(void);
int serial_get_rx_head(void);
int serial_get_rx_tail(void);

void serial_init(void);

#define SERIAL_SEND_ARRAY(array) serial_send_bytes(array, sizeof(array));
