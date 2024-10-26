CC=gcc
CFLAGS=-Wall -Wextra -ggdb
BIN=cdf

all: main.o
	$(CC) $(CFLAGS) $^ -o $(BIN)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(BIN)

.PHONY: clean
