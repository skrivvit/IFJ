NAME=interpret
FILES=main.c garbage_collector.c scanner.c precedence.c ilist.c parser.c ial.c interpret.c
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic  -lm

$(NAME): $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@

