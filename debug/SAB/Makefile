CC=gcc
CFLAGS?=-std=c99 -Wall -Wextra -pedantic -g
LDFLAGS?=-std=c99 -Wall -Wextra -pedantic -g
LIBS=-lespeak
LIBSWIN=-lkernel32 -lws2_32

SRCDIR=src
BINDIR=bin
OBJDIR=obj

SRC=$(wildcard $(SRCDIR)/*.c)
TARGET=SAB

OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)

all: linux

linux: $(BINDIR)/$(TARGET)

windows: $(BINDIR)/$(TARGET).exe

$(OBJ) : $(OBJDIR)/%.c.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo Compiled $< successfully!

$(BINDIR)/$(TARGET): $(OBJ)
	@echo Linking executable...
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	@echo Executable built with success!

$(BINDIR)/$(TARGET).exe: $(OBJ)
	@echo Linking executable...
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBSWIN)
	@echo Executable built with success!

.PHONY: clean clean-windows

clean:
	@echo Cleaning project...
	@rm -f *~ $(SRCDIR)/*~ $(OBJDIR)/*.o $(BINDIR)/$(TARGET)
	@echo Cleanup complete!

clean-windows:
	@echo Cleaning project...
	@del $(OBJDIR)\*.o $(BINDIR)\$(TARGET).exe
	@echo Cleanup complete...
