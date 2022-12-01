CC = gcc
CFLAGS = -Wall -O3

all: hijack

hijack:
	$(CC) $(CFLAGS) -FPIC -shared hijack.c -o hijack.so

clean:
	rm -f hijack.so