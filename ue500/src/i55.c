/*======================================================================
 * The routines in this file provide support for ANSI style terminals
 * over a serial line. The serial I/O services are provided by routines
 *  in termio.c. It compiles into nothing if not an ANSI device.
 *====================================================================*/

/*====================================================================*/
#define I55_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#define termdef 1                       /* don't define term external */

#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

#if     I55

EXTERN int PASCAL NEAR fnclabel DCL((int f, int n));
EXTERN int PASCAL NEAR readparam DCL((int *v));
EXTERN VOID PASCAL NEAR dobbnmouse DCL((void));
EXTERN VOID PASCAL NEAR docsi DCL((int oh));
EXTERN VOID PASCAL NEAR ttputs DCL((CONST char *string));

# define NROW    24                     /* Screen size.                 */
# define NCOL    80                     /* Edit if you want to.         */
# define NPAUSE  100                    /* # times thru update to pause */
# define MARGIN  8                      /* size of minimim margin and   */
# define SCRSIZ  64                     /* scroll size for extended lines */
# define BEL     0x07                   /* BEL character.               */
# define ESC     0x1B                   /* ESC character.               */

/* Forward references.          */
EXTERN int PASCAL NEAR i55move();
EXTERN int PASCAL NEAR i55eeol();
EXTERN int PASCAL NEAR i55eeop();
EXTERN int PASCAL NEAR i55beep();
EXTERN int PASCAL NEAR i55open();
EXTERN int PASCAL NEAR i55rev();
EXTERN int PASCAL NEAR i55close();
EXTERN int PASCAL NEAR i55kopen();
EXTERN int PASCAL NEAR i55kclose();
EXTERN int PASCAL NEAR i55cres();
EXTERN int PASCAL NEAR i55parm();
EXTERN int PASCAL NEAR i55getc();

# if     COLOR
EXTERN int PASCAL NEAR i55fcol();
EXTERN int PASCAL NEAR i55bcol();

static int cfcolor = -1;        /* current forground color */
static int cbcolor = -1;        /* current background color */
# endif

/*
 * Standard terminal interface dispatch table. Most of the fields point into
 * "termio" code.
 */
NOSHARE TERM  term  = {
    NROW-1,
    NROW-1,
    NCOL,
    NCOL,
    0, 0,
    MARGIN,
    SCRSIZ,
    NPAUSE,
    i55open,
    i55close,
    i55kopen,
    i55kclose,
    i55getc,
    ttputc,
    ttflush,
    i55move,
    i55eeol,
    i55eeop,
    i55eeop,
    i55beep,
    i55rev,
    i55cres
#if     COLOR
    , i55fcol,
    i55bcol
#endif
};

# if     COLOR
PASCAL NEAR i55fcol(color)              /* set the current output color */

int color;      /* color to set */

{
    color = color & 7;
    if ( color == cfcolor )
        return;

    ttputc(ESC);
    ttputc('[');
    i55parm(color+30);
    ttputc('m');
    cfcolor = color;
}

PASCAL NEAR i55bcol(color)              /* set the current background color */

int color;      /* color to set */

{
    color = color & 7;
    if ( color == cbcolor )
        return;

    ttputc(ESC);
    ttputc('[');
    i55parm(color+40);
    ttputc('m');
    cbcolor = color;
}
# endif

PASCAL NEAR i55move(row, col)
{
    ttputc(ESC);
    ttputc('[');
    i55parm(row+1);
    ttputc(';');
    i55parm(col+1);
    ttputc('H');
}

PASCAL NEAR i55eeol()
{
    ttputc(ESC);
    ttputc('[');
    ttputc('K');
}

PASCAL NEAR i55eeop()
{
# if     COLOR
    i55fcol(gfcolor);
    i55bcol(gbcolor);
# endif
    ttputc(ESC);
    ttputc('[');
    ttputc('J');
}

PASCAL NEAR i55rev(state)               /* change reverse video state */

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
        i55fcol(ftmp);
        i55bcol(btmp);
    }
# endif
}

PASCAL NEAR i55cres()   /* change screen resolution */
{
    return (TRUE);
}

PASCAL NEAR spal P1_(char *, dummy) /* change pallette settings */
{
    /* none for now */
}

PASCAL NEAR i55beep()
{
    ttputc(BEL);
    ttflush();
}

PASCAL NEAR i55parm(n)
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

PASCAL NEAR i55open()
{
    xstrcpy(sres, "NORMAL");
    revexist = TRUE;
    ttopen();

# if     KEYPAD
    ttputc(ESC);
    ttputc('=');
# endif
}

PASCAL NEAR i55close()
{
# if     COLOR
    i55fcol(7);
    i55bcol(0);
# endif
# if     KEYPAD
    ttputc(ESC);
    ttputc('>');
# endif
    ttclose();
}

PASCAL NEAR i55kopen()  /* open the keyboard (a noop here) */
{
}

PASCAL NEAR i55kclose() /* close the keyboard (a noop here) */
{
}

PASCAL NEAR  i55getc()
{
    return ( ttgetc() );
}

# if     FLABEL
int PASCAL NEAR fnclabel(f, n)          /* label a function key */

int f, n;        /* default flag, numeric argument [unused] */

{
    /* on machines with no function keys...don't bother */
    return (TRUE);
}
# endif
#else
i55hello()
{
}
#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
