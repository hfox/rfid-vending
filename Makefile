CFLAGS=-Wall -Wextra -pedantic


client: client.c
	$(CC) $(CFLAGS) -lphidget21 -o $@ $<