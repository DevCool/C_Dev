BINDIR=../../bin
OBJDIR=.
SRCDIR=../../src

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
LDFLAGS=-L$(BINDIR)
LIBS=-lprs_socket

TARGET=SAB_transfer
INSTALLDIR?=/usr/local/bin
OBJECTS=$(OBJDIR)/sab_transfer.c.o $(OBJDIR)/transfer.c.o $(OBJDIR)/socket.c.o

.PHONY: all linux
all: linux

linux: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OBJDIR)/sab_transfer.c.o: main.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/transfer.c.o: $(SRCDIR)/transfer.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/socket.c.o: $(SRCDIR)/prs_socket/socket.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean remove install uninstall
clean:
	rm -f *~ $(OBJECTS) $(BINDIR)/$(TARGET)

remove: clean
	rm -f $(BINDIR)/$(TARGET)

install:
	@echo Installing.
	@cp $(BINDIR)/$(TARGET) $(INSTALLDIR)/$(TARGET)
	@echo done.

uninstall:
	@echo Uninstalling.
	@rm $(INSTALLDIR)/$(TARGET)
	@echo Done.
