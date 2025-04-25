/*======================================================================
 *      DOLOCK.C:       Machine specific code for File Locking
 *                      for MicroEMACS
 *                      (C)Copyright 1995 by Daniel M Lawrence
 *====================================================================*/

/*====================================================================*/
#define DOLOCK_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include <stdio.h>
#include "estruct.h"
#include "eproto.h"
#include "elang.h"
#if  ( b_IS_UNIX )
# if ( !b_IS_ANCIENT_UNIX )
#  include <unistd.h>
# else
   EXTERN int getpid      DCL((void));
#  ifdef GETHOSTNAME_AVAILABLE
   EXTERN int gethostname DCL((char *name, int len));
#  else
#   define  MYHOSTNAME_   "darkstar"

   static int gethostname P2_(char *, name, int, len)
   {
     int                i       = 0;

     for ( i = 0; i < MIN2(SIZEOF(MYHOSTNAME_), len) - 1; i++ ) {
       name[i]  = MYHOSTNAME_[i];
     }
     name[i]  = '\0';

     return 0;
   }
#   undef   MYHOSTNAME_
#  endif
# endif
#endif

#if ( FILOCK && WMCS )
/* file locking for WMCS */

# include "sys$disk/sysincl.sys/sysequ.h"
# include <stdio.h>
# include <ctype.h>

static CONST char msg[] = TEXT35;
/*           "another user" */

/* DOLOCK:
 */
char *dolock P1_(CONST char *, fname)
{
    int lun     = 0;
    int status  = 0;

    status = _open(fname, OPREADACC|OPWRITEACC|OPWRITELOCK, -1, &lun);
    if ( status == 133 || status == 0 ) return (NULL);

    return (msg);
}

/* UNDOLOCK:
 */
char *undolock P1_(CONST char *, fname)
{
    int   i       = 0;
    int   j       = 0;
    int   k       = 0;
    int   lun     = 0;
    int   status  = 0;
    char  c       = '\0';
    char  xname[C_95];

    ZEROMEM(xname);

    for ( lun=4; _getfnam(lun, xname) == 0; lun++ ) {
        for ( i=0; i<STRLEN(xname); i++ ) {
            k = i;
            for ( j=0; j<STRLEN(fname); j++ ) {
                c = fname[j];
                if ( is_lower(c) ) c = toupper(c);
                if ( c == xname[k] ) { ++k; continue; }
                if ( c == '\0' ) break;
                break;
            }
            if ( j == STRLEN(fname) ) {
                _close(lun, 0);

                return (NULL);
            }
        }
    }

    return (NULL);
}


#else
#if ( FILOCK && ( b_IS_UNIX || MSDOS || WINNT || WINXP || OS2 || AMIGA) )
/* b_IS_UNIX: `sys/types.h' and `sys/stat.h' already included
 * in `eproto.h':
 */
# if  ( OS2 || ( ( MSDOS || WINNT || WINXP ) && MSC) )
#  include    <sys/types.h>
# endif
# if ( ! b_IS_UNIX )
#   include   <sys/stat.h>
# endif
# include     <errno.h>
# if  ( MSDOS && TURBO )
#  include    <dir.h>
# endif
# if  ( b_IS_UNIX )
#  if ( !b_IS_ANCIENT_UNIX )
#   include   <dirent.h>
#  endif
#  include    <signal.h>
# endif

# if  ( (WINNT || WINXP ) && MSC )
#  include    <direct.h>
#  define chdir       _chdir
#  define getcwd      _getcwd
# endif

# if  ( !OS2 )
#  if  ( ZTC )
COMMON volatile int errno;
#  else
#   if  ( !MSC )
COMMON int errno;
#   endif
#  endif
# endif

# define LOCKDIR    "_xlk"
# define LOCKMSG    "LOCK ERROR -- "
/**# define LOCKDEBUG  FALSE**/
# define LOCKDEBUG  TRUE


/* PARSE_NAME:
 *
 * Get name component of filespec:
 */
static CONST char *parse_name P1_(CONST char *, filespec)
{
    CONST char  *rname  = &filespec[STRLEN(filespec) - 1];

    while ( rname >= filespec ) {
        if ( *rname == DIRSEPCHAR || *rname == DRIVESEPCHAR ) {
            ++rname;
            break;
        }
        --rname;
    }

    if ( rname >= filespec )  {
        return (rname);
    } else                    {
        return (filespec);
    }
}

/* Joachim Schneider, 2025-04-25:
 * The original coding explicitely return a trailing slash --- why?
 */
# define  NO_TRAILING_SLASH (1)
/* PARSE_PATH:
 */
static char *parse_path P1_(CONST char *, filespec)
{
    static char rbuff[NFILEN];
    char        *rname  = NULL;

    ZEROMEM(rbuff);

    /* make a copy we can mung */
    XSTRCPY(rbuff, filespec);

    /* starting from the end */
    rname = &rbuff[STRLEN(rbuff)-1];

    /* scan for a directory separator */
    while ( rname >= rbuff ) {
        if ( *rname == DIRSEPCHAR ) {
            break;
        }
        --rname;
    }

    /* no path here! */
    if ( rname < rbuff ) {
        rbuff[0] = '.';
# if ( NO_TRAILING_SLASH )
        rbuff[1] = '\0';
#else
        rbuff[1] = DIRSEPCHAR;
        rbuff[2] = '\0';
#endif

        return (rbuff);
    }

# if ( NO_TRAILING_SLASH )
    *rname        = '\0'; /* No trailing slash        */
# else
    *(rname + 1)  = '\0'; /* point just beyond slash  */
# endif

    /* no skip beyond any drive spec */
    rname = rbuff;
    while ( *rname ) {
        if ( *rname == DRIVESEPCHAR ) {
            return (++rname);
        }
        ++rname;
    }

    /* none to be found, return the whole string */
    return (rbuff);
}

/* PARSE_DRIVE:
 */
static CONST char *parse_drive P1_(CONST char *, filespec)
{
    static char rbuff[NFILEN];
    char        *rname  = NULL;

    ZEROMEM(rbuff);

    /* search for a drive specifier */
    XSTRCPY(rbuff, filespec);
    rname = rbuff;
    while ( *rname ) {
        if ( *rname == DRIVESEPCHAR ) {
            *(++rname) = 0;

            return (rbuff);
        }
        ++rname;
    }

    /* no drive letter/name */
    return ("");
}

/* TERM_TRIM:
 *
 * Trim line terminators and whitespace from end of string
 */
static VOID term_trim P1_(char *, buf)
{
    /* ptr to current character to examine: */
    char  *c  = buf + STRLEN(buf) - 1;

    while ( (c >= buf) && ( (*c == '\r') || (*c == '\n') || (*c == ' ')
                                         || (*c == '\t') ) )  {
        *c  = '\0';
        c--;
    }

    return;
}

/* DOLOCK:
 *
 * Generic non-UNIX file locking mechanism
 *
 **********************
 *
 * dolock -- lock the file fname
 *
 * if successful, returns NULL
 * if file locked, returns username of person locking the file
 * if other error, returns "LOCK ERROR -- explanation"
 *
 *********************/
char *dolock P1_(CONST char *, filespec)
/* filespec:  Full file spec of file to lock  */
{
    struct stat sb;             /* stat buffer for info on files/dirs */
    FILE        *fp     = NULL;     /* ptr to lock file */
    long        proc_id = 0;        /* process id from lock file */
# if ( b_IS_UNIX )
    int         rc      = 0;        /* syscall return code */
# endif
    char        filename[NFILEN];   /* name of file to lock */
    char        pathname[NFILEN];   /* path leading to file to lock */
    char        drivename[NFILEN];  /* drive for file to lock */
    char        lockpath[NFILEN];   /* lock directory name */
    char        lockfile[NFILEN];   /* lock file name */
    char        buf[NSTRING];       /* input buffer */
    char        host[NSTRING];      /* current host name */
    static char result[NSTRING];    /* error return string */

    ZEROMEM(sb);
    ZEROMEM(filename);
    ZEROMEM(pathname);
    ZEROMEM(drivename);
    ZEROMEM(lockpath);
    ZEROMEM(lockfile);
    ZEROMEM(buf);
    ZEROMEM(host);
    ZEROMEM(result);

    /* separate filespec into components */
# if ( b_IS_UNIX )
    {
        char  new_filespec[NFILEN];

        ZEROMEM(new_filespec);
        xstrlcpy(new_filespec, GetPathUNX(filespec), SIZEOF(new_filespec));

        XSTRCPY(filename, parse_name(new_filespec));
        XSTRCPY(pathname, parse_path(new_filespec));
        XSTRCPY(drivename, parse_drive(new_filespec));
    }
# else
    XSTRCPY(filename, parse_name(filespec));
    XSTRCPY(pathname, parse_path(filespec));
    XSTRCPY(drivename, parse_drive(filespec));
# endif
    if ( pathname[0] == 0 ) {
        XSTRCPY(pathname, ".");
    }

# if  LOCKDEBUG
    TRC(("Locking [%s] [%s] [%s]", drivename, pathname, filename));
# endif
    /* merge the drive into the pathname */
    XSTRCAT(drivename, pathname);
    XSTRCPY(pathname, drivename);

# if  ( b_IS_UNIX )
    /* check to see if we can access the path */
    if ( (rc = umc_stat(pathname, &sb)) != 0 )  {
#  if  LOCKDEBUG
        int errno_sv  = errno;

        TRC(("umc_stat() = %d, errno = %d: %s", rc, errno_sv,
             umc_strerror(errno_sv)));
#  endif
        XSTRCPY(result, LOCKMSG);
        XSTRCAT(result, "Path not found");

        return (result);
    }
    if ( !S_ISDIR(sb.st_mode) ) {
        XSTRCPY(result, LOCKMSG);
        XSTRCAT(result, "Illegal Path");

        return (result);
    }
# endif /* b_IS_UNIX  */

    /* create the lock directory if it does not exist */
    XSTRCPY(lockpath, pathname);
    XSTRCAT(lockpath, DIRSEPSTR);
    XSTRCAT(lockpath, LOCKDIR);
# if  LOCKDEBUG
    TRC(("Lockdir [%s]", lockpath));
# endif

    if ( umc_stat(lockpath, &sb) != 0 ) {
        /* create it! */
# if  LOCKDEBUG
        TRC(("MKDIR(%s)", lockpath));
# endif
        if ( umc_mkdir(lockpath) != 0 ) {
            XSTRCPY(result, LOCKMSG);
            switch ( errno ) {

            case EACCES:
                XSTRCAT(result, "Permission Denied");
                break;

            case ENOENT:
                XSTRCAT(result, "No such file or directory");
                break;

            default:
                XSTRCAT(result, "Unknown error creating lockpath");
                break;
            }

            return (result);
        }
# if  ( b_IS_UNIX )
        chmod(lockpath, 01777);
# endif
    }

    /* check for the existance of this lockfile */
    XSTRCPY(lockfile, lockpath);
    XSTRCAT(lockfile, DIRSEPSTR);
    XSTRCAT(lockfile, filename);
# if  LOCKDEBUG
    TRC(("Lockfile [%s]", lockfile));
# endif

    if ( umc_stat(lockfile, &sb) != 0 ) {
        /* create the lock file */
        fp = fopen(lockfile, "w");
        if ( fp == (FILE *)NULL ) {
            XSTRCPY(result, LOCKMSG);
            XSTRCAT(result, "Can not open lock file");
# if  LOCKDEBUG
            TRC(("Could not open lockfile [%s](%s)", lockfile, result));
# endif

            return (result);
        }

        /* and output the info needed */
# if  ( b_IS_UNIX )
        fprintf(fp, "%lu\n", (unsigned long int)getpid());
# else
        fprintf(fp, "%lu\n", 0ul); /* process ID */
# endif

        /* user name */
        if ( getenv("USER") )           {
            fprintf( fp, "%s\n", getenv("USER") );
        } else if ( getenv("LOGNAME") ) {
            fprintf( fp, "%s\n", getenv("LOGNAME") );
        } else                          {
            fprintf(fp, "<unknown>\n");
        }

        /* host name */
        if ( getenv("HOST") ) {
            fprintf( fp, "%s\n", getenv("HOST") );
        } else                {
# if  ( b_IS_UNIX )
            ZEROMEM(buf);
            gethostname(buf, SIZEOF(buf) - 1);
            fprintf(fp, "%s\n", buf);
# else
            fprintf(fp, "<unknown>\n");
# endif
        }

        /* time... */
        fprintf( fp, "%s\n", timeset() );

        fclose(fp);

        return (NULL);
    } else                              {
        /* get the existing lock info */
        fp = fopen(lockfile, "r");
        if ( fp == (FILE *)NULL ) {
            XSTRCPY(result, LOCKMSG);
            XSTRCAT(result, "Can not read lock file");

            return (result);
        }

        /* get the process id */
        fgets(buf, SIZEOF(buf), fp);
        proc_id = asc_int(buf);

        /* get the user name */
        fgets(result, SIZEOF(result), fp);
        term_trim(result);

        /* get the host name */
        XSTRCAT(result, "@");
        fgets(buf, NSTRING, fp);
        term_trim(buf);
        XSTRCAT(result, buf);

# if  ( b_IS_UNIX )
        /* is it the current host? */
        ZEROMEM(host);
        gethostname(host, SIZEOF(host) - 1);
        if ( strcmp(buf, host) == 0 ) {
            /* see if the process is dead already */
            if ( kill(proc_id, 0) != 0 && errno == ESRCH ) {
                /* kill the lock file and retry the lock */
                fclose(fp);
                umc_unlink(lockfile);

                return ( dolock(filespec) );
            }
        }
# endif

        /* get the time */
        XSTRCAT(result, " at ");
        fgets(buf, SIZEOF(buf), fp);
        term_trim(buf);
        XSTRCAT(result, buf);
        fclose(fp);
# if  LOCKDEBUG
        TRC(("Could not get lock: (%s)", result));
# endif

        return (result);
    }
}

/* UNDOLOCK:
 *
 **********************
 *
 * undolock -- unlock the file fname
 *
 * if successful, returns NULL
 * if other error, returns "LOCK ERROR: explanation"
 *
 *********************/
char *undolock P1_(CONST char *, filespec)
/* filespec:  Filespec to unlock  */
{
    char        filename[NFILEN];   /* name of file to lock */
    char        pathname[NFILEN];   /* path leading to file to lock */
    char        drivename[NFILEN];  /* drive for file to lock */
    char        lockpath[NFILEN];   /* lock directory name */
    char        lockfile[NFILEN];   /* lock file name */
    static char result[NSTRING];    /* error return string */

    ZEROMEM(filename);
    ZEROMEM(pathname);
    ZEROMEM(drivename);
    ZEROMEM(lockpath);
    ZEROMEM(lockfile);
    ZEROMEM(result);

    /* separate filespec into components */
# if ( b_IS_UNIX )
    {
        char  new_filespec[NFILEN];

        ZEROMEM(new_filespec);
        xstrlcpy(new_filespec, GetPathUNX(filespec), SIZEOF(new_filespec));

        XSTRCPY(filename, parse_name(new_filespec));
        XSTRCPY(pathname, parse_path(new_filespec));
        XSTRCPY(drivename, parse_drive(new_filespec));
    }
# else
    XSTRCPY(filename, parse_name(filespec));
    XSTRCPY(pathname, parse_path(filespec));
    XSTRCPY(drivename, parse_drive(filespec));
# endif
    if ( pathname[0] == 0 ) {
        XSTRCPY(pathname, ".");
    }

# if  LOCKDEBUG
    TRC(("Unlocking [%s] [%s] [%s]", drivename, pathname, filename));
# endif
    /* merge the drive into the pathname */
    XSTRCAT(drivename, pathname);
    XSTRCPY(pathname, drivename);

    /* create the lock directory if it does not exist */
    XSTRCPY(lockpath, pathname);
    XSTRCAT(lockpath, DIRSEPSTR);
    XSTRCAT(lockpath, LOCKDIR);
# if  LOCKDEBUG
    TRC(("Lockdir [%s]", lockpath));
# endif
    /* check for the existance of this lockfile */
    XSTRCPY(lockfile, lockpath);
    XSTRCAT(lockfile, DIRSEPSTR);
    XSTRCAT(lockfile, filename);
# if  LOCKDEBUG
    TRC(("Lockfile [%s]", lockfile));
# endif
    if ( umc_unlink(lockfile) ) {
        XSTRCAT(result, "could not remove lock file");

        return (result);
    } else {
        umc_rmdir(lockpath);  /* this will work only if dir is empty */

        return (NULL);
    }
}


#else

VOID dolockhello P0_()
{
}

#endif
#endif  /* ( FILOCK && WMCS ) */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
