/*======================================================================
 *
 * The functions in this file are a general set of line management
 * utilities. They are the only routines that touch the text. They also
 * touch the buffer and window structures, to make sure that the
 * necessary updating gets done. There are routines in this file that
 * handle the kill buffer too. It isn't here for any good reason.

 * Note that this code only updates the dot and mark values in the
 * window list. Since all the code acts on the current window, the
 * buffer that we are editing must be being displayed, which means that
 * "b_nwnd" is non zero, which means that the dot and mark values in
 * the buffer headers are nonsense.
 *
 *====================================================================*/

#include        <stdio.h>
#include        "estruct.h"
#include        "eproto.h"
#include        "edef.h"
#include        "elang.h"

#define BSIZE(a)        (a + NBLOCK - 1) & ( ~(NBLOCK - 1) )

static long last_size = -1L;    /* last # of bytes yanked */

/* LALLOC:
 *
 * This routine allocates a block of memory large enough to hold a LINE
 * containing "used" characters plus one trailing '\0'.
 * Return a pointer to the new block, or NULL if there isn't any memory
 * left. Print a message in the message line if no space.
 */
LINE *PASCAL NEAR lalloc P1_(REGISTER int, used)
{
    REGISTER LINE *lp = NULL;

    if ( ( lp = (LINE *)room( sizeof (LINE) + used ) ) == NULL ) {
        mlabort(TEXT94);

/*                      "%%Out of memory" */
        return (NULL);
    }
    lp->l_size_ = used;
    /* JES_TODO: Setting l_used to 0 here dos *not* work, but I don't know why. */
    lp->l_used_ = used;
#if     WINDOW_MSWIN
    {
        static int o = 0;
        if ( --o < 0 ) {
            longop(TRUE);
            o = 10;                 /* to lower overhead, only 10% calls to
                                     * longop */
        }
    }
#endif

    return (lp);
}

/* LFREE:
 *
 * Delete line "lp". Fix all of the links that might point at it (they are moved
 * to offset 0 of the next line. Unlink the line from whatever buffer it might
 * be in. Release the memory. The buffers are updated too; the magic conditions
 * described in the above comments don't hold here.
 */
int PASCAL NEAR lfree P1_(REGISTER LINE *, lp)
{
    REGISTER BUFFER   *bp     = NULL;
    SCREEN_T          *scrp   = NULL; /* screen to fix pointers in  */
    REGISTER EWINDOW  *wp     = NULL;
    REGISTER int      cmark   = 0;    /* current mark               */

    /* in all screens.... */
    scrp = first_screen;
    while ( scrp ) {
        wp = scrp->s_first_window;
        while ( wp != NULL ) {
            if ( wp->w_linep == lp )
                wp->w_linep = lforw(lp);
            if ( wp->w_dotp  == lp ) {
                wp->w_dotp  = lforw(lp);
                set_w_doto(wp, 0);
            }
            for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                if ( wp->w_markp[cmark] == lp ) {
                    wp->w_markp[cmark] = lforw(lp);
                    wp->w_marko[cmark] = 0;
                }
            }
            wp = wp->w_wndp;
        }

        /* next screen! */
        scrp = scrp->s_next_screen;
    }

    bp = bheadp;
    while ( bp != NULL ) {
        if ( bp->b_nwnd == 0 ) {
            if ( bp->b_dotp  == lp ) {
                bp->b_dotp = lforw(lp);
                set_b_doto(bp, 0);
            }
            for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                if ( bp->b_markp[cmark] == lp ) {
                    bp->b_markp[cmark] = lforw(lp);
                    bp->b_marko[cmark] = 0;
                }
            }
        }
        bp = bp->b_bufp;
    }
    lp->l_bp->l_fp = lp->l_fp;
    lp->l_fp->l_bp = lp->l_bp;
    free( (char *) lp );
#if     WINDOW_MSWIN
    {
        static int o = 0;
        if ( --o < 0 ) {
            longop(TRUE);
            o = 10;                 /* to lower overhead, only 10% calls to
                                     * longop */
        }
    }
#endif

    return 0;
}

/* LCHANGE:
 *
 * This routine gets called when a character is changed in place in the current
 * buffer. It updates all of the required flags in the buffer and window system.
 * The flag used is passed as an argument; if the buffer is being displayed in
 * more than 1 window we change EDIT t HARD. Set MODE if the mode line needs to
 * be updated (the "*" has to be set).
 */
int PASCAL NEAR lchange P1_(int, flag)
{
    REGISTER EWINDOW  *wp   = NULL;
    SCREEN_T          *scrp = NULL; /* screen to fix pointers in  */

    if ( curbp->b_nwnd != 1 )                   /* Ensure hard.     */
        flag = WFHARD;
    if ( (curbp->b_flag&BFCHG) == 0 ) {         /* First change, so */
        flag |= WFMODE;                         /* update mode lines.   */
        curbp->b_flag |= BFCHG;
    }

    /* in all screens.... */
    scrp = first_screen;
    while ( scrp ) {
        /* make sure all the needed windows get this flag */
        wp = scrp->s_first_window;
        while ( wp != NULL ) {
            if ( wp->w_bufp == curbp )
                wp->w_flag |= flag;
            wp = wp->w_wndp;
        }

        /* next screen! */
        scrp = scrp->s_next_screen;
    }

    return 0;
}

/* INSSPACE:
 *
 * Insert spaces forward into text
 */
int PASCAL NEAR insspace P2_(int, f, int, n)
/* f, n:  default flag and numeric argument */
{
    REGISTER int  status  = 0;

    if ( ( status = linsert(n, ' ') ) == TRUE )
        status = backchar(f, n);

    return status;
}

/* LINSTR:
 *
 * Insert a string at the current point
 */
int PASCAL NEAR linstr P1_(CONST char *, instr)
{
    REGISTER int status     = 0;
    REGISTER int saved_undo = 0;  /* saved undo flag  */

    status = TRUE;
    if ( instr != NULL && *instr != '\0' ) {
        /* record the insertion for the undo stack.... */
        undo_insert(OP_ISTR, (long)strlen(instr), obj);

        /* insert the string one character at a time */
        saved_undo = undoing;
        undoing = TRUE;
        do {
            status = ( (*instr == '\r') ? lnewline(): linsert(1, *instr) );

            /* Insertion error? */
            if ( status != TRUE ) {
                mlwrite(TEXT168);
/*                                      "%%Can not insert string" */
                break;
            }
            instr++;
        } while (*instr);
        undoing = saved_undo;
    }

    return (status);
}

/* LINSERT:
 *
 * Insert "n" copies of the character "c" at the current location of dot. In the
 * easy case all that happens is the text is stored in the line. In the hard
 * case, the line has to be reallocated. When the window list is updated, take
 * special care; I screwed it up once. You always update dot in the current
 * window. You update mark, and a dot in another window, if it is greater than
 * the place where you did the insert. Return TRUE if all is well, and FALSE on
 * errors.
 */
int PASCAL NEAR linsert P2_(int, n, char, c)
{
    REGISTER char     *cp1  = NULL;
    REGISTER char     *cp2  = NULL;
    REGISTER LINE     *lp1  = NULL;
    REGISTER LINE     *lp2  = NULL;
    REGISTER LINE     *lp3  = NULL;
    REGISTER int      doto  = 0;
    REGISTER int      i     = 0;
    REGISTER EWINDOW  *wp   = NULL;
    SCREEN_T          *scrp = NULL; /* screen to fix pointers in  */
    int               cmark = 0;    /* current mark               */

    if ( curbp->b_mode&MDVIEW )         /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode */

    /* a zero insert means do nothing! */
    if ( n == 0 )
        return (TRUE);

    /* Negative numbers of inserted characters are right out! */
    if ( n < 1 )
        return (FALSE);

    /* remember we did this! */
    obj.obj_char = c;
    undo_insert(OP_INSC, (long)n, obj);

    /* mark the current window's buffer as changed */
    lchange(WFEDIT);

    lp1 = curwp->w_dotp;                        /* Current line     */
    if ( lp1 == curbp->b_linep ) {              /* At the end: special  */
        if ( get_w_doto(curwp) != 0 ) {
            mlwrite(TEXT170);

/*                              "bug: linsert" */
            return (FALSE);
        }
        if ( ( lp2=lalloc( BSIZE(n) ) ) == NULL )       /* Allocate new line    */
            return (FALSE);

        set_lused(lp2, n);
        lp3 = lp1->l_bp;                        /* Previous line    */
        lp3->l_fp = lp2;                        /* Link in      */
        lp2->l_fp = lp1;
        lp1->l_bp = lp2;
        lp2->l_bp = lp3;
        for ( i = 0; i < n; ++i )
            lputc(lp2, i, c);
        curwp->w_dotp = lp2;
        set_w_doto(curwp, n);

        return (TRUE);
    }
    doto = get_w_doto(curwp);                         /* Save for later.  */
    if ( get_lused(lp1) + n > get_lsize(lp1) ) {  /* Hard: reallocate */
        if ( ( lp2=lalloc( BSIZE(get_lused(lp1) + n) ) ) == NULL )
            return (FALSE);

        set_lused(lp2, get_lused(lp1) + n);
        cp1 = lgetcp(lp1, 0);
        cp2 = lgetcp(lp2, 0);
        while ( cp1 < lgetcp(lp1, doto) )
            *cp2++ = *cp1++;
        cp2 += n;
        while ( cp1 < lgetcp(lp1, get_lused(lp1)) )
            *cp2++ = *cp1++;
        lp1->l_bp->l_fp = lp2;
        lp2->l_fp = lp1->l_fp;
        lp1->l_fp->l_bp = lp2;
        lp2->l_bp = lp1->l_bp;
        free( (char *) lp1 );
    } else {                                    /* Easy: in place   */
        lp2 = lp1;                              /* Pretend new line */
        set_lused(lp2, get_lused(lp2) + n);
        cp2 = lgetcp(lp1, get_lused(lp1));
        cp1 = cp2-n;
        while ( cp1 > lgetcp(lp1, doto) )
            *--cp2 = *--cp1;
    }
    for ( i = 0; i < n; ++i )                   /* Add the characters   */
        lputc(lp2, doto + i, c);
    /* in all screens.... */
    scrp = first_screen;
    while ( scrp ) {

        wp = scrp->s_first_window;
        while ( wp != NULL ) {
            if ( wp->w_linep == lp1 )
                wp->w_linep = lp2;
            if ( wp->w_dotp == lp1 ) {
                wp->w_dotp = lp2;
                if ( wp==curwp || get_w_doto(wp) > doto )
                    set_w_doto(wp, get_w_doto(wp) + n);
            }
            for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                if ( wp->w_markp[cmark] == lp1 ) {
                    wp->w_markp[cmark] = lp2;
                    if ( wp->w_marko[cmark] > doto )
                        wp->w_marko[cmark] += n;
                }
            }
            wp = wp->w_wndp;
        }

        /* next screen! */
        scrp = scrp->s_next_screen;
    }

    return (TRUE);
}

/* LOWRITE:
 *
 * Overwrite a character into the current line at the current position
 */
int PASCAL NEAR lowrite P1_(char, c)
/* c: character to overwrite on current position  */
{
    if ( get_w_doto(curwp) < get_lused(curwp->w_dotp) &&
         ( (lgetc(curwp->w_dotp, get_w_doto(curwp)) != '\t' || tabsize == 0) ||
           (get_w_doto(curwp)) % tabsize == tabsize -1 ) )
        ldelete(1L, FALSE);

    return ( linsert(1, c) );
}

/* lover -- Overwrite a string at the current point
 */
int PASCAL NEAR lover P1_(char *, ostr)
{
    REGISTER int status = TRUE;

    if ( ostr != NULL )
        while ( *ostr && status == TRUE ) {
            status = ( (*ostr == '\r') ? lnewline(): lowrite(*ostr) );

            /* Insertion error? */
            if ( status != TRUE ) {
                mlwrite(TEXT172);
/*                                      "%%Out of memory while overwriting" */
                break;
            }
            ostr++;
        }

    return (status);
}

/* LNEWLINE:
 *
 * Insert a newline into the buffer at the current location of dot in the
 * current window. The funny ass-backwards way it does things is not a botch;
 * it just makes the last line in the file not a special case. Return TRUE if
 * everything works out and FALSE on error (memory allocation failure). The
 * update of dot and mark is a bit easier then in the above case, because the
 * split forces more updating.
 */
int PASCAL NEAR lnewline P0_()
{
    REGISTER char     *cp1  = NULL;
    REGISTER char     *cp2  = NULL;
    REGISTER LINE     *lp1  = NULL;
    REGISTER LINE     *lp2  = NULL;
    REGISTER int      doto  = 0;
    REGISTER EWINDOW  *wp   = NULL;
    SCREEN_T          *scrp = NULL;     /* screen to fix pointers in  */
    int               cmark = 0;        /* current mark               */

    if ( curbp->b_mode&MDVIEW )         /* don't allow this command if  */
        return ( rdonly() );            /* we are in read only mode     */

    /* remember we did this! */
    obj.obj_char = 13;
    undo_insert(OP_INSC, 1L, obj);

    lchange(WFHARD);
    lp1  = curwp->w_dotp;                       /* Get the address and  */
    doto = get_w_doto(curwp);                   /* offset of "."        */
    if ( ( lp2=lalloc(doto) ) == NULL )         /* New first half line  */
        return (FALSE);

    cp1 = lgetcp(lp1, 0);                       /* Shuffle text around  */
    cp2 = lgetcp(lp2, 0);
    while ( cp1 < lgetcp(lp1, doto) )
        *cp2++ = *cp1++;
    cp2 = lgetcp(lp1, 0);
    while ( cp1 < lgetcp(lp1, get_lused(lp1)) )
        *cp2++ = *cp1++;
    set_lused(lp1, get_lused(lp1) - doto);
    lp2->l_bp = lp1->l_bp;
    lp1->l_bp = lp2;
    lp2->l_bp->l_fp = lp2;
    lp2->l_fp = lp1;

    /* in all screens.... */
    scrp = first_screen;
    while ( scrp ) {

        wp = scrp->s_first_window;
        while ( wp != NULL ) {
            if ( wp->w_linep == lp1 )
                wp->w_linep = lp2;
            if ( wp->w_dotp == lp1 ) {
                if ( get_w_doto(wp) < doto )
                    wp->w_dotp = lp2;
                else
                    set_w_doto(wp, get_w_doto(wp) - doto);
            }
            for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                if ( wp->w_markp[cmark] == lp1 ) {
                    if ( wp->w_marko[cmark] < doto )
                        wp->w_markp[cmark] = lp2;
                    else
                        wp->w_marko[cmark] -= doto;
                }
            }
            wp = wp->w_wndp;
        }

        /* next screen! */
        scrp = scrp->s_next_screen;
    }

    return (TRUE);
}

/* LDELETE:
 *
 * This function deletes "n" bytes, starting at dot. Positive n deletes
 * forward, negative n deletes backwords. It understands how to deal
 * with end of lines, and with two byte characters. It returns TRUE if
 * all of the characters were deleted, and FALSE if they were not
 * (because dot ran into the buffer end). The "kflag" is TRUE if the
 * text should be put in the kill buffer.
 */
int PASCAL NEAR ldelete P2_(long, n, int, kflag)
/* n:     # of chars to delete                */
/* kflag: put killed text in kill buffer flag */
{
    REGISTER char     *cp1  = NULL;
    REGISTER char     *cp2  = NULL;
    REGISTER LINE     *dotp = NULL;
    REGISTER int      doto  = 0;
    REGISTER int      chunk = 0;
    REGISTER EWINDOW  *wp   = NULL;
    REGISTER int      cmark = 0;  /* current mark */

    if ( curbp->b_mode&MDVIEW )   /* don't allow this command if  */
        return ( rdonly() );      /* we are in read only mode     */

    /* going Forward? */
    if ( n >= 0 ) {

        while ( n > 0 ) {
#if     DBCS
            /* never start forward on a 2 byte char */
            if ( get_w_doto(curwp) > 0 &&
                 is2byte(lgetcp(curwp->w_dotp,                 0),
                         lgetcp(curwp->w_dotp, get_w_doto(curwp) - 1)) ) {
                set_w_doto(curwp, get_w_doto(curwp) - 1);
                n++;
            }
#endif
            /* record the current point */
            dotp = curwp->w_dotp;
            doto = get_w_doto(curwp);

#if REPAIR_CODE_LINE
            REPAIR(doto <= get_lused(dotp), doto = get_lused(dotp));
#else
            ASRT  (doto <= get_lused(dotp));
#endif

            /* can't delete past the end of the buffer */
            if ( dotp == curbp->b_linep )
                return (FALSE);

            /* find out how many chars to delete on this line */
            chunk = get_lused(dotp) - doto;   /* Size of chunk.   */
            if ( chunk > n )
                chunk = n;

            /* if at the end of a line, merge with the next */
            if ( chunk == 0 ) {

                /* flag that we are making a hard change */
                lchange(WFHARD);
                if ( ldelnewline() == FALSE ||
                     (kflag != FALSE &&kinsert(FORWARD, '\r')==FALSE) )
                    return (FALSE);

                --n;
                continue;
            }

            /* flag the fact we are changing the current line */
            lchange(WFEDIT);

            /* find the limits of the kill */
            cp1 = lgetcp(dotp, doto);
            cp2 = cp1 + chunk;
#if     DBCS
            /* never leave half a character */
            if ( is2byte(lgetcp(dotp, 0), cp2 - 1) ) {
                ++chunk;
                ++cp2;
                ++n;
            }
#endif

            /* save deleted characters for an undo... */
            if ( undoflag == TRUE ) {
                obj.obj_sptr = cp1;
                undo_insert(OP_DSTR, (long)chunk, obj);
            }

            /* save the text to the kill buffer */
            if ( kflag != FALSE ) {
                while ( cp1 != cp2 ) {
                    if ( kinsert(FORWARD, *cp1) == FALSE )
                        return (FALSE);

                    ++cp1;
                }
                cp1 = lgetcp(dotp, doto);
            }

            /* copy what is left of the line upward */
            while ( cp2 < lgetcp(dotp, get_lused(dotp)) )
                *cp1++ = *cp2++;
            /* See `ASRT(0 <= get_lused(dotp) - doto)' above. */
            set_lused(dotp, get_lused(dotp) - chunk);

            /* fix any other windows with the same text displayed */
            wp = wheadp;
            while ( wp != NULL ) {

                /* reset the dot if needed */
                if ( wp->w_dotp==dotp && get_w_doto(wp) >= doto ) {
                    set_w_doto(wp, MAX2(get_w_doto(wp) - chunk, doto));
                }

                /* reset any marks if needed */
                for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                    if ( wp->w_markp[cmark]==dotp &&
                         wp->w_marko[cmark]>=doto ) {
                        wp->w_marko[cmark] -= chunk;
                        if ( wp->w_marko[cmark] < doto )
                            wp->w_marko[cmark] = doto;
                    }
                }

                /* onward to the next window */
                wp = wp->w_wndp;
            }

            /* indicate we have deleted chunk characters */
            n -= chunk;
        }
    } else {
        while ( n < 0 ) {
#if     DBCS
            /* never start backwards on the 1st of a 2 byte character */
            if ( get_w_doto(curwp) > 1 &&
                 is2byte(lgetcp(curwp->w_dotp,                 0),
                         lgetcp(curwp->w_dotp, get_w_doto(curwp) - 1)) ) {
                set_w_doto(curwp, get_w_doto(curwp) + 1);
                n--;
            }
#endif
            /* record the current point */
            dotp = curwp->w_dotp;
            doto = get_w_doto(curwp);

            /* can't delete past the beginning of the buffer */
            if ( dotp == lforw(curbp->b_linep) && (doto == 0) )
                return (FALSE);

            /* find out how many chars to delete on this line */
            chunk = doto;                       /* Size of chunk.   */
            if ( chunk > -n )
                chunk = -n;

            /* if at the beginning of a line, merge with the last */
            if ( chunk == 0 ) {

                /* flag that we are making a hard change */
                lchange(WFHARD);
                backchar(TRUE, 1);
                if ( ldelnewline() == FALSE ||
                     (kflag != FALSE &&kinsert(REVERSE, '\r')==FALSE) )
                    return (FALSE);

                ++n;
                continue;
            }

            /* flag the fact we are changing the current line */
            lchange(WFEDIT);

            /* find the limits of the kill */
            cp1 = lgetcp(dotp, doto);
            cp2 = cp1 - chunk;
#if     DBCS
            if ( is2byte(lgetcp(dotp, 0), cp2 - 1) ) {
                ++chunk;
                --cp2;
                ++n;
            }
#endif

            /* save deleted characters for an undo... */
            if ( undoflag == TRUE ) {
                set_w_doto(curwp, get_w_doto(curwp) - chunk);
                obj.obj_sptr = cp2;
                undo_insert(OP_DSTR, (long)chunk, obj);
                set_w_doto(curwp, get_w_doto(curwp) + chunk);
            }

            /* save the text to the kill buffer */
            if ( kflag != FALSE ) {
                while ( cp1 > cp2 ) {
                    if ( kinsert( REVERSE, *(--cp1) ) == FALSE )
                        return (FALSE);
                }
                cp1 = lgetcp(dotp, doto);
            }

            /* copy what is left of the line downward */
            while ( cp1 < lgetcp(dotp, get_lused(dotp)) )
                *cp2++ = *cp1++;
            set_lused(dotp, get_lused(dotp) - chunk);
            set_w_doto(curwp, get_w_doto(curwp) - chunk);

            /* fix any other windows with the same text displayed */
            wp = wheadp;
            while ( wp != NULL ) {

                /* reset the dot if needed */
                if ( wp->w_dotp==dotp && get_w_doto(wp) >= doto ) {
                    set_w_doto(wp, get_w_doto(wp) - chunk);
                    if ( get_w_doto(wp) < doto )
                        set_w_doto(wp, doto);
                }

                /* reset any marks if needed */
                for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                    if ( wp->w_markp[cmark]==dotp &&
                         wp->w_marko[cmark]>=doto ) {
                        wp->w_marko[cmark] -= chunk;
                        if ( wp->w_marko[cmark] < doto )
                            wp->w_marko[cmark] = doto;
                    }
                }

                /* onward to the next window */
                wp = wp->w_wndp;
            }

            /* indicate we have deleted chunk characters */
            n += chunk;
        }
    }

    return (TRUE);
}

/* GETCTEXT:
 *
 * grab and return a string with the text of the current line
 */
char *PASCAL NEAR getctext P1_(char *, rline)
{
    REGISTER LINE *lp   = NULL;   /* line to copy                       */
    REGISTER int  size  = 0;      /* length of line to return           */
    REGISTER char *sp   = NULL;   /* string pointer into line           */
    REGISTER char *dp   = NULL;   /* string pointer into returned line  */

    /* find the contents of the current line and its length */
    lp = curwp->w_dotp;
    sp = ltext(lp);
    size = get_lused(lp);
    if ( size >= NSTRING )
        size = NSTRING - 1;

    /* copy it across */
    dp = rline;
    while ( size-- )
        *dp++ = *sp++;
    *dp = 0;

    return (rline);
}

/* PUTCTEXT:
 * replace the current line with the passed in text
 */
int PASCAL NEAR putctext P1_(char *, iline/* contents of new line */)
{
    REGISTER int  status  = 0;

    /* delete the current line */
    set_w_doto(curwp, 0);   /* starting at the beginning of the line */
    if ( ( status = killtext(TRUE, 1) ) != TRUE )
        return (status);

    /* insert the new line */
    if ( ( status = linstr(iline) ) != TRUE )
        return (status);

    status = lnewline();
    backline(TRUE, 1);

    return (status);
}

/* LDELNEWLINE:
 *
 * Delete a newline. Join the current line with the next line. If the next line
 * is the magic header line always return TRUE; merging the last line with the
 * header line can be thought of as always being a successful operation, even if
 * nothing is done, and this makes the kill buffer work "right". Easy cases can
 * be done by shuffling data around. Hard cases require that lines be moved
 * about in memory. Return FALSE on error and TRUE if all looks ok. Called by
 * "ldelete" only.
 */
int PASCAL NEAR ldelnewline P0_()
{
    REGISTER char     *cp1  = NULL;
    REGISTER char     *cp2  = NULL;
    REGISTER LINE     *lp1  = NULL;
    REGISTER LINE     *lp2  = NULL;
    REGISTER LINE     *lp3  = NULL;
    REGISTER EWINDOW  *wp   = NULL;
    SCREEN_T          *scrp = NULL; /* screen to fix pointers in  */
    int               cmark = 0;    /* current mark               */

    if ( curbp->b_mode&MDVIEW )     /* don't allow this command if  */
        return ( rdonly() );        /* we are in read only mode     */

    /* remember we did this! */
    obj.obj_char = 13;
    undo_insert(OP_DELC, 1L, obj);

    lp1 = curwp->w_dotp;
    lp2 = lp1->l_fp;
    if ( lp2 == curbp->b_linep ) {              /* At the buffer end.   */
        if ( get_lused(lp1) == 0 )              /* Blank line.          */
            lfree(lp1);

        return (TRUE);
    }
    if ( get_lused(lp2) <= get_lsize(lp1) - get_lused(lp1) ) {
        cp1 = lgetcp(lp1, get_lused(lp1));
        cp2 = lgetcp(lp2, 0);
        while ( cp2 < lgetcp(lp2, get_lused(lp2)) )
            *cp1++ = *cp2++;

        /* in all screens.... */
        scrp = first_screen;
        while ( scrp ) {

            wp = scrp->s_first_window;
            while ( wp != NULL ) {
                if ( wp->w_linep == lp2 )
                    wp->w_linep = lp1;
                if ( wp->w_dotp == lp2 ) {
                    wp->w_dotp  = lp1;
                    set_w_doto(wp, get_w_doto(wp) + get_lused(lp1));
                }
                for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                    if ( wp->w_markp[cmark] == lp2 ) {
                        wp->w_markp[cmark]  = lp1;
                        wp->w_marko[cmark] += get_lused(lp1);
                    }
                }
                wp = wp->w_wndp;
            }

            /* next screen! */
            scrp = scrp->s_next_screen;
        }

        set_lused(lp1, get_lused(lp1) + get_lused(lp2));
        lp1->l_fp = lp2->l_fp;
        lp2->l_fp->l_bp = lp1;
        free( (char *) lp2 );

        return (TRUE);
    }
    if ( ( lp3=lalloc(get_lused(lp1)+get_lused(lp2)) ) == NULL )
        return (FALSE);

    cp1 = lgetcp(lp1, 0);
    cp2 = lgetcp(lp3, 0);
    while ( cp1 < lgetcp(lp1, get_lused(lp1)) )
        *cp2++ = *cp1++;
    cp1 = lgetcp(lp2, 0);
    while ( cp1 < lgetcp(lp2, get_lused(lp2)) )
        *cp2++ = *cp1++;
    lp1->l_bp->l_fp = lp3;
    lp3->l_fp = lp2->l_fp;
    lp2->l_fp->l_bp = lp3;
    lp3->l_bp = lp1->l_bp;

    /* in all screens.... */
    scrp = first_screen;
    while ( scrp ) {

        wp = scrp->s_first_window;
        while ( wp != NULL ) {
            if ( wp->w_linep==lp1 || wp->w_linep==lp2 )
                wp->w_linep = lp3;
            if ( wp->w_dotp == lp1 )
                wp->w_dotp  = lp3;
            else if ( wp->w_dotp == lp2 ) {
                wp->w_dotp  = lp3;
                set_w_doto(wp, get_w_doto(wp) + get_lused(lp1));
            }
            for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                if ( wp->w_markp[cmark] == lp1 )
                    wp->w_markp[cmark]  = lp3;
                else if ( wp->w_markp[cmark] == lp2 ) {
                    wp->w_markp[cmark]  = lp3;
                    wp->w_marko[cmark] += get_lused(lp1);
                }
            }
            wp = wp->w_wndp;
        }

        /* next screen! */
        scrp = scrp->s_next_screen;
    }

    free( (char *) lp1 );
    free( (char *) lp2 );

    return (TRUE);
}

/* ADDLINE:
 *
 * Add a new line to the end of the indicated buffer.
 * - Return FALSE if we run out of memory.
 * - Note that this works on non-displayed buffers as well!
 */
int PASCAL NEAR addline P2_(BUFFER *,     bp,   /* buffer to add text to  */
                            CONST char *, text  /* line to add            */
                          )
{
    REGISTER LINE *lp   = NULL;
    REGISTER int  i     = 0;
    REGISTER int  ntext = 0;

    /* allocate the memory to hold the line */
    ntext = strlen(text);
    if ( ( lp=lalloc(ntext) ) == NULL )
        return (FALSE);

    /* copy the text into the new line */
    for ( i=0; i<ntext; ++i )
        lputc(lp, i, text[i]);

    /* add the new line to the end of the buffer */
    bp->b_linep->l_bp->l_fp = lp;
    lp->l_bp = bp->b_linep->l_bp;
    bp->b_linep->l_bp = lp;
    lp->l_fp = bp->b_linep;

    /* if the point was at the end of the buffer, move it to the beginning of
     * the new line */
    if ( bp->b_dotp == bp->b_linep )
        bp->b_dotp = lp;

    return (TRUE);
}

/* KDELETE:
 *
 * Delete all of the text saved in the kill buffer. Called by commands when a
 * new kill context is being created. The kill buffer array is released, just in
 * case the buffer has grown to immense size. No errors.
 */
VOID PASCAL NEAR kdelete P0_()
{
    KILL  *kp = NULL;   /* ptr to scan kill buffer chunk list */

    if ( kbufh[kill_index] != NULL ) {

        /* first, delete all the chunks */
        kbufp[kill_index] = kbufh[kill_index];
        while ( kbufp[kill_index] != NULL ) {
            kp = kbufp[kill_index]->d_next;
            free( (char *)kbufp[kill_index] );
            kbufp[kill_index] = kp;
#if     WINDOW_MSWIN
            {
                static int o = 0;
                if ( --o < 0 ) {
                    longop(TRUE);
                    o = 10;                         /* to lower overhead, only
                                                     * 10% calls to longop */
                }
            }
#endif
        }

        /* and reset all the kill buffer pointers */
        kbufh[kill_index] = kbufp[kill_index] = NULL;
        kskip[kill_index] = 0;
        kused[kill_index] = KBLOCK;
    }
}

/* NEXT_KILL:
 *
 * Advance to the next position in the kill ring, pushing the current
 * kill buffer and clearing what will be the new kill buffer
 */
VOID PASCAL NEAR next_kill P0_()
{
    /* advance to the next kill ring entry */
    kill_index++;
    if ( kill_index == NRING )
        kill_index = 0;

    /* and clear it, so it is ready for use */
    kdelete();
}

/* KINSERT:
 *
 * Insert a character to the kill buffer, allocating new chunks as needed.
 * Return TRUE if all is well, and FALSE on errors.
 */
int PASCAL NEAR kinsert P2_(int,  direct, /* direction (FORWARD/REVERSE) to insert characters */
                            char, c       /* character to insert in the kill buffer           */
                          )
{
    KILL  *nchunk = NULL;   /* ptr to newly roomed chunk */

    if ( direct == FORWARD ) {

        /* check to see if we need a new chunk */
        if ( kused[kill_index] >= KBLOCK ) {
            if ( ( nchunk = (KILL *)room( sizeof (KILL) ) ) == NULL ) {
                mlwrite(TEXT94);

/*                  "%%Out of memory" */
                return (FALSE);
            }
            if ( kbufh[kill_index] == NULL )            /* set head ptr if first
                                                         * time */
                kbufh[kill_index] = nchunk;
            if ( kbufp[kill_index] != NULL )            /* point the current to
                                                         * this new one */
                kbufp[kill_index]->d_next = nchunk;
            kbufp[kill_index] = nchunk;
            kbufp[kill_index]->d_next = NULL;
            kused[kill_index] = 0;
#if     WINDOW_MSWIN
            {
                static int o = 0;
                if ( --o < 0 ) {
                    longop(TRUE);
                    o = 10;                         /* to lower overhead, only
                                                     * 10% calls to longop */
                }
            }
#endif
        }

        /* and now insert the character */
        kbufp[kill_index]->d_chunk[kused[kill_index]++] = c;
    } else {
        /* REVERSE */
        /* check to see if we need a new chunk */
        if ( kskip[kill_index] == 0 ) {
            if ( ( nchunk = (KILL *)room( sizeof (KILL) ) ) == NULL ) {
                mlwrite(TEXT94);

/*                  "%%Out of memory" */
                return (FALSE);
            }
            if ( kbufh[kill_index] == NULL ) {                  /* set head ptr
                                                                 * if first time
                                                                 */
                kbufh[kill_index] = nchunk;
                kbufp[kill_index] = nchunk;
                kskip[kill_index] = KBLOCK;
                kused[kill_index] = KBLOCK;
                nchunk->d_next = (KILL *)NULL;
            } else {
                nchunk->d_next = kbufh[kill_index];
                kbufh[kill_index] = nchunk;
                kskip[kill_index] = KBLOCK;
            }
#if     WINDOW_MSWIN
            {
                static int o = 0;
                if ( --o < 0 ) {
                    longop(TRUE);
                    o = 10;                         /* to lower overhead, only
                                                     * 10% calls to longop */
                }
            }
#endif
        }

        /* and now insert the character */
        kbufh[kill_index]->d_chunk[--kskip[kill_index]] = c;
    }

    return (TRUE);
}

/* YANK:
 *
 * Yank text back from the kill buffer. This is really easy. All of the work is
 * done by the standard insert routines. All you do is run the loop, and check
 * for errors. Bound to "C-Y".
 */
#define Char_insert(a)  ( (a) == '\r' ? lnewline() : linsert(1, (a)) )

int PASCAL NEAR yank P2_(int, f, int, n)
/* f, n:  prefix flag and argument  */
{
    REGISTER int  counter   = 0;    /* counter into kill buffer data    */
    REGISTER char *sp       = NULL; /* pointer into string to insert    */
    short int     curoff    = 0;    /* storage for line before yanking  */
    LINE          *curline  = NULL;
    KILL          *kptr     = NULL; /* pointer into kill buffer */

    if ( curbp->b_mode&MDVIEW )   /* don't allow this command if  */
        return ( rdonly() );      /* we are in read only mode     */

    if ( n < 0 )
        return (FALSE);

    /* make sure there is something to yank */
    if ( kbufh[kill_index] == NULL ) {
        last_size = 0L;

        return (TRUE);                  /* not an error, just nothing */
    }

    /*
     * Save the local pointers to hold global ".".
     */
    if ( yankflag ) {
        /* Find the *previous* line, since the line we are on may disappear due
         * to re-allocation.  This works even if we are on the first line of the
         * file.
         */
        curline = lback(curwp->w_dotp);
        curoff = get_w_doto(curwp);
    }

    /* for each time.... */
    while ( n-- ) {
        last_size = 0L;
        if ( kskip[kill_index] > 0 ) {
            kptr = kbufh[kill_index];
            sp = &(kptr->d_chunk[kskip[kill_index]]);
            counter = kskip[kill_index];
            while ( counter++ < KBLOCK ) {
                Char_insert(*sp);
                last_size++;
                ++sp;
            }
            kptr = kptr->d_next;
        } else {
            kptr = kbufh[kill_index];
        }

        if ( kptr != (KILL *)NULL ) {
            while ( kptr != kbufp[kill_index] ) {
                sp = kptr->d_chunk;
                for ( counter = 0; counter < KBLOCK; counter++ ) {
                    Char_insert(*sp);
                    last_size++;
                    ++sp;
                }
                kptr = kptr->d_next;
            }
            counter = kused[kill_index];
            sp = kptr->d_chunk;
            while ( counter-- ) {
                Char_insert(*sp);
                last_size++;
                ++sp;
            }
        }
    }

    /* If requested, set global "." back to the beginning of the yanked text.
     */
    if ( yankflag ) {
        curwp->w_dotp = lforw(curline);
        set_w_doto(curwp, curoff);
    }
    thisflag |= CFYANK;

    return (TRUE);
}

int PASCAL NEAR cycle_ring P2_(int, f, int, n)
/* f, n:  prefix flag and argument  */
{
    REGISTER int  orig_index  = 0;  /* original kill_index */

    /* if there is an argument, cycle the kill index */
    if ( f ) {
        while ( n ) {
            orig_index = kill_index;
            do {
                kill_index--;
                if ( kill_index < 0 )
                    kill_index = NRING - 1;
            } while ( (orig_index != kill_index) &&
                      (kbufh[kill_index] == (KILL *)NULL) );
            n--;
        }
    }

    return TRUE;
}

int PASCAL NEAR yank_pop P2_(int, f, int, n)
/* f, n:  prefix flag and argument  */
{
    /* defaulted non first call will cycle by 1 */
    if ( (lastflag & CFYANK) && (f == FALSE) ) {
        f = TRUE;
        n = 1;
    }

    /* cycle the kill ring appropriately */
    cycle_ring(f, n);

    /* if not the first consectutive time, delete the last yank */
    if ( (lastflag & CFYANK) )
        ldelete(-last_size, FALSE);

    /* and insert the current kill buffer */
    return ( yank(FALSE, 1) );
}

int PASCAL NEAR clear_ring P2_(int, f, int, n)
/* f, n:  prefix flag and argument  */
{
    REGISTER int  index = 0;

    for ( index = 0; index < NRING; index++ )
        next_kill();
    mlwrite(TEXT228);

/*      "[Kill ring cleared]" */
    return (TRUE);
}

#if     0
dispkill P0_()
{
    KILL  *kptr   = NULL;
    int   index   = 0;
    char  *sp     = NULL;
    int   counter = 0;

    if ( kbufh[kill_index] == (KILL *)NULL ) {
        printf("<EMPTY>\n");

        return;
    }

    index = 1;
    if ( kskip[kill_index] > 0 ) {
        kptr = kbufh[kill_index];
        printf("kskip[kill_index] = %d\nBLOCK %d <", kskip[kill_index],
               index++);
        sp = &(kptr->d_chunk[kskip[kill_index]]);
        counter = kskip[kill_index];
        while ( counter++ < KBLOCK ) {
            putchar(*sp++);
        }
        printf(">\n");
        kptr = kptr->d_next;
    } else {
        kptr = kbufh[kill_index];
    }

    if ( kptr != (KILL *)NULL ) {
        while ( kptr != kbufp[kill_index] ) {
            printf("BLOCK %d <%255s>\n", index++, kptr->d_chunk);
            kptr = kptr->d_next;
        }
        printf("BLOCK %d <", index++);
        counter = kused[kill_index];
        sp = kptr->d_chunk;
        while ( counter-- ) {
            putchar(*sp++);
        }
        printf(">\nkused[kill_index] = %d\n", kused[kill_index]);
    }

}
#endif



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
