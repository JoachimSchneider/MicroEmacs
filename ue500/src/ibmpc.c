/*======================================================================
 * The routines in this file provide support for the IBM-PC and other
 * compatible terminals. It goes directly to the graphics RAM to do
 * screen output. It compiles into nothing if not an IBM-PC driver
 * Supported monitor cards include CGA, MONO and EGA.
 *====================================================================*/

/*====================================================================*/
#define IBMPC_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#define termdef 1                     /* don't define "term" external */

#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"


#if     IBMPC


# define NROW    50                     /* Max Screen size.     */
# define NCOL    80                     /* Edit if you want to.         */
# define MARGIN  8                      /* size of minimim margin and   */
# define SCRSIZ  64                     /* scroll size for extended lines */
# define NPAUSE  200                    /* # times thru update to pause */
# define BEL     0x07                   /* BEL character.               */
# define ESC     0x1B                   /* ESC character.               */
# define SPACE   32                     /* space character      */

# define SCADC   0xb8000000L            /* CGA address of screen RAM    */
# define SCADM   0xb0000000L            /* MONO address of screen RAM   */
# define SCADE   0xb8000000L            /* EGA/VGA address of screen RAM*/

# define MONOCRSR 0x0B0D                /* monochrome cursor        */
# define CGACRSR 0x0607                 /* CGA cursor           */
# define EGACRSR 0x0709                 /* EGA/VGA cursor       */

# define CDCGA   0                      /* color graphics card      */
# define CDMONO  1                      /* monochrome text card     */
# define CDEGA   2                      /* EGA color adapter        */
# define CDVGA   3                      /* VGA color adapter        */
# define CDVGA12 4                      /* VGA 12 line mode     */
# define CDCGA40 5                      /* CGA 40 wide mode     */
# define CDSENSE 9                      /* detect the card type     */

# define NDRIVE  6                      /* number of screen drivers */

typedef struct sdrive {
    char drv_name[8];                   /* screen driver name   */
    long drv_scradd;                    /* address of segment of screen ram */
    int drv_rows;                       /* # of rows for screen driver */
    int drv_cols;                       /* # of columns for screen driver */
} SDRIVE;

SDRIVE scr_drive[] =
{
    "CGA", SCADC, 25, 80,               /* standard color card      */
    "MONO", SCADM, 25, 80,              /* momochrome graphics adapter  */
    "EGA", SCADE, 43, 80,               /* Enhanced graphics adapter    */
    "VGA", SCADE, 50, 80,               /* Very Enhanced graphics adapter*/
    "VGA12", SCADE, 12, 80,             /* 12 lines x 80 cols       */
    "CGA40", SCADC, 25, 40,             /* low resolution CGA       */
};


COMMON union REGS rg;                     /* cpu REGISTER for use of DOS calls */

static int           dtype       = -1;    /* current display type     */
static long          scadd       = 0;     /* address of screen ram    */
static int           num_cols    = 0;     /* current number of columns    */
static int           orig_mode   = 0;     /* screen mode on entry     */
static int           egaexist    = FALSE; /* is an EGA card available?    */
static int           vgaexist    = FALSE; /* is video graphics array available? */
static int           revflag     = FALSE; /* are we currently in rev video? */
static int           desk_rows   = 0;     /* number of rows on current desktop */
static int           desk_cols   = 0;     /* number of cols on current desktop */
static int           break_flag  = 0;     /* state of MSDOS control break processing */
static int           *scptr[NROW];        /* pointer to screen lines  */
static unsigned int  sline[NCOL];         /* screen line image        */

static int PASCAL NEAR ibmmove      DCL((int, int));
static int PASCAL NEAR ibmeeol      DCL((void));
static int PASCAL NEAR ibmputc      DCL((int));
static int PASCAL NEAR ibmeeop      DCL((void));
static int PASCAL NEAR ibmclrdesk   DCL((void));
static int PASCAL NEAR ibmrev       DCL((int));
static int PASCAL NEAR ibmcres      DCL((char *));
static int PASCAL NEAR ibmbeep      DCL((void));
static int PASCAL NEAR ibmopen      DCL((void));
static int PASCAL NEAR ibmclose     DCL((void));
static int PASCAL NEAR ibmkopen     DCL((void));
static int PASCAL NEAR ibmkclose    DCL((void));
static int PASCAL NEAR scinit       DCL((int));
static int PASCAL NEAR screen_init  DCL((int, int));
static int PASCAL NEAR getboard     DCL((void));
static int PASCAL NEAR egaopen      DCL((int));
static int PASCAL NEAR egaclose     DCL((void));
static int PASCAL NEAR cga40_open   DCL((void));
static int PASCAL NEAR cga40_close  DCL((void));
static int PASCAL NEAR change_width DCL((int));
static int PASCAL NEAR fnclabel     DCL((int, int));

int PASCAL NEAR spal();

# if     COLOR
static int PASCAL NEAR ibmfcol DCL((int));
static int PASCAL NEAR ibmbcol DCL((int));
static int cfcolor = -1;                  /* current forground color */
static int cbcolor = -1;                  /* current background color */
static int ctrans[] =                     /* ansi to ibm color translation table */
{
    0, 4, 2, 6, 1, 5, 3, 7, 8, 12, 10, 14, 9, 13, 11, 15
};
# endif

/*
 * Standard terminal interface dispatch table. Most of the fields point
 * into "termio" code.
 */
TERM term    =
{
    NROW-1,
    NROW-1,
    NCOL,
    NCOL,
    0,
    0,
    MARGIN,
    SCRSIZ,
    NPAUSE,
    ibmopen,
    ibmclose,
    ibmkopen,
    ibmkclose,
    ttgetc,
    ibmputc,
    ttflush,
    ibmmove,
    ibmeeol,
    ibmeeop,
    ibmclrdesk,
    ibmbeep,
    ibmrev,
    ibmcres
    # if     COLOR
    ,
    ibmfcol,
    ibmbcol
    # endif
};

# if     COLOR

/* IBMFCOL:
 *
/* set the current output color
 */
int PASCAL NEAR ibmfcol P1_(int, color)
/* color: Color to set  */
{
    cfcolor = ctrans[color];

    return TRUE;
}

/* IBMBCOL:
 *
 * Set the current background color
 */
int PASCAL NEAR ibmbcol P1_(int, color)
/* color: Color to set  */
{
    cbcolor = ctrans[color];

    return TRUE;
}

# endif

/* IBMMOVE:
 */
int PASCAL NEAR ibmmove P2_(int, row, int, col)
{
    rg.h.ah = 2;                /* set cursor position function code */
    rg.h.dl = col + term.t_colorg;
    rg.h.dh = row + term.t_roworg;
    rg.h.bh = 0;                /* set screen page number */
    int86(0x10, &rg, &rg);

    return TRUE;
}

/* IBMEEOL:
 *
 * erase to the end of the line
 */
int PASCAL NEAR ibmeeol P0_()
{
    unsigned int  attr  = 0;      /* attribute byte mask to place in RAM */
    unsigned int *lnptr = NULL;   /* pointer to the destination line */
    int           i     = 0;
    int           ccol  = 0;      /* current column cursor lives */
    int           crow  = 0;      /*     row  */

    /* find the current cursor position */
    rg.h.ah = 3;                /* read cursor position function code */
    rg.h.bh = 0;                /* current video page */
    int86(0x10, &rg, &rg);
    ccol = rg.h.dl - term.t_colorg;     /* record current column */
    crow = rg.h.dh - term.t_roworg;     /* and row */

    /* build the attribute byte and setup the screen pointer */
# if     COLOR
    if ( dtype != CDMONO )
        if ( revflag )
            attr = ( ( (cfcolor & 15) << 4 ) | (cbcolor & 15) ) << 8;
        else
            attr = ( ( (cbcolor & 15) << 4 ) | (cfcolor & 15) ) << 8;

    else if ( revflag )
        attr = 0x7000;
    else
        attr = 0x0700;
# else
    attr = 0x0700;
# endif
    lnptr = &sline[0];
    for ( i=0; i < term.t_ncol; i++ )
        *lnptr++ = SPACE | attr;

    if ( flickcode && (dtype == CDCGA || dtype == CDCGA40) ) {
        /* wait for vertical retrace to be off */
        while ( (inp(0x3da) & 8) )
            ;

        /* and to be back on */
        while ( (inp(0x3da) & 8) == 0 )
            ;
    }

    /* and send the string out */
    movmem(&sline[0], scptr[crow+term.t_roworg]+ccol+term.t_colorg,
           (term.t_ncol-ccol)*2);

    return TRUE;
}

/* IBMPUTC:
 *
 * put a character at the current position in the current colors
 */
int PASCAL NEAR ibmputc P1_(int, ch)
{
    /* if its a newline, we have to move the cursor */
    if ( ch == '\n' || ch == '\r' ) {
        rg.h.ah = 3;
        int86(0x10, &rg, &rg);
        if ( rg.h.dh == 24 ) {
            ibmmove(20, 0);
            /* we must scroll the screen */
            rg.h.ah = 6;                /* scroll up */
            rg.h.al = 1;                /* # of lines to scroll by */
# if     COLOR
            rg.h.bh = cfcolor;             /* attribute for blank line */
# else
            rg.h.bh = 0;             /* attribute for blank line */
# endif
            rg.x.cx = 0;                /* upper left corner of scroll */
            rg.x.dx = 0x184f;            /* lower right */
            int86(0x10, &rg, &rg);
            rg.h.dh = 23;
        }
        ibmmove(rg.h.dh + 1, 0);

        return (TRUE);
    }

# if     1
    if ( ch == '\b' ) {

        /* backup the cursor by 1 position */
        rg.h.ah = 3;                    /* read current position */
        int86(0x10, &rg, &rg);
        rg.h.dl--;                      /* move it forward one */
        rg.h.ah = 2;                    /* set its new position */
        int86(0x10, &rg, &rg);

        rg.h.ah = 9;                    /* write char with attributes to cursor
                                         * position */
        rg.h.bh = 0;                    /* display page zero */
        rg.x.cx = 1;                    /* only one please! */
        rg.h.al = ' ';                  /* character to write */
#  if     COLOR
        rg.h.bl = ( (ctrans[gbcolor] << 4) | ctrans[gfcolor] );      /*
                                                                      * attribute
                                                                      */
#  else
        rg.h.bl = 07;
#  endif
        int86(0x10, &rg, &rg);

        return (TRUE);
    }

    if ( ch == 7 ) {
        TTbeep();

        return (TRUE);
    }

    rg.h.ah = 9;                /* write char with attributes to cursor position
                                 */
    rg.h.bh = 0;                /* display page zero */
    rg.x.cx = 1;                /* only one please! */
    rg.h.al = ch;               /* character to write */
#  if     COLOR
    rg.h.bl = ( (ctrans[gbcolor] << 4) | ctrans[gfcolor] );     /* attribute */
#  else
    rg.h.bl = 07;
#  endif
    int86(0x10, &rg, &rg);

    /* advance the cursor by 1 position */
    rg.h.ah = 3;                /* read current position */
    int86(0x10, &rg, &rg);
    rg.h.dl++;                  /* move it forward one */
    rg.h.ah = 2;                /* set its new position */
    int86(0x10, &rg, &rg);
# else
    rg.h.ah = 14;               /* write char to screen with current attrs */
    rg.h.al = ch;
#  if     COLOR
    if ( dtype != CDMONO )
        rg.h.bl = cfcolor;
    else
        rg.h.bl = 0x07;
#  else
    rg.h.bl = 0x07;
#  endif
    int86(0x10, &rg, &rg);
# endif

    return TRUE;
}

/* IBMEEOP:
 */
int PASCAL NEAR ibmeeop P0_()
{
    rg.h.ah = 6;                /* scroll page up function code */
    rg.h.al = 0;                /* # lines to scroll (clear it) */
    rg.x.cx = (term.t_roworg << 8) | (term.t_colorg);
    /* upper left corner of scroll */
    rg.x.dx =
        ( (term.t_nrow + term.t_roworg) <<
    8 ) |(term.t_ncol + term.t_colorg - 1);
    /* lower right corner of scroll */
# if     COLOR
    if ( dtype != CDMONO )
        if ( revflag )
            rg.h.bh = ( (ctrans[gfcolor] & 15) << 4 ) | (ctrans[gbcolor] & 15);
        else
            rg.h.bh = ( (ctrans[gbcolor] & 15) << 4 ) | (ctrans[gfcolor] & 15);

    else if ( revflag )
        rg.h.bh = 70;
    else
        rg.h.bh = 07;
# else
    rg.h.bh = 07;
# endif
    int86(0x10, &rg, &rg);

    return TRUE;
}

/* IBMCLRDESK:
 */
int PASCAL NEAR ibmclrdesk P0_()
{
    int attr  = 0;              /* attribute to fill screen with */

    rg.h.ah = 6;                /* scroll page up function code */
    rg.h.al = 0;                /* # lines to scroll (clear it) */
    rg.x.cx = 0;                /* upper left corner of scroll */
    rg.x.dx = (desk_rows << 8) | desk_cols;
    /* lower right corner of scroll */
# if     COLOR
    if ( dtype != CDMONO )
        if ( revflag )
            attr = ( (ctrans[gfcolor] & 15) << 4 ) | (ctrans[deskcolor] & 15);
        else
            attr = ( (ctrans[deskcolor] & 15) << 4 ) | (ctrans[gfcolor] & 15);

    else if ( revflag )
        attr = 70;
    else
        attr = 07;
# else
    attr = 07;
# endif

    rg.h.bh = attr;
    int86(0x10, &rg, &rg);

    return TRUE;
}

/* IBMREV:
 *
 * change reverse video state
 */
int PASCAL NEAR ibmrev P1_(int, state)
/* state: TRUE = reverse, FALSE = normal  */
{
    revflag = state;

    return TRUE;
}

/* In `screen.c' for debug purposes.  */
EXTERN VOID dumpscreens DCL((char *msg));

/* IBMCRES:
 *
 * Change screen resolution
 */
int PASCAL NEAR ibmcres P1_(char *, res)
/* res: Resolution to change to */
{
    int i = 0;

    for ( i = 0; i < NDRIVE; i++ )
        if ( strcmp(res, scr_drive[i].drv_name) == 0 ) {
            scinit(i);

            return (TRUE);
        }

    return (FALSE);
}

/* SPAL:
 *
 * Reset the pallette registers
 *
 * RC:
 *  - 0: Success
 *  - 1: Error
 */
int PASCAL NEAR spal P1_(char *, mode)
{
    /* nothin here now..... */

    return 0;
}

/* IBMBEEP:
 */
int PASCAL NEAR ibmbeep P0_()
{
# if     MWC
    ttputc(BEL);
# else /* MWC */
#  if     IC
    bdos(6, BEL);
#  else /* IC */
    bdos(6, BEL, 0);
#  endif /* IC */
# endif /* MWC */

    return TRUE;
}

/* IBMOPEN:
 */
int PASCAL NEAR ibmopen P0_()
{
    scinit(CDSENSE);
    revexist = TRUE;
    revflag = FALSE;
    ttopen();

    return TRUE;
}

/* IBMCLOSE:
 */
int PASCAL NEAR ibmclose P0_()
{
# if     COLOR
    ibmfcol(7);
    ibmbcol(0);
# endif

    /* exit in the same mode that we entered */
    if ( dtype != orig_mode ) {
        scinit(orig_mode);
        movecursor(term.t_nrow, 0);
        TTflush();
    }
    dtype = -1;
    ttclose();

    return TRUE;
}

/* IBMKOPEN:
 *
 * Open the keyboard
 */
int PASCAL NEAR ibmkopen P0_()
{
    /* find the current state of the control break inturrupt */
    rg.h.ah = 0x33;     /* ctrl-break check */
    rg.h.al = 0;        /* request the state of the ctrl-break check */
    intdos(&rg, &rg);
    break_flag = rg.h.dl;

    /* set the break processing off if it is on */
    if ( break_flag == 1 ) {
        rg.h.ah = 0x33;         /* ctrl-break check */
        rg.h.al = 1;            /* set the state of the ctrl-break check */
        rg.h.dl = 0;            /* turn it off */
        intdos(&rg, &rg);
    }

    return TRUE;
}

/* IBMKCLOSE:
 *
 * Close the keyboard
 */
int PASCAL NEAR ibmkclose P0_()
{
    if ( break_flag == 1 ) {
        rg.h.ah = 0x33;         /* ctrl-break check */
        rg.h.al = 1;            /* set the state of the ctrl-break check */
        rg.h.dl = 1;            /* turn it on */
        intdos(&rg, &rg);
    }

    return TRUE;
}

/* SCINIT:
 *
 * Initialize the screen head pointers
 */
int PASCAL NEAR scinit P1_(int, type)
/* type:  Type of adapter to init for */
{
    /* if asked...find out what display is connected */
    if ( type == CDSENSE )
        type = getboard();

    /* if we have nothing to do....don't do it */
    if ( dtype == type )
        return (TRUE);

    /* if we try to switch to EGA and there is none, don't */
    if ( type == CDEGA && !egaexist )
        return (FALSE);

    /* if we try to switch to VGA and there is none, don't */
    if ( type == CDVGA && !vgaexist )
        return (FALSE);

    /* if we had the EGA or VGA open... close it */
    if ( dtype == CDEGA || dtype == CDVGA || dtype == CDVGA12 )
        egaclose();

    /* if we had the CGA40 open... close it */
    if ( dtype == CDCGA40 )
        cga40_close();

    /* and set up the various parameters as needed */
    scadd = scr_drive[type].drv_scradd;
    switch ( type ) {
    case CDMONO:                /* Monochrome adapter */
    case CDCGA:                 /* Color graphics adapter */
        break;

    case CDCGA40:               /* Color graphics adapter */
        cga40_open();
        break;

    case CDEGA:                 /* Enhanced graphics adapter */
        egaopen(CDEGA);
        break;

    case CDVGA:                 /* video graphics array - acts as EGA but more
                                 * lines */
        egaopen(CDVGA);
        break;

    case CDVGA12:               /* Video Graphics Array 12 line mode */
        egaopen(CDVGA12);
        break;

    }
    maxlines(scr_drive[type].drv_rows);

    /* reset the $sres environment variable */
    XSTRCPY(sres, scr_drive[type].drv_name);

    /* resize any screens that need it! */
    screen_init(dtype, type);
    dtype = type;

    return (TRUE);
}

/* SCREEN_INIT:
 *
 * Initialize the screen head pointers
 */
int PASCAL NEAR screen_init P2_(int, dtype, int, type)
/* dtype: Original screen type (-1 if first time!)  */
/* type:  New type of adapter to adjust screens for */
{
    int full_screen = 0;    /* is the active screen full size */

    /* is the front screen full size? */
    if ( (dtype != -1) &&( scr_drive[dtype].drv_rows == (term.t_nrow + 1) ) &&
         (scr_drive[dtype].drv_cols == term.t_ncol) )
        full_screen = TRUE;
    else
        full_screen = FALSE;

    /* set up the new desktop size */
    desk_rows = scr_drive[type].drv_rows;
    desk_cols = scr_drive[type].drv_cols;

    /* recalculate the screen line pointer array */
    change_width(desk_cols);

    /* first time, or if we are full screen */
    if ( (dtype == -1) || full_screen ) {
        newsize(TRUE, desk_rows);
        newwidth(TRUE, desk_cols);
    }

# if     WINDOW_TEXT
    refresh_screen(first_screen);
# endif

    return (TRUE);
}

/* CHANGE_WIDTH:
 */
int PASCAL NEAR change_width P1_(int, ncols)
/* ncols: Number of columns across  */
{
    union {
        long laddr;             /* long form of address */
        int *paddr;             /* pointer form of address */
    } addr;
    int i = 0;

    ZEROMEM(addr);
    /* re-initialize the screen pointer array */
    for ( i = 0; i < NROW; i++ ) {
        addr.laddr = scadd + (long)(ncols * i * 2);
        scptr[i] = addr.paddr;
    }

    return TRUE;
}

/* GETBOARD:
 *
 * Determine which type of display board is attached.
 * Current known types include:
 *
 * CDMONO       Monochrome graphics adapter
 * CDCGA        Color Graphics Adapter
 * CDEGA        Extended graphics Adapter
 * CDVGA        Vidio Graphics Array
 *
 * if MONO      set to MONO
 *   CGA40 set to CGA40 test as appropriate
 *   CGA        set to CGA      EGAexist = FALSE VGAexist = FALSE
 *   EGA        set to CGA      EGAexist = TRUE  VGAexist = FALSE
 *   VGA        set to CGA      EGAexist = TRUE  VGAexist = TRUE
 */
int PASCAL NEAR getboard P0_()
{
    int type  = 0;      /* board type to return */

    type = CDCGA;
    int86(0x11, &rg, &rg);
    if ( ( ( (rg.x.ax >> 4) & 3 ) == 3 ) )
        type = CDMONO;

    /* test for 40 col mode */
    rg.h.ah = 15;
    int86(0x10, &rg, &rg);
    if ( rg.h.al == 1 )
        type = CDCGA40;

    rg.x.ax = 0x1130;
    int86(0x10, &rg, &rg);

    if ( rg.h.dl == 42 )
        type = CDEGA;

    if ( rg.h.dl == 49 )
        type = CDVGA;

    /* save the original video mode */
    orig_mode = type;

    /* test if EGA present */
    rg.x.ax = 0x1200;
    rg.x.bx = 0xff10;
    int86(0x10, &rg, &rg);              /* If EGA, bh=0-1 and bl=0-3 */
    egaexist = !(rg.x.bx & 0xfefc);     /* Yes, it's EGA */
    if ( egaexist ) {
        /* Adapter says it's an EGA. We'll get the same response from a VGA, so
         * try to tell the two apart */
        rg.x.ax = 0x1a00;               /* read display combination */
        int86(0x10, &rg, &rg);
        if ( rg.h.al == 0x1a && (rg.h.bl == 7 || rg.h.bl == 8) ) {
            /* Function is supported and it's a PS/2 50,60,80 with analog
             * display, so it's VGA (I hope!) */
            vgaexist = TRUE;
        } else {
            /* Either BIOS function not supported or something other then VGA so
             * set it to be EGA */
            vgaexist = FALSE;
        }
    }

    return (type);
}

/* EGAOPEN:
 *
 * Init the computer to work with the EGA or VGA
 */
int PASCAL NEAR egaopen P1_(int, mode)
/* mode:  Mode to select [CDEGA/CDVGA]  */
{
    /* set the proper number of scan lines */
    rg.h.ah = 18;
    switch ( mode ) {

    case CDEGA:
        rg.h.al = 1;
        break;

    case CDVGA:
        rg.h.al = 2;
        break;

    case CDVGA12:
        rg.h.al = 0;
        break;

    }
    rg.h.bl = 48;
    int86(16, &rg, &rg);

    /* put the beast into EGA 43/VGA 50/VGA 12 line mode */
    rg.x.ax = 3;
    int86(16, &rg, &rg);

    /* set the proper character set */
    if ( mode == CDVGA12 ) {
        rg.h.al = 20;           /*  to 8 by 16 double dot ROM         */
    } else {
        rg.h.al = 18;           /*  to 8 by 8 double dot ROM         */
    }
    rg.h.ah = 17;               /* set char. generator function code */
    rg.h.bl = 0;                /* block 0                           */
    int86(16, &rg, &rg);

    /* select the alternative Print Screen function */
    rg.h.ah = 18;               /* alternate select function code    */
    rg.h.al = 0;                /* clear AL for no good reason       */
    rg.h.bl = 32;               /* alt. print screen routine         */
    int86(16, &rg, &rg);

    /* resize the cursor */
    rg.h.ah = 1;                /* set cursor size function code */
    rg.x.cx = 0x0607;           /* turn cursor on code */
    int86(0x10, &rg, &rg);

    /* video bios bug patch */
    outp(0x3d4, 10);
    outp(0x3d5, 6);

    return TRUE;
}

/* EGACLOSE:
 */
int PASCAL NEAR egaclose P0_()
{
    /* set the proper number of scan lines for CGA */
    rg.h.ah = 18;
    rg.h.al = 2;
    rg.h.bl = 48;
    int86(16, &rg, &rg);

    /* put the beast into 80 column mode */
    rg.x.ax = 3;
    int86(16, &rg, &rg);

    return TRUE;
}

/* CGA40_OPEN:
 */
int PASCAL NEAR cga40_open P0_()
{
    /* put the beast into 40 column mode */
    rg.x.ax = 1;
    int86(16, &rg, &rg);

    return TRUE;
}

/* CGA40_CLOSE:
 */
int PASCAL NEAR cga40_close P0_()
{
    /* put the beast into 80 column mode */
    rg.x.ax = 3;
    int86(16, &rg, &rg);

    return TRUE;
}

/* SCWRITE:
 *
 * Write a line out to the physical screen
 */
int PASCAL NEAR scwrite P6_(int,    row,
                            char *, outstr,
                            int,    forg,
                            int,    bacg,
                            int,    revleft,
                            int,    revright)
/* row:       Row of screen to place outstr on                */
/* outstr:    String to write out (must be term.t_ncol long)  */
/* forg:      Forground color of string to write              */
/* bacg:      Background color                                */
/* revleft:   First character of reverse video area           */
/* revright:  First character of non-reverse video area       */
{
    unsigned int  norm_attrib = 0;    /* normal attribute byte mask */
    unsigned int  rev_attrib  = 0;    /* reversed attribute byte mask */
    unsigned int  *lnptr      = NULL; /* pointer to the destination line */
    int           i           = 0;

    /* build the attribute bytes */
# if     COLOR
    if ( dtype != CDMONO ) {
        norm_attrib = ( (ctrans[bacg]&7) << 12 ) | (ctrans[forg] << 8);
        rev_attrib = ( (ctrans[forg]&7) << 12 ) | (ctrans[bacg] << 8);
    } else {
        norm_attrib = 0x0700;
        rev_attrib = 0x7000;
    }
# else
    norm_attrib = ( ( (bacg & 15) << 4 ) | (forg & 15) ) << 8;
    rev_attrib = ( ( (forg & 15) << 4 ) | (bacg & 15) ) << 8;
# endif

    /* init the pointer to screen line image */
    lnptr = &sline[0];

    /* step throught the line, setting the character and attributes */
    for ( i=0; i<term.t_ncol; i++ ) {
        if ( i < revleft || i >= revright )
            *lnptr++ = (outstr[i] & 255) | norm_attrib;
        else
            *lnptr++ = (outstr[i] & 255) | rev_attrib;
    }

    if ( flickcode && (dtype == CDCGA) ) {
        /* wait for vertical retrace to be off */
        while ( (inp(0x3da) & 8) )
            ;

        /* and to be back on */
        while ( (inp(0x3da) & 8) == 0 )
            ;
    }

    /* and send the string out */
    movmem(&sline[0], scptr[row+term.t_roworg]+term.t_colorg, term.t_ncol*2);

    return TRUE;
}

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

VOID  ibmhello P0_()
{
}

#endif  /* IBMPC  */



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
