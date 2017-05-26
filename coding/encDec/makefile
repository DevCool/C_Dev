CFLAGS=-O2 -Wall -std=gnu89 -fno-builtin-strlen
LIBS=
OBJECTS=encdec.o helper.o
TARGET=encdec

all: $(OBJECTS) $(TARGET)

rebuild: clean all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

helper.o: helper.c
encdec.o: encdec.c

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	@echo "Cleaning project..."
	@rm -f *.o $(TARGET)
	@echo "Done."

