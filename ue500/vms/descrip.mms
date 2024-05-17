!
! MMS/MMK file to compile and link MicroEmacs on the OpenVMS
! operating system (SMG version).
!
! To use:
!	MMS [/MACRO=DEBUG=1]
!

# MMK doesn't support MMS$ARCH_NAME, and MMS doesn't support __platform__
.ifdef __MMK__
.ifdef __x86_64__
MMS$ARCH_NAME = x86_64
.else
.ifdef __ia64__
MMS$ARCH_NAME = IA64
.else
.ifdef __ALPHA__
MMS$ARCH_NAME = Alpha
.else
.ifdef __VAX__
MMS$ARCH_NAME = VAX
.else
.error "Unknown architecture"
.endif
.endif
.endif
.endif
.endif


SRC=[-.SRC]
INC=[-.H]
LIS=[.BUILD.LIS_$(MMS$ARCH_NAME)]
.ifdef DEBUG
BLD = [.BUILD.BINDBG_$(MMS$ARCH_NAME)]
.else
BLD = [.BUILD.BIN_$(MMS$ARCH_NAME)]
.endif

.IFDEF DEBUG
CDBG = /nooptimize /debug
LDBG = /debug
LIST = 1
.ELSE
CDBG = /optimize
LDBG =
.ENDIF

.IFDEF LIST
CLST = /list=$(LIS) /show=(all)
LLST = /map=$(LIS) /cross_reference
.ELSE
CLST = /nolist
LLST = /map=$(LIS) /brief
.ENDIF

CFLAGS = /include=([], $(INC)) /object=$(MMS$TARGET) $(CDBG) $(CLST)
LINKFLAGS = $(LDBG) $(LLST)

!
! The header file epath.h is used only by
! bind.c at this writing, so it appears
! on bind.c's dependancy line only.
!
hfiles=	$(INC)ebind.h, -
	$(INC)edef.h, -
	$(INC)efunc.h, -
	$(INC)evar.h, -
	$(INC)estruct.h

ofiles=	$(BLD)abbrev.obj, -
	$(BLD)basic.obj, -
	$(BLD)bind.obj, -
	$(BLD)buffer.obj, -
	$(BLD)char.obj, -
	$(BLD)crypt.obj, -
	$(BLD)display.obj, -
	$(BLD)dolock.obj, -
	$(BLD)eval.obj, -
	$(BLD)exec.obj, -
	$(BLD)file.obj, -
	$(BLD)fileio.obj, -
	$(BLD)input.obj, -
	$(BLD)isearch.obj, -
	$(BLD)keyboard.obj, -
	$(BLD)line.obj, -
	$(BLD)lock.obj, -
	$(BLD)main.obj, -
	$(BLD)mouse.obj, -
	$(BLD)random.obj, -
	$(BLD)region.obj, -
	$(BLD)replace.obj, -
	$(BLD)screen.obj, -
	$(BLD)search.obj, -
	$(BLD)smg.obj, -
	$(BLD)tags.obj, -
	$(BLD)undo.obj, -
	$(BLD)vms.obj, -
	$(BLD)window.obj, -
	$(BLD)word.obj

omodules= -
       ABBREV=$(BLD)abbrev.obj, -
       BASIC=$(BLD)basic.obj, -
       BIND=$(BLD)bind.obj, -
       BUFFER=$(BLD)buffer.obj, -
       CHAR=$(BLD)char.obj, -
       CRYPT=$(BLD)crypt.obj, -
       DISPLAY=$(BLD)display.obj, -
       DOLOCK=$(BLD)dolock.obj, -
       EVAL=$(BLD)eval.obj, -
       EXEC=$(BLD)exec.obj, -
       FILE=$(BLD)file.obj, -
       FILEIO=$(BLD)fileio.obj, -
       INPUT=$(BLD)input.obj, -
       ISEARCH=$(BLD)isearch.obj, -
       KEYBOARD=$(BLD)keyboard.obj, -
       LINE=$(BLD)line.obj, -
       LOCK=$(BLD)lock.obj, -
       MAIN=$(BLD)main.obj, -
       MOUSE=$(BLD)mouse.obj, -
       RANDOM=$(BLD)random.obj, -
       REGION=$(BLD)region.obj, -
       REPLACE=$(BLD)replace.obj, -
       SCREEN=$(BLD)screen.obj, -
       SEARCH=$(BLD)search.obj, -
       SMG=$(BLD)smg.obj, -
       TAGS=$(BLD)tags.obj, -
       UNDO=$(BLD)undo.obj, -
       VMS=$(BLD)vms.obj, -
       WINDOW=$(BLD)window.obj, -
       WORD=$(BLD)word.obj

.first :
        @ IF F$SEARCH("[.BUILD]BIN_$(MMS$ARCH_NAME).DIR") .EQS. "" THEN -
                CREATE/DIRECTORY [.BUILD.BIN_$(MMS$ARCH_NAME)]
        @ IF F$SEARCH("[.BUILD]BINDBG_$(MMS$ARCH_NAME).DIR") .EQS. "" THEN -
                CREATE/DIRECTORY [.BUILD.BINDBG_$(MMS$ARCH_NAME)]
        @ IF F$SEARCH("[.BUILD]LIS_$(MMS$ARCH_NAME).DIR") .EQS. "" THEN -
                CREATE/DIRECTORY [.BUILD.LIS_$(MMS$ARCH_NAME)]

all : $(BLD)meshr.exe, $(BLD)me.exe
	@ write sys$output "All done."

$(BLD)meshr.exe : $(BLD)melib.olb, mesmg.mms, mesmg.opt
	$(LINK) $(LINKFLAGS) /shareable=$(MMS$TARGET) -
	$(BLD)melib.olb/library/include=main, -
	$(BLD)melib.olb/library, -
	[]me_$(MMS$ARCH_NAME).opt/option

$(BLD)me.exe : $(BLD)me.obj
	$(LINK) $(LINKFLAGS) /executable=$(MMS$TARGET) -
	$(BLD)me.obj

$(BLD)melib.olb : $(BLD)melib.olb( $(omodules) )
        ! $(LIBR) /compress $(MMS$TARGET) $(MMS$TARGET)
        @ continue

$(BLD)abbrev.obj : $(SRC)abbrev.c, $(HFILES)
$(BLD)basic.obj : $(SRC)basic.c, $(HFILES)
$(BLD)bind.obj : $(SRC)bind.c, $(INC)epath.h, $(HFILES)
$(BLD)buffer.obj : $(SRC)buffer.c, $(HFILES)
$(BLD)char.obj : $(SRC)char.c, $(HFILES)
$(BLD)crypt.obj : $(SRC)crypt.c, $(HFILES)
$(BLD)display.obj : $(SRC)display.c, $(HFILES)
$(BLD)dolock.obj : $(SRC)dolock.c, $(HFILES)
$(BLD)eval.obj : $(SRC)eval.c, $(HFILES)
$(BLD)exec.obj : $(SRC)exec.c, $(HFILES)
$(BLD)file.obj : $(SRC)file.c, $(HFILES)
$(BLD)fileio.obj : $(SRC)fileio.c, $(HFILES)
$(BLD)input.obj : $(SRC)input.c, $(HFILES)
$(BLD)isearch.obj : $(SRC)isearch.c, $(HFILES)
$(BLD)keyboard.obj : $(SRC)keyboard.c, $(HFILES)
$(BLD)line.obj : $(SRC)line.c, $(HFILES)
$(BLD)lock.obj : $(SRC)lock.c, $(HFILES)
$(BLD)main.obj : $(SRC)main.c, $(HFILES)
$(BLD)me.obj : []me.c
$(BLD)mouse.obj : $(SRC)mouse.c, $(HFILES)
$(BLD)random.obj : $(SRC)random.c, $(HFILES)
$(BLD)region.obj : $(SRC)region.c, $(HFILES)
$(BLD)replace.obj : $(SRC)replace.c, $(HFILES)
$(BLD)screen.obj : $(SRC)screen.c, $(HFILES)
$(BLD)search.obj : $(SRC)search.c, $(HFILES)
$(BLD)tags.obj : $(SRC)tags.c, $(HFILES)
$(BLD)undo.obj : $(SRC)undo.c, $(HFILES)
$(BLD)vms.obj : $(SRC)vms.c, $(HFILES)
$(BLD)smg.obj : $(SRC)smg.c, $(HFILES)
$(BLD)window.obj : $(SRC)window.c, $(HFILES)
$(BLD)word.obj : $(SRC)word.c, $(HFILES)

clean :
	if f$search("$(BLD)*.obj").nes."" then delete $(BLD)*.obj;*
	if f$search("$(BLD)*.olb").nes."" then delete $(BLD)*.olb;*

realclean : clean
	if f$search("$(BLD)*.exe").nes."" then delete $(BLD)*.exe;*
	if f$search("$(LIS)*.lis").nes."" then delete $(LIS)*.lis;*
	if f$search("$(LIS)*.map").nes."" then delete $(LIS)*.map;*
