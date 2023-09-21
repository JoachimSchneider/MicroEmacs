/*======================================================================
 *  BUFFER.C:   buffer mgmt. routines
 *              MicroEMACS 4.00
 *
 * Buffer management.
 * Some of the functions are internal,
 * and some are actually attached to user
 * keys. Like everyone else, they set hints
 * for the display system.
 *====================================================================*/

/*====================================================================*/
#define BUFFER_C_
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

/* USEBUFFER:
 *
 * Attach a buffer to a window. The values of dot and mark come from the buffer
 * if the use count is 0. Otherwise, they come from some other window.
 */
int PASCAL NEAR usebuffer P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER BUFFER *bp;        /* temporary buffer pointer */

    /* get the buffer name to switch to */
    bp = getdefb();
    bp = getcbuf(TEXT24, bp ? bp->b_bname : mainbuf, TRUE);
/*              "Use buffer" */
    if ( !bp )
        return (ABORT);

    /* make it invisible if there is an argument */
    if ( f == TRUE )
        bp->b_flag |= BFINVS;

    /* switch to it in any case */
    return ( swbuffer(bp) );
}

/* NEXTBUFFER:
 *
 * Switch to the next buffer in the buffer list
 */
int PASCAL NEAR nextbuffer P2_(int, f, int, n)
/* f, n:  Default flag, numeric argument  */
{
    REGISTER BUFFER *bp;        /* current eligable buffer */
    REGISTER int status;

    /* make sure the arg is legit */
    if ( f == FALSE )
        n = 1;
    if ( n < 1 )
        return (FALSE);

    /* cycle thru buffers until n runs out */
    while ( n-- > 0 ) {
        bp = getdefb();
        if ( bp == NULL )
            return (FALSE);

        status = swbuffer(bp);
        if ( status != TRUE )
            return (status);
    }

    return (status);
}

/* SWBUFFER:
 *
 * Make buffer BP current
 */
int PASCAL NEAR swbuffer P1_(BUFFER *, bp)
{
    REGISTER EWINDOW *wp;
    SCREEN_T *scrp;             /* screen to fix pointers in */
    REGISTER int cmark;                 /* current mark */

    /* let a user macro get hold of things...if he wants */
    execkey(&exbhook, FALSE, 1);

    /* unuse the current buffer, saving window info to buffer struct */
    if ( --curbp->b_nwnd == 0 ) {               /* Last use.        */
        curbp->b_dotp  = curwp->w_dotp;
        set_b_doto(curbp, get_w_doto(curwp));
        for ( cmark = 0; cmark < NMARKS; cmark++ ) {
            curbp->b_markp[cmark] = curwp->w_markp[cmark];
            curbp->b_marko[cmark] = curwp->w_marko[cmark];
        }
        curbp->b_fcol  = curwp->w_fcol;
    }

    /* let time march forward! */
    access_time++;

    curbp = bp;                                 /* Switch.      */
    bp->last_access = access_time;
    if ( curbp->b_active != TRUE ) {            /* buffer not active yet*/
        /* read it in and activate it */
        readin( curbp->b_fname, ( (curbp->b_mode&MDVIEW) == 0 ) );
        curbp->b_dotp = lforw(curbp->b_linep);
        set_b_doto(curbp, 0);
        curbp->b_active = TRUE;
    }
    curwp->w_bufp  = bp;
    curwp->w_linep = bp->b_linep;               /* For macros, ignored. */
    curwp->w_flag |= WFMODE|WFFORCE|WFHARD;     /* Quite nasty.     */
    if ( bp->b_nwnd++ == 0 ) {                  /* First use.       */
        curwp->w_dotp  = bp->b_dotp;
        set_w_doto(curwp, get_b_doto(bp));
        for ( cmark = 0; cmark < NMARKS; cmark++ ) {
            curwp->w_markp[cmark] = bp->b_markp[cmark];
            curwp->w_marko[cmark] = bp->b_marko[cmark];
        }
        curwp->w_fcol  = bp->b_fcol;
    } else {
        /* in all screens.... */
        scrp = first_screen;
        while ( scrp ) {
            wp = scrp->s_first_window;
            while ( wp != NULL ) {
                if ( wp!=curwp && wp->w_bufp==bp ) {
                    curwp->w_dotp  = wp->w_dotp;
                    set_w_doto(curwp, get_w_doto(wp));
                    for ( cmark = 0; cmark < NMARKS; cmark++ ) {
                        curwp->w_markp[cmark] = wp->w_markp[cmark];
                        curwp->w_marko[cmark] = wp->w_marko[cmark];
                    }
                    curwp->w_fcol  = wp->w_fcol;
                    break;
                }
                /* next window */
                wp = wp->w_wndp;
            }

            /* next screen! */
            scrp = scrp->s_next_screen;
        }
    }

    /* let a user macro get hold of things...if he wants */
    execkey(&bufhook, FALSE, 1);

    return (TRUE);
}

/* KILLBUFFER:
 *
 * Dispose of a buffer, by name. Ask for the name. Look it up (don't get too
 * upset if it isn't there at all!). Get quite upset if the buffer is being
 * displayed. Clear the buffer (ask if the buffer has been changed). Then free
 * the header line and the buffer header. Bound to "C-X K".
 */
int PASCAL NEAR killbuffer P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER BUFFER *bp;        /* ptr to buffer to dump */

    /* get the buffer name to kill */
    bp = getdefb();
    bp = getcbuf(TEXT26, bp ? bp->b_bname : mainbuf, TRUE);
/*           "Kill buffer" */
    if ( bp == NULL )
        return (ABORT);

    return ( zotbuf(bp) );
}

/* POPBUFFER:
 *
 * Allow the user to pop up a buffer, like we do....
 */
int PASCAL NEAR popbuffer P2_(int, f, int, n)
/* f, n:  Default and numeric arguments */
{
    REGISTER BUFFER *bp;        /* ptr to buffer to dump */

    /* get the buffer name to pop */
    bp = getdefb();
    bp = getcbuf(TEXT27, bp ? bp->b_bname : mainbuf, TRUE);
/*           "Pop buffer" */
    if ( bp == NULL )
        return (ABORT);

    /* make it invisible if there is an argument */
    if ( f == TRUE )
        bp->b_flag |= BFINVS;

    return ( pop(bp) );
}

/* GETDEFB:
 *
 * Get the default buffer for a use or kill
 */
BUFFER *PASCAL NEAR getdefb P0_()
{
    BUFFER *bp;         /* default buffer */

    /* Find the next buffer, which will be the default */
    bp = curbp->b_bufp;

    /* cycle through the buffers to find an eligable one */
    while ( bp == NULL || bp->b_flag & BFINVS ) {
        if ( bp == NULL )
            bp = bheadp;
        else
            bp = bp->b_bufp;

        /* don't get caught in an infinite loop! */
        if ( bp == curbp ) {
            bp = NULL;
            break;
        }
    }

    return (bp);
}

/* ZOTBUF:
 *
 * Kill the buffer pointed to by bp
 */
int PASCAL NEAR zotbuf P1_(BUFFER *, bp)
{
    REGISTER BUFFER *bp1;
    REGISTER BUFFER *bp2;
    REGISTER int result;
    REGISTER PARG *tmp_arg;

    /* we can not kill a displayed buffer */
    if ( bp->b_nwnd != 0 ) {
        mlwrite(TEXT28);

/*          "Buffer is being displayed" */
        return (FALSE);
    }

    /* we can not kill an executing buffer */
    if ( bp->b_exec != 0 ) {
        mlwrite(TEXT226);

/*          "%%Can not delete an executing buffer" */
        return (FALSE);
    }

    /* dump any arguments */
    while ( bp->b_args ) {
        tmp_arg = bp->b_args;
        bp->b_args = bp->b_args->next;
        FREE(tmp_arg);
    }

    /* if anything is bound to the buffer, unbind them */
    unbind_buf(bp);

    /* dump it's undo stack */
    undo_zot(bp);

    if ( ( result = bclear(bp) ) != TRUE )      /* Blow text away.  */
        return (result);

    FREE(bp->b_linep);                          /* Release header line. */
    bp1 = NULL;                                 /* Find the header. */
    bp2 = bheadp;
    while ( bp2 != bp ) {
        bp1 = bp2;
        bp2 = bp2->b_bufp;
    }
    bp2 = bp2->b_bufp;                          /* Next one in chain.   */
    if ( bp1 == NULL )                          /* Unlink it.       */
        bheadp = bp2;
    else
        bp1->b_bufp = bp2;
    FREE(bp);                                   /* Release buffer block */

    return (TRUE);
}

/* NAMEBUFFER:
 *
 * Rename the current buffer
 */
int PASCAL NEAR namebuffer P2_(int, f, int, n)
/* f, n:  Default Flag & Numeric arg  */
{
    REGISTER BUFFER *bp;        /* pointer to scan through all buffers */
    char bufn[NBUFN];           /* buffer to hold buffer name */

    /* prompt for and get the new buffer name */
ask:    if ( mlreply(TEXT29, bufn, NBUFN) != TRUE )
/*          "Change buffer name to: " */
        return (FALSE);

    /* and check for duplicates */
    bp = bheadp;
    while ( bp != NULL ) {
        if ( bp != curbp ) {
            /* if the names the same */
            if ( strcmp(bufn, bp->b_bname) == 0 )
                goto ask;                  /* try again */
        }
        bp = bp->b_bufp;                /* onward */
    }

    XSTRCPY(curbp->b_bname, bufn);      /* copy buffer name to structure */
    upmode();                           /* make all mode lines replot */
    mlerase();

    return (TRUE);
}

/* LISTBUFFERS:
 *
 * Build and popup a buffer containing the list of all buffers. Bound to "C-X
 * C-B". A numeric argument forces it to list invisible buffers as well.
 */
int PASCAL NEAR listbuffers P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int status;        /* stutus return */

    if ( ( status = makelist(f) ) != TRUE )
        return (status);

    return ( wpopup(blistp) );
}

/* MAKELIST:
 *
 * This routine rebuilds the text in the special secret buffer that holds the
 * buffer list. It is called by the list buffers command. Return TRUE if
 * everything works. Return FALSE if there is an error (if there is no memory).
 * Iflag indicates whether to list hidden buffers.
 */
int PASCAL NEAR makelist P1_(int, iflag)
/* iflag: List hidden buffer flag */
{
    REGISTER char   *cp1;
    REGISTER char   *cp2;
    REGISTER BUFFER *bp;
    REGISTER LINE   *lp;
    REGISTER int s;
    REGISTER int i;
    long nbytes;                /* # of bytes in current buffer */
    char b[7+1];
    char line[128];

    blistp->b_flag &= ~BFCHG;                   /* Don't complain!  */
    if ( ( s=bclear(blistp) ) != TRUE )         /* Blow old text away   */
        return (s);

    XSTRCPY(blistp->b_fname, "");
    if ( addline(blistp, TEXT30) == FALSE
/*          "ACTN   Modes        Size Buffer       File" */
         ||  addline(blistp,
                     "---- ----------- ------- --------------- ----") == FALSE )
        return (FALSE);

    bp = bheadp;                                /* For all buffers  */

    /* build line to report global mode settings */
    cp1 = &line[0];
    *cp1++ = ' ';
    *cp1++ = ' ';
    *cp1++ = ' ';
    *cp1++ = ' ';
    *cp1++ = ' ';

    /* output the mode codes */
    for ( i = 0; i < NUMMODES; i++ )
        if ( gmode & (1 << i) )
            *cp1++ = modecode[i];
        else
            *cp1++ = '.';

    xstrcpy(cp1, TEXT31);
/*          "         Global Modes" */
    if ( addline(blistp, line) == FALSE )
        return (FALSE);

    /* output the list of buffers */
    while ( bp != NULL ) {
        /* skip invisible buffers if iflag is false */
        if ( ( (bp->b_flag&BFINVS) != 0 ) && (iflag != TRUE) ) {
            bp = bp->b_bufp;
            continue;
        }
        cp1 = &line[0];                         /* Start at left edge   */

        /* output status of ACTIVE flag (has the file been read in? */
        if ( bp->b_active == TRUE )             /* "@" if activated       */
            *cp1++ = '@';
        else
            *cp1++ = ' ';

        /* output status of changed flag */
        if ( (bp->b_flag&BFCHG) != 0 )          /* "*" if changed   */
            *cp1++ = '*';
        else
            *cp1++ = ' ';

        /* report if the file is truncated */
        if ( (bp->b_flag&BFTRUNC) != 0 )
            *cp1++ = '#';
        else
            *cp1++ = ' ';

        /* report if the file is narrowed */
        if ( (bp->b_flag&BFNAROW) != 0 )
            *cp1++ = '<';
        else
            *cp1++ = ' ';

        *cp1++ = ' ';           /* space */

        /* output the mode codes */
        for ( i = 0; i < NUMMODES; i++ ) {
            if ( bp->b_mode & (1 << i) )
                *cp1++ = modecode[i];
            else
                *cp1++ = '.';
        }
        *cp1++ = ' ';                           /* Gap.         */
        nbytes = 0L;                            /* Count bytes in buf.  */
        lp = lforw(bp->b_linep);
        while ( lp != bp->b_linep ) {
            nbytes += (long)get_lused(lp)+1L;
            lp = lforw(lp);
        }
        flong_asc(b, 7, nbytes);                /* 7 digit buffer size. */
        cp2 = &b[0];
        while ( *cp2 )
            *cp1++ = *cp2++;
        *cp1++ = ' ';                           /* Gap.         */
        cp2 = &bp->b_bname[0];                  /* Buffer name      */
        while ( *cp2 )
            *cp1++ = *cp2++;
        *cp1++ = ' ';                           /* Gap.         */
        cp2 = &bp->b_fname[0];                  /* File name        */
        if ( *cp2 != 0 ) {
            while ( cp1 < &line[41] )
                *cp1++ = ' ';
            while ( *cp2 )
                *cp1++ = *cp2++;
        }

        if ( dispundo ) {
            while ( cp1 - line < 50 )
                *cp1++ = ' ';
            flong_asc(b, 5, bp->last_access);
            cp2 = &b[0];
            while ( *cp2 )
                *cp1++ = *cp2++;
            flong_asc(b, 7, bp->undo_count);
            cp2 = &b[0];
            while ( *cp2 )
                *cp1++ = *cp2++;
        }

        *cp1 = 0;                               /* Add to the buffer.   */
        if ( addline(blistp, line) == FALSE )
            return (FALSE);

        bp = bp->b_bufp;
    }

    return (TRUE);                              /* All done         */
}

/* FLONG_ASC:
 *
 * Translate a long to ascii form. Don't trust various systems ltoa() routines..
 * they aren't consistant
 */
VOID PASCAL NEAR flong_asc P3_(char *, buf, int, width, long, num)
{
    buf[width] = 0;                             /* End of string.   */
    while ( num >= 10 ) {                       /* Conditional digits.  */
        buf[--width] = (int)(num%10L) + '0';
        num /= 10L;
    }
    buf[--width] = (int)num + '0';              /* Always 1 digit.  */
    while ( width != 0 )                        /* Pad with blanks. */
        buf[--width] = ' ';
}

/* ANYCB:
 *
 * Look through the list of buffers. Return TRUE if there are any changed
 * buffers. Buffers that hold magic internal stuff are not considered; who cares
 * if the list of buffer names is hacked. Return FALSE if no buffers have been
 * changed.
 */
int PASCAL NEAR anycb P0_()
{
    REGISTER BUFFER *bp;

    bp = bheadp;
    while ( bp != NULL ) {
        if ( (bp->b_flag&BFINVS)==0 && (bp->b_flag&BFCHG)!=0 )
            return (TRUE);

        bp = bp->b_bufp;
    }

    return (FALSE);
}

/* BFIND:
 *
 * Find a buffer, by name. Return a pointer to the BUFFER structure associated
 * with it. If the buffer is not found and the "cflag" is TRUE, create it. The
 * "bflag" is the settings for the flags in in buffer.
 */
BUFFER *PASCAL NEAR bfind P3_(CONST char *, bname, int, cflag, int, bflag)
/* bname: Name of buffer to find        */
/* cflag: Create it if not found?       */
/* bflag: Bit settings for a new buffer */
{
    REGISTER BUFFER *bp;
    REGISTER BUFFER *sb;        /* buffer to insert after */
    REGISTER LINE   *lp;
    int cmark;                  /* current mark */

    bp = bheadp;
    while ( bp != NULL ) {
        if ( strcmp(bname, bp->b_bname) == 0 )
            return (bp);

        bp = bp->b_bufp;
    }

    /* no such buffer exists, create it? */
    if ( cflag != FALSE ) {

        /* allocate the needed memory */
        if ( ( bp=(BUFFER *)room( SIZEOF (BUFFER) ) ) == NULL )
            return (NULL);

        if ( ( lp=lalloc(0) ) == NULL ) {
            FREE(bp);

            return (NULL);
        }

        /* find the place in the list to insert this buffer */
        if ( bheadp == NULL || strcmp(bheadp->b_bname, bname) > 0 ) {
            /* insert at the beginning */
            bp->b_bufp = bheadp;
            bheadp = bp;
        } else {
            sb = bheadp;
            while ( sb->b_bufp != NULL ) {
                if ( strcmp(sb->b_bufp->b_bname, bname) > 0 )
                    break;
                sb = sb->b_bufp;
            }

            /* and insert it */
            bp->b_bufp = sb->b_bufp;
            sb->b_bufp = bp;
        }

        /* and set up the other buffer fields */
        bp->b_topline = NULL;
        bp->b_botline = NULL;
        bp->undo_head = (UNDO_OBJ *)NULL;
        bp->undo_count = 0L;
        bp->last_access = access_time;
        bp->b_active = TRUE;
        bp->b_dotp  = lp;
        set_b_doto(bp, 0);
        for ( cmark = 0; cmark < NMARKS; cmark++ ) {
            bp->b_markp[cmark] = NULL;
            bp->b_marko[cmark] = 0;
        }
        bp->b_fcol  = 0;
        bp->b_flag  = bflag;
        bp->b_mode  = gmode;
        bp->b_nwnd  = 0;
        bp->b_exec  = 0;
        bp->b_linep = lp;
        XSTRCPY(bp->b_fname, "");
        XSTRCPY(bp->b_bname, bname);
#if     CRYPT
        bp->b_key[0] = 0;
#endif
        bp->b_numargs = NOTPROC;
        bp->b_args = (PARG *)NULL;
        lp->l_fp = lp;
        lp->l_bp = lp;
    }

    return (bp);
}

/* BCLEAR:
 *
 * This routine blows away all of the text in a buffer. If the buffer is marked
 * as changed then we ask if it is ok to blow it away; this is to save the user
 * the grief of losing text. The window chain is nearly always wrong if this
 * gets called; the caller must arrange for the updates that are required.
 * Return TRUE if everything looks good.
 */
int PASCAL NEAR bclear P1_(BUFFER *, bp)
{
    REGISTER LINE   *lp;
    REGISTER int s;
    int cmark;                  /* current mark */

    if ( (bp->b_flag&BFINVS) == 0               /* Not scratch buffer.  */
         && (bp->b_flag&BFCHG) != 0             /* Something changed    */
         && ( s=mlyesno(TEXT32) ) != TRUE )
/*            "Discard changes" */
        return (s);

    bp->b_flag  &= ~BFCHG;                      /* Not changed      */
    while ( ( lp=lforw(bp->b_linep) ) != bp->b_linep )
        lfree(lp);
    bp->b_dotp  = bp->b_linep;                  /* Fix "."      */
    set_b_doto(bp, 0);
    for ( cmark = 0; cmark < NMARKS; cmark++ ) {
        bp->b_markp[cmark] = NULL;          /* Invalidate "mark"    */
        bp->b_marko[cmark] = 0;
    }
    bp->b_fcol = 0;
    undo_zot(bp);       /* discard undo info for this buffer! */

    return (TRUE);
}

/* UNMARK:
 *
 * Unmark the current buffers change flag
 */
int PASCAL NEAR unmark P2_(int, f, int, n)
/* f, n:  Unused command arguments  */
{
    /* unmark the buffer */
    curbp->b_flag &= ~BFCHG;

    /* unmark all windows as well */
    upmode();

    return (TRUE);
}

/* GETOLDB:
 *
 * Get the most ancient visited buffer
 */
BUFFER *PASCAL NEAR getoldb P0_()
{
    BUFFER *bp;         /* current buffer */
    BUFFER *old_bp;     /* ptr to oldest buffer */
    long old_count;     /* oldest count */

    /* Find the next buffer, which will be the default */
    bp = bheadp;
    old_bp = (BUFFER *)NULL;
    old_count = 0;

    /* Scan the buffer list */
    while ( bp ) {

        /* if this one is older..... */
        if ( (bp->last_access > 0L) &&
             ( (old_bp == (BUFFER *)NULL) ||(bp->last_access < old_count) ) ) {

            /* record this as the oldest buffer */
            old_bp = bp;
            old_count = bp->last_access;
        }

        /* on to the next buffer in the list */
        bp = bp->b_bufp;
    }

    return (old_bp);
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
