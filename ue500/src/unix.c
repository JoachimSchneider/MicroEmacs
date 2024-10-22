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
#include "estruct.h"            /* Emacs definitions        */
#include "eproto.h"             /* Function definitions     */


/*==============================================================*/
/* FEATURES                                                     */
/*==============================================================*/
/* Accordung to R. Stevens curses should contain some functions */
/* for terminal control, but obviously they aren't used here    */
/* correctly: It seems, that someone started it but didn't end. */
#define USE_CURSES              ( 0 )   /* NOT WORKING */
#if ( IS_ANCIENT_UNIX() )
# define USE_SGTTY              ( 1 )
# define USE_TERMIO_IOCTL       ( 0 )
# define USE_TERMIOS_TCXX       ( 0 )
#elif ( !IS_POSIX_UNIX() )
# define USE_SGTTY              ( 0 )
# define USE_TERMIO_IOCTL       ( 1 )
# define USE_TERMIOS_TCXX       ( 0 )
#else
# define USE_SGTTY              ( 0 )
# define USE_TERMIO_IOCTL       ( 0 )
# define USE_TERMIOS_TCXX       ( 1 )
#endif
/* Enable/disable XON/XOFF: We want to use ^S/^Q. I do not believe the flow
 * control settings of the OS should be diddled by an application program. But
 * if you do, change this 1 to a 0, but be warned, all sorts of terminals will
 * get grief with this
 */
#if ( DJGPP_DOS )
# define USE_CTL_SQ               ( 1 )
#else
# define USE_CTL_SQ               ( 0 )
#endif

#if ( CYGWIN )
/* Old CYGWIN Versions don't have these:  */
# /**define  USE_CYGWIN_CONV_PATH    ( 1 )**/
# define  USE_CYGWIN_CONV_PATH    ( 0 )
/* On CygWin spawn is more reliable than fork/exec but it's not
 * available on older CygWin versions */
# /**define  USE_CYGWIN_SPAWN        ( 1 )**/
# define  USE_CYGWIN_SPAWN        ( 0 )
# if ( 3 >= __GNUC__ )
#  undef  USE_CYGWIN_CONV_PATH
#  undef  USE_CYGWIN_SPAWN
#  define USE_CYGWIN_CONV_PATH    ( 0 )
#  define USE_CYGWIN_SPAWN        ( 0 )
# endif
#endif


/* Several different methods to do noblocking read from a terminal:
 *
 * - VMIN/VTIME setting of the terminal attributes.
 * - The `select()' system call:
 *   This *must* be used with CygWin or DJGPP's setting VMIN/VTIME won't
 *   work with CygWin or DJGPP.
 * - As a substitute one may try the readx() implemented here.
 *
 * If these settings do not work properly the  termination of search
 * strings with <META> is not working (two <META>s needed).
 */
#define USE_TERMINAL_VTIME  (1)
#define USE_TERMINAL_SELECT (2)
#define USE_TERMINAL_READX  (3)

#ifndef TERMINAL_NOBLOCK_READ
# if    ( CYGWIN || DJGPP_DOS )
#  define TERMINAL_NOBLOCK_READ USE_TERMINAL_SELECT
# elif  ( IS_ANCIENT_UNIX() )
#  define TERMINAL_NOBLOCK_READ USE_TERMINAL_READX
# else
#  define TERMINAL_NOBLOCK_READ USE_TERMINAL_SELECT
# endif
#endif
#if   ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_VTIME  )
#elif ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_SELECT )
#elif ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_READX  )
#else
# error Invalid value for TERMINAL_NOBLOCK_READ
#endif


#if ( DJGPP_DOS )
/* Use spawn() or DJGPP's smart system() for `dossystem()': */
# define USE_SPAWN_FOR_SYSTEM   ( 1 )
#endif
/*==============================================================*/


/** Do nothing routine **/
int scnothing P1_(char *, s)
{
    return (0);
}

/** Only compile for UNIX machines **/
#if ( IS_UNIX() )

/** Include files **/
# include <stdlib.h>            /* getenv()                 */
# include <time.h>              /* time(), ...              */
# include <errno.h>             /* errno, ...               */
# include <sys/stat.h>          /* stat(), ...              */
# if ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_SELECT )
#  if ( !DJGPP_DOS )  /* select() prototype in time.h */
#   include <sys/select.h>
#  endif
#  include <sys/time.h>
# endif
# include "edef.h"              /* Global variable definitions  */
# include "elang.h"             /* Language definitions     */

/** Kill predefined **/
# undef CTRF                            /* Problems with CTRF       */


/** Overall include files **/
# include <sys/types.h>                 /* System type definitions  */
# include <sys/stat.h>                  /* File status definitions  */
# include <sys/time.h>
# include <sys/param.h>
# include <sys/ioctl.h>                 /* I/O control definitions  */
# include <signal.h>                    /* Signal definitions       */
# if ( !IS_ANCIENT_UNIX() )
#  include <unistd.h>
# endif

# if   ( USE_SGTTY )
#  include <sgtty.h>                    /* stty() / gtty()          */
# elif ( USE_TERMIO_IOCTL )
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
#  if USE_CYGWIN_CONV_PATH
#   include <sys/cygwin.h>
#  endif
#  if USE_CYGWIN_SPAWN
#   include <spawn.h>
#  endif
#  include <sys/wait.h>
# endif /* CYGWIN */

# if ( DJGPP_DOS )
#  if ( USE_SPAWN_FOR_SYSTEM )
#   include <process.h>
#  endif
# endif

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
# if CYGWIN
EXTERN const char *cygpwd_      DCL((void));
# endif /* CYGWIN */
/*==============================================================*/


/*====================================================================*/
/* Static functions declared here:                                    */
/*====================================================================*/
static int  IsExecutable  DCL((CONST char * file));
#if ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_READX )
static int  rdstdin       DCL((int getnread));
# define nread()  ( rdstdin(!0) )
# define readx()  ( rdstdin(0)  )
#endif
/*====================================================================*/


/*==============================================================*/
#define   TGETFLAG(x)     tgetflag((char *)(x))
#define   TGETNUM(x)      tgetnum((char *)(x))
# if VAT
#  define TGETSTR(a, b)   tgetstr( (char *)(a), *(b) )
# else
#  define TGETSTR(a, b)   tgetstr( (char *)(a), (b) )
# endif

# define MkUNXDirSep_(path) do  {         \
    char  *cp__ = (path);                 \
                                          \
    while ( *cp__ ) {                     \
        if ( '\\' == *cp__ )  {           \
            *cp__ = '/';                  \
        }                                 \
        cp__++;                           \
    }                                     \
} while ( 0 )

# define MkDOSDirSep_(path) do  {         \
    char  *cp__ = (path);                 \
                                          \
    while ( *cp__ ) {                     \
        if ( '/' == *cp__ ) {             \
            *cp__ = '\\';                 \
        }                                 \
        cp__++;                           \
    }                                     \
} while ( 0 )

# if ( CYGWIN )

/* Will not fail  */
#  define CYGDRIVE_DFT_ "/cygdrive/"
static CONST char *cygdrive_ P0_()
{
    static char       res[NFILEN];
    static CONST char *rc       = res;
    static int        FirstCall = !0;

    if ( FirstCall )  {
        CONST char  drives[]  = "CDEFGHIJKLMNOPQRSTUVWXYZAB";
        char        cwd[NFILEN];
        int         i         = 0;
        int         l         = 0;

        ZEROMEM(cwd);

        if ( NULL == getcwd(cwd, SIZEOF(cwd)) ) {
            goto notfound;
        }
        /*------------------------------------------------------------*/

        for ( i = 0; i < SIZEOF(drives) - 1; i++ )  {
            char  dir[4];

            dir[0]  = drives[i];
            dir[1]  = ':';
            dir[2]  = '/';
            dir[3]  = '\0';

            ZEROMEM(res);
            if ( 0 == chdir(dir) )  {
                if ( NULL != getcwd(res, SIZEOF(res)) ) {
                    break;
                }
            }
        }

        /*------------------------------------------------------------*/
        if ( 0 != chdir(cwd) )  {
            goto notfound;
        }

        if ( SIZEOF(drives) - 1 <= i )  {
            goto notfound;
        }

        if ( 0 == (l = strlen(res)) ) {
            goto notfound;
        }

        /* If there is no "/cygdrive" then `C:\' ---> `/':  */
        if ( 1 == l && '/' == res[0] )  {
            TRC(("cygdrive_(): `%s'", "no `/cygdrive' here, returning NULL"));
            rc  = NULL;

            goto end;
        }

        l--;
        res[l]  = '\0';
        /* Is it `/abcde/'? If not classify as "notfound":  */
        if ( !( 3 <= l && '/' == res[0] && '/' == res[l - 1] ) )  {
            ZEROMEM(res);

            goto notfound;
        }

        TRC(("cygdrive_(): `%s'", res));

        goto end;


notfound:
        xstrlcpy(res, CYGDRIVE_DFT_, SIZEOF(res));
        TRC(("cygdrive_(): Not found using default `%s'", res));

        goto end;


end:
        FirstCall = 0;
    }

    return rc;
}

static int cygdrive_len_ P0_()
{
    static int  FirstCall = !0;
    static int  len       = (-1);

    if ( FirstCall )  {
        CONST char  *cd = NULL;

        if ( NULL == (cd = cygdrive_()) ) {
            len = (-1);
        } else                            {
            len = strlen(cd);
        }

        FirstCall = 0;
    }

    return len;
}

#if ( !0 )
# define CYGDRIVE_      ( cygdrive_() )
# define CYGDRIVE_LEN_  ( cygdrive_len_() )
#else
# define CYGDRIVE_      "/cygdrive/"
# define CYGDRIVE_LEN_  ( SIZEOF(CYGDRIVE_) - 1 )
#endif

#  define NormalizePathUNX(path)  do  {                     \
    char  *cp_  = (path);                                   \
                                                            \
    if ( 0 <= IsDOSPath(cp_) )  {                           \
        MkDOSDirSep_(cp_);                                  \
        xstrlcpy(cp_, getunxpath(cp_), SIZEOF(path));       \
    }                                                       \
    MkUNXDirSep_(cp_);                                      \
} while ( 0 )
#  define NormalizePathDOS(path)  do  {                     \
    char  *cp_  = (path);                                   \
                                                            \
    if ( 0 >= IsDOSPath(cp_) )  {                           \
        MkUNXDirSep_(cp_);                                  \
        xstrlcpy(cp_, getdospath(cp_), SIZEOF(path));       \
    }                                                       \
    MkDOSDirSep_(cp_);                                      \
} while ( 0 )
#  define NULL_DEVICE             "NUL"
# elif ( DJGPP_DOS )
#  define NormalizePathUNX(path)  do  {                     \
    char  *cp_  = (path);                                   \
                                                            \
    MkUNXDirSep_(cp_);                                      \
} while ( 0 )
#  define NormalizePathDOS(path)  do  {                     \
    char  *cp_  = (path);                                   \
                                                            \
    if ( 0 >= IsDOSPath(cp_) )  {                           \
        xstrlcpy(cp_, getdospath(cp_), SIZEOF(path));       \
    }                                                       \
    MkDOSDirSep_(cp_);                                      \
} while ( 0 )
#  define NULL_DEVICE             "NUL"
# else
#  define NormalizePathUNX(path)  VOIDCAST(0)
#  define NormalizePathDOS(path)  VOIDCAST(0)
#  define NULL_DEVICE             "/dev/null"
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
# if   ( USE_SGTTY )
static struct sgttyb  curterm;          /* Current modes              */
static struct sgttyb  oldterm;          /* Original modes             */
/*======================================================================
/o
 o List of special characters
 o/
struct tchars {
        char    t_intrc;        /o interrupt o/
        char    t_quitc;        /o quit o/
        char    t_startc;       /o start output o/
        char    t_stopc;        /o stop output o/
        char    t_eofc;         /o end-of-file o/
        char    t_brkc;         /o input delimiter (like nl) o/
};
======================================================================*/
static struct tchars  curtchars = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static struct tchars  oldtchars;	/* Org terminal special chars */
# elif ( USE_TERMIO_IOCTL )
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
# if    ( USE_SGTTY )
    gtty(0, &oldterm);
    curterm = oldterm;
    curterm.sg_flags |= RAW;
    curterm.sg_flags &= ~(ECHO|CRMOD);
    stty(0, &curterm);
    ioctl(0, TIOCGETC, &oldtchars);
    ioctl(0, TIOCSETC, &curtchars);
# elif  ( USE_TERMIO_IOCTL )

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
# elif  ( USE_TERMIOS_TCXX )
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
# elif  ( USE_CURSES )
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

# if    ( USE_SGTTY )
  stty(0, &oldterm);
  ioctl(0, TIOCSETC, &oldtchars);
# elif  ( USE_TERMIO_IOCTL )
#  if SMOS
    /* Extended settings; 890619mhs A3 */
    set_parm(0, -1, -1);
#  endif /* SMOS */
    if ( ioctl(0, TCSETA, &oldterm) )
        return (-1);

# elif  ( USE_TERMIOS_TCXX )
    /* Set tty mode */
    if ( tcsetattr(0, TCSANOW, &oldterm) ) {
        perror("Cannot tcsetattr");

        return (-1);
    }
# elif  ( USE_CURSES )
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
# if ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_READX  )
    int           rv  = 0;
# endif

# if ( USE_SGTTY )
# else
    /* Change mode, if necessary */
    if ( curterm.c_cc[VTIME] ) {
        curterm.c_cc[VMIN] = 1;
        curterm.c_cc[VTIME] = 0;
#  if   ( USE_TERMIOS_TCXX )
        tcsetattr(0, TCSANOW, &curterm);
#  elif ( USE_TERMIO_IOCTL )
        ioctl(0, TCSETA, &curterm);
#  elif ( USE_CURSES )
        /* ? */
#  else
#  endif
    }
# endif

# if ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_READX  )
    /* Perform read */
#  if HANDLE_WINCH
    while ( ( rv = readx() ) < 0 )  {
            if ( winch_flag )
                return 0;
    }
#  else
    rv  = readx();
    if ( 0 > rv ) {
        puts("** Horrible read error occured **");
        exit(1);
    }
#  endif
    ch  = rv;
# else
    /* Perform read */
#  if HANDLE_WINCH
    while ( read(0, &ch, 1) != 1 ) {
        if ( winch_flag )
            return 0;
    }
#  else
    if ( read(0, &ch, 1) != 1 ) {
        puts("** Horrible read error occured **");
        exit(1);
    }
#  endif
# endif

    /* Return new character */
# if ( 0 )
    TRC(("grabwait(): 0x%02X, <%c>", (unsigned int)(ch), (char)ch));
# endif

    return (ch);
}

/** Grab input characters, short wait **/
# if    ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_SELECT )
unsigned char PASCAL NEAR grabnowait P0_()
{
    fd_set          rfds;
    struct timeval  tv;
    int             retval  = 0;

    ZEROMEM(rfds);
    ZEROMEM(tv);

    /* Watch stdin (fd 0) to see when it has input. */

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /* Wait up to UNIX_READ_TOUT * 1/10 seconds:  */
    tv.tv_sec = UNIX_READ_TOUT / 10;
    tv.tv_usec = 100000 * (UNIX_READ_TOUT % 10);

    retval = select(1, &rfds, NULL, NULL, &tv);
    if        ( 0 >  retval ) { /* error          */
        TRC(("grabnowait(): %s", "select error"));

        return (grabnowait_TIMEOUT);
    } else if ( 0 == retval ) { /* timeout        */
        return (grabnowait_TIMEOUT);
    } else /* 0 < retval */   { /* data available */
        int           count = 0;
        unsigned char ch    = '\0';

        /* Perform read */
#  if HANDLE_WINCH
        while ( ( count = read(0, &ch, 1) ) < 0 ) {
            if ( winch_flag )
                return 0;
        }
#  else
        count = read(0, &ch, 1);
        if ( count < 0 ) {
            puts("** Horrible read error occured **");
            exit(1);
        }
#  endif
        if ( count == 0 ) { /* Should not happen  */
            TRC(("grabnowait(): %s", "select .GT. 0 but no data"));

            return (grabnowait_TIMEOUT);
        }
        /* Return new character */
#  if ( 0 )
        TRC(("grabnowait(): 0x%02X, <%c>", (unsigned int)(ch), (char)ch));
#  endif
        return (ch);
    }
}
# elif  ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_VTIME  )
unsigned char PASCAL NEAR grabnowait P0_()
{
    int           count = 0;
    unsigned char ch    = '\0';

    /* Change mode, if necessary */
    if ( curterm.c_cc[VTIME] == 0 ) {
        curterm.c_cc[VMIN] = 0;
        curterm.c_cc[VTIME] = UNIX_READ_TOUT;
#  if   ( USE_TERMIOS_TCXX )
        tcsetattr(0, TCSANOW, &curterm);
#  elif ( USE_TERMIO_IOCTL )
        ioctl(0, TCSETA, &curterm);
#  elif ( USE_CURSES )
        /* ? */
#  else
#  error MISSING TERMINAL CONTROL DEFINITION
#  endif
    }

    /* Perform read */
#  if HANDLE_WINCH
    while ( ( count = read(0, &ch, 1) ) < 0 ) {
        if ( winch_flag )
            return 0;
    }
#  else
    count = read(0, &ch, 1);
    if ( count < 0 ) {
        puts("** Horrible read error occured **");
        exit(1);
    }
#  endif
    if ( count == 0 ) {
        return (grabnowait_TIMEOUT);
    }

    /* Return new character */
#  if ( 0 )
    TRC(("grabnowait(): 0x%02X, <%c>", (unsigned int)(ch), (char)ch));
#  endif
    return (ch);
}
# elif  ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_READX  )
unsigned char PASCAL NEAR grabnowait P0_()
{
    if ( 0 >= nread() ) {
        return (grabnowait_TIMEOUT);
    } else              { /* data available */
        int           rv  = 0;
        unsigned char ch  = '\0';

        /* Perform read */
#  if HANDLE_WINCH
        while ( ( rv = readx() ) < 0 )  {
            if ( winch_flag )
                return 0;
        }
#  else
        rv  = readx();
        if ( 0 > rv ) {
            puts("** Horrible read error occured **");
            exit(1);
        }
#  endif
        /* Return new character */
        ch  = rv;
#  if ( 0 )
        TRC(("grabnowait(): 0x%02X, <%c>", (unsigned int)(ch), (char)ch));
#  endif
        return (ch);
    }
}
# else
#   error IMPOSSIBLE
# endif /* TERMINAL_NOBLOCK_READ */

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

int ttgetc_nowait P0_()
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

/** Set palette
 * RC:
 *  - 0: Success
 *  - 1: Error
 **/
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
        dokeymap = !0;
#  if COLOR
    } else if ( strncmp(cmd, "CLRMAP ", 7) == 0 ) {
        doclrmap = !0;
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

# if ( !DJGPP_DOS )
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
# endif

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

# if ( CYGWIN || DJGPP_DOS )

/* ISDOSPATH:
 *
 * Classify path:
 * .GT. 0:  DOS Path
 * .EQ. 0:  Unknown
 * .LT. 0:  UNIX Path
 */
static int  IsDOSPath P1_(CONST char *, path)
{
    int         len       = 0;
    CONST char  *pFSlash  = NULL; /* Points to '/'  */
    CONST char  *pBSlash  = NULL; /* Points to '\\' */

    if ( NULL == path || '\0' == *path )  {
        return 0;
    }

    ASRT(1 <= (len = strlen(path)));

    if        ( '\\' == *path ) {
        return  ( 1 );
    } else if ( '/' == *path )  {
        return  ( -1 );
    } else if ( 1 == len )      {
        return ( 0 );
    }


    /* 2 .GE. len .AND. *path .NOT. .IN. { '\\', '/' }  */

    /* Path with drive letter, e.g. "C:": */
    if  ( ISALPHA(*path) && ':' == *(path + 1) )  {
        return  ( 1 );
    }

#  if   CYGWIN    /* /cygdrive/c is C:  */
    if ( NULL != CYGDRIVE_                                            &&
         strcasestart(CYGDRIVE_, path)                                &&
         CYGDRIVE_LEN_  < len                                         &&
         ISALPHA(path[CYGDRIVE_LEN_])                                 &&
         (CYGDRIVE_LEN_ + 1 == len || '/' == path[CYGDRIVE_LEN_ + 1])
        )   {
        return (-1);
    }
#  elif DJGPP_DOS /* /dev/c is C: */
#   define DJGPPDRIVE_      "/dev/"
#   define DJGPPDRIVE_LEN_  ( SIZEOF(DJGPPDRIVE_) - 1 )
    if ( strcasestart(DJGPPDRIVE_, path)                              &&
         DJGPPDRIVE_LEN_  < len                                       &&
         ISALPHA(path[DJGPPDRIVE_LEN_])                               &&
         (DJGPPDRIVE_LEN_ + 1 == len || '/' == path[DJGPPDRIVE_LEN_ + 1])
        )   {
        return (-1);
    }
#   undef DJGPPDRIVE_
#   undef DJGPPDRIVE_LEN_
#  endif

    pFSlash = strchr(path, '/');
    pBSlash = strchr(path, '\\');
    if ( NULL == pFSlash  ) {
        if ( NULL == pBSlash )  {
            return ( 0 );
        } else                  {
            return ( 1 );
        }
    } else                  {
        if ( NULL == pBSlash )  {
            return ( -1 );
        } else                  {
            /* '/' and '\\' occur in path:  */
            if ( pBSlash < pFSlash )  {
                return ( 1 );
            } else if ( pBSlash > pFSlash ) {
                return ( - 1 );
            } else                          {
                return ( 0 );
            }
        }
    }
}

static CONST char *wingetshell P0_()
{
    static int        FirstCall = !0;
    static CONST char *res      = NULL;

    if ( FirstCall )  {
        char        SystemRoot[NFILEN];
        static char WinCmd[NFILEN];

        ZEROMEM(SystemRoot);
        ZEROMEM(WinCmd);

        if ( 0 == xstrlcpy(SystemRoot, getenv("SYSTEMROOT"), SIZEOF(SystemRoot)) )  {
            xstrlcpy(SystemRoot, "C:\\WINDOWS", SIZEOF(SystemRoot));
        }
        MkDOSDirSep_(SystemRoot);
        xsnprintf(WinCmd, SIZEOF(WinCmd), "%s\\system32\\cmd.exe", SystemRoot);
        if ( IsExecutable(WinCmd) ) {
            res = WinCmd;
            TRC(("wingetshell(): %s", res));
        } else                      {
            res = NULL;
        }

        FirstCall = 0;
    }

    return res;
}

# endif

# if ( CYGWIN )

#  define TO_DOS_SEP_(x)  do  {                 \
    unsigned char *cp__ = (unsigned char *)(x); \
    unsigned char c__   = '\0';                 \
                                                \
    while ( ( c__ = *cp__) )  {                 \
        if ( '/' == c__ ) {                     \
            *cp__ = '\\';                       \
        }                                       \
        cp__++;                                 \
    }                                           \
} while ( 0 )

#  define TO_UNX_SEP_(x)  do  {                 \
    unsigned char *cp__ = (unsigned char *)(x); \
    unsigned char c__   = '\0';                 \
                                                \
    while ( ( c__ = *cp__) )  {                 \
        if ( '\\' == c__ )  {                   \
            *cp__ = '/';                        \
        }                                       \
        cp__++;                                 \
    }                                           \
} while ( 0 )

/* Cannot fail  */
static CONST char *cygpwdads P0_()
{
    static char       res[NFILEN];
    CONST char        *cp = NULL;

    ZEROMEM(res);

    ASRT(NULL != (cp = cygpwd_()));
    xstrlcpy(res, cp, SIZEOF(res));

    return res;
}

/* Cannot fail  */
static int cygdrvdos P0_()
{
    return (unsigned char)cygpwdads()[0];
}

/* Return NULL if dos is not an absolute DOS path               */
/* Return NULL if NULL == CYGDRIVE_ .AND. drv .NE. cygdrvdos()  */
/* Pass through NULL                                            */
static CONST char *cygads2enx_ P1_(CONST char *, dos)
{
    unsigned char in[NFILEN];
    static char   res[NFILEN];
    int           len = 0;
    int           drv = '\0';

    ZEROMEM(in);
    ZEROMEM(res);

    if ( NULL == dos )  {
        return NULL;
    }

    xstrlcpy((char *)in, dos, SIZEOF(in));
    TO_DOS_SEP_(in);
    len = strlen((CONST char *)in);

    if ( 2 <= len && ':' == in[1] && ISALPHA(drv = in[0]) ) {
        int i = 2;
        int j = 0;

        if ( NULL != CYGDRIVE_ )  {
            j = CYGDRIVE_LEN_;
            drv = tolower(drv);
            xstrlcpy(res, CYGDRIVE_, SIZEOF(res));
            res[j++]  = drv;
            if ( 2 == len ) {
                return res;
            }
            res[j++]  = '/';
            CASRT(SIZEOF(res) - 1 >= CYGDRIVE_LEN_ + 1 + 1);
            if ( '\\' == in[i] )  {
                i++;
            }
            for ( ; i < len && j < SIZEOF(res) - 1; i++, j++ )  {
                if ( '\\' == in[i] )  {
                    res[j]  = '/';
                } else                {
                    res[j]  = in[i];
                }
            }
        } else                    {
            if ( tolower(drv) != tolower(cygdrvdos()) ) {
                return NULL;
            }
            res[j++]  = '/';
            if ( 2 == len ) {
                return res;
            }
            if ( '\\' == in[i] )  {
                i++;
            }
            for ( ; i < len && j < SIZEOF(res) - 1; i++, j++ )  {
                if ( '\\' == in[i] )  {
                    res[j]  = '/';
                } else                {
                    res[j]  = in[i];
                }
            }
        }
    } else                                                  {
        return NULL;
    }

    return res;
}


/* Return NULL if unx is *not* an expanded UNIX path  */
/* Pass through NULL                                  */
static CONST char *cygenx2ads_ P1_(CONST char *, unx)
{
    unsigned char in[NFILEN];
    static char   res[NFILEN];
    int           len = 0;
    int           drv = '\0';

    ZEROMEM(in);
    ZEROMEM(res);

    if ( NULL == unx )  {
        return NULL;
    }

    xstrlcpy((char *)in, unx, SIZEOF(in));
    TO_UNX_SEP_(in);
    len = strlen((CONST char *)in);
    if ( NULL == CYGDRIVE_ )  {
        int i = 0;
        int j = 2;

        if ( '/' != in[0] ) {
            return NULL;
        }
        res[0]  = cygdrvdos();
        res[1]  = ':';
        CASRT(SIZEOF(res) - 1 >= 2);
        for ( ; i < len && j < SIZEOF(res) - 1; i++, j++ )  {
            if ( '/' == in[i] ) {
                res[j]  = '\\';
            } else              {
                res[j]  = in[i];
            }
        }
    } else                    {
        if ( strcasestart(CYGDRIVE_, (CONST char *)in)                    &&
             CYGDRIVE_LEN_  < len                                         &&
             ISALPHA(drv = in[CYGDRIVE_LEN_])                             &&
             (CYGDRIVE_LEN_ + 1 == len || '/' == in[CYGDRIVE_LEN_ + 1])
            )   {
            int i = 0;
            int j = 0;

            drv = toupper(drv);
            res[0]  = drv;
            res[1]  = ':';
            res[2]  = '\\';
            CASRT(SIZEOF(res) - 1 >= 3);
            i = CYGDRIVE_LEN_ + 1;
            j = CYGDRIVE_LEN_ + 1 == len ? 3 : 2;
            for ( ; i < len && j < SIZEOF(res) - 1; i++, j++ )  {
                if ( '/' == in[i] ) {
                    res[j]  = '\\';
                } else              {
                    res[j]  = in[i];
                }
            }
        } else  {
            return NULL;
        }
    }

    return res;
}

#  if ( 0 ) /* Not needed */
/* Cannot fail  */
static CONST char *cygrootads P0_()
{
    static int  FirstCall = !0;
    static char res[NFILEN];

    if ( FirstCall )  {
        char  cwd[NFILEN];

        ZEROMEM(cwd);

        ZEROMEM(res);
        ASRT( NULL != getcwd(cwd, SIZEOF(cwd)) );
        ASRT( 0 == chdir("/") );
        xstrlcpy(res, cygpwdads(), SIZEOF(res));
        ASRT( 0 == chdir(cwd) );


        FirstCall = 0;
    }

    return res;
}
#  endif  /* Not needed */

#  if ( 0 ) /* Not needed */
/* Cannot fail  */
static CONST char *cygrootenx P0_()
{
    static int  FirstCall = !0;
    static char res[NFILEN];

    if ( FirstCall )  {
        CONST char  *cp = NULL;

        ZEROMEM(res);
        ASRT( NULL != (cp = cygads2enx_(cygrootads())) );
        xstrlcpy(res, cp, SIZEOF(res));
    }

    return res;
}
#  endif  /* Not needed */

/* Cannot fail  */
static CONST char *cygpwdenx P0_()
{
    static char res[NFILEN];

    CONST char  *cp = NULL;

    ZEROMEM(res);

    ASRT( NULL != (cp = cygads2enx_(cygpwdads())) );
    xstrlcpy(res, cp, SIZEOF(res));

    return res;
}

#  if ( 0 ) /* Not needed */
/* Return DOS path of an *existing* (`xunx') UNIX directory */
/* Return NULL if unx does not exist as a directory         */
/* Pass through NULL                                        */
static CONST char *cygxunx2ads_ P1_(CONST char *, unx)
{
    static char res[NFILEN];
    CONST char  *rc = res;
    char        cwd[NFILEN];

    ZEROMEM(res);
    ZEROMEM(cwd);

    if ( NULL == unx )  {
        return NULL;
    }

    ASRT( NULL != getcwd(cwd, SIZEOF(cwd)) );

    if ( 0 != chdir(unx) )  {
        rc  = NULL;
    } else                  {
        xstrlcpy(res, cygpwdads(), SIZEOF(res));
    }

    ASRT( 0 == chdir(cwd) );

    return rc;

}
#  endif  /* Not needed */

/* Return expanded UNIX path of an *existing* (`xunx') UNIX directory */
/* Return NULL if unx does not exist as a directory                   */
/* Pass through NULL                                                  */
static CONST char *cygxunx2enx_ P1_(CONST char *, unx)
{
    static char res[NFILEN];
    CONST char  *rc = res;
    char        cwd[NFILEN];

    ZEROMEM(res);
    ZEROMEM(cwd);

    if ( NULL == unx )  {
        return NULL;
    }

    ASRT( NULL != getcwd(cwd, SIZEOF(cwd)) );

    if ( 0 != chdir(unx) )  {
        rc  = NULL;
    } else                  {
        xstrlcpy(res, cygpwdenx(), SIZEOF(res));
    }

    ASRT( 0 == chdir(cwd) );

    return rc;

}

/* Return expanded (`/cygdrive/...') UNIX path of an UNIX directory */
/* Cannot fail                                                      */
static CONST char *cygunx2enx P1_(CONST char *, unx)
{
    static char res[NFILEN];

    char        cwd[NFILEN];
    static char prevcwd[NFILEN];
    static char prevunx[NFILEN];

    CONST char  *hp   = NULL;
    int         l     = 0;
    int         i     = 0;


    ZEROMEM(cwd);

#  if ( 0 )
    ASRT(NULL != unx);
#  else
    if ( NULL == unx )  {
        unx = "";
    }
#  endif

    /* Optimize for:
     * - CWD and unx same as previous call
     * - Return old res
     */
    if ( *res  /* NOT the first call */    &&
         NULL != getcwd(cwd, SIZEOF(cwd))  &&
         0 == strcmp(prevcwd, cwd)         &&
         0 == strcmp(prevunx, unx) ) {
        return res;
    }

    xstrlcpy(prevcwd, cwd, SIZEOF(prevcwd));
    xstrlcpy(prevunx, unx, SIZEOF(prevunx));
    ZEROMEM(res);

    /* Empty input: Return CWD: */
    if ( 0 == (l = strlen(unx)) ) {
        xstrlcpy(res, cygpwdenx(), SIZEOF(res));

        goto found;
    }
    /* Return existing absolute or relative directory:  */
    if ( NULL != (hp = cygxunx2enx_(unx)) )  {
        xstrlcpy(res, hp, SIZEOF(res));

        goto found;
    }
    for ( i = l - 1; i >= 0; i-- )  {
        char  head[NFILEN];
        char  tail[NFILEN];

        ZEROMEM(head);
        ZEROMEM(tail);

        CASRT(SIZEOF(res) == SIZEOF(head));
        CASRT(SIZEOF(res) == SIZEOF(tail));

        if ( '/' == ((unsigned char *)unx)[i] ) {
            int j = 0;

            for ( j = 0; j < i; j ++ )  {
                head[j] = unx[j];
            }
            for ( j = i + 1; j < l; j ++ )  {
                tail[j - i - 1] = unx[j];
            }
            if        ( ! *head ) {
                /* The case of an exisiting `/<something>' was
                 * handled already above.
                 */
                if ( NULL != (hp = cygxunx2enx_("/")) )  {
                    xstrlcpy(res, hp,   SIZEOF(res));
                    xstrlcat(res, "/",  SIZEOF(res));
                    xstrlcat(res, tail, SIZEOF(res));

                    goto found;
                }
            } else if ( ! *tail ) {
                if ( NULL != (hp = cygxunx2enx_(head)) ) {
                    xstrlcpy(res, hp,   SIZEOF(res));

                    goto found;
                }
            } else                {
                if ( NULL != (hp = cygxunx2enx_(head)) ) {
                    xstrlcpy(res, hp,   SIZEOF(res));
                    xstrlcat(res, "/",  SIZEOF(res));
                    xstrlcat(res, tail, SIZEOF(res));

                    goto found;
                }
            }
        }
    }

    /* When we arrive here it cannot be one of these cases:
     * - `/<something>' existing
     * - `/<something>' not existing
     * - `<something>'  existing
     */
    xstrlcpy(res, cygpwdenx(), SIZEOF(res));
    xstrlcat(res, "/", SIZEOF(res));
    xstrlcat(res, unx, SIZEOF(res));

found:
    return res;
}

/* Return absolute DOS path of an UNIX directory  */
/* Cannot fail                                    */
static CONST char *cygunx2ads P1_(CONST char *, unx)
{
    return cygenx2ads_(cygunx2enx(unx));
}

/* Cannot fail  */
static CONST char *cygdos2enx P1_(CONST char *, dos)
{
    unsigned char in[NFILEN];
    static char   res[NFILEN];
    int           len = 0;

    ZEROMEM(in);
    ZEROMEM(res);

#  if ( 0 )
    ASRT(NULL != dos);
#  else
    if ( NULL == dos )  {
        dos = "";
    }
#  endif

    xstrlcpy((char *)in, dos, SIZEOF(in));
    TO_DOS_SEP_(in);
    len = strlen((CONST char *)in);
    if ( 2 <= len       &&
         ':' == in[1]   &&
         ISALPHA(in[0]) &&
         (NULL != CYGDRIVE_ || tolower(in[0]) == tolower(cygdrvdos()))
        )   {
        CONST char  *cp = NULL;

        ASRT( NULL != (cp = cygads2enx_((CONST char *)in)) );
        xstrlcpy(res, cp, SIZEOF(res));
    } else  {
        CONST char  *cp = NULL;
        char        xpath[NFILEN];

        ZEROMEM(xpath);

        if ( '\\' == in[0] )  {
            xpath[0]  = cygdrvdos();
            xpath[1]  = ':';
            CASRT(3 <= SIZEOF(xpath));
            xstrlcat(xpath, (CONST char *)in, SIZEOF(xpath));
        } else                {
            int l = 0;

            xstrlcpy(xpath, cygpwdads(), SIZEOF(xpath) - 1 /* '\\' */);
            l = strlen(xpath);
            xpath[l]  = '\\';
            xstrlcat(xpath, (CONST char *)in, SIZEOF(xpath));
        }
        ASRT( NULL != (cp = cygads2enx_(xpath)) );
        xstrlcpy(res, cp, SIZEOF(res));
    }

    return res;
}

#  if ( 0 ) /* Not needed */
/* Return absolute DOS path of a DOS directory  */
/* Cannot fail                                  */
static CONST char *cygdos2ads P1_(CONST char *, dos)
{
    CONST char  *rc = NULL;

    ASRT( NULL != (rc = cygenx2ads_(cygdos2enx(dos))) );

    return rc;
}
#  endif  /* Not needed */


static CONST char *getdospath P1_(CONST char *, in)
#  if !USE_CYGWIN_CONV_PATH
{
    return cygunx2ads(in);
}
#  else
{
    static char dospath[NFILEN];

    ZEROMEM(dospath);
    if ( NULL == in ) {
        in  = "";
    }

    if ( 0 != cygwin_conv_path(CCP_POSIX_TO_WIN_A | CCP_ABSOLUTE, in,
                               dospath, SIZEOF(dospath)) )            {
        int errno_sv  = errno;

        TRC(("cygwin_conv_path(%s) (==> DOS): %s", in,
             strerror(errno_sv)));
        return "";
    }

    return dospath;
}
#  endif

static CONST char *getunxpath P1_(CONST char *, in)
#  if !USE_CYGWIN_CONV_PATH
{
    return cygdos2enx(in);
}
#  else
{
    static char unxpath[NFILEN];

    ZEROMEM(unxpath);
    if ( NULL == in ) {
        in  = "";
    }

    if ( 0 != cygwin_conv_path(CCP_WIN_A_TO_POSIX | CCP_ABSOLUTE, in,
                               unxpath, SIZEOF(unxpath)) )            {
        int errno_sv  = errno;

        TRC(("cygwin_conv_path(%s) (==> UNX): %s", in,
             strerror(errno_sv)));
        return "";
    }

    return unxpath;
}
#  endif

#  if !USE_CYGWIN_SPAWN

/* We need an own system as CygWin's system uses `/bin/sh' and
 * therefore does *not* work outside of a complete cygwin environment,
 * e.g. with cygwin-DLL only.
 */
/* See
 * W. Richard Stevens:  Advanced Programming in the UNIX Environment. Reading, MA, 1992.
 *                      Program 10.20
 */
static int xsystem P3_(CONST char *, shell, CONST char *, shell_flag, CONST char *, cmdstring)
{
    pid_t             pid     = 0;
    int               status  = 0;
    CONST char        *arg0   = NULL;
    int               l       = 0;
    struct sigaction  ignore;
    struct sigaction  saveintr;
    struct sigaction  savequit;
    sigset_t          chldmask;
    sigset_t          savemask;

    ZEROMEM(ignore);
    ZEROMEM(saveintr);
    ZEROMEM(savequit);
    ZEROMEM(chldmask);
    ZEROMEM(savemask);

    if ( NULL == cmdstring )  {
        if ( NULL == shell )  {
            return ( 0 );
        } else                {
            return ( !0 );
        }
    }
    for ( l = strlen(shell) - 1; l >= 0; l-- )  {
        if ( '/' == shell[l] || '\\' == shell[l] )  {
            l++;

            break;
        }
    }
    arg0  = shell + l;

    ignore.sa_handler = SIG_IGN;        /* ignore SIGINT and SIGQUIT  */
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    if ( sigaction(SIGINT, &ignore, &saveintr) < 0 )  {
        return ( -1 );
    }
    if ( sigaction(SIGQUIT, &ignore, &savequit) < 0 ) {
        return ( -1 );
    }

    sigemptyset(&chldmask);             /* now block SIGCHLD  */
    sigaddset(&chldmask, SIGCHLD);
    if ( sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0 ) {
        return(-1);
    }

    if        ( (pid = fork()) < 0) {
        status  = -1;                   /* probably out of processes  */
    } else if ( pid == 0 )          {   /* child  */
        /* restore previous signal actions & reset signal mask  */
        sigaction(SIGINT, &saveintr, NULL);
        sigaction(SIGQUIT, &savequit, NULL);
        sigprocmask(SIG_SETMASK, &savemask, NULL);

        if ( NULL != shell_flag ) {
            execl(shell, arg0, shell_flag, cmdstring, (char *) 0);
        } else                    {
            execl(shell, arg0, cmdstring, (char *) 0);
        }

        _exit(127);                     /* exec error */
    } else                          {   /* parent */
        while ( waitpid(pid, &status, 0) < 0 )  {
            if ( errno != EINTR)  {
                status  = -1;           /* error other than EINTR from
                                         * waitpid()  */

                break;
            }
        }
    }

    /* restore previous signal actions & reset signal mask  */
    if ( sigaction(SIGINT, &saveintr, NULL) < 0 )         {
        return (-1);
    }
    if ( sigaction(SIGQUIT, &savequit, NULL) < 0 )        {
        return (-1);
    }
    if ( sigprocmask(SIG_SETMASK, &savemask, NULL) < 0 )  {
        return(-1);
    }

    return WEXITSTATUS(status);
}
#  endif

#  define WIN_SHELL_C_  "/c"
static int winsystem P1_(CONST char *, cmd)
#  if !USE_CYGWIN_SPAWN
{
    int         status  = 0;
    CONST char  *shell  = NULL;

    if ( NULL == cmd )  {
        cmd = "";
    }

    if ( (shell = wingetshell()) )  {
        TRC(("winsystem(): Got shell `%s'", shell));
    } else                                {
        TRC(("winsystem(): %s", "Could not get a shell"));

        return ( -C_10 );
    }
#   if ( !0 )
    TRC(("Executing <%s>", cmd));
#   endif

    if ( 0 != (status = xsystem(shell, WIN_SHELL_C_, cmd)) )  {
        TRC(("xsystem(%s) returned %d", cmd, status));
    }

    return ( status );
}
#  else
{
    pid_t               child_pid = 0;
    int                 status    = 0;
    int                 errno_sv  = 0;
    CONST char          *shell    = NULL;
    sigset_t            mask;
    posix_spawnattr_t   attr;
    char  * /**CONST**/ sargv[] = { NULL, (char *)WIN_SHELL_C_, (char *)cmd, NULL };

    extern char **environ;

    ZEROMEM(mask);
    ZEROMEM(attr);

    if ( (shell = wingetshell()) )  {
        TRC(("winsystem(): Got shell `%s'", shell));
    } else                                {
        TRC(("winsystem(): %s", "Could not get a shell"));

        return ( -C_10 );
    }
    sargv[0]  = (char *)shell;

    if ( NULL == cmd )  {
        cmd = "";
        sargv[2] = (char *)cmd;
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

#  if ( !0 )
    TRC(("Executing <%s %s %s>", shell, WIN_SHELL_C_, cmd));
#  endif
    if ( 0 != posix_spawnp(&child_pid, shell, NULL, &attr, sargv, environ) )  {
        errno_sv  = errno;
        TRC(("Error executing <%s %s %s>, errno = %d: %s", shell, WIN_SHELL_C_,
             cmd, errno_sv, strerror(errno_sv)));
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
#  endif
#  undef WIN_SHELL_C_
# endif /* CYGWIN */

# if ( DJGPP_DOS )
static CONST char *getdospath P1_(CONST char *, in)
{
    char        temp_dospath[NFILEN];
    static char full_dospath[MAX2(NFILEN, FILENAME_MAX + 1)];

    ZEROMEM(temp_dospath);
    ZEROMEM(full_dospath);
    if ( NULL == in ) {
        in  = "";
    }

    xstrlcpy(temp_dospath, in, SIZEOF(temp_dospath));
    _fixpath(temp_dospath, full_dospath);
    MkDOSDirSep_(full_dospath);

    return full_dospath;
}

static CONST char *dosgetshell P0_()
{
    static int        FirstCall = !0;
    static CONST char *res      = NULL;
    int               Drive     = '\0';

    if ( FirstCall )  {
        CONST char  *comspec  = NULL;
        static char DOSCmd[NFILEN];

        ZEROMEM(DOSCmd);

        if ( NULL != (comspec = getenv("COMSPEC")) )  {
            xstrlcpy(DOSCmd, comspec, SIZEOF(DOSCmd));
        }

        for ( Drive = 'C'; !IsExecutable(DOSCmd) && Drive <= 'Z'; Drive++ ) {
            xsnprintf(DOSCmd, SIZEOF(DOSCmd), "%c:\\%s", Drive, "command.com");
            if ( !IsExecutable(DOSCmd) )  {
                xsnprintf(DOSCmd, SIZEOF(DOSCmd), "%c:\\DOS\\%s", Drive, "command.com");
            }
        }
        if ( !IsExecutable(DOSCmd) )  {
            xsnprintf(DOSCmd, SIZEOF(DOSCmd), "%c:\\%s",      'A', "command.com");
        }
        if ( !IsExecutable(DOSCmd) )  {
            xsnprintf(DOSCmd, SIZEOF(DOSCmd), "%c:\\DOS\\%s", 'A', "command.com");
        }
        if ( !IsExecutable(DOSCmd) )  {
            xsnprintf(DOSCmd, SIZEOF(DOSCmd), "%c:\\%s",      'B', "command.com");
        }
        if ( !IsExecutable(DOSCmd) )  {
            xsnprintf(DOSCmd, SIZEOF(DOSCmd), "%c:\\DOS\\%s", 'B', "command.com");
        }

        if ( IsExecutable(DOSCmd) ) {
            res = DOSCmd;
        } else {
            res = NULL;
        }

        FirstCall = 0;
    }

    return res;
}

/* WE MUST USE A LOWERCASE 'C' HERE:
 *
 * DJGPP checks for '/c' in dosexec.c:direct_exec():
 *
 * static int direct_exec(const char *program, char **argv, char **envp)
 * {
 *   int i, arglen;
 *   char *args, *argp;
 *   int need_quote = !__dosexec_in_system;
 *   int unescape_quote = __dosexec_in_system;
 *
 *   /o PROGRAM can be a shell which expects a single argument
 *      (beyond the /c or -c switch) that is the entire command
 *      line.  With some shells, we must NOT quote that command
 *      line, because that will confuse the shell.
 *
 *      The hard problem is to know when PROGRAM names a shell
 *      that doesn't like its command line quoted...  o/
 *
 *   if (need_quote
 *       && argv[1] && !strcmp (argv[1], "/c")
 *       && argv[2] && !argv[3]
 *       && _is_dos_shell (program))
 *     need_quote = 0;
 *
 * ...
 */
#  define DOS_SHELL_C_  "/c"
#  define WIN_SHELL_C_  "/c"

#  if ( USE_SPAWN_FOR_SYSTEM )

static int dossystem P1_(CONST char *, cmd)
{
    int                 status      = 0;
    CONST char          *dos_shell  = NULL;
    CONST char          *win_shell  = NULL;
    int                 errno_sv    = 0;
    char  * /**CONST**/ dos_argv[]  = { NULL, (char *)DOS_SHELL_C_, (char *)cmd, NULL };
    char  * /**CONST**/ win_argv[]  = { NULL, (char *)WIN_SHELL_C_, (char *)cmd, NULL };

    win_shell = wingetshell();
    dos_shell = dosgetshell();
    if ( NULL == win_shell && NULL == dos_shell ) {
        TRC(("%s", "Could not get a shell"));

        return ( -C_10 );
    }
    if ( NULL == cmd )  {
        cmd = "";
        win_argv[2] = (char *)cmd;
        dos_argv[2] = (char *)cmd;
    }

    dos_argv[0] = (char *)dos_shell;
    win_argv[0] = (char *)win_shell;
#   if ( !0 )
    if        ( win_shell ) {
        TRC(("Executing <%s %s %s>", win_shell, WIN_SHELL_C_, cmd));
    } else if ( dos_shell ) {
        TRC(("Executing <%s %s %s>", dos_shell, DOS_SHELL_C_, cmd));
    } else                  {
        ASRT(!"IMPOSSIBLE");
    }
#   endif
    if        ( win_shell ) {
        status    = spawnv(P_WAIT, win_shell, win_argv);
        errno_sv  = errno;
    } else if ( dos_shell ) {
        status    = spawnv(P_WAIT, dos_shell, dos_argv);
        errno_sv  = errno;
    } else                  {
        ASRT(!"IMPOSSIBLE");
    }

    if        ( 0 > status )        {
        if        ( win_shell ) {
            TRC(("Error executing <%s %s %s>, errno = %d: %s", win_shell, WIN_SHELL_C_,
                 cmd, errno_sv, strerror(errno_sv)));
        } else if ( dos_shell ) {
            TRC(("Error executing <%s %s %s>, errno = %d: %s", dos_shell, DOS_SHELL_C_,
                 cmd, errno_sv, strerror(errno_sv)));
        } else                  {
            ASRT(!"IMPOSSIBLE");
        }

        return ( (-1) * errno_sv );
    } else if ( EINTR == errno_sv ) {
        int signo = 0;

        signo = ( (status & 0xFF00) >> 8 );
        if        ( win_shell ) {
            TRC(("RC <%s %s %s>: Got signal %d, errno = %d: %s",
                 win_shell, WIN_SHELL_C_, cmd, signo, errno_sv,
                 strerror(errno_sv)));
        } else if ( dos_shell ) {
            TRC(("RC <%s %s %s>: Got signal %d, errno = %d: %s",
                 dos_shell, DOS_SHELL_C_, cmd, signo, errno_sv,
                 strerror(errno_sv)));
        } else                  {
            ASRT(!"IMPOSSIBLE");
        }

        return ( (-1) * signo );
    } else                          {
        if        ( win_shell ) {
            TRC(("RC <%s %s %s>: Status = %d",
                 win_shell, WIN_SHELL_C_, cmd, status));
        } else if ( dos_shell ) {
            TRC(("RC <%s %s %s>: Status = %d",
                 dos_shell, DOS_SHELL_C_, cmd, status));
        } else                  {
            ASRT(!"IMPOSSIBLE");
        }

        return ( 0xFF & status );
    }
}

#  else

static int dossystem P1_(CONST char *, cmd)
{
    int                 status      = 0;
    CONST char          *win_shell  = NULL;
    CONST char          *dos_shell  = NULL;
    char                cmdstr[NFILEN];

    ZEROMEM(cmdstr);

    win_shell = wingetshell();
    dos_shell = dosgetshell();
    if ( NULL == cmd )  {
        cmd = "";
    }

    if        ( win_shell ) {
        xsnprintf(cmdstr, SIZEOF(cmdstr), "%s %s \"%s\"", win_shell, WIN_SHELL_C_, cmd);
    } else if ( dos_shell ) {
        xsnprintf(cmdstr, SIZEOF(cmdstr), "%s %s %s", dos_shell, DOS_SHELL_C_, cmd);
    } else                  {
        TRC(("%s", "Could not get a shell"));

        return ( -C_10 );
    }
#   if ( !0 )
    TRC(("Executing <%s>", cmdstr));
#   endif
    __system_flags &= ~__system_use_shell;

    if ( 0 != (status = system(cmdstr)) ) {
        TRC(("system(%s) returned %d", cmdstr, status));
    }

    return ( status );
}

#  endif  /* USE_SPAWN_FOR_SYSTEM */

#  undef WIN_SHELL_C_
#  undef DOS_SHELL_C_

# endif /* DJGPP_DOS */

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
# elif ( DJGPP_DOS )
    status = dossystem(cmd) == 0;
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
# if ( CYGWIN || DJGPP_DOS )
    char        dossh[NFILEN];

    ZEROMEM(dossh);
# endif

    /* Don't allow this command if restricted */
    if ( restflag ) {
        return ( resterr() );
    }

    /* Get shell path if envvar `SHELL' is set: */
    if ( NULL != (sh = getenv("SHELL")) ) {
# if ( CYGWIN || DJGPP_DOS )  /* e.g. in a cygwin term  */
        xstrlcpy(dossh, sh, SIZEOF(dossh));
        NormalizePathDOS(dossh);
        sh  = dossh;
# endif
    } else                                {
# if ( LINUX )
        sh = "/bin/bash";
# elif ( SOLARIS )
        sh = "/usr/bin/ksh";
# elif ( CYGWIN )
        sh = wingetshell();
# elif ( DJGPP_DOS )
        /* First try Windows shell --- are we running in the 16-Bit
         * Windows subsystem?
         */
        if ( NULL == (sh = wingetshell()) ) {
            sh = dosgetshell();
        }
# else
        sh = "/bin/sh";
# endif /* LINUX */
    }

    if ( IsExecutable(sh) ) {
        /* Do shell */
        TRC(("spawncli: Opening `SHELL=%s'", sh));

        return ( callout(sh) );
    } else {
        TRC(("spawncli: `SHELL=%s' is not executable", sh));

        return FALSE;
    }
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
    if        ( 0 > umc_stat(dir, &sb) )  {
        return FALSE;
    } else if ( S_ISDIR(sb.st_mode) )     {
        return TRUE;
    } else                                {
        return FALSE;
    }
}

static int  IsFil P1_(CONST char *, fil)
{
    struct stat sb;

    ZEROMEM(sb);

    if ( !fil || !*fil )  {
        return FALSE;
    }
    if        ( 0 > umc_stat(fil, &sb) )  {
        return FALSE;
    } else if ( S_ISREG(sb.st_mode) )     {
        return TRUE;
    } else                                {
        return FALSE;
    }
}

static int IsAccessable P2_(CONST char *, d, CONST char *, mode)
{
/* This is *not* perfect: `umc_access()' only checks for uid/gid but  */
/* not for euid/egid.                                                 */
    int mflag = 0;

    if        ( NULL == d )     {
        return FALSE;
    } else if ( NULL == mode )  {
        return TRUE;
    }

    if ( strchr(mode, 'r') || strchr(mode, 'R') ) {
        mflag |= R_OK;
    }
    if ( strchr(mode, 'w') || strchr(mode, 'W') ) {
        mflag |= W_OK;
    }
    if ( strchr(mode, 'x') || strchr(mode, 'X') ) {
        mflag |= X_OK;
    }

    if ( 0 == umc_access(d, mflag) )  {
        return TRUE;
    } else                            {
        return FALSE;
    }
}

static int  IsExecutable P1_(CONST char *, file)
{
    return ( IsFil(file) && IsAccessable(file, "rx") );
}

#if ( TERMINAL_NOBLOCK_READ == USE_TERMINAL_READX )
static int  rdstdin P1_(int, getnread)
{
# define BUFSZ_rdstdin_   (64)
# define INC_rdstdin_(x)  do {                  \
    if ( (BUFSZ_rdstdin_ - 1) == (x) )  {       \
        (x) = 0;                                \
    } else {                                    \
        (x)++;                                  \
    }                                           \
} while ( 0 )
# define NREAD_rdstdin_   ( ((wpos) >= (rpos))? \
    ((wpos) - (rpos))                           \
        :                                       \
    (BUFSZ_rdstdin_ - (rpos) + (wpos))          \
)
    static unsigned char ringbuf[BUFSZ_rdstdin_];
    static unsigned char readbuf[BUFSZ_rdstdin_];
    static int  wpos  = 0;
    static int  rpos  = 0;

    int retval  = '\0';
    int rc      = 0;

    if ( getnread ) {
        return NREAD_rdstdin_;
    }

    if ( 0 < NREAD_rdstdin_ )  {
        retval  = ringbuf[rpos];
        INC_rdstdin_(rpos);

        return retval;
    }

    if ( 0 >= (rc  = read(0, readbuf, BUFSZ_rdstdin_)) ) {
        return (-1);
    } else                                      {
        int i = 0;

        for ( i = 0; i < rc; i++ )  {
            ringbuf[wpos] = readbuf[i];
            INC_rdstdin_(wpos);
        }

        retval  = ringbuf[rpos];
        INC_rdstdin_(rpos);

        return retval;
    }
# undef NREAD_rdstdin_
# undef INC_rdstdin_
# undef BUFSZ_rdstdin_
}
#endif  /* TERMINAL_NOBLOCK_READ */

/* Get a directory for temporary files: Cannot fail.  */
static char *gettmpdir P0_()
{
    static char *res  = NULL;

    if ( NULL == res )  {
        static char tmpd[NFILEN];
        int         l   = 0;

        /**ZEROMEM(tmpd);**/

# define CHKDIR_(d)  do  {                                            \
    /**ZEROMEM(tmpd);**/                                              \
    if ( 0 < xstrlcpy(tmpd, (d), SIZEOF(tmpd)) )  {                   \
        TRC(("gettmpdir(): Testing <%s> as candidate", tmpd));        \
        NormalizePathUNX(tmpd);                                       \
        if ( IsDir(tmpd) )  {                                         \
            if ( IsAccessable(tmpd, "rwx") )  {                       \
                goto found;                                           \
            } else                  {                                 \
                TRC(("gettmpdir(): <%s> is not a accessable", tmpd)); \
            }                                                         \
        } else              {                                         \
            TRC(("gettmpdir(): <%s> is not a directory", tmpd));      \
        }                                                             \
    }                                                                 \
} while ( 0 )

        /* Test environment variables UETMPDIR, TMPDIR, TMP, and TEMP:  */
        CHKDIR_(getenv("UETMPDIR"));
        CHKDIR_(getenv("TMPDIR"));
        CHKDIR_(getenv("TMP"));
        CHKDIR_(getenv("TEMP"));

        /* Test the directory used by `tmpnam()': */
# ifdef P_tmpdir
        CHKDIR_(P_tmpdir);
# else
        CHKDIR_("/tmp");
#  if ( CYGWIN || DJGPP_DOS )
        CHKDIR_("C:/TEMP");
        CHKDIR_("C:/TMP");
#  endif
# endif

        /* Last resort: Use current directory:  */
        xstrlcpy(tmpd, ".", SIZEOF(tmpd));

# undef CHKDIR_

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
# if ( !0 )
        TRC(("gettmpdir(): Found <%s>", res));
# endif
    }

    return res;
}

/* gettmpfname:
 *
 * Return in a static buffer the name of a temporary currently not
 * existing file name containing ident in its name.
 */
CONST char *gettmpfname P1_(CONST char *, ident)
{
    char        str[NFILEN];
    int         i     = 0;
    static int  seed  = 0;
    static char res[NFILEN];
    char        l_ident[C_1 + 1]  = "x";

    ZEROMEM(str);
    ZEROMEM(res);

    xstrlcpy(str, gettmpdir(),                SIZEOF(str));
    /* The filename part should have DOS 6.0 format --- remind DOS's
     * 126 byte command line limit
     */
    xstrlcat(str, "/ue",                      SIZEOF(str));
    if ( NULL != ident )  {
        int i = 0;

        for ( i = 0; i < SIZEOF(l_ident) - 1 && ident[i]; i++ ) {
            l_ident[i]  = ident[i];
        }
        mklower(l_ident);
    }
    xstrlcat(str, l_ident,                      SIZEOF(str));
    xstrlcat(str, nni2s36_(getpid() % (C_36)),  SIZEOF(str));

    for ( i = 0; i < (C_36 * C_36); i++ ) {
        struct stat sb;

        ZEROMEM(sb);

        xstrlcpy(res, str,                      SIZEOF(res));
        xstrlcat(res, nni2s36_((seed + i) % (C_36 * C_36)),
                 SIZEOF(res));
        if ( 0 > umc_stat(res, &sb) ) {
            if ( ENOENT == errno ) {            /* found */
                seed = (seed + i + 1) % (C_36 * C_36);

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
 * redirected. If it is NULL or an empty string, stderr is not redirected.
*/
static int LaunchPrg P4_(const char *,  Cmd,
                         const char *,  InFile,
                         const char *,  OutFile,
                         const char *,  ErrFile)
{
    char  FullCmd[NLINE];
    char  lInFile[NFILEN];
    char  lOutFile[NFILEN];
    char  lErrFile[NFILEN];

    ZEROMEM(FullCmd);
    ZEROMEM(lInFile);
    ZEROMEM(lOutFile);
    ZEROMEM(lErrFile);

    if ( !Cmd ) {
        return FALSE;
    }

    if ( !InFile || !*InFile ) {
        XSTRCPY(lInFile, NULL_DEVICE);
    } else  {
# if ( CYGWIN || DJGPP_DOS )
        XSTRCPY(lInFile, getdospath(InFile));
# else
        XSTRCPY(lInFile, InFile);
# endif
    }
    if ( !OutFile || !*OutFile ) {
        XSTRCPY(lOutFile, NULL_DEVICE);
    } else  {
# if ( CYGWIN || DJGPP_DOS )
        XSTRCPY(lOutFile, getdospath(OutFile));
# else
        XSTRCPY(lOutFile, OutFile);
# endif
    }
    if ( !ErrFile || !*ErrFile ) {
        XSTRCPY(lErrFile, NULL_DEVICE);
    } else  {
# if ( CYGWIN || DJGPP_DOS )
        XSTRCPY(lErrFile, getdospath(ErrFile));
# else
        XSTRCPY(lErrFile, ErrFile);
# endif
    }

# if ( CYGWIN  )
    xsnprintf(FullCmd, SIZEOF (FullCmd),
              "%s < %s > %s 2>%s",
              Cmd, lInFile, lOutFile, lErrFile);
# elif ( DJGPP_DOS )
    if ( NULL != wingetshell() )  {
        xsnprintf(FullCmd, SIZEOF (FullCmd),
                  "%s < %s > %s 2>%s",
                  Cmd, lInFile, lOutFile, lErrFile);
    } else {
        xsnprintf(FullCmd, SIZEOF (FullCmd),
                  "%s < %s > %s",
                  Cmd, lInFile, lOutFile);
    }
# else
    xsnprintf(FullCmd, SIZEOF (FullCmd),
              "( %s ) < %s > %s 2>%s",
              Cmd, lInFile, lOutFile, lErrFile);
# endif

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
    char        Command[NLINE];
    int         Result          = 0;
    char        tmpnam[NFILEN];
    char        InFile[NFILEN];
    char        OutFile[NFILEN];
    char        bname[NFILEN]   = "command";
    CONST char  *cp             = NULL;
    BUFFER      *bp             = NULL;

    ZEROMEM(Command);
    ZEROMEM(tmpnam);
    ZEROMEM(InFile);
    ZEROMEM(OutFile);

    /* Don't allow this command if restricted */
    if ( restflag ) {
        return resterr();
    }

    /* Get pipe-in command */
    if ( !mlreply("@", Command, NLINE) ) {
        return FALSE;
    }

    if ( NULL != ( cp = gettmpfname("i") ) )  {
        XSTRCPY(InFile, cp);
    } else {
        return FALSE;
    }
    if ( NULL != ( cp = gettmpfname("o") ) )  {
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
        umc_unlink(InFile);
        sleep(MLWAIT);

        return FALSE;
    }
    /* Reset file name */
    XSTRCPY(bp->b_fname, tmpnam);

# if   ( 0 )    /* Activate multiple "command" buffers  */
    makename(bname, OutFile);           /* New buffer name. */
# elif ( !0 )
    if ( !makecmdbname(bname, SIZEOF (bname), Command, "@Cmd") ) {
        umc_unlink(InFile);

        return FALSE;
    }
# endif

    /*-find the "command" buffer */
    if ( ( bp = bfind (bname, FALSE, 0) ) != NULL ) {
        /*-make sure the contents can safely be blown away */
        if ( bp->b_flag & BFCHG ) {
            if ( mlyesno (TEXT32) != TRUE ) {
                umc_unlink(InFile);

                return FALSE;
            }
        }
        /* discard changes */
    } else if ( ( bp = bfind (bname, TRUE, 0) ) == NULL ) {
        mlwrite (TEXT137);
        /* cannot create buffer */
        umc_unlink(InFile);
        sleep(MLWAIT);

        return FALSE;
    }

    if ( !( Result = LaunchPrg (Command, InFile, OutFile, NULL) ) ) {
        mlwrite (TEXT3);
        /* [execution failed] */
        umc_unlink(InFile);
        umc_unlink(OutFile);
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
            umc_unlink(InFile);
            umc_unlink(OutFile);

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
        umc_unlink(InFile);
        umc_unlink(OutFile);
    }

    return TRUE;
}

/** Filter buffer through command **/
int f_filter P2_(int, f, int, n)
/* f: Flags           */
/* n: Argument count  */
{
    char        line[NLINE];
    int         s   = 0;
    BUFFER      *bp = NULL;
    CONST char  *cp = NULL;
    char        tmpnam[NFILEN];
    char        InFile[NFILEN];
    char        OutFile[NFILEN];

    ZEROMEM(line);
    ZEROMEM(tmpnam);
    ZEROMEM(InFile);
    ZEROMEM(OutFile);

    if ( NULL != ( cp = gettmpfname("i") ) )  {
        XSTRCPY(InFile, cp);
    } else {
        return FALSE;
    }
    if ( NULL != ( cp = gettmpfname("o") ) )  {
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
        umc_unlink(InFile);
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
    umc_unlink(InFile);
    umc_unlink(OutFile);

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
        if ( !(dp = readdir(dirptr)) )  {
            return (NULL);
        }

        xstrlcpy(nameptr, dp->d_name, SIZEOF(rbuf) - (nameptr - rbuf));
        /* Check to make sure we skip all weird entries except
         * regular files and directories:
         */
    } while ( !( 0 == umc_stat(rbuf, &fstat) &&
                  (
                    (fstat.st_mode & S_IFMT) == S_IFDIR
                      ||
                    (fstat.st_mode & S_IFMT) == S_IFREG
                  )
               )
            );

    /* if this entry is a directory name, say so */
    if ( (fstat.st_mode & S_IFMT) == S_IFDIR )  {
        XSTRCAT(rbuf, DIRSEPSTR);
    }

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

/*======================================================================
 * CYGWIN, DJGPP_DOS need wrappers for some (but not all) functions with file
 * name arguments to be able to work with DOS and UNIX style file names:
 * - access(), stat() only work with UNIX style file names.
 * - fopen(), ... work with DOS and UNIX style file names.
 *====================================================================*/
/* GETPATHUNX:
 *
 * Return UNIX style path.
 */
CONST char *GetPathUNX P1_(CONST char *, path)
{
    static char new_path[NFILEN];

    ZEROMEM(new_path);
    xstrlcpy(new_path, path, SIZEOF(new_path));
    NormalizePathUNX(new_path);

    return (CONST char *)&new_path[0];
}

int unx_access_ P2_(CONST char *, path, int, mode)
{
    /* It is OK here to *not* immediatley copy GetPathUNX's internal
     * static buffer, because we *know* that `access' won't call
     * GetPathUNX
     */
    return access(GetPathUNX(path), mode);
}

int unx_rename_ P2_(CONST char *, from, CONST char *, to)
{
    char  new_from[NFILEN];
    char  new_to[NFILEN];

    ZEROMEM(new_from);
    ZEROMEM(new_to);

    xstrlcpy(new_from, GetPathUNX(from), SIZEOF(new_from));
    xstrlcpy(new_to,   GetPathUNX(to),   SIZEOF(new_to));

    return rename(new_from, new_to);
}

int unx_stat_ P2_(CONST char *, path, struct stat *, sb)
{
    /* It is OK here to *not* immediatley copy GetPathUNX's internal
     * static buffer, because we *know* that `stat' won't call
     * GetPathUNX
     */
    return stat(GetPathUNX(path), sb);
}

int unx_unlink_ P1_(CONST char *, path)
{
    /* It is OK here to *not* immediatley copy GetPathUNX's internal
     * static buffer, because we *know* that `unlink' won't call
     * GetPathUNX
     */
    return unlink(GetPathUNX(path));
}


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
