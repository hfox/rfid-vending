CC=gcc
WARNINGS=-Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS=$(WARNINGS) -ggdb -fPIC
LDFLAGS=-shared -lphidget21
WORKAROUNDFLAGS=
BINS=client client-workaround
LIBFILES=/usr/local/lib/libphidget21.so

.PHONY: all clean

all: $(BINS)

clean:
	rm -f $(BINS) *.o

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

client: client.o 
	$(CC) $(LDFLAGS) -o $@ $<

client-workaround: client.o
	$(CC) $(WORKAROUNDFLAGS) -o $@ $< $(LIBFILES)

