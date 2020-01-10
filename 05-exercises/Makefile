CC := gcc
CFLAGS := -g -Wall -I ../include
DEP := ../include/error_functions.o

default: append atomic_append

append: append.o $(DEP)
	$(CC) -o append append.o $(DEP)

atomic_append: atomic_append.o $(DEP)
	$(CC) -o atomic_append atomic_append.o $(DEP)

dup: dup.o $(DEP)
	$(CC) -o dup dup.o $(DEP)

vectorio: vectorio.o $(DEP)
	$(CC) -o vectorio vectorio.o $(DEP)

format:
	clang-format -i -style=WebKit *.c

clean:
	rm -rf append atomic_append dup vectorio *.o
