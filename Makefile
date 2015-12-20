CFLAGS := -Wall -Wextra -pedantic -ansi -g -O2 -pipe
CC := gcc
RM := rm -f

all: main

main: main.o

.PHONY: run
run: main
	sudo ./$<

.PHONY: clean
clean:
	$(RM) main
	$(RM) main.o
