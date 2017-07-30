CC=gcc
CFLAGS?=-std=gnu99 -Wall -Wextra -pedantic
LDFLAGS?=-std=gnu99 -Wall -Wextra -pedantic
LIBS=

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.c.o)
LIST=$(SOURCES:%.c=%)
LISTW=$(SOURCES:%.c=%.exe)

.PHONY: all bmsg
all: bmsg linux

bmsg:
	@echo Started compiling...

linux: $(LIST)
windows: $(LISTW)

%: %.c.o
	@$(CC) $(LDFLAGS) $< -o $@
	@echo Linked $@ successfully!

%.exe: %.c.o
	@$(CC) $(LDFLAGS) $< -o $@
	@echo Linked $@ successfully!

fdtest.exe: fdtest.c.o
	@$(CC) $(LDFLAGS) $^ -o $@ -lws2_32
	@echo Linked $@ successfully!

sendrecv.exe: sendrecv.c.o
	@$(CC) $(LDFLAGS) $^ -o $@ -lws2_32
	@echo Linked $@ successfully!

$(OBJECTS): %.c.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo Compiled $< successfully!

.PHONY: clean clean-windows
clean:
	@echo Cleaning up!
	@rm -f *~ *.o $(LIST)
	@echo Cleaning done.

clean-windows:
	@echo Cleaning up!
	@del *.o $(LISTW)
	@echo Cleaning done.
