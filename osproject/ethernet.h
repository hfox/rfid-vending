int ethernet_init(void);
int ethernet_send_byte(char c);
int ethernet_send_bytes(char *ptr, int len);
int ethernet_send_string(char *str);
#define ETHERNET_SEND_ARRAY(array) ethernet_send_bytes(array, sizeof(array))
