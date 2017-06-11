noinst_HEADERS = game.h

flashdir = $(prefix)/share/@PACKAGE@/data
flash_DATA = ../data/heroeast.bmp \
	../data/herowest.bmp \
	../data/heronorth.bmp \
	../data/herosouth.bmp \
	../data/tile0.bmp \
	../data/tile1.bmp \
	../data/tile2.bmp \
	../data/title.bmp \
	../data/world1.map

bin_PROGRAMS = game
game_SOURCES = main.c
game_CPPFLAGS = -I@top_srcdir@
game_CFLAGS = `pkg-config --cflags allegro` -DDATADIR='"$(flashdir)"'
game_LDFLAGS = `pkg-config --libs allegro`

AUTOMAKE_OPTIONS = foreign
