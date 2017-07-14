CC=gcc
CFLAGS?=-std=c99 -Wall -Wextra -pedantic
LDFLAGS?=-std=c99 -Wall -Wextra -pedantic
LIBS=

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.c.o)
LIST=$(SOURCES:%.c=%)

all: bmsg $(LIST)

bmsg:
	@echo Started compiling...

%: %.c.o
	@$(CC) $(LDFLAGS) $< -o $@
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
	@del *.o $(LIST)
	@echo Cleaning done.
