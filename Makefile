OBJS	= mines.o
SOURCE	= mines.c
HEADER	= 
OUT	= mines
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lbsd -lncurses

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

mines.o: mines.c
	$(CC) $(FLAGS) mines.c 


clean:
	rm -f $(OBJS) $(OUT)
