/*======================================================================
 * The functions in this file handle redisplay. There are two halves, the ones
 * that update the virtual display screen, and the ones that make the physical
 * display screen the same as the virtual display screen. These functions use
 * hints that are left in the windows by the commands.
 *====================================================================*/

/*====================================================================*/
#define DISPLAY_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include        <stdio.h>
#include        <stdlib.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

#define FARRIGHT        999             /* column beyond the right edge! */

#if     WINDOW_MSWIN
static int foulcursor = FALSE;          /* see vtscreen() & movecursor() */
#endif

static VIDEO   **vscreen;                      /* Virtual screen. */
#if     MEMMAP == 0
static VIDEO   **pscreen;                      /* Physical screen. */
#endif

/*  some local function declarations    */

#if     MEMMAP
EXTERN VOID PASCAL NEAR upt_line DCL((int row, struct VIDEO *vp1));
#else
EXTERN VOID PASCAL NEAR upt_line DCL((int          row,
                                         struct VIDEO *vp1,
                                         struct VIDEO *vp2));
#endif
EXTERN VOID PASCAL NEAR upt_hilite DCL((void));


/* VTINIT:
 *
 * Initialize the data structures used by the display code. The edge vectors
 * used to access the screens are set up. The operating system's terminal I/O
 * channel is set up. All the other things get initialized at compile time. The
 * original window has "WFCHG" set, so that it will get completely redrawn on
 * the first call to "update".
 */
int PASCAL NEAR vtinit P0_()
{
    REGISTER int i;
    REGISTER VIDEO *vp;

    TTopen();                   /* open the screen */
    TTkopen();                  /* open the keyboard */
    TTrev(FALSE);

    /* allocate the virtual screen pointer array */
    vscreen = (VIDEO **)ROOM( term.t_mrow*SIZEOF (VIDEO *) );

    if ( vscreen == NULL )
#if     WINDOW_MSWIN
        return (FALSE);

#else
        meexit(1);
#endif


#if     MEMMAP == 0
    /* allocate the physical shadow screen array */
    pscreen = (VIDEO **)ROOM( term.t_mrow*SIZEOF (VIDEO *) );
    if ( pscreen == NULL )
# if     WINDOW_MSWIN
        return (FALSE);

# else
        meexit(1);
# endif
#endif

    /* for every line in the display */
    for ( i = 0; i < term.t_mrow; ++i ) {

        /* allocate a virtual screen line */
        vp = (VIDEO *)ROOM(SIZEOF (VIDEO)+term.t_mcol);
        if ( vp == NULL )
#if     WINDOW_MSWIN
            return (FALSE);

#else
            meexit(1);
#endif

        vp->v_flag = 0;                 /* init change flags */
        vp->v_left = FARRIGHT;          /* init requested rev video boundries */
        vp->v_right = 0;
#if     COLOR
        vp->v_rfcolor = 7;              /* init fore/background colors */
        vp->v_rbcolor = 0;
#endif
        /* connect virtual line to line array */
        vscreen[i] = vp;

#if     MEMMAP == 0
        /* allocate and initialize physical shadow screen line */
        vp = (VIDEO *)ROOM(SIZEOF (VIDEO)+term.t_mcol);
        if ( vp == NULL )
# if     WINDOW_MSWIN
            return (FALSE);

# else
            meexit(1);
# endif

        vp->v_flag = VFNEW;
        vp->v_left = FARRIGHT;
        vp->v_right = 0;
# if     INSDEL
        vp->v_rline = i;                /* set requested line position */
# endif
        pscreen[i] = vp;
#endif
    }

    return (TRUE);
}

#if     CLEAN || WINDOW_MSWIN
/* VTFREE:
 *
 * Free up all the dynamically allocated video structures
 */
VOID PASCAL NEAR vtfree P0_()
{
    int i = 0;

    for ( i = 0; i < term.t_mrow; ++i ) {
        if ( vscreen && vscreen[i] ) CLROOM(vscreen[i]);
# if     MEMMAP == 0
        if ( pscreen && pscreen[i] ) CLROOM(pscreen[i]);
# endif
    }
    CLROOM(vscreen);
# if     MEMMAP == 0
    CLROOM(pscreen);
# endif
}
#endif

#if     WINDOW_MSWIN
/* VTSCREEN:
 *
 * Map a screen into the Virtual Terminal system
 */
VOID PASCAL NEAR vtscreen P1_(SCREEN_T *, sp)
{
    TTflush();
    term.t_roworg = sp->s_roworg;
    term.t_colorg = sp->s_colorg;
    term.t_nrow = sp->s_nrow;
    term.t_ncol = sp->s_ncol;

    vscreen = sp->s_virtual;
    pscreen = sp->s_physical;
    term.t_selscr(sp);
    foulcursor = TRUE;
} /* vtscreen */

/* VTINITSCR:
 *
 * Build a virtual terminal resource for a new screen
 */
int PASCAL NEAR vtinitscr P3_(SCREEN_T *, sp, int, nrow, int, ncol)
/* returns TRUE if successful */
{
    int result;

    if ( nrow < 2 ) nrow = 2;
    term.t_nrow = nrow;
    term.t_ncol = ncol;
    term.t_roworg = 0;
    term.t_colorg = 0;
    if ( ( result = vtinit() ) == TRUE ) {
        sp->s_virtual = vscreen;
        sp->s_physical = pscreen;
        sp->s_nrow = nrow;
        sp->s_ncol = ncol;
        sp->s_roworg = 0;
        sp->s_colorg = 0;
    } else {
        vtfree();
        sp->s_virtual = NULL;
        sp->s_physical = NULL;
    }

    return result;
} /* vtinitscr */

/* VTFREESCR:
 *
 * Delete a virtual terminal resource for a dying screen
 */
int PASCAL NEAR vtfreescr P1_(SCREEN_T, *sp)
{
    vtscreen(sp);
    vtfree();
    vtscreen(first_screen);     /* switch to an existing screen */
} /* vtfreescr */

/* VTSIZESCR:
 *
 * Resize the virtual terminal resources
 *
 * returns TRUE if successful. Otherwise, the old VIDEO structures are
 * preserved.
 */
int PASCAL NEAR vtsizescr P3_(SCREEN_T *, sp, int, nrow, int, ncol)
{
    VIDEO   **oldvvp, **oldpvp;
    int oldnrow, oldncol;

    oldvvp = sp->s_virtual;
    oldpvp = sp->s_physical;
    oldnrow = sp->s_nrow;
    oldncol = sp->s_ncol;
    if ( vtinitscr(sp, nrow, ncol) == TRUE ) {
        /* success! let's free the old VIDEO structures */
        EWINDOW *wp;

        vscreen = oldvvp;
        pscreen = oldpvp;
        term.t_nrow = oldnrow;
        term.t_ncol = oldncol;
        vtfree();                       /* get rid of the old VIDEOs (kept up to
                                         * now in case the new allocation had
                                         * failed) */
        vtscreen(sp);           /* put the new VIDEOs into active duty */
        for ( wp = sp->s_first_window; wp != NULL; wp = wp->w_wndp ) {
            /* the VIDEOs have been wiped clean. Let's have every window marked
             * for a complete redraw */
            wp->w_flag |= WFHARD|WFMODE;
        }
        term.t_sizscr(sp);         /* resize the MDI window */

        return (TRUE);
    } else {
        /* failure! we still need some kind of VIDEO structures, so we reuse the
         * old ones */
        term.t_nrow = oldnrow;
        term.t_ncol = oldncol;
        sp->s_virtual = vscreen = oldvvp;
        sp->s_physical = pscreen = oldpvp;
        mlabort(TEXT94);                /* "out of memory" */

        return (FALSE);
    }
} /* vtsizescr */
#endif

/* VTTIDY:
 *
 * Clean up the virtual terminal system, in anticipation for a return to the
 * operating system. Move down to the last line and clear it out (the next
 * system prompt will be written in the line). Shut down the channel to the
 * terminal.
 */
VOID PASCAL NEAR vttidy P0_()
{
    mlerase();
    movecursor(term.t_nrow, 0);
    TTflush();
    TTclose();
    TTkclose();
}

/* VTMOVE:
 *
 * Set the virtual cursor to the specified row and column on the virtual screen.
 * There is no checking for nonsense values; this might be a good idea during
 * the early stages.
 */
VOID PASCAL NEAR vtmove P2_(int, row, int, col)
{
    vtrow = row;
    vtcol = col;
}

/* VTPUTC:
 *
 * Write a character to the virtual screen. The virtual row and column are
 * updated. If we are not yet on left edge, don't print it yet. If the line is
 * too long put a "$" in the last column. This routine only puts printing
 * characters into the virtual terminal buffers. Only column overflow is
 * checked.
 */
VOID PASCAL NEAR vtputc P1_(int, c)
{
    REGISTER VIDEO *vp;         /* ptr to line being updated */

    /* defeate automatic sign extenstion */
    c = c & 0xff;

    /* this is the line to put this character! */
    vp = vscreen[vtrow];

    if ( c == '\t' && tabsize > 0 ) {

        /* output a hardware tab as the right number of spaces */
        do {
            vtputc(' ');
        } while ( ( (vtcol + taboff) % (tabsize) ) != 0 );

    } else if ( vtcol >= term.t_ncol ) {

        /* we are at the right edge! */
        ++vtcol;
        vp->v_text[term.t_ncol - 1] = '$';

    } else if ( disphigh && c > 0x7f ) {

        /* char with high bit set is displayed symbolically on 7 bit screens */
        vtputc('^');
        vtputc('!');
        c -= 0x80;
        if ( c == '\t' ) {
            vtputc('^');
            vtputc('I');
        } else
            vtputc(c);

    } else if ( c < 0x20 || c == 0x7F ) {

        /* control character? */
        vtputc('^');
        vtputc(c ^ 0x40);

    } else {

        /* it's normal, just put it in the screen map */
        if ( vtcol >= 0 )
            vp->v_text[vtcol] = c;
        ++vtcol;
    }
}

/* VTEEOL:
 *
 * Erase from the end of the software cursor to the end of the line on which the
 * software cursor is located.
 */
VOID PASCAL NEAR vteeol P0_()
{
    REGISTER VIDEO      *vp;

    vp = vscreen[vtrow];
    while ( vtcol < term.t_ncol ) {
        if ( vtcol >= 0 )
            vp->v_text[vtcol] = ' ';
        vtcol++;
    }
}

/* UPSCREEN:
 *
 * User routine to force a screen update always finishes complete
 * update
 */
int PASCAL NEAR upscreen P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    update(TRUE);

    return (TRUE);
}

/* UPDATE:
 *
 * Make sure that the display is right. This is a three part process. First,
 * scan through all of the windows looking for dirty ones. Check the framing,
 * and refresh the screen. Second, make sure that "currow" and "curcol" are
 * correct for the current window. Third, make the virtual and physical screens
 * the same.
 */
VOID PASCAL NEAR update P1_(int, force)
/* force: Force update past type ahead? */
{
    REGISTER EWINDOW *wp;
#if     WINDOW_MSWIN
    SCREEN_T  *sp;
#endif

#if     TYPEAH
    /* if we are not forcing the update, and there are keystrokes waiting to be
     * processed, skip the update */
    if ( force == FALSE && typahead() )
        return;

#endif

#if     VISMAC == 0
    /* if we are replaying a keyboard macro, don't bother keeping updated */
    if ( force == FALSE && kbdmode == PLAY )
        return;

#endif

#if     WINDOW_MSWIN
    /* if we are not forcing the update, allow us to defer it */
    if ( force == FALSE ) {
        defferupdate = TRUE;

        return;
    } else
        defferupdate = FALSE;

    /* loop through all screens to update even partially hidden ones. We update
     * screens from back to front! */
    sp = first_screen;
    do {
        char scroll_flag = 0;
        int scroll_fcol;
        static EWINDOW *scroll_wp = (EWINDOW *)NULL;

        sp = sp->s_next_screen;
        if ( sp == (SCREEN_T *)NULL ) {
            sp = first_screen;
            sp->s_cur_window = curwp;             /* not always up to date */
        }
        vtscreen(sp);
        wheadp = sp->s_first_window;
        scroll_fcol = sp->s_cur_window->w_fcol;
#endif

    /* update any windows that need refreshing */
    wp = wheadp;
    while ( wp != NULL ) {
        if ( wp->w_flag ) {
            /* if the window has changed, service it */
            reframe(wp);                        /* check the framing */
            if ( (wp->w_flag & ~WFMODE) == WFEDIT )
                updone(wp);                             /* update EDITed line */
            else if ( wp->w_flag & ~WFMOVE )
                updall(wp);                             /* update all lines */
            if ( wp->w_flag & WFMODE )
                modeline(wp);                           /* update modeline */
#if WINDOW_MSWIN
            if ( wp == sp->s_cur_window ) {
                if ( wp != scroll_wp ) {
                    /* switched to another window, force scroll bar updates */
                    scroll_flag = WFHARD;
                    scroll_wp = wp;
                } else
                    scroll_flag = wp->w_flag & WFHARD;

            }
#endif
            wp->w_flag = 0;
            wp->w_force = 0;
        }

        /* on to the next window */
        wp = wp->w_wndp;
    }

    /* recalc the current hardware cursor location */
#if     WINDOW_MSWIN
    if ( sp == first_screen ) {
#endif
    updpos();

    /* update the current window if we have to move it around */
    if ( curwp->w_flag & WFHARD )
        updall(curwp);
    if ( curwp->w_flag & WFMODE )
        modeline(curwp);
    curwp->w_flag = 0;

    /* highlight region in the current window if needed */
    upt_hilite();

#if     WINDOW_MSWIN
}
#endif

#if     MEMMAP
    /* update the cursor and flush the buffers */
    movecursor(currow, curcol - lbound);
#endif

    /* check for lines to de-extend */
    upddex();

    /* if screen is garbage, re-plot it */
    if ( sgarbf != FALSE )  {
        if ( gflags & GFSDRAW )
            sgarbf = FALSE;
        else
            updgar();
    }

    /* update the virtual screen to the physical screen */
    updupd(force);
#if     WINDOW_MSWIN
    if ( scroll_fcol != sp->s_cur_window->w_fcol ) {
        scroll_flag |= WFMOVE;
    }
    if ( scroll_flag )
        updscrollbars(sp, scroll_flag);

}
while ( sp != first_screen );

sgarbf = FALSE;
#endif

    /* update the cursor and flush the buffers */
    movecursor(currow, curcol - lbound);
    TTflush();

    return;
}

/* REFRAME:
 *
 * Check to see if the cursor is on in the window and re-frame it
 * if needed or wanted
 */
VOID PASCAL NEAR reframe P1_(EWINDOW *, wp)
{
    REGISTER LINE *lp;          /* search pointer */
    REGISTER LINE *rp;          /* reverse search pointer */
    REGISTER LINE *hp;          /* ptr to header line in buffer */
    REGISTER LINE *tp;          /* temp debugging pointer */
    REGISTER int i;             /* general index/# lines to scroll */
    REGISTER int nlines;        /* number of lines in current window */

    /* figure out our window size */
    nlines = wp->w_ntrows;
    if ( modeflag == FALSE )
        nlines++;

    /* if not a requested reframe, check for a needed one */
    if ( (wp->w_flag & WFFORCE) == 0 ) {
        lp = wp->w_linep;
        for ( i = 0; i < nlines; i++ ) {

            /* if the line is in the window, no reframe */
            if ( lp == wp->w_dotp )
                return;

            /* if we are at the end of the file, reframe */
            if ( lp == wp->w_bufp->b_linep )
                break;

            /* on to the next line */
            lp = lforw(lp);
        }
    }

    /* reaching here, we need a window refresh */
    i = wp->w_force;

    /* if smooth scrolling is enabled, first.. have we gone off the top? */
    if ( sscroll && ( (wp->w_flag & WFFORCE) == 0 ) ) {
        /* search thru the buffer looking for the point */
        tp = lp = rp = wp->w_linep;
        hp = wp->w_bufp->b_linep;

        while ( (lp != hp) || (rp != hp) ) {

            /* did we scroll downward? */
            if ( lp == wp->w_dotp ) {
                i = nlines - 1;
                break;
            }

            /* did we scroll upward? */
            if ( rp == wp->w_dotp ) {
                i = 0;
                break;
            }

            /* advance forward and back */
            if ( lp != hp )
                lp = lforw(lp);
            if ( rp != hp )
                rp = lback(rp);

            /* problems????? */
            if ( lp == tp || rp == tp ) {
                mlforce("BUG IN SMOOTH SCROLL--GET DAN!\n");
                TTgetc();
            }
        }
        /* how far back to reframe? */
    } else if ( i > 0 ) {       /* only one screen worth of lines max */
        if ( --i >= nlines )
            i = nlines - 1;
    } else if ( i < 0 ) {       /* negative update???? */
        i += nlines;
        if ( i < 0 )
            i = 0;
    } else
        i = nlines / 2;

    /* backup to new line at top of window */
    lp = wp->w_dotp;
    while ( i != 0 && lback(lp) != wp->w_bufp->b_linep ) {
        --i;
        if ( i < 0 ) {
            mlforce("OTHER BUG IN DISPLAY --- GET DAN!!!\n");
            TTgetc();
        }
        lp = lback(lp);
    }

    /* and reset the current line at top of window */
    wp->w_linep = lp;
    wp->w_flag |= WFHARD;
    wp->w_flag &= ~WFFORCE;
}

/* HILITE:
 *
 * In the current window, marks 10 and 11 are set and the area between
 * them is on screen, hilite that area
 */
VOID PASCAL NEAR upt_hilite P0_()
{
    int   first_line  = 0;      /* first screen line to highlight */
    short first_pos   = 0;      /* position in that line */
    int   last_line   = 0;      /* last screen line to highlight */
    short last_pos    = 0;      /* position in that line */
    LINE  *forptr     = NULL;   /* line pointers searching in current buffer */
    LINE  *bckptr     = NULL;   /* ......................................... */
    int   forline     = 0;      /* screen lines of for/bck ptrs */
    int   bckline     = 0;      /* ............................ */
    int   nlines      = 0;      /* number of text lines in current window */
    LINE  *first_mark = NULL;   /* first mark to highlighted text */
    LINE  *last_mark  = NULL;   /* last mark to highlighted text */
    LINE  *b_linep    = NULL;   /* header line of current buffer */
    int   temp_line   = 0;      /* temp line # for swap */
    short temp_pos    = 0;      /*  more of the same */

    /* $hilight must be set to the first of 2 consecutive marks used to define
     * the region to highlight */
    if ( !hilite_IsValid() )
        return;

    /* Both marks must be set to define a highlighted region */
    first_mark = curwp->w_markp[hilite];
    last_mark = curwp->w_markp[hilite+1];
    if ( (first_mark == (LINE *)NULL) ||(last_mark == (LINE *)NULL) )
        return;

    /* search for the two marks starting at the top line of this window */
    first_pos = last_pos = -1;
    forptr = curwp->w_linep;
    bckptr = curwp->w_linep;
    forline = bckline = 0;
    b_linep = curwp->w_bufp->b_linep;
    while ( ( (first_pos == -1) || (last_pos == -1) ) &&
            ( (forptr != (LINE *)NULL) || (bckptr != (LINE *)NULL) ) ) {
        /* have we found either mark? */
        if ( forptr == first_mark ) {
            first_line = forline;
            first_pos = findcol(forptr, curwp->w_marko[hilite]);
        }
        if ( forptr == last_mark ) {
            last_line = forline;
            last_pos = findcol(forptr, curwp->w_marko[hilite+1]);
        }
        if ( bckptr == first_mark ) {
            first_line = bckline;
            first_pos = findcol(bckptr, curwp->w_marko[hilite]);
        }
        if ( bckptr == last_mark ) {
            last_line = bckline;
            last_pos = findcol(bckptr, curwp->w_marko[hilite+1]);
        }

        /* step outward one more line */
        if ( forptr != (LINE *)NULL ) {
            if ( forptr != b_linep )
                forptr = lforw(forptr);
            else
                forptr = (LINE *)NULL;
            forline++;
        }
        if ( bckptr != (LINE *)NULL ) {
            bckptr = lback(bckptr);
            if ( bckptr == b_linep )
                bckptr = (LINE *)NULL;
            bckline--;
        }
    }

    /* if both lines are before the current window */
    if ( (first_line < 0) && (last_line < 0) )
        return;

    /* if both lines are after the current window */
    nlines = curwp->w_ntrows;
    if ( modeflag == FALSE )
        nlines++;
    if ( (first_line >= nlines) && (last_line >= nlines) )
        return;

    /* if we got them backwards, swap them around */
    if ( (first_line > last_line) ||
         ( (first_line == last_line) && (first_pos > last_pos) ) ) {
        temp_line = first_line;
        first_line = last_line;
        last_line = temp_line;
        temp_pos = first_pos;
        first_pos = last_pos;
        last_pos = temp_pos;
    }

    forptr = curwp->w_linep;
    forline = curwp->w_toprow;
    first_line += forline;
    last_line += forline;
    while ( forline < curwp->w_toprow + nlines ) {
        if ( (forline >= first_line) && (forline <= last_line) ) {
            vscreen[forline]->v_left = 0;
            vscreen[forline]->v_right = findcol( forptr, get_lused(forptr) );
            if ( forline == first_line )
                vscreen[forline]->v_left = first_pos;
            if ( forline == last_line )
                vscreen[forline]->v_right = last_pos;

            /* adjust for shifted window */
            vscreen[forline]->v_left -= curwp->w_fcol;
            vscreen[forline]->v_right -= curwp->w_fcol;

            /* adjust for shifted line */
            if ( vscreen[forline]->v_flag & VFEXT ) {
                vscreen[forline]->v_left -= lbound;
                vscreen[forline]->v_right -= lbound;
            }

            /* This is Malcolm Kay's fix for the horizontal scrolling problem
             * after a search/replace command */
            if ( vscreen[forline]->v_left < 0 )
                vscreen[forline]->v_left = 0;
            if ( vscreen[forline]->v_right < 0 )
                vscreen[forline]->v_right = 0;
            if ( vscreen[forline]->v_left>term.t_ncol ) {
                vscreen[forline]->v_left = FARRIGHT;
                vscreen[forline]->v_right = 0;
            } else if ( vscreen[forline]->v_right>term.t_ncol )
                vscreen[forline]->v_right = term.t_ncol;
        } else {
            vscreen[forline]->v_left = FARRIGHT;
            vscreen[forline]->v_right = 0;
        }

        /* step up one more line */
        if ( forptr != b_linep )
            forptr = lforw(forptr);
        forline++;
    }

    /* we need to flag a redraw to update the hilighted region */
    curwp->w_flag |= WFHARD;

    return;
}

/* UPDONE:
 *
 * Update the current line to the virtual screen
 */
VOID PASCAL NEAR updone P1_(EWINDOW *, wp)
/* wp:  Window to update current line in  */
{
    REGISTER LINE *lp;          /* line to update */
    REGISTER int sline;         /* physical screen line to update */
    REGISTER int i;

    /* search down the line we want */
    lp = wp->w_linep;
    sline = wp->w_toprow;
    while ( lp != wp->w_dotp ) {
        ++sline;
        lp = lforw(lp);
    }

    /* and update the virtual line */
    vscreen[sline]->v_flag |= VFCHG;
    taboff = wp->w_fcol;
    vtmove(sline, -taboff);

    /* move each char of line to virtual screen until at end */
    for ( i=0; i < get_lused(lp); ++i )
        vtputc( lgetc(lp, i) );
#if     COLOR
    vscreen[sline]->v_rfcolor = wp->w_fcolor;
    vscreen[sline]->v_rbcolor = wp->w_bcolor;
#endif
    vteeol();
    taboff = 0;
}

/* UPDALL:
 *
 * Update all the lines in a window on the virtual screen
 */
VOID PASCAL NEAR updall P1_(EWINDOW *, wp)
/* wp:  Window to update lines in */
{
    REGISTER LINE *lp;          /* line to update */
    REGISTER int sline;         /* physical screen line to update */
    REGISTER int i;
    REGISTER int nlines;        /* number of lines in the current window */

    /* search down the lines, updating them */
    lp = wp->w_linep;
    sline = wp->w_toprow;
    nlines = wp->w_ntrows;
    if ( modeflag == FALSE )
        nlines++;
    taboff = wp->w_fcol;
    while ( sline < wp->w_toprow + nlines ) {

        /* and update the virtual line */
        vscreen[sline]->v_flag |= VFCHG;
        vscreen[sline]->v_left = FARRIGHT;
        vscreen[sline]->v_right = 0;
        vtmove(sline, -taboff);
        if ( lp != wp->w_bufp->b_linep ) {
            /* if we are not at the end */
            for ( i=0; i < get_lused(lp); ++i )
                vtputc( lgetc(lp, i) );
            lp = lforw(lp);
        }

        /* make sure we are on screen */
        if ( vtcol < 0 )
            vtcol = 0;

        /* on to the next one */
#if     COLOR
        vscreen[sline]->v_rfcolor = wp->w_fcolor;
        vscreen[sline]->v_rbcolor = wp->w_bcolor;
#endif
        vteeol();
        ++sline;
    }
    taboff = 0;
}

/* UPDPOS:
 *
 * Update the position of the hardware cursor and handle extended
 * lines. This is the only update for simple moves.
 */
VOID PASCAL NEAR updpos P0_()
{
    REGISTER LINE *lp;

    /* find the current row */
    lp = curwp->w_linep;
    currow = curwp->w_toprow;
    while ( lp != curwp->w_dotp ) {
        ++currow;
        lp = lforw(lp);
    }

    /* find the current column */
    curcol = getccol(FALSE);

    /* adjust by the current first column position */
    curcol -= curwp->w_fcol;

    /* make sure it is not off the left side of the screen */
    while ( curcol < 0 ) {
        if ( curwp->w_fcol >= hjump ) {
            curcol += hjump;
            curwp->w_fcol -= hjump;
        } else {
            curcol += curwp->w_fcol;
            curwp->w_fcol = 0;
        }
        curwp->w_flag |= WFHARD | WFMODE;
    }

    /* if horizontall scrolling is enabled, shift if needed */
    if ( hscroll ) {
        while ( curcol >= term.t_ncol - 1 ) {
            curcol -= hjump;
            curwp->w_fcol += hjump;
            curwp->w_flag |= WFHARD | WFMODE;
        }
    } else {
        /* if extended, flag so and update the virtual line image */
        if ( curcol >=  term.t_ncol - 1 ) {
            vscreen[currow]->v_flag |= (VFEXT | VFCHG);
            updext();
        } else
            lbound = 0;
    }
}

/* UPDDEX:
 *
 * De-extend any line that derserves it
 */
VOID PASCAL NEAR upddex P0_()
{
    REGISTER EWINDOW *wp;
    REGISTER LINE *lp;
    REGISTER int i, j;
    REGISTER int nlines;        /* number of lines in the current window */

    wp = wheadp;

    while ( wp != NULL ) {
        lp = wp->w_linep;
        i = wp->w_toprow;
        nlines = wp->w_ntrows;
        if ( modeflag == FALSE )
            nlines++;

        while ( i < wp->w_toprow + nlines ) {
            if ( vscreen[i]->v_flag & VFEXT ) {
                if ( (wp != curwp) || (lp != wp->w_dotp) ||
                     (curcol < term.t_ncol - 1) ) {
                    taboff = wp->w_fcol;
                    vtmove(i, -taboff);
                    for ( j = 0; j < get_lused(lp); ++j )
                        vtputc( lgetc(lp, j) );
                    vteeol();
                    taboff = 0;

                    /* this line no longer is extended */
                    vscreen[i]->v_flag &= ~VFEXT;
                    vscreen[i]->v_flag |= VFCHG;
                }
            }
            lp = lforw(lp);
            ++i;
        }
        /* and onward to the next window */
        wp = wp->w_wndp;
    }
}

/* UPDGAR:
 *
 * If the screen is garbage, clear the physical screen and the
 * virtual screen and force a full update
 */
VOID PASCAL NEAR updgar P0_()
{
    REGISTER int i;
#if     MEMMAP == 0
    REGISTER int j;
    REGISTER char *txt;
#endif

    for ( i = 0; i < term.t_nrow; ++i ) {
        vscreen[i]->v_flag |= VFCHG;
#if     COLOR
        vscreen[i]->v_fcolor = gfcolor;
        vscreen[i]->v_bcolor = gbcolor;
#endif
#if     MEMMAP == 0
        pscreen[i]->v_left = FARRIGHT;
        pscreen[i]->v_right = 0;
        txt = pscreen[i]->v_text;
        for ( j = 0; j < term.t_ncol; ++j )
            txt[j] = ' ';
        pscreen[i]->v_flag &= ~VFNEW;
#endif
    }

    movecursor(0, 0);                    /* Erase the screen. */
#if     COLOR && WINDOW_MSWIN
    TTforg(gfcolor);            /* inform the driver of the colors */
    TTbacg(gbcolor);            /* to be used for erase to end of page */
#endif
#if REVSTA && WINDOW_MSWIN
    TTrev(FALSE);
#endif
    TTeeop();
#if     !WINDOW_MSWIN
    sgarbf = FALSE;                      /* Erase-page clears */
    mpresf = FALSE;                      /* the message area. */
#endif
#if     COLOR
    mlerase();                          /* needs to be cleared if colored */
# if     WINDOW_MSWIN
    mpresf = FALSE;             /* MSWIN's message area handled differently.*/
# endif
#endif
}

#if !WINDOW_MSWIN
/* For simple screen size changes (no window re-allocation involved) do
 * the following things:
 */

VOID PASCAL NEAR upt_size P0_()
{
    /* if we need the size update */
    if ( (first_screen->s_roworg !=
          term.t_roworg) |
         (first_screen->s_colorg !=
          term.t_colorg) |
         (first_screen->s_nrow !=
          term.t_nrow) |(first_screen->s_ncol != term.t_ncol) ) {

        /* reset the terminal drivers size concept */
        term.t_roworg = first_screen->s_roworg;
        term.t_colorg = first_screen->s_colorg;
        term.t_nrow = first_screen->s_nrow;
        term.t_ncol = first_screen->s_ncol;

        /* make sure the update routines know we need a full update */
        sgarbf = TRUE;
    }
}
#endif

/* POP:
 *
 * Display a pop up window.  Page it for the user.  Any key other than a
 * space gets pushed back into the input stream to be interpeted later as a
 * command.
 */
int PASCAL NEAR pop P1_(BUFFER *, popbuf)
{
    REGISTER int index;         /* index into the current output line */
    REGISTER int llen;          /* length of the current output line */
    REGISTER int cline;         /* current screen line number */
    LINE *lp;           /* ptr to next line to display */
    int numlines;       /* remaining number of lines to display */
    int c;              /* input character */

    /* add the barrior line to the end of the pop up buffer */
    addline(popbuf, "------------------------------------------");

    /* set up to scan pop up buffer */
    lp = lforw(popbuf->b_linep);
    numlines = term.t_nrow-2 + !modeflag;
    cline = 0;
    mmove_flag = FALSE;         /* disable mouse move events */

    while ( lp != popbuf->b_linep ) {

        /* update the virtual screen image for this one line */
        vtmove(cline, 0);
        llen = get_lused(lp);
        for ( index = 0; index < llen; index++ )
            vtputc( lgetc(lp, index) );
        vteeol();
#if     COLOR
        vscreen[cline]->v_rfcolor = gfcolor;
        vscreen[cline]->v_rbcolor = gbcolor;
#endif
        vscreen[cline]->v_left = FARRIGHT;
        vscreen[cline]->v_right = 0;
        vscreen[cline++]->v_flag |= VFCHG|VFCOL;

        if ( numlines-- < 1 ) {

            /* update the virtual screen to the physical screen */
            updupd(FALSE);

            /* tell the user there is more */
            mlwrite("--- more ---");
            TTflush();

            /* and see if they want more */
            if ( (popwait) && ( ( c = tgetc() ) != ' ' ) ) {
                cpending = TRUE;
                charpending = c;
                upwind();

                /* re-enable mouse move events */
                mmove_flag = TRUE;

                return (TRUE);
            }

            /* reset the line counters */
            numlines = term.t_nrow-2 + !modeflag;
            cline = 0;

            /* if we at the end, don't requeue for more */
            if ( lforw(lp) == popbuf->b_linep )
                numlines = -1;

        }

        /* on to the next line */
        lp = lforw(lp);
    }
    if ( numlines >= 0 ) {

        /* update the virtual screen to the physical screen */
        updupd(FALSE);
        TTflush();

        if ( (popwait) && ( ( c = tgetc() ) != ' ' ) ) {
            cpending = TRUE;
            charpending = c;
        }
    }
    upwind();

    /* re-enable mouse move events */
    mmove_flag = TRUE;

    return (TRUE);
}

/* UPDUPD:
 *
 * Update the physical screen from the virtual screen
 */
VOID PASCAL NEAR updupd P1_(int, force)
/* force: Forced update flag  */
{
    REGISTER VIDEO *vp1;
    REGISTER int i;

    for ( i = 0; i < term.t_nrow; ++i ) {
        vp1 = vscreen[i];

        /* for each line that needs to be updated*/
        if ( vp1->v_flag & VFCHG ) {
#if     TYPEAH
            if ( force == FALSE && typahead() )
                return;

#endif
#if     MEMMAP
            upt_line(i, vp1);
#else
            upt_line(i, vp1, pscreen[i]);
#endif
        }
    }

    return;
}

/* UPDEXT:
 *
 * Update the extended line which the cursor is currently on at a
 * column greater than the terminal width. The line will be scrolled
 * right or left to let the user see where the cursor is
 */
VOID PASCAL NEAR updext P0_()
{
    REGISTER int rcursor;       /* real cursor location */
    REGISTER LINE *lp;          /* pointer to current line */
    REGISTER int j;             /* index into line */

    /* calculate what column the real cursor will end up in */
    rcursor = ( (curcol - term.t_ncol) % term.t_scrsiz )+ term.t_margin;
    lbound = curcol - rcursor + 1;
    taboff = lbound + curwp->w_fcol;

    /* scan through the line outputing characters to the virtual screen */
    /* once we reach the left edge                                  */
    vtmove(currow, -taboff);     /* start scanning offscreen */
    lp = curwp->w_dotp;                 /* line to output */
    for ( j=0; j<get_lused(lp); ++j )       /* until the end-of-line */
        vtputc( lgetc(lp, j) );

    /* truncate the virtual line, restore tab offset */
    vteeol();
    taboff = 0;

    /* and put a '$' in column 1 */
    vscreen[currow]->v_text[0] = '$';
}

/* UPT_LINE:
 *
 * Update a single line. This does not know how to use insert or delete
 * character sequences; we are using VT52 functionality. Update the physical row
 * and column variables. It does try an exploit erase to end of line.
 */

#if     MEMMAP
/* specific code for memory mapped displays */

VOID PASCAL NEAR upt_line P2_(int , row, struct VIDEO *, vp)
/* row: Row of screen to update */
/* vp:  Virtual screen image    */
{
# if     COLOR
    /* update the color request */
    vp->v_fcolor = vp->v_rfcolor;
    vp->v_bcolor = vp->v_rbcolor;

    /* write the line to the display */
    scwrite(row, vp->v_text, vp->v_fcolor, vp->v_bcolor, vp->v_left,
            vp->v_right);
# else
    /* write the line to the display */
    scwrite(row, vp->v_text, 7, 0, vp->v_left, vp->v_right);
# endif
    /* flag this line as changed */
    vp->v_flag &= ~(VFCHG | VFCOL);
}

#else

VOID PASCAL NEAR upt_line P3_(int, row, struct VIDEO *, vp,
                                 struct VIDEO *, pp)
/* row: Row of screen to update */
/* vp:  Virtual screen image    */
/* pp:  Physical screen image   */
{

    REGISTER char *vir_left;            /* left pointer to virtual line */
    REGISTER char *phy_left;            /* left pointer to physical line */
    REGISTER char *vir_right;           /* right pointer to virtual line */
    REGISTER char *phy_right;           /* right pointer to physical line */
    int rev_left;                       /* leftmost reversed char index */
    int rev_right;                      /* rightmost reversed char index */
    char *left_blank;                   /* left-most trailing blank */
    int non_blanks;                     /* non-blanks to the right flag */
    int update_column;                  /* update column */
    int old_rev_state = FALSE;          /* reverse video states */
    int new_rev_state;

    /* set up pointers to virtual and physical lines */
    vir_left = &vp->v_text[0];
    vir_right = &vp->v_text[term.t_ncol];
    phy_left = &pp->v_text[0];
    phy_right = &pp->v_text[term.t_ncol];
    update_column = 0;
    rev_left = FARRIGHT;
    rev_right = 0;
    non_blanks = TRUE;

    /* if this is a legitimate line to optimize */
    if ( !(pp->v_flag & VFNEW) ) {

        /* advance past any common chars at the left */
        while ( (vir_left != &vp->v_text[term.t_ncol])&&
                (vir_left[0] == phy_left[0]) ) {
            ++vir_left;
            ++update_column;
            ++phy_left;
        }

# if     DBCS
        /* don't optimize on the left in the middle of a 2 byte char */
        if ( (vir_left > &vp->v_text[0]) &&
             is2byte(vp->v_text, vir_left - 1) ) {
            --vir_left;
            --update_column;
            --phy_left;
        }
# endif

        /* advance past any common chars at the right */
        non_blanks = FALSE;
        while ( (vir_right[-1] == phy_right[-1]) &&(vir_right >= vir_left) ) {
            --vir_right;
            --phy_right;

            /* Note if any nonblank in right match. */
            if ( vir_right[0] != ' ' )
                non_blanks = TRUE;
        }

# if     DBCS
        /* don't stop in the middle of a 2 byte char */
        if ( is2byte(vp->v_text, vir_right-1) || is2byte(pp->v_text,
                                                         phy_right-1) ) {
            ++vir_right;
            ++phy_right;
        }
# endif
    }

# if     COLOR
    /* new line color? */
    if ( ( (vp->v_rfcolor != vp->v_fcolor) ||
           (vp->v_rbcolor != vp->v_bcolor) ) ) {
        vp->v_fcolor = vp->v_rfcolor;
        vp->v_bcolor = vp->v_rbcolor;
        vp->v_flag &= ~VFCOL;
        vir_left = &vp->v_text[0];
        vir_right = &vp->v_text[term.t_ncol];
        phy_left = &pp->v_text[0];
        phy_right = &pp->v_text[term.t_ncol];
        update_column = 0;
    }

    TTforg(vp->v_fcolor);
    TTbacg(vp->v_bcolor);
# endif

    /* reverse video changes? */
    if ( (vp->v_left != pp->v_left) || (vp->v_right != pp->v_right) ) {

        /* adjust leftmost edge */
        if ( vp->v_left < pp->v_left )
            rev_left = vp->v_left;
        else
            rev_left = pp->v_left;
        pp->v_left = vp->v_left;
        if ( rev_left < update_column ) {
            vir_left = &vp->v_text[rev_left];
            phy_left = &pp->v_text[rev_left];
            update_column = rev_left;
        }

        /* adjust rightmost edge */
        if ( vp->v_right > pp->v_right )
            rev_right = vp->v_right;
        else
            rev_right = pp->v_right;
        pp->v_right = vp->v_right;
        if ( &vp->v_text[rev_right] > vir_right ) {
            vir_right = &vp->v_text[rev_right];
            phy_right = &pp->v_text[rev_right];
        }
    } else {
        rev_left = vp->v_left;
        rev_right = vp->v_right;
    }

    /* if both lines are the same, no update needs to be done */
    if ( !(pp->v_flag & VFNEW) && (vir_left > vir_right) ) {
        vp->v_flag &= ~VFCHG;           /* flag this line as changed */

        return;
    }

    left_blank = vir_right;

    /* Erase to EOL ? */
    if ( non_blanks == FALSE && eolexist == TRUE ) {
        while ( left_blank!=vir_left && left_blank[-1]==' ' )
            --left_blank;

        if ( vir_right-left_blank <= 3 )                /* Use only if erase is
                                                         */
            left_blank = vir_right;                     /* fewer characters. */
    }

    /* move to the beginning of the text to update */
    movecursor(row, update_column);

    while ( vir_left != left_blank ) {                  /* Ordinary. */

        /* are we in a reverse video field? */
        if ( pp->v_left <= update_column && update_column < pp->v_right )
            new_rev_state = TRUE;
        else
            new_rev_state = FALSE;

        /* if moving in or out of rev video, change it */
        if ( new_rev_state != old_rev_state ) {
            TTrev(new_rev_state);
            old_rev_state = new_rev_state;
        }

        /* output the next character! */
        TTputc(*vir_left);
        ++ttcol;
        ++update_column;
        *phy_left++ = *vir_left++;
    }

    if ( left_blank != vir_right ) {            /* Erase. */

        /* are we in a reverse video field? */
        if ( pp->v_left <= update_column && update_column < pp->v_right )
            new_rev_state = TRUE;
        else
            new_rev_state = FALSE;

        /* if moving in or out of rev video, change it */
        if ( new_rev_state != old_rev_state ) {
            TTrev(new_rev_state);
            old_rev_state = new_rev_state;
        }

# if ( IS_UNIX() )
        /* TERMCAP does not tell us if the current terminal propagates the
        * current attributes to the end of the line when an erase to end of line
        * sequence is sent. Don't let TERMCAP use EEOL if in a reverse video
        * line!  (ARG...Pain....Agony....)      */
        if ( new_rev_state == TRUE )
            while ( update_column++ < term.t_ncol )
                TTputc(' ');

        else
            TTeeol();
# else
        TTeeol();
# endif
        while ( vir_left != vir_right )
            *phy_left++ = *vir_left++;
    }

    vp->v_flag &= ~VFCHG;               /* flag this line as updated */
    vp->v_flag &= ~VFCOL;

    /* Always leave in the default state */
    if ( old_rev_state == TRUE )
        TTrev(FALSE);

    return;
}
#endif

/* MODELINE:
 *
 * Redisplay the mode line for the window pointed to by the "wp". This is the
 * only routine that has any idea of how the modeline is formatted. You can
 * change the modeline format by hacking at this routine. Called by "update"
 * any time there is a dirty window.
 */
VOID PASCAL NEAR modeline P1_(EWINDOW *, wp)
/* wp:  Window to update modeline for */
{
    REGISTER CONST char *cp;
    REGISTER unsigned char c;
    REGISTER int n;             /* cursor position count */
    REGISTER BUFFER *bp;
    REGISTER int i;             /* loop index */
    REGISTER int lchar;         /* character to draw line in buffer with */
    REGISTER int firstm;        /* is this the first mode? */
    char tline[NLINE];          /* buffer for part of mode line */
    char time[6];               /* to hold current time */

    /* don't bother if there is none! */
    if ( modeflag == FALSE )
        return;

    n = wp->w_toprow+wp->w_ntrows;              /* Location. */

    /*
     * Note that we assume that setting REVERSE will cause the terminal
     * driver to draw with the inverted relationship of fcolor and
     * bcolor, so that when we say to set the foreground color to
     * "white" and background color to "black", the fact that "reverse"
     * is enabled means that the terminal driver actually draws "black"
     * on a background of "white". Makes sense, no? This way, devices
     * for which the color controls are optional will still get the
     * "reverse" signals.
     */

    vscreen[n]->v_flag |= VFCHG | VFCOL;        /* Redraw next time. */
    vscreen[n]->v_left = 0;
    vscreen[n]->v_right = term.t_ncol;
#if     COLOR
    vscreen[n]->v_rfcolor = 7;                          /* black on */
    vscreen[n]->v_rbcolor = 0;                          /* white.....*/
#endif
    vtmove(n, 0);                               /* Seek to right line. */
    if ( wp == curwp )                          /* mark the current buffer */
        lchar = '=';
    else
#if     REVSTA
    if ( revexist )
# if ( !VMS ) /* There reverse video doesn't work for trailing ' '. */
        lchar = ' ';
# else
        lchar = '-';
# endif
    else
#endif
        lchar = '-';

    bp = wp->w_bufp;
    if ( (bp->b_flag&BFTRUNC) != 0 )            /* "#" if truncated */
        vtputc('#');
    else
        vtputc(lchar);

    if ( (bp->b_flag&BFCHG) != 0 )              /* "*" if changed. */
        vtputc('*');
    else
        vtputc(lchar);

    if ( (bp->b_flag&BFNAROW) != 0 ) {                  /* "<>" if narrowed */
        vtputc('<');
        vtputc('>');
    } else {
        vtputc(lchar);
        vtputc(lchar);
    }

    n  = 4;
    XSTRCPY(tline, " ");                        /* Buffer name. */
#if     !WINDOW_MSWIN
    XSTRCAT(tline, PROGNAME);
    XSTRCAT(tline, " ");
    XSTRCAT(tline, VERSION);
    XSTRCAT(tline, " ");
#endif
    /* display the time on the bottom most modeline if active */
    if ( timeflag && wp->w_wndp == (EWINDOW *)NULL ) {

        /* get the current time/date string */
        getdtime(time);
        if ( strcmp(time, "") != 0 ) {

            /* append the hour/min string */
            XSTRCAT(tline, "[");
            XSTRCAT(tline, time);
            XSTRCAT(tline, "] ");
            xstrcpy(lasttime, time);
        }
    }

    /* display the size of the undo stack on the current modeline */
    if ( dispundo && wp == curwp ) {
        XSTRCAT(tline, "{");
        XSTRCAT( tline, long_asc(wp->w_bufp->undo_count) );
        XSTRCAT(tline, "} ");
    }

    /* are we horizontally scrolled? */
    if ( wp->w_fcol > 0 ) {
        XSTRCAT(tline, "[<");
        XSTRCAT( tline, int_asc(wp->w_fcol) );
        XSTRCAT(tline, "]");
    }

    /* display the point position in buffer if on current modeline */
    if ( posflag && wp == curwp ) {

        XSTRCAT(tline, "L:");
        XSTRCAT( tline, long_asc( getlinenum(bp, wp->w_dotp) ) );
        XSTRCAT(tline, " C:");
        XSTRCAT( tline, int_asc( getccol(FALSE) ) );
        XSTRCAT(tline, " ");
    }

    /* display the modes */
    XSTRCAT(tline, "(");
    firstm = TRUE;
    for ( i = 0; i < NUMMODES; i++ )    /* add in the mode flags */
        if ( wp->w_bufp->b_mode & (1 << i) ) {
            if ( firstm != TRUE )
                XSTRCAT(tline, " ");
            firstm = FALSE;
            XSTRCAT(tline, modename[i]);
        }
    XSTRCAT(tline, ") ");

    cp = &tline[0];
    while ( (c = *cp++) != 0 ) {
        vtputc(c);
        ++n;
    }

#if     0       /* display internal modes on modeline */
    vtputc(lchar);
    vtputc( (wp->w_flag&WFCOLR) != 0  ? 'C' : lchar );
    vtputc( (wp->w_flag&WFMODE) != 0  ? 'M' : lchar );
    vtputc( (wp->w_flag&WFHARD) != 0  ? 'H' : lchar );
    vtputc( (wp->w_flag&WFEDIT) != 0  ? 'E' : lchar );
    vtputc( (wp->w_flag&WFMOVE) != 0  ? 'V' : lchar );
    vtputc( (wp->w_flag&WFFORCE) != 0 ? 'F' : lchar );
    vtputc(lchar);
    n += 8;
#endif

    vtputc(lchar);
    vtputc(lchar);
    vtputc(' ');
    n += 3;
    cp = &bp->b_bname[0];

    while ( (c = *cp++) != 0 ) {
        vtputc(c);
        ++n;
    }

    vtputc(' ');
    vtputc(lchar);
    vtputc(lchar);
    n += 3;

    if ( bp->b_fname[0] != 0 ) {        /* File name. */
        vtputc(' ');
        ++n;
        cp = TEXT34;
/*                   "File: " */

        while ( (c = *cp++) != 0 ) {
            vtputc(c);
            ++n;
        }

        cp = &bp->b_fname[0];

        while ( (c = *cp++) != 0 ) {
            vtputc(c);
            ++n;
        }

        vtputc(' ');
        ++n;
    }

    while ( n < term.t_ncol ) {         /* Pad to full width. */
        vtputc(lchar);
        ++n;
    }
}

/* GETDTIME:
 *
 * Get the current display time string
 */
VOID PASCAL NEAR getdtime P1_(char *, ts) /***TODO: Needs a size argument***/
{
    char buf[80];

    XSTRCPY( buf, timeset() );
    if ( strcmp(buf, errorm) == 0 ) {
        *ts = 0;

        return;
    }

    buf[16] = 0;
    xstrcpy(ts, &buf[11]);

    return;
}

/* UPMODE:
 *
 * Update all the mode lines
 */
VOID PASCAL NEAR upmode P0_()
{
    REGISTER EWINDOW *wp;
#if     WINDOW_MSWIN
    SCREEN_T  *sp;

    /* loop through all screens to update even partially hidden ones. Note that
     * we process the "first" screen last */
    sp = first_screen;
    do {
        sp = sp->s_next_screen;
        if ( sp == (SCREEN_T *)NULL ) sp = first_screen;
        vtscreen (sp);
        wheadp = sp->s_first_window;
#endif

    wp = wheadp;
    while ( wp != NULL ) {
        wp->w_flag |= WFMODE;
        wp = wp->w_wndp;
    }
#if     WINDOW_MSWIN
}
while ( sp != first_screen );
#endif
}

/* UPWIND:
 *
 * Force hard updates on all windows
 */
VOID PASCAL NEAR upwind P0_()
{
    REGISTER EWINDOW *wp;
#if     WINDOW_MSWIN
    SCREEN_T  *sp;

    /* loop through all screens to update even partially hidden ones. Note that
     * we process the "first" screen last */
    sp = first_screen;
    do {
        sp = sp->s_next_screen;
        if ( sp == (SCREEN_T *)NULL ) sp = first_screen;
        vtscreen (sp);
        wheadp = sp->s_first_window;
#endif

    wp = wheadp;
    while ( wp != NULL ) {
        wp->w_flag |= WFHARD|WFMODE;
        wp = wp->w_wndp;
    }
#if     WINDOW_MSWIN
}
while ( sp != first_screen );
#endif
}

/* MOVECURSOR:
 *
 * Send a command to the terminal to move the hardware cursor to row "row"
 * and column "col". The row and column arguments are origin 0. Optimize out
 * random calls. Update "ttrow" and "ttcol".
 */
VOID PASCAL NEAR movecursor P2_(int, row, int, col)
{
#if     WINDOW_MSWIN
    /* emphasize move into message line to avoid confusion with another, larger,
     * screen */
    if ( row >= term.t_nrow )
        row = term.t_mrow;

    /* a pending update could move the cursor somewhere else, so we make sure it
     * can't happen */
    if ( defferupdate )
        update (TRUE);

    /* in MSWIN, the message line is a separate entity and a call to vtscreen
     * after a movecursor calls might actually have "stolen" the cursor away
     * from the message line! */
    if ( row!=ttrow || col!=ttcol || foulcursor ) {
        foulcursor = FALSE;
#else
    /* only move it if there is a difference */
    if ( row != ttrow || col != ttcol ) {
#endif
        ttrow = row;
        ttcol = col;
        TTmove(row, col);
    }
}

/* MLFERASE:
 *
 * Erase the message line. This is a special routine because the message line is
 * not considered to be part of the virtual screen. It always works immediately;
 * the terminal buffer is flushed via a call to the flusher.
 */
VOID PASCAL NEAR mlferase P0_()
{
    REGISTER int save_discmd;

    save_discmd = discmd;
    discmd = TRUE;
    mlerase();
    discmd = save_discmd;
    ;
}

/* MLERASE:
 */
VOID PASCAL NEAR mlerase P0_()
{
    int i;

    movecursor(term.t_nrow, 0);
    if ( discmd == FALSE )
        return;

#if     COLOR
    TTforg(gfcolor);
    TTbacg(gbcolor);
#endif

    if ( eolexist == TRUE )
        TTeeol();
    else {
        for ( i = 0; i < term.t_ncol - 1; i++ )
            TTputc(' ');

        /* force the move! */
/*      movecursor(term.t_nrow, 1);*/
        movecursor(term.t_nrow, 0);
    }
    TTflush();
    mpresf = FALSE;
}

/* MLOUT:
 *
 * Write a message into the message line. Keep track of the physical cursor
 * position. A small class of printf like format items is handled. Assumes the
 * stack grows down; this assumption is made by the "+=" in the argument scan
 * loop. If  STACK_GROWS_UP  is set in estruct.h, then we'll assume that the
 * stack grows up and use "-=" instead of "+=". Set the "message line"
 * flag TRUE.  Don't write beyond the end of the current terminal width.
 */
VOID PASCAL NEAR mlout P1_(int, c)
/* c: Character to write  */
{
#if WINDOW_MSWIN
    if ( ttcol + 1 < NSTRING ) {
#else
    if ( ttcol + 1 < term.t_ncol ) {
#endif
#if     WINNT || WINXP
        putchar(c);
        fflush(stdout);
#endif
        TTputc(c);
    }
    if ( c != '\b' )
        *lastptr++ = c;
    else if ( lastptr > &lastmesg[0] )
        --lastptr;
}

#if     VARARG

# if     VARG

VOID CDECL NEAR mlwrite(va_alist)
/* Variable argument list:                */
/*  arg1  = format string                 */
/*  arg2+ = arguments in that string:     */
va_dcl
{
    REGISTER int c;             /* current char in format string */
    REGISTER char *fmt;         /* ptr to format string */
    REGISTER va_list ap;        /* ptr to current data field */
    int arg_int;                /* integer argument */
    long arg_long;              /* long argument */
    char *arg_str;              /* string argument */

    /* if we are not currently echoing on the command line, abort this */
    if ( discmd == FALSE )
        return;

#  if     COLOR
    /* set up the proper colors for the command line */
    TTforg(gfcolor);
    TTbacg(gbcolor);
#  endif

    /* point to the first argument */
    va_start(ap);
    fmt = va_arg(ap, char *);

    /* if we can not erase to end-of-line, do it manually */
    if ( eolexist == FALSE ) {
        mlerase();
        TTflush();
    }

    movecursor(term.t_nrow, 0);
    lastptr = &lastmesg[0];             /* setup to record message */
    while ( (c = *fmt++) != 0 ) {
        if ( c != '%' ) {
            mlout(c);
            ++ttcol;
        } else {
            c = *fmt++;
            switch ( c ) {
            case 'd':
                arg_int = va_arg(ap, int);
                mlputi(arg_int, 10);
                break;

            case 'o':
                arg_int = va_arg(ap, int);
                mlputi(arg_int, 8);
                break;

            case 'x':
                arg_int = va_arg(ap, int);
                mlputi(arg_int, 16);
                break;

            case 'D':
                arg_long = va_arg(ap, long);
                mlputli(arg_long, 10);
                break;

            case 's':
                arg_str = va_arg(ap, char *);
                mlputs(arg_str);
                break;

            case 'f':
                arg_int = va_arg(ap, int);
                mlputf(arg_int);
                break;

            default:
                mlout(c);
                ++ttcol;
            }
        }
    }

    /* if we can, erase to the end of screen */
    if ( eolexist == TRUE )
        TTeeol();
    TTflush();
    mpresf = TRUE;
    *lastptr = 0;       /* terminate lastmesg[] */
    va_end(ap);
}

# else

#   if PROTO
VOID CDECL NEAR mlwrite(CONST char *fmt, ...)
#   else
VOID CDECL NEAR mlwrite()
    CONST char  *fmt;
#   endif
/* Variable argument list:                */
/*  arg1  = format string                 */
/*  arg2+ = arguments in that string:     */
{
    REGISTER int c;             /* current char in format string */
    va_list ap;                 /* ptr to current data field */
    int arg_int;                /* integer argument */
    long arg_long;              /* long argument */
    char *arg_str;              /* string argument */

    /* if we are not currently echoing on the command line, abort this */
    if ( discmd == FALSE )
        return;

#  if     COLOR
    /* set up the proper colors for the command line */
    TTforg(gfcolor);
    TTbacg(gbcolor);
#  endif

    /* point to the first argument */
    va_start(ap, fmt);

    /* if we can not erase to end-of-line, do it manually */
    if ( eolexist == FALSE ) {
        mlerase();
        TTflush();
    }

    movecursor(term.t_nrow, 0);
    lastptr = &lastmesg[0];             /* setup to record message */
    while ( (c = *fmt++) != 0 ) {
        if ( c != '%' ) {
            mlout(c);
            ++ttcol;
        } else {
            c = *fmt++;
            switch ( c ) {
            case 'd':
                arg_int = va_arg(ap, int);
                mlputi(arg_int, 10);
                break;

            case 'o':
                arg_int = va_arg(ap, int);
                mlputi(arg_int, 8);
                break;

            case 'x':
                arg_int = va_arg(ap, int);
                mlputi(arg_int, 16);
                break;

            case 'D':
                arg_long = va_arg(ap, long);
                mlputli(arg_long, 10);
                break;

            case 's':
                arg_str = va_arg(ap, char *);
                mlputs(arg_str);
                break;

            case 'f':
                arg_int = va_arg(ap, int);
                mlputf(arg_int);
                break;

            default:
                mlout(c);
                ++ttcol;
            }
        }
    }

    /* if we can, erase to the end of screen */
    if ( eolexist == TRUE )
        TTeeol();
    TTflush();
    mpresf = TRUE;
    *lastptr = 0;       /* terminate lastmesg[] */
    va_end(ap);
}

# endif

#else

# if     STACK_GROWS_UP
#  define ADJUST(ptr, dtype)      ptr -= SIZEOF (dtype)
# else
#  define ADJUST(ptr, dtype)      ptr += SIZEOF (dtype)
# endif

VOID CDECL NEAR mlwrite P1_(CONST char *, fmt)
/* fmt: Format string for output  */
{
    REGISTER int c;             /* current char in format string */
    REGISTER char *ap;          /* ptr to current data field */

    /* if we are not currently echoing on the command line, abort this */
    if ( discmd == FALSE )
        return;

# if     COLOR
    /* set up the proper colors for the command line */
    TTforg(gfcolor);
    TTbacg(gbcolor);
# endif

    /* point to the first argument */
    ap = &fmt;
    ADJUST(ap, char *);

    /* if we can not erase to end-of-line, do it manually */
    if ( eolexist == FALSE ) {
        mlerase();
        TTflush();
    }

    movecursor(term.t_nrow, 0);
    lastptr = &lastmesg[0];             /* setup to record message */
    while ( (c = *fmt++) != 0 ) {
        if ( c != '%' ) {
            mlout(c);
            ++ttcol;
        } else {
            c = *fmt++;
            switch ( c ) {
            case 'd':
                mlputi(*(int *)ap, 10);
                ADJUST(ap, int);
                break;

            case 'o':
                mlputi(*(int *)ap, 8);
                ADJUST(ap, int);
                break;

            case 'x':
                mlputi(*(int *)ap, 16);
                ADJUST(ap, int);
                break;

            case 'D':
                mlputli(*(long *)ap, 10);
                ADJUST(ap, long);
                break;

            case 's':
                mlputs(*(char **)ap);
                ADJUST(ap, char *);
                break;

            case 'f':
                mlputf(*(int *)ap);
                ADJUST(ap, int);
                break;

            default:
                mlout(c);
                ++ttcol;
            }
        }
    }

    /* if we can, erase to the end of screen */
    if ( eolexist == TRUE )
        TTeeol();
    TTflush();
    mpresf = TRUE;
    *lastptr = 0;       /* terminate lastmesg[] */
}

#endif

/* MLFORCE:
 *
 * Force a string out to the message line regardless of the current $discmd
 * setting. This is needed when $debug is TRUE and for the write-message and
 * clear-message-line commands
 */
VOID PASCAL NEAR mlforce P1_(CONST char *, s)
/* s: String to force out */
{
    REGISTER int oldcmd;        /* original command display flag */

    oldcmd = discmd;            /* save the discmd value */
    discmd = TRUE;              /* and turn display on */
    mlwrite("%s", s);           /* write the string out */
    discmd = oldcmd;            /* and restore the original setting */
}


#if     !WINDOW_MSWIN

/* MLABORT:
 *
 * Display a serious error message (like "out of memory"). This is replaced by a
 * system-specific function when a multitasking system that does not like these
 * kind of errors is used, so that the user can be offered to abort the
 * application
 */
VOID PASCAL NEAR mlabort P1_(CONST char *, s)
{
    mlforce(s);
    abort();
}

#endif

/* MLPUTS:
 *
 * Write out a string. Update the physical cursor position. This assumes that
 * the characters in the string all have width "1"; if this is not the case
 * things will get screwed up a little.
 */
VOID PASCAL NEAR mlputs P1_(CONST char *, s)
{
    REGISTER int c;

    while ( (c = *s++) != 0 ) {
        mlout(c);
        ++ttcol;
    }
}

/* MLPUTI:
 *
 * Write out an integer, in the specified radix. Update the physical cursor
 * position.
 */
VOID PASCAL NEAR mlputi P2_(int, i, int, r)
{
    REGISTER int q;
    static char hexdigits[] = "0123456789ABCDEF";

    if ( i < 0 ) {
        i = -i;
        mlout('-');
    }

    q = i/r;

    if ( q != 0 )
        mlputi(q, r);

    mlout(hexdigits[i%r]);
    ++ttcol;
}

/* MLPUTLI:
 *
 * do the same except as a long integer.
 */
VOID PASCAL NEAR mlputli P2_(long, l, int, r)
{
    REGISTER long q;

    if ( l < 0 ) {
        l = -l;
        mlout('-');
    }

    q = l/r;

    if ( q != 0 )
        mlputli(q, r);

    mlout( (int)(l%r)+'0' );
    ++ttcol;
}

/* MLPUTF:
 *
 * Write out a scaled integer with two decimal places
 */
VOID PASCAL NEAR mlputf P1_(int, s)
/* s: Scaled integer to output  */
{
    int i;      /* integer portion of number */
    int f;      /* fractional portion of number */

    /* break it up */
    i = s / 100;
    f = s % 100;

    /* send out the integer portion */
    mlputi(i, 10);
    mlout('.');
    mlout( (f / 10) + '0' );
    mlout( (f % 10) + '0' );
    ttcol += 3;
}

#if HANDLE_WINCH

/* WINCH_VTRESIZE:
 */
VOID winch_vtresize P2_(int, rows, int, cols)
{
    int             i   = 0;
    REGISTER VIDEO  *vp = NULL;

    for ( i = 0; i < term.t_mrow; ++i ) {
        CLROOM(vscreen[i]);
        CLROOM(pscreen[i]);
    }
    CLROOM(vscreen);
    CLROOM(pscreen);

    term.t_mrow=term.t_nrow=rows-1;
    term.t_mcol=term.t_ncol=cols;

    vscreen = (VIDEO **)ROOM( term.t_mrow * SIZEOF (VIDEO *) );

    if ( vscreen == NULL )
        meexit(1);

    pscreen = (VIDEO **)ROOM( term.t_mrow*SIZEOF (VIDEO *) );

    if ( pscreen == NULL )
        meexit(1);

    for ( i = 0; i < term.t_mrow; ++i ) {
        vp = (VIDEO *)ROOM(SIZEOF (VIDEO)+term.t_mcol);

        if ( vp == NULL )
            meexit(1);

        vp->v_flag = 0;
        vp->v_left = FARRIGHT;
        vp->v_right = 0;
        vp->v_flag = VFNEW;
# if     COLOR
        vp->v_rfcolor = 7;
        vp->v_rbcolor = 0;
# endif
# if     INSDEL
        vp->v_rline = i;
# endif
        vscreen[i] = vp;
        vp = (VIDEO *)ROOM(SIZEOF (VIDEO)+term.t_mcol);

        if ( vp == NULL )
            meexit(1);

        vp->v_flag = VFNEW;
        vp->v_left = FARRIGHT;
        vp->v_right = 0;
# if     INSDEL
        vp->v_rline = i; /* set requested line position */
# endif

        pscreen[i] = vp;
    }
}

#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
