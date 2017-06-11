CC=gcc
CFLAGS=-std=c89 -Wall -g -c
LDFLAGS=-std=c89 -Wall -g -DWINVER=0x0501
LIBS:=-lws2_32 -luser32 -lkernel32 -lgdi32

SRC=main.c gui.c
OBJ=$(SRC:%.c=%.obj)
APP:=SAB_Client.exe

all: clean $(APP)

$(APP): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

%.obj: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	@echo Cleaning project...
	del *.obj $(APP)
	@echo Done cleaning.
