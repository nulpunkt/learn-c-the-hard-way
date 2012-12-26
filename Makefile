CFLAGS=-Wall -g

all:
	cc ${CFLAGS} ex1.c -o ex1
clean:
	rm -f ex1
