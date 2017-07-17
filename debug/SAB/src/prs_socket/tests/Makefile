SRCDIR=../../../src/prs_socket/tests
OBJDIR=../../../src/prs_socket/tests
BINDIR=../../../bin

CC=gcc
CFLAGS?=-std=c99 -Wall -Wextra -pedantic
LDFLAGS?=-L$(BINDIR) -lprs_socket

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)
LIST=$(SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%)

.PHONY: all
all: $(LIST)

$(BINDIR)/%: $(OBJECTS)
	$(CC) $(LDFLAGS) $< -o $@

$(OBJECTS): %.c.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean remove clean-windows
clean:
	rm -f *~ $(OBJDIR)/*.o

remove: clean
	rm -f $(LIST)

clean-windows:
	@echo Cleaning up!
	@del *.o $(LIST)
	@echo Cleaning done.
