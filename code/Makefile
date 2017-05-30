CC=gcc
CFLAGS=-std=c89 -Wall -g
LIB1=
LIB2=-lws2_32

PROGRAMS:=ex1-1.exe test.exe someprog.exe getln_dyn.exe servreq.exe

all: clean $(PROGRAMS)

ex1-1.exe: ex1-1.c.obj
	$(CC) $(CFLAGS) $^ -o $@ $(LIB1)

test.exe: test.c.obj
	$(CC) $(CFLAGS) $^ -o $@ $(LIB2)

servreq.exe: servreq.c.obj
	$(CC) $(CFLAGS) $^ -o $@ $(LIB2)

someprog.exe: someprog.c.obj
	$(CC) $(CFLAGS) $^ -o $@ $(LIB1)

getln_dyn.exe: getln_dyn.c.obj
	$(CC) $(CFLAGS) $^ -o $@ $(LIB1)

%.c.obj: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	echo Cleaning project...
	del *.obj $(PROGRAMS)
	echo Done cleaning.
