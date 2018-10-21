CC=g++
GLFLAGS=-lGL -lglut -lGLU
CFLAGS=-lm

all: SchShooter

SchShooter: main.o image.o
	$(CC) main.o image.o -o SchShooter $(GLFLAGS) $(CFLAGS)
	
main.o: source/main.c
	$(CC) -c source/main.c $(GLFLAGS) $(CFLAGS)
	
image.o: source/image.c
	$(CC) -c source/image.c $(CFLAGS)
	
clean:
	rm -rf *.o bilijar
