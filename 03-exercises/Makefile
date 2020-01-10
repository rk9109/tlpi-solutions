CC := gcc
CFLAGS := -g -Wall -I ../include
DEP := ../include/error_functions.o

default: reboot

reboot: reboot.o $(DEP)
	$(CC) -o reboot reboot.o $(DEP)

format:
	clang-format -i -style=WebKit *.c

clean:
	rm -rf reboot *.o
