BINS=keygen

CFLAGS=-Wall -Wextra -ggdb

.PHONY: all clean

all: $(BINS)

clean:
	-rm -f *.o
	-rm -f *.lst
	-rm -f *.d
	-rm -f $(BINS)

keygen: keygen.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


