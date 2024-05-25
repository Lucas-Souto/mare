CC = clang++ -std=c++20
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: target bin/main.o bin/server.o
	$(CC) bin/main.o bin/server.o bin/const.o bin/http.o bin/route.o bin/lua.o bin/response.o $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: main.cpp bin/server.o bin/lua.o
	$(CC) -c main.cpp -o bin/main.o

bin/server.o: src/server.hpp src/server.cpp bin/const.o bin/http.o bin/route.o bin/response.o
	$(CC) -c src/server.cpp -o bin/server.o

bin/const.o: src/const.hpp src/const.cpp
	$(CC) -c src/const.cpp -o bin/const.o

bin/http.o: src/http.hpp src/http.cpp
	$(CC) -c src/http.cpp -o bin/http.o

bin/route.o: src/route.hpp src/route.cpp
	$(CC) -c src/route.cpp -o bin/route.o

bin/lua.o: src/lua.hpp src/lua.cpp bin/server.o bin/response.o
	$(CC) -c src/lua.cpp -o bin/lua.o

bin/response.o: src/response.hpp src/response.cpp
	$(CC) -c src/response.cpp -o bin/response.o

target:
	mkdir -p bin

clean:
	rm -f bin/*.o bin/build
