CC=gcc
CFLAGS=-std=c89 -Wall -g
LIB1=
LIB2=-lws2_32

PROGRAMS:=ex1-1.exe test.exe servreq.exe

all: clean $(PROGRAMS)

ex1-1.exe: ex1-1.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB1)

test.exe: test.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB2)

servreq.exe: servreq.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB2)

clean:
	echo Cleaning project...
	del *.obj $(PROGRAMS)
	echo Done cleaning.
