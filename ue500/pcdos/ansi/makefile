#	Turbo C 2.0 makefile for
#		MicroEMACS 3.12
#		(C)Copyright 1993 by Daniel Lawrence
#		all rights reserved

# modify the following lines with the proper relative directories

OBJD = obj			# object file directory
SRCD = ..\..\src		# source file directory
HDIR = ..\..\h			# header file directory
INCD = e:\bc\include		# system include directory

# options and arguments to the C compiler

#CC = tcc
CC = bcc -g0

#These options or with debugging on!
CFLAGS=-c -d -I. -I$(HDIR) -I$(INCD) -K -ml -N- -n$(OBJD) -O -r -v -w-par -y -Z

#These options are production (no debugging)
#CFLAGS=-c -d -I. -I$(HDIR) -I$(INCD) -K -ml -N- -n$(OBJD) -O -r -w-par -Z

# list of header files

HFILES = ..\..\h\estruct.h ..\..\h\edef.h ..\..\h\efunc.h ..\..\h\epath.h ..\..\h\ebind.h ..\..\h\eproto.h

#list of source files

F1 = $(OBJD)\abbrev.obj $(OBJD)\basic.obj $(OBJD)\bind.obj $(OBJD)\buffer.obj
F2 = $(OBJD)\char.obj $(OBJD)\crypt.obj $(OBJD)\display.obj $(OBJD)\undo.obj
F3 = $(OBJD)\eval.obj $(OBJD)\exec.obj $(OBJD)\file.obj
F4 = $(OBJD)\fileio.obj $(OBJD)\ansi.obj $(OBJD)\input.obj $(OBJD)\keyboard.obj
F5 = $(OBJD)\isearch.obj $(OBJD)\line.obj $(OBJD)\lock.obj $(OBJD)\dolock.obj
F6 = $(OBJD)\main.obj $(OBJD)\mouse.obj $(OBJD)\msdos.obj $(OBJD)\screen.obj
F7 = $(OBJD)\random.obj $(OBJD)\region.obj $(OBJD)\search.obj $(OBJD)\replace.obj
F8 = $(OBJD)\tags.obj $(OBJD)\window.obj $(OBJD)\word.obj  $(OBJD)\execpr.obj

# product list

all:	bin\emacs.exe bin\cmdfix.exe

bin\emacs.exe: $(F1) $(F2) $(F3) $(F4) $(F5) $(F6) $(F7) $(F8)
	tlink @emacs.lnk

bin\cmdfix.exe: $(OBJD)\cmdfix.obj
	tlink @cmdfix.lnk

# compilation rules

$(OBJD)\abbrev.obj: $(SRCD)\abbrev.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\basic.obj: $(SRCD)\basic.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\bind.obj: $(SRCD)\bind.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\buffer.obj: $(SRCD)\buffer.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\char.obj: $(SRCD)\char.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\crypt.obj: $(SRCD)\crypt.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\cmdfix.obj: $(SRCD)\cmdfix.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\display.obj: $(SRCD)\display.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\eval.obj: $(SRCD)\eval.c $(HFILES) $(HDIR)\evar.h
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\exec.obj: $(SRCD)\exec.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\file.obj: $(SRCD)\file.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\fileio.obj: $(SRCD)\fileio.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\hp150.obj: $(SRCD)\hp150.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\ansi.obj: $(SRCD)\ansi.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\input.obj: $(SRCD)\input.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\isearch.obj: $(SRCD)\isearch.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\keyboard.obj: $(SRCD)\keyboard.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\line.obj: $(SRCD)\line.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\lock.obj: $(SRCD)\lock.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\dolock.obj: $(SRCD)\dolock.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\main.obj: $(SRCD)\main.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\mouse.obj: $(SRCD)\mouse.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\msdos.obj: $(SRCD)\msdos.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\random.obj: $(SRCD)\random.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\region.obj: $(SRCD)\region.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\search.obj: $(SRCD)\search.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\replace.obj: $(SRCD)\replace.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\screen.obj: $(SRCD)\screen.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\tags.obj: $(SRCD)\tags.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\undo.obj: $(SRCD)\undo.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\window.obj: $(SRCD)\window.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
$(OBJD)\word.obj: $(SRCD)\word.c $(HFILES)
	$(CC) $(CFLAGS) -o$(OBJD)\$&.obj $(SRCD)\$&.c
#$(OBJD)\execpr.obj: $(SRCD)\execpr.asm
#	masm $(SRCD)\execpr.asm, $(OBJD)\execpr.obj, , ,
$(OBJD)\execpr.obj: $(SRCD)\execpr.asm
	tasm $(SRCD)\execpr.asm,$(OBJD)\execpr.obj


clean:
	del obj\*.obj
	del emacs.map
	del cmdfix.map
	del bin\emacs.exe
	del bin\cmdfix.exe



# EOF
