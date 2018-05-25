NAME=interpret
FILES=main.c garbage_collector.c scanner.c precedence.c ilist.c parser.c ial.c interpret.c
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic  -lm 
#VPATH=./src/
SRCDIR=./src
INCLUDE=../include

$(NAME): $(FILES)
	$(CC) -I $(INCLUDE)  $(CFLAGS) $(FILES) -o $@

