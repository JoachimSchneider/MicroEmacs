/*======================================================================
 * The routines in this file provide support for ANSI style terminals
 * over a serial line. The serial I/O services are provided by routines
 * in termio.c. It compiles into nothing if not an ANSI device.
 *====================================================================*/

/*====================================================================*/
#define ANSI_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#define termdef 1                       /* don't define term external */
/* don't define term external */

#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

#if     ANSI

EXTERN int  PASCAL NEAR fnclabel    DCL((int f, int n));
EXTERN int  PASCAL NEAR readparam   DCL((int *v));
EXTERN VOID PASCAL NEAR dobbnmouse  DCL((void));
EXTERN VOID PASCAL NEAR docsi       DCL((int oh));
EXTERN VOID PASCAL NEAR ttputs      DCL((CONST char *string));

# if VMS
#  include <ttdef.h>
#  include <tt2def.h>

/*
 * This structure, along with ttdef.h, is good for manipulating
 * terminal characteristics.
 */
typedef struct {
    /* Terminal characteristics buffer */
    unsigned char   catgy;  /* Renamed from `class' to allow C++ compile  */
    unsigned char   type;
    unsigned short  width;
    unsigned int    tt1 : 24;
    unsigned char   page;
    unsigned long   tt2;
} TTCHAR;
COMMON NOSHARE TTCHAR orgchar;  /* Original characteristics */
# endif /* VMS */

# define NROW    25         /* Screen size.                   */
# define NCOL    80         /* Edit if you want to.           */
# define NPAUSE  100        /* # times thru update to pause   */
# define MARGIN  8          /* size of minimim margin and     */
# define SCRSIZ  64         /* scroll size for extended lines */
# define BEL     0x07       /* BEL character.                 */
# define ESC     0x1B       /* ESC character.                 */

/* Forward references.          */
static int PASCAL NEAR  ansimove    DCL((int row, int col));
static int PASCAL NEAR  ansieeol    DCL((void));
static int PASCAL NEAR  ansieeop    DCL((void));
static int PASCAL NEAR  ansibeep    DCL((void));
static int PASCAL NEAR  ansiopen    DCL((void));
static int PASCAL NEAR  ansirev     DCL((int state));
static int PASCAL NEAR  ansiclose   DCL((void));
static int PASCAL NEAR  ansikopen   DCL((void));
static int PASCAL NEAR  ansikclose  DCL((void));
static int  PASCAL NEAR ansicres    DCL((char * dummy));
static VOID PASCAL NEAR ansiparm    DCL((int n));
static int  PASCAL NEAR ansigetc    DCL((void));

# if     COLOR
static int  PASCAL NEAR ansifcol    DCL((int color));
static int  PASCAL NEAR ansibcol    DCL((int color));
static int rev_state = FALSE;

static int cfcolor = -1;        /* current foreground color */
static int cbcolor = -1;        /* current background color */

#  if     AMIGA
/*
 * Apparently the AMIGA does not follow the ANSI standards as regards
 * to colors....maybe because of the default pallette settings? Color
 * translation table needed.
 */

int coltran[16] =
{
    2, 3, 5, 7, 0, 4, 6, 1, 8, 12, 10, 14, 9, 13, 11, 15
};
#  endif
# endif

/*
 * Standard terminal interface dispatch table. Most of the fields point into
 * "termio" code.
 */
NOSHARE TERM term = {
    NROW-1,
    NROW-1,
    NCOL,
    NCOL,
    0, 0,
    MARGIN,
    SCRSIZ,
    NPAUSE,
    ansiopen,
    ansiclose,
    ansikopen,
    ansikclose,
    ansigetc,
    ttputc,
    ttflush,
    ansimove,
    ansieeol,
    ansieeop,
    ansieeop,
    ansibeep,
    ansirev,
    ansicres
# if    COLOR
    , ansifcol,
    ansibcol
#endif
};

# if    COLOR
/* ANSIFCOL:
 *
 * Set the current output color
 */
static int PASCAL NEAR ansifcol P1_(int, color)
/* color: Color to set  */
{
    if ( color == cfcolor )
        return 0;

    ttputc(ESC);
    ttputc('[');
#  if     MSDOS
    if ( color > 7 ) {
        ansiparm(1);
        ttputc(';');
        ansiparm( (color&7)+30 );
    } else {
        ansiparm(0);
        ttputc(';');
        ansiparm(cbcolor+40);
        ttputc(';');
        ansiparm(color+30);
    }
#  else
#   if     AMIGA
    ansiparm(coltran[color]+30);
#   else
    ansiparm(color+30);
#   endif
#  endif
    ttputc('m');
    cfcolor = color;

    return 0;
}

/* ANSIBCOL:
 *
 * Set the current background color
 */
static int PASCAL NEAR ansibcol P1_(int, color)
/* color: Color to set  */
{
    if ( color == cbcolor )
        return 0;

    ttputc(ESC);
    ttputc('[');
#  if     AMIGA
    ansiparm(coltran[color]+40);
#  else
    ansiparm( (color&7)+40 );
#  endif
    ttputc('m');
    cbcolor = color;

    return 0;
}
# endif /* COLOR */

static int PASCAL NEAR ansimove P2_(int, row, int, col)
{
    ttputc(ESC);
    ttputc('[');
    ansiparm(row+1);
    ttputc(';');
    ansiparm(col+1);
    ttputc('H');

    return 0;
}

static int PASCAL NEAR ansieeol P0_()
{
    ttputc(ESC);
    ttputc('[');
    ttputc('K');

    return 0;
}

static int PASCAL NEAR ansieeop P0_()
{
# if     COLOR
    ansifcol(gfcolor);
    ansibcol(gbcolor);
# endif
    ttputc(ESC);
    ttputc('[');
    ttputc('0');
    ttputc('J');

    return 0;
}

/* ANSIREV:
 *
 * Change reverse video state
 */
static int PASCAL NEAR ansirev P1_(int, state)
/* state: TRUE = reverse, FALSE = normal  */
{
# if     COLOR
    /* temporaries for colors:  */
    int ftmp  = 0;
    int btmp  = 0;

    if ( state != rev_state ) {
        ftmp = cfcolor;
        btmp = cbcolor;
        cfcolor = -1;
        cbcolor = -1;
        ansifcol(btmp);
        ansibcol(ftmp);
        rev_state = state;
    }
# else
    ttputc(ESC);
    ttputc('[');
    ttputc(state ? '7': '0');
    ttputc('m');
# endif

    return 0;
}

/* ANSICRES:
 *
 * Change screen resolution
 */
static int PASCAL NEAR ansicres P1_(char *, dummy)
{
    return (TRUE);
}

/* SPAL:
 *
 * Change pallette settings
 */
int PASCAL NEAR spal P1_(char *, dummy)
{
    /* none for now */

    return 0;
}

static int PASCAL NEAR ansibeep P0_()
{
    ttputc(BEL);
    ttflush();

    return 0;
}

static VOID PASCAL NEAR ansiparm P1_(int, n)
{
    REGISTER int  q = 0;
    REGISTER int  r = 0;

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

static int PASCAL NEAR ansiopen P0_()
{
# if     IS_UNIX()
    REGISTER char *cp = NULL;

    if ( ( cp = getenv("TERM") ) == NULL ) {
        fputs(TEXT4, stderr);
        TRC(("%s", TEXT4));
/*                   "Shell variable TERM not defined!" */

# if ( 0 )
        meexit(1);
# endif
    }
    if ( strcmp(cp, "vt100") != 0 &&
         strcmp(cp, "vt200") != 0 &&
         strcmp(cp, "vt300") != 0 ) {
        fputs(TEXT5, stderr);
        TRC(("%s", TEXT5));
/*                   "Terminal type not 'vt100'!" */

# if ( 0 )
        meexit(1);
# endif
    }
# endif
# if     MOUSE && (IS_UNIX || VMS)
   /*
    * If this is an ansi terminal of at least DEC level 2 capability,
    * some terminals of this level, such as the "Whack" emulator, the
    * VWS terminal emulator, and some versions of XTERM, support access
    * to the workstation mouse via escape sequences. In addition, any
    * terminal that conforms to level 2 will, at very least, IGNORE the
    * escape sequences for the mouse.
    */
    {
        char  *s  = NULL;

        s = getenv("MICROEMACS$MOUSE_ENABLE");
        if ( !s ) s = "\033[1)u\033[1;3'{\033[1;2'z";
        ttputs(s);
    }
# endif
    xstrcpy(sres, "NORMAL");
    revexist = TRUE;
    ttopen();

# if     KEYPAD
    ttputc(ESC);
    ttputc('=');
# endif

    return 0;
}

static int PASCAL NEAR ansiclose P0_()
{
# if     COLOR
    ansifcol(7);
    ansibcol(0);
# endif
# if     MOUSE && (IS_UNIX() || VMS)
    {
        CONST char  *s  = NULL;

        s = getenv("MICROEMACS$MOUSE_DISABLE");

        if ( !s )               /* Regular DEC workstation */
            s = "\033[0'{\033[0;0'z";
        ttputs(s);
    }
# endif
# if     KEYPAD
#  if     VMS
    if ( (orgchar.tt2 & TT2$M_APP_KEYPAD)==0 )
#  endif
    {
        ttputc(ESC);
        ttputc('>');
    }
# endif
    ttclose();

    return 0;
}

/* ANSIKOPEN:
 *
 * Open the keyboard (a noop here)
 */
static int PASCAL NEAR ansikopen P0_()
{
    return 0;
}

/* ANSIKCLOSE:
 *
 * close the keyboard (a noop here)
 */
static int PASCAL NEAR ansikclose P0_()
{
    return 0;
}

# if   IS_UNIX() || VMS
/***
 *  ttputs  -  Send a string to ttputc
 *
 *  Nothing returned
 ***/
VOID PASCAL NEAR ttputs P1_(CONST char *, string)
/* string:  String to write */
{
    if ( string )
        while ( *string != '\0' )
            ttputc(*string++);
}

/*
 * On the "real" ansi terminals, used on "mainframe" type
 * terminal/CPU connections of the above operating systems, we do
 * conversion from VT100/VT200 style function keys into the Emacs
 * standard key sequence form.
 */
static unsigned char  inbuffer[10];
static int            inpos = 0;
static int            inlen = 0;

static NOSHARE int  mouserow;
static NOSHARE int  mousecol;

/* READPARAM:
 *
 * Read an ansi parameter
 */
int PASCAL NEAR readparam P1_(int *, v)
/* v: Place to put parameter value  */
{
    int ch  = 0;

    *v = 0;
    for (;;)  { /* Read in a number */
        ch = ttgetc();
        if ( ch >= '0' && ch <= '9' ) *v = 10 * *v + (ch - '0');
        else return ( ch );
    }
}

/* DOCSI:
 *
 * Handle the CSI (<esc>[) and SS3 (<esc>O) sequences. Static arrays
 * are set up to translate the ANSI escape sequence into the MicroEMACS
 * keycode.
 *
 * The 'x' in the arrays keypad[] and dec_fnkey[] are merely
 * placeholders.
 */
VOID PASCAL NEAR docsi P1_(int, oh)
{
    static char crsr[4] = { 'P', 'N', 'F', 'B' };
    static char keypad[14] =
    {
        ',', '-', '.', 'x', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    static char dec_fnkey[32] =
    {
        'x', 'S', 'C', 'D', '@', 'Z', 'V', 'x', '1', '2', '3', '4', '5', 'x',
        '6', '7', '8', '9', '0', 'x', '1', '2', '3', '4', 'x', '5', '6', 'x',
        '7', '8', '9', '0',
    };

    int params[5];
    int i   = 0;
    int ch  = 0;

    ZEROMEM(params);

    for ( i = 0 ;; )  {
        ch = readparam(&params[i]);
        if ( ch >= '@' ) { /* This ends the sequence, check for the ones we care
                            * about */
            mousecol = params[0];
            mouserow = params[1];
            if ( ch == 'R' && oh != 'O' ) { /* Cursor pos report */
                inbuffer[inlen++] = 0x0;
                inbuffer[inlen++] = MOUS>>8;
                inbuffer[inlen++] = mouserow;
                inbuffer[inlen++] = mousecol;
                inbuffer[inlen++] = '1';
            } else if ( ch == '~' ) {/* LK201 function key */
                inbuffer[inlen++] = 0x0;
                if ( params[0] > 8 ) params[0] -= 3;
                if ( params[0] > 18 )
                    inbuffer[inlen++] = (SHFT|SPEC)>>8;
                else
                    inbuffer[inlen++] = SPEC>>8;
                inbuffer[inlen++] = dec_fnkey[params[0]];
            } else if ( ch == 'w' && oh != 'O' ) { /* mouse report */
                mousecol = params[3]-1;
                mouserow = params[2]-1;
                inbuffer[inlen++] = 0x0;
                inbuffer[inlen++] = MOUS>>8;
                inbuffer[inlen++] = mousecol;
                inbuffer[inlen++] = mouserow;
                inbuffer[inlen++] = ('a'-2)+params[0];
            } else if ( ch == 'd' && oh != 'O' ) { /* Pixette mouse report */
                mousecol = params[0]-1;
                mouserow = params[1]-1;
                inbuffer[inlen++] = 0x0;
                inbuffer[inlen++] = MOUS>>8;
                inbuffer[inlen++] = mousecol;
                inbuffer[inlen++] = mouserow;
                inbuffer[inlen++] = ('a'-2)+params[2];
            } else { /* Ordinary keypad or arrow key */
                inbuffer[inlen++] = 0x0;
                if ( ch <= 'D' && ch >= 'A' ) { /* Arrow keys.*/
                    inbuffer[inlen++] = (SPEC)>>8;
                    inbuffer[inlen++] = crsr[ch - 'A'];
                } else if ( ch <= 'S' && ch >= 'P' ) { /* PF keys.*/
                    inbuffer[inlen++] = (SPEC|CTRL)>>8;
                    inbuffer[inlen++] = ch - ('P' - '1');
                } else {
                    inbuffer[inlen++] = (ALTD)>>8;
                    if ( ch == 'M' )
                        inbuffer[inlen++] = 'E';
                    else
                        inbuffer[inlen++] = keypad[ch - 'l'];
                }
            }

            return;
        }
        if ( i < NELEM(params) ) i++;
    }
}

VOID PASCAL NEAR dobbnmouse P0_()
{
    int params[5];
    int i     = 0;
    int ch    = 0;
    int event = 0;
    int flags = 0;
    static char prev  = 0;

    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    params[4] = 0;
    for ( i=0;; ) {
        /*
         * Is the sequence finished?
         * check for the ones we care about.
         */
        if ( ( ch = readparam(&params[i]) ) >= '@' ) {
            mousecol = (params[1]+4)/9;
            mouserow = (1015-params[2])/16;
            flags = params[3] & 7;
            event = flags ^ prev;
            prev = flags;
            flags = ( (flags & event) == 0 );
            event = flags + ( 6 - (event & 6) );
            if ( ch == 'c' ) {  /* Cursor pos report */
                inbuffer[inlen++] = 0x0;
                inbuffer[inlen++] = MOUS>>8;
                inbuffer[inlen++] = mousecol;
                inbuffer[inlen++] = mouserow;
                inbuffer[inlen++] = ('a'-2)+event;
            }

            return;
        }
        if ( i < 5 ) i++;
    }
}

/* ANSIGETC:
 *
 * Read a keystroke from the terminal. Interpret escape sequences that
 * come from function keys, mouse reports, and cursor location reports,
 * and return them using Emacs's coding of these events.
 */
static int PASCAL NEAR ansigetc P0_()
{
    int ch  = 0;

    for (;;)  { /* Until we get a character to return */
        if ( inpos < inlen )  {
            /* Working out a multi-byte input sequence  */

            return ( inbuffer[inpos++]);
        }
        inpos = 0;
        inlen = 0;
        ch = ttgetc();
        if ( ch == 27 ) {   /* ESC, see if sequence follows */
            /*
             * If the "terminator" is ESC, and if we are currently
             * reading a string where the terminator is ESC, then
             * return the ESC and do not allow function keys or mouse
             * to operate properly. This makes VT100 users much
             * happier.
             */
            ch = grabnowait();
            if ( ch < 0 ) return ( 27);   /* Wasn't a function key  */

            if ( ch == '[' )      docsi(ch);
            else if ( ch == ':' ) dobbnmouse();
            else if ( ch == 'O' ) docsi(ch);
            else {  /* This isn't an escape sequence, return it unmodified  */
                inbuffer[inlen++] = ch;

                return ( 27);
            }
        } else if ( ch == 27 + 128 )  {
            docsi(ch);
        } else                        {
            return (ch);
        }
    }
}
# else
static int PASCAL NEAR ansigetc P0_()
{
    return ( ttgetc() );
}
# endif /* IS_UNIX() || VMS */

# if     FLABEL
/* FNCLABEL:
 *
 * Label a function key
 */
int PASCAL NEAR fnclabel P2_(int, f, int, n)
/* f, n:  Default flag, numeric argument [unused] */
{
    /* on machines with no function keys...don't bother */
    return (TRUE);
}
# endif
#else
VOID ansihello P0_()
{
}
#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
