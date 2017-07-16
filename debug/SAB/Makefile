# S.A.B. project main makefile

PROG = sab
TARGETS = SAB SAB_transfer libprs_socket libprs_tests

$(PROG): $(TARGETS)

all: $(TARGETS)

SAB:
	@echo Building S.A.B.
	@cd ./src ; make
	@echo Done.

SAB_transfer:
	@echo Building S.A.B. Transfer
	@cd ./src/transfer ; make
	@echo Done.

libprs_socket:
	@echo Building PRS Socket Library...
	@cd ./src/prs_socket ; make
	@echo Done.

libprs_tests:
	@echo Building PRS Socket Library Tests...
	@cd ./src/prs_socket/tests ; make
	@echo Done.

.PHONY: clean install uninstall
clean:
	@echo Cleaning project...
	@rm -f *~
	@cd ./src/prs_socket/tests ; make clean
	@cd ./src/prs_socket ; make clean
	@cd ./src/transfer ; make clean
	@cd ./src ; make clean
	@echo Done cleaning!

install:
	@echo Installing... S.A.B.
	@cd ./src/prs_socket ; make install
	@cd ./src/transfer ; make install
	@cd ./src ; make install
	@echo Done installing!

uninstall:
	@echo Uninstalling... S.A.B.
	@cd ./src/prs_socket ; make uninstall
	@cd ./src/transfer ; make uninstall
	@cd ./src ; make uninstall
	@echo Done uninstalling!
