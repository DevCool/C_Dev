CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fPIC
LDFLAGS=-shared -Wl,-soname,libprs_socket.so.1 -lc

TARGET=libprs_socket.so.1.0
BINDIR=../../bin
OBJDIR=../../obj
SRCDIR=../../src
INSTALLDIR?=/opt
SOURCES = $(wildcard $(SRCDIR)/prs_socket/*.c)
INCLUDES = $(wildcard $(SRCDIR)/prs_socket/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/prs_socket/%.c=$(OBJDIR)/%.c.o)

all: linux

linux: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJECTS) : $(OBJDIR)/%.c.o: $(SOURCES)
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean install uninstall
clean:
	@echo Cleaning up...
	@rm -f *~ $(OBJDIR)/*.c.o $(BINDIR)/$(TARGET)
	@echo Done cleaning!

install:
	@echo Installing...
	@mkdir -p $(INSTALLDIR)/include/prs_socket
	@cp $(SRCDIR)/prs_socket/socket.h $(INSTALLDIR)/include/socket.h
	@cp $(BINDIR)/$(TARGET) $(INSTALLDIR)/lib
	@ln -sf $(INSTALLDIR)/lib/$(TARGET) $(INSTALLDIR)/lib/$(TARGET).1
	@ln -sf $(INSTALLDIR)/lib/$(TARGET) $(INSTALLDIR)/lib/libprs_socket.so
	@chmod 0755 $(INSTALLDIR)/lib/$(TARGET)
	@chmod 0755 $(INSTALLDIR)/lib/libprs_socket.so.1
	@chmod 0755 $(INSTALLDIR)/lib/libprs_socket.so
	@echo Install finished!

uninstall:
	@echo Uninstalling...
	@rm -rf $(INSTALLDIR)/include/prs_socket
	@rm -f $(INSTALLDIR)/lib/libprs_socket.so.1
	@rm -f $(INSTALLDIR)/lib/libprs_socket.so
	@rm -f $(INSTALLDIR)/lib/$(TARGET)
	@echo Uninstall finished.
