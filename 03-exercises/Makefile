CC := gcc
CFLAGS := -g -Wall -I ../include
DEP := ../include/error_functions.o

default: reboot

reboot: reboot.o $(DEP)
	$(CC) -o reboot reboot.o $(DEP)

clean:
	rm -rf reboot *.o
