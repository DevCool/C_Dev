CC=gcc
CFLAGS?=-std=c99 -Wall -Wextra -pedantic -g
LDFLAGS?=-std=c99 -Wall -Wextra -pedantic -g
LIBS=-lespeak
LIBSWIN=-lkernel32 -lws2_32

SRCDIR=src
BINDIR=bin
OBJDIR=obj
INSTALLDIR?=/usr/local/bin

SRC=$(wildcard $(SRCDIR)/*.c)
TARGET=SAB

OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)

all: linux

linux: $(BINDIR)/$(TARGET)

windows: $(BINDIR)/$(TARGET).exe

$(OBJ) : $(OBJDIR)/%.c.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo Compiled $< successfully!

$(BINDIR)/$(TARGET): $(OBJ)
	@echo Linking executable...
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	@echo Executable built with success!

$(BINDIR)/$(TARGET).exe: $(OBJ)
	@echo Linking executable...
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBSWIN)
	@echo Executable built with success!

.PHONY: clean clean-windows install uninstall depend

clean:
	@echo Cleaning project...
	@rm -f *~ $(SRCDIR)/*~ $(OBJDIR)/*.o $(BINDIR)/$(TARGET)
	@echo Cleanup complete!

clean-windows:
	@echo Cleaning project...
	@del $(OBJDIR)\*.o $(BINDIR)\$(TARGET).exe
	@echo Cleanup complete...

install:
	@echo Installing...
	@cp $(BINDIR)/$(TARGET) $(INSTALLDIR)/$(TARGET)
	@echo Done with install!

uninstall:
	@echo Uninstalling...
	@rm $(INSTALLDIR)/$(TARGET)
	@echo Uninstalled!

depend:
	makedepend -- $(CFLAGS) -- $(SRC)

# DO NOT DELETE

src/socket.o: /usr/include/stdio.h /usr/include/features.h
src/socket.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
src/socket.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/socket.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/socket.o: /usr/include/libio.h /usr/include/_G_config.h
src/socket.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
src/socket.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
src/socket.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
src/socket.o: /usr/include/endian.h /usr/include/bits/endian.h
src/socket.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
src/socket.o: /usr/include/sys/types.h /usr/include/time.h
src/socket.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/socket.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
src/socket.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
src/socket.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
src/socket.o: /usr/include/string.h /usr/include/xlocale.h
src/socket.o: /usr/include/errno.h /usr/include/bits/errno.h
src/socket.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
src/socket.o: /usr/include/asm-generic/errno.h
src/socket.o: /usr/include/asm-generic/errno-base.h src/socket.h ../debug.h
src/helper.o: /usr/include/stdio.h /usr/include/features.h
src/helper.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
src/helper.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/helper.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/helper.o: /usr/include/libio.h /usr/include/_G_config.h
src/helper.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
src/helper.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
src/helper.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
src/helper.o: /usr/include/endian.h /usr/include/bits/endian.h
src/helper.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
src/helper.o: /usr/include/sys/types.h /usr/include/time.h
src/helper.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/helper.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
src/helper.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
src/helper.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
src/helper.o: /usr/include/string.h /usr/include/xlocale.h
src/helper.o: /usr/include/errno.h /usr/include/bits/errno.h
src/helper.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
src/helper.o: /usr/include/asm-generic/errno.h
src/helper.o: /usr/include/asm-generic/errno-base.h /usr/include/dirent.h
src/helper.o: /usr/include/bits/dirent.h /usr/include/bits/posix1_lim.h
src/helper.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
src/helper.o: src/socket.h ../debug.h src/helper.h
src/espeech.o: /usr/include/stdio.h /usr/include/features.h
src/espeech.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
src/espeech.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/espeech.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/espeech.o: /usr/include/libio.h /usr/include/_G_config.h
src/espeech.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
src/espeech.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
src/espeech.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
src/espeech.o: /usr/include/endian.h /usr/include/bits/endian.h
src/espeech.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
src/espeech.o: /usr/include/sys/types.h /usr/include/time.h
src/espeech.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/espeech.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
src/espeech.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
src/espeech.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
src/espeech.o: /usr/include/string.h /usr/include/xlocale.h
src/espeech.o: /usr/include/errno.h /usr/include/bits/errno.h
src/espeech.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
src/espeech.o: /usr/include/asm-generic/errno.h
src/espeech.o: /usr/include/asm-generic/errno-base.h
src/espeech.o: /usr/include/espeak/speak_lib.h src/espeech.h ../debug.h
src/main.o: /usr/include/stdio.h /usr/include/features.h
src/main.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
src/main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
src/main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/main.o: /usr/include/libio.h /usr/include/_G_config.h
src/main.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
src/main.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
src/main.o: /usr/include/xlocale.h src/socket.h src/helper.h
