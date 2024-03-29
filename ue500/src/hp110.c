/*======================================================================
 *  HP110:  Hewlett Packard 110 Screen Driver
 *====================================================================*/

/*====================================================================*/
#define HP110_C_
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

#if     HP110

# define NROW    16                     /* Screen size.                 */
# define NCOL    80                     /* Edit if you want to.         */
# define NPAUSE  100                    /* # times thru update to pause */
# define MARGIN  8                      /* size of minimim margin and   */
# define SCRSIZ  64                     /* scroll size for extended lines */
# define BEL     0x07                   /* BEL character.               */
# define ESC     0x1B                   /* ESC character.               */

EXTERN PASCAL NEAR ttopen();               /* Forward references.          */
EXTERN PASCAL NEAR ttgetc();
EXTERN PASCAL NEAR ttputc();
EXTERN PASCAL NEAR ttflush();
EXTERN PASCAL NEAR ttclose();
EXTERN PASCAL NEAR h110move();
EXTERN PASCAL NEAR h110eeol();
EXTERN PASCAL NEAR h110eeop();
EXTERN PASCAL NEAR h110beep();
EXTERN PASCAL NEAR h110open();
EXTERN PASCAL NEAR h110rev();
EXTERN PASCAL NEAR h110cres();
EXTERN PASCAL NEAR h110close();
EXTERN PASCAL NEAR h110kopen();
EXTERN PASCAL NEAR h110kclose();
EXTERN PASCAL NEAR h110parm();

# if     COLOR
EXTERN PASCAL NEAR h110fcol();
EXTERN PASCAL NEAR h110bcol();

int cfcolor = -1;               /* current forground color */
int cbcolor = -1;               /* current background color */
# endif

/*
 * Standard terminal interface dispatch table. Most of the fields point into
 * "termio" code.
 */
TERM  term  = {
    NROW-1,
    NROW-1,
    NCOL,
    NCOL,
    MARGIN,
    SCRSIZ,
    NPAUSE,
    h110open,
    h110close,
    h110kopen,
    h110kclose,
    ttgetc,
    ttputc,
    ttflush,
    h110move,
    h110eeol,
    h110eeop,
    h110eeop,
    h110beep,
    h110rev,
    h110cres
#if     COLOR
    , h110fcol,
    h110bcol
#endif
};

# if     COLOR
PASCAL NEAR h110fcol(color)             /* set the current output color */

int color;      /* color to set */

{
    if ( color == cfcolor )
        return;

    ttputc(ESC);
    ttputc('[');
    h110parm(color+30);
    ttputc('m');
    cfcolor = color;
}

PASCAL NEAR h110bcol(color)             /* set the current background color */

int color;      /* color to set */

{
    if ( color == cbcolor )
        return;

    ttputc(ESC);
    ttputc('[');
    h110parm(color+40);
    ttputc('m');
    cbcolor = color;
}
# endif

PASCAL NEAR h110move(row, col)
{
    ttputc(ESC);
    ttputc('[');
    h110parm(row+1);
    ttputc(';');
    h110parm(col+1);
    ttputc('H');
}

PASCAL NEAR h110eeol()
{
    ttputc(ESC);
    ttputc('[');
    ttputc('0');
    ttputc('K');
}

PASCAL NEAR h110eeop()
{
# if     COLOR
    h110fcol(gfcolor);
    h110bcol(gbcolor);
# endif
    ttputc(ESC);
    ttputc('[');
    ttputc('0');
    ttputc('J');
}

PASCAL NEAR h110rev(state)              /* change reverse video state */

int state;      /* TRUE = reverse, FALSE = normal */

{
# if     COLOR
    int ftmp, btmp;             /* temporaries for colors */
# endif

    ttputc(ESC);
    ttputc('[');
    ttputc(state ? '7': '0');
    ttputc('m');
# if     COLOR
    if ( state == FALSE ) {
        ftmp = cfcolor;
        btmp = cbcolor;
        cfcolor = -1;
        cbcolor = -1;
        h110fcol(ftmp);
        h110bcol(btmp);
    }
# endif
}

PASCAL NEAR h110cres()  /* change screen resolution */
{
    return (TRUE);
}

PASCAL NEAR spal()              /* change pallette REGISTER */
{
    /*   not here */
}

PASCAL NEAR h110beep()
{
    ttputc(BEL);
    ttflush();
}

PASCAL NEAR h110parm(n)
REGISTER int n;
{
    REGISTER int q, r;

    q = n/10;
    if ( q != 0 ) {
        r = q/10;
        if ( r != 0 ) {
            ttputc( (r%10)+'0' );
        }
        ttputc( (q%10) + '0' );
    }
    ttputc( (n%10) + '0' );
}

PASCAL NEAR h110open()
{
    xstrcpy(sres, "15LINE");
    xstrcpy(os, "MSDOS");
    revexist = TRUE;
    ttopen();
}

PASCAL NEAR h110close()
{
# if     COLOR
    h110fcol(7);
    h110bcol(0);
# endif
    ttclose();
}

PASCAL NEAR h110kopen()
{
}

PASCAL NEAR h110kclose()
{
}

# if     FLABEL
PASCAL NEAR fnclabel(f, n)              /* label a function key */

int f, n;        /* default flag, numeric argument [unused] */

{
    /* on machines with no function keys...don't bother */
    return (TRUE);
}
# endif
#else
h110hello()
{
}
#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
