CC = clang++ -std=c++20
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: bin/main.o bin/LinkedPair.o bin/http/utils.o
	$(CC) $^ $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: main.cpp
	$(CC) -c main.cpp -o $@

bin/LinkedPair.o: LinkedPair.hpp LinkedPair.cpp
	$(CC) -c LinkedPair.cpp -o $@

bin/http/utils.o: http/utils.hpp http/utils.cpp
	$(CC) -c http/utils.cpp -o $@

target:
	mkdir -p bin
	mkdir -p bin/http

clean:
	rm -f bin/*.o bin/http/*.o bin/build
