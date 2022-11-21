OBJS	= mines.o game.o display.o controls.o menu.o
SOURCE	= mines.c game.c display.c controls.c menu.c
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

install: 
	cp mines /usr/local/bin
