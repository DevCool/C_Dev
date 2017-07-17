# S.A.B. project main makefile

MAKEFILE = Makefile
TARGETS = build

all: $(TARGETS)

.PHONY: build clean install uninstall
build:
	@echo Building S.A.B.
	(cd src ; make all)
	@echo Building S.A.B. Transfer
	(cd src/transfer ; make all)
	@echo Building PRS Socket Library...
	(cd src/prs_socket ; make all)
	@echo Building PRS Socket Library Tests...
	(cd src/prs_socket/tests ; make all)
	@echo Done.

clean:
	@echo Cleaning project...
	@rm -f *~
	@cd src/prs_socket/tests ; make clean
	@cd src/prs_socket ; make clean
	@cd src/transfer ; make clean
	@cd src ; make clean
	@echo Done cleaning!

install:
	@echo Installing... S.A.B.
	@cd src/prs_socket ; make install
	@cd src/transfer ; make install
	@cd src ; make install
	@echo Done installing!

uninstall:
	@echo Uninstalling... S.A.B.
	@cd src/prs_socket ; make uninstall
	@cd src/transfer ; make uninstall
	@cd src ; make uninstall
	@echo Done uninstalling!
