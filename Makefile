SRCS =$(wildcard src/*.c)
OBJS	= $(patsubst src/%.c, obj/%.o, $(SRCS))
CC	 = cc
FLAGS	 = -Wall -O2
LFLAGS	 = -lncursesw
BIN = bin/mines-tui

all:$(BIN)

$(BIN): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@ $(LFLAGS) 
obj/%.o: src/%.c config.h
	$(CC) $(FLAGS)  -c $< -o $@

clean:
	$(RM) -r bin/* obj/*

install: 
	cp bin/mines-tui /usr/local/bin/
	cp man/mines-tui.6 /usr/share/man/man6/
	mkdir -p /var/games/mines/save/
	chmod -R 777 /var/games/mines/save/
