CC=g++
GLFLAGS=-lGL -lglut -lGLU
CFLAGS=-lm

all: SchShooter

SchShooter: main.o image.o
	$(CC) main.o image.o -o SchShooter $(GLFLAGS) $(CFLAGS)
	
main.o: source/main.cpp
	$(CC) -c source/main.cpp $(GLFLAGS) $(CFLAGS)
	
image.o: source/image.cpp
	$(CC) -c source/image.cpp $(CFLAGS)
	
clean:
	rm -rf *.o SchShooter
