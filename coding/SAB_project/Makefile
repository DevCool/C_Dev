CC=gcc
CFLAGS?=-std=c89 -Wall -g
LDFLAGS?=-g
LIB1?=-lws2_32 -lkernel32 -luser32
LIB2?=

SRCS=bdclient.c sab.c
OBJS=$(SRCS:.c=.c.o)
PROGS=bdclient sab

all: $(PROGS)

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

bdclient: bdclient.c.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIB2)

sab: sab.c.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIB2)

.PHONY: clean

clean:
	@echo Cleaning project...
	rm -f *.o *~ $(PROGS)
	@echo Done.

win32: bdclient.exe sab.exe

bdclient.exe: bdclient.c.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIB1)

sab.exe: sab.c.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIB1)

.PHONY: win32-clean

win32-clean:
	@echo Cleaning project...
	del *.o bdclient.exe sab.exe
	@echo Done.

