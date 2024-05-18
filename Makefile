CC = clang++
CFLAGS = -g -Wall

LUA_CFLAGS = -I/usr/local/include
LUA_LDLIBS = -L/usr/local/lib -llua -lm

build: target bin/main.o
	$(CC) bin/main.o $(CFLAGS) -o bin/build $(LUA_CFLAGS) $(LUA_LDLIBS)

bin/main.o: main.cpp
	$(CC) -c main.cpp -o bin/main.o

target:
	mkdir -p bin

clean:
	rm bin/*.o build
