#               Unix makefile for
#               MicroEMACS 3.12
#               (C)Copyright 1993 by Daniel Lawrence
#               all rights reserved

# modify the following lines with the proper relative directories

OBJD = obj			# object file directory
SRCD = ../../src		# source file directory
HDIR = ../../h			# header file directory


# C compiler
CC =gcc


# options and arguments to the C compiler
#
# - JES_REPAIR_CODE: Fix overwrites of the line data instead of bailing out
# - EMACS_TRC: See eproto.h: If TRC_FILE_ENVVAR is defined generate trace output into this file.
#
#CFLAGS = -DJES_REPAIR_CODE -DEMACS_TRC -I. -I$(HDIR) -g
#CFLAGS = -I. -I$(HDIR) -g
CFLAGS = -I. -I$(HDIR) -O2


# list of header files

HFILES = estruct.h $(HDIR)/edef.h $(HDIR)/efunc.h $(HDIR)/epath.h $(HDIR)/ebind.h $(HDIR)/eproto.h

#list of object files

F1 = $(OBJD)/abbrev.o $(OBJD)/basic.o $(OBJD)/bind.o $(OBJD)/buffer.o
F2 = $(OBJD)/char.o $(OBJD)/crypt.o $(OBJD)/display.o $(OBJD)/dolock.o
F3 = $(OBJD)/eval.o $(OBJD)/exec.o $(OBJD)/file.o $(OBJD)/keyboard.o
F4 = $(OBJD)/fileio.o $(OBJD)/unix.o $(OBJD)/input.o
F5 = $(OBJD)/isearch.o $(OBJD)/line.o $(OBJD)/lock.o
F6 = $(OBJD)/main.o $(OBJD)/mouse.o $(OBJD)/screen.o
F7 = $(OBJD)/random.o $(OBJD)/region.o $(OBJD)/search.o $(OBJD)/replace.o
F8 = $(OBJD)/tags.o $(OBJD)/undo.o $(OBJD)/window.o $(OBJD)/word.o

# product list

all:	bin/emacs

OFILES =	$(F1) $(F2) $(F3) $(F4) $(F5) $(F6) $(F7) $(F8)

bin/emacs:	$(OFILES)
		$(CC) $(CFLAGS) $(OFILES) -ltermcap -lc -o bin/emacs

$(OBJD)/abbrev.o: $(SRCD)/abbrev.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/abbrev.o $(SRCD)/abbrev.c
$(OBJD)/basic.o: $(SRCD)/basic.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/basic.o $(SRCD)/basic.c
$(OBJD)/bind.o: $(SRCD)/bind.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/bind.o $(SRCD)/bind.c
$(OBJD)/buffer.o: $(SRCD)/buffer.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/buffer.o $(SRCD)/buffer.c
$(OBJD)/char.o: $(SRCD)/char.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/char.o $(SRCD)/char.c
$(OBJD)/crypt.o: $(SRCD)/crypt.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/crypt.o $(SRCD)/crypt.c
$(OBJD)/cmdfix.o: $(SRCD)/cmdfix.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/cmdfix.o $(SRCD)/cmdfix.c
$(OBJD)/display.o: $(SRCD)/display.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/display.o $(SRCD)/display.c
$(OBJD)/dolock.o: $(SRCD)/dolock.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/dolock.o $(SRCD)/dolock.c
$(OBJD)/eval.o: $(SRCD)/eval.c $(HFILES) $(HDIR)/evar.h
	$(CC) $(CFLAGS) -c -o $(OBJD)/eval.o $(SRCD)/eval.c
$(OBJD)/exec.o: $(SRCD)/exec.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/exec.o $(SRCD)/exec.c
$(OBJD)/file.o: $(SRCD)/file.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/file.o $(SRCD)/file.c
$(OBJD)/fileio.o: $(SRCD)/fileio.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/fileio.o $(SRCD)/fileio.c
$(OBJD)/input.o: $(SRCD)/input.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/input.o $(SRCD)/input.c
$(OBJD)/isearch.o: $(SRCD)/isearch.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/isearch.o $(SRCD)/isearch.c
$(OBJD)/keyboard.o: $(SRCD)/keyboard.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/keyboard.o $(SRCD)/keyboard.c
$(OBJD)/line.o: $(SRCD)/line.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/line.o $(SRCD)/line.c
$(OBJD)/lock.o: $(SRCD)/lock.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/lock.o $(SRCD)/lock.c
$(OBJD)/main.o: $(SRCD)/main.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/main.o $(SRCD)/main.c
$(OBJD)/mouse.o: $(SRCD)/mouse.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/mouse.o $(SRCD)/mouse.c
$(OBJD)/screen.o: $(SRCD)/screen.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/screen.o $(SRCD)/screen.c
$(OBJD)/unix.o: $(SRCD)/unix.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/unix.o $(SRCD)/unix.c
$(OBJD)/random.o: $(SRCD)/random.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/random.o $(SRCD)/random.c
$(OBJD)/region.o: $(SRCD)/region.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/region.o $(SRCD)/region.c
$(OBJD)/search.o: $(SRCD)/search.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/search.o $(SRCD)/search.c
$(OBJD)/replace.o: $(SRCD)/replace.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/replace.o $(SRCD)/replace.c
$(OBJD)/tags.o: $(SRCD)/tags.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/tags.o $(SRCD)/tags.c
$(OBJD)/undo.o: $(SRCD)/undo.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/undo.o $(SRCD)/undo.c
$(OBJD)/window.o: $(SRCD)/window.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/window.o $(SRCD)/window.c
$(OBJD)/word.o: $(SRCD)/word.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $(OBJD)/word.o $(SRCD)/word.c


clean:
	rm -f $(OBJD)/*.o
	rm -f bin/emacs



# EOF

