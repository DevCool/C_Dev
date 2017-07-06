CC=gcc
CFLAGS=-std=c89 -Wall -Wextra -pedantic -g
LDFLAGS=-I./

SRC=texed.c dlist.c cmd.c
OBJ=$(SRC:.c=.c.o)
EXE=texed

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@echo "Cleaning project..."
	rm -f *.o *~ $(EXE)
	@echo "Done."
