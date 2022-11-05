PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC :=gcc
CFLAGS := -Werror -Wall
LDFLAGS := -lrt -lpthread -g
OBJS  := Player GameMaster

all: Player GameMaster

Player: Player.c
	$(CC) $(CFLAGS) Player.c util.c -o Player $(LDFLAGS) -DSTART=startPlayer

GameMaster: GameMaster.c
	$(CC) $(CFLAGS) GameMaster.c util.c -o GameMaster $(LDFLAGS) -DSTART=startGameMaster

run:
	./Player

clean:
	rm -fr ASS4 $(OBJS)