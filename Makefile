CC = clang++ -std=c++20
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: bin/main.o
	$(CC) bin/main.o bin/LinkedPair.o $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: main.cpp bin/LinkedPair.o
	$(CC) -c main.cpp -o bin/main.o

bin/LinkedPair.o: LinkedPair.hpp LinkedPair.cpp
	$(CC) -c LinkedPair.cpp -o bin/LinkedPair.o

target:
	mkdir -p bin

clean:
	rm -f bin/*.o bin/build
