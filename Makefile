CC = clang++
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: target bin/main.o bin/server.o
	$(CC) bin/main.o bin/server.o $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: main.cpp bin/server.o
	$(CC) -c main.cpp -o bin/main.o

bin/server.o: src/server.hpp
	$(CC) -c src/server.cpp -o bin/server.o

target:
	mkdir -p bin

clean:
	rm -f bin/*.o bin/build
