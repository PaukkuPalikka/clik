CC=gcc
# TODO: some flag for adding -g
# apparently didn't even use this
CFLAGS=-Wall -Werror -pedantic -g

# TODO: make a proper make system

all:
	$(CC) -c -g main.c -o main.o
	$(CC) -c -g lexer.c -o lexer.o
	$(CC) main.o lexer.o -o clik -g

clean:
	rm main.o lexer.o clik

