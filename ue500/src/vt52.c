/*======================================================================
 * The routines in this file provide support for VT52 style terminals
 * over a serial line. The serial I/O services are provided by routines
 * in "termio.c". It compiles into nothing if not a VT52 style device.
 * The bell on the VT52 is terrible, so the "beep" routine is
 * conditionalized on defining BEL.
 *====================================================================*/

/*====================================================================*/
#define VT52_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#define termdef 1                       /* don't define "term" external */

#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

#if     VT52

# define NROW    24                     /* Screen size.                 */
# define NCOL    80                     /* Edit if you want to.         */
# define MARGIN  8                      /* size of minimim margin and   */
# define SCRSIZ  64                     /* scroll size for extended lines */
# define NPAUSE  100                    /* # times thru update to pause */
# define BIAS    0x20                   /* Origin 0 coordinate bias.    */
# define ESC     0x1B                   /* ESC character.               */
# define BEL     0x07                   /* ascii bell character         */

EXTERN int     ttopen();                /* Forward references.          */
EXTERN int     ttgetc();
EXTERN int     ttputc();
EXTERN int     ttflush();
EXTERN int     ttclose();
EXTERN int     vt52move();
EXTERN int     vt52eeol();
EXTERN int     vt52eeop();
EXTERN int     vt52beep();
EXTERN int     vt52open();
EXTERN int     vt52rev();
EXTERN int     vt52cres();
EXTERN int     vt52kopen();
EXTERN int     vt52kclose();

# if     COLOR
EXTERN int     vt52fcol();
EXTERN int     vt52bcol();
# endif

/*
 * Dispatch table. All the hard fields just point into the terminal
 * I/O code.
 */
TERM  term  = {
    NROW-1,
    NROW-1,
    NCOL,
    NCOL,
    0, 0,
    MARGIN,
    SCRSIZ,
    NPAUSE,
    &vt52open,
    &ttclose,
    &vt52kopen,
    &vt52kclose,
    &ttgetc,
    &ttputc,
    &ttflush,
    &vt52move,
    &vt52eeol,
    &vt52eeop,
    &vt52eeop,
    &vt52beep,
    &vt52rev,
    &vt52cres
#if     COLOR
    , &vt52fcol,
    &vt52bcol
#endif
};

vt52move(row, col)
{
    ttputc(ESC);
    ttputc('Y');
    ttputc(row+BIAS);
    ttputc(col+BIAS);
}

vt52eeol()
{
    ttputc(ESC);
    ttputc('K');
}

vt52eeop()
{
    ttputc(ESC);
    ttputc('J');
}

vt52rev(status) /* set the reverse video state */

int status;     /* TRUE = reverse video, FALSE = normal video */

{
    /* can't do this here, so we won't */
}

vt52cres()      /* change screen resolution - (not here though) */
{
    return (TRUE);
}

spal()          /* change palette string */
{
    /*  Does nothing here   */
}

# if     COLOR
vt52fcol()      /* set the forground color [NOT IMPLIMENTED] */
{
}

vt52bcol()      /* set the background color [NOT IMPLIMENTED] */
{
}
# endif

vt52beep()
{
# ifdef  BEL
    ttputc(BEL);
    ttflush();
# endif
}

vt52open()
{
# if     BSD
    REGISTER char *cp;

    if ( ( cp = getenv("TERM") ) == NULL ) {
        puts(TEXT4);
/*                   "Shell variable TERM not defined!" */
        meexit(1);
    }
    if ( strcmp(cp, "vt52") != 0 && strcmp(cp, "z19") != 0 ) {
        puts(TEXT202);
/*                   "Terminal type not 'vt52'or 'z19' !" */
        meexit(1);
    }
# endif
    ttopen();
}

vt52kopen()
{
}

vt52kclose()
{
}


# if     FLABEL
fnclabel(f, n)          /* label a function key */

int f, n;        /* default flag, numeric argument [unused] */

{
    /* on machines with no function keys...don't bother */
    return (TRUE);
}
# endif
#else

vt52hello()
{
}

#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
