OBJS = conio.o minesweeper.o
CFLAGS = -Wall
CC = gcc
TARGET = minesweeper
.PHONY: clean

all: $(OBJS)
	$(CC) -o $(TARGET) $^
%.o: %.c
	$(CC) $(CFLAGS) -c $<
