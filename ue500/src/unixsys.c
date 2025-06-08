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
#define UNIXSYS_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/*==============================================================*/
/* Include files                                                */
/*==============================================================*/
#include "estruct.h"            /* Emacs definitions            */
#include "eproto.h"             /* Function definitions         */
#include "edef.h"               /* Global variable definitions  */
#include "elang.h"              /* Language definitions         */
/*==============================================================*/


/*==============================================================*/
/* SETTINGS configurable via CPP defines --- i.e. `cc -DX=z'    */
/*--------------------------------------------------------------*/
/* e.g. use                                                     */
/*  `cc -DSWITCH_BSD_FFS=USE_BSD_FFS_LATE                       */
/* for BSD 4.2 and later.                                       */
/*==============================================================*/
# if  ( b_IS_ANCIENT_UNIX )
#  define USE_BSD_FFS_EARLY (1)
#  define USE_BSD_FFS_LATE  (2)
#  ifndef SWITCH_BSD_FFS
#   define SWITCH_BSD_FFS USE_BSD_FFS_EARLY
#  endif
# endif
/*==============================================================*/


/*==============================================================*/
/* FEATURES                                                     */
/*==============================================================*/
#if ( CYGWIN )
/* Old CYGWIN Versions don't have these:  */
# /**define  USE_CYGWIN_CONV_PATH        ( 1 )**/
# define  USE_CYGWIN_CONV_PATH        ( 0 )
/* On CygWin spawn is more reliable than fork/exec but it's not
 * available on older CygWin versions */
# /**define  USE_CYGWIN_SPAWN           ( 1 )**/
# define  USE_CYGWIN_SPAWN            ( 0 )
# if ( 3 >= __GNUC__ )
#  undef  USE_CYGWIN_CONV_PATH
#  undef  USE_CYGWIN_SPAWN
#  define USE_CYGWIN_CONV_PATH        ( 0 )
#  define USE_CYGWIN_SPAWN            ( 0 )
# endif
#endif


#if ( DJGPP_DOS )
/* Use spawn() or DJGPP's smart system() for `dossystem()': */
# define USE_SPAWN_FOR_SYSTEM   ( 1 )
#endif
/*==============================================================*/


/** Do nothing routine **/
int unixsys0  P1_(char *, s)
{
    return (0);
}

/** Only compile for UNIX machines **/
#if ( b_IS_UNIX )


/*==============================================================*/
/* Include files and platform dependent declarations            */
/*==============================================================*/
# if ( !b_IS_ANCIENT_UNIX )
#  include <time.h>             /* time(), ctime(), ...         */
# else
   EXTERN long          time    DCL((long *));
   EXTERN char          *ctime  DCL((long *));
# endif
# include <errno.h>             /* errno, ...                   */

/** Overall include files **/
# if BEGIN_COMMENT_   /* Already included in `eproto.h' */
# include <sys/types.h>                 /* System type definitions  */
# include <sys/stat.h>                  /* File status definitions  */
# endif  /*END_COMMENT_*/
# include <signal.h>                    /* Signal definitions       */
# if ( !b_IS_ANCIENT_UNIX )
#  include <unistd.h>
#  include <fcntl.h>
# else
   EXTERN int           getpid  DCL((void));
   EXTERN int           ioctl   DCL((int, unsigned long int, ...));
   EXTERN unsigned int  sleep   DCL((unsigned int));
   EXTERN int           unlink  DCL((CONST char *));
   EXTERN int           open    DCL((char *, int));
   EXTERN int           close   DCL((int));
   EXTERN int           read    DCL((int, char *, int));
   EXTERN int           write   DCL((int, CONST char *, int));
   EXTERN int           access  DCL((CONST char *, int));
   EXTERN int           link    DCL((CONST char *, CONST char *));
# endif


/*==============================================================*/
/* Include files for directory handling, e.g functions like     */
/* opendir(), readdir(), closedir() if available.               */
/* UMC_DIRENTRY:  Name of the struct containing a directory's   */
/*                file names: direct, dirent, ... .             */
/*                Might also be set from outside.               */
/* UMC_DIR:       Type of pointer returned by opendir --- if    */
/*                available at all.                             */
/*==============================================================*/

/** Directory accessing: Try and figure this out... if you can! **/
# if  ( b_IS_ANCIENT_UNIX )
#  include <sys/dir.h>              /* Directory entry definitions  */
#  ifndef UMC_DIRENTRY
#   define UMC_DIRENTRY    direct
#  endif
#  if ( SWITCH_BSD_FFS == USE_BSD_FFS_EARLY )
    typedef struct bsd_ffs_early_dir_s_ {
      int fd;
    } bsd_ffs_early_dir_t_;
#   define UMC_DIR        bsd_ffs_early_dir_t_
#  else
#   define UMC_DIR        DIR
#  endif
# else
# if  ( XENIX || VAT )
#  include <sys/ndir.h>             /* Directory entry definitions  */
#  ifndef UMC_DIRENTRY
#   define UMC_DIRENTRY   direct
#  endif
#  define UMC_DIR         DIR
# else
#  include <dirent.h>               /* Directory entry definitions  */
#  ifndef UMC_DIRENTRY
#   define UMC_DIRENTRY   dirent
#  endif
#  define UMC_DIR         DIR
# endif
# endif

# if ( CYGWIN )
#  if USE_CYGWIN_CONV_PATH
#   include <sys/cygwin.h>
#  endif
#  if USE_CYGWIN_SPAWN
#   include <spawn.h>
#  endif
#  include <sys/wait.h>
EXTERN CONST char *     cygpwd_ DCL((void));
# endif /* CYGWIN */

# if ( DJGPP_DOS )
#  if ( USE_SPAWN_FOR_SYSTEM )
#   include <process.h>
#  endif
# endif


/*====================================================================*/
/* CONSTANTS                                                          */
/*====================================================================*/
# define MLWAIT   3
/*====================================================================*/


/*====================================================================*/
/* Static functions declared here:                                    */
/*====================================================================*/
static int  IsExecutable  DCL((CONST char * file));
/*====================================================================*/


/*==============================================================*/
# define MkUNXDirSep_(path) do  {         \
    char  *cp__ = (char *)(path);         \
                                          \
    while ( *cp__ ) {                     \
        if ( '\\' == *cp__ )  {           \
            *cp__ = '/';                  \
        }                                 \
        cp__++;                           \
    }                                     \
} while ( 0 )

# define MkDOSDirSep_(path) do  {         \
    char  *cp__ = (char *)(path);         \
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
    static int        FirstCall = 1;

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
    static int  FirstCall = 1;
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

#  if ( !0 )
#   define CYGDRIVE_      ( cygdrive_() )
#   define CYGDRIVE_LEN_  ( cygdrive_len_() )
#  else
#   define CYGDRIVE_      "/cygdrive/"
#   define CYGDRIVE_LEN_  ( SIZEOF(CYGDRIVE_) - 1 )
#  endif

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
# else
# if ( DJGPP_DOS )
#  define NormalizePathUNX(path)  do  {                     \
    char  *cp_  = (path);                                   \
                                                            \
    xstrlcpy(cp_, getunxpath(cp_), SIZEOF(path));           \
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
# endif /* CYGWIN */
/*==============================================================*/


/*==============================================================*/
/* Implementation of or wrapper for opendir/readdir/closedir:   */
/*==============================================================*/
# if  ( b_IS_ANCIENT_UNIX && SWITCH_BSD_FFS == USE_BSD_FFS_EARLY )
/* Read avoiding "short reads", see Richard Stevens' books on   */
/* UNIX programmin.                                             */
static int  readn P3_(int, fd, char *, buf, int, nbytes)
{
    char  *bp   = (char *)buf;
    int   nleft = nbytes;
    int   nread = 0;
    int   n     = 0;

    while ( 0 < nleft ) {
        n = read(fd, bp, nleft);
        if ( 0 > n )         { /* ERROR  */
            return n;
        } else if ( 0 == n)  { /* EOF    */
            return nread;
        } else   /* 0 < n */ {
            nread += n;
            nleft -= n;
            bp    += n;
        }
    }

    return nread;
}
# endif

static VOIDP  umc_opendir P1_(CONST char *, name)
{
# if  ( b_IS_ANCIENT_UNIX && SWITCH_BSD_FFS == USE_BSD_FFS_EARLY )
#  ifndef   O_RDONLY
#   define  O_RDONLY  (0)
#  endif
    int         rc  = 0;
    int         fd  = 0;
    struct stat sb;
    UMC_DIR     *res  = NULL;

    ZEROMEM(sb);
    ASRT(NULL != name);

    if        ( 0 > (rc = stat(name, &sb)) )                          {
        return NULL;
    } else if ( !S_ISDIR(sb.st_mode) )                                {
        return NULL;
    } else if ( 0 > (fd = open(name, O_RDONLY)) )                     {
        return NULL;
    } else if ( NULL == (res = (UMC_DIR *)calloc(1, SIZEOF(*res))) )  {
        return NULL;
    }

    res->fd = fd;

    return (VOIDP)res;
# else
    ASRT(NULL != name);

    return (VOIDP)opendir(name);
# endif
}

static CONST char *umc_readdir P1_(VOIDP, dirp)
{
    static char res[NFILEN];
# if  ( b_IS_ANCIENT_UNIX && SWITCH_BSD_FFS == USE_BSD_FFS_EARLY )
    struct UMC_DIRENTRY de;

    ZEROMEM(res);
    ZEROMEM(de);
    ASRT(NULL != dirp);

    while ( SIZEOF(de) == readn(((UMC_DIR *)dirp)->fd, &de, sizeof(de)) )  {
        if ( 0 != de.d_ino )  {
            XSTRCPY(res, de.d_name);

            return res;
        }
    }

    return NULL;
# else
    struct UMC_DIRENTRY *dep  = NULL;

    ZEROMEM(res);
    ASRT(NULL != dirp);

    if ( NULL != (dep = readdir((UMC_DIR *)dirp)) ) {
        XSTRCPY(res, dep->d_name);

        return res;
    } else                                          {
        return NULL;
    }
# endif
}

static int  umc_closedir P1_(VOIDP, dirp)
{
# if  ( b_IS_ANCIENT_UNIX && SWITCH_BSD_FFS == USE_BSD_FFS_EARLY )
    ASRT(NULL != dirp);

    if ( 0 != close(((UMC_DIR *)dirp)->fd) ) {
        return (-1);
    }
    free(dirp);

    return 0;
# else
# if  ( b_IS_ANCIENT_UNIX && SWITCH_BSD_FFS == USE_BSD_FFS_LATE )
    closedir((UMC_DIR *)dirp);

    return 0;
# else
    return closedir((UMC_DIR *)dirp);
# endif
# endif
}

#undef  UMC_DIR
#undef  UMC_DIRENTRY
/*==============================================================*/


static VOIDP    dirptr  = NULL;       /* Current directory stream     */
static char     path[NFILEN];         /* Path of file to find         */
static char     rbuf[NFILEN];         /* Return file buffer           */
static char     *nameptr;             /* Ptr past end of path in rbuf */

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
#  else
#  if DJGPP_DOS /* /dev/c is C: */
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
#  endif

    pFSlash = umc_strchr(path, '/');
    pBSlash = umc_strchr(path, '\\');
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
    static int        FirstCall = 1;
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
            TRC(("wingetshell(): `%s'", res));
        } else                      {
            res = NULL;
        }

        FirstCall = 0;
    }

    return res;
}

# endif


/*======================================================================
 * Some naming conventions for the next lines of code:
 *----------------------------------------------------------------------
 * `unx':   Unix Path
 * `enx':   Expanded Unix Path
 * `ads':   Absolute DOS Path
 * `cyg':   Cygwin specific function
 *====================================================================*/


# if ( CYGWIN )

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
    MkDOSDirSep_(in);
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
            if ( '\\' == in[i] )  {
                i++;
            }
            for ( ; i < len && j < SIZEOF(res) - 1; i++, j++ )  {
                if ( '\\' == in[i] )  {
                    xstrlccpy(res + j, '/',   SIZEOF(res) - j);
                } else                {
                    xstrlccpy(res + j, in[i], SIZEOF(res) - j);
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
    MkUNXDirSep_(in);
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
    static int  FirstCall = 1;
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
    static int  FirstCall = 1;
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
    MkDOSDirSep_(in);
    len = strlen((CONST char *)in);
    if        ( 5 <= len        &&    /* UNC-Path: \\<Server>\<Share> */
                '\\' == in[0]   &&
                '\\' == in[1]   &&
                '\\' != in[2]   &&
                NULL != umc_strchr((CONST char *)(in + 2), '\\')
              )   {
        MkUNXDirSep_(in);
        xstrlcpy(res, (CONST char *)in, SIZEOF(res));
    } else if ( 2 <= len        &&    /* <Drv>:<Path> <Drv>:\<Path>   */
                ':' == in[1]    &&
                ISALPHA(in[0])  &&
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
             unx_strerror_(errno_sv)));
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
             unx_strerror_(errno_sv)));
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
            return ( 1 );
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
             cmd, errno_sv, unx_strerror_(errno_sv)));
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

/* Cannot fail  */
static CONST char *getunxpath P1_(CONST char *, dos)
{
    char        in[NFILEN];
    static char res[NFILEN];
    int         len   = 0;
    char        *inp  = NULL;

    ZEROMEM(in);
    ZEROMEM(res);

#  if ( 0 )
    ASRT(NULL != dos);
#  else
    if ( NULL == dos )  {
        dos = "";
    }
#  endif

    xstrlcpy(in, dos, SIZEOF(in));
    MkDOSDirSep_(in);
    len = strlen((CONST char *)in);
    if        ( 5 <= len        &&    /* UNC-Path: \\<Server>\<Share> */
                '\\' == in[0]   &&
                '\\' == in[1]   &&
                '\\' != in[2]   &&
                NULL != (inp = umc_strchr(in + 2, '\\'))
              )   {
        /* DJGPP likes `\\<server>\<share>/<path>': */
        MkUNXDirSep_(inp + 1);
    } else        {
        MkUNXDirSep_(in);
    }
    xstrlcpy(res, in, SIZEOF(res));

    return res;
}

static CONST char *dosgetshell P0_()
{
    static int        FirstCall = 1;
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
        ASRT(IMPOSSIBLE);
    }
#   endif
    if        ( win_shell ) {
        status    = spawnv(P_WAIT, win_shell, win_argv);
        errno_sv  = errno;
    } else if ( dos_shell ) {
        status    = spawnv(P_WAIT, dos_shell, dos_argv);
        errno_sv  = errno;
    } else                  {
        ASRT(IMPOSSIBLE);
    }

    if        ( 0 > status )        {
        if        ( win_shell ) {
            TRC(("Error executing <%s %s %s>, errno = %d: %s", win_shell, WIN_SHELL_C_,
                 cmd, errno_sv, unx_strerror_(errno_sv)));
        } else if ( dos_shell ) {
            TRC(("Error executing <%s %s %s>, errno = %d: %s", dos_shell, DOS_SHELL_C_,
                 cmd, errno_sv, unx_strerror_(errno_sv)));
        } else                  {
            ASRT(IMPOSSIBLE);
        }

        return ( (-1) * errno_sv );
    } else if ( EINTR == errno_sv ) {
        int signo = 0;

        signo = ( (status & 0xFF00) >> 8 );
        if        ( win_shell ) {
            TRC(("RC <%s %s %s>: Got signal %d, errno = %d: %s",
                 win_shell, WIN_SHELL_C_, cmd, signo, errno_sv,
                 unx_strerror_(errno_sv)));
        } else if ( dos_shell ) {
            TRC(("RC <%s %s %s>: Got signal %d, errno = %d: %s",
                 dos_shell, DOS_SHELL_C_, cmd, signo, errno_sv,
                 unx_strerror_(errno_sv)));
        } else                  {
            ASRT(IMPOSSIBLE);
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
            ASRT(IMPOSSIBLE);
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
    int rc      = 0;
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
    status = (rc = winsystem(cmd)) == 0;
# else
# if ( DJGPP_DOS )
    status = (rc = dossystem(cmd)) == 0;
# else
    status = (rc = system(cmd))    == 0;
# endif
# endif
    TRC(("callout(): system(%s) returned %d", cmd, rc));
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
# else
# if ( SOLARIS )
        sh = "/usr/bin/ksh";
# else
# if ( CYGWIN )
        sh = wingetshell();
# else
# if ( DJGPP_DOS )
        /* First try Windows shell --- are we running in the 16-Bit
         * Windows subsystem?
         */
        if ( NULL == (sh = wingetshell()) ) {
            sh = dosgetshell();
        }
# else
        sh = "/bin/sh";
# endif
# endif
# endif
# endif /* LINUX */
    }

    if ( IsExecutable(sh) ) {
        /* Do shell */
        TRC(("spawncli(): Opening `SHELL=%s'", sh));

        return ( callout(sh) );
    } else {
        TRC(("spawncli(): `SHELL=%s' is not executable", sh));

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
    TRC(("spawn(): excecuting `%s'", line));
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

    /*==================================================================
    ACCESS(2)           UNIX Programmer's Manual            ACCESS(2)



    NAME
        access - determine accessibility of file

    SYNOPSIS
        access(name, mode)
        char *name;

    DESCRIPTION
        Access checks the given file name for accessibility accord-
        ing to mode, which is 4 (read), 2 (write) or 1 (execute) or
        a combination thereof.  Specifying mode 0 tests whether the
        directories leading to the file can be searched and the file
        exists.

        An appropriate error indication is returned if name cannot
        be found or if any of the desired access modes would not be
        granted.  On disallowed accesses -1 is returned and the
        error code is in errno . 0 is returned from successful
        tests.

        The user and group IDs with respect to which permission is
        checked are the real UID and GID of the process, so this
        call is useful to set-UID programs.

        Notice that it is only access bits that are checked.  A
        directory may be announced as writable by access, but an
        attempt to open it for writing will fail (although files may
        be created there); a file may look executable, but exec will
        fail unless it is in proper format.

    SEE ALSO
        stat(2)

    ASSEMBLER
        (access = 33.)
        sys access; name; mode
    ==================================================================*/

    if ( umc_strchr(mode, 'r') || umc_strchr(mode, 'R') ) {
        mflag |= 4/*R_OK*/;
    }
    if ( umc_strchr(mode, 'w') || umc_strchr(mode, 'W') ) {
        mflag |= 2/*W_OK*/;
    }
    if ( umc_strchr(mode, 'x') || umc_strchr(mode, 'X') ) {
        mflag |= 1/*X_OK*/;
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
    char        l_ident[C_1 + 1];

    ZEROMEM(str);
    ZEROMEM(res);
    ZEROMEM(l_ident);

    l_ident[0]  = 'x';
    xstrlcpy(str, gettmpdir(), SIZEOF(str));
    /* The filename part should have DOS 6.0 format --- remind DOS's
     * 126 byte command line limit
     */
    xstrlcat(str, "/ue", SIZEOF(str));
    if ( NULL != ident )  {
        int i = 0;

        for ( i = 0; i < SIZEOF(l_ident) - 1 && ident[i]; i++ ) {
            l_ident[i]  = ident[i];
        }
        mklower(l_ident);
    }
    xstrlcat(str, l_ident, SIZEOF(str));
    xstrlcat(str, ui2s36_memacs((unsigned int)(getpid() % (C_36)), C_1,
             FALSE), SIZEOF(str));

    for ( i = 0; i < (C_36 * C_36); i++ ) {
        struct stat sb;

        ZEROMEM(sb);

        xstrlcpy(res, str, SIZEOF(res));
        xstrlcat(res, ui2s36_memacs(
                 (unsigned int)((seed + i) % (C_36 * C_36)), C_2,
                 FALSE),  SIZEOF(res));
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
static int LaunchPrg P4_(CONST char *,  Cmd,
                         CONST char *,  InFile,
                         CONST char *,  OutFile,
                         CONST char *,  ErrFile)
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

    TRC(("LaunchPrg(): Cmd:       `%s'", Cmd));
    TRC(("LaunchPrg(): lInFile:   `%s'", lInFile));
    TRC(("LaunchPrg(): lOutFile:  `%s'", lOutFile));
    TRC(("LaunchPrg(): lErrFile:  `%s'", lErrFile));
# if ( CYGWIN  )
    xsnprintf(FullCmd, SIZEOF (FullCmd),
              "%s < %s > %s 2>%s",
              Cmd, lInFile, lOutFile, lErrFile);
# else
# if ( DJGPP_DOS )
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
# endif
    TRC(("LaunchPrg(): executing `%s'", FullCmd));

    return callout(FullCmd);
} /* LaunchPrg */

/*=============================================================================*/
/* Some helper functions that could also be in char.c/eproto.h                 */
/* We do not want to use the ctype.h functions as they depend on the locale.   */
/*=============================================================================*/

static int IsIn P3_(CONST char, c, CONST char *, set, int, len)
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
static char ToUpper P1_(CONST char, c)
{
    int i   = 0;

    for ( i = 0; i < SIZEOF (LOWERS_) - 1; i++ ) {
        if ( c == LOWERS_[i] ) {
            return UPPERS_[i];
        }
    }

    return c;
}

static char ToLower P1_(CONST char, c)
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
                            CONST char *, cmd,
                            CONST char *, tag)
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
                            CONST char *, cmd,
                            CONST char *, tag)
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
    char        bname[NFILEN];
    CONST char  *cp             = NULL;
    BUFFER      *bp             = NULL;

    ZEROMEM(Command);
    ZEROMEM(tmpnam);
    ZEROMEM(InFile);
    ZEROMEM(OutFile);
    ZEROMEM(bname);

    XSTRCPY(bname, "command");
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
# else
# if ( !0 )
    if ( !makecmdbname(bname, SIZEOF (bname), Command, "@Cmd") ) {
        umc_unlink(InFile);

        return FALSE;
    }
# endif
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
        umc_closedir(dirptr);
        dirptr = NULL;
    }

    dirptr = umc_opendir( (path[0] == '\0') ? "./" : path );

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
    CONST char  *fname  = NULL;
    struct stat fstat;

    ZEROMEM(fstat);

    /* ...and call for the next file */
    do {
        if ( NULL == (fname = umc_readdir(dirptr)) )  {
            return (NULL);
        }

        xstrlcpy(nameptr, fname, SIZEOF(rbuf) - (nameptr - rbuf));
        /* Check to make sure we skip all weird entries except
         * regular files and directories:
         */
    } while ( !( 0 == umc_stat(rbuf, &fstat) &&
                  (
                    S_ISDIR(fstat.st_mode)
                      ||
                    S_ISREG(fstat.st_mode)
                  )
               )
            );

    /* if this entry is a directory name, say so */
    if ( S_ISDIR(fstat.st_mode) ) {
        XSTRCAT(rbuf, DIRSEPSTR);
    }

    /* Return the next file name! */
    return (rbuf);
}

char  *unx_strerror_ P1_(int, num)
{
# if b_IS_ANCIENT_UNIX
    extern CONST char * CONST sys_errlist[];
    extern int sys_nerr;

    return (0 <= num && num < sys_nerr)?
        (char *)sys_errlist[num] : (char *)"NIL";
# else
    return strerror(num);
# endif
}

# if b_IS_ANCIENT_UNIX
/* SHELLQUOTE:
 *
 * Protect string from Bourne Shell expansion
 */
static char *shellquote P1_(CONST char *, in)
{
#  define SQREP_            "'\"'\"'"
#  define CHKRES_(j, incr)  ( (j) + (incr) <= NSTRING - 1 )
    static char res[NSTRING];
    int i = 0;
    int j = 0;

    ZEROMEM(res);
    ASRT(NULL != in);

    res[j++]  = '\'';
    for ( ;; )  {
        char  c = in[i];

        if ( '\0' == c )        {
            if ( ! CHKRES_(j, 1) )                  return NULL;
            res[j++]  = '\'';
            res[j++]  = c;

            return res;
        } else if ( '\'' != c ) {
            if ( ! CHKRES_(j, 1) )                  return NULL;
            res[j++]  = c;
        } else                  {
            /* res += SQREP_  */
            if ( ! CHKRES_(j, SIZEOF(SQREP_) - 1) ) return NULL;
            xstrcat(res + j, SQREP_);
            j += SIZEOF(SQREP_) - 1;
        }
        i++;
    }
#  undef  SQREP_
#  undef  CHKRES_
}
# endif

int unx_mkdir_ P1_(CONST char*, path)
{
    char  new_path[NFILEN];

    ZEROMEM(new_path);

    xstrlcpy(new_path, GetPathUNX(path), SIZEOF(new_path));
# if b_IS_ANCIENT_UNIX
    /*==================================================================
     * Very old Unix systems do not have a MKDIR(2) system call. Instead
     * directories are created via MKNOD(2) a system call that is only
     * allowed for root.
     *================================================================*/
    {
        int   rc  = 0;
        char  buf[NFILEN];
        char  *np = shellquote(new_path);

        ZEROMEM(buf);

        if ( NULL == np ) {
            return (-1);
        }
        XSTRCPY(buf, "mkdir ");
        XSTRCAT(buf, np);
        XSTRCAT(buf, " > /dev/null 2>&1");
        rc  = system(buf);

        return (rc == 0)? 0 : (-1);
    }
# else
    return mkdir(new_path, 0777);
# endif
}

int unx_rmdir_ P1_(CONST char*, path)
{
    char  new_path[NFILEN];

    ZEROMEM(new_path);

    xstrlcpy(new_path, GetPathUNX(path), SIZEOF(new_path));
# if b_IS_ANCIENT_UNIX
    /*==================================================================
     * Very old Unix systems do not have a RMDIR(2) system call. Instead
     * directories are removed via the seqence
     * unlink `<dir>/..'
     * unlink `<dir>/.'
     * unlink `<dir>'
     *================================================================*/
    {
        int   rc  = 0;
        char  buf[NFILEN];
        char  *np = shellquote(new_path);

        ZEROMEM(buf);

        if ( NULL == np ) {
            return (-1);
        }
        XSTRCPY(buf, "rmdir ");
        XSTRCAT(buf, np);
        XSTRCAT(buf, " > /dev/null 2>&1");

        rc  = system(buf);

        return (rc == 0)? 0 : (-1);
    }
# else
    return rmdir(new_path);
# endif
}


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
    int         rc      = 0;
    CONST char  *upath  = NULL;

    ASRT(NULL != path);
    /* It is OK here to *not* immediatley copy GetPathUNX's internal
     * static buffer, because we *know* that `access' won't call
     * GetPathUNX
     */
    rc = access(upath = GetPathUNX(path), mode);
    TRC(("unx_access_(): upath = %s, path = %s", upath, path));
    if ( 0 == rc) {
        return 0;
    }

    /* On CygWin, DJGPP and similar environments there might exist
     * path's which ar not (easily) convertable to some UNIX-path
     * --- e.g an UNC-path (i.e. `\\<Server>\<Share>').
     * The workaround here enables us to do an `access()' also for
     * such path's. See also `unx_access_()'.
     */
    {
        struct stat         sb;
        unsigned int        fmode = 0;
        int                 uid   = getuid();
        int                 gid   = getgid();
        CONST unsigned int  pmask = 0007;
        unsigned int        pbits = 0;
        unsigned int        umode = (unsigned int)mode;

        ZEROMEM(sb);

        if ( 0 != (rc  = unx_stat_(path, &sb)) )  {
            return (-1);
        }
        fmode  = sb.st_mode & ~S_IFMT & 0777;
        TRC(("unx_access_(): Mode of %s = 0%03o\n", path, fmode));

        pbits = fmode & pmask;  /* Everyone has "world" permissions */
        if ( gid == sb.st_gid ) {
            pbits |= (fmode & (pmask << 3)) >> 3; /* Add group mode */
            /* TODO: Add permissions resulting from secondary groups  */
        }
        if ( uid == sb.st_uid ) {
            pbits |= (fmode & (pmask << 6)) >> 6; /* Add owner mode */
        }
        TRC(("unx_access_(): pbits = 0%o\n", pbits));

        if ( (pbits & umode) == umode ) {
            return 0;
        } else                          {
            return (-1);
        }
    }
}

int unx_rename_ P2_(CONST char *, from, CONST char *, to)
{
    char  new_from[NFILEN];
    char  new_to[NFILEN];

    ZEROMEM(new_from);
    ZEROMEM(new_to);

    xstrlcpy(new_from, GetPathUNX(from), SIZEOF(new_from));
    xstrlcpy(new_to,   GetPathUNX(to),   SIZEOF(new_to));

# if ( b_IS_ANCIENT_UNIX )
    {
        struct stat buf1;
        struct stat buf2;

        ZEROMEM(buf1);
        ZEROMEM(buf2);

        /* No good if source file doesn't exist */
        if ( 0 != stat(new_from, &buf1) ) {
            return (-1);
        }

        /* Check for target */
        if ( 0 == stat(new_to, &buf2) ) {
            /* See if file is the same */
            if ( buf1.st_dev == buf2.st_dev &&
                 buf1.st_ino == buf2.st_ino ) {

                /* Not necessary to rename file */
                return (0);
            }
        }

        /* Get rid of target */
        unlink(new_to);

        /* Link two files together */
        if ( 0 != link(new_from, new_to) )  {
            return (-1);
        }

        /* Unlink original file */
        return ( unlink(new_from) );
    }
# else

    return rename(new_from, new_to);
# endif
}

int unx_stat_ P2_(CONST char *, path, struct stat *, sb)
{
    int         rc      = 0;
    int         fd      = 0;
    CONST char  *upath  = NULL;

    ASRT(NULL != path);
    ASRT(NULL != sb);

    /* It is OK here to *not* immediatley copy GetPathUNX's internal
     * static buffer, because we *know* that `stat' won't call
     * GetPathUNX
     */
    rc  = stat(upath = GetPathUNX(path), sb);
    TRC(("unx_stat_(): upath = %s, path = %s, stat rc: %d", upath, path, rc));
    if ( 0 == rc )  {
        return 0;
    }

    /* On CygWin, DJGPP and similar environments an `open()'
     * might be possible for a *native* OS-path, like e.g an
     * UNC-path (i.e. `\\<Server>\<Share>') which is not (easily)
     * convertable to some UNIX-path. The workaround here enables
     * us to do a `stat()' also for such path's.
     */
    fd  = open(path, O_RDONLY);

    if ( 0 > fd ) {
        int errno_sv  = errno;

        TRC(("unx_stat_(): open(%s, O_RDONLY) failed, errno = %d: %s",
             path, errno_sv, umc_strerror(errno_sv)));

        return (-1);
    }

    rc  = fstat(fd, sb);
    close(fd);
    TRC(("unx_stat_(): stat(%s) failed, open(%s, O_RDONLY) gave %d and fstat() returned %d.",
         upath, path, fd, rc));

    return rc;
}

# if    BEGIN_COMMENT_
int unx_link_ P2_(CONST char *, from, CONST char *, to)
{
    char  new_from[NFILEN];
    char  new_to[NFILEN];

    ZEROMEM(new_from);
    ZEROMEM(new_to);

    xstrlcpy(new_from, GetPathUNX(from), SIZEOF(new_from));
    xstrlcpy(new_to,   GetPathUNX(to),   SIZEOF(new_to));

    return link(new_from, new_to);
}
# endif /*END_COMMENT_*/

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
# endif /* HANDLE_WINCH */



#endif /* b_IS_UNIX */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
