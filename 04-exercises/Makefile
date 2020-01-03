CC := gcc
CFLAGS := -g -Wall -I ../include
DEP := ../include/error_functions.o

default: tee hole cp

tee: tee.o $(DEP)
	$(CC) -o tee tee.o $(DEP)

hole: hole.o $(DEP)
	$(CC) -o hole hole.o $(DEP)

cp: cp.o $(DEP)
	$(CC) -o cp cp.o $(DEP)

clean:
	rm -rf tee hole cp *.o
