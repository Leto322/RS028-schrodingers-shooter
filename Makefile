CC=g++
GLFLAGS=-lGL -lglut -lGLU
CFLAGS=-lm

all: SchShooter

SchShooter: main.o image.o player.o
	$(CC) main.o image.o player.o -o SchShooter $(GLFLAGS) $(CFLAGS)
	
main.o: source/main.cpp
	$(CC) -c source/main.cpp $(GLFLAGS) $(CFLAGS)
	
image.o: source/image.cpp header/image.h
	$(CC) -c source/image.cpp $(CFLAGS)
	
player.o: source/player.cpp header/player.h
	$(CC) -c source/player.cpp $(CFLAGS)
	
clean:
	rm -rf *.o SchShooter
