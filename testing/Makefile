CC=gcc
CFLAGS=-std=c89 -Wall -g

SRCS=b1.c b2.c b3.c max2.c max3.c maxdo.c odds.c pow.c texed.c token.c
OBJS=$(SRCS:.c=.o)
PROGRAMS:=b1 b2 b3 max2 max3 maxdo odds pow texed token

all: $(PROGRAMS)

%.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
b1: b1.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

b2: b2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

b3: b3.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

max2: max2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

max3: max3.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

maxdo: maxdo.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

odds: odds.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

pow: pow.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

texed: texed.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

token: token.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	@echo Cleaning programs...
	rm -f *~ $(OBJS) $(PROGRAMS)
	@echo Done.

