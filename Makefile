CC = clang++
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: target bin/main.o bin/server.o
	$(CC) bin/main.o bin/server.o bin/const.o bin/http.o $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: main.cpp bin/server.o
	$(CC) -c main.cpp -o bin/main.o

bin/server.o: src/server.hpp bin/const.o bin/http.o
	$(CC) -c src/server.cpp -o bin/server.o

bin/const.o: src/const.hpp
	$(CC) -c src/const.cpp -o bin/const.o

bin/http.o: src/http.hpp
	$(CC) -c src/http.cpp -o bin/http.o

target:
	mkdir -p bin

clean:
	rm -f bin/*.o bin/build
