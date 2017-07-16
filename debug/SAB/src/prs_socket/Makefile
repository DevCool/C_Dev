CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fPIC
LDFLAGS=-shared -Wl,-soname,libprs_socket.so.1
LIBS=

TARGET=libprs_socket.so
BINDIR=../../bin
OBJDIR=../../obj
SRCDIR=../../src
INSTALLDIR?=/opt
SOURCES = $(wildcard $(SRCDIR)/prs_socket/*.c)
INCLUDES = $(wildcard $(SRCDIR)/prs_socket/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/prs_socket/%.c=$(OBJDIR)/%.c.o)

all: start linux end

start:
	@echo Building PRS Socket Library...

end:
	@echo Done building.

linux: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET).1.0: $(OBJECTS)
	@echo Linking library...
	@$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo Done linking library!

$(OBJECTS) : $(OBJDIR)/%.c.o: $(SOURCES)
	@echo Compiling $< now..
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo Compiled $< successfully!

.PHONY: clean install uninstall
clean:
	@echo Cleaning up...
	@rm -f *~ $(OBJDIR)/*.c.o $(BINDIR)/$(TARGET).1.0
	@echo Done cleaning!

install:
	@echo Installing...
	@cp $(BINDIR)/$(TARGET) $(INSTALLDIR)/lib
	@ln -sf $(INSTALLDIR)/lib/$(TARGET).1.0 $(INSTALLDIR)/lib/$(TARGET).1
	@ln -sf $(INSTALLDIR)/lib/$(TARGET).1.0 $(INSTALLDIR)/lib/$(TARGET)
	@echo Install finished!

uninstall:
	@echo Uninstalling...
	@rm -f $(INSTALLDIR)/lib/$(TARGET).1.0
	@rm -f $(INSTALLDIR)/lib/$(TARGET).1
	@rm -f $(INSTALLDIR)/lib/$(TARGET)
	@echo Uninstall finished.
