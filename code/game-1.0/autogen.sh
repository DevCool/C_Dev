#!/bin/sh
case `uname` in Darwin*) glibtoolize --install ;;
    *) libtoolize --install ;; esac

autoheader
aclocal -I m4 --install
autoconf
automake -a -c -f
