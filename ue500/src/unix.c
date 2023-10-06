/*======================================================================
 *      UNIX:   Unix specific terminal driver
 *              for MicroEMACS 4.0
 *      (C)Copyright 1995 D. Lawrence, C. Smith
 *
 *----------------------------------------------------------------------
 *
 *      New features: (as of version 3.10)
 *
 *      1. Timeouts waiting on a function key have been changed from
 *      35000 to 500000 microseconds.
 *
 *      2. Additional keymapping entries can be made from the command
 *      language by issuing a 'set $palette xxx'.  The format of
 *      xxx is a string as follows:
 *              "KEYMAP keybinding escape-sequence".
 *      To add "<ESC><[><A>" as a keybinding of FNN, issue:
 *              "KEYMAP FNN ~e[A".
 *      Note that the "~e" sequence represents the escape character in
 *      the MicroEMACS command language.
 *
 *      3. Colors are supported.  Under AIX the colors will be pulled
 *      in automaticly.  For other environments, you can either add
 *      the termcap entries, C0 to D7.  Or the colors may be defined
 *      using the command language by issuing a 'set $palette xxx'
 *      command.  The format of xxx is a string as follows:
 *              "CLRMAP # escape-sequence".
 *      The number is a number from 0 to 15, where 0 to 7 is the
 *      foreground colors, and 8 to 15 as background colors.
 *      To add foreground color 0 for ansi terminals, issue:
 *              "CLRMAP 0 ~e[30m".
 *
 *      'Porting notes:
 *
 *      I have tried to create this file so that it should work
 *      as well as possible without changes on your part.
 *
 *      However, if something does go wrong, read the following
 *      helpful hints:
 *
 *      1. On SUN-OS4, there is a problem trying to include both
 *      the termio.h and ioctl.h files.  I wish Sun would get their
 *      act together.  Even though you get lots of redefined messages,
 *      it shouldn't cause any problems with the final object.
 *
 *      2. In the type-ahead detection code, the individual UNIX
 *      system either has a FIONREAD or a FIORDCHK ioctl call.
 *      Hopefully, your system uses one of them and this be detected
 *      correctly without any intervention.
 *
 *      3. Also lookout for directory handling.  The SCO Xenix system
 *      is the weirdest I've seen, requiring a special load file
 *      (see below).  Some machine call the result of a readdir() call
 *      a "struct direct" or "struct dirent".  Includes files are
 *      named differently depending of the O/S.  If your system doesn't
 *      have an opendir()/closedir()/readdir() library call, then
 *      you should use the public domain utility "ndir".
 *
 *      To compile:
 *              Compile all files normally.
 *      To link:
 *              Select one of the following operating systems:
 *                      SCO Xenix:
 *                              use "-ltermcap" and "-lx";
 *                      SUN 3 and 4:
 *                              use "-ltermcap";
 *                      IBM-RT, IBM-AIX, ATT UNIX, Altos UNIX, Interactive:
 *                              use "-lcurses".
 *
 *      - 20 feb 95     New version 4.00 features
 *        We added new code to implient a TERMIOS driver
 *====================================================================*/

/*====================================================================*/
#define UNIX_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/** Include files **/
#include <stdio.h>              /* Standard I/O definitions */
#include <stdlib.h>             /* getenv()                 */
#include <time.h>               /* time(), ...              */
#include <errno.h>              /* errno, ...               */
#include <sys/stat.h>           /* stat(), ...              */
#include "estruct.h"            /* Emacs definitions        */
#include "eproto.h"             /* Function definitions     */


/*==============================================================*/
/* FEATURES                                                     */
/*==============================================================*/
/* Accordung to R. Stevens curses should contain some functions */
/* for terminal control, but obviously they aren't used here    */
/* correctly: It seems, that someone started it but didn't end. */
#define USE_CURSES              ( 0 )   /* NOT WORKING */
#if ( !IS_POSIX_UNIX() )
# define USE_TERMIO_IOCTL       ( 1 )
# define USE_TERMIOS_TCXX       ( 0 )
#else
# define USE_TERMIO_IOCTL       ( 0 )
# define USE_TERMIOS_TCXX       ( 1 )
#endif
/* Enable/disable XON/XOFF: We want to use ^S/^Q. I do not believe the flow
 * control settings of the OS should be diddled by an application program. But
 * if you do, change this 1 to a 0, but be warned, all sorts of terminals will
 * get grief with this
 */
#define USE_CTL_SQ              ( 0 )
/*==============================================================*/


/** Do nothing routine **/
int scnothing P1_(char *, s)
{
    return (0);
}

/** Only compile for UNIX machines **/
#if ( IS_UNIX() )

/** Include files **/
# include "edef.h"                      /* Global variable definitions  */
# include "elang.h"                     /* Language definitions     */

/** Kill predefined **/
# undef CTRF                            /* Problems with CTRF       */


/** Overall include files **/
# include <sys/types.h>                 /* System type definitions  */
# include <sys/stat.h>                  /* File status definitions  */
# include <sys/time.h>
# include <sys/param.h>
# include <sys/ioctl.h>                 /* I/O control definitions  */
# include <signal.h>                    /* Signal definitions       */
# include <unistd.h>

# if   ( USE_TERMIO_IOCTL )
#  include <termio.h>                   /* Terminal I/O definitions */
# elif ( USE_TERMIOS_TCXX )
#  include <termios.h>                  /* Terminal I/O definitions */
# elif ( USE_CURSES )
#  include <curses.h>                   /* Curses screen output     */
#  undef WINDOW                         /* Oh no!                   */
# else
#  error MISSING TERMINAL CONTROL DEFINITION
# endif

/** Completion include files **/
/** Directory accessing: Try and figure this out... if you can! **/
# if ( XENIX || VAT )
#  include <sys/ndir.h>                 /* Directory entry definitions  */
#  define DIRENTRY       direct
# else
#  include <dirent.h>                   /* Directory entry definitions  */
#  define DIRENTRY       dirent
# endif /* XENIX || VAT */

# if ( CYGWIN )
#  include <spawn.h>
#  include <sys/wait.h>
# endif /* CYGWIN */

/*==============================================================*/
/* Found in `curses.h':                                         */
/*==============================================================*/
# if ( !USE_CURSES )
#  if !ANSI
EXTERN int  tgetflag            DCL((char *id));
EXTERN int  tgetnum             DCL((char *id));
EXTERN int  tgetent             DCL((char *bp, const char *name));
EXTERN char *tgetstr            DCL((char *, char **));
EXTERN char *tgoto              DCL((CONST char *cap, int col, int row));
EXTERN int  tputs               DCL((CONST char *str, int affcnt, int (*putc)(int)));
#  endif  /* !ANSI */
# endif /* !USE_CURSES */
# if ANSI
EXTERN VOID PASCAL NEAR ttputs  DCL((CONST char *string));
# endif /* ANSI */
/*==============================================================*/

/*==============================================================*/
#define   TGETFLAG(x)     tgetflag((char *)(x))
#define   TGETNUM(x)      tgetnum((char *)(x))
# if VAT
#  define TGETSTR(a, b)   tgetstr( (char *)(a), *(b) )
# else
#  define TGETSTR(a, b)   tgetstr( (char *)(a), (b) )
# endif
# if CYGWIN
#  define NormalizeDirSep(path) do  {     \
    char  *cp_  = (path);                 \
                                          \
    while ( *cp_ )  {                     \
        if ( '\\' == *cp_ ) {             \
            *cp_ = '/';                   \
        }                                 \
        cp_++;                            \
    }                                     \
} while ( 0 )
# else
#  define  NormalizeDirSep(path)
# endif
/*==============================================================*/


/** Restore predefined definitions **/
# undef CTRF                            /* Restore CTRF               */
# define CTRF 0x0100

/** Parameters **/
# define NINCHAR         64             /* Input buffer size          */
# define NOUTCHAR        256            /* Output buffer size         */
# define NCAPBUF         1024           /* Termcap storage size       */
# define MARGIN          8              /* Margin size                */
# define SCRSIZ          64             /* Scroll for margin          */
# define NPAUSE          10           /* # times thru update to pause */

/** CONSTANTS **/
# define MLWAIT          3

/** Type definitions **/
struct capbind {                        /* Capability binding entry   */
    CONST char  *name;                  /* Termcap name               */
    char        *store;                 /* Storage variable           */
};
struct keybind {                        /* Keybinding entry           */
    CONST char  *name;                  /* Termcap name               */
    int         value;                  /* Binding value              */
};
# if ( !AIX )
char *reset = (char*) NULL;             /* reset string kjc           */
# endif

/** Local variables **/
# if ( USE_TERMIO_IOCTL )
static struct termio curterm;           /* Current modes              */
static struct termio oldterm;           /* Original modes             */
# elif ( USE_TERMIOS_TCXX )
static struct termios curterm;          /* Current modes              */
static struct termios oldterm;          /* Original modes             */
# elif ( USE_CURSES )
# else
#  error MISSING TERMINAL CONTROL DEFINITION
# endif
# if !ANSI
static char tcapbuf[NCAPBUF];           /* Termcap character storage  */
# endif /* !ANSI */
# define CAP_CL          0              /* Clear to end of page       */
# define CAP_CM          1              /* Cursor motion              */
# define CAP_CE          2              /* Clear to end of line       */
# define CAP_SE          3              /* Standout ends              */
# define CAP_SO          4              /* Standout (reverse video)   */
# define CAP_IS          5              /* Initialize screen          */
# define CAP_KS          6              /* Keypad mode starts         */
# define CAP_KE          7              /* Keypad mode ends           */
# define CAP_VB          8              /* Visible bell               */
# if COLOR
#  define CAP_C0          9             /* Foreground color #0        */
#  define CAP_C1          10            /* Foreground color #1        */
#  define CAP_C2          11            /* Foreground color #2        */
#  define CAP_C3          12            /* Foreground color #3        */
#  define CAP_C4          13            /* Foreground color #4        */
#  define CAP_C5          14            /* Foreground color #5        */
#  define CAP_C6          15            /* Foreground color #6        */
#  define CAP_C7          16            /* Foreground color #7        */
#  define CAP_D0          17            /* Background color #0        */
#  define CAP_D1          18            /* Background color #1        */
#  define CAP_D2          19            /* Background color #2        */
#  define CAP_D3          20            /* Background color #3        */
#  define CAP_D4          21            /* Background color #4        */
#  define CAP_D5          22            /* Background color #5        */
#  define CAP_D6          23            /* Background color #6        */
#  define CAP_D7          24            /* Background color #7        */
#  if ( USG || AIX || AUX )
#   define CAP_SF          25           /* Set foreground color       */
#   define CAP_SB          26           /* Set background color       */
#  endif /* USG || AIX || AUX */
# endif /* COLOR */
# if !ANSI
static struct capbind capbind[] =       /* Capability binding list    */
{
    { "cl" },                           /* Clear to end of page       */
    { "cm" },                           /* Cursor motion              */
    { "ce" },                           /* Clear to end of line       */
    { "se" },                           /* Standout ends              */
    { "so" },                           /* Standout (reverse video)   */
    { "is" },                           /* Initialize screen          */
    { "ks" },                           /* Keypad mode starts         */
    { "ke" },                           /* Keypad mode ends           */
    { "vb" },                           /* Visible bell               */
# if COLOR
    { "c0" },                           /* Foreground color #0        */
    { "c1" },                           /* Foreground color #1        */
    { "c2" },                           /* Foreground color #2        */
    { "c3" },                           /* Foreground color #3        */
    { "c4" },                           /* Foreground color #4        */
    { "c5" },                           /* Foreground color #5        */
    { "c6" },                           /* Foreground color #6        */
    { "c7" },                           /* Foreground color #7        */
    { "d0" },                           /* Background color #0        */
    { "d1" },                           /* Background color #1        */
    { "d2" },                           /* Background color #2        */
    { "d3" },                           /* Background color #3        */
    { "d4" },                           /* Background color #4        */
    { "d5" },                           /* Background color #5        */
    { "d6" },                           /* Background color #6        */
    { "d7" },                           /* Background color #7        */
#  if ( USG || AIX || AUX )
    { "Sf" },                           /* Set foreground color       */
    { "Sb" },                           /* Set background color       */
#  endif /* USG || AIX || AUX */
# endif /* COLOR */
};

# if COLOR
static int cfcolor = -1;                /* Current forground color    */
static int cbcolor = -1;                /* Current background color   */
# endif /* COLOR */

static struct keybind keybind[] =       /* Keybinding list            */
{
    { "bt", SHFT|CTRF|'i' },            /* Back-tab key               */
    { "k1", SPEC|'1' },                 /* F1 key                     */
    { "k2", SPEC|'2' },                 /* F2 key                     */
    { "k3", SPEC|'3' },                 /* F3 key                     */
    { "k4", SPEC|'4' },                 /* F4 key                     */
    { "k5", SPEC|'5' },                 /* F5 key                     */
    { "k6", SPEC|'6' },                 /* F6 key                     */
    { "k7", SPEC|'7' },                 /* F7 key                     */
    { "k8", SPEC|'8' },                 /* F8 key                     */
    { "k9", SPEC|'9' },                 /* F9 key                     */
    { "k0", SPEC|'0' },                 /* F0 or F10 key              */
    { "k;", SPEC|'0' },                 /* F0 or F10 key    (kjc)     */
    { "F1", SHFT|SPEC|'1' },            /* Shift-F1 or F11 key        */
    { "F2", SHFT|SPEC|'2' },            /* Shift-F2 or F12 key        */
    { "F3", SHFT|SPEC|'3' },            /* Shift-F3 or F13 key        */
    { "F4", SHFT|SPEC|'4' },            /* Shift-F4 or F14 key        */
    { "F5", SHFT|SPEC|'5' },            /* Shift-F5 or F15 key        */
    { "F6", SHFT|SPEC|'6' },            /* Shift-F6 or F16 key        */
    { "F7", SHFT|SPEC|'7' },            /* Shift-F7 or F17 key        */
    { "F8", SHFT|SPEC|'8' },            /* Shift-F8 or F18 key        */
    { "F9", SHFT|SPEC|'9' },            /* Shift-F9 or F19 key        */
    { "FA", SHFT|SPEC|'0' },            /* Shift-F0 or F20 key        */
    { "kA", CTRF|'O' },                 /* Insert line key            */
    { "kb", CTRF|'H' },                 /* Backspace key              */
    { "kC", CTRF|'L' },                 /* Clear screen key           */
    { "kD", SPEC|'D' },                 /* Delete character key       */
    { "kd", SPEC|'N' },                 /* Down arrow key             */
    { "kE", CTRF|'K' },                 /* Clear to end of line key   */
    { "kF", CTRF|'V' },                 /* Scroll forward key         */
    { "kH", SPEC|'>' },                 /* Home down key              */
    { "@7", SPEC|'>' },                 /* Home down key    (kjc)     */
    { "kh", SPEC|'<' },                 /* Home key                   */
    { "kI", SPEC|'C' },                 /* Insert character key       */
    { "kL", CTRF|'K' },                 /* Delete line key            */
    { "kl", SPEC|'B' },                 /* Left arrow key             */
    { "kN", SPEC|'V' },                 /* Next page key              */
    { "kP", SPEC|'Z' },                 /* Previous page key          */
    { "kR", CTRF|'Z' },                 /* Scroll backward key        */
    { "kr", SPEC|'F' },                 /* Right arrow key            */
    { "ku", SPEC|'P' },                 /* Up arrow key               */
    { "K1", SPEC|'<' },                 /* Keypad 7 -> Home           */
    { "K2", SPEC|'V' },                 /* Keypad 9 -> Page Up        */
    { "K3", ' ' },                      /* Keypad 5                   */
    { "K4", SPEC|'>' },                 /* Keypad 1 -> End            */
    { "K5", CTRF|'V' },                 /* Keypad 3 -> Page Down      */
    { "kw", CTRF|'E' }                  /* End of line                */
};
# endif /* !ANSI */
static int inbuf[NINCHAR];              /* Input buffer               */
static int * inbufh = inbuf;            /* Head of input buffer       */
static int * inbuft = inbuf;            /* Tail of input buffer       */
#define PRINT_inbuf(where) do  {                                  \
    int i = 0;                                                    \
                                                                  \
    fprintf(stderr, "%12s: inbuft = %d, inbufh = %d, inbuf = ",   \
                    (char *)(where), (int)(inbuft - inbuf),       \
                                     (int)(inbufh - inbuf));      \
    for ( i = 0; i < NELEM(inbuf) - 1; i++ )  {                   \
        fprintf(stderr, "0x%04X, ", inbuf[i]);                    \
    }                                                             \
    fprintf(stderr, "0x%04X\n", inbuf[i]);                        \
} while ( 0 )
static unsigned char outbuf[NOUTCHAR];  /* Output buffer              */
static unsigned char * outbuft = outbuf;/* Output buffer tail         */


static DIR *dirptr = NULL;              /* Current directory stream   */
static char path[NFILEN];               /* Path of file to find       */
static char rbuf[NFILEN];               /* Return file buffer         */
static char *nameptr;                 /* Ptr past end of path in rbuf */

/** Terminal definition block **/
# if !ANSI
static int scmove   DCL((int, int));
static int scbeep   DCL((void));
static int sckclose DCL((void));
static int sckopen  DCL((void));
static int scopen   DCL((void));
static int scclose  DCL((void));
static int sceeol   DCL((void));
static int sceeop   DCL((void));
static int screv    DCL((int));
# if COLOR
static int scfcol   DCL((int));
static int scbcol   DCL((int));
# endif /* COLOR */
# endif /* ANSI */

# if ( FLABEL )
static VOID dis_sfk DCL((void));
static VOID dis_ufk DCL((void));
# endif

# if  ANSI
COMMON TERM term;
# else
TERM term =
{
    120,                        /* Maximum number of rows             */
    0,                          /* Current number of rows             */
    132,                        /* Maximum number of columns          */
    0,                          /* Current number of columns          */
    0, 0,                       /* upper left corner default screen   */
    MARGIN,                     /* Margin for extending lines         */
    SCRSIZ,                     /* Scroll size for extending          */
    NPAUSE,                     /* # times thru update to pause       */
    scopen,                     /* Open terminal routine              */
    scclose,                    /* Close terminal routine             */
    sckopen,                    /* Open keyboard routine              */
    sckclose,                   /* Close keyboard routine             */
    ttgetc,                     /* Get character routine              */
    ttputc,                     /* Put character routine              */
    ttflush,                    /* Flush output routine               */
    scmove,                     /* Move cursor routine                */
    sceeol,                     /* Erase to end of line routine       */
    sceeop,                     /* Erase to end of page routine       */
    sceeop,                     /* Clear the desktop                  */
    scbeep,                     /* Beep! routine                      */
    screv,                      /* Set reverse video routine          */
    scnothing,                  /* Set resolution routine             */
# if COLOR
    scfcol,                     /* Set forground color routine        */
    scbcol,                     /* Set background color routine       */
# endif /* COLOR */
# if     INSDEL
    scinsline,                  /* insert a screen line               */
    scdelline,                  /* delete a screen line               */
# endif /* INSDEL */
};
# endif /* ANSI */

# if !ANSI
static int hpterm =0;           /* global flag braindead HP-terminal  */
# endif


/** Open terminal device **/
int ttopen P0_()
{
    XSTRCPY(os, "UNIX");
# if ( USE_TERMIO_IOCTL )

#  if SMOS
    /* Extended settings; 890619mhs A3 */
    set_parm(0, -1, -1);
#  endif /* SMOS */

    /* Get modes */
    if ( ioctl(0, TCGETA, &oldterm) ) {
        perror("Cannot TCGETA");

        return (-1);
    }

    /* Save to original mode variable */
    curterm = oldterm;

    /* Set new modes */
#  if ( USE_CTL_SQ )
    curterm.c_iflag &= ~(INLCR|ICRNL|IGNCR);
#  else
    curterm.c_iflag &= ~(INLCR|ICRNL|IGNCR|IXON|IXANY|IXOFF);
#  endif
    curterm.c_iflag &= ~(INLCR|ICRNL|IGNCR|IXON|IXANY|IXOFF);
    curterm.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN);
    curterm.c_cc[VMIN] = 1;
    curterm.c_cc[VTIME] = 0;

#  if SMOS
    /****THIS IS A BIG GUESS ON MY PART... the code changed too much between
     * versions for me to be sure this will work - DML */

    /* Allow multiple (dual) sessions if already enabled */
    curterm.c_lflag = oldterm.c_lflag & ISIG;

    /* Use old SWTCH char if necessary */
    if ( curterm.c_lflag != 0 )
        curterm.c_cc[VSWTCH] = oldterm.c_cc[VSWTCH];

    /* Copy VTI settings    */
    curterm.c_cc[VTBIT] = oldterm.c_cc[VTBIT];

    /* Extended settings; 890619mhs A3 */
    set_parm(0, -1, -1);
#  endif /* SMOS */

    /* Set tty mode */
    if ( ioctl(0, TCSETA, &curterm) ) {
        perror("Cannot TCSETA");

        return (-1);
    }
# elif ( USE_TERMIOS_TCXX )
    /* Get modes */
    if ( tcgetattr(0, &oldterm) ) {
        perror("Cannot tcgetattr");

        return (-1);
    }

    /* Save to original mode variable */
    curterm = oldterm;

    /* Set new modes */
#  if ( USE_CTL_SQ )
    curterm.c_iflag &= ~(INLCR|ICRNL|IGNCR);
#  else
    curterm.c_iflag &= ~(INLCR|ICRNL|IGNCR|IXON|IXANY|IXOFF);
#  endif
    curterm.c_iflag &= ~(INLCR|ICRNL|IGNCR|IXON|IXANY|IXOFF);
    curterm.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN);
    curterm.c_cc[VMIN] = 1;
    curterm.c_cc[VTIME] = 0;
#  ifdef  VLNEXT
    curterm.c_cc[VLNEXT] = -1;
#  endif

    /* Set tty mode */
    if ( tcsetattr(0, TCSANOW, &curterm) ) {
        perror("Cannot tcsetattr");

        return (-1);
    }
# elif ( USE_CURSES )
    /* ? */
# else
#  error MISSING TERMINAL CONTROL DEFINITION
# endif

    /* Success */
    return (0);
}

/** Close terminal device **/
int ttclose P0_()
{
# if ( !AIX  )
    /* Restore original terminal modes */
    if ( reset != (char*)NULL )
        write( 1, reset, STRLEN(reset) );
# endif

# if ( USE_TERMIO_IOCTL )
#  if SMOS
    /* Extended settings; 890619mhs A3 */
    set_parm(0, -1, -1);
#  endif /* SMOS */
    if ( ioctl(0, TCSETA, &oldterm) )
        return (-1);

# elif ( USE_TERMIOS_TCXX )
    /* Set tty mode */
    if ( tcsetattr(0, TCSANOW, &oldterm) ) {
        perror("Cannot tcsetattr");

        return (-1);
    }
# elif ( USE_CURSES )
    /* ? */
# else
#  error MISSING TERMINAL CONTROL DEFINITION
# endif

    /* Success */
    return (0);
}

/** Flush output buffer to display **/
int ttflush P0_()
{
# if ( !USE_CURSES )
    int len;

    /* Compute length of write */
    len = outbuft - outbuf;
    if ( len == 0 )
        return (0);

    /* Reset buffer position */
    outbuft = outbuf;

    /* Perform write to screen */
    return (write(1, outbuf, len) != len);

# else
    refresh();

    return (0);

# endif /* USE_CURSES */
}

/** Put character onto display **/
int ttputc P1_(int, ch)
/* ch:  Character to display                                        */
/*      int will be converted to unsigned char like e.g. in fputc() */
{
# if ( !USE_CURSES )
    /* Check for buffer full */
    if ( outbuft == &outbuf[SIZEOF(outbuf)] )
        ttflush();

    /* Add to buffer */
    *outbuft++ = ch;
# else
    /* Put character on screen */
    addch(ch);
# endif /* USE_CURSES */

    return (0);
}

/** Grab input characters, with wait **/
unsigned char grabwait()
{
    unsigned char ch  = '\0';

    /* Change mode, if necessary */
    if ( curterm.c_cc[VTIME] ) {
        curterm.c_cc[VMIN] = 1;
        curterm.c_cc[VTIME] = 0;
# if   ( USE_TERMIOS_TCXX )
        tcsetattr(0, TCSANOW, &curterm);
# elif ( USE_TERMIO_IOCTL )
        ioctl(0, TCSETA, &curterm);
# elif ( USE_CURSES )
        /* ? */
# else
#  error MISSING TERMINAL CONTROL DEFINITION
# endif
    }

    /* Perform read */
# if HANDLE_WINCH
    while ( read(0, &ch, 1) != 1 ) {
        if ( winch_flag )
            return 0;
    }
# else
    if ( read(0, &ch, 1) != 1 ) {
        puts("** Horrible read error occured **");
        exit(1);
    }
# endif

    /* Return new character */
# if ( 0 )
    TRC(("grabwait(): 0x%02X, <%c>", (unsigned int)(ch), (char)ch));
# endif
    return (ch);
}

/** Grab input characters, short wait **/
unsigned char PASCAL NEAR grabnowait P0_()
{
    int           count = 0;
    unsigned char ch    = '\0';

    /* Change mode, if necessary */
    if ( curterm.c_cc[VTIME] == 0 ) {
        curterm.c_cc[VMIN] = 0;
        curterm.c_cc[VTIME] = UNIX_READ_TOUT;
# if   ( USE_TERMIOS_TCXX )
        tcsetattr(0, TCSANOW, &curterm);
# elif ( USE_TERMIO_IOCTL )
        ioctl(0, TCSETA, &curterm);
# elif ( USE_CURSES )
        /* ? */
# else
#  error MISSING TERMINAL CONTROL DEFINITION
# endif
    }

    /* Perform read */
# if HANDLE_WINCH
    while ( ( count = read(0, &ch, 1) ) < 0 ) {
        if ( winch_flag )
            return 0;
    }
# else
    count = read(0, &ch, 1);
    if ( count < 0 ) {
        puts("** Horrible read error occured **");
        exit(1);
    }
# endif
    if ( count == 0 ) {
        return (grabnowait_TIMEOUT);
    }

    /* Return new character */
# if ( 0 )
    TRC(("grabnowait(): 0x%02X, <%c>", (unsigned int)(ch), (char)ch));
# endif
    return (ch);
}

/* QIN:
 *
 * Queue in a character to the input buffer.
 */
VOID qin P1_(int, ch)
{
# if ( 0 )
    PRINT_inbuf("BEGIN qin");
# endif
    /* Check for overflow */
    if ( inbuft == &inbuf[NELEM(inbuf)] ) {
        /* Annoy user */
        term.t_beep();

        return;
    }

    /* Add character */
    *inbuft++ = ch;
# if ( 0 )
    PRINT_inbuf("  END qin");
# endif
}

/* QREP:
 *
 * Replace a key sequence with a single character in the input buffer.
 */
VOID qrep P1_(int, ch)
{
# if ( 0 )
    PRINT_inbuf("BEGIN qrep");
# endif
    inbuft = inbuf;
    qin(ch);
# if ( 0 )
    PRINT_inbuf("  END qrep");
# endif
}

/** Return cooked characters **/
int PASCAL NEAR ttgetc P0_()
{
    int ch  = 0;

    ttflush();
    /* Loop until character is in input buffer */
    while ( inbufh == inbuft )
        cook();

    /* Get input from buffer, now that it is available */
    ch = *inbufh++;

    /* reset us to the beginning of the buffer if there are no more pending
     * characters */
    if ( inbufh == inbuft )
        inbufh = inbuft = inbuf;

    /* Return next character */
# if ( 0 )
    TRC(("ttgetc(): 0x%04X", (unsigned int)ch));
# endif
    return (ch);
}

int PASCAL NEAR ttgetc_nowait P0_()
{
    int ch  = 0;

    ttflush();
    /* Loop until character is in input buffer */
    while ( inbufh == inbuft )  {
        if ( !cook_nowait() ) {
            return grabnowait_TIMEOUT;
        }
    }

    /* Get input from buffer, now that it is available */
    ch = *inbufh++;

    /* reset us to the beginning of the buffer if there are no more pending
     * characters */
    if ( inbufh == inbuft )
        inbufh = inbuft = inbuf;

    /* Return next character */
# if ( 0 )
    TRC(("ttgetc_nowait(): 0x%04X", (unsigned int)ch));
# endif
    return (ch);
}

# if TYPEAH

int typahead P0_()
{
#   if ( defined(FIONREAD) || ( !VAT && defined(FIORDCHK) ) )
    int count = 0;
#   endif

    /* See if internal buffer is non-empty */
    if ( inbufh != inbuft )
        return (1);

    /* Now check with system */
#  ifdef FIONREAD /* Watch out!  This could bite you! */
    /* Get number of pending characters */
    if ( ioctl(0, FIONREAD, &count) )
        return (0);

    return (count);

#  else /* not FIONREAD */
#   if VAT

    return (0);

#   else /* not VAT */
#    ifdef FIORDCHK
    /* Ask hardware for count */
    count = ioctl(0, FIORDCHK, 0);
    if ( count < 0 )
        return (0);

    return (count);

#    else

    return (0);

#    endif /* FIORDCHK */
#   endif /* VAT */
#  endif /* FIONREAD */
}

# endif /* TYPEAH */

/** Put out sequence, with padding **/
VOID putpad P1_(char *, seq)
/* seq: Character sequence  */
{
    /* Check for null */
    if ( !seq )
        return;

    /* Call on termcap to send sequence */
# if ANSI
    ttputs(seq);
    TRC( ("ttputs(%s)", seq) );
# else
    tputs(seq, 1, ttputc);
    TRC( ("tputs(%s, 1, ttputc)", seq) );
# endif /* ANSI */
}

# if !ANSI
/** Initialize screen package **/
int scopen P0_()
{
    char * cp, tcbuf[1024];
    int status;
    struct capbind * cb;
    struct keybind * kp;
    char err_str[NSTRING];

#  if ( HPUX8 || HPUX9 || VAT || AUX || AIX5 )
    /* HP-UX, AUX and AIX5 doesn't seem to have these in the
     * termcap library  */
    char PC, * UP;
    short ospeed;
#  else /* not HPUX8 || HPUX9 || VAT || AUX */
    COMMON char   PC;
    COMMON char   *UP;
    COMMON short  ospeed;
#  endif /* HPUX8 || HPUX9 || VAT || AUX */

    /* Get terminal type */
    cp = getenv("TERM");
    if ( !cp ) {
        puts(TEXT182);
/*      "Environment variable \"TERM\" not defined!" */
        exit(1);
    }

    /* Try to load termcap */
    status = tgetent(tcbuf, cp);
    if ( status == -1 ) {
        puts("Cannot open termcap file");
        exit(1);
    }
    if ( status == 0 ) {
        sprintf(err_str, TEXT183, cp);
/*      "No entry for terminal type \"%s\"\n" */
        puts(err_str);
        exit(1);
    }

    /* Get size from termcap */
    term.t_nrow = TGETNUM("li") - 1;
    term.t_ncol = TGETNUM("co");
    if ( term.t_nrow < 3 || term.t_ncol < 3 ) {
        puts("Screen size is too small!");
        exit(1);
    }

    /* initialize max number of rows and cols    */
    term.t_mrow = term.t_nrow;
    term.t_mcol = term.t_ncol;

    /* Start grabbing termcap commands */
    cp = tcapbuf;

    /* Get the reset string */
    reset = TGETSTR("is", &cp);

    /* Get the pad character */
    if ( TGETSTR("pc", &cp) )
        PC = tcapbuf[0];

    /* Get up line capability */
    UP = TGETSTR("up", &cp);

    /* Get other capabilities */
    cb = capbind;
    while ( cb < &capbind[SIZEOF (capbind)/SIZEOF (*capbind)] ) {
        cb->store = TGETSTR(cb->name, &cp);
        cb++;
    }

    /* Check for minimum */
    if ( !capbind[CAP_CL].store && (!capbind[CAP_CM].store || !UP) ) {
        puts("This terminal doesn't have enough power to run microEmacs!");
        exit(1);
    }

    /* Set reverse video and erase to end of line */
    if ( capbind[CAP_SO].store && capbind[CAP_SE].store )
        revexist = TRUE;
    if ( !capbind[CAP_CE].store )
        eolexist = FALSE;

    /* Get keybindings */
    kp = keybind;
    while ( kp < &keybind[SIZEOF (keybind)/SIZEOF (*keybind)] ) {
        addkey((unsigned char *)TGETSTR(kp->name, &cp), kp->value);
        kp++;
    }

    /* check for HP-Terminal (so we can label its function keys) */
    hpterm = TGETFLAG("xs");

    /* Open terminal device */
    if ( ttopen() ) {
        puts("Cannot open terminal");
        exit(1);
    }

    /* Set speed for padding sequences */
#  if   ( USE_TERMIOS_TCXX )
    ospeed = cfgetospeed(&curterm);
#  elif ( USE_TERMIO_IOCTL )
    ospeed = curterm.c_cflag & CBAUD;
#  elif ( USE_CURSES )
    /* ? */
#  else
#   error MISSING TERMINAL CONTROL DEFINITION
#  endif

    /* Send out initialization sequences */
#  if ( !AIX )
    putpad(capbind[CAP_IS].store);
#  endif
    putpad(capbind[CAP_KS].store);
    sckopen();

#  if ( USE_CURSES )
    /* Initialize screen */
    initscr();

    /* Set size of screen */
    term.t_nrow = LINES - 1;
    term.t_ncol = COLS;

    /* Open terminal device */
    if ( ttopen() ) {
        puts("Cannot open terminal");
        exit(1);
    }
#  endif /* USE_CURSES */

    /* Success */
    return ( 0 );
}

/** Close screen package **/
int scclose P0_()
{
    /* Turn off keypad mode */
    putpad(capbind[CAP_KE].store);
    sckclose();

#  if ( USE_CURSES )
    /* Turn off curses */
    endwin();
#  endif /* USE_CURSES */
    /* Close terminal device */
    ttflush();
    ttclose();

    /* Success */
    return ( 0 );
}

/* open keyboard -hm */
int sckopen P0_()
{
    putpad(capbind[CAP_KS].store);
    ttflush();
#  if     FLABEL
    dis_ufk();
#  endif

    return ( 0 );
}

/* close keyboard -hm */
int sckclose P0_()
{
    putpad(capbind[CAP_KE].store);
    ttflush();
#  if     FLABEL
    dis_sfk();
#  endif

    return ( 0 );
}

/** Move cursor **/
int scmove P2_(int, row, int, col)
/* row: Row number    */
/* col: Column number */
{
    /* Call on termcap to create move sequence */
    putpad( tgoto(capbind[CAP_CM].store, col, row) );

#  if ( USE_CURSES )
    move(row, col);
#  endif /* USE_CURSES */

    /* Success */
    return (0);
}

/** Erase to end of line **/
int sceeol P0_()
{
    /* Send erase sequence */
    putpad(capbind[CAP_CE].store);

#  if ( USE_CURSES )
    clrtoeol();
#  endif /* USE_CURSES */

    /* Success */
    return (0);
}

/** Clear screen **/
int sceeop P0_()
{
#  if COLOR
    scfcol(gfcolor);
    scbcol(gbcolor);
#  endif /* COLOR */
    /* Send clear sequence */
    putpad(capbind[CAP_CL].store);

#  if ( USE_CURSES )
    erase();
#  endif /* USE_CURSES */


    /* Success */
    return (0);
}

/** Set reverse video state **/
int screv P1_(int, state)
/* state: New state */
{
#  if COLOR
    int ftmp, btmp;             /* temporaries for colors */
#  endif /* COLOR */

    /* Set reverse video state */
    putpad(state ? capbind[CAP_SO].store : capbind[CAP_SE].store);

#  if COLOR
    if ( state == FALSE ) {
        ftmp = cfcolor;
        btmp = cbcolor;
        cfcolor = -1;
        cbcolor = -1;
        scfcol(ftmp);
        scbcol(btmp);
    }
#  endif /* COLOR */

#  if ( USE_CURSES )
    if ( state )
        standout();
    else
        standend();
#  endif /* USE_CURSES */

    /* Success */
    return (0);
}

/** Beep **/
int scbeep P0_()
{
#  if !NOISY
    /* Send out visible bell, if it exists */
    if ( capbind[CAP_VB].store )
        putpad(capbind[CAP_VB].store);
    else
#  endif /* not NOISY */
    /* The old standby method */
    ttputc('\7');

#  if ( USE_CURSES )
    addch('\7');                /* FIX THIS! beep() and flash comes up undefined
                                 */
#  endif /* USE_CURSES */

    /* Success */
    return (0);
}

#  if COLOR
#   if USG || AUX
static char cmap[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
#   endif /* USG || AUX */

/** Set foreground color **/
int scfcol P1_(int, color)
/* color: Color to set  */
{
    TRC( ("scfcol(%d)", (int) color) );
    /* Skip if already the correct color */
    if ( color == cfcolor )
        return (0);

    TRC( ("scfcol(): %s", "color != cfcolor") );

    /* Send out color sequence */
    TRC( ( "scfcol(): capbind[CAP_C0].store == 0x%lX",
           (unsigned long int)(capbind[CAP_C0].store) ) );
    if ( capbind[CAP_C0].store ) {
        TRC( ( "scfcol(): capbind[CAP_C0 + (color & 7)].store = %s",
               STR(capbind[CAP_C0 + (color & 7)].store) ) );
        putpad(capbind[CAP_C0 + (color & 7)].store);
        cfcolor = color;
    }
#   if USG || AUX
    else if ( capbind[CAP_SF].store ) {
        putpad( tparm(capbind[CAP_SF].store, cmap[color & 7]) );
        cfcolor = color;
    }
#   endif /* USG || AUX */

#   if ( USE_CURSES )
    /* ? */
#   endif /* USE_CURSES */

    return (0);
}

/** Set background color **/
int scbcol P1_(int, color)
/* color: Color to set  */
{
    TRC( ("scbcol(%d)", (int) color) );
    /* Skip if already the correct color */
    if ( color == cbcolor )
        return (0);

    TRC( ("scbcol(): %s", "color != cbcolor") );

    /* Send out color sequence */
    TRC( ( "scbcol(): capbind[CAP_C0].store == 0x%lX",
           (unsigned long int)(capbind[CAP_C0].store) ) );
    if ( capbind[CAP_C0].store ) {
        TRC( ( "scbcol(): capbind[CAP_D0 + (color & 7)].store = %s",
               STR(capbind[CAP_D0 + (color & 7)].store) ) );
        putpad(capbind[CAP_D0 + (color & 7)].store);
        cbcolor = color;
    }
#   if USG || AUX
    else if ( capbind[CAP_SB].store ) {
        putpad( tparm(capbind[CAP_SB].store, cmap[color & 7]) );
        cbcolor = color;
    }
#   endif /* USG || AUX */

#   if ( USE_CURSES )
    /* ? */
#   endif /* USE_CURSES */

    return (0);
}
#  endif /* COLOR */

/** Set palette **/
int PASCAL NEAR spal P1_(char *, cmd)
/* cmd: Palette command */
{
    int   code      = 0;
    int   dokeymap  = 0;
#  if COLOR
    int   doclrmap  = 0;
#  endif /* COLOR */
    char  *cp       = NULL;

    /* Check for keymapping command */
    if        ( strncmp(cmd, "KEYMAP ", 7) == 0 ) {
        dokeymap = 1;
#  if COLOR
    } else if ( strncmp(cmd, "CLRMAP ", 7) == 0 ) {
            doclrmap = 1;
#  endif /* COLOR */
    } else                                        {
        return (0);
    }

    cmd += 7;

    /* Look for space */
    for ( cp = cmd; *cp == ' '; cp++ );
    for (; *cp != '\0'; cp++ )
        if ( *cp == ' ' ) {
            *cp++ = '\0';
            break;
        }
    if ( *cp == '\0' )
        return (1);

    for (; *cp == ' '; cp++ );

    /* Perform operation */
    if        ( dokeymap )  {
        /* Convert to keycode */
        code = stock(cmd);

        /* Add to tree */
        addkey((unsigned char *)cp, code);
#  if COLOR
    } else if ( doclrmap )  {
        /* Convert to color number */
        code = atoi(cmd);
        if ( code < 0 || code > 15 )
            return (1);

        /* Move color code to capability structure */
        capbind[CAP_C0 + code].store = ROOM(STRLEN(cp) + 1);
        if ( capbind[CAP_C0 + code].store ) {
            XSTRCPY(capbind[CAP_C0 + code].store, cp);
            TRC( ( "capbind[CAP_C0 + %d].store = %s", (int)code,
                  STR(capbind[CAP_C0 + code].store) ) );
        }
#  endif /* COLOR */
    } else                  {
        /**EMPTY**/
    }

    return (0);
}
# endif /* !ANSI */

/* Surely more than just BSD systems do this: */

/** Perform a stop signal **/
int bktoshell P2_(int, f, int, n)
{
    /* Reset the terminal and go to the last line */
    vttidy();

    /* Okay, stop... */
    kill(getpid(), SIGTSTP);

    /* We should now be back here after resuming */

    /* Reopen the screen and redraw */
    term.t_open();
    curwp->w_flag = WFHARD;
    sgarbf = TRUE;

    /* Success */
    return (0);
}

/** Get time of day **/
char * timeset P0_()
{
    time_t buf;
    char * sp, * cp;

    /* Get system time */
    time(&buf);

    /* Pass system time to converter */
    sp = ctime(&buf);

    /* Eat newline character */
    for ( cp = sp; *cp; cp++ )
        if ( *cp == '\n' ) {
            *cp = '\0';
            break;
        }

    return (sp);
}

/*====================================================================*/
/* Only to keep this very old code for documentation purposes:        */
/*====================================================================*/
# if ( 0 )
/** Rename a file **/
int rename P2_(char *, file1, char *, file2)
/* file1: Old file name */
/* file2: New file name */
{
    struct stat buf1;
    struct stat buf2;

    /* No good if source file doesn't exist */
    if ( stat(file1, &buf1) )
        return (-1);

    /* Check for target */
    if ( stat(file2, &buf2) == 0 ) {

        /* See if file is the same */
        if ( buf1.st_dev == buf2.st_dev &&buf1.st_ino == buf2.st_ino )

            /* Not necessary to rename file */
            return (0);
    }

    /* Get rid of target */
    unlink(file2);

    /* Link two files together */
    if ( link(file1, file2) )
        return (-1);

    /* Unlink original file */
    return ( unlink(file1) );
}
# endif
/*====================================================================*/

# if CYGWIN
CONST char *wingetshell P0_()
{
    static CONST char *res  = NULL;

    if ( NULL == res )  {
        char        SystemRoot[NFILEN];
        static char WinCmd[NFILEN];

        ZEROMEM(SystemRoot);
        ZEROMEM(WinCmd);

        if ( 0 == xstrlcpy(SystemRoot, getenv("SYSTEMROOT"), SIZEOF(SystemRoot)) )  {
            xstrlcpy(SystemRoot, "C:/WINDOWS", SIZEOF(SystemRoot));
        }
        NormalizeDirSep(SystemRoot);
        xsnprintf(WinCmd, SIZEOF(WinCmd), "%s/system32/cmd.exe", SystemRoot);
        res = WinCmd;
    }

    return res;
}

#define SHELL_C_  "/C"
static int winsystem P1_(CONST char *, cmd)
{
    pid_t               child_pid = 0;
    int                 status    = 0;
    int                 errno_sv  = 0;
    CONST char          *shell    = NULL;
    sigset_t            mask;
    posix_spawnattr_t   attr;
    char  * /**CONST**/ sargv[] = { NULL, (char *)SHELL_C_, (char *)cmd, NULL };

    extern char **environ;

    ZEROMEM(mask);
    ZEROMEM(attr);

    ASRT( NULL != (shell = wingetshell()) );
    sargv[0]  = (char *)shell;

    if ( NULL == cmd )  {
        cmd = "";
    }

    /* Create an attributes object and add a "set signal mask"
       action to it. */

    if ( 0 != posix_spawnattr_init(&attr) ) {
        return ( -C_20 );
    }
    if ( 0 != posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGMASK) ) {
        return ( -C_30 );
    }
    sigfillset(&mask);
    if ( 0 != posix_spawnattr_setsigmask(&attr, &mask) )  {
        return ( -C_40 );
    }

    if ( 0 != posix_spawnp(&child_pid, shell, NULL, &attr, sargv, environ) )  {
        errno_sv  = errno;
        TRC(("Error executing <%s>, errno = %d: %s", shell, errno_sv, strerror(errno_sv)));
    }

    /* Destroy any objects that we created earlier. */
    if ( 0 != posix_spawnattr_destroy(&attr) )  {
        /**EMPTY**/
    }

    if ( 0 != errno_sv )  {
        return ( (-1) * errno_sv );
    }

    /* Monitor status of the child until it terminates. */
    do {
        if ( 0 > waitpid(child_pid, &status, WUNTRACED | WCONTINUED) )  {
            return ( -1 );
        }

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            return WTERMSIG(status);
        } else if (WIFSTOPPED(status)) {
            /**EMPTY**/
        } else if (WIFCONTINUED(status)) {
            /**EMPTY**/
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    return WEXITSTATUS ( status );
}
#  undef SHELL_C_
# endif

/** Callout to system to perform command **/
int callout P1_(CONST char *, cmd)
/* cmd: Command to execute  */
{
    int status  = 0;

    if ( NULL == cmd )  {
        return 0;
    }
    /* Close down */
    term.t_move(term.t_nrow, 0);
    ttflush();
    term.t_kclose();
    ttclose();

    /* Do command */
# if ( CYGWIN )
    status = winsystem(cmd) == 0;
# else
    status = system(cmd) == 0;
# endif

    /* Restart system */
    sgarbf = TRUE;
    term.t_kopen();
    if ( ttopen() ) {
        puts("** Error reopening terminal device **");
        exit(1);
    }

    /* Success */
    return (status);
}

/** Create subshell **/
int spawncli P2_(int, f, int, n)
/* f: Flags           */
/* n: Argument count  */
{
    CONST char  *sh = NULL;

    /* Don't allow this command if restricted */
    if ( restflag )
        return ( resterr() );

    /* Get shell path */
    sh = getenv("SHELL");
    if ( !sh )
# if ( LINUX )
        sh = "/bin/bash";
# elif ( SOLARIS )
        sh = "/usr/bin/ksh";
# elif ( CYGWIN )
        sh = wingetshell();
# else
        sh = "/bin/sh";
# endif

    /* Do shell */
    return ( callout(sh) );
}

/** Spawn a command **/
int spawn P2_(int, f, int, n)
/* f: Flags           */
/* n: Argument count  */
{
    char  line[NLINE];
    int   s = 0;

    ZEROMEM(line);

    /* Don't allow this command if restricted */
    if ( restflag )
        return ( resterr() );

    /* Get command line */
    s = mlreply("!", line, NLINE);
    if ( !s )
        return (s);

    /* Perform the command */
    s = callout(line);

    /* if we are interactive, pause here */
    if ( clexec == FALSE ) {
        mlwrite("[End]");
        ttflush();
        ttgetc();
    }

    return (s);
}

/** Execute program **/
int execprg P2_(int, f, int, n)
/* f: Flags           */
/* n: Argument count  */
{
    /* Same as spawn */
    return ( spawn(f, n) );
}


/*=============================================================================*/
/* Joachim Schneider, 2018-11-02/2023-08-19, joachim<at>hal.rhein-neckar.org   */
/*=============================================================================*/

static int  IsDir P1_(CONST char *, dir)
{
    struct stat sb;

    ZEROMEM(sb);

    if ( !dir || !*dir )  {
        return FALSE;
    }
    if        ( 0 > stat(dir, &sb) )  {
        return FALSE;
    } else if ( S_ISDIR(sb.st_mode) ) {
        return TRUE;
    } else                            {
        return FALSE;
    }
}

/* Get a directory for temporary files: Cannot fail.  */
static char *gettmpdir P0_()
{
    static char *res  = NULL;

    if ( NULL == res )  {
        static char tmpd[NFILEN];
        int         l   = 0;

        /**ZEROMEM(tmpd);**/

        /* Test environment variables TMP, and TEMP: */
        if ( 0 < xstrlcpy(tmpd, getenv("TMP"), SIZEOF(tmpd)) )  {
            NormalizeDirSep(tmpd);
            if ( IsDir(tmpd) )  {
                goto found;
            }
        }
        if ( 0 < xstrlcpy(tmpd, getenv("TEMP"), SIZEOF(tmpd)) ) {
            NormalizeDirSep(tmpd);
            if ( IsDir(tmpd) )  {
                goto found;
            }
        }

        /* Test the directory used by `tmpnam()': */
# ifdef P_tmpdir
        xstrlcpy(tmpd, P_tmpdir, SIZEOF(tmpd));
        if ( IsDir(tmpd) )  {
            goto found;
        }
# else
        if ( IsDir("/tmp") )  {
            xstrlcpy(tmpd, "/tmp", SIZEOF(tmpd));
            goto found;
        }
# endif

        /* Last resort: Use current directory:  */
        xstrlcpy(tmpd, ".", SIZEOF(tmpd));


    found:
        l = strlen(tmpd);
        if        ( 0 == l )              {
            xstrlcpy(tmpd, ".", SIZEOF(tmpd));
        } else if ( '/' == tmpd[l - 1] )  {
            if ( 1 < l )  {
                tmpd[l - 1] = '\0';
            } else {  /* "/"  */
                tmpd[l - 1] = '.';
            }
        }

        res = tmpd;
    }

    return res;
}

/* gettmpfname:
 *
 * Return in a static buffer the name of a temporary currently not
 * existing file name containing ident in its name.
 */
char *gettmpfname P1_(CONST char *, ident)
{
    char        str[NFILEN];
    int         i   = 0;
    static int  seed = 0;
    static char res[NFILEN];

    ZEROMEM(str);
    ZEROMEM(res);

    xsnprintf( str, SIZEOF (str), "%s/me-%s-%02x", gettmpdir(), ident,
               ( (int)getpid() % 0x100 ) );
    for ( i = 0; i < 0x100; i++ ) {
        struct stat sb;

        ZEROMEM(sb);

        xsnprintf(res, SIZEOF (res), "%s-%02x", str, (seed + i) % 0x100);
        if ( 0 > stat(res, &sb) ) {
            if ( ENOENT == errno ) {            /* found */
                seed = (seed + i + 1) % 0x100;

                return res;
            }
        }
    }

    return NULL;
}

/* LAUNCHPRG:
 *
 * Launches an external program
 *
 * Returns TRUE if all went well, FALSE if failed to launch or return code
 * is not 0.
 *
 * Cmd is the command string to launch. It must be a valid SH command.
 *
 * InFile is the name of the file to pipe into stdin (if NULL, nothing
 * is piped in)
 *
 * OutFile is the name of the file where stdout is expected to be
 * redirected. If it is NULL or an empty string, stdout is not redirected.
 *
 * ErrFile is the name of the file where stderr is expected to be
 * redirected. If it is NULL or an empty string, stdout is not redirected.
*/
static int LaunchPrg P4_(const char *,  Cmd,
                         const char *,  InFile,
                         const char *,  OutFile,
                         const char *,  ErrFile)
{
    char FullCmd[NLINE];

    if ( !Cmd ) {
        return FALSE;
    }

    if ( !InFile || !*InFile ) {
# if CYGWIN
        InFile  = "NUL";
# else
        InFile  = "/dev/null";
# endif
    }
    if ( !OutFile || !*OutFile ) {
# if CYGWIN
        OutFile  = "NUL";
# else
        OutFile  = "/dev/null";
# endif
    }
    if ( !ErrFile || !*ErrFile ) {
# if CYGWIN
        ErrFile  = "NUL";
# else
        ErrFile  = "/dev/null";
# endif
    }

    xsnprintf(FullCmd,
              SIZEOF (FullCmd),
# if CYGWIN
              "%s < %s > %s 2>%s",
# else
              "( %s ) < %s > %s 2>%s",
# endif
              Cmd,
              InFile,
              OutFile,
              ErrFile);

    return callout(FullCmd);
} /* LaunchPrg */

/*=============================================================================*/
/* Some helper functions that could also be in char.c/eproto.h                 */
/* We do not want to use the ctype.h functions as they depend on the locale.   */
/*=============================================================================*/

static int IsIn P3_(const char, c, const char *, set, int, len)
{
    int i = 0;

    if ( len <=0 || !set || !*set ) {
        return FALSE;
    }

    for ( i = 0; i < len; i++ ) {
        if ( c == set[i] ) {
            return TRUE;
        }
    }

    return FALSE;
}

# define UPPERS_         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define LOWERS_         "abcdefghijklmnopqrstuvwxyz"
# define DIGITS_         "0123456789"
# define SPACES_         " \t\r\n\v\f"
# define UNDERSQS_       "_"

# define IsUpper(c)      ( IsIn( (c), UPPERS_, SIZEOF (UPPERS_) - 1 ) )
# define IsLower(c)      ( IsIn( (c), LOWERS_, SIZEOF (LOWERS_) - 1 ) )
# define IsDigit(c)      ( IsIn( (c), DIGITS_, SIZEOF (DIGITS_) - 1 ) )
# define IsSpace(c)      ( IsIn( (c), SPACES_, SIZEOF (SPACES_) - 1 ) )
# define IsUndersqr(c)   ( IsIn( (c), UNDERSQS_, SIZEOF (UNDERSQS_) - 1 ) )

# define IsLetter(c)     ( IsUpper( (c) ) || IsLower( (c) ) )

#if(0)/**NOT_USED**/
static char ToUpper P1_(const char, c)
{
    int i   = 0;

    for ( i = 0; i < SIZEOF (LOWERS_) - 1; i++ ) {
        if ( c == LOWERS_[i] ) {
            return UPPERS_[i];
        }
    }

    return c;
}

static char ToLower P1_(const char, c)
{
    int i   = 0;

    for ( i = 0; i < SIZEOF (UPPERS_) - 1; i++ ) {
        if ( c == UPPERS_[i] ) {
            return LOWERS_[i];
        }
    }

    return c;
}
#endif/**NOT_USED**/

/*=============================================================================*/

# if ( 0 )
static int makecmdbname P4_(char *,       bname,
                            int,          size,
                            const char *, cmd,
                            const char *, tag)
/* Create a buffer name for the output of s shell command */
{
    static int  seed = 0;
    int         i    = 0;
    int         j    = 0;
    int         l    = 0;

    if ( !bname || size <= 1 || !cmd || !*cmd ) {
        return FALSE;
    }
    if ( !tag || !*tag ) {
        tag = "@cmd";
    }
    while ( !( IsLetter(cmd[j]) || IsDigit(cmd[j]) || IsUndersqr(cmd[j]) ) ) {
        j++;
    }
    while ( '\0' != cmd[j] && i < size - 1 ) {
        if ( IsLetter(cmd[j]) || IsDigit(cmd[j]) || IsUndersqr(cmd[j]) ) {
            bname[i++] = ToLower(cmd[j++]);
        } else if ( IsSpace(cmd[j]) ) {
            bname[i++] = '_';
            while ( IsSpace(cmd[j]) ) {
                j++;
            }
        } else {
            bname[i++] = '.';
            while ( !( IsLetter(cmd[j]) || IsDigit(cmd[j]) ||
                       IsUndersqr(cmd[j]) || IsSpace(cmd[j]) ) ) {
                j++;
            }
        }
    }

    bname[i] = '\0';
    l = xsnprintf(bname, size, "%s@-%s-%02x", tag, bname, seed % 0x100);
    seed++;

    if ( l <= size - 1 ) {
        return TRUE;
    } else {
        return FALSE;
    }
}
# else
static int makecmdbname P4_(char *,       bname,
                            int,          size,
                            const char *, cmd,
                            const char *, tag)
/* Create a buffer name for the output of s shell command */
{
    static int seed    = 0;
    int i       = 0;
    int i0      = 0;
    int l       = 0;
    char            *xcmd   = NULL;

    if ( !bname || size <= 1 || !cmd || !*cmd ) {
        return FALSE;
    }
    if ( !tag || !*tag ) {
        tag = "@cmd";
    }

    while ( IsSpace(cmd[i]) ) {
        i++;
    }

    for (;; ) {
        i0 = i;
        while ( cmd[i] != '\0' && !IsSpace(cmd[i]) && cmd[i] != '/' ) {
            i++;
        }
        if ( '/' == cmd[i] ) {
            i++;
            continue;
        } else {
            break;
        }
    }
    xcmd = xstrdup(&cmd[i0]);
    i = STRLEN(xcmd);
    while ( 0 < i ) {
        if ( IsSpace(xcmd[i - 1]) ) {
            xcmd[i - 1] = '\0';
        } else {
            break;
        }
        i--;
    }

    l = xsnprintf(bname, size, "%s-%02x: %s", tag, seed % 0x100, xcmd);
    CLROOM(xcmd);
    seed++;

    if ( l <= size - 1 ) {
        return TRUE;
    } else {
        return FALSE;
    }
}
# endif

/*=============================================================================*/

/** Pipe output of program to buffer **/
int pipecmd P2_(int, f, int, n)
/* f: Flags           */
/* n: Argument count  */
{
    char Command[NLINE];
    int Result;
    char tmpnam[NFILEN];
    char InFile[NFILEN];
    char OutFile[NFILEN];
    char bname[NFILEN]   = "command";
    char            *cp;
    BUFFER          *bp;

    /* Don't allow this command if restricted */
    if ( restflag ) {
        return resterr();
    }

    /* Get pipe-in command */
    if ( !mlreply("@", Command, NLINE) ) {
        return FALSE;
    }

    if ( NULL != ( cp = gettmpfname("fltinp") ) ) {
        XSTRCPY(InFile, cp);
    } else {
        return FALSE;
    }
    if ( NULL != ( cp = gettmpfname("command") ) ) {
        XSTRCPY(OutFile, cp);
    } else {
        return FALSE;
    }

    /* Setup the proper file names */
    bp = curbp;
    XSTRCPY(tmpnam, bp->b_fname);       /* Save the original name */
    XSTRCPY(bp->b_fname, InFile);       /* Set it to our new one */

    /* Write it out, checking for errors */
    if ( !writeout(InFile, "w") ) {
        mlwrite("[Cannot write filter file <%s>]", InFile);
        XSTRCPY(bp->b_fname, tmpnam);
        unlink(InFile);
        sleep(MLWAIT);

        return FALSE;
    }
    /* Reset file name */
    XSTRCPY(bp->b_fname, tmpnam);

# if   ( 0 )    /* Activate multiple "command" buffers  */
    makename(bname, OutFile);           /* New buffer name. */
# elif ( !0 )
    if ( !makecmdbname(bname, SIZEOF (bname), Command, "@Cmd") ) {
        unlink(InFile);

        return FALSE;
    }
# endif

    /*-find the "command" buffer */
    if ( ( bp = bfind (bname, FALSE, 0) ) != NULL ) {
        /*-make sure the contents can safely be blown away */
        if ( bp->b_flag & BFCHG ) {
            if ( mlyesno (TEXT32) != TRUE ) {
                unlink(InFile);

                return FALSE;
            }
        }
        /* discard changes */
    } else if ( ( bp = bfind (bname, TRUE, 0) ) == NULL ) {
        mlwrite (TEXT137);
        /* cannot create buffer */
        unlink(InFile);
        sleep(MLWAIT);

        return FALSE;
    }

    if ( !( Result = LaunchPrg (Command, InFile, OutFile, NULL) ) ) {
        mlwrite (TEXT3);
        /* [execution failed] */
        unlink(InFile);
        unlink(OutFile);
        sleep(MLWAIT);

        return FALSE;
    }

    {
        BUFFER  *temp_bp = curbp;
        int bmode;
        char bflag;
        int Result;

# if ( 0 )
/* Use multiple "command" windows                               */
/* Split the current window to make room for the command output */
        if ( !splitwind(FALSE, 1) ) {
            unlink(InFile);
            unlink(OutFile);

            return FALSE;
        }
# endif
        swbuffer(bp);       /* make this buffer the current one */
        bmode = bp->b_mode;
        bp->b_mode &= ~MDVIEW;
        bflag = bp->b_flag;
        bp->b_flag &= ~BFCHG;
        Result = readin(OutFile, FALSE);
        bp->b_fname[0] = '\0';          /* clear file name */
        if ( Result ) {
            bp->b_mode |= MDVIEW;               /* force VIEW mode */
            lchange (WFMODE);                   /* update all relevant mode
                                                 * lines */
            bp->b_flag &= ~BFCHG;               /* remove by-product BFCHG flag
                                                 */
        } else {
            bp->b_mode = bmode;                 /* restore mode */
            bp->b_flag = bflag;
            swbuffer (temp_bp);
        }
        unlink(InFile);
        unlink(OutFile);
    }

    return TRUE;
}

/** Filter buffer through command **/
int f_filter P2_(int, f, int, n)
/* f: Flags           */
/* n: Argument count  */
{
    char line[NLINE];
    int s;
    BUFFER  *bp;
    char    *cp;
    char tmpnam[NFILEN];
    char InFile[NFILEN];
    char OutFile[NFILEN];

    if ( NULL != ( cp = gettmpfname("fltinp") ) ) {
        XSTRCPY(InFile, cp);
    } else {
        return FALSE;
    }
    if ( NULL != ( cp = gettmpfname("fltout") ) ) {
        XSTRCPY(OutFile, cp);
    } else {
        return FALSE;
    }

    /* Don't allow this command if restricted */
    if ( restflag ) {
        return resterr();
    }

    /* Don't allow filtering of VIEW mode buffer */
    if ( curbp->b_mode & MDVIEW ) {
        return rdonly();
    }

    /* Get the filter name and its args */
    s = mlreply("#", line, NLINE);
    if ( !s ) {
        return FALSE;
    }

    /* Setup the proper file names */
    bp = curbp;
    XSTRCPY(tmpnam, bp->b_fname);       /* Save the original name */
    XSTRCPY(bp->b_fname, InFile);       /* Set it to our new one */

    /* Write it out, checking for errors */
    if ( !writeout(InFile, "w") ) {
        mlwrite("[Cannot write filter file <%s>]", InFile);
        XSTRCPY(bp->b_fname, tmpnam);
        unlink(InFile);
        sleep(MLWAIT);

        return FALSE;
    }

    /* Perform command */
    s = LaunchPrg(line, InFile, OutFile, NULL);
    /* If successful, read in file */
    if ( s ) {
        s = readin(OutFile, FALSE);
        if ( s ) {
            /* Mark buffer as changed */
            bp->b_flag |= BFCHG;
        }
    }


    /* Reset file name */
    XSTRCPY(bp->b_fname, tmpnam);

    /* and get rid of the temporary file */
    unlink(InFile);
    unlink(OutFile);

    /* Show status */
    if ( !s ) {
        mlwrite("[Execution failed]");
        sleep(MLWAIT);
    }

    return s;
}

/** Get first filename from pattern **/
char *getffile P1_(char *, fspec)
/* fspec: Filename specification  */
{
    int index, point;

    /* First parse the file path off the file spec */
    XSTRCPY(path, fspec);
    index = STRLEN(path) - 1;
    while ( index >= 0 && (path[index] != '/') )
        --index;
    path[index+1] = '\0';


    /* Check for an extension */
    point = STRLEN(fspec) - 1;
    while ( point >= 0 ) {
        if ( fspec[point] == '.' ) {
            break;
        }
        point--;
    }

    /* Open the directory pointer */
    if ( dirptr ) {
        closedir(dirptr);
        dirptr = NULL;
    }

    dirptr = opendir( (path[0] == '\0') ? "./" : path );

    if ( !dirptr )
        return (NULL);

    XSTRCPY(rbuf, path);
    nameptr = &rbuf[STRLEN(rbuf)];

    /* ...and call for the first file */
    return ( getnfile() );
}

/** Get next filename from pattern **/
char *getnfile P0_()
{
    struct DIRENTRY *dp = NULL;
    struct stat     fstat;

    ZEROMEM(fstat);

    /* ...and call for the next file */
    do {
        dp = readdir(dirptr);
        if ( !dp )
            return (NULL);

        /* Check to make sure we skip all weird entries except directories */
        XSTRCPY(nameptr, dp->d_name);

    } while (stat(rbuf,
                  &fstat) ||
             ( (fstat.st_mode & S_IFMT) & (S_IFREG | S_IFDIR) ) == 0);

    /* if this entry is a directory name, say so */
    if ( (fstat.st_mode & S_IFMT) == S_IFDIR )
        XSTRCAT(rbuf, DIRSEPSTR);

    /* Return the next file name! */
    return (rbuf);
}

# if FLABEL

/*---------------------------------------------------------------------*
 *
 *     handle the function keys and function key labels on HP-Terminals
 *     -----------------------------------------------------------------
 *
 *     Hellmuth Michaelis       e-mail: hm@hcshh.hcs.de
 *
 *--------------------------------------------------------------------*/

static unsigned char flabstor[8][50];    /* label & xmit backup store */
static char flabstof[8] = { 0,0,0,0,0,0,0,0 };  /* filled flag        */

/* FNCLABEL:
 *
 * Label a function key
 */
int fnclabel P2_(int, f, int, n)
/* f: Default argument                          */
/* n: Function key number 1...8 on hp-terminals */
{
    char lbl[20];       /* label string buffer */
    char xmit[5];       /* transmitted string ( ESC 'p'...'w' ) */
    char buf[80];       /* writeout buffer */
    int i;              /* general purpose index */
    int status;         /* return status */

    /* check if we are connected to an hp-terminal */
    if ( !hpterm )
        return (FALSE);

    /* must be called with an argument */
    if ( f == FALSE ) {
        mlwrite(TEXT246);

/*          "%%Need function key number"*/
        return (FALSE);
    }

    /* and it must be a legal key number */
    if ( n < 1 || n > 8 ) {
        mlwrite(TEXT247);

/*          "%%Function key number out of range"*/
        return (FALSE);
    }

    /* get the string to send */
    lbl[0] = '\0';      /* we don't now the label yet */

    if ( ( status = mlreply(TEXT248, lbl, 19) ) != TRUE )
/*                "Enter Label String: "*/
        return (status);

    lbl[16] = '\0';
    i = STRLEN(lbl);

    /* set up escape sequence to send to terminal */
    xmit[0] = 0x1b;
    xmit[1] = 'o' + n;
    xmit[2] = '\0';

    sprintf(flabstor[n-1], "%c&f0a%dk%dd2L%s%s", (char)0x1b, n, i, lbl, xmit);
    write( 1, flabstor[n-1], STRLEN(flabstor[n-1]) );
    flabstof[n-1] = 1;

    sprintf(buf, "%c&jB", (char)0x1b);
    write( 1, buf, STRLEN(buf) );

    return (TRUE);
}

/* display user function key labels */
static VOID dis_ufk P0_()
{
    int label_num;
    char buf[6];

    if ( !hpterm )
        return;

    for ( label_num = 0; label_num < 8; label_num++ )
        if ( flabstof[label_num] )
            write( 1, flabstor[label_num], STRLEN(flabstor[label_num]) );

    sprintf(buf, "%c&jB", (char)0x1b);
    write( 1, buf, STRLEN(buf) );
}

/* display system function key labels */
static VOID dis_sfk P0_()
{
    char buf[6];

    if ( !hpterm )
        return;

    sprintf(buf, "%c&jA", (char)0x1b);
    write( 1, buf, STRLEN(buf) );
}

# endif /* FLABEL */

# if XENIX && FILOCK

int mkdir P2(char *, name, int , mode)
/* name:  Name of directory to create                         */
/* mode:  Umask for creation (which we blissfully ignore...)  */
{
    char buf[80];

    XSTRCPY(buf, "mkdir ");
    XSTRCAT(buf, name);
    XSTRCAT(buf, " > /dev/null 2>&1");

    return ( system(buf) );
}

int rmdir P1_(char *, name)
/* name:  Name of directory to delete */
{
    char buf[80];

    XSTRCPY(buf, "rmdir ");
    XSTRCAT(buf, name);
    XSTRCAT(buf, " > /dev/null 2>&1");

    return ( system(buf) );
}

# endif /* XENIX & FILOCK */

# if HANDLE_WINCH
/* Window size changes handled via signals. */

VOID winch_changed P1_(int, signo)
{
    signal(SIGWINCH, winch_changed);
    winch_flag = 1;
}

VOID winch_new_size P0_()
{
    struct winsize win;

    ZEROMEM(win);

    winch_flag  = 0;
    ioctl(fileno(stdin), TIOCGWINSZ, &win);
    winch_vtresize(win.ws_row, win.ws_col);
    onlywind(0, 0);
    TTmove(0, 0);
    TTeeop();
}

# endif /* HANDLE_WINCH */

#endif /* IS_UNIX() */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
