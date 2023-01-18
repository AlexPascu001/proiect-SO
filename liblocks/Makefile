CC=gcc
CFLAGS=-c -Wall -fPIC
LDFLAGS=-shared

SOURCES=mutex.c semaphore.c rwlock.c
OBJECTS=$(SOURCES:.c=.o)

TARGET=liblocks.so

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

install:
	cp $(TARGET) /usr/local/lib
	ldconfig

uninstall:
	rm /usr/local/lib/$(TARGET)
	ldconfig

clean:
	rm -f $(OBJECTS) $(TARGET)

