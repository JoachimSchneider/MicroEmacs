/*======================================================================
 *  FILEIO.C:   Low level file i/o routines
 *              MicroEMACS 3.12
 *
 * The routines in this file read and write ASCII files from the disk. All of
 * the knowledge about files are here.
 *====================================================================*/

/*====================================================================*/
#define FILEIO_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

#if     AOSVS | MV_UX
# define fopen   xxfopen
#endif

#if !(AOSVS | MV_UX)
static
#endif
NOSHARE FILE *ffp;              /* File pointer, all functions. */
static int eofflag;             /* end-of-file flag */

#if     (MSC || TURBO || IC) && MSDOS
# define FILE_BUFSIZE    4096
static char file_buffer[FILE_BUFSIZE];
#endif

#if !(VMS & RMSIO)
/* If using RMS under VMS, the code following is in VMS.C */

/* FFROPEN:
 *
 * Open a file for reading.
 */
int PASCAL NEAR ffropen P1_(CONST char *, fn)
{
    if ( ( ffp=fopen(fn, "r") ) == NULL )
        return (FIOFNF);

# if     (MSC || TURBO || IC) && MSDOS
    /* tell the library to give us a LARGE buffer to speed I/O */
    setvbuf(ffp, file_buffer, _IOFBF, FILE_BUFSIZE);
# endif

# if     WINDOW_MSWIN
    fbusy = FREADING;
# endif
    eofflag = FALSE;

    return (FIOSUC);
}

/*
 * Open a file for writing. Return TRUE if all is well, and FALSE on error
 * (cannot create).
 */
# if  !(AOSVS | MV_UX)
int PASCAL NEAR ffwopen P2_(CONST char *, fn, CONST char *, mode)
/* fn:    File name             */
/* mode:  Mode to open file for */
{
    char xmode[6];              /* extended file open mode */

    /* nonstandard line terminators? */
    if ( *lterm ) {
        /* open in binary mode */
        XSTRCPY(xmode, mode);
        XSTRCAT(xmode, "b");
        ffp = fopen(fn, xmode);
    } else {
        /* open in ascii(text) mode */
        ffp = fopen(fn, mode);
    }

    if ( ffp == NULL ) {
        mlwrite(TEXT155);

/*          "Cannot open file for writing" */
        return (FIOERR);
    }

#  if     (MSC || TURBO || IC) && MSDOS
    /* tell the library to give us a LARGE buffer to speed I/O */
    setvbuf(ffp, file_buffer, _IOFBF, FILE_BUFSIZE);
#  endif

#  if     WINDOW_MSWIN
    fbusy = FWRITING;
#  endif

    return (FIOSUC);
}
# endif

/* FFCLOSE:
 *
 * Close a file. Should look at the status in all systems.
 */
int PASCAL NEAR ffclose P0_()
{
    /* free this since we do not need it anymore */
    if ( fline ) {
        free(fline);
        fline = NULL;
    }

# if     WINDOW_MSWIN
    fbusy = FALSE;
# endif
# if     MSDOS & CTRLZ
    putc(26, ffp);              /* add a ^Z at the end of the file */
# endif

# if ( IS_UNIX() || WMCS || VMS || (MSDOS && ( LATTICE || MSC || TURBO || IC || \
                                               ZTC) ) || WINNT || WINXP || \
    OS2 | ( TOS && MWC) )
    if ( fclose(ffp) != FALSE ) {
        mlwrite(TEXT156);

/*                      "Error closing file" */
        return (FIOERR);
    }

    return (FIOSUC);

# else
    fclose(ffp);

    return (FIOSUC);

# endif
}

/* FFPUTLINE:
 *
 * Write a line to the already opened file. The "buf" points to the buffer, and
 * the "nbuf" is its length, less the free newline. Return the status. Check
 * only at the newline.
 */
int PASCAL NEAR ffputline P2_(char *, buf, int, nbuf)
{
    REGISTER int i;             /* index into line to write */
    REGISTER char *lptr;        /* ptr into the line terminator */
# if     CRYPT
    char c;             /* character to translate */

    if ( cryptflag ) {
        for ( i = 0; i < nbuf; ++i ) {
            c = buf[i];
            ecrypt(&c, 1);
            putc(c, ffp);
        }
    } else
        for ( i = 0; i < nbuf; ++i )
            putc(buf[i], ffp);

# else
    for ( i = 0; i < nbuf; ++i )
        putc(buf[i], ffp);
# endif

    /* write out the appropriate line terminator(s) */
    if ( *lterm ) {
        lptr = &lterm[0];
        while ( *lptr )
            putc(*lptr++, ffp);
    } else {
        putc('\n', ffp);
    }

    /* check for write errors */
    if ( ferror(ffp) ) {
        mlwrite(TEXT157);

/*                      "Write I/O error" */
        return (FIOERR);
    }

# if     WINDOW_MSWIN
    {
        static int o = 0;
        if ( --o < 0 ) {
            longop(TRUE);
            o = 10;        /* to lower overhead, only 10% calls to longop */
        }
    }
# endif

    return (FIOSUC);
}

/* FFGETLINE:
 *
 * Read a line from a file, and store the bytes in the supplied buffer. The
 * "nbuf" is the length of the buffer. Complain about long lines and lines at
 * the end of the file that don't have a newline present. Check for I/O errors
 * too. Return status.
 */
int PASCAL NEAR ffgetline P1_(int *, nbytes)
{
    REGISTER int c;             /* current character read */
    REGISTER int i;             /* current index into fline */

    /* if we are at the end...return it */
    if ( eofflag )
        return (FIOEOF);

    /* dump fline if it ended up too big */
    if ( flen > NSTRING && fline != NULL ) {
        free(fline);
        fline = NULL;
    }

    /* if we don't have an fline, allocate one */
    if ( fline == NULL )
        if ( ( fline = room(flen = NSTRING) ) == NULL )
            return (FIOMEM);



    /* read the line in */
    i = 0;
    while ( ( c = getc(ffp) ) != EOF && c != '\n' ) {
        fline[i++] = c;
        /* if it's longer, get more room */
        if ( i >= flen ) {
# if     MSDOS
            if ( flen >= 16636 )
                return (FIOMEM);

# endif
            flen *= 2;
            if ( ( fline = reroom(fline, flen) ) == NULL ) {
                return (FIOMEM);
            }
        }
    }

    /* dump any extra line terminators at the end */
    while ( i > 0 && (fline[i-1] == 10 || fline[i-1] == 13) )
        i--;

    /* and save the length for our caller... */
    *nbytes = i;

    /* we should be ready to dump leading terminators too - ADD THIS DAN */

    /* test for any errors that may have occured */
    if ( c == EOF ) {
        if ( ferror(ffp) ) {
            mlwrite(TEXT158);

/*                              "File read error" */
            return (FIOERR);
        }

        if ( i != 0 )
            eofflag = TRUE;
        else
            return (FIOEOF);
    }

    /* terminate and decrypt the string */
    fline[i] = 0;
# if     CRYPT
    if ( cryptflag )
        ecrypt( fline, STRLEN(fline) );
# endif

    return (FIOSUC);
}

#endif  /* !(VMS & RMSIO) */

/* FEXIST:
 *
 * does <fname> exist on disk?
 */
int PASCAL NEAR fexist P1_(CONST char *, fname)
/* fname: File to check for existance */
{
    FILE *fp;

    /* try to open the file for reading */
    fp = fopen(fname, "r");

    /* if it fails, just return false! */
    if ( fp == NULL )
        return (FALSE);

    /* otherwise, close it and report true */
    fclose(fp);

    return (TRUE);
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
