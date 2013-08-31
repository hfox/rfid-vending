CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb
LDFLAGS=-shared -lphidget21
WORKAROUNDFLAGS=
BINS=client RFID-simple client-workaround
LIBFILES=/usr/local/lib/libphidget21.so

all: client

clean:
	rm -f $(BINS) *.o

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

client: client.o 
	$(CC) $(LDFLAGS) -o $@ $<

client-workaround: client.o
	$(CC) $(WORKAROUNDFLAGS) -o $@ $< $(LIBFILES)

