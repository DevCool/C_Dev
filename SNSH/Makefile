# S.A.B. project main makefile

CC=gcc
CFLAGS=-std=gnu99 -Wall -pedantic
LDFLAGS=
TARGETS = libprs_socket libprs_tests SNSH SNSH_client SNSH_transfer

.PHONY: all linux windows SNSH SNSH_client SNSH_transfer libprs_socket libprs_tests clean remove install uninstall
all: linux

linux: libprs_socket SNSH SNSH_client SNSH_transfer libprs_tests
windows: libprs_socketw SNSHw SNSHw_client SNSH_transferw libprs_testsw

# Linux Building
SNSH:
	@echo Building S.N.S.H.
	cd src && make
	@echo Done with S.N.S.H.

SNSH_client:
	@echo Building S.N.S.H client...
	cd src/client && make
	@echo Done with S.N.S.H. client!

SNSH_transfer:
	@echo Building S.N.S.H. transfer...
	cd src/transfer && make
	@echo Done with S.N.S.H. transfer!

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
	cd src/client && make clean
	cd src && make clean
	@echo Done cleaning!

remove:
	@echo Cleaning project...
	rm -f *~
	cd src/prs_socket && make remove
	cd src/prs_socket/tests && make remove
	cd src/transfer && make remove
	cd src/client && make remove
	cd src && make remove
	@echo Done cleaning!

install:
	@echo Installing... S.A.B.
	cd src/prs_socket && make install
	cd src/transfer && make install
	cd src/client && make install
	cd src && make install
	@echo Done installing!

uninstall:
	@echo Uninstalling... S.A.B.
	cd src/prs_socket && make uninstall
	cd src/transfer && make uninstall
	cd src/client && make uninstall
	cd src && make uninstall
	@echo Done uninstalling!

# Windows Building

SNSHw:
	@echo Building S.A.B.
	cd src && mingw32-make -f Makefile.win
	@echo Done with S.A.B.

SNSHw_client:
	@echo Building S.N.S.H. client...
	cd src/client && mingw32-make -f Makefile.win
	@echo Done with S.N.S.H. client!

SNSH_transferw:
	@echo Building S.A.B. transfer...
	cd src/transfer && mingw32-make -f Makefile.win
	@echo Done with S.A.B. transfer!

libprs_socketw:
	@echo Building PRS Sockets...
	cd src/prs_socket && mingw32-make -f Makefile.win
	@echo Done with PRS Sockets!

libprs_testsw:
	@echo Building PRS Sockets Tests...
	cd src/prs_socket/tests && mingw32-make -f Makefile.win
	@echo Building PRS Sockets Tests!

clean-windows:
	@echo Cleaning project...
	cd src/prs_socket && mingw32-make -f Makefile.win clean
	cd src/prs_socket/tests && mingw32-make -f Makefile.win clean
	cd src/transfer && mingw32-make -f Makefile.win clean
	cd src/client && mingw32-make -f Makefile.win clean
	cd src && mingw32-make -f Makefile.win clean
	@echo Done cleaning!

remove-windows:
	@echo Cleaning project...
	cd src/prs_socket && mingw32-make -f Makefile.win remove
	cd src/prs_socket/tests && mingw32-make -f Makefile.win remove
	cd src/transfer && mingw32-make -f Makefile.win remove
	cd src/client && mingw32-make -f Makefile.win remove
	cd src && mingw32-make -f Makefile.win remove
	@echo Done cleaning!
