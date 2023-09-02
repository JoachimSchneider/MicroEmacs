/*======================================================================
 * ESTRUCT: Structure and preprocesser defined for MicroEMACS 4.01
 *
 *          (C)Copyright 1997 by Daniel Lawrence
 *          written by Daniel Lawrence
 *          based on code by Dave G. Conroy,
 *          Steve Wilhite and George Jones
 *====================================================================*/

/*====================================================================*/
#ifndef ESTRUCT_H_
#define ESTRUCT_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#ifdef  LATTICE
# undef  LATTICE        /* don't use their definitions...use ours     */
#endif
#ifdef  MSDOS
# undef  MSDOS
#endif
#ifdef  WINNT
# undef  WINNT
#endif
#ifdef  AMIGA
# undef  AMIGA
#endif
#ifdef  EGA
# undef  EGA
#endif
#ifdef  CTRLZ
# undef  CTRLZ
#endif

/*
 *      Program Identification.....
 *
 * PROGNAME should always be MicroEMACS for a distribution unmodified *
 * version. People using MicroEMACS as a shell for other products
 * should change * this to reflect their product. Macros can query this
 * via the $progname variable
 */
#define PROGNAME        "MicroEMACS"
#define VERSION         "5.00"

/*      Machine/OS definitions                                        */
/*===== [Set one of these!!] ======================================== */

#define AMIGA   0                     /* AmigaDOS                     */
#define AOSVS   0                     /* Data General AOS/VS          */
#define AUX     0                     /* Apple UNIX for Macintosh     */
#define AIX     0                     /* IBM UNIX for various machines*/
#define AIX5    0                     /* IBM UNIX newer rs6000        */
#define AVIION  0                     /* Data General AViiON          */
#define BSD     0                     /* UNIX BSD 4.2 and ULTRIX      */
#define FINDER  0                     /* Macintosh OS                 */
#define FREEBSD 0                     /* FREEBSD 386 version 2 or +   */
#define LINUX   1                     /* Linux                        */
#define HPUX8   0                     /* HPUX HP 9000 ver 8 or less   */
#define HPUX9   0                     /* HPUX HP 9000 ver 9           */
#define MPE     0                     /* HP MPE/XL                    */
#define MSDOS   0                     /* MS-DOS                       */
#define MV_UX   0                     /* Data General MV/UX (Eclipse) */
#define OPENBSD 0                     /* OPENBSD 386                  */
#define OS2     0                     /* Microsoft or IBM OS/2        */
#define SMOS    0                     /* Supermax UNIX System V       */
#define SOLARIS 0                     /* SUN Solaris (SYSV)           */
#define SUN     0                     /* SUN v4.0                     */
#define TOS     0                     /* ST520, TOS                   */
#define USG     0                     /* UNIX system V                */
#define VMS     0                     /* VAX/VMS                      */
#define WINNT   0                     /* MS-Win NT                    */
#define WINXP   0                     /* Windows XP/Visual studio 2008*/
#define WMCS    0                     /* Wicat's MCS                  */
#define XENIX   0                     /* IBM-PC SCO XENIX             */


#define IS_UNIX()       ( AIX || AIX5 || AUX || AVIION || BSD       \
                          || FREEBSD || HPUX8 || HPUX9 || LINUX     \
                          || OPENBSD || SMOS || SOLARIS || SUN      \
                          || USG || XENIX )
#define IS_POSIX_UNIX() ( IS_UNIX()                                 \
                          && !( USG || AIX || AUX || SMOS || HPUX8  \
                                || HPUX9 || SUN || XENIX ) )


/*      Compiler definitions                                          */
/*===== [Set one of these!!] ======================================== */
#define ALCYON  0         /* ALCYON Atari ST compiler                 */
#define AZTEC   0         /* Aztec C 4.00e ONLY for the amiga now...  */
#define DGC     0         /* Data General AOS/VS C...                 */
#define GCC     1         /* the GNU C compiler                       */
#define IC      0         /* Rational Systems Instant C               */
#define LATTICE 0         /* Lattice 2.14 through 3.0 compilers       */
#define MSC     0         /* MicroSoft C compile version 3 and up     */
#define MWC     0         /* Mark Williams C                          */
#define TURBO   0         /* Turbo C and Borland C++ under MSDOS      */
#define UNIX    0         /* a standard UNIX compiler (cc)            */
#define ZTC     0         /* Zortech C/C++ 1.02 thru 2.10 under MSDOS */

/*      Machine stack growth direction.                               */
/*===== [Set one of these!!] ======================================== */
/*      data general mv/eclipse series stack grows up.                */
/*      dec vax series stack grows down... got it???                  */

#define STACK_GROWS_UP  0

/*      Debugging options                                             */
#define RAMSIZE         0 /* dynamic RAM memory usage tracking        */
#define RAMSHOW         0 /* auto dynamic RAM reporting               */
#define RAMTRCK         0 /* send debug info to MALLOC.DAT            */
#define DEBUG_SEARCH    0 /* pop some search info on patterns         */

/*      Special keyboard/network definitions                          */

#define ATKBD   0     /* AT-style keyboard with F11, F12 & grey keys  */
#define WANGPC  0     /* WangPC - mostly escape sequences             */
#define VT100   1     /* Handle VT100 style keypad - NOT VMS.         */
#define KEYPAD  0     /* VMS - turn on and off application            */
                      /* keypad automatically                         */
#define XONDATA 0     /* VMS - set to force /NOTTSYNC/NOHOSTSY        */
#define RMSIO   0     /* VMS - skip the rotten C runtime and          */
                      /* use RMS directly                             */
#define OPTMEM  0     /* VMS 5.0 and up - use a less standard         */
                      /* but more efficient memory allocator          */

/*      Terminal Output definitions                                   */
/*===== [If not on UNIX: Set one of these!!] =========================*/

#define ANSI    0           /* ANSI escape sequences                  */
#define DASHER  0           /* DG Dasher 2xx/4xx crts                 */
#define DG10    0           /* Data General system/10                 */
#define FMR     0           /* Fujitsu FMR series driver              */
#define HP110   0           /* HP110 screen driver                    */
#define HP150   0           /* HP150 screen driver                    */
#define I55     0           /* IBM PS55 DOS J4.0/V                    */
#define IBMPC   0           /* IBM-PC CGA/MONO/EGA/VGA drvr           */
#define MAC     0           /* Macintosh                              */
#define NEC     0           /* NEC-9801VM driver                      */
#define OS2NPM  0           /* OS/2 non-Presentation Mgr.             */
#define SMG     0           /* SMG library on VMS                     */
#define ST52    0           /* Atari 520/1040ST screen                */
#define TIPC    0           /* TI Profesional PC driver               */
#define VT52    0           /* VT52 terminal (Zenith).                */
#define NTCON   0           /* Windows NT console                     */
#define XPCON   0           /* windows XP console app                 */
#define XVT     0           /* XVT windowing system                   */
#define Z309    0           /* Zenith 100 PC family driver            */

/*      Windowing system style (pick one)                             */

#define WINDOW_TEXT     1   /* [default] Text mode                    */
#define WINDOW_MSWIN    0   /* MicroSoft Windows                      */
#define WINDOW_MSWIN32  0   /* MicroSoft Windows 32 bit API           */
#define WINDOW_X        0   /* X/Unix                                 */

/*      Language text options   (pick one)                            */

#define ENGLISH 1           /* [default]                              */
#define FRENCH  0
#define SPANISH 0
#define GERMAN  0
#define DUTCH   0
#define PLATIN  0           /* Pig Latin                              */
#define JAPAN   0
#define LATIN   0           /* real Latin                             */

/*      Configuration options                                         */

/* Does not work on SOLARIS:    */
#define TYPEAH  1   /* type ahead causes update to be skipped         */
#define LOGFLG  0   /* send all executed commands to EMACS.LOG        */
#define VISMAC  0   /* update display during keyboard macros          */
#define CTRLZ   0   /* add a ^Z at end of files under MSDOS only      */
#define CLEAN   0   /* de-alloc memory on exit                        */
#define CALLED  0   /* is emacs a called subroutine? or stand alone   */

#define REVSTA  1   /* Status line appears in reverse video           */
#define COLOR   1   /* color commands and windows                     */

#define FILOCK  1   /* generic file locking under unix                */
#define ISRCH   1   /* Incremental searches like ITS EMACS            */
#define FLABEL  0   /* function key label code [HP150]                */
#define CRYPT   1   /* file encryption enabled?                       */
#define MAGIC   1   /* include regular expression matching?           */
#define MOUSE   1   /* Include routines for mouse actions             */
#define NOISY   1   /* Use a fancy BELL if it exists                  */
#define CTAGS   1   /* include vi-like tagging?                       */
#define SPEECH  0   /* spoken EMACS, for the sight impared [not ready]*/
#define VARARG  1   /* use varargs.h/stdarg.h for mlwrite()           */

#if     XVT
# undef  COLOR
# define COLOR   1  /* overide this to be TRUE for XVT always         */
#endif

/*      Character set options                                         */
/*===== [Set one of these!!] ======================================== */
#define ASCII   1   /* always using ASCII char sequences for now      */
#define EBCDIC  0   /* later IBM mainfraim versions will use EBCDIC   */

/*      Settings related to tracing/debugging MicroEmacs              */
/*====================================================================*/
/* Fix overwrites of the line data instead of bailing out:            */
/* -- All errors in the line code seem to be fixed now (2023-07-25),  */
/* -- but if you want to find new errors set this to `0'.             */
#define REPAIR_CODE_LINE      (!0)
/* See eproto.h: If TRC_FILE_ENVVAR is defined generate trace output  */
/* into this file:                                                    */
#define UEMACS_TRC            (!0)
#define TRC_FILE_ENVVAR       "EMACS_TRC_FILE"


/*=================================================================== */


/*===== handle constant and voids properly ===========================*/
/* The `VOID' definitions (to `void' or `') only affect the function  */
/* return types *not* the argument types: These are handled by the    */
/* P#_-macros in eproto.h.                                            */
/*====================================================================*/

#if     VMS && defined(VAXC)
# define CONST      readonly
# define VOID       void
# define VOIDCAST   (void)
  typedef void *    voidp_;
# define NOSHARE    noshare
#elif   AOSVS
# define CONST      $shared $align(1)   /* fake a  const */
# define VOID
# define VOIDCAST
  typedef char *    voidp_;
  /* attempt to optimize read/write vars. */
# define NOSHARE    $low32k $align(1)
#elif  __STDC__ || IS_UNIX() || MSC || TURBO || GCC   \
  || (AMIGA && LATTICE)
# define CONST      const
# define VOID       void
# define VOIDCAST   (void)
  typedef void *    voidp_;
# define NOSHARE
#else
# define CONST
# if     IC
#   define VOID     void
    typedef void *  voidp_;
# else
#   define VOID
#   define VOIDCAST
    typedef char *  voidp_;
# endif
# define NOSHARE
#endif
#define VOIDP voidp_


/*===== System dep. library redefinitions, structures and includes ===*/

/*===== multibyte character support? =================================*/

#if     NEC || FMR || I55
# define DBCS    1  /* double byte character sets enabled */
# define INSDEL  1  /* use insert/delete line display optimizations */

/* define DBCS prefix bytes */
# define is2char(ch) ( ( (ch&0xff) >= 0x81 && (ch&0xff) <= 0x9f ) || \
                       ( (ch&0xff) >=0xe0 && (ch&0xff) <= 0xfc ) )

#else
# define DBCS    0
# define INSDEL  0
#endif

/*      Can we catch the SIGWINCH (the window size change signal)? */

#if     IS_UNIX()
# define HANDLE_WINCH    1
#else
# define HANDLE_WINCH    0
#endif

/*      Prototypes in use?      */

#if     MSC || TURBO || IC || VMS || GCC || ZTC
# define PROTO   1
#else
# define PROTO   0
#endif

/*
 *      the following define allows me to initialize unions...
 *      otherwise we make them structures (like the keybinding table)
 */
#if     __STDC__ || MSC || TURBO || IC || ZTC
# define ETYPE   union
#else
# define ETYPE   struct
#endif

/* Instant C can't do stat()s. Arrrg. No file locking for you */
#if     IC && MSDOS
# undef FILOCK
# define FILOCK  0
#endif

/* Mark Williams/Atari has no standard or varargs or directory functions */
#if     TOS & MWC
# undef VARARG
# define VARARG  0
# undef  FILOCK
# define FILOCK  0
#endif

/* MS-Windows */

#if     WINNT || WINDOW_MSWIN || WINDOW_MSWIN32
# if     WINDOW_MSWIN32
#  undef  WINDOW_MSWIN
#  define WINDOW_MSWIN    1
# endif
# if     WINDOW_MSWIN && (WINNT || WINXP)
#  undef  WINDOW_MSWIN32
#  define WINDOW_MSWIN32  1
# endif
# if 1
#  undef  MAC   /* Mac conflicts with a definition used by rpc.h */
#  undef  VOID  /* windows.h will wind up defining this when compiled as a
                 * console app */
#  include <windows.h>  /* --------- Huge include file here !!! ---------*/
#  ifndef VOID
#   define VOID void /* Redefine, incase we are compiled as a Windows app */
#  endif
# endif
# if     NTCON
#  include <WinCon.h>
#  include <stdio.h>
#  include <dos.h>
# endif

# undef NEAR
# define NEAR
# define DNEAR
# if     MSC || IC
#  undef CDECL
#  define CDECL   __cdecl
/* dummy size for unsized extern arrays to avoid silly DGROUP fixup:  */
/* I don't like this hack --- JES, 2023-08-13 --- TODO                */
/*  sizeof(.) would be wrong, but compilation on other platforms      */
/*  shows, that this does not happen as sizeof gives a compile error  */
/*  with incomplete array types.                                      */
#  define DUMMYSZ 1
# else
#  if     TURBO
#   define DUMMYSZ  /* nothing */
#  else
#   define CDECL   _cdecl/* ZTC */
#   define DUMMYSZ  /* nothing */
#  endif
# endif

# if     (WINNT || WINXP) && !WINDOW_MSWIN
#  define EXPORT /* Windows NT doesn't like this */
# endif

# if     WINDOW_MSWIN
#  undef  TYPEAH
#  define TYPEAH  0 /* typeahead is handled at the term driver level */
#  undef  CALLED
#  define CALLED  1 /* under MS Windows, "main" resides in the sys driver */
#  if     WINNT || WINXP
#   define EXPORT /* Windows NT doesn't like this */
#  elif   MSC
#   define EXPORT  __export
#  else
#   define EXPORT  _export/* Fine for TURBO and ZTC */
#  endif
# endif

#else /* !(WINNT || WINDOW_MSWIN || WINDOW_MSWIN32) */

/* neither Windows NT nor MS-Windows */

# define DUMMYSZ    /* nothing */

# if MSDOS & (TURBO | MSC | TIPC)
#  define NEAR
#  define DNEAR
#  define PASCAL pascal
#  define CDECL cdecl
# else
#  if MSDOS & ZTC
#   define NEAR
#   define DNEAR
#   define PASCAL _pascal
#   define CDECL _cdecl
#  else
#   define NEAR
#   define DNEAR
#   define PASCAL
#   define CDECL
#  endif
# endif

#endif

#if     TURBO
# include      <dos.h>
# include      <mem.h>
# undef peek
# undef poke
# define       peek(a, b, c, d)   movedata(a, b, FP_SEG(c), FP_OFF(c), d)
# define       poke(a, b, c, d)   movedata(FP_SEG(c), FP_OFF(c), a, b, d)
#endif

#if     IC
# include      <dos.h>
# undef peek
# undef poke
# define       peek(a, b, c, d)   movedata(a, b, FP_SEG(c), FP_OFF(c), d)
# define       poke(a, b, c, d)   movedata(FP_SEG(c), FP_OFF(c), a, b, d)
#endif

#if     LATTICE & MSDOS
/*
 * you may have to remove this one definition with LATTICE version 3.2
 * and above
 */
# define unsigned
#endif

#if     IC
# define inp     inportb
# define outp    outportb
# define intdos(a, b)    int86(33, a, b)
#endif

#if     AZTEC
# undef  putc
# undef  getc
# if     MSDOS
#  define getc    a1getc
#  define int86   sysint
#  define intdos(a, b)    sysint(33, a, b)
#  define inp     inportb
#  define outp    outportb
# else
#  define getc    agetc
# endif
# define putc    aputc

struct XREG {
    unsigned ax, bx, cx, dx, si, di, ds, es;
};

struct HREG {
    char al, ah, bl, bh, cl, ch, dl, dh, d1, d2, e1, e2;
};

union REGS {
    struct XREG x;
    struct HREG h;
};

struct SREGS {
    unsigned cs, ss, ds, es;
};
#endif


/*====================================================================*/
#define movmem(a, b, c) memcpy( (b), (a), (c) )
/*====================================================================*/


#if     MSDOS & MWC
# include        <dos.h>
# define int86(a, b, c)  intcall(b, c, a)
# define intdos(a, b)    intcall(a, b, DOSINT)
# define inp(a)          in(a)
# define outp(a, b)      out(a, b)

struct XREG {
    unsigned int ax, bx, cx, dx, si, di, ds, es, flags;
};

struct HREG {
    char al, ah, bl, bh, cl, ch, dl, dh;
    unsigned int ds, es, flags;
};

union REGS {
    struct XREG x;
    struct HREG h;
};
#endif

#if     MSDOS & MSC
# include        <dos.h>
# include        <memory.h>
# define peek(a, b, c, d)   movedata(a, b, FP_SEG(c), FP_OFF(c), d)
# define poke(a, b, c, d)   movedata(FP_SEG(c), FP_OFF(c), a, b, d)
# define _strrev(a)      strrev(a)
#endif

#if     MSDOS & LATTICE
# undef  CPM
# undef  LATTICE
# include        <dos.h>
# undef  CPM
#endif

/* this keeps VMS happy */
#if     VMS
# define getname xgetname
# define unlink(a)       delete(a)
#endif

/* some options for AOS/VS */
#if     AOSVS
# define ORMDNI  1
#endif

/*      define some ability flags */

#if     (IBMPC | Z309 | FMR | TIPC) & !(WINDOW_MSWIN | WINDOW_MSWIN32)
# define MEMMAP  1
#else
# define MEMMAP  0
#endif


#if ( IS_UNIX() || MSDOS || WINNT || WINXP || OS2 || (TOS && MWC) || WMCS  || \
    MPE )
# define ENVFUNC 1
#else
# define ENVFUNC 0
#endif

#if     AUX
# define RETCHAR '\n'
#else
# define RETCHAR '\r'
#endif

#if     MPE
# define DIRSEPSTR       "."
# define DIRSEPCHAR      '.'
#else
# if     TOS || MSDOS || WINNT || WINXP || OS2
#  define DIRSEPSTR       "\\"
#  define DIRSEPCHAR      '\\'
#  define DRIVESEPCHAR    ':'
# else
#  define DIRSEPSTR       "/"
#  define DIRSEPCHAR      '/'
#  define DRIVESEPCHAR    '\0'
# endif
#endif


/*====================================================================*/
/* `varargs.h' vs. `stdarg.h':                                        */
/* - VARARG: One of these is supported.                               */
/* - VARARG && VARG:  `varargs.h'-support                             */
/* - VARARG && !VARG: `stdarg.h'-support                              */
/*                                                                    */
/* Example from                                                       */
/* <https://www.ibm.com/docs/en/aix/7.2?topic=v-varargs-macros>       */
/*====================================================================*/
/*....................................................................*/
# if ( 0 )  /**BEGIN COMMENT**/
/*....................................................................*/

#include <stdarg.h>
#define MAXargs 31
int execl (const char *path, ...)
{
   va_list Argp;
   char *array [MAXargs];
   int argno=0;
   va_start (Argp, path);
   while ((array[argno++] = va_arg(Argp, char*)) != (char*)0)
           ;
   va_end(Argp);
   return(execv(path, array));

}
main()
{
   execl("/usr/bin/echo", "ArgV[0]", "This", "Is", "A", "Test",      "\0");
   /* ArguementV[0] will be discarded by the execv in main(): */
   /* by convention ArgV[0] should be a copy of path parameter */
}

/*....................................................................*/

#include <varargs.h>
#define MAXargS 100
/*
**  execl is called by
**  execl(file, arg1, arg2, . . . , (char *) 0);
*/
execl(va_alist)
   va_dcl
{  va_list ap;
   char *file;
   char *args[MAXargS];
   int argno = 0;
   va_start(ap);
   file = va_arg(ap, char *);
   while ((args[argno++] = va_arg(ap, char *)) != (char *) 0)
      ;   /* Empty loop body */
   va_end(ap);
   return (execv(file, args));
}

/*....................................................................*/
# endif     /**  END COMMENT**/
/*....................................................................*/
/*====================================================================*/

#if     VARARG
# if ( (GCC == 0 ) && ( IS_UNIX() || MPE) )
#  define VARG    1
#  define SARG    0
#  include        <varargs.h>
# else
#  define VARG    0
#  define SARG    1
#  include        <stdarg.h>
# endif
#endif

#if ZTC
# include     <dos.h>
#endif


/*===== global includes to get some constants ========================*/
#include <limits.h>
/*====================================================================*/


/*===== Emacs global flag bit definitions (for gflags) ===============*/

#define GFREAD  1       /* read first file in at startup              */
#define GFSDRAW 2       /* suppress a screen redraw                   */
#define GFEXIT  4       /* exit from emacs pending                    */

/*===== internal constants ===========================================*/

#define NBINDS  300                     /* max # of bound keys        */
#if ( defined( PATH_MAX) )
# define NFILEN (PATH_MAX + 1)
#elif ( defined( MAXPATHLEN) )
# define NFILEN (MAXPATHLEN + 1)
#elif ( defined( _POSIX_PATH_MAX) )
# define NFILEN (_POSIX_PATH_MAX + 1)
#else
# if ( AOSVS || VMS || WINNT || WINXP || OS2 || IS_UNIX() )
#  define NFILEN  256
# else
#  define NFILEN  80            /* # of bytes, file name              */
# endif
#endif
#define NBUFN   128             /* # of bytes, buffer name            */
#ifdef MSDOS  /* Reduce sizes to UE312 vals --- else `stack overflow' */
# define NLINE   256            /* # of bytes, input line             */
# define NSTRING 128            /* # of bytes, string buffers         */
# define NPAT    128            /* # of bytes, pattern                */
#else
# define NLINE   512            /* # of bytes, input line             */
# define NSTRING 512            /* # of bytes, string buffers         */
# define NPAT    512            /* # of bytes, pattern                */
#endif
#define NKBDM   4096            /* # of strokes, keyboard macro       */
#define HUGENUM 1000            /* Huge number                        */
#define NLOCKS  256             /* max # of file locks active         */
#define NCOLORS 16              /* number of supported colors         */
#define KBLOCK  250             /* sizeof kill buffer chunks          */
#define NRING   16              /* # of buffers in kill ring          */
#define NBLOCK  16              /* line block chunk size              */
#define NVSIZE  16              /* max #chars in a var name           */
#define NMARKS  16              /* number of marks                    */
#define MAXSYM  32              /* max # chars in symbol to expand    */
#define MINFLEN 3               /* min # chars to match &func         */

#define CTRL    0x0100          /* Control flag, or'ed in             */
#define META    0x0200          /* Meta flag, or'ed in                */
#define CTLX    0x0400          /* ^X flag, or'ed in                  */
#define SPEC    0x0800          /* special key (function keys)        */
#define MOUS    0x1000          /* alternative input device (mouse)   */
#define SHFT    0x2000          /* shifted (for function keys)        */
#define ALTD    0x4000          /* ALT key...                         */
#define MENU    MOUS+SPEC       /* menu selection (WINDOW_MSWIN)      */

#define BINDNUL 0               /* not bound to anything              */
#define BINDFNC 1               /* key bound to a function            */
#define BINDBUF 2               /* key bound to a buffer              */

#ifdef  FALSE
# undef  FALSE
#endif
#ifdef  TRUE
# undef  TRUE
#endif

#define FALSE   0               /* False, no, bad, etc.               */
#define TRUE    1               /* True, yes, good, etc.              */
#define ABORT   2               /* Death, ^G, abort, etc.             */
#define FAILD   3               /* not-quite fatal false return       */

#define STOP    0               /* keyboard macro not in use          */
#define PLAY    1               /*                playing             */
#define RECORD  2               /*                recording           */

/*===== Completion types =============================================*/

#define CMP_BUFFER      0
#define CMP_COMMAND     1
#define CMP_FILENAME    2

/*===== Directive definitions ========================================*/

#define DIF             0
#define DELSE           1
#define DENDIF          2
#define DGOTO           3
#define DRETURN         4
#define DENDM           5
#define DWHILE          6
#define DENDWHILE       7
#define DBREAK          8
#define DFORCE          9

#define NUMDIRS         10

/*
 * PTBEG, PTEND, FORWARD, and REVERSE are all toggle-able values for routines
 * that need directions.
 */
#define PTBEG   0       /* Leave the point at the beginning on search */
#define PTEND   1       /* Leave the point at the end on search       */
#define FORWARD 0       /* do things in a forward direction           */
#define REVERSE 1       /* do things in a backwards direction         */

#define FIOSUC  0                   /* File I/O, success.             */
#define FIOFNF  1                   /* File I/O, file not found.      */
#define FIOEOF  2                   /* File I/O, end of file.         */
#define FIOERR  3                   /* File I/O, error.               */
#define FIOMEM  4                   /* File I/O, out of memory        */
#define FIOFUN  5                   /* File I/O, eod of file/bad line */
#define FIODEL  6                   /* Can't delete/rename file       */

#if     WINDOW_MSWIN
/* values for the fbusy flag */
# define FREADING 1                 /* file read in progress          */
# define FWRITING 2                 /* file write in progress         */
/* if no file activity, the value FALSE is used */
#endif

#define CFCPCN  0x0001              /* Last command was C-P, C-N      */
#define CFKILL  0x0002              /* Last command was a kill        */
#define CFSRCH  0x0004              /* last command was a search      */
#define CFYANK  0x0008              /* last command was a yank        */

#define SRNORM  0                   /* end past, begin front          */
#define SRBEGIN 1                   /* always at front                */
#define SREND   2                   /* always one past end            */

#define BELL    0x07                /* a bell character               */
#define TAB     0x09                /* a tab character                */

#if ( IS_UNIX() )
# define PATHCHR ':'
#else
# if  ( WMCS || MPE )
#  define PATHCHR ','
# else
#  define PATHCHR ';'
# endif
#endif

#define INTWIDTH        sizeof (int) * 3
#define LONGWIDTH       sizeof (long) * 3

/*===== Macro argument token types ===================================*/

#define TKNUL   0                       /* end-of-string              */
#define TKARG   1                       /* interactive argument       */
#define TKBUF   2                       /* buffer argument            */
#define TKVAR   3                       /* user variables             */
#define TKENV   4                       /* environment variables      */
#define TKFUN   5                       /* function....               */
#define TKDIR   6                       /* directive                  */
#define TKLBL   7                       /* line label                 */
#define TKLIT   8                       /* numeric literal            */
#define TKSTR   9                       /* quoted string literal      */
#define TKCMD   10                      /* command name               */

/*===== Internal defined functions ===================================*/

#define nextab(a)       ( ( (a) - ((a) % tabsize) ) + tabsize )

/*
 * DIFCASE represents the integer difference between upper and lower
 * case * letters. It is an xor-able value, which is fortunate, since
 * the relative * positions of upper to lower case letters is the
 * opposite of ascii in ebcdic.
 */

#if     ASCII
# define DIFCASE         0x20
#else
# define DIFCASE         0x40
#endif

/*===== Dynamic RAM tracking and reporting redefinitions =============*/

#if     RAMSIZE
# define malloc  Eallocate
# define free    Erelease
#else
# if     VMS & OPTMEM
#  define malloc  VAXC$MALLOC_OPT
#  define free    VAXC$FREE_OPT
#  define calloc  VAXC$CALLOC_OPT
#  define realloc VAXC$REALLOC_OPT
#  define cfree   VAXC$CFREE_OPT
# endif
#endif

/*
 * under MS Windows, we use dialog boxes to prompt the user for
 * filenames
 */
#if     WINDOW_MSWIN
# define FILENAMEREPLY(p, b, nb)   filenamedlg(p, b, nb, TRUE)
#else
# define FILENAMEREPLY(p, b, nb)   mlreply(p, b, nb)
#endif


/*======================================================================
 *
 *  This is the message which should be added to any "About MicroEMACS"
 *  boxes on any of the machines with window managers.
 *
 *  ------------------------------------------
 *  |                                        |
 *  |        MicroEMACS v4.xx                |
 *  |               for the ............     |
 *  |                                        |
 *  |    Text Editor and Corrector           |
 *  |                                        |
 *  |    written by Daniel M. Lawrence       |
 *  |    [based on code by Dave Conroy]      |
 *  |                                        |
 *  |    Send inquiries and donations to:    |
 *  |    617 New York St                     |
 *  |    Lafayette, IN 47901                 |
 *  |                                        |
 *  ------------------------------------------
 *
 *====================================================================*/



/*====================================================================*/
#endif/**#ifndef ESTRUCT_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
