CC = gcc
CFLAGS = -I../liblocks -pthread
LDFLAGS = -llocks

SOURCES = $(wildcard *.c)
EXECUTABLES = $(patsubst %.c,%,$(SOURCES))

all: $(EXECUTABLES)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(EXECUTABLES)
