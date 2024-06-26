/*======================================================================
 *      SCREEN.C:       Screen manipulation commands
 *                      for MicroEMACS 4.00
 *                      written by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#define SCREEN_C_
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


#if     0

VOID dumpscreens P1_(char *, msg)
{
    SCREEN_T *sp;

    printf("<%s>\n", msg);
    sp = first_screen;
    while ( sp != (SCREEN_T *)NULL ) {
        printf("%lu - [%s] %d/%d to %d/%d \n",
               sp,
               sp->s_screen_name,
               sp->s_roworg,
               sp->s_colorg,
               sp->s_nrow,
               sp->s_ncol);
        sp = sp->s_next_screen;
    }
    printf("   0     -   [EOL]\n");
    tgetc();
}

#endif

#if     WINDOW_TEXT

/* refresh_screen:
 *
 * Redraw given screen and all screens behind it
 */
VOID PASCAL NEAR refresh_screen P1_(SCREEN_T *, sp)
/* sp:  Screen image to refresh */
{
    /* if we are suppressing redraws */
    if ( gflags & GFSDRAW )
        return;

    /* at end of list, do nothing */
    if ( sp == (SCREEN_T *)NULL )
        return;

    /* if first refresh, erase the page */
    if ( sp == first_screen ) {
        (*term.t_clrdesk)();
        if ( sp->s_next_screen == (SCREEN_T *)NULL )
            sgarbf = TRUE;
    }

    /* if there are others below, defer to them first */
    if ( sp->s_next_screen )
        refresh_screen(sp->s_next_screen);

    select_screen(sp, FALSE);
    update(TRUE);
}

#endif  /* WINDOW_TEXT  */

/* CYCLE_SCREENS:
 *
 * This command takes the last window in the linked window list, which
 * is visibly rearmost, and brings it to front. It is bound to A-N on
 * machines with an ALT key
 */
int PASCAL NEAR cycle_screens P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    SCREEN_T *sp;               /* ptr to screen to switch to */

    /* find the last screen */
    sp = first_screen;
    while ( sp->s_next_screen != (SCREEN_T *)NULL )
        sp = sp->s_next_screen;

    /* and make this screen current */
    return ( select_screen(sp, TRUE) );
}

/* FIND_SCREEN:
 */
int PASCAL NEAR find_screen P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    char scr_name[NSTRING];     /* buffer to hold screen name */
    SCREEN_T *sp;               /* ptr to screen to switch to */
    int result;

    /* get the name of the screen to switch to */
    if ( ( result = mlreply(TEXT242, scr_name, NSTRING) ) != TRUE ) {
        /* "Find Screen: " */
        return result;
    }
    sp = lookup_screen(scr_name);

    if ( sp == (SCREEN_T *)NULL ) {

        /* save the current dot position in the buffer info so the new screen
         * will start there! */
        curbp->b_dotp = curwp->w_dotp;
        set_b_doto(curbp, get_w_doto(curwp));

        /* screen does not exist, create it */
        sp = init_screen(scr_name, curbp);
    }

    /* and make this screen current */
    return ( select_screen(sp, TRUE) );
}

/* free_screen:
 *
 * Free all resouces associated with a screen
 */
VOID PASCAL NEAR free_screen P1_(SCREEN_T *, sp)
/* sp:  Screen to dump  */
{
    REGISTER int      cmark = 0;      /* mark ordinal index     */
    EWINDOW           *wp   = NULL;   /* ptr to window to free  */
    REGISTER EWINDOW  *tp   = NULL;   /* temp window pointer    */

    /* first, free the screen's windows */
    wp = sp->s_first_window;
    while ( wp ) {
        if ( --wp->w_bufp->b_nwnd == 0 ) {
            wp->w_bufp->b_dotp  = wp->w_dotp;
            set_b_doto(wp->w_bufp, get_w_doto(wp));
            for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                wp->w_bufp->b_markp[cmark] = wp->w_markp[cmark];
                wp->w_bufp->b_marko[cmark] = wp->w_marko[cmark];
            }
            wp->w_bufp->b_fcol  = wp->w_fcol;
        }

        /* on to the next window, free this one */
        tp = wp->w_wndp;
        CLROOM(wp);
        wp = tp;
    }

#if     WINDOW_MSWIN
    term.t_delscr(sp);
#endif
    /* and now, free the screen struct itself */
    CLROOM(sp->s_screen_name);
    CLROOM(sp);
}

/* UNLIST_SCREEN:
 */
VOID PASCAL NEAR unlist_screen P1_(SCREEN_T *, sp)
/* sp:  Screen to remove from the list  */
{
    SCREEN_T *last_scr;         /* screen previous to one to delete */

    last_scr = first_screen;
    while ( last_scr ) {
        if ( last_scr->s_next_screen == sp )
            break;
        last_scr = last_scr->s_next_screen;
    }
    last_scr->s_next_screen = sp->s_next_screen;
}

/* DELETE_SCREEN:
 */
int PASCAL NEAR delete_screen P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    char scr_name[NSTRING];     /* buffer to hold screen name */
    SCREEN_T *sp;               /* ptr to screen to switch to */
    int result;

    /* get the name of the screen to delete */
    if ( ( result = mlreply(TEXT243, scr_name, NSTRING) ) != TRUE ) {
        /* "Delete Screen: " */
        return result;
    }
    sp = lookup_screen(scr_name);

    /* make sure it exists... */
    if ( sp == (SCREEN_T *)NULL ) {
        mlwrite(TEXT240);           /* "[No such screen]" */

        return (FALSE);
    }

    /* it can't be current... */
    if ( sp == first_screen ) {
        mlwrite(TEXT241);           /* "%%Can't delete current screen" */

        return (FALSE);
    }

    unlist_screen(sp);
    free_screen(sp);
#if     WINDOW_TEXT
    refresh_screen(first_screen);
#endif

    return (TRUE);
}

/* INIT_SCREEN:
 *
 * This function initializes a new screen....
 */
SCREEN_T *PASCAL NEAR init_screen P2_(CONST char *, scr_name,
                                      BUFFER *,     scr_buf)
/* scr_name:  screen name                               */
/* scr_buf:   buffer to place in first window of screen */
{
    int cmark;                  /* current mark to initialize */
    SCREEN_T *sp;               /* pointer to allocated screen */
    SCREEN_T *last_sp;          /* pointer to last screen */
    EWINDOW *wp;                /* ptr to first window of new screen */

    /* allocate memory for this screen */
    sp = (SCREEN_T *)ROOM( SIZEOF (SCREEN_T) );
    if ( sp == (SCREEN_T *)NULL )
        return (sp);

    /* set up this new screens fields! */
    sp->s_next_screen = (SCREEN_T *)NULL;
    sp->s_screen_name = copystr(scr_name);
#if     WINDOW_MSWIN
    if ( term.t_newscr (sp) != TRUE ) {         /* failed */
        CLROOM (sp);

        return ( (SCREEN_T *)NULL );
    }
    /* ... in MSWIN, the s_nrow/ncol etc... values are kept up to date by
     * vtinitscr; besides, term entries may actually match the first_screen
     * instead of the new screen */
    term.t_roworg = sp->s_roworg;
    term.t_colorg = sp->s_colorg;
    term.t_nrow = sp->s_nrow;
    term.t_ncol = sp->s_ncol;
#else
    sp->s_roworg = term.t_roworg;
    sp->s_colorg = term.t_colorg;
    sp->s_nrow = term.t_nrow;
    sp->s_ncol = term.t_ncol;
#endif

    /* allocate its first window */
    wp = (EWINDOW *)ROOM( SIZEOF (EWINDOW) );
    if ( wp == (EWINDOW *)NULL ) {
        CLROOM(sp);

        return ( (SCREEN_T *)NULL );
    }
    sp->s_first_window = sp->s_cur_window = wp;

    /* and setup the windows info */
    wp->w_wndp = NULL;
    wp->w_bufp = scr_buf;
    scr_buf->b_nwnd += 1;
    wp->w_linep = scr_buf->b_linep;

    /* position us at the buffers dot */
    wp->w_dotp  = scr_buf->b_dotp;
    set_w_doto(wp, get_b_doto(scr_buf));

    /* set all the marks to UNSET */
    for ( cmark = 0; cmark < NMARKS; cmark++ ) {
        wp->w_markp[cmark] = NULL;
        wp->w_marko[cmark] = 0;
    }
    wp->w_toprow = 0;
#if     COLOR
    /* initalize colors to global defaults */
    wp->w_fcolor = gfcolor;
    wp->w_bcolor = gbcolor;
#endif
    wp->w_fcol = 0;
#if WINDOW_MSWIN
    wp->w_ntrows = sp->s_nrow-1;
#else
    wp->w_ntrows = term.t_nrow-1;               /* "-1" for mode line.  */
#endif
    wp->w_force = 0;
    wp->w_flag  = WFMODE|WFHARD;                /* Full.        */

    /* first screen? */
    if ( first_screen == (SCREEN_T *)NULL ) {
        first_screen = sp;

        return (sp);
    }

    /* insert it at the tail of the screen list */
    last_sp = first_screen;
    while ( last_sp->s_next_screen != (SCREEN_T *)NULL )
        last_sp = last_sp->s_next_screen;
    last_sp->s_next_screen = sp;

    /* and return the new screen pointer */
    return (sp);
}

/* LOOKUP_SCREEN:
 */
SCREEN_T *PASCAL NEAR lookup_screen P1_(CONST char *, scr_name)
/* scr_name:  Named screen to find  */
{
    SCREEN_T *result;

    /* scan the screen list */
    result = first_screen;
    while ( result ) {

        /* if this is it, return its handle! */
        if ( strcmp(scr_name, result->s_screen_name) == 0 )
            return (result);

        /* on to the next screen */
        result = result->s_next_screen;
    }

    /* we didn't find it..... */
    return ( (SCREEN_T *)NULL );
}

/* SELECT_SCREEN:
 */
int PASCAL NEAR select_screen P2_(SCREEN_T *, sp, int, announce)
/* sp:        Ptr to screen to switch to  */
/* announce:  Announce the selection?     */
{
    EWINDOW *temp_wp;           /* backup of current window ptr (curwp) */
    SCREEN_T *temp_screen;      /* temp ptr into screen list */

    /* make sure there is something here to set to! */
    if ( sp == (SCREEN_T *)NULL )
        return (FALSE);

    /* nothing to do, it is already current */
    if ( sp == first_screen )
        return (TRUE);

    /* deselect the current window */
#if     WINDOW_MSWIN
    curwp->w_flag |= WFMODE;
#else
    temp_wp = curwp;
    curwp = (EWINDOW *)NULL;
    modeline(temp_wp);
    updupd(TRUE);
    curwp = temp_wp;
#endif

    /* save the current screens concept of current window */
    first_screen->s_cur_window = curwp;
#if     WINDOW_MSWIN
    /* in MSWIN, the term entries may (but not always) already reflect the new
     * screen's size and the s_n... stuff is always kept up to date by vtinitscr
     */
    vtscreen (sp);
#else
    first_screen->s_roworg = term.t_roworg;
    first_screen->s_colorg = term.t_colorg;
    first_screen->s_nrow = term.t_nrow;
    first_screen->s_ncol = term.t_ncol;
#endif

    /* re-order the screen list */
    temp_screen = first_screen;
    while ( temp_screen->s_next_screen != (SCREEN_T *)NULL ) {
        if ( temp_screen->s_next_screen == sp ) {
            temp_screen->s_next_screen = sp->s_next_screen;
            break;
        }
        temp_screen = temp_screen->s_next_screen;
    }
    sp->s_next_screen = first_screen;
    first_screen = sp;

    /* reset the current screen, window and buffer */
    wheadp = first_screen->s_first_window;
    curwp = first_screen->s_cur_window;
    curbp = curwp->w_bufp;

    /* let the display driver know we need a full screen update */
#if     WINDOW_MSWIN
    term.t_topscr (first_screen);
    curwp->w_flag |= WFMODE;
    update(FALSE);
#else
    update_size();
    upwind();
#endif
    if ( announce ) {
        mlwrite(TEXT225, first_screen->s_screen_name);
/*          "[Switched to screen %s]" */
    }

    return (TRUE);
}

/* LIST_SCREENS:
 *
 * Build and popup a buffer containing the list of all screens. Bound
 * to "A-B".
 */
int PASCAL NEAR list_screens P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int status;        /* stutus return */

    if ( ( status = screenlist(f) ) != TRUE )
        return (status);

    return ( wpopup(slistp) );
}


/* SCREENLIST:
 *
 * This routine rebuilds the text in the special secret buffer that holds the
 * screen list. It is called by the list screens command. Return TRUE if
 * everything works. Return FALSE if there is an error (if there is no memory).
 * Iflag indicates whether to list hidden screens.
 */
int PASCAL NEAR screenlist P1_(int, iflag)
/* iflag: List hidden screen flag */
{
    SCREEN_T *sp;               /* ptr to current screen to list */
    EWINDOW *wp;                /* ptr into current screens window list */
    int status;                 /* return status from functions */
    char line[NSTRING];         /* buffer to construct list lines */
    char bname[NSTRING];        /* name of next buffer */

    /* mark this buffer as unchanged so... */
    slistp->b_flag &= ~BFCHG;

    /* we can dump it's old contents without complaint */
    if ( ( status = bclear(slistp) ) != TRUE )
        return (status);

    /* there is no file connected with this buffer */
    XSTRCPY(slistp->b_fname,
            "");

    /* construct the header of this list */
    if ( addline(slistp,
                 "Screen         Buffers") == FALSE|| addline(slistp,
                                                              "------         -------")
         ==
         FALSE )
        return (FALSE);

    /* starting from the first screen */
    sp = first_screen;

    /* scan all the screens */
    while ( sp ) {

        /* construct the screen name */
        XSTRCPY(line, sp->s_screen_name);
        XSTRCAT(line, "                ");
        line[15] = 0;

        /* list this screens windows's buffer names */
        wp = sp->s_first_window;
        while ( wp ) {

            /* grab this window's buffer name */
            XSTRCPY(bname, wp->w_bufp->b_bname);

            /* handle full lines */
            if ( STRLEN(line) + STRLEN(bname) + 1 > 78 ) {
                if ( addline(slistp, line) == FALSE )
                    return (FALSE);

                XSTRCPY(line, "               ");
            }

            /* append this buffer name */
            if ( STRLEN(line) > 15 )
                XSTRCAT(line, " ");
            XSTRCAT(line, bname);

            /* on to the next window */
            wp = wp->w_wndp;
        }

        /* and add the line to the buffer */
        if ( addline(slistp, line) == FALSE )
            return (FALSE);

        /* on to the next screen */
        sp = sp->s_next_screen;
    }

    /* all constructed! */
    return (TRUE);
}

/* RENAME_SCREEN:
 *
 * Change the current screen's name
 */
int PASCAL NEAR rename_screen P2_(int, f, int, n)
/* f, n:  Default number and arguments  */
{
    char scr_name[NSTRING];      /* buffer to hold screen name */
    int result;

    /* get the new name of the screen */
    if ( ( result = mlreply(TEXT335, scr_name, NSTRING) ) != TRUE ) {
/*                "Change screen name to: " */
        return (result);
    }

    if ( lookup_screen(scr_name) != (SCREEN_T*)NULL ) {
        mlwrite(TEXT336);

/*          "[Screen name already in use]" */
        return (FALSE);
    }

    /* replace the old screen name with the new */
    CLROOM(first_screen->s_screen_name);
    first_screen->s_screen_name = copystr(scr_name);
#if     WINDOW_MSWIN
    SetWindowText(first_screen->s_drvhandle, scr_name);
#endif

    return (TRUE);

}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
