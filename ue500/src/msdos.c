/*======================================================================
 *      MSDOS.C:        Operating specific I/O and Spawning functions
 *                      under the MS/PCDOS operating system
 *                      for MicroEMACS 4.00
 *                      (C)Copyright 1995 by Daniel M. Lawrence
 *====================================================================*/

/*====================================================================*/
#define MSDOS_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"

#ifdef  MSDOS
# include        "edef.h"
# include        "elang.h"

/* The Mouse driver only works with typeahead defined */
# if     MOUSE
#  undef  TYPEAH
#  define TYPEAH  1
# endif

# if  TURBO
#  include <conio.h>
#  include <dir.h>
#  include <dos.h>
#  include <bios.h>

static struct ffblk fileblock;    /* structure for directory searches */
# endif

# if     MSC | ZTC
#  include <dos.h>

static struct find_t  fileblock;  /* structure for directory searches */
# endif

# if     LATTICE | MSC | TURBO | IC | MWC | ZTC
union REGS          rg;           /* cpu REGISTER for use of DOS calls
                                   */
static struct SREGS segreg;       /* cpu segment registers          */
static int          nxtchar = -1; /* character held from type ahead */
# endif

# if     MSC | TURBO
#  include        <process.h>
# endif

# if     IC
#  include        <time.h>
# endif

/* Some constants which we do not want to have as literals inside
 * the code:  */
# if ( 0 ) /* Old value  */
#   define TIME_BUF_SIZE  16
# else
#   if IC | TURBO
#     define TIME_BUF_SIZE ( SIZEOF(time_t) )
#   else
#     define TIME_BUF_SIZE ( 32 )
#   endif
# endif

/*  Some global variable    */
# define INBUFSIZ        40
static int mexist;      /* is the mouse driver installed? */
static int nbuttons;    /* number of buttons on the mouse */
static int oldbut;      /* Previous state of mouse buttons */
static int oldcol;      /* previous x position of mouse */
static int oldrow;      /* previous y position of mouse */

static int PASCAL NEAR execprog DCL((CONST char *cmd));

/*  input buffers and pointers  */

# define IBUFSIZE        64     /* this must be a power of 2 */

static unsigned char  in_buf[IBUFSIZE]; /* input character buffer */
static int            in_next = 0;      /* pos to retrieve next input character */
static int            in_last = 0;      /* pos to place most recent input character */

/* IN_INIT:
 *
 * Initialize the input buffer
 */
static VOID in_init P0_()
{
    in_next = in_last = 0;
}

/* IN_CHECK:
 *
 * Is the input buffer non-empty?
 */
static int in_check P0_()
{
    if ( in_next == in_last )
        return (FALSE);
    else
        return (TRUE);
}

/* IN_PUT:
 */
static VOID in_put P1_(int, event)
/* event: Event to enter into the input buffer  */
{
    in_buf[in_last++] = event;
    in_last &= (IBUFSIZE - 1);
}

/* IN_GET:
 *
 * Get an event from the input buffer
 */
int in_get P0_()
{
    REGISTER int  event = 0;    /* event to return */

    event = in_buf[in_next++];
    in_next &= (IBUFSIZE - 1);

    return (event);
}

/* TTOPEN:
 *
 * This function is called once to set up the terminal device streams.
 */
int PASCAL NEAR ttopen P0_()
{
# if     MOUSE
    long  miaddr  = 0;    /* mouse interupt routine address */
# endif

    /* on all screens we are not sure of the initial position of the cursor                 */
    ttrow = 999;
    ttcol = 999;
    XSTRCPY(os, "MSDOS");

# if     MOUSE
    /* check if the mouse drive exists first */
    rg.x.ax = 0x3533;           /* look at the interrupt 33 address */
#  if    MSC | TURBO | IC | LATTICE | MWC
    int86x(0x21, &rg, &rg, &segreg);
    miaddr = ( ( (long)segreg.es ) << 16 ) + (long)rg.x.bx;
    if ( miaddr == 0 || *(char * far)miaddr == 0xcf ) {
        mexist = FALSE;

        return (TRUE);
    }
#  elif  ZTC
    int86x(0x21, &rg, &rg, &segreg);
    miaddr = ( ( (long)segreg.es ) << 16 ) + (long)rg.x.bx;
    if ( miaddr == 0 || *(char far *)miaddr == 0xcf ) {
        mexist = FALSE;

        return (TRUE);
    }
#  else
#   error UNSUPPORTED COMPILER
#  endif

    /* and then check for the mouse itself */
    rg.x.ax = 0;                /* mouse status flag */
    int86(0x33, &rg, &rg);      /* check for the mouse interupt */
    mexist = (rg.x.ax != 0);
    nbuttons = rg.x.bx;

    /* initialize our character input queue */
    in_init();
    if ( mexist == FALSE )
        return (TRUE);

    /* if the mouse exists.. get it in the upper right corner */
    rg.x.ax = 4;                /* set mouse cursor position */
    oldcol = (term.t_ncol - 1);
    rg.x.cx = oldcol << 3;      /* last col of display */
    oldrow = 0;
    rg.x.dx = oldrow;           /* top row */
    oldbut = 0;
    int86(0x33, &rg, &rg);

    /* and set its attributes */
    rg.x.ax = 10;               /* set text cursor */
    rg.x.bx = 0;                /* software text cursor please */
    rg.x.cx = 0x77ff;           /* screen mask */
    rg.x.dx = 0x7700;           /* cursor mask */
    int86(0x33, &rg, &rg);
# else  /* !MOUSE */
    mexist = 0;
# endif /* MOUSE  */

    return TRUE;
}

/* MAXLINES:
 *
 * Set number of vertical rows for mouse
 */
VOID PASCAL NEAR  maxlines P1_(int, lines)
/* lines: # of vertical lines */
{
# if     MOUSE
    if ( mexist ) {
        rg.x.ax = 8;                    /* set min/max vertical cursor position
                                         */
        rg.x.cx = 0;                    /* start at 0 */
        rg.x.dx = (lines - 1)<<3;         /* end at the end */
        int86(0x33, &rg, &rg);
    }
# endif
}

/* TTCLOSE:
 *
 * This function gets called just before we go back home to the command
 * interpreter. On VMS it puts the terminal back in a reasonable state. Another
 * no-operation on CPM.
 */
int PASCAL NEAR ttclose P0_()
{
    /* nothing here! */

    return TRUE;
}

/* TTPUTC:
 *
 * Write a character to the display. On VMS, terminal output is buffered, and we
 * just put the characters in the big array, after checking for overflow. On CPM
 * terminal I/O unbuffered, so we just write the byte out. Ditto on MS-DOS (use
 * the very very raw console output routine).
 */
int PASCAL NEAR ttputc P1_(int, c)
{
# if     MWC
    putcnb(c);
# endif

# if     (LATTICE | TURBO | IC | MSC | ZTC) & ~IBMPC
    bdos(6, c, 0);
# endif

    return TRUE;
}

/* TTFLUSH:
 *
 * Flush terminal buffer. Does real work where the terminal output is buffered
 * up. A no-operation on systems where byte at a time terminal I/O is done.
 */
int PASCAL NEAR ttflush P0_()
{
    return TRUE;
}

/* doschar:
 *
 * Call the dos to get a char
 */
int doschar P0_()
{
# if ATKBD      /* AT-style extended keyboard with grey keys */

    rg.h.ah = 0x10;                     /* Get Keyboard Input call */
    int86(0x16, &rg, &rg);

    /* function key!! */
    if ( rg.h.al == 0 || rg.h.al == 0xE0 ) {

        rg.h.cl = rg.h.ah;              /* swap scan code and value */
        rg.h.ch = rg.h.al;
        rg.x.cx = extcode(rg.x.cx);
        in_put(rg.h.ch & 255);          /* prefix byte */
        in_put(rg.h.cl & 255);          /* event code byte */

        return (0);                     /* extended escape sequence */
    }

    return (rg.h.al & 255);

# else  /* !ATKBD */

#  if     (TURBO | IC) && HP150 == 0
/* Added 8/13/89 by DRK to get Turbo C version to use BIOS for keyboard input.
 * Low byte of k has first byte of extended scan (ASCII value for reg. char, 0
 * for extended char), high byte has extended key scan code.
 */

    unsigned int  k = (unsigned)bioskey(0);
    unsigned int  c = 0;  /* Extended character to return. */

    if ( (k & 0xFF) == 0 ) {      /* Check for extended key. */
                                  /* Convert extended key scan code to a uEMACS
                                   * internal form. */
        c = extcode(k >> 8);
        in_put(c >> 8);           /* Report prefix and event code bytes. */
        in_put(c & 0xFF);

        return (0);               /* Return extended escape sequence. */
    }

    return (k & 0xFF);            /* Return regular ASCII value. */

#  else
    REGISTER unsigned int c = 0;  /* extended character to return */

    rg.h.ah = 7;                  /* dos Direct Console Input call */
    intdos(&rg, &rg);
#   if     HP150 == 0             /* this translation level is deeper on the HP150
                                   */
    if ( rg.h.al == 0 ) {         /* function key!! */
        rg.h.ah = 7;              /* get the next character */
        intdos(&rg, &rg);
        c = extcode(rg.h.al);
        in_put(c >> 8);           /* prefix byte */
        in_put(c & 255);          /* event code byte */

        return (0);               /* extended escape sequence */
    }
#   endif

    return (rg.h.al & 255);

#  endif  /* (TURBO | IC) && HP150 == 0 */
# endif /* ATKBD  */
}

/* TTGETC:
 *
 * Read a character from the terminal, performing no editing and doing no echo
 * at all. Also mouse events are forced into the input stream here.
 */
int PASCAL NEAR ttgetc P0_()
{
ttc:    /* return any keystrokes waiting in the type ahead buffer */
    if ( in_check() )
        return ( in_get() );

# if     TYPEAH
    if ( typahead() )
        return ( doschar() );

    /* with no mouse, this is a simple get char routine */
    if ( mexist == FALSE || mouseflag == FALSE )
        return ( doschar() );

#  if     MOUSE
    /* turn the mouse cursor on */
    rg.x.ax = 1;        /* Show Cursor */
    int86(0x33, &rg, &rg);

    /* loop waiting for something to happen */
    while ( TRUE ) {
        if ( typahead() )
            break;
        if ( checkmouse() )
            break;
    }

    /* turn the mouse cursor back off */
    rg.x.ax = 2;        /* Hide Cursor */
    int86(0x33, &rg, &rg);

    goto ttc;
#  endif  /* MOUSE  */
# else  /* !TYPEAH  */

    return ( doschar() );

# endif /* TYPEAH */
}

# if     MOUSE

/* CHECKMOUSE:
 */
int PASCAL NEAR checkmouse P0_()
{
    REGISTER int  k         = 0;  /* current bit/button of mouse */
    REGISTER int  etype     = 0;  /* event type byte */
    REGISTER int  event     = 0;  /* encoded mouse event */
    int           mousecol  = 0;  /* current mouse column */
    int           mouserow  = 0;  /* current mouse row */
    int           sstate    = 0;  /* current shift key status */
    int           newbut    = 0;  /* new state of the mouse buttons */

    /* check to see if any mouse buttons are different */
    rg.x.ax = 3;        /* Get button status and mouse position */
    int86(0x33, &rg, &rg);
    newbut = rg.x.bx;
    if ( strcmp(sres, "CGA40") == 0 )
        mousecol = rg.x.cx >> 4;
    else     /* if (term.t_ncol == 80) */
        mousecol = rg.x.cx >> 3;
    mouserow = (rg.x.dx >> 3);

    /* only notice changes */
    if ( (oldbut == newbut) && (mousecol == oldcol)&& (mouserow == oldrow) )
        return (FALSE);

    /* get the shift key status as well */
    etype = MOUS >> 8;
    sstate = 0;
    rg.h.ah = 2;        /* return current shift status */
    int86(0x16, &rg, &rg);
    sstate = rg.h.al;
    if ( sstate & 3 )           /* shifted? */
        etype |= (SHFT >> 8);
    if ( sstate & 4 )           /* controled? */
        etype |= (CTRF >> 8);

    /* no buttons changes */
    if ( oldbut == newbut ) {
        /* generate a mouse movement */
        if ( ( (mouse_move == 1) && (mmove_flag == TRUE) ) ||
             (mouse_move == 2) ) {
            in_put(0);
            in_put(etype);
            in_put(mousecol);
            in_put(mouserow);
            in_put('m');
        }
        oldcol = mousecol;
        oldrow = mouserow;

        return (TRUE);
    }

    /* only on screen presses are legit! */
    if ( mousecol < 0 )
        mousecol = 0;
    if ( mouserow < 0 )
        mouserow = 0;

    for ( k=1; k != (1 << nbuttons); k = k<<1 ) {
        /* For each button on the mouse */
        if ( (oldbut&k) != (newbut&k) ) {
            /* This button changed, generate an event */
            in_put(0);
            in_put(etype);
            in_put(mousecol);
            in_put(mouserow);

            event = ( (newbut&k) ? 0 : 1 );             /* up or down? */
            if ( k == 2 )                               /* center button? */
                event += 4;
            if ( k == 4 )                               /* right button? */
                event += 2;
            event += 'a';                       /* plain */
            in_put(event);
            oldbut = newbut;
            oldcol = mousecol;
            oldrow = mouserow;

            return (TRUE);
        }
    }

    return (FALSE);
}

# endif /* MOUSE  */

# if     TYPEAH

/* TYPAHEAD:
 *
 * Check to see if any characters are already in the keyboard buffer
 */
int PASCAL NEAR typahead P0_()
{
#  if     (TURBO | IC ) && HP150 == 0 && ATKBD == 0
    if ( bioskey(1) == 0 )
        return FALSE;
    else
        return TRUE;

#  else
    int flags = 0;        /* cpu flags from dos call */

    rg.x.ax = 0x4406;           /* IOCTL input status */
    rg.x.bx = 0;                /* File handle = stdin */
#   if     MSC
    int86(0x21, &rg, &rg);
    flags = rg.h.al;
#   else
#    if     LATTICE | TURBO | IC | ZTC
    flags = intdos(&rg, &rg);
#    else
    intcall(&rg, &rg, 0x21);
    flags = rg.x.flags;
#    endif
#   endif
    if ( flags & 1 )            /* AL = 0xFF if ready */
        return (TRUE);
    else
        return (FALSE);

#  endif
}

# endif /* TYPEAH */

/* SPAWNCLI:
 *
 * Create a subjob with a copy of the command intrepreter in it. When the
 * command interpreter exits, mark the screen as garbage so that you do a full
 * repaint. Bound to "^X C".
 */
int PASCAL NEAR spawncli P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    /* don't allow this command if restricted */
    if ( restflag )
        return ( resterr() );

    movecursor(term.t_nrow, 0);                 /* Seek to last line.   */
    TTflush();
    TTkclose();
    shellprog("");
# if     WINDOW_TEXT
    refresh_screen(first_screen);
# endif
    TTkopen();
    sgarbf = TRUE;

    return (TRUE);
}

/* SPAWN:
 *
 * Run a one-liner in a subjob. When the command returns, wait for a single
 * character to be typed, then mark the screen as garbage so a full repaint is
 * done.
 *
 * Bound to "C-X !".
 */
int PASCAL NEAR spawn P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int  s = 0;
    char          line[NLINE];

    ZEROMEM(line);

    /* don't allow this command if restricted */
    if ( restflag )
        return ( resterr() );

    if ( ( s=mlreply("!", line, NLINE) ) != TRUE )
        return (s);

    movecursor(term.t_nrow - 1, 0);
    TTkclose();
    shellprog(line);
    TTkopen();

    /* if we are interactive, pause here */
    if ( clexec == FALSE ) {
# if     XVT == 0
        printf(TEXT227);
/*          "\n--- Press any key to Continue ---" */
# endif
        tgetc();
    }
# if     WINDOW_TEXT
    refresh_screen(first_screen);
# endif
    sgarbf = TRUE;

    return (TRUE);
}

/* EXECPRG:
 *
 * Run an external program with arguments. When it returns, wait for a single
 * character to be typed, then mark the screen as garbage so a full repaint is
 * done. Bound to "C-X $".
 */
int PASCAL NEAR execprg P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int  s = 0;
    char          line[NLINE];

    ZEROMEM(line);

    /* don't allow this command if restricted */
    if ( restflag )
        return ( resterr() );

    if ( ( s=mlreply("$", line, NLINE) ) != TRUE )
        return (s);

    movecursor(term.t_nrow - 1, 0);
    TTkclose();
    execprog(line);
# if     WINDOW_TEXT
    refresh_screen(first_screen);
# endif
    TTkopen();
    /* if we are interactive, pause here */
    if ( clexec == FALSE ) {
        mlputs(TEXT6);
/*                     "\r\n\n[End]" */
        tgetc();
    }
    sgarbf = TRUE;

    return (TRUE);
}

/* PIPECMD:
 *
 * Pipe a one line command into a window Bound to ^X @
 */
int PASCAL NEAR pipecmd P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER EWINDOW  *wp   = NULL;   /* pointer to new window */
    REGISTER BUFFER   *bp   = NULL;   /* pointer to buffer to zot */
    REGISTER char     *tmp  = NULL;   /* ptr to TMP DOS environment variable */
    FILE              *fp   = NULL;
    char              line[NLINE];    /* command line send to shell */
    static char       bname[]         = "command";
    static char       filnam[NSTRING] = "command";

    ZEROMEM(line);

    /* don't allow this command if restricted */
    if ( restflag )
        return ( resterr() );

    if ( ( tmp = getenv("TMP") ) == NULL )
        filnam[0] = 0;
    else {
        XSTRCPY(filnam, tmp);
        if ( filnam[STRLEN(filnam) - 1] != '\\' )
            XSTRCAT(filnam, "\\");
    }
    XSTRCAT(filnam, "command");

    /* get the command to pipe in */
    if ( mlreply("@", line, NLINE) != TRUE )
        return (FALSE);

    /* get rid of the command output buffer if it exists */
    if ( ( bp=bfind(bname, FALSE, 0) ) != FALSE ) {
        /* try to make sure we are off screen */
        wp = wheadp;
        while ( wp != NULL ) {
            if ( wp->w_bufp == bp ) {
                onlywind(FALSE, 1);
                break;
            }
            wp = wp->w_wndp;
        }
        /* get rid of the existing command buffer */
        if ( zotbuf(bp) != TRUE )
            return (FALSE);
    }

    /* redirect the command output to the output file */
    XSTRCAT(line, " >>");
    XSTRCAT(line, filnam);
    movecursor(term.t_nrow - 1, 0);

    /* execute the command */
    TTkclose();
    shellprog(line);
# if     WINDOW_TEXT
    refresh_screen(first_screen);
# endif
    TTkopen();
    sgarbf = TRUE;

    /* did the output file get generated? */
    if ( ( fp = fopen(filnam, "r") ) == NULL )
        return (FALSE);

    fclose(fp);

    /* split the current window to make room for the command output */
    if ( splitwind(FALSE, 1) == FALSE )
        return (FALSE);

    /* and read the stuff in */
    if ( getfile(filnam, FALSE) == FALSE )
        return (FALSE);

    /* make this window in VIEW mode, update all mode lines */
    curwp->w_bufp->b_mode |= MDVIEW;
    wp = wheadp;
    while ( wp != NULL ) {
        wp->w_flag |= WFMODE;
        wp = wp->w_wndp;
    }

    /* and get rid of the temporary file */
    umc_unlink(filnam);

    return (TRUE);
}

/* F_FILTER:
 *
 * Filter a buffer through an external DOS program Bound to ^X #
 */
int PASCAL NEAR f_filter P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int    s   = 0;        /* return status from CLI */
    REGISTER BUFFER *bp = NULL;     /* pointer to buffer to zot */
    char            line[NLINE];    /* command line send to shell */
    char            tmpnam[NFILEN]; /* place to store real file name */
    static char     bname1[]  = "fltinp";
    static char     filnam1[] = "fltinp";
    static char     filnam2[] = "fltout";

    ZEROMEM(line);
    ZEROMEM(tmpnam);

    /* don't allow this command if restricted */
    if ( restflag )
        return ( resterr() );

    if ( curbp->b_mode&MDVIEW )         /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    /* get the filter name and its args */
    if ( ( s=mlreply("#", line, NLINE) ) != TRUE )
        return (s);

    /* setup the proper file names */
    bp = curbp;
    XSTRCPY(tmpnam, bp->b_fname);       /* save the original name */
    XSTRCPY(bp->b_fname, bname1);       /* set it to our new one */

    /* write it out, checking for errors */
    if ( writeout(filnam1, "w") != TRUE ) {
        mlwrite(TEXT2);
/*                      "[Cannot write filter file]" */
        XSTRCPY(bp->b_fname, tmpnam);

        return (FALSE);
    }

    XSTRCAT(line, " <fltinp >fltout");
    movecursor(term.t_nrow - 1, 0);
    TTkclose();
    shellprog(line);
# if     WINDOW_TEXT
    refresh_screen(first_screen);
# endif
    TTkopen();
    sgarbf = TRUE;
    s = TRUE;

    /* on failure, escape gracefully */
    if ( s != TRUE || (readin(filnam2, FALSE) == FALSE) ) {
        mlwrite(TEXT3);
/*                      "[Execution failed]" */
        XSTRCPY(bp->b_fname, tmpnam);
        umc_unlink(filnam1);
        umc_unlink(filnam2);

        return (s);
    }

    /* reset file name */
    XSTRCPY(bp->b_fname, tmpnam);       /* restore name */
    bp->b_flag |= BFCHG;                /* flag it as changed */

    /* and get rid of the temporary file */
    umc_unlink(filnam1);
    umc_unlink(filnam2);

    return (TRUE);
}

# if     LATTICE
COMMON int _oserr;
# endif

# if     MWC
COMMON int errno;
# endif

# if     MSC
COMMON int _doserrno;
# endif

/* SHELLPROG:
 *
 * Execute a command in a subshell
 */
int PASCAL NEAR shellprog P1_(char *, cmd)
/* cmd: Incoming command line to execute  */
{
    char        *shell  = NULL;     /* Name of system command processor */
    char        swchar  = 0;        /* switch character to use */
    union REGS  regs;               /* parameters for dos call */
    char        comline[NSTRING];   /* constructed command line */

    ZEROMEM(regs);
    ZEROMEM(comline);

    /*  detect current switch character and set us up to use it */
    regs.h.ah = 0x37;           /*  get setting data  */
    regs.h.al = 0x00;           /*  get switch character  */
    intdos(&regs, &regs);
    swchar = (char)regs.h.dl;

    /*  get name of system shell  */
    if ( ( shell = getenv("COMSPEC") ) == NULL ) {
        return (FALSE);                 /*  No shell located  */
    }

    /* trim leading whitespace off the command */
    while ( *cmd == ' ' || *cmd == '\t' )       /*  find out if null command */
        cmd++;

    /**  If the command line is not empty, bring up the shell  **/
    /**  and execute the command.  Otherwise, bring up the     **/
    /**  shell in interactive mode.                            **/

    if ( *cmd ) {
        XSTRCPY(comline, shell);
        XSTRCAT(comline, " ");
        comline[STRLEN(comline) + 1] = 0;
        comline[STRLEN(comline)] = swchar;
        XSTRCAT(comline, "c ");
        XSTRCAT(comline, cmd);

        return ( execprog(comline) );
    } else
        return ( execprog(shell) );
}


/* EXECPROG:
 *
 * A function to execute a named program with arguments
 */
static int PASCAL NEAR execprog P1_(CONST char *, cmd)
/* cmd: Incoming command line to execute  */
{
# if     LATTICE | MWC
#  define CFLAG   1
# endif

    char        *sp   = NULL;   /* temporary string pointer */
    CONST char  *csp  = NULL;   /* temporary string pointer */
    int         rv    = 0;      /* numeric return value from subprocess */
    char        f1[38];         /* FCB1 area (not initialized */
    char        f2[38];         /* FCB2 area (not initialized */
    char        prog[NSTRING];  /* program filespec */
    char        tail[NSTRING];  /* command tail with length byte */
    union REGS  regs;           /* parameters for dos call  */
# if     MWC == 0
    struct SREGS segreg;        /* segment registers for dis call */
# endif
    struct Pblock {             /* EXEC parameter block */
        short envptr;           /* 2 byte pointer to environment string */
        char *cline;            /* 4 byte pointer to command line */
        char *fcb1;             /* 4 byte pointer to FCB at PSP+5Ch */
        char *fcb2;             /* 4 byte pointer to FCB at PSP+6Ch */
    }
    pblock;

    ZEROMEM(f1);
    ZEROMEM(f2);
    ZEROMEM(prog);
    ZEROMEM(tail);
    ZEROMEM(regs);
# if     MWC == 0
    ZEROMEM(segreg);
# endif
    ZEROMEM(pblock);

    /* parse the command name from the command line */
    sp = prog;
    while ( (*cmd != '\0') && (*cmd != ' ') && (*cmd != '\t') )
        *sp++ = *cmd++;
    *sp = 0;

    /* and parse out the command tail */
    while ( *cmd && ( (*cmd == ' ') || (*cmd == '\t') ) )
        ++cmd;
    *tail = (char)( STRLEN(cmd) );   /* record the byte length */
    xstrlcpy(&tail[1], cmd, SIZEOF(tail) - 1);
    xstrlcat(&tail[1], "\r", SIZEOF(tail) - 1);

    /* look up the program on the path trying various extentions */
    if ( ( csp = flook(prog, TRUE, TRUE) ) == NULL )
        if ( ( csp = flook(XSTRCAT(prog, ".exe"), TRUE, TRUE) ) == NULL ) {
            xstrcpy(&prog[STRLEN(prog)-4], ".com"); /**UNSAFE_OK**/
            if ( ( csp = flook(prog, TRUE, TRUE) ) == NULL )
                return (FALSE);
        }
    XSTRCPY(prog, csp);

# if     MWC == 0
    /* get a pointer to this PSPs environment segment number */
#  if     IC
    _segread(&segreg);
#  else /* IC */
    segread(&segreg);
#  endif /* IC */
# endif /* MWC == 0 */

    /* set up the EXEC parameter block */
    pblock.envptr = 0;          /* make the child inherit the parents env */
    pblock.fcb1 = f1;                   /* point to a blank FCB */
    pblock.fcb2 = f2;                   /* point to a blank FCB */
    pblock.cline = tail;                /* parameter line pointer */

    /* and make the call */
    regs.h.ah = 0x4b;           /* EXEC Load or Execute a Program */
    regs.h.al = 0x00;           /* load end execute function subcode */
# if     MWC
    regs.x.ds = ( (unsigned long)(prog) >> 16 );        /* program name ptr */
    regs.x.dx = (unsigned int)(prog);
    regs.x.es = regs.x.ds;
    /*regs.x.es = ((unsigned long)(&pblock) >> 16); * set up param block ptr */
    regs.x.bx = (unsigned int)(&pblock);
# endif
# if     LATTICE | MSC | TURBO | IC | ZTC
    segreg.ds = ( (unsigned long)(prog) >> 16 );        /* program name ptr */
    regs.x.dx = (unsigned int)(prog);
    segreg.es = ( (unsigned long)(&pblock) >> 16 );     /* set up param block
                                                         * ptr */
    regs.x.bx = (unsigned int)(&pblock);
# endif
# if     LATTICE
    if ( (intdosx(&regs, &regs, &segreg) & CFLAG) == 0 ) {
        regs.h.ah = 0x4d;               /* get child process return code */
        intdos(&regs, &regs);           /* go do it */
        rv = regs.x.ax;                 /* save child's return code */
    } else
        rv = -_oserr;                   /* failed child call */
# endif
# if     MWC
    intcall(&regs, &regs, DOSINT);
    if ( (regs.x.flags & CFLAG) == 0 ) {
        regs.h.ah = 0x4d;               /* get child process return code */
        intcall(&regs, &regs, DOSINT);          /* go do it */
        rv = regs.x.ax;                 /* save child's return code */
    } else
        rv = -errno;                    /* failed child call */
# endif
# if     TURBO | IC | MSC | ZTC
    intdosx(&regs, &regs, &segreg);
    if ( regs.x.cflag == 0 ) {
        regs.h.ah = 0x4d;               /* get child process return code */
        intdos(&regs, &regs);           /* go do it */
        rv = regs.x.ax;                 /* save child's return code */
    } else
#  if     IC
        rv = -1;
#  else /* IC */
        rv = -_doserrno;                /* failed child call */
#  endif /* IC */
# endif
    xstrcpy( rval, int_asc(rv) );   /**TODO: rval**/

    return ( (rval < 0) ? FALSE : TRUE );
}

/* TIMESET:
 *
 * Return a system dependant string with the current time
 */
char *PASCAL NEAR timeset P0_()
{
# if  MWC | TURBO | IC | MSC | ZTC
    REGISTER char *sp = NULL;     /* temp string pointer */
    char     buf[TIME_BUF_SIZE];  /* time data buffer */

    ZEROMEM(buf);

#  if IC | TURBO
    time((time_t *)buf);
    sp = ctime((time_t *)buf);
#  else
    time(buf);
    sp = ctime(buf);
#  endif
    sp[STRLEN(sp)-1] = 0;

    return (sp);

# else

    return (errorm);

# endif
}

# if     TURBO
/*  FILE Directory routines     */

static char path[NFILEN];   /* path of file to find */
static char rbuf[NFILEN];   /* return file buffer */

/* GETFFILE:
 *
 * Do a wild card directory search (for file name completion)
 */
char *PASCAL NEAR getffile P1_(char *, fspec)
/* fspec: Pattern to match  */
{
    REGISTER int  index   = 0;      /* index into various strings */
    REGISTER int  point   = 0;      /* index into other strings */
    REGISTER int  extflag = 0;      /* does the file have an extention? */
    char          fname[NFILEN];    /* file/path for DOS call */

    ZEROMEM(fname);

    /* first parse the file path off the file spec */
    XSTRCPY(path, fspec);
    index = STRLEN(path) - 1;
    while ( index >= 0 &&
            (path[index] != '/' &&path[index] != '\\' && path[index] != ':') )
        --index;
    path[index+1] = 0;

    /* check for an extension */
    point = STRLEN(fspec) - 1;
    extflag = FALSE;
    while ( point > index ) {
        if ( fspec[point] == '.' ) {
            extflag = TRUE;
            break;
        }
        point--;
    }

    /* construct the composite wild card spec */
    XSTRCPY(fname, path);
    XSTRCAT(fname, &fspec[index+1]);
    XSTRCAT(fname, "*");
    if ( extflag == FALSE )
        XSTRCAT(fname, ".*");

    /* and call for the first file */
    if ( findfirst(fname, &fileblock, FA_DIREC) == -1 )
        return (NULL);

    /* return the first file name! */
    XSTRCPY(rbuf, path);
    XSTRCAT(rbuf, fileblock.ff_name);
    mklower(rbuf);
    if ( fileblock.ff_attrib == 16 )
        XSTRCAT(rbuf, DIRSEPSTR);

    return (rbuf);
}

/* GETNFILE:
 */
char *PASCAL NEAR getnfile P0_()
{
    char    fname[NFILEN];    /* file/path for DOS call */

    ZEROMEM(fname);

    /* and call for the first file */
    if ( findnext(&fileblock) == -1 )
        return (NULL);

    /* return the first file name! */
    XSTRCPY(rbuf, path);
    XSTRCAT(rbuf, fileblock.ff_name);
    mklower(rbuf);
    if ( fileblock.ff_attrib == 16 )
        XSTRCAT(rbuf, DIRSEPSTR);

    return (rbuf);
}
# else  /* !TURBO */

#  if     MSC | ZTC
/*  FILE Directory routines     */

static char path[NFILEN];   /* path of file to find */
static char rbuf[NFILEN];   /* return file buffer */

/* GETFFILE:
 *
 * Do a wild card directory search (for file name completion)
 */
char *PASCAL NEAR getffile P1_(char *, fspec)
/* fspec: Pattern to match  */
{
    REGISTER int  index   = 0;    /* index into various strings */
    REGISTER int  point   = 0;    /* index into other strings */
    REGISTER int  extflag = 0;    /* does the file have an extention? */
    char fname[NFILEN];           /* file/path for DOS call */

    ZEROMEM(fname);

    /* first parse the file path off the file spec */
    XSTRCPY(path, fspec);
    index = STRLEN(path) - 1;
    while ( index >= 0 &&
            (path[index] != '/' &&path[index] != '\\' && path[index] != ':') )
        --index;
    path[index+1] = 0;

    /* check for an extension */
    point = STRLEN(fspec) - 1;
    extflag = FALSE;
    while ( point > index ) {
        if ( fspec[point] == '.' ) {
            extflag = TRUE;
            break;
        }
        point--;
    }

    /* construct the composite wild card spec */
    XSTRCPY(fname, path);
    XSTRCAT(fname, &fspec[index+1]);
    XSTRCAT(fname, "*");
    if ( extflag == FALSE )
        XSTRCAT(fname, ".*");

    /* and call for the first file */
    if ( _dos_findfirst(fname, _A_NORMAL|_A_SUBDIR, &fileblock) != 0 )
        return (NULL);

    /* return the first file name! */
    XSTRCPY(rbuf, path);
    XSTRCAT(rbuf, fileblock.name);
    mklower(rbuf);
    if ( fileblock.attrib == 16 )
        XSTRCAT(rbuf, DIRSEPSTR);

    return (rbuf);
}

/* GETNFILE:
 */
char *PASCAL NEAR getnfile P0_()
{
    REGISTER int  index   = 0;      /* index into various strings */
    REGISTER int  point   = 0;      /* index into other strings */
    REGISTER int  extflag = 0;      /* does the file have an extention? */
    char          fname[NFILEN];    /* file/path for DOS call */

    ZEROMEM(fname);

    /* and call for the first file */
    if ( _dos_findnext(&fileblock) != 0 )
        return (NULL);

    /* return the first file name! */
    XSTRCPY(rbuf, path);
    XSTRCAT(rbuf, fileblock.name);
    mklower(rbuf);
    if ( fileblock.attrib == 16 )
        XSTRCAT(rbuf, DIRSEPSTR);

    return (rbuf);
}

#  else /* !(MSC | ZTC) */

/* GETFFILE:
 */
char *PASCAL NEAR getffile P1_(char *, fspec)
/* fspec: File to match */
{
    return (NULL);
}

/* GETNFILE:
 */
char *PASCAL NEAR getnfile P0_()
{
    return (NULL);
}

#  endif  /* MSC | ZTC  */

# endif /* TURBO  */

#endif  /* MSDOS  */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
