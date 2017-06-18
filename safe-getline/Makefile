CC=gcc
CFLAGS?=-std=c89 -Wall -Wextra -pedantic
LDFLAGS?=
SRC=main.c helper.c dlist.c
OBJ=$(SRC:.c=.c.o)

all: getline

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

getline: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean

clean:
	@echo "Cleaning project..."
	rm -f *.o getline
	@echo "Done cleaning."
