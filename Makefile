SRCS =$(wildcard src/*.c)
OBJS	= $(patsubst src/%.c, obj/%.o, $(SRCS))
CC	 = cc
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
	cp deb/mines_1.0.0_amd64/usr/share/applications/mines.desktop /usr/share/applications/
	cp deb/mines_1.0.0_amd64/usr/share/icons/hicolor/128x128/apps/mines.png /usr/share/icons/hicolor/128x128/apps/
