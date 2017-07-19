SRCDIR=../src
BINDIR=../bin
OBJDIR=.

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
LDFLAGS=-L$(BINDIR)
LIBS=-lespeak -lprs_socket
LIBSWIN=-lkernel32 -lws2_32
INSTALLDIR?=/usr/local/bin

SRC=$(wildcard $(SRCDIR)/*.c)
TARGET=SAB

OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.c.o)

all: linux

linux: $(BINDIR)/$(TARGET)

$(OBJ) : $(OBJDIR)/%.c.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean remove install uninstall depend

clean:
	rm -f *~ $(SRCDIR)/*~ $(OBJDIR)/*.o

remove: clean
	rm -f $(BINDIR)/$(TARGET)

install:
	echo Installing...
	cp $(BINDIR)/$(TARGET) $(INSTALLDIR)/$(TARGET)
	echo Done with install!

uninstall:
	echo Uninstalling...
	rm $(INSTALLDIR)/$(TARGET)
	echo Uninstalled!

depend:
	makedepend -- $(CFLAGS) -- $(SRC)

# DO NOT DELETE

../src/socket.o: /usr/include/stdio.h /usr/include/features.h
../src/socket.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
../src/socket.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/socket.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/socket.o: /usr/include/libio.h /usr/include/_G_config.h
../src/socket.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/socket.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
../src/socket.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
../src/socket.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/socket.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
../src/socket.o: /usr/include/sys/types.h /usr/include/time.h
../src/socket.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/socket.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/socket.o: /usr/include/sys/sysmacros.h
../src/socket.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/socket.o: /usr/include/bits/stdlib-float.h /usr/include/string.h
../src/socket.o: /usr/include/xlocale.h /usr/include/errno.h
../src/socket.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
../src/socket.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
../src/socket.o: /usr/include/asm-generic/errno-base.h socket.h ../../debug.h
../src/helper.o: /usr/include/stdio.h /usr/include/features.h
../src/helper.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
../src/helper.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/helper.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/helper.o: /usr/include/libio.h /usr/include/_G_config.h
../src/helper.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/helper.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
../src/helper.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
../src/helper.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/helper.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
../src/helper.o: /usr/include/sys/types.h /usr/include/time.h
../src/helper.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/helper.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/helper.o: /usr/include/sys/sysmacros.h
../src/helper.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/helper.o: /usr/include/bits/stdlib-float.h /usr/include/string.h
../src/helper.o: /usr/include/xlocale.h /usr/include/errno.h
../src/helper.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
../src/helper.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
../src/helper.o: /usr/include/asm-generic/errno-base.h /usr/include/dirent.h
../src/helper.o: /usr/include/bits/dirent.h /usr/include/bits/posix1_lim.h
../src/helper.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
../src/helper.o: socket.h ../../debug.h helper.h transfer.h
../src/espeech.o: /usr/include/stdio.h /usr/include/features.h
../src/espeech.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
../src/espeech.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/espeech.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/espeech.o: /usr/include/libio.h /usr/include/_G_config.h
../src/espeech.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/espeech.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
../src/espeech.o: /usr/include/bits/waitflags.h
../src/espeech.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
../src/espeech.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
../src/espeech.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
../src/espeech.o: /usr/include/time.h /usr/include/sys/select.h
../src/espeech.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../src/espeech.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
../src/espeech.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/espeech.o: /usr/include/bits/stdlib-float.h /usr/include/string.h
../src/espeech.o: /usr/include/xlocale.h /usr/include/errno.h
../src/espeech.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
../src/espeech.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
../src/espeech.o: /usr/include/asm-generic/errno-base.h
../src/espeech.o: /usr/include/espeak/speak_lib.h espeech.h ../../debug.h
../src/main.o: /usr/include/stdio.h /usr/include/features.h
../src/main.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
../src/main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/main.o: /usr/include/libio.h /usr/include/_G_config.h
../src/main.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/main.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
../src/main.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
../src/main.o: /usr/include/endian.h /usr/include/bits/endian.h
../src/main.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
../src/main.o: /usr/include/sys/types.h /usr/include/time.h
../src/main.o: /usr/include/sys/select.h /usr/include/bits/select.h
../src/main.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../src/main.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
../src/main.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
../src/main.o: /usr/include/string.h /usr/include/xlocale.h
../src/main.o: /usr/include/errno.h /usr/include/bits/errno.h
../src/main.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
../src/main.o: /usr/include/asm-generic/errno.h
../src/main.o: /usr/include/asm-generic/errno-base.h socket.h helper.h
../src/main.o: ../../debug.h
../src/transfer.o: /usr/include/stdio.h /usr/include/features.h
../src/transfer.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
../src/transfer.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../src/transfer.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
../src/transfer.o: /usr/include/libio.h /usr/include/_G_config.h
../src/transfer.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
../src/transfer.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
../src/transfer.o: /usr/include/bits/waitflags.h
../src/transfer.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
../src/transfer.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
../src/transfer.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
../src/transfer.o: /usr/include/time.h /usr/include/sys/select.h
../src/transfer.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../src/transfer.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
../src/transfer.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../src/transfer.o: /usr/include/bits/stdlib-float.h /usr/include/string.h
../src/transfer.o: /usr/include/xlocale.h /usr/include/errno.h
../src/transfer.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
../src/transfer.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
../src/transfer.o: /usr/include/asm-generic/errno-base.h socket.h
../src/transfer.o: ../../debug.h transfer.h
