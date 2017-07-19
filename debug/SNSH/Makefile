# S.A.B. project main makefile

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic
LDFLAGS=
TARGETS = libprs_socket libprs_tests SAB SAB_transfer

.PHONY: all linux SAB SAB_transfer libprs_socket libprs_tests clean remove install uninstall
all: linux

linux: libprs_socket SNSH SNSH_transfer libprs_tests

SNSH:
	@echo Building S.A.B.
	cd src && make
	@echo Done with S.A.B.

SNSH_transfer:
	@echo Building S.A.B. transfer...
	cd src/transfer && make
	@echo Done with S.A.B. transfer!

libprs_socket:
	@echo Building PRS Sockets...
	cd src/prs_socket && make
	@echo Done with PRS Sockets!

libprs_tests:
	@echo Building PRS Sockets Tests...
	cd src/prs_socket/tests && make
	@echo Building PRS Sockets Tests!

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
