CC = clang++
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: target bin/main.o bin/server.o
	$(CC) bin/main.o bin/server.o bin/const.o bin/http.o bin/route.o bin/convert.o bin/lua.o $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: main.cpp bin/server.o bin/lua.o
	$(CC) -c main.cpp -o bin/main.o

bin/server.o: src/server.hpp src/server.cpp bin/const.o bin/http.o bin/route.o bin/convert.o
	$(CC) -c src/server.cpp -o bin/server.o

bin/const.o: src/const.hpp src/const.cpp
	$(CC) -c src/const.cpp -o bin/const.o

bin/http.o: src/http.hpp src/http.cpp
	$(CC) -c src/http.cpp -o bin/http.o

bin/route.o: src/route.hpp src/route.cpp
	$(CC) -c src/route.cpp -o bin/route.o

bin/convert.o: src/convert.hpp src/convert.cpp
	$(CC) -c src/convert.cpp -o bin/convert.o

bin/lua.o: src/lua.hpp src/lua.cpp bin/server.o
	$(CC) -c src/lua.cpp -o bin/lua.o

target:
	mkdir -p bin

clean:
	rm -f bin/*.o bin/build
