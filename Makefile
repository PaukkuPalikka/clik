CC=gcc
CFLAGS=-Wall -Werror -pedantic

# TODO: make a proper make system

all:
	$(CC) -c main.c -o main.o
	$(CC) main.o -o clik

clean:
	rm main.o clik

