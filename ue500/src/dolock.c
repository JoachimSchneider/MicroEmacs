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
#if  ( IS_UNIX() )
# if ( !IS_ANCIENT_UNIX() )
#  include <unistd.h>
# else
   EXTERN int getpid  DCL((void));
   EXTERN int rmdir   DCL((CONST char *));
   static int gethostname P2_(char *, name, int, len)
   {
     static CONST char  mname[] = "darkstar";
     int                i       = 0;

     for ( i = 0; i < MIN2(SIZEOF(mname), len) - 1; i++ ) {
       name[i]  = mname[i];
     }
     name[i]  = '\0';

     return 0;
   }
# endif
#endif

#if ( FILOCK && WMCS )
/* file locking for WMCS */

# include "sys$disk/sysincl.sys/sysequ.h"
# include <stdio.h>
# include <ctype.h>

static CONST char mg[]  = TEXT35;
/*           "another user" */

/* DOLOCK:
 */
char *dolock P1_(CONST char *, fname)
{
    int lun, status;
    status = _open(fname, OPREADACC|OPWRITEACC|OPWRITELOCK, -1, &lun);
    if ( status == 133 || status == 0 ) return (NULL);

    return (msg);
}

/* UNDOLOCK:
 */
char *undolock P1_(CONST char *, fname)
{
    int i, j, k, lun, status;
    char xname[95], c;

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


#elif ( FILOCK && ( IS_UNIX() || MSDOS || WINNT || WINXP || OS2 || AMIGA) )

# if  ( OS2 || ( ( MSDOS || WINNT || WINXP ) && MSC) || IS_UNIX() )
#  include     <sys/types.h>
# endif
# include       <sys/stat.h>
# include       <errno.h>
# if  ( MSDOS && TURBO )
#  include     <dir.h>
# endif
# if  ( IS_UNIX() )
#  include     <dirent.h>
#  include     <signal.h>
# endif

# if  ( (WINNT || WINXP ) && MSC )
#  include <direct.h>
#  define chdir        _chdir
#  define getcwd       _getcwd
#  define mkdir        _mkdir
#  define rmdir        _rmdir
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
# define LOCKDEBUG  FALSE
/**# define LOCKDEBUG TRUE**/


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

    if ( rname >= filespec )

        return (rname);
    else

        return (filespec);
}

/* PARSE_PATH:
 */
static char *parse_path P1_(CONST char *, filespec)
{
    static char rbuff[NFILEN];
    char        *rname  = NULL;

    /* make a copy we can mung */
    XSTRCPY(rbuff, filespec);

    /* starting from the end */
    rname = &rbuff[STRLEN(rbuff)-1];

    /* scan for a directory separator */
    while ( rname >= rbuff ) {
        if ( *rname == DIRSEPCHAR )
            break;
        --rname;
    }

    /* no path here! */
    if ( rname < rbuff ) {
        rbuff[0] = '.';
        rbuff[1] = DIRSEPCHAR;
        rbuff[2] = 0;

        return (rbuff);
    }

    *(rname + 1) = '\0'; /* point just beyond slash */

    /* no skip beyond any drive spec */
    rname = rbuff;
    while ( *rname ) {
        if ( *rname == DRIVESEPCHAR )

            return (++rname);

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
    char *c;  /* ptr to current character to examine */

    c = buf + STRLEN(buf) - 1;
    while ( (c >= buf) &&
            ( (*c == '\r') || (*c == '\n') || (*c == ' ') || (*c == '\t') ) ) {
        *c = 0;
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
 * if other error, returns "LOCK ERROR: explanation"
 *
 *********************/
char *dolock P1_(CONST char *, filespec)
/* filespec:  Full file spec of file to lock  */
{
    struct stat sb;             /* stat buffer for info on files/dirs */
    FILE *fp;                   /* ptr to lock file */
    long proc_id;               /* process id from lock file */
#if ( IS_UNIX() )
    int rc;                     /* syscall return code */
#endif
    char filename[NFILEN];      /* name of file to lock */
    char pathname[NFILEN];      /* path leading to file to lock */
    char drivename[NFILEN];     /* drive for file to lock */
    char lockpath[NFILEN];      /* lock directory name */
    char lockfile[NFILEN];      /* lock file name */
    char buf[NSTRING];          /* input buffer */
    char host[NSTRING];         /* current host name */
    static char result[NSTRING]; /* error return string */

    /* separate filespec into components */
#if ( IS_UNIX() )
    {
        char  new_filespec[NFILEN];

        ZEROMEM(new_filespec);
        xstrlcpy(new_filespec, GetPathUNX(filespec), SIZEOF(new_filespec));

        XSTRCPY( filename, parse_name(new_filespec) );
        XSTRCPY( pathname, parse_path(new_filespec) );
        XSTRCPY( drivename, parse_drive(new_filespec) );
    }
#else
    XSTRCPY( filename, parse_name(filespec) );
    XSTRCPY( pathname, parse_path(filespec) );
    XSTRCPY( drivename, parse_drive(filespec) );
#endif
    if ( pathname[0] == 0 )
        XSTRCPY(pathname, ".");

    /* merge the drive into the pathname */
    XSTRCAT(drivename, pathname);
    XSTRCPY(pathname, drivename);

# if  LOCKDEBUG
    printf("Locking [%s] [%s]\n", pathname, filename);
    tgetc();
# endif

# if  ( IS_UNIX() )
    /* check to see if we can access the path */
    if ( (rc = umc_stat(pathname, &sb)) != 0 )  {
#  if  LOCKDEBUG
        printf("umc_stat() = %u   errno = %u\n", rc, errno);
        tgetc();
#  endif
        XSTRCPY(result, LOCKMSG);
        XSTRCAT(result, "Path not found");

        return (result);
    }
    if ( (sb.st_mode & S_IFDIR) == 0 ) {
        XSTRCPY(result, LOCKMSG);
        XSTRCAT(result, "Illegal Path");

        return (result);
    }
# endif /* IS_UNIX()  */

    /* create the lock directory if it does not exist */
    XSTRCPY(lockpath, pathname);
    XSTRCAT(lockpath, DIRSEPSTR);
    XSTRCAT(lockpath, LOCKDIR);
# if  LOCKDEBUG
    printf("Lockdir [%s]\n", lockpath);
    tgetc();
# endif

    if ( umc_stat(lockpath, &sb) != 0 ) {
        /* create it! */
# if  LOCKDEBUG
        printf("MKDIR(%s)\n", lockpath);
        tgetc();
# endif
# if  ( IS_UNIX() )
        if ( mkdir(lockpath, 0777) != 0 ) {
# else
        if ( mkdir(lockpath) != 0 ) {
# endif
            XSTRCPY(result, LOCKMSG);
            switch ( errno ) {

            case EACCES:
                XSTRCAT(result, "Permission Denied");
                break;

            case ENOENT:
                XSTRCAT(result, "No such file or directory");
                break;
            }

            return (result);
        }
# if  ( IS_UNIX() )
        chmod(lockpath, 01777);
# endif
    }

    /* check for the existance of this lockfile */
    XSTRCPY(lockfile, lockpath);
    XSTRCAT(lockfile, DIRSEPSTR);
    XSTRCAT(lockfile, filename);
# if  LOCKDEBUG
    printf("Lockfile [%s]\n", lockfile);
    tgetc();
# endif

    if ( umc_stat(lockfile, &sb) != 0 ) {
        /* create the lock file */
        fp = fopen(lockfile, "w");
        if ( fp == (FILE *)NULL ) {
            XSTRCPY(result, LOCKMSG);
            XSTRCAT(result, "Can not open lock file");
# if  LOCKDEBUG
            printf("Could not open lockfile [%s](%s)\n", lockfile, result);
            tgetc();
# endif

            return (result);
        }

        /* and output the info needed */
# if  ( IS_UNIX() )
        fprintf( fp, "%lu\n", (long int)getpid() );
# else
        fprintf(fp, "%lu\n", 0ul); /* process ID */
# endif

        /* user name */
        if ( getenv("USER") )
            fprintf( fp, "%s\n", getenv("USER") );
        else if ( getenv("LOGNAME") )
            fprintf( fp, "%s\n", getenv("LOGNAME") );
        else
            fprintf(fp, "<unknown>\n");

        /* host name */
        if ( getenv("HOST") )
            fprintf( fp, "%s\n", getenv("HOST") );
        else {
# if  ( IS_UNIX() )
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
    } else {
        /* get the existing lock info */
        fp = fopen(lockfile, "r");
        if ( fp == (FILE *)NULL ) {
            XSTRCPY(result, LOCKMSG);
            XSTRCAT(result, "Can not read lock file");

            return (result);
        }

        /* get the process id */
        fgets(buf, NSTRING, fp);
        proc_id = asc_int(buf);

        /* get the user name */
        fgets(result, NSTRING, fp);
        term_trim(result);

        /* get the host name */
        XSTRCAT(result, "@");
        fgets(buf, NSTRING, fp);
        term_trim(buf);
        XSTRCAT(result, buf);

# if  ( IS_UNIX() )
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
        fgets(buf, NSTRING, fp);
        term_trim(buf);
        XSTRCAT(result, buf);
        fclose(fp);
# if  LOCKDEBUG
        printf("Could not get lock: (%s)\n", result);
        tgetc();
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
    char filename[NFILEN];          /* name of file to lock */
    char pathname[NFILEN];          /* path leading to file to lock */
    char drivename[NFILEN];         /* drive for file to lock */
    char lockpath[NFILEN];          /* lock directory name */
    char lockfile[NFILEN];          /* lock file name */
    static char result[NSTRING];    /* error return string */

    /* separate filespec into components */
#if ( IS_UNIX() )
    {
        char  new_filespec[NFILEN];

        ZEROMEM(new_filespec);
        xstrlcpy(new_filespec, GetPathUNX(filespec), SIZEOF(new_filespec));

        XSTRCPY( filename, parse_name(new_filespec) );
        XSTRCPY( pathname, parse_path(new_filespec) );
        XSTRCPY( drivename, parse_drive(new_filespec) );
    }
#else
    XSTRCPY( filename, parse_name(filespec) );
    XSTRCPY( pathname, parse_path(filespec) );
    XSTRCPY( drivename, parse_drive(filespec) );
#endif
    if ( pathname[0] == 0 )
        XSTRCPY(pathname, ".");

    /* merge the drive into the pathname */
    XSTRCAT(drivename, pathname);
    XSTRCPY(pathname, drivename);

# if  LOCKDEBUG
    printf("\nUnLocking [%s] [%s]\n", pathname, filename);
    tgetc();
# endif

    /* create the lock directory if it does not exist */
    XSTRCPY(lockpath, pathname);
    XSTRCAT(lockpath, DIRSEPSTR);
    XSTRCAT(lockpath, LOCKDIR);
# if  LOCKDEBUG
    printf("Lockdir [%s]\n", lockpath);
    tgetc();
# endif
    /* check for the existance of this lockfile */
    XSTRCPY(lockfile, lockpath);
    XSTRCAT(lockfile, DIRSEPSTR);
    XSTRCAT(lockfile, filename);
# if  LOCKDEBUG
    printf("Lockfile [%s]\n", lockfile);
    tgetc();
# endif
    if ( umc_unlink(lockfile) ) {
        XSTRCAT(result, "could not remove lock file");

        return (result);
    } else {
        rmdir(lockpath); /* this will work only if dir is empty */

        return (NULL);
    }
}


#else

VOID dohello P0_()
{
}

#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
