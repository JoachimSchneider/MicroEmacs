/*======================================================================
 *      UNDO.C:         Undo commands and functionality for
 *                      MicroEMACS
 *                      (C)Copyright 1995 by Daniel Lawrence
 *
 *      The functions in this file record and allow the playback
 *      of basic editing changes. For each buffer, a stack of these
 *      changes is maintained. The beginning of each command which
 *      can change the buffer is flaged with a command entry. The
 *      undo command then can back out of these changes one command
 *      at a time. The UNDO stack is flushed by any command that
 *      writes the buffer to disk, or clears the buffer.
 *====================================================================*/

/*====================================================================*/
#define UNDO_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/*==============================================================*/
/* Include files                                                */
/*==============================================================*/
#include "estruct.h"            /* Emacs definitions            */
#include "eproto.h"             /* Function definitions         */
#include "edef.h"               /* Global variable definitions  */
#include "elang.h"              /* Language definitions         */
/*==============================================================*/


/*==============================================================*/
/* Static function declarations                                 */
/*==============================================================*/
static int PASCAL NEAR  undolist DCL((void));
/*==============================================================*/


/* UNDO_INSERT:
 *
 * Insert an editing operation at the top of the current buffer's
 * undo stack.
 */
VOID undo_insert P3_(OPTYPE, op_type, long, count, OBJECT, op_erand)
/* op_type:   Type of operation being recorded  */
/* count:     Operand count                     */
/* op_erand:  The operand of the operation      */
{
    int undo_size;      /* size of the undo object */
    UNDO_OBJ *up;       /* ptr to a newly allocated undo object */

    /* don't bother if we are not keeping track! */
    if ( undoflag == FALSE )
        return;

    /* or don't do it if we are admist undoing something else */
    if ( undoing == TRUE )
        return;

    /* if it is a command object, and the last one pushed was one as well, don't
     * bother */
    if ( (curbp->undo_head != (UNDO_OBJ *)NULL) &&(op_type == OP_CMND) &&
         (curbp->undo_head->type == OP_CMND) )
        return;

    /* if it is a insert char, and the last one pushed was one as well, just
     * increment its count */
    if ( (curbp->undo_head != (UNDO_OBJ *)NULL) &&(op_type == OP_INSC) &&
         (curbp->undo_head->type == OP_INSC) ) {
        curbp->undo_head->count++;

        return;
    }

    /* get the correct size for this entry */
    undo_size = SIZEOF (UNDO_OBJ);
    if ( op_type == OP_DSTR )
        undo_size += (int)count;

    /* and allocate the memory */
    up = (UNDO_OBJ *)calloc(undo_size, 1);  /* Don't use room() here  */
    if ( up == (UNDO_OBJ *)NULL )
        return;

    umc_memset(up, 0, undo_size);

    /* update the buffer undo count */
    curwp->w_bufp->undo_count++;

    /* record the buffer position and undo object type */
    up->line_num = getlinenum(curbp, curwp->w_dotp);
    up->offset = get_w_doto(curwp);
    up->type = op_type;
    up->count = count;

    /* save the actual object... */
    switch ( op_type ) {

    case OP_CMND:
    case OP_ISTR:
    case OP_CPOS:
        break;

    case OP_DSTR:
        xstrncpy(up->undo_obj.obj_string, op_erand.obj_sptr, (int)count);
        up->undo_obj.obj_string[(int)count] = 0;
        break;

    case OP_REPC:
    case OP_DELC:
    case OP_INSC:
        up->undo_obj.obj_char = op_erand.obj_char;
        break;
    }

    /* and add it to the head of the current buffer's undo list */
    up->next = curbp->undo_head;
    curbp->undo_head = up;

    return;
}

/* UNDO_OP:
 *
 * Reverse the editing operation at the top of the current buffer's
 * undo stack.
 */
int undo_op P0_()
{
    OPTYPE op_type;             /* type of operation being recorded */
    UNDO_OBJ *up;               /* ptr to the undo object */
    int status;                 /* return status from undone operation */
    long count;                 /* repeat count for some operations */

    /* don't bother if we are not keeping track! */
    if ( undoflag == FALSE )
        return (FALSE);

    /* make sure we flag we are doing an undo.... [avoid recursion!] */
    undoing = TRUE;

    /* grab the operation to undo */
    up = curwp->w_bufp->undo_head;

    /* restore the buffer position */
    gotoline(TRUE, up->line_num);
    set_w_doto(curwp, up->offset);
    op_type = up->type;

    /* undo the actual operation */
    status = FALSE;
    switch ( op_type ) {

    case OP_CMND:
    case OP_CPOS:
        status = TRUE;
        break;

    case OP_REPC:
        ldelete(1L, FALSE);
    /* fall into the next case! */

    case OP_DELC:
        if ( up->undo_obj.obj_char == '\r' ) {
            count = up->count;
            while ( count-- )
                status = lnewline();
        } else
            status = linsert(up->count, up->undo_obj.obj_char);
        break;

    case OP_DSTR:
        status = linstr(up->undo_obj.obj_string);
        break;

    case OP_INSC:
    case OP_ISTR:
        status = ldelete(up->count, FALSE);
        break;
    }

    /* update the buffer undo count */
    curwp->w_bufp->undo_count--;

    /* and discard the undo entry */
    curwp->w_bufp->undo_head = up->next;
    CLROOM(up);
    undoing = FALSE;

    return (status);
}

/* UNDO_ZOT:
 *
 * Clear and deallocate the contents of a buffer's undo stack
 */
VOID undo_zot P1_(BUFFER *, bp)
{
    UNDO_OBJ *up;       /* current undo object being cleared */
    UNDO_OBJ *np;       /* next undo object to be cleared */

    bp->undo_count = 0L;
    np = bp->undo_head;
    while ( np != (UNDO_OBJ *)NULL ) {

        /* advance to the next undo object */
        up = np;
        np = up->next;

        /* and clear it */
        CLROOM(up);
    }

    /* and tell the buffer it's gone */
    bp->undo_head = (UNDO_OBJ *)NULL;
}

/* UNDO:
 *
 * Undo last done command
 */
int PASCAL NEAR undo P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    int status;         /* status return from undone operations */
    UNDO_OBJ *up;       /* ptr to the undo object */

    /* handle the default argument */
    if ( f == FALSE )
        n = 1;

    /* non-positive arguments loose */
    if ( n < 1 )
        return (FALSE);

    /* repeat a single undo for n iterations */
    status = TRUE;
    up = curwp->w_bufp->undo_head;
    while ( (status == TRUE) && (n > 0) && up != (UNDO_OBJ *)NULL ) {

        /* undo individual operations until the next statement boundry */
        do {
            status = undo_op();
            up = curwp->w_bufp->undo_head;
        } while ( (status == TRUE) &&(up != (UNDO_OBJ *)NULL) &&
                  (up->type != OP_CMND) );

        /* one less rep to do... */
        n--;
    }

    return (status);
}

/* UNDO_DELETE:
 *
 * Delete current buffer's undo stack
 */
int PASCAL NEAR undo_delete P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    /* well, do that..... and we know it will work.... */
    undo_zot(curbp);

    return (TRUE);
}

/* UNDO_LIST:
 *
 * Pop up a list of the current buffer's undo stack
 */
int PASCAL NEAR undo_list P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER int  status  = FALSE;  /* status return  */

    if ( ( status = undolist() ) != TRUE )  {
        return (status);
    }

    return ( wpopup(ulistp) );
}

/* UNDOLIST:
 */
static int PASCAL NEAR undolist P0_()
{
    /* scanning pointer into line to build:         */
    REGISTER char *cp1  = NULL;
    REGISTER char *cp2  = NULL;
    /* number of remaining undo elements in stack:  */
    long          count = 0;
    /* current undo object being listed:            */
    UNDO_OBJ      *up   = NULL;
    /* place to build ascii version of longs:       */
    char          b[C_8];
    /* text buffer to hold current line:            */
    char          line[NLINE];
    /* return status from subcommands:              */
    int           status  = 0;

    ZEROMEM(b);
    ZEROMEM(line);

    /* prepare and clear the buffer holding the undo list */
    ulistp->b_flag &= ~BFCHG;   /* Don't complain! */
    /* Blow old text away:  */
    if ( ( status = bclear(ulistp) ) != TRUE )  {
        return (status);
    }

    XSTRCPY(ulistp->b_fname, "");

    /* add in the header text */
    if ( addline(ulistp,
            "           Line/Pos  REP   Type  Data") == FALSE ||
         addline(ulistp,
            "           --------  ---   ----  ----") == FALSE    )  {
        return (FALSE);
    }

    /* scan through the undo stack, starting at the top! */
    up    = curwp->w_bufp->undo_head;
    count = curwp->w_bufp->undo_count;
    while ( up != NULL ) {
        /* Starting at the beginning of the line */
        cp1 = &line[0];

        /* add in the undo stack ordinal number */
        flong_asc(b, SIZEOF(b) - 1, count);
        cp2 = &b[0];
        while ( *cp2 )  {
            if ( !BNDCHK_PTR(cp1, line) ) {
                goto badend;
            }
            *cp1++ = *cp2++;
        }
        if ( !BNDCHK_PTR(cp1, line) ) {
            goto badend;
        }
        *cp1++ = ' ';

        /* and then the line number */
        flong_asc(b, SIZEOF(b) - 1, up->line_num);
        cp2 = &b[0];
        while ( *cp2 )  {
            if ( !BNDCHK_PTR(cp1, line) ) {
                goto badend;
            }
            *cp1++ = *cp2++;
        }
        if ( !BNDCHK_PTR(cp1, line) ) {
            goto badend;
        }
        *cp1++ = '/';

        /* and the offset into the line */
        xstrlcpy(b, int_asc(up->offset), SIZEOF(b));
        while ( STRLEN(b) < C_6 ) {
            xstrlcat(b, " ", SIZEOF(b));
        }
        cp2 = &b[0];
        while ( *cp2 )  {
            if ( !BNDCHK_PTR(cp1, line) ) {
                goto badend;
            }
            *cp1++ = *cp2++;
        }
        if ( !BNDCHK_PTR(cp1, line) ) {
            goto badend;
        }
        *cp1++ = ' ';

        /* and the count */
        xstrlcpy(b, int_asc(up->count), SIZEOF(b));
        while ( STRLEN(b) < C_3 ) {
            xstrlcat(b, " ", SIZEOF(b));
        }
        cp2 = &b[0];
        while ( *cp2 )  {
            if ( !BNDCHK_PTR(cp1, line) ) {
                goto badend;
            }
            *cp1++ = *cp2++;
        }
        if ( !BNDCHK_PTR(cp1, line) ) {
            goto badend;
        }
        *cp1++ = ' ';   /* Gap. */

        /* next, the undo type */
        switch ( up->type ) {

        case OP_CMND:
            if ( !BNDCHK_PTRN(cp1, line, SIZEOF("CMND  ")) )  {
                goto badend;
            }
            xstrcpy(cp1, "CMND  ");
            break;

        case OP_CPOS:
            if ( !BNDCHK_PTRN(cp1, line, SIZEOF("CPOS  ")) )  {
                goto badend;
            }
            xstrcpy(cp1, "CPOS  ");
            break;

        case OP_DELC:
            if ( !BNDCHK_PTRN(cp1, line, SIZEOF("DELC  ")) )  {
                goto badend;
            }
            xstrcpy(cp1, "DELC  ");
            getecnam(up->undo_obj.obj_char, cp1 + SLTLEN("DELC  "),
                     SIZEOF(line) - (cp1 - line) - SLTLEN("DELC  "));
            break;

        case OP_DSTR:
            if ( !BNDCHK_PTRN(cp1, line, SIZEOF("DSTR  ")) )  {
                goto badend;
            }
            xstrcpy(cp1, "DSTR  ");
            if ( !BNDCHK_PTR(cp1, line) )                     {
                goto badend;
            }
            xstrcat(cp1, "\"");
            if ( !BNDCHK_PTRN(cp1, line, C_40 + 1) )          {
                goto badend;
            }
            strncat(cp1, up->undo_obj.obj_string, C_40);
            if ( !BNDCHK_PTRN(cp1, line, C_3) )               {
                goto badend;
            }
            cp1[26] = '+';
            cp1[27] = 0;
            xstrcat(cp1, "\"");
            break;

        case OP_INSC:
            if ( !BNDCHK_PTRN(cp1, line, SIZEOF("INSC  ")) )  {
                goto badend;
            }
            xstrcpy(cp1, "INSC  ");
            getecnam(up->undo_obj.obj_char, cp1 + SLTLEN("INSC  "),
                     SIZEOF(line) - (cp1 - line) - SLTLEN("INSC  "));
            break;

        case OP_ISTR:
            if ( !BNDCHK_PTRN(cp1, line, SIZEOF("ISTR  ")) )  {
                goto badend;
            }
            xstrcpy(cp1, "ISTR  ");
            break;

        case OP_REPC:
            if ( !BNDCHK_PTRN(cp1, line, SIZEOF("REPC  ")) )  {
                goto badend;
            }
            xstrcpy(cp1, "REPC  ");
            getecnam(up->undo_obj.obj_char, cp1 + SLTLEN("REPC  "),
                     SIZEOF(line) - (cp1 - line) - SLTLEN("REPC  "));
            break;
        }

        /* terminate and add the built line into the buffer */
        if ( addline(ulistp, line) == FALSE ) {
            TRC(("undolist(): --- addline() == FALSE --- line = <%s>",
                 line));
            return (FALSE);
        } else                                {
            TRC(("undolist(): --- addline() == TRUE --- line = <%s>",
                 line));
        }

        /* on to the next undo! */
        count--;
        up = up->next;
    }

    goto goodend;
badend:
    TRC(("undolist(): --- *badend* --- line = <%s>", line));
    return (FALSE);

goodend:
    return (TRUE);
}

/* UNDO_DUMP:
 *
 * Clear ALL the undo stacks
 */
VOID undo_dump P0_()
{
    BUFFER *bp;

    /* clear all the buffer's undo stacks */
    bp = bheadp;
    while ( bp ) {
        undo_zot(bp);
        bp = bp->b_bufp;
    }
}


/**********************************************************************/
/* Memory (de-)allocation:                                            */
/**********************************************************************/

#define AIRBAG_SIZE_  ( 1 << 14 )
static char *AirBag_  = NULL;

/* ROOM:
 *
 * Allocate memory using malloc() on failure, discard oldest undo
 * information and retry. Memory region is initialized to zero.
 */
char *room P3_(int, nbytes, CONST char *, file, int, line)
/* nbytes:  Number of bytes to malloc() */
{
    char      *ptr  = NULL;   /* temporary pointer */
    BUFFER    *bp   = NULL;   /* buffer to dealloc memory from */
    UNDO_OBJ  *up   = NULL;   /* ptr to undo struct to free */
    UNDO_OBJ  *lp   = NULL;   /* last undo struct before up */

    static int  firstcall = 1;

    if ( firstcall )  {
        firstcall = 0;

        ASRT( NULL != (AirBag_ = (char *)malloc(AIRBAG_SIZE_)) );
    }

    ASRT(0 <= nbytes);
    if ( 0 >= nbytes ) return NULL;

    ptr = NULL;
    for ( ;; )  {
        /* attempt to allocate the memory */
        ptr = (char *)malloc(nbytes);
        if ( ptr != NULL )  {
            umc_memset(ptr, 0, nbytes);

            return ptr;
        }

        FREE_(AirBag_);
        TRCK(("%s", "room(): Malloc failed! Trying to release undo"), file, line);
        /* find the oldest visited buffer */
nextbuf:
        bp = getoldb();

        /* no buffers left to check? */
        if ( bp == NULL ) {
            TRCK(("%s", "room(): ERROR: Could not allocate memory"), file, line);

            return NULL;
        }

        /* any undo info to discard? */
        if ( bp->undo_count == 0 ) {
            bp->last_access = 0;

            goto nextbuf;
        }

        /* dump the last undo structure */
        lp = NULL;
        up = bp->undo_head;
        while ( up->next != NULL )  {
            lp = up;
            up = up->next;
        }

        /* dump the oldest undo */
        CLROOM(up);
        lp->next = NULL;
        bp->undo_count--;
    }

    return NULL;  /**AVOID_WARNING**/
}

/* RE-ROOM:
 *
 * Allocate memory using realloc() on failure, discard oldest undo
 * information and retry
 */
char *reroom P4_(VOIDP, orig_ptr, int, nbytes, CONST char *, file, int, line)
/* orig_ptr:  Pointer to re-allocate      */
/* nbytes:    Number of bytes to malloc() */
{
    char      *ptr  = NULL;   /* temporary pointer              */
    BUFFER    *bp   = NULL;   /* buffer to dealloc memory from  */
    UNDO_OBJ  *up   = NULL;   /* ptr to undo struct to free     */
    UNDO_OBJ  *lp   = NULL;   /* last undo struct before up     */

    ASRT(0 <= nbytes);
    if ( 0 >= nbytes ) return NULL;

    /*
     * Avoid the whole problem of non-ANSI realloc() functions that don't handle
     * NULL pointers correctly by calling malloc() (by way of room()) directly
     * if orig_ptr is NULL.
     */
    if ( orig_ptr == NULL ) {
        return ( room(nbytes, file, line) );
    }

    /* ptr == NULL  */
    for ( ;; )  {
        /* attempt to allocate the memory */
        ptr = (char *)realloc(orig_ptr, nbytes);
        if ( ptr != NULL )  {
            return ptr;
        }

        FREE_(AirBag_);
        TRCK(("%s", "reroom(): Malloc failed! Trying to release undo"), file, line);
        /* find the oldest visited buffer */
nxtbuf:
        bp = getoldb();

        /* no buffers left to check? */
        if ( bp == NULL ) {
            TRCK(("%s", "reroom(): ERROR: Could not allocate memory"), file, line);

            return NULL;
        }

        /* any undo info to discard? */
        if ( bp->undo_count == 0 ) {
            bp->last_access = 0;
            goto nxtbuf;
        }

        /* dump the last undo structure */
        lp = NULL;
        up = bp->undo_head;
        while ( up->next != NULL )  {
            lp = up;
            up = up->next;
        }

        /* dump the oldest undo */
        CLROOM(up);
        lp->next = NULL;
        bp->undo_count--;
    }

    return NULL;  /**AVOID_WARNING**/
}

/* DEROOM:
 *
 * Free the memory allocated by room/reroom.
 */
VOID deroom P3_(VOIDP, p, CONST char *, file, int, line)
{
    if ( NULL != p )  {
        free(p);
    }
}
/**********************************************************************/



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
