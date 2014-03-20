void serial_send_byte(int c);
void serial_send_bytes(char *str, int len);
void serial_send_string(char *str);
void serial_init(void);

#define SERIAL_SEND_ARRAY(array) serial_send_bytes(array, sizeof(array));
