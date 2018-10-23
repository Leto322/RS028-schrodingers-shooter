CC=g++
GLFLAGS= -std=c++17 -lGL -lglut -lGLU
CFLAGS=-lm

all: SchShooter

SchShooter: main.o image.o player.o geometry.o items.o
	$(CC) main.o image.o player.o geometry.o -o SchShooter $(GLFLAGS) $(CFLAGS)
	
main.o: source/main.cpp
	$(CC) -c source/main.cpp $(GLFLAGS) $(CFLAGS)
	
image.o: source/image.cpp header/image.h
	$(CC) -c source/image.cpp $(CFLAGS)
	
player.o: source/player.cpp header/player.h
	$(CC) -c source/player.cpp $(CFLAGS)

geometry.o: source/geometry.cpp header/geometry.h
	$(CC) -c source/geometry.cpp $(CFLAGS)

items.o: source/items.cpp header/items.h
	$(CC) -c source/items.cpp $(CFLAGS)
	
clean:
	rm -rf *.o SchShooter
