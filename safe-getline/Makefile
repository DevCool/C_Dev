CC=gcc
CFLAGS?=-std=c89 -Wall -Wextra -pedantic
LDFLAGS?=
SRC=main.c helper.c 
OBJ=$(SRC:.c=.c.o)

all: progname

%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

progname: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean

clean:
	@echo "Cleaning project..."
	rm -f *.o progname
	@echo "Done cleaning."
