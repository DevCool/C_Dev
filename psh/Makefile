CC=gcc
CFLAGS=-std=c89 -ansi -Wall -Wextra -pedantic -g
INCLUDES=-I./src
LDFLAGS=-ansi -g
LIBS=-lkernel32 -ladvapi32

BINDIR=bin
SRCDIR=src
OBJDIR=obj
INSTDIR?=/usr/local/bin/
INSTDIRWIN?=/Windows/System32/
TARGET=psh
TARGETWIN=psh.exe

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDE := $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)

all: linux

makedirs:
	@mkdir $(BINDIR) $(OBJDIR)
	@echo Starting to compile!

linux: makedirs $(BINDIR)/$(TARGET)

windows: makedirs $(BINDIR)/$(TARGETWIN)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo Linking complete!

$(BINDIR)/$(TARGETWIN): $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	@echo Linking complete!

$(OBJECTS): $(OBJDIR)/%.c.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo Compiled $< successfully!

.PHONY: remove
remove:
	@rm -f $(OBJDIR)/*.o
	@rmdir $(OBJDIR)
	@echo Cleanup complete!

.PHONY: remove-windows
remove-windows:
	@del $(OBJDIR)\*.o
	@rmdir $(OBJDIR)
	@echo Cleanup Complete!

.PHONY: clean
clean: remove
	@rm -f $(BINDIR)/$(TARGET)
	@rmdir $(BINDIR)
	@echo Executable removed!

.PHONY: clean-windows
clean-windows: remove-windows
	@del $(BINDIR)\$(TARGETWIN)
	@rmdir $(BINDIR)
	@echo Executable removed!
