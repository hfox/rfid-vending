CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb
LDFLAGS=-shared -lphidget21
BINS=client RFID-simple

all: client

clean:
	rm -f $(BINS) *.o

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

client: client.o 
	$(CC) $(LDFLAGS) -o $@ $<


