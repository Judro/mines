SRCS =$(wildcard src/*.c)
OBJS	= $(patsubst src/%.c, obj/%.o, $(SRCS))
CC	 = gcc
FLAGS	 = -Wall -O2
LFLAGS	 = -lncursesw
BIN = bin/mines

all:$(BIN)

$(BIN): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@ $(LFLAGS) 
obj/%.o: src/%.c
	$(CC) $(FLAGS)  -c $< -o $@

clean:
	$(RM) -r bin/* obj/*

install: 
	cp bin/mines /usr/local/bin
