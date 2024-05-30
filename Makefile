CC = clang++ -std=c++20
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

INCLUDE = bin/main.o bin/Server.o bin/LinkedPair.o bin/HTML.o \
	bin/http/Route.o bin/http/Request.o bin/http/utils.o \
	bin/lua/export.o bin/lua/import.o

build: $(INCLUDE)
	$(CC) $^ $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: src/main.cpp
	$(CC) -c $^ -o $@

bin/Server.o: src/Server.hpp src/Server.cpp
	$(CC) -c src/Server.cpp -o $@

bin/LinkedPair.o: src/LinkedPair.hpp src/LinkedPair.cpp
	$(CC) -c src/LinkedPair.cpp -o $@

bin/HTML.o: src/HTML.hpp src/HTML.cpp
	$(CC) -c src/HTML.cpp -o $@

bin/http/utils.o: src/http/utils.hpp src/http/utils.cpp
	$(CC) -c src/http/utils.cpp -o $@

bin/http/Route.o: src/http/Route.hpp src/http/Route.cpp
	$(CC) -c src/http/Route.cpp -o $@

bin/http/Request.o: src/http/Request.hpp src/http/Request.cpp
	$(CC) -c src/http/Request.cpp -o $@

bin/lua/export.o: src/lua/export.hpp src/lua/export.cpp
	$(CC) -c src/lua/export.cpp -o $@

bin/lua/import.o: src/lua/import.hpp src/lua/import.cpp
	$(CC) -c src/lua/import.cpp -o $@

target:
	mkdir -p bin
	mkdir -p bin/http
	mkdir -p bin/lua

clean:
	rm -f bin/*.o bin/http/*.o bin/lua/*.o bin/build
