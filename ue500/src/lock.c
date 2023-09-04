/*======================================================================
 *  LOCK:   File locking command routines for MicroEMACS written by
 *          Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#define LOCK_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include <stdio.h>
#include "estruct.h"
#include "eproto.h"
#include "edef.h"
#include "elang.h"

#if     FILOCK

# if ( IS_UNIX() || WMCS )
#  include <string.h>
#  include <sys/errno.h>
# endif

# if     MSC || VMS
#  include <errno.h>
# else
COMMON int errno;       /* current error  */
# endif

static char *lname[NLOCKS]; /* names of all locked files  */
static int  numlocks;       /* # of current locks active  */

/* LOCKCHK:
 *
 * Check a file for locking and add it to the list
 */
int lockchk P1_(CONST char *, fname /* file to check for a lock */)
{
    REGISTER int  i       = 0;    /* loop indexes   */
    REGISTER int  status  = 0;    /* return status  */

    /* check to see if that file is already locked here */
    if ( numlocks > 0 )
        for ( i=0; i < numlocks; ++i )
            if ( strcmp(fname, lname[i]) == 0 )
                return (TRUE);

    /* if we have a full locking table, bitch and leave */
    if ( numlocks == NLOCKS ) {
        mlwrite(TEXT173);

/*                      "LOCK ERROR: Lock table full" */
        return (ABORT);
    }

    /* next, try to lock it */
    status = xlock(fname);
    if ( status == ABORT )      /* file is locked, no override */
        return (ABORT);

    if ( status == FALSE )      /* locked, overriden, dont add to table */
        return (TRUE);

    /* we have now locked it, add it to our table */
    lname[++numlocks - 1] = (char *)room(strlen(fname) + 1);
    if ( lname[numlocks - 1] == NULL ) {        /* room failure */
        undolock(fname);                        /* free the lock */
        mlwrite(TEXT174);
/*                      "Cannot lock, out of memory" */
        --numlocks;

        return (ABORT);
    }

    /* everthing is cool, add it to the table */
    xstrlcpy(lname[numlocks - 1], fname, SIZEOF(lname) - (numlocks - 1));

    return (TRUE);
}

/* LOCKREL:
 *
 * Release all the file locks so others may edit
 */
int lockrel P0_()
{
    REGISTER int  status  = 0;    /* status of locks      */
    REGISTER int  s       = 0;    /* status of one unlock */

    status = TRUE;
    while ( numlocks-- > 0 ) {
        if ( ( s = xunlock(lname[numlocks]) ) != TRUE )
            status = s;
        free(lname[numlocks]);
    }

    return (status);
}

/* XLOCK:
 *
 * Check and lock a file from access by others returns TRUE = files was
 * not locked and now is FALSE = file was locked and overridden ABORT = file was
 * locked, abort command
 */

int xlock P1_(CONST char *, fname /* file name to lock */)
{
    REGISTER char *locker = NULL; /* lock error message */
    REGISTER int  status  = 0;    /* return status      */
    char msg[NSTRING];            /* message string     */

    ZEROMEM(msg);

    /* attempt to lock the file */
    locker = dolock(fname);
    if ( locker == NULL )       /* we win */
        return (TRUE);

    /* file failed...abort */
    if ( strncmp(locker, "LOCK", SIZEOF ("LOCK") - 1) == 0 ) {
        lckerror(locker);

        return (ABORT);
    }

    /* someone else has it....override? */
    XSTRCPY(msg, TEXT176);
/*                  "File in use by " */
    XSTRCAT(msg, locker);
    XSTRCAT(msg, TEXT177);
/*                  ", overide?" */
    status = mlyesno(msg);              /* ask them */
    if ( status == TRUE )
        return (FALSE);
    else
        return (ABORT);
}

/* XUNLOCK:
 *
 * Unlock a file this only warns the user if it fails
 */
int xunlock P1_(char *, fname /* file to unlock */)
{
    REGISTER char *locker = NULL;   /* undolock return string */

    /* unclock and return */
    locker = undolock(fname);
    if ( locker == NULL )
        return (TRUE);

    /* report the error and come back */
    lckerror(locker);

    return (FALSE);
}

/* LCKERROR:
 *
 * Report a lock error
 */
VOID lckerror P1_(char *, errstr /* lock error string to print out */)
{
    char obuf[NSTRING];   /* output buffer for error message */
    char *sys_errstr = strerror(errno);

    XSTRCPY(obuf, errstr);
    XSTRCAT(obuf, " - ");
# if ( IS_UNIX() || WMCS )
    if ( sys_errstr && *sys_errstr )
        XSTRCAT(obuf, sys_errstr);
    else
        XSTRCAT(obuf, TEXT178);
/*                           "[can not get system error message]" */
# else
    XSTRCAT(obuf, "Error # ");
    XSTRCAT( obuf, int_asc(errno) );
# endif
    mlwrite(obuf);
    update(TRUE);
}

#else

VOID lckhello P0_()   /* dummy function */
{
}

#endif  /* FILOCK */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
