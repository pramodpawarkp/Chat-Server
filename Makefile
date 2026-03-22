CC = gcc
CFLAGS = -Wall -pthread

SRC = src/main.c src/server.c src/client_handler.c src/protocol.c
OUT = build/server

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf build/