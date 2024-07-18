
CC = g++
CFLAGS = -std=c++17 -Wall -pedantic -O4
CLEAN = rm 
APP = tictac

all: constants.hpp main.o game.o board.o move.o
	$(CC) $(CFLAGS) main.o game.o board.o move.o -o $(APP)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

game.o: game.hpp game.cpp
	$(CC) $(CFLAGS) -c game.cpp

board.o: board.hpp board.cpp 	
	$(CC) $(CFLAGS) -c board.cpp

move.o: move.hpp move.cpp
	$(CC) $(CFLAGS) -c move.cpp

clean:
	$(CLEAN) *.o $(APP)
