CFLAGS=-Wall -Wextra -pedantic
LDFLAGS=-lphidget21
BINS=client RFID-simple

all: client RFID-simple

clean:
	rm -f $(BINS) *.o

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

client: client.o 
	$(CC) $(LDFLAGS) -o $@ $<

RFID-simple:RFID-simple.o
	$(CC) $(LDFLAGS) -o $@ $<


