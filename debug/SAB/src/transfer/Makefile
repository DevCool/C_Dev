CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic
LDFLAGS=
LIBS=


TARGET=SAB_transfer
BINDIR=../../bin
OBJDIR=../../obj
SRCDIR=../../src
INSTALLDIR?=/usr/local/bin
OBJECTS=$(OBJDIR)/sab_transfer.c.o $(OBJDIR)/transfer.c.o $(OBJDIR)/socket.c.o

all: linux

linux: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OBJDIR)/sab_transfer.c.o: main.c
	@$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/transfer.c.o: $(SRCDIR)/transfer.c
	@$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/socket.c.o: $(SRCDIR)/socket.c
	@$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean install uninstall
clean:
	@echo Cleaning up...
	@rm -f *~ $(OBJECTS) $(BINDIR)/$(TARGET)
	@echo Done.

install:
	@echo Installing.
	@cp $(BINDIR)/$(TARGET) $(INSTALLDIR)/$(TARGET)
	@echo done.

uninstall:
	@echo Uninstalling.
	@rm $(INSTALLDIR)/$(TARGET)
	@echo Done.
