CC = clang++ -std=c++20
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: bin/main.o bin/LinkedPair.o bin/http/Route.o bin/http/utils.o
	$(CC) $^ $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: src/main.cpp
	$(CC) -c src/main.cpp -o $@

bin/LinkedPair.o: src/LinkedPair.hpp src/LinkedPair.cpp
	$(CC) -c src/LinkedPair.cpp -o $@

bin/http/utils.o: src/http/utils.hpp src/http/utils.cpp
	$(CC) -c src/http/utils.cpp -o $@

bin/http/Route.o: src/http/Route.hpp src/http/Route.cpp
	$(CC) -c src/http/Route.cpp -o $@

target:
	mkdir -p bin
	mkdir -p bin/http

clean:
	rm -f bin/*.o bin/http/*.o bin/build
