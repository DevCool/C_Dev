# S.A.B. project main makefile

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic
LDFLAGS=

MAKEFILE = Makefile
TARGETS = build

.PHONY: all build clean remove install uninstall
all: $(TARGETS)

build:
	@echo Building S.A.B.
	cd src && make
	cd src/transfer && make
	cd src/prs_socket && make
	cd src/prs_socket/tests && make

clean:
	@echo Cleaning project...
	rm -f *~
	cd src/prs_socket && make clean
	cd src/prs_socket/tests && make clean
	cd src/transfer && make clean
	cd src && make clean
	@echo Done cleaning!

remove:
	@echo Cleaning project...
	rm -f *~
	cd src/prs_socket && make remove
	cd src/prs_socket/tests && make remove
	cd src/transfer && make remove
	cd src && make remove
	@echo Done cleaning!

install:
	@echo Installing... S.A.B.
	cd src/prs_socket && make install
	cd src/transfer && make install
	cd src && make install
	@echo Done installing!

uninstall:
	@echo Uninstalling... S.A.B.
	cd src/prs_socket && make uninstall
	cd src/transfer && make uninstall
	cd src && make uninstall
	@echo Done uninstalling!
