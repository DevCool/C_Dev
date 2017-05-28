CC=gcc
CFLAGS=-std=c89 -Wall -g
LDFLAGS=-std=c89 -g
LIB1=-lws2_32 -lkernel32 -ladvapi32
LIB2=-lws2_32

SRCS:=server.c helper.c logon.c client.c
OBJS:=$(SRCS:.c=.c.obj)

APP1:=server.exe
APP2:=client.exe


all: $(APP1) $(APP2)

$(APP1): server.c.obj logon.c.obj helper.c.obj
	$(CC) $(LDFLAGS) $^ -o $@ $(LIB1)

$(APP2): client.c.obj helper.c.obj
	$(CC) $(LDFLAGS) $^ -o $@ $(LIB2)

%.c.obj: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	echo Cleaning projects...
	del *.c.obj $(APP1) $(APP2)
	echo Done cleaning.
