OBJDIR=./obj
BINDIR=./bin
SRCDIR=.

CC=cc
CFLAGS=-std=c89 -Wall -Wno-unused-function
LDFLAGS=

C_SRC=$(wildcard $(SRCDIR)/*.c)
C_OBJ=$(C_SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)
TARGETS=$(C_SRC:$(SRCDIR)/%.c=$(BINDIR)/%)
LT_OBJ=$(OBJDIR)/linetest.c.o $(OBJDIR)/helper.c.o

.PHONY: all makeobj testing testing-clean clean
all: makeobj $(TARGETS)

makeobj:
	@test -x obj && test -x bin || { mkdir -p obj bin; }

$(BINDIR)/%: $(OBJDIR)/%.c.o
	@$(CC) $(CFLAGS) -o $@ $<
	@echo Linked $@ successfully!

$(C_OBJ): $(OBJDIR)/%.c.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo Compiled $< successfully!

clean:
	@test -d $(OBJDIR) || test -d $(OBJDIR) && echo cleaning && rm -f $(OBJDIR)/*.o $(TARGETS) && rmdir $(OBJDIR) $(BINDIR) && echo done || echo No objects or executables. Not cleaning!

