
CC = gcc
CFLAGS = -Wall -DPLATFORM_DESKTOP

all: bf++ compiled

bf++: main.c
	$(CC) $(CFLAGS) -o $@ $^ 

compiled: main.c
	$(CC) $(CFLAGS) -DCOMPILED -o $@ $^

clean:
	rm -f bf++ compiled 

