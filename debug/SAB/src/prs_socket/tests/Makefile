CC=gcc
CFLAGS?=-std=c99 -Wall -Wextra -pedantic
LDFLAGS?=-std=c99 -Wall -Wextra -pedantic
LIBS=

SRCDIR=.
OBJDIR=.
BINDIR=.
SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)
LIST=$(SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%)

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
