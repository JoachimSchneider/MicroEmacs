/*======================================================================
 *  FILEIO.C:   Low level file i/o routines
 *              MicroEMACS 3.12
 *
 * The routines in this file read and write ASCII files from the disk.
 * All of the knowledge about files are here.
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
# define fopen    xxfopen
#endif

#if !(AOSVS | MV_UX)
static
#endif
NOSHARE FILE  *ffp;               /* File pointer, all functions. */
static int    eofflag;            /* end-of-file flag */


#if !(VMS & RMSIO)
/* If using RMS under VMS, the code following is in VMS.C */

# if    (MSC || TURBO || IC) && MSDOS
#  define FILE_BUFSIZE    C_4096
# else
#  define FILE_BUFSIZE    BUFSIZ   /* setbuf MUST use BUFSIZ */
# endif
static char file_buffer[FILE_BUFSIZE];

# if ( 1 )
/* Normally we'd expect the OS to do this kind of optimization: But on
 * - Linux with GNU libc version 2.40
 * - CYGWIN 3.3.5(0.341/5/3)
 * it does not --- JES, 2026-05-03.
 */
static int  uegetc P1_(FILE *, fp)
{
    static char buf[FILE_BUFSIZE];
    /* Position of next character to read. The buffer is filled by
     * fread() which reads at most SIZEOF(buf) charcters.
     * First position: 0
     * Last  position: SIZEOF(buf) - 1
     */
    static int  buf_pos   = (-1);
    static int  read_res  = (-1);

    if ( 0  > buf_pos  || read_res - 1 < buf_pos  ) {
        errno = 0;
        if ( 0 == (read_res = fread(buf, 1, SIZEOF(buf), fp)) ) {
            return EOF;
        }

        buf_pos = 0;
    }

    return (int)(unsigned char)buf[buf_pos++];
}

static int  ueputc P2_(int, c, FILE *, fp)
{
    static char buf[FILE_BUFSIZE];
    static int  buf_pos  = 0;           /* next free pos in buf */

    if ( EOF == c ) {                   /* write out */
        int rc  = 0;

        if ( 0 == buf_pos ) {
            return rc;
        }
        ASRT(0 < buf_pos && buf_pos <= SIZEOF(buf));
        errno = 0;
        if ( buf_pos != fwrite(buf, 1, buf_pos, fp) ) {
            rc  = (-1);
        }
        buf_pos = 0;

        return rc;
    }

    if ( SIZEOF(buf) <= buf_pos ) {
        if ( 0 > ueputc(EOF, fp) )  {   /* error */
            return EOF;
        }
    }
    buf[buf_pos++]  = (char)(unsigned char)c;

    return c;
}
#  define UEPUTC_USED   (1)
#  define UEGETC_USED   (1)
# else
#  define UEPUTC_USED   (0)
#  define UEGETC_USED   (0)
#  define uegetc        getc
#  define ueputc        putc
# endif

/* FFROPEN:
 *
 * Open a file for reading.
 */
int PASCAL NEAR ffropen P1_(CONST char *, fn)
{
    if ( ( ffp = fopen(fn, "r") ) == NULL ) {
        return FIOFNF;
    }

    /* tell the library to give us a LARGE buffer to speed I/O */
# if     (MSC || TURBO || IC) && MSDOS
    setvbuf(ffp, file_buffer, _IOFBF, FILE_BUFSIZE);
# else
    /* No setvbuf() for e.g.ANCIENT_UNIX */
    setbuf(ffp, file_buffer);
# endif

# if     WINDOW_MSWIN
    fbusy = FREADING;
# endif
    eofflag = FALSE;

    return FIOSUC;
}

/* FFWOPEN:
 *
 * Open a file for writing. Return TRUE if all is well, and FALSE on
 * error (cannot create).
 */
# if  !(AOSVS | MV_UX)
int PASCAL NEAR ffwopen P2_(CONST char *, fn, CONST char *, mode)
/* fn:    File name             */
/* mode:  Mode to open file for */
{
    char xmode[6];              /* extended file open mode */

    ZEROMEM(xmode);
    /* nonstandard line terminators? */
    if ( *lterm ) {
        /* open in binary mode */
        BUFCPY(xmode, mode);
        BUFCAT(xmode, "b");
        ffp = fopen(fn, xmode);
    } else        {
        /* open in ascii(text) mode */
        ffp = fopen(fn, mode);
    }

    if ( ffp == NULL ) {
        mlwrite(TEXT155);
/*              "Cannot open file for writing" */

        return (FIOERR);
    }

    /* tell the library to give us a LARGE buffer to speed I/O */
# if     (MSC || TURBO || IC) && MSDOS
    setvbuf(ffp, file_buffer, _IOFBF, FILE_BUFSIZE);
# else
    /* No setvbuf() for e.g.ANCIENT_UNIX */
    setbuf(ffp, file_buffer);
# endif

#  if     WINDOW_MSWIN
    fbusy = FWRITING;
#  endif

    return FIOSUC;
}
# endif

/* FFCLOSE:
 *
 * Close a file. Should look at the status in all systems.
 */
int PASCAL NEAR ffclose P0_()
{
    /* Free this since we do not need it anymore */
    CLROOM(fline);

# if     WINDOW_MSWIN
    fbusy = FALSE;
# endif
# if     MSDOS & CTRLZ
    ueputc('Z' - 'A' + 1, ffp);   /* add a ^Z at the end of the file */
# endif
# if UEPUTC_USED
    ueputc(EOF, ffp);             /* write out */
# endif

# if ( b_IS_UNIX || WMCS || VMS                               \
      || (MSDOS && ( LATTICE || MSC || TURBO || IC || ZTC) )  \
      || WINNT || WINXP || OS2 | ( TOS && MWC) )

    if ( EOF == fclose(ffp) ) {
        mlwrite(TEXT156);
/*              "Error closing file" */

        return FIOERR;
    }

    return FIOSUC;

# else

    fclose(ffp);

    return FIOSUC;

# endif
}

/* FFPUTLINE:
 *
 * Write a line to the already opened file. The "buf" points to the
 * buffer, and the "nbuf" is its length, less the free newline. Return
 * the status. Check only at the newline.
 */
int PASCAL NEAR ffputline P2_(char *, buf, int, nbuf)
{
    REGISTER int  i     = 0;        /* index into line to write */
    REGISTER char *lptr = NULL;     /* ptr into the line terminator */
# if    CRYPT
    char          c     = '\0';     /* character to translate */

    if ( cryptflag )  {
        for ( i = 0; i < nbuf; ++i ) {
            c = buf[i];
            ecrypt(&c, 1);
            ueputc(c, ffp);
        }
    } else            {
        for ( i = 0; i < nbuf; ++i )  {
            ueputc(buf[i], ffp);
        }
    }
# else
    for ( i = 0; i < nbuf; ++i )  {
        ueputc(buf[i], ffp);
    }
# endif

    /* write out the appropriate line terminator(s) */
    if ( *lterm ) {
        lptr = &lterm[0];
        while ( *lptr ) {
            ueputc(*lptr++, ffp);
        }
    } else        {
        ueputc('\n', ffp);
    }

    /* check for write errors */
    if ( ferror(ffp) )  {
        mlwrite(TEXT157);
/*              "Write I/O error" */

        return FIOERR;
    }

# if     WINDOW_MSWIN
    {
        static int o = 0;

        if ( --o < 0 )  {
            longop(TRUE);
            o = 10;   /* to lower overhead, only 10% calls to longop */
        }
    }
# endif

    return FIOSUC;
}

/* FFGETLINE:
 *
 * Read a line from a file, and store the bytes in the supplied buffer.
 * The "nbuf" is the length of the buffer. Complain about long lines
 * and lines at the end of the file that don't have a newline present.
 * Check for I/O errors too. Return status.
 */
int PASCAL NEAR ffgetline P1_(int *, nbytes)
{
    REGISTER int  c = 0;    /* current character read   */
    REGISTER int  i = 0;    /* current index into fline */

    /* if we are at the end...return it */
    if ( eofflag )  {
        return FIOEOF;
    }

    /* dump fline if it ended up too big */
    if ( flen > NSTRING && fline != NULL )  {
        CLROOM(fline);
    }

    /* if we don't have an fline, allocate one */
    if ( fline == NULL )  {
        if ( ( fline = ROOM(flen = NSTRING) ) == NULL ) {
            return FIOMEM;
        }
    }

    /* read the line in */
    while ( ( c = uegetc(ffp) ) != EOF && c != '\n' ) {
        fline[i++] = c;
        /* if it's longer, get more room */
        if ( i >= flen ) {
            /* Double flen and re-allocate but make shure that flen
             * won't overflow:
             * The maximum positive value of an n-bit signed int in
             * two's complement representation is 2^(n-1) - 1.
             * That is flen needs to be less than 2^(n-2) which is
             * 2^(SIZEOF(flen) * BITS_IN_BYTE - 2)
             * == 1 << (SIZEOF(flen) * BITS_IN_BYTE - 2).
             */
            if ( (ulong_t)flen >=
                  (ulong_t)(1 << (SIZEOF(flen) * BITS_IN_BYTE - 2)) ) {

                return FIOMEM;
            }
            flen *= 2;
            if ( ( fline = REROOM(fline, flen) ) == NULL ) {
                return FIOMEM;
            }
        }
    }

    /* dump any extra line terminators at the end */
    while ( i > 0 && (fline[i - 1] == '\n' || fline[i - 1] == '\r') ) {
        i--;
    }

    /* and save the length for our caller... */
    *nbytes = i;

    /* we should be ready to dump leading terminators too - ADD THIS DAN */

    /* test for any errors that may have occured */
    if ( c == EOF ) {
        if ( ferror(ffp) ) {
            mlwrite(TEXT158);
/*                  "File read error" */

            return FIOERR;
        }

        if ( i != 0 ) {
            eofflag = TRUE;
        } else        {
            return FIOEOF;
        }
    }

    /* terminate and decrypt the string */
    fline[i] = 0;
# if     CRYPT
    if ( cryptflag )  {
        ecrypt(fline, STRLEN(fline));
    }
# endif

    return FIOSUC;
}

#endif  /* !(VMS & RMSIO) */


/* FEXIST:
 *
 * does <fname> exist on disk?
 */
int PASCAL NEAR fexist P1_(CONST char *, fname)
/* fname: File to check for existance */
{
    FILE  *fp = NULL;

    /* try to open the file for reading */
    fp = fopen(fname, "r");

    /* if it fails, just return false! */
    if ( fp == NULL ) {
        return FALSE;
    }

    /* otherwise, close it and report true */
    fclose(fp);

    return TRUE;
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
