CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fPIC
LDFLAGS=-shared -Wl,-soname,libprs_socket.so.1 -lc

PROJECTDIR := $(shell pwd)
TARGET=libprs_socket.so.1.0
BINDIR=../../bin
OBJDIR=../../src/prs_socket
SRCDIR=../../src/prs_socket
INSTALLDIR?=/usr
SOURCES = $(wildcard $(SRCDIR)/*.c)
INCLUDES = $(wildcard $(SRCDIR)/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)

.PHONY: all linux
all: linux

linux: $(BINDIR)/$(TARGET)

$(OBJECTS) : $(OBJDIR)/%.c.o: $(SOURCES)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	ln -sf $(PROJECTDIR)/$(BINDIR)/$(TARGET) $(PROJECTDIR)/$(BINDIR)/libprs_socket.so.1
	ln -sf $(PROJECTDIR)/$(BINDIR)/$(TARGET) $(PROJECTDIR)/$(BINDIR)/libprs_socket.so
	chmod 0755 $(BINDIR)/$(TARGET)
	chmod 0755 $(BINDIR)/libprs_socket.so.1
	chmod 0755 $(BINDIR)/libprs_socket.so

.PHONY: clean install uninstall
clean:
	rm -f *~ $(OBJDIR)/*.c.o $(BINDIR)/$(TARGET)

remove: clean
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(BINDIR)/libprs_socket.so.1
	rm -f $(BINDIR)/libprs_socket.so
	rm -f $(BINDIR)/$(TARGET)

install:
	@echo Installing...
	mkdir -p $(INSTALLDIR)/include/prs_socket
	cp $(SRCDIR)/socket.h $(INSTALLDIR)/include/socket.h
	cp $(BINDIR)/$(TARGET) $(INSTALLDIR)/lib
	ln -sf $(INSTALLDIR)/lib/$(TARGET) $(INSTALLDIR)/lib/libprs_socket.so.1
	ln -sf $(INSTALLDIR)/lib/$(TARGET) $(INSTALLDIR)/lib/libprs_socket.so
	chmod 0755 $(INSTALLDIR)/lib/$(TARGET)
	chmod 0755 $(INSTALLDIR)/lib/libprs_socket.so.1
	chmod 0755 $(INSTALLDIR)/lib/libprs_socket.so
	@ldconfig
	@echo Install finished!

uninstall:
	@echo Uninstalling...
	rm -rf $(INSTALLDIR)/include/prs_socket
	rm -f $(INSTALLDIR)/lib/libprs_socket.so.1
	rm -f $(INSTALLDIR)/lib/libprs_socket.so
	rm -f $(INSTALLDIR)/lib/$(TARGET)
	@ldconfig
	@echo Uninstall finished.
