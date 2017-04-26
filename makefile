OBJECTS= main.o draw.o display.o matrix.o parser.o stack.o
CFLAGS= -Wall
LDFLAGS= -lm
CC= gcc

all: $(OBJECTS)
	$(CC) -o main $(OBJECTS) $(LDFLAGS)

main.o: main.c display.h draw.h ml6.h matrix.h parser.h stack.h
	$(CC) -c main.c

draw.o: draw.c draw.h display.h ml6.h matrix.h
	$(CC) $(CFLAGS) -c draw.c

display.o: display.c display.h ml6.h matrix.h
	$(CC) $(CFLAGS) -c display.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

parser.o: parser.c parser.h matrix.h draw.h display.h ml6.h stack.h
	$(CC) $(CFLAGS) -c parser.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

run: all
	./main script

clean:
	rm *.o *~