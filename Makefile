CC = gcc
CFLAGS = -Wall

SRC = src/main.c src/server.c
OUT = build/server

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf build/