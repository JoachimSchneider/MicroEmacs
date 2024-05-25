/*======================================================================
 *  EVAL.C: Expresion evaluation functions for
 *               MicroEMACS
 *
 *  written 1993 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#define EVAL_C_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


#include <stdio.h>
#include "estruct.h"
#include "eproto.h"
#include "edef.h"
#include "elang.h"
#include "evar.h"


#define RETURN  STATIC_STR_RET_RETURN


/* UV_INIT:
 *
 * Initialize the entries in one user variable table
 */
VOID PASCAL NEAR uv_init P1_(UTABLE *, ut)
/* ut:  User variable table to initialize */
{
    REGISTER int i;

    for ( i=0; i < ut->size; i++ ) {
        ut->uv[i].u_name[0] = 0;
        ut->uv[i].u_value = (char *)NULL;
    }
}

/* VARINIT:
 *
 * Initialize the global user variable table
 */
VOID PASCAL NEAR varinit P0_()
{
    /* allocate the global user variable table */
    uv_global = uv_head =
        (UTABLE *)ROOM( SIZEOF (UTABLE) + MAXVARS * SIZEOF (UVAR) );

    /* and set up its fields */
    uv_head->next = (UTABLE *)NULL;
    uv_head->size = MAXVARS;
    uv_head->bufp = (BUFFER *)NULL;
    uv_init(uv_head);
}

/* UV_CLEAN:
 *
 * Discard the contents of a user variable table
 */
VOID PASCAL NEAR uv_clean P1_(UTABLE *, ut)
/* ut:  Ptr to table to clear */
{
    REGISTER int i;

    /* now clear the entries in this one */
    for ( i=0; i < ut->size; i++ )
        if ( ut->uv[i].u_name[0] != 0 )
            CLROOM(ut->uv[i].u_value);
}

/* VARCLEAN:
 *
 * Discard and clear all user variable tables
 */
VOID PASCAL NEAR varclean P1_(UTABLE *, ut)
/* ut:  Table to clear  */
{
    /* first clean all the ones under this one */
    if ( ut->next != (UTABLE *)NULL )
        varclean(ut->next);

    /* clear the contents of this table */
    uv_clean(ut);

    /* and then deallocate the this table itself */
    CLROOM(ut);
}

/* GTFUN:
 *
 * Evaluate a function
 *
 * Function returns a static result string: Copy immediately!
 */
CONST char *PASCAL NEAR gtfun P1_(CONST char *, fname /* name of function to evaluate */)
{
    STATIC_STR_RET_PROLOG();

    char          *fnameL = NULL;
    REGISTER int  fnum    = 0;          /* index to function to eval  */
    REGISTER int  arg     = 0;          /* value of some arguments    */
    BUFFER        *bp     = NULL;       /* scratch buffer pointer     */
    char          arg1[NSTRING];
    char          arg2[NSTRING];
    char          arg3[NSTRING];
    char          result[2 * NSTRING];

    ZEROMEM(arg1);
    ZEROMEM(arg2);
    ZEROMEM(arg3);
    ZEROMEM(result);

    ASRT(NULL != fname);

    fnameL = xstrdup(fname);
    mklower(fnameL); /* and let it be upper or lower case */

    /* look the function up in the function table */
    fnum = binary(fnameL, funval, NELEM(funcs), MINFLEN);

    /* return errorm on a bad reference */
    if ( fnum == -1 ) {
        mlwrite(TEXT244, fnameL);
/*          "%%No such function as '%s'" */
        CLROOM(fnameL);

        RETURN ( errorm );
    }
    CLROOM(fnameL);

    /* if needed, retrieve the first argument */
    if ( funcs[fnum].f_type >= MONAMIC ) {
        if ( macarg(arg1) != TRUE )
            RETURN ( errorm );

        /* if needed, retrieve the second argument */
        if ( funcs[fnum].f_type >= DYNAMIC ) {
            if ( macarg(arg2) != TRUE )
                RETURN ( errorm );

            /* if needed, retrieve the third argument */
            if ( funcs[fnum].f_type >= TRINAMIC )
                if ( macarg(arg3) != TRUE )
                    RETURN ( errorm );

        }
    }

    /* and now evaluate it! */
    switch ( fnum ) {
    case UFABBREV:
        RETURN ( fixnull( ab_lookup(arg1) ) );

    case UFABS:
        RETURN ( int_asc( absv( asc_int(arg1) ) ) );

    case UFADD:
        RETURN ( int_asc( asc_int(arg1) + asc_int(arg2) ) );

    case UFAND:
        RETURN ( ltos( stol(arg1) && stol(arg2) ) );

    case UFASCII:
        RETURN ( int_asc( (int)arg1[0] ) );

    case UFBAND:
        RETURN ( int_asc( asc_int(arg1) & asc_int(arg2) ) );

    case UFBIND:
        RETURN ( transbind(arg1) );

    case UFBNOT:
        RETURN ( int_asc( ~asc_int(arg1) ) );

    case UFBOR:
        RETURN ( int_asc( asc_int(arg1) | asc_int(arg2) ) );

    case UFBXOR:
        RETURN ( int_asc( asc_int(arg1) ^ asc_int(arg2) ) );

    case UFCALL:                /* construct buffer name to execute */
        result[0] = '[';
        xstrlcpy(&result[1], arg1, SIZEOF(result) - 2);
        /* `-2' instead `-1' above to have room for ']' in any case */
        XSTRCAT(result, "]");

        /* find it, return ERROR if it does not exist */
        bp = bfind(result, FALSE, 0);
        if ( bp == NULL )
            RETURN ( errorm );

        /* execute it and return whats in the $rval */
        dobuf(bp);

        RETURN ( fixnull(rval) );

    case UFCAT:
        XSTRCPY(result, arg1);
        XSTRCAT(result, arg2);
        /***TODO: The original code did a `result[NSTRING - 1] = 0;' at
         ***      this place effectively cutting result to NSTRING - 1.
         ***      This is inconsistent with all other uses inside of
         ***      this function.
         ***/

        RETURN ( result );

    case UFCHR:
        result[0] = asc_int(arg1);
        result[1] = 0;

        RETURN ( result );

    case UFDIV:
        if ( ( arg = asc_int(arg2) ) != 0 )
            RETURN ( int_asc(asc_int(arg1) / arg) );
        else {
            mlwrite(TEXT245);

/*                      "%%Division by Zero is illegal" */
            RETURN ( errorm );
        }

    case UFENV:
#if     ENVFUNC
        RETURN ( fixnull( getenv(arg1) ) );
#else
        RETURN ( "" );
#endif

    case UFEQUAL:
        RETURN ( ltos( asc_int(arg1) == asc_int(arg2) ) );

    case UFEXIST:
        RETURN ( ltos( fexist(arg1) ) );

    case UFFIND:
        RETURN ( fixnull( flook(arg1, TRUE, TRUE) ) );

    case UFGREATER:
        RETURN ( ltos( asc_int(arg1) > asc_int(arg2) ) );

    case UFGROUP:
        arg = asc_int(arg1);
#if     MAGIC
        if ( arg < 0 || arg >= MAXGROUPS )
            RETURN ( bytecopy(result, errorm, NSTRING * 2 - 1) );

        RETURN ( bytecopy(result, fixnull(grpmatch[arg]), NSTRING * 2 - 1) );
#else
        if ( arg == 0 )
            bytecopy(result, patmatch, NSTRING * 2 - 1);
        else
            RETURN ( bytecopy(result, errorm, NSTRING * 2 - 1) );

        RETURN ( result );
#endif

    case UFGTCMD:
        RETURN ( cmdstr(getcmd(), result) );

    case UFGTKEY:
        result[0] = tgetc();
        result[1] = 0;

        RETURN ( result );

    case UFIND:
        RETURN ( XSTRCPY( result, fixnull( getval(arg1) ) ) );

    case UFISNUM:
        RETURN ( ltos( is_num(arg1) ) );

    case UFLEFT:
        /***TODO: OVERWRITE POSSIBLE***/
        RETURN ( bytecopy( result, arg1, asc_int(arg2) ) );

    case UFLENGTH:
        RETURN ( int_asc( STRLEN(arg1) ) );

    case UFLESS:
        RETURN ( ltos( asc_int(arg1) < asc_int(arg2) ) );

    case UFLOWER:
        RETURN ( mklower(arg1) );

    case UFMID:
        arg = asc_int(arg2);
        if ( arg > STRLEN(arg1) )
            RETURN ( XSTRCPY(result, "") );

        /***TODO: OVERWRITE POSSIBLE***/
        RETURN ( bytecopy( result, &arg1[arg-1], asc_int(arg3) ) );

    case UFMKCOL:
        if ( ( arg = asc_int(arg1) ) < 0 || arg >= NMARKS ||
             curwp->w_markp[arg] == NULL ) {
            mlwrite(TEXT11, arg);

            RETURN ( int_asc(-1) );
        }

        RETURN ( int_asc( findcol(curwp->w_markp[arg], curwp->w_marko[arg]) ) );

    case UFMKLINE:
        if ( ( arg = asc_int(arg1) ) < 0 || arg >= NMARKS ||
             curwp->w_markp[arg] == NULL ) {
            mlwrite(TEXT11, arg);

            RETURN ( int_asc(0) );
        }

        RETURN ( long_asc( getlinenum(curbp, curwp->w_markp[arg]) ) );

    case UFMOD:
        if ( ( arg = asc_int(arg2) ) != 0 )
            RETURN ( int_asc(asc_int(arg1) % arg) );
        else {
            mlwrite(TEXT245);

/*                      "%%Division by Zero is illegal" */
            RETURN ( errorm );
        }

    case UFNEG:
        RETURN ( int_asc( -asc_int(arg1) ) );

    case UFNOT:
        RETURN ( ltos(stol(arg1) == FALSE) );

    case UFOR:
        RETURN ( ltos( stol(arg1) || stol(arg2) ) );

    case UFREVERSE:
        RETURN ( strrev( bytecopy(result, arg1, NSTRING * 2 - 1) ) );

    case UFRIGHT:
        arg = asc_int(arg2);
        if ( arg > STRLEN(arg1) )
            arg = STRLEN(arg1);

        RETURN ( XSTRCPY(result, &arg1[STRLEN(arg1) - arg]) );

    case UFRND:
        RETURN ( int_asc( (int)( ernd() % (long)absv( asc_int(arg1) ) ) +
                          1L ) );

    case UFSEQUAL:
        RETURN ( ltos(strcmp(arg1, arg2) == 0) );

    case UFSGREAT:
        RETURN ( ltos(strcmp(arg1, arg2) > 0) );

    case UFSINDEX:
        RETURN ( int_asc( sindex(arg1, arg2) ) );

    case UFSLESS:
        RETURN ( ltos(strcmp(arg1, arg2) < 0) );

    case UFSLOWER:
        RETURN ( (setlower(arg1, arg2), "") );

    case UFSUB:
        RETURN ( int_asc( asc_int(arg1) - asc_int(arg2) ) );

    case UFSUPPER:
        RETURN ( (setupper(arg1, arg2), "") );

    case UFTIMES:
        RETURN ( int_asc( asc_int(arg1) * asc_int(arg2) ) );

    case UFTRIM:
        RETURN ( trimstr(arg1) );

    case UFTRUTH:
        RETURN ( ltos(asc_int(arg1) == 42) );

    case UFUPPER:
        RETURN ( mkupper(arg1) );

    case UFXLATE:
        RETURN ( xlat(arg1, arg2, arg3) );
    }

    meexit(-11);        /* never should get here */


    STATIC_STR_RET_EPILOG(gtfun, CONST char *, 2 * NSTRING);
}

/* GTUSR:
 *
 * Look up a user var's value
 */
CONST char *PASCAL NEAR gtusr P1_(CONST char *, vname)
/* vname: Name of user variable to fetch  */
{
    char            *vnameL =  NULL;
    REGISTER int    vnum    = 0;      /* ordinal number of user var */
    REGISTER char   *vptr   = NULL;   /* temp pointer to function value */
    REGISTER UTABLE *ut     = NULL;   /* ptr to the current variable table */

    vnameL  = xstrdup(vname);
    /* limit comparisons to significant length */
    if ( STRLEN(vnameL) >= NVSIZE )     /* "%" counts, but is not passed */
        vnameL[NVSIZE] = '\0';

    /* scan through each user variable table starting with the most local and
     * going to the global table */
    ut = uv_head;
    while ( ut ) {
        /* scan this table looking for the user var name */
        for ( vnum = 0; vnum < ut->size; vnum++ ) {
            /* out of entries? */
            if ( ut->uv[vnum].u_name[0] == 0 )
                goto next_ut;

            /* is this the one? */
            if ( strcmp(vnameL, ut->uv[vnum].u_name) == 0 ) {
                CLROOM(vnameL);
                /* return its value..... */
                vptr = ut->uv[vnum].u_value;
                if ( vptr )
                    return (vptr);
                else
                    return (errorm);
            }
        }

next_ut:
        ut = ut->next;
    }

    /* return errorm if we run off the end */
    CLROOM(vnameL);

    return (errorm);
}

/* FUNVAL:
 */
CONST char *PASCAL NEAR funval P1_(int, i)
{
    return (funcs[i].f_name);
}

/* ENVVAL:
 */
CONST char *PASCAL NEAR envval P1_(int, i)
{
    return (envars[i]);
}

/* BINARY:
 */
int PASCAL NEAR binary P4_(CONST char *, key, ue_tvfetch_T, tval, int, tlength, int, klength)
/* key:     Key string to look for                    */
/* tval:    Ptr to function to fetch table value with */
/* tlength: Length of table to search                 */
/* klength: Maximum length of string to compare       */
{
    int l, u;           /* lower and upper limits of binary search */
    int i;              /* current search index */
    int cresult;        /* result of comparison */

    /* set current search limit as entire list */
    l = 0;
    u = tlength - 1;

    /* get the midpoint! */
    while ( u >= l ) {
        i = (l + u) >> 1;

        /* do the comparison */
        cresult = strncmp(key, (*tval)(i), klength);
        if ( cresult == 0 )
            return (i);

        if ( cresult < 0 )
            u = i - 1;
        else
            l = i + 1;
    }

    return (-1);
}

/* GTENV:
 *
 * Retrieve environmant variable
 *
 * Function returns a static result string: Copy immediately!
 */
CONST char *PASCAL NEAR gtenv P1_(CONST char *, vname)
/* vname: Name of environment variable to retrieve  */
{
    STATIC_STR_RET_PROLOG();

    REGISTER int  vnum  = 0;            /* ordinal number of var
                                         * referenced     */
    char          result[2 * NSTRING];  /* string result  */

    ZEROMEM(result);

    ASRT(NULL != vname);

    /* scan the list, looking for the referenced name */
    vnum = binary(vname, envval, NELEM(envars), NVSIZE);

    /* return errorm on a bad reference */
    if ( vnum == -1 )
        RETURN ( errorm );

    /* otherwise, fetch the appropriate value */
    switch ( vnum ) {
    case EVABBELL:
        RETURN ( ltos(ab_bell) );

    case EVABCAP:
        RETURN ( ltos(ab_cap) );

    case EVABFULL:
        RETURN ( ltos(ab_full) );

    case EVABQUICK:
        RETURN ( ltos(ab_quick) );

    case EVACOUNT:
        RETURN ( int_asc(gacount) );

    case EVASAVE:
        RETURN ( int_asc(gasave) );

    case EVBUFHOOK:
        RETURN ( fixnull(getfname(&bufhook)) );

    case EVCBFLAGS:
        RETURN ( int_asc(curbp->b_flag) );

    case EVCBUFNAME:
        RETURN ( curbp->b_bname );

    case EVCFNAME:
        RETURN ( curbp->b_fname );

    case EVCMDHK:
        RETURN ( fixnull( getfname(&cmdhook) ) );

    case EVCMODE:
        RETURN ( int_asc(curbp->b_mode) );

    case EVCQUOTE:
        RETURN ( int_asc(cquote) );

    case EVCURCHAR:
        RETURN ( get_lused(curwp->w_dotp) ==
                 get_w_doto(curwp) ? int_asc('\r') :int_asc( lgetc(curwp->w_dotp,
                                                               get_w_doto(curwp)) ) );

    case EVCURCOL:
        RETURN ( int_asc(getccol(FALSE)) );

    case EVCURLINE:
        RETURN ( long_asc(getlinenum(curbp, curwp->w_dotp)) );

    case EVCURWIDTH:
        RETURN ( int_asc(term.t_ncol) );

    case EVCURWIND:
        RETURN ( int_asc(getcwnum()) );

    case EVCWLINE:
        RETURN ( int_asc(getwpos()) );

    case EVDEBUG:
        RETURN ( ltos(macbug) );

    case EVDESKCLR:
        RETURN ( cname[deskcolor] );

    case EVDIAGFLAG:
        RETURN ( ltos(diagflag) );

    case EVDISCMD:
        RETURN ( ltos(discmd) );

    case EVDISINP:
        RETURN ( ltos(disinp) );

    case EVDISPHIGH:
        RETURN ( ltos(disphigh) );

    case EVDISPUNDO:
        RETURN ( ltos(dispundo) );

    case EVEXBHOOK:
        RETURN ( fixnull(getfname(&exbhook)) );

    case EVEXITHOOK:
        RETURN ( fixnull(getfname(&exithook)) );

    case EVFCOL:
        RETURN ( int_asc(curwp->w_fcol) );

    case EVFILLCOL:
        RETURN ( int_asc(fillcol) );

    case EVFLICKER:
        RETURN ( ltos(flickcode) );

    case EVFMTLEAD:
        RETURN ( fmtlead );

    case EVGFLAGS:
        RETURN ( int_asc(gflags) );

    case EVGMODE:
        RETURN ( int_asc(gmode) );

    case EVHARDTAB:
        RETURN ( int_asc(tabsize) );

    case EVHILITE:
        RETURN ( int_asc(hilite) );

    case EVHJUMP:
        RETURN ( int_asc(hjump) );

    case EVHSCRLBAR:
        RETURN ( ltos(hscrollbar) );

    case EVHSCROLL:
        RETURN ( ltos(hscroll) );

    case EVISTERM:
        RETURN ( cmdstr(isterm, result) );

    case EVKILL:
        RETURN ( getkill() );

    case EVLANG:
        RETURN ( LANGUAGE );

    case EVLASTKEY:
        RETURN ( int_asc(lastkey) );

    case EVLASTMESG:
        RETURN ( lastmesg );

    case EVLINE:
        RETURN ( getctext(result) );

    case EVLTERM:
        RETURN ( lterm );

    case EVLWIDTH:
        RETURN ( int_asc(get_lused(curwp->w_dotp)) );

    case EVMATCH:
        RETURN ( fixnull(patmatch) );

    case EVMMOVE:
        RETURN ( int_asc(mouse_move) );

    case EVMODEFLAG:
        RETURN ( ltos(modeflag) );

    case EVMSFLAG:
        RETURN ( ltos(mouseflag) );

    case EVNEWSCRN:
        RETURN ( ltos(newscreenflag) );

    case EVNUMWIND:
        RETURN ( int_asc( gettwnum() ) );

    case EVOQUOTE:
        RETURN ( int_asc(oquote) );

    case EVORGCOL:
        RETURN ( int_asc(term.t_colorg) );

    case EVORGROW:
        RETURN ( int_asc(term.t_roworg) );

    case EVOS:
        RETURN ( os );

    case EVOVERLAP:
        RETURN ( int_asc(overlap) );

    case EVPARINDENT:
        RETURN ( int_asc(parindent) );

    case EVPAGELEN:
        RETURN ( int_asc(term.t_nrow + 1) );

    case EVPALETTE:
        RETURN ( palstr );

    case EVPARALEAD:
        RETURN ( paralead );

    case EVPENDING:
#if     TYPEAH || WINDOW_MSWIN
        RETURN ( ltos(typahead()) );
#else
        RETURN ( falsem );
#endif

    case EVPOPFLAG:
        RETURN ( ltos(popflag) );

    case EVPOPWAIT:
        RETURN ( ltos(popwait) );

    case EVPOSFLAG:
        RETURN ( ltos(posflag) );

    case EVPROGNAME:
        RETURN ( PROGNAME );

    case EVRAM:
        RETURN ( int_asc((int)(envram / 1024l)) );

    case EVREADHK:
        RETURN ( fixnull(getfname(&readhook)) );

    case EVREGION:
        RETURN ( getreg(result) );

    case EVREPLACE:
        RETURN ( (char *)rpat );

    case EVRVAL:
        RETURN ( rval );

    case EVSCRNAME:
        RETURN ( first_screen->s_screen_name );

    case EVSEARCH:
        RETURN ( (char *)pat );

    case EVSEARCHPNT:
        RETURN ( int_asc(searchtype) );

    case EVSEED:
        RETURN ( int_asc((int)seed) );

    case EVSOFTTAB:
        RETURN ( int_asc(stabsize) );

    case EVSRES:
        RETURN ( sres );

    case EVSSAVE:
        RETURN ( ltos(ssave) );

    case EVSSCROLL:
        RETURN ( ltos(sscroll) );

    case EVSTATUS:
        RETURN ( ltos(cmdstatus) );

    case EVSTERM:
        RETURN ( cmdstr(sterm, result) );

    case EVTARGET:
        saveflag = lastflag;

        RETURN ( int_asc(curgoal) );

    case EVTIME:
        RETURN ( timeset() );

    case EVTIMEFLAG:
        RETURN ( ltos(timeflag) );

    case EVTPAUSE:
        RETURN ( int_asc(term.t_pause) );

    case EVUNDOFLAG:
        RETURN ( ltos(undoflag) );

    case EVVERSION:
        RETURN ( VERSION );

    case EVVSCRLBAR:
        RETURN ( ltos(vscrollbar) );

    case EVWCHARS:
        RETURN ( getwlist(result) );

    case EVWLINE:
        RETURN ( int_asc(curwp->w_ntrows) );

    case EVWRAPHK:
        RETURN ( fixnull( getfname(&wraphook) ) );

    case EVWRITEHK:
        RETURN ( fixnull( getfname(&writehook) ) );

    case EVXPOS:
        RETURN ( int_asc(xpos) );

    case EVYANKFLAG:
        RETURN ( ltos(yankflag) );

    case EVYPOS:
        RETURN ( int_asc(ypos) );
    }

    meexit(-12);        /* again, we should never get here */


    STATIC_STR_RET_EPILOG(gtenv, CONST char *, 2 * NSTRING);
}

/* FIXNULL:
 *
 * Don't return NULL pointers!
 */
CONST char *PASCAL NEAR fixnull P1_(CONST char *, s)
{
    if ( s == NULL )
        return ( (CONST char *)"" );
    else
        return (s);
}

/* GETKILL:
 *
 * Return some of the contents of the kill buffer
 *
 * Function returns a static result string: Copy immediately!
 */
char *PASCAL NEAR getkill P0_()
{
    STATIC_STR_RET_PROLOG();

    REGISTER int  size    = 0;    /* max num of chars left to return  */
    REGISTER char *sp     = NULL; /* ptr into KILL block data chunk   */
    REGISTER char *vp     = NULL; /* ptr into return value            */
    KILL          *kptr   = NULL; /* ptr to the current KILL block    */
    int           counter = 0;    /* index into data chunk            */
    char          value[NSTRING]; /* temp buffer for value            */

    ZEROMEM(value);

    /* no kill buffer....just a null string */
    if ( kbufh[kill_index] == (KILL *)NULL ) {
        value[0] = 0;

        RETURN ( value );
    }

    /* set up the output buffer */
    vp = value;
    size = NSTRING - 1;

    /* backed up characters? */
    if ( kskip[kill_index] > 0 ) {
        kptr = kbufh[kill_index];
        sp = &(kptr->d_chunk[kskip[kill_index]]);
        counter = kskip[kill_index];
        while ( counter++ < KBLOCK ) {
            *vp++ = *sp++;
            if ( --size == 0 ) {
                *vp = 0;

                RETURN ( value );
            }
        }
        kptr = kptr->d_next;
    } else {
        kptr = kbufh[kill_index];
    }

    if ( kptr != (KILL *)NULL ) {
        while ( kptr != kbufp[kill_index] ) {
            sp = kptr->d_chunk;
            for ( counter = 0; counter < KBLOCK; counter++ ) {
                *vp++ = *sp++;
                if ( --size == 0 ) {
                    *vp = 0;

                    RETURN ( value );
                }
            }
            kptr = kptr->d_next;
        }
        counter = kused[kill_index];
        sp = kptr->d_chunk;
        while ( counter-- ) {
            *vp++ = *sp++;
            if ( --size == 0 ) {
                *vp = 0;

                RETURN ( value );
            }
        }
    }

    /* and return the constructed value */
    *vp = 0;

    RETURN ( value );


    STATIC_STR_RET_EPILOG(getkill, char *, NSTRING);
}

/* TRIMSTR:
 *
 * Trim whitespace off the end of a string
 */
char *PASCAL NEAR trimstr P1_(char *, s)
/* s: String to trim  */
{
    char *sp;           /* backward index */

    sp = s + STRLEN(s) - 1;
    while ( (sp >= s) && (*sp == ' ' || *sp == '\t') )
        --sp;
    *(sp+1) = 0;

    return (s);
}

/* SETVAR:
 *
 * set a variable
 */
int PASCAL NEAR setvar P2_(int, f, int, n)
/* f: Default flag                              */
/* n: Numeric arg (can overide prompted value)  */
{
    REGISTER int status;        /* status return */
    VDESC vd;                   /* variable num/type */
    char var[NVSIZE+1];         /* name of variable to fetch */
    char value[NSTRING];        /* value to set variable to */

    /* first get the variable to set.. */
    if ( clexec == FALSE ) {
        status = mlreply(TEXT51, &var[0], NVSIZE+1);
/*               "Variable to set: " */
        if ( status != TRUE )
            return (status);
    } else {            /* macro line argument */
        /* grab token and skip it */
        execstr = token(execstr, var, NVSIZE + 1);
    }

    /* check the legality and find the var */
    findvar(var, &vd, NVSIZE + 1, VT_GLOBAL);

    /* if its not legal....bitch */
    if ( vd.v_type == -1 ) {
        mlwrite(TEXT52, var);

/*          "%%No such variable as '%s'" */
        return (FALSE);
    }

    /* get the value for that variable */
    if ( f == TRUE )
        XSTRCPY( value, int_asc(n) );
    else {
        status = mlreply(TEXT53, &value[0], NSTRING);
/*               "Value: " */
        if ( status == ABORT )
            return (status);
    }

    /* and set the appropriate value */
    status = svar(&vd, value);

    /* if $debug == TRUE, every assignment will echo a statment to that effect
     * here. */

    if ( macbug && (strcmp(var, "%track") != 0) ) {
        xstrcpy(outline, "(((");

        xstrcat(outline, var);
        xstrcat(outline, " <- ");

        /* and lastly the value we tried to assign */
        xstrcat(outline, value);
        xstrcat(outline, ")))");

        /* write out the debug line */
        mlforce(outline);
        update(TRUE);

        /* and get the keystroke to hold the output */
        if ( get_key() == abortc ) {
            mlforce(TEXT54);
/*              "[Macro aborted]" */
            status = FALSE;
        }
    }

    /* and return it */
    return (status);
}

/* GLOBAL_VAR:
 *
 * Declare a global variable
 */
int PASCAL NEAR global_var P2_(int, f, int, n)
/* f: Default flag                */
/* n: Numeric arg (ignored here)  */
{
    REGISTER int status;        /* status return */
    VDESC vd;                   /* variable num/type */
    char var[NVSIZE+1];         /* name of variable to fetch */

    /* first get the variable to set.. */
    if ( clexec == FALSE ) {
        status = mlreply(TEXT249, &var[0], NVSIZE+1);
/*               "Global variable to declare: " */
        if ( status != TRUE )
            return (status);
    } else {            /* macro line argument */
        /* grab token and skip it */
        execstr = token(execstr, var, NVSIZE + 1);
    }

    /* check the legality and find the var */
    findvar(var, &vd, NVSIZE + 1, VT_GLOBAL);

    /* if its not legal....bitch */
    if ( vd.v_type == -1 ) {
        mlwrite(TEXT52, var);

/*          "%%No such variable as '%s'" */
        return (FALSE);
    }

    /* and set the appropriate value */
    status = svar(&vd, "");

    /* if $debug == TRUE, every assignment will echo a statment to that effect
     * here. */

    if ( macbug && (strcmp(var, "%track") != 0) ) {
        xstrcpy(outline, "(((Globally declare ");

        xstrcat(outline, var);
        xstrcat(outline, ")))");

        /* write out the debug line */
        mlforce(outline);
        update(TRUE);

        /* and get the keystroke to hold the output */
        if ( get_key() == abortc ) {
            mlforce(TEXT54);
/*              "[Macro aborted]" */
            status = FALSE;
        }
    }

    /* and return it */
    return (status);
}

/* LOCAL_VAR:
 *
 * Declare a local variable
 */
int PASCAL NEAR local_var P2_(int, f, int, n)
/* f: Default flag                */
/* n: Numeric arg (ignored here)  */
{
    REGISTER int status;        /* status return */
    VDESC vd;                   /* variable num/type */
    char var[NVSIZE+1];         /* name of variable to fetch */

    /* first get the variable to set.. */
    if ( clexec == FALSE ) {
        status = mlreply(TEXT250, &var[0], NVSIZE+1);
/*               "Local variable to declare: " */
        if ( status != TRUE )
            return (status);
    } else {            /* macro line argument */
        /* grab token and skip it */
        execstr = token(execstr, var, NVSIZE + 1);
    }

    /* check the legality and find the var */
    findvar(var, &vd, NVSIZE + 1, VT_LOCAL);

    /* if its not legal....bitch */
    if ( vd.v_type == -1 ) {
        mlwrite(TEXT52, var);

/*          "%%No such variable as '%s'" */
        return (FALSE);
    }

    /* and set the appropriate value */
    status = svar(&vd, "");

    /* if $debug == TRUE, every assignment will echo a statment to that effect
     * here. */

    if ( macbug && (strcmp(var, "%track") != 0) ) {
        xstrcpy(outline, "(((Locally declare ");

        xstrcat(outline, var);
        xstrcat(outline, ")))");

        /* write out the debug line */
        mlforce(outline);
        update(TRUE);

        /* and get the keystroke to hold the output */
        if ( get_key() == abortc ) {
            mlforce(TEXT54);
/*              "[Macro aborted]" */
            status = FALSE;
        }
    }

    /* and return it */
    return (status);
}

/* FINDVAR:
 *
 * Find a variables type and name
 */
VOID PASCAL NEAR findvar P4_(
        char  *,  var,  /* name of var to get */
        VDESC *,  vd,   /* structure to hold type and ptr */
        int,      size, /* size of var array */
        int,      scope /* intended scope of any created user variables */
    )
{
    REGISTER int    vnum  = 0;      /* subscript in varable arrays */
    REGISTER int    vtype = 0;      /* type to return */
    REGISTER UTABLE *vut  = NULL;   /* user var table to search */

fvar:   vtype = -1;
    vut = uv_head;

    switch ( var[0] ) {

    case '$':             /* check for legal enviromnent var */
        if ( ( vnum = binary(&var[1], envval, NELEM(envars), NVSIZE) ) != -1 )
            vtype = TKENV;
        break;

    case '%':             /* check for existing legal user variable */
        while ( vut ) {
            for ( vnum = 0; vnum < vut->size; vnum++ )
                if ( strcmp(&var[1], vut->uv[vnum].u_name) == 0 ) {
                    vtype = TKVAR;
                    goto retvar;
                }
            vut = vut->next;
            if ( scope == VT_LOCAL )
                break;
        }

        /* if we should not define one.... */
        if ( scope == VT_NONE )
            break;

        /* scope it as requested */
        if ( scope == VT_LOCAL )
            vut = uv_head;
        else
            vut = uv_global;

        /* no room left in requested user var table? */
        if ( vnum < vut->size )
            break;

        /* create a new variable */
        for ( vnum = 0; vnum < vut->size; vnum++ )
            if ( vut->uv[vnum].u_name[0] == 0 ) {
                vtype = TKVAR;
                memset( (char *)&vut->uv[vnum].u_name[0], '\0', NVSIZE );
                xstrncpy(vut->uv[vnum].u_name, &var[1], NVSIZE);
                vut->uv[vnum].u_value = NULL;
                break;
            }
        break;

    case '&':                   /* indirect operator? */
        var[4] = 0;
        if ( strcmp(&var[1], "ind") == 0 ) {
            /* grab token, and eval it */
            execstr = token(execstr, var, size);
            XSTRCPY( var, fixnull( getval(var) ) );
            goto fvar;
        }
    }

    /* return the results */
retvar: vd->v_num = vnum;
    vd->v_type = vtype;
    vd->v_ut = vut;

    return;
}

/* SVAR:
 *
 * Set a variable
 */
int PASCAL NEAR svar P2_(VDESC *, var, CONST char *, value)
/* var:   Variable to set */
/* value: Value to set to */
{
    char            *valueL = NULL;
    REGISTER int    vnum    = 0;      /* ordinal number of var refrenced */
    REGISTER int    vtype   = 0;      /* type of variable to set */
    REGISTER UTABLE *vut    = NULL;   /* user table pointer */
    REGISTER int    status  = 0;      /* status return */
    REGISTER int    c       = 0;      /* translated character */

    valueL  = xstrdup(value);
    /* simplify the vd structure (we are gonna look at it a lot) */
    vnum  = var->v_num;
    vtype = var->v_type;
    vut   = var->v_ut;

    /* and set the appropriate value */
    status  = TRUE;
    switch ( vtype ) {
    case TKVAR:     /* set a user variable */
        CLROOM(vut->uv[vnum].u_value);
        vut->uv[vnum].u_value = xstrdup(valueL);

        /* setting a variable to error stops macro execution */
        if ( strcmp(valueL, errorm) == 0 )
            status = FALSE;

        break;

    case TKENV:     /* set an environment variable */
        switch ( vnum ) {
        case EVABBELL:
            ab_bell = stol(valueL);
            break;

        case EVABCAP:
            ab_cap = stol(valueL);
            break;

        case EVABFULL:
            ab_full = stol(valueL);
            break;

        case EVABQUICK:
            ab_quick = stol(valueL);
            break;

        case EVACOUNT:
            gacount = asc_int(valueL);
            break;

        case EVASAVE:
            gasave = asc_int(valueL);
            break;

        case EVBUFHOOK:
            set_key(&bufhook, valueL);
            break;

        case EVCBFLAGS:
            c = asc_int(valueL);
            curbp->b_flag = ( curbp->b_flag & ~(BFCHG|BFINVS) )|
                            ( c & (BFCHG|BFINVS) );
            if ( (c & BFCHG) == BFCHG )
                lchange(WFMODE);
            break;

        case EVCBUFNAME:
            XSTRCPY(curbp->b_bname, valueL);
            curwp->w_flag |= WFMODE;
            break;

        case EVCFNAME:
            XSTRCPY(curbp->b_fname, valueL);
#if     WINDOW_MSWIN
            fullpathname(curbp->b_fname, NFILEN);
#endif
            curwp->w_flag |= WFMODE;
            break;

        case EVCMDHK:
            set_key(&cmdhook, valueL);
            break;

        case EVCMODE:
            curbp->b_mode = asc_int(valueL);
            curwp->w_flag |= WFMODE;
            break;

        case EVCQUOTE:
            cquote = asc_int(valueL);
            break;

        case EVCURCHAR:
            ldelete(1L, FALSE);       /* delete 1 char */
            c = asc_int(valueL);
            if ( c == '\r' )
                lnewline();
            else
                linsert(1, (char)c);
            backchar(FALSE, 1);
            break;

        case EVCURCOL:
            status = setccol( asc_int(valueL) );
            break;

        case EVCURLINE:
            status = gotoline( TRUE, asc_int(valueL) );
            break;

        case EVCURWIDTH:
            status = newwidth( TRUE, asc_int(valueL) );
            break;

        case EVCURWIND:
            nextwind( TRUE, asc_int(valueL) );
            break;

        case EVCWLINE:
            status = forwline( TRUE, asc_int(valueL) - getwpos() );
            break;

        case EVDEBUG:
            macbug = stol(valueL);
            break;

        case EVDESKCLR:
            c = lookup_color( mkupper(valueL) );
            if ( c != -1 ) {
                deskcolor = c;
#if     WINDOW_TEXT
                refresh_screen(first_screen);
#endif
            }
            break;

        case EVDIAGFLAG:
            diagflag = stol(valueL);
            break;

        case EVDISCMD:
            discmd = stol(valueL);
            break;

        case EVDISINP:
            disinp = stol(valueL);
            break;

        case EVDISPHIGH:
            c = disphigh;
            disphigh = stol(valueL);
            if ( c != disphigh )
                upwind();
            break;

        case EVDISPUNDO:
            dispundo = stol(valueL);
            break;

        case EVEXBHOOK:
            set_key(&exbhook, valueL);
            break;

        case EVEXITHOOK:
            set_key(&exithook, valueL);
            break;

        case EVFCOL:
            curwp->w_fcol = asc_int(valueL);
            if ( curwp->w_fcol < 0 )
                curwp->w_fcol = 0;
            curwp->w_flag |= WFHARD | WFMODE;
            break;

        case EVFILLCOL:
            fillcol = asc_int(valueL);
            break;

        case EVFLICKER:
            flickcode = stol(valueL);
            break;

        case EVFMTLEAD:
            xstrlcpy(fmtlead, valueL, NSTRING);
            break;

        case EVGFLAGS:
            gflags = asc_int(valueL);
            break;

        case EVGMODE:
            gmode = asc_int(valueL);
            break;

        case EVHARDTAB:
            if ( ( c = asc_int(valueL) ) >= 0 ) {
                tabsize = c;
                upwind();
            }
            break;

        case EVHILITE:
            hilite = asc_int(valueL);
            if ( !hilite_IsValid() )
                hilite_InValidate();
            break;

        case EVHJUMP:
            hjump = asc_int(valueL);
            if ( hjump < 1 )
                hjump = 1;
            if ( hjump > term.t_ncol - 1 )
                hjump = term.t_ncol - 1;
            break;

        case EVHSCRLBAR:
            hscrollbar = stol(valueL);
            break;

        case EVHSCROLL:
            hscroll = stol(valueL);
            lbound = 0;
            break;

        case EVISTERM:
            isterm = stock(valueL);
            break;

        case EVKILL:
            break;

        case EVLANG:
            break;

        case EVLASTKEY:
            lastkey = asc_int(valueL);
            break;

        case EVLASTMESG:
            xstrcpy(lastmesg, valueL);
            break;

        case EVLINE:
            putctext(valueL);
            break;

        case EVLTERM:
            xstrlcpy(lterm, valueL, NSTRING);
            break;

        case EVLWIDTH:
            break;

        case EVMATCH:
            break;

        case EVMMOVE:
            mouse_move = asc_int(valueL);
            if ( mouse_move < 0 ) mouse_move = 0;
            if ( mouse_move > 2 ) mouse_move = 2;
            break;

        case EVMODEFLAG:
            modeflag = stol(valueL);
            upwind();
            break;

        case EVMSFLAG:
            mouseflag = stol(valueL);
            break;

        case EVNEWSCRN:
            newscreenflag = stol(valueL);
            break;

        case EVNUMWIND:
            break;

        case EVOQUOTE:
            oquote = asc_int(valueL);
            break;

        case EVORGCOL:
            status = new_col_org( TRUE, asc_int(valueL) );
            break;

        case EVORGROW:
            status = new_row_org( TRUE, asc_int(valueL) );
            break;

        case EVOS:
            break;

        case EVOVERLAP:
            overlap = asc_int(valueL);
            break;

        case EVPARINDENT:
            parindent = asc_int(valueL);
            break;

        case EVPAGELEN:
            status = newsize( TRUE, asc_int(valueL) );
            break;

        case EVPALETTE:
            xstrlcpy(palstr, valueL, palstr_LEN);
            spal(palstr);
            break;

        case EVPARALEAD:
            xstrlcpy(paralead, valueL, NSTRING);
            break;

        case EVPENDING:
            break;

        case EVPOPFLAG:
            popflag = stol(valueL);
            break;

        case EVPOPWAIT:
            popwait = stol(valueL);
            break;

        case EVPOSFLAG:
            posflag = stol(valueL);
            upmode();
            break;

        case EVPROGNAME:
            break;

        case EVRAM:
            break;

        case EVREADHK:
            set_key(&readhook, valueL);
            break;

        case EVREGION:
            break;

        case EVREPLACE:
            xstrcpy( (char *)rpat, valueL );
#if     MAGIC
            rmcclear();
#endif
            break;

        case EVRVAL:
            xstrcpy(rval, valueL);
            break;

        case EVSCRNAME:
            select_screen(lookup_screen(valueL), TRUE);
            break;

        case EVSEARCH:
            xstrcpy( (char *)pat, valueL );
            setjtable();                     /* Set up fast search arrays  */
#if     MAGIC
            mcclear();
#endif
            break;

        case EVSEARCHPNT:
            searchtype = asc_int(valueL);
            if ( searchtype < SRNORM  || searchtype > SREND )
                searchtype = SRNORM;
            break;

        case EVSEED:
            seed = (long)abs( asc_int(valueL) );
            break;

        case EVSOFTTAB:
            stabsize = asc_int(valueL);
            upwind();
            break;

        case EVSRES:
            status = TTrez(valueL);
            break;

        case EVSSAVE:
            ssave = stol(valueL);
            break;

        case EVSSCROLL:
            sscroll = stol(valueL);
            break;

        case EVSTATUS:
            cmdstatus = stol(valueL);
            break;

        case EVSTERM:
            sterm = stock(valueL);
            break;

        case EVTARGET:
            curgoal = asc_int(valueL);
            thisflag = saveflag;
            break;

        case EVTIME:
            break;

        case EVTIMEFLAG:
            timeflag = stol(valueL);
            upmode();
            break;

        case EVTPAUSE:
            term.t_pause = asc_int(valueL);
            break;

        case EVUNDOFLAG:
            if ( undoflag != stol(valueL) )
                undo_dump();
            undoflag = stol(valueL);
            break;

        case EVVERSION:
            break;

        case EVVSCRLBAR:
            vscrollbar = stol(valueL);
            break;

        case EVWCHARS:
            setwlist(valueL);
            break;

        case EVWLINE:
            status = resize( TRUE, asc_int(valueL) );
            break;

        case EVWRAPHK:
            set_key(&wraphook, valueL);
            break;

        case EVWRITEHK:
            set_key(&writehook, valueL);
            break;

        case EVXPOS:
            xpos = asc_int(valueL);
            break;

        case EVYANKFLAG:
            yankflag = stol(valueL);
            break;

        case EVYPOS:
            ypos = asc_int(valueL);
            break;
        }
        break;
    }


    CLROOM(valueL);

    return (status);
}

/* ASC_INT:
 *
 * ASCII string to integer......This is too inconsistant to use the
 * system's
 */
int PASCAL NEAR asc_int P1_(CONST char *, st)
{
    int result;         /* resulting number */
    int sign;           /* sign of resulting number */
    char c;             /* current char being examined */

    result = 0;
    sign = 1;

    /* skip preceding whitespace */
    while ( *st == ' ' || *st == '\t' )
        ++st;

    /* check for sign */
    if ( *st == '-' ) {
        sign = -1;
        ++st;
    }
    if ( *st == '+' )
        ++st;

    /* scan digits, build value */
    while ( '\0' != (c = *st++) )
        if ( c >= '0' && c <= '9' )
            result = result * 10 + c - '0';
        else
            break;


    return (result * sign);
}

/* INT_ASC:
 *
 * Integer to ascii string .......... This is too inconsistant to use
 * the system's
 *
 * Function returns a static result string: Copy immediately!
 */
char *PASCAL NEAR int_asc P1_(int, i)
/* i: Integer to translate to a string  */
{
    STATIC_STR_RET_PROLOG();

    REGISTER int  digit = 0;            /* current digit being used */
    REGISTER char *sp   = NULL;         /* pointer into result      */
    REGISTER int  sign  = 0;            /* sign of resulting number */
    char          result[INTWIDTH+1];   /* resulting string         */

    ZEROMEM(result);

    /* this is a special case for two byte integers but not wrong in
     * the general case. One could extend this to four or eight byte
     * integers as well --- TODO.
     */
    if ( i == -32768 ) {
        XSTRCPY(result, "-32768");

        RETURN ( result );
    }

    /* record the sign...*/
    sign = 1;
    if ( i < 0 ) {
        sign = -1;
        i = -i;
    }

    /* and build the string (backwards!) */
    sp = result + SIZEOF(result) - 1;
    *sp = 0;
    do {
        digit = i % 10;
        *(--sp) = '0' + digit;  /* and install the new digit */
        i = i / 10;
    } while (i);

    /* and fix the sign */
    if ( sign == -1 ) {
        *(--sp) = '-';          /* and install the minus sign */
    }

    RETURN ( sp );


    STATIC_STR_RET_EPILOG(int_asc, char *, INTWIDTH + 1);
}

/* LONG_ASC:
 *
 * Long int to ascii string .......... This is too inconsistant to use
 * the system's
 *
 * Function returns a static result string: Copy immediately!
 */
char *PASCAL NEAR long_asc P1_(long int, num)
/* num: Integer to translate to a string  */
{
    STATIC_STR_RET_PROLOG();

    REGISTER int  digit = 0;    /* current digit being used */
    REGISTER char *sp   = NULL; /* pointer into result      */
    REGISTER int  sign  = 0;    /* sign of resulting number */
    char result[LONGWIDTH+1];   /* resulting string         */

    ZEROMEM(result);

    /* record the sign...*/
    sign = 1;
    if ( num < 0L ) {
        sign = -1;
        num = -num;
    }

    /* and build the string (backwards!) */
    sp = result + SIZEOF(result) - 1;
    *sp = 0;
    do {
        digit = num % 10;
        *(--sp) = '0' + digit;          /* and install the new digit */
        num = num / 10L;
    } while (num);

    /* and fix the sign */
    if ( sign == -1 ) {
        *(--sp) = '-';          /* and install the minus sign */
    }

    RETURN ( sp );


    STATIC_STR_RET_EPILOG(long_asc, char *, LONGWIDTH + 1);
}

/* GETTYP:
 *
 * Find the type of a passed token
 */
int PASCAL NEAR gettyp P1_(char *, token)
/* token: Token to analyze  */
{
    REGISTER char c;            /* first char in token */

    /* grab the first char (this is all we need) */
    c = *token;

    /* no blanks!!! */
    if ( c == 0 )
        return (TKNUL);

    /* a numeric literal? */
    if ( c >= '0' && c <= '9' )
        return (TKLIT);

    switch ( c ) {
    case '"':
        return (TKSTR);

    case '!':
        return (TKDIR);

    case '@':
        return (TKARG);

    case '#':
        return (TKBUF);

    case '$':
        return (TKENV);

    case '%':
        return (TKVAR);

    case '&':
        return (TKFUN);

    case '*':
        return (TKLBL);

    default:
        return (TKCMD);
    }
}

/* GETVAL:
 *
 * Find the value of a token
 *
 * Function returns a static result string: Copy immediately!
 */
CONST char *PASCAL NEAR getval P1_(char *, token)
/* token: token to evaluate */
{
    STATIC_STR_RET_PROLOG();

    REGISTER int    status  = 0;      /* error return                 */
    REGISTER BUFFER *bp     = NULL;   /* temp buffer pointer          */
    REGISTER int    blen    = 0;      /* length of buffer argument    */
    char            buf[NSTRING];     /* string buf for some returns  */

    ZEROMEM(buf);

    ASRT(NULL != token);

    switch ( gettyp(token) )  {
    case TKNUL:
        RETURN ( "" );

    case TKARG:                 /* interactive argument */
        XSTRCPY( token, fixnull( getval(&token[1]) ) );
        mlwrite("%s", token);
        status = getstring( (unsigned char *)buf, NSTRING, ctoec(RETCHAR) );
        if ( status == ABORT )
            RETURN ( NULL );

        RETURN ( buf );

    case TKBUF:                 /* buffer contents fetch */

        /* grab the right buffer */
        XSTRCPY( token, fixnull( getval(&token[1]) ) );
        bp = bfind(token, FALSE, 0);
        if ( bp == NULL )
            RETURN ( NULL );

        /* if the buffer is displayed, get the window vars instead of the buffer
         * vars */
        if ( bp->b_nwnd > 0 ) {
            curbp->b_dotp = curwp->w_dotp;
            set_b_doto(curbp, get_w_doto(curwp));
        }

        /* if we are at the end, return <END> */
        if ( bp->b_linep == bp->b_dotp )
            RETURN ( "<END>" );

        /* grab the line as an argument */
        blen = get_lused(bp->b_dotp) - get_b_doto(bp);
        if ( blen >= NSTRING )
            blen = NSTRING - 1;
        bytecopy(buf, ltext(bp->b_dotp) + get_b_doto(bp), blen);
        /* buf[blen] = 0; /o Done by bytecopy o/  */

        /* and step the buffer's line ptr ahead a line */
        bp->b_dotp = lforw(bp->b_dotp);
        set_b_doto(bp, 0);

        /* if displayed buffer, reset window ptr vars*/
        if ( bp->b_nwnd > 0 ) {
            curwp->w_dotp = curbp->b_dotp;
            set_w_doto(curwp, 0);
            curwp->w_flag |= WFMOVE;
        }

        /* and return the spoils */
        RETURN ( buf );

    case TKVAR:
        RETURN ( gtusr(token+1) );

    case TKENV:
        RETURN ( gtenv(token+1) );

    case TKFUN:
        RETURN ( gtfun(token+1) );

    case TKDIR:
        RETURN ( NULL );

    case TKLBL:
        RETURN ( NULL );

    case TKLIT:
        RETURN ( token );

    case TKSTR:
        RETURN ( token + 1 );

    case TKCMD:
        RETURN ( token );
    }


    STATIC_STR_RET_EPILOG(getval, CONST char *, NSTRING);
}

/* STOL:
 *
 * Convert a string to a numeric logical
 */
int PASCAL NEAR stol P1_(CONST char *, val)
/* val: Value to check for stol */
{
    /* check for logical values */
    if ( val[0] == 'F' )
        return (FALSE);

    if ( val[0] == 'T' )
        return (TRUE);

    /* check for numeric truth (!= 0) */
    return ( (asc_int(val) != 0) );
}

/* LTOS:
 *
 * Numeric logical to string logical
 */
CONST char *PASCAL NEAR ltos P1_(int, val)
/* val: Value to translate  */
{
    if ( val )
        return (truem);
    else
        return (falsem);
}

/* MKUPPER:
 *
 * Make a string upper case
 */
char *PASCAL NEAR mkupper P1_(char *, str)
/* str: String to upper case  */
{
    char *sp;

    sp = str;
    while ( *sp )
        uppercase( (unsigned char *)sp++ );

    return (str);
}

/* MKLOWER:
 *
 * Make a string lower case
 */
char *PASCAL NEAR mklower P1_(char *, str)
/* str: String to lower case  */
{
    char *sp;

    sp = str;
    while ( *sp )
        lowercase( (unsigned char *)sp++ );

    return (str);
}

/* ABSV:
 *
 * Take the absolute value of an integer
 */
int PASCAL NEAR absv P1_(int, x)
{
    return (x < 0 ? -x : x);
}

/* ERND:
 *
 * Returns a random integer
 *
 * This function implements the "minimal standard" RNG from the paper "RNGs:
 * Good Ones are Hard to Find" by Park and Miller, CACM, Volume 31, Number 10,
 * October 1988.
 */
long PASCAL NEAR ernd P0_()
{
    long int a=16807L, m=2147483647L, q=127773L, r=2836L;
    long lo, hi, test;

    hi = seed / q;
    lo = seed % q;
    test = a * lo - r * hi;
    seed = (test > 0) ? test : test + m;

    return (seed);
}

/* SINDEX:
 *
 * Find pattern within source
 */
int PASCAL NEAR sindex P2_(CONST char *, source, CONST char *, pattern)
/* source:  Source string to search */
/* pattern: String to look for      */
{
    CONST char *sp;     /* ptr to current position to scan */
    CONST char *csp;    /* ptr to source string during comparison */
    CONST char *cp;     /* ptr to place to check for equality */

    /* scanning through the source string */
    sp = source;
    while ( *sp ) {
        /* scan through the pattern */
        cp = pattern;
        csp = sp;
        while ( *cp ) {
            if ( !eq(*cp, *csp) )
                break;
            ++cp;
            ++csp;
        }

        /* was it a match? */
        if ( *cp == 0 )
            return ( (int)(sp - source) + 1 );

        ++sp;
    }

    /* no match at all.. */
    return (0);
}

/* XLAT:
 *
 * Filter a string through a translation table
 *
 * Function returns a static result string: Copy immediately!
 */
char *PASCAL NEAR xlat P3_(char *, source, char *, lookup, char *, trans)
/* source:  string to filter                */
/* lookup:  characters to translate         */
/* trans:   resulting translated characters */
{
    STATIC_STR_RET_PROLOG();
    REGISTER char *sp = NULL;       /* pointer into source table  */
    REGISTER char *lp = NULL;       /* pointer into lookup table  */
    REGISTER char *rp = NULL;       /* pointer into result        */
    char          result[NSTRING];  /* temporary result           */

    ZEROMEM(result);

    ASRT(NULL != source);
    ASRT(NULL != lookup);
    ASRT(NULL != trans);

    /* scan source string */
    sp = source;
    rp = result;
    while ( *sp ) {
        /* scan lookup table for a match */
        lp = lookup;
        while ( *lp ) {
            if ( *sp == *lp ) {
                *rp++ = trans[lp - lookup];
                goto xnext;
            }
            ++lp;
        }

        /* no match, copy in the source char untranslated */
        *rp++ = *sp;

xnext:  ++sp;
    }

    /* terminate and return the result */
    *rp = 0;

    RETURN (result);

    STATIC_STR_RET_EPILOG(xlat, char *, NSTRING);
}

/* SETWLIST:
 *
 * Set an alternative list of character to be considered "in a word
 */
int PASCAL NEAR setwlist P1_(char *, wclist)
/* wclist:  List of characters to consider "in a word"  */
{
    REGISTER int index;

    /* if we are turning this facility off, just flag so */
    if ( wclist == NULL || *wclist == 0 ) {
        wlflag = FALSE;

        return 0;
    }

    /* first clear the table */
    for ( index = 0; index < 256; index++ )
        wordlist[index] = FALSE;

    /* and for each character in the new value, set that element of the word
     * character list */
    while ( *wclist )
        wordlist[(unsigned char)(*wclist++)] = TRUE;            /* ep */
    wlflag = TRUE;

    return 0;
}

/* GETWLIST:
 *
 * Place in a buffer a list of characters considered "in a word"
 */
 /***TODO: Missing size info***/
CONST char *PASCAL NEAR getwlist P1_(char *, buf)
/* buf: Buffer to place list of characters  */
{
    REGISTER int index;
    REGISTER char *sp;

    /* if we are defaulting to a standard word char list... */
    if ( wlflag == FALSE )
        return ("");

    /* build the string of characters in the return buffer */
    sp = buf;
    for ( index = 0; index < 256; index++ )
        if ( wordlist[index] )
            *sp++ = index;

    *sp = 0;

    return (buf);
}

/* IS_NUM:
 *
 * ASCII string is integer......This is too inconsistant to use the
 * system's
 */
int PASCAL NEAR is_num P1_(char *, st)
{
    int period_flag;            /* have we seen a period yet? */

    /* skip preceding whitespace */
    while ( *st == ' ' || *st == '\t' )
        ++st;

    /* check for sign */
    if ( (*st == '-') || (*st == '+') )
        ++st;

    /* scan digits */
    period_flag = FALSE;
    while ( (*st >= '0' && *st <= '9') ||
            (*st == '.' && period_flag == FALSE) ) {
        if ( *st == '.' )
            period_flag = TRUE;
        st++;
    }

    /* scan rest of line for just white space */
    while ( *st ) {
        if ( (*st != '\t') && (*st != ' ') )
            return (FALSE);

        st++;
    }

    return (TRUE);
}

/* DISPVAR:
 *
 * Display a variable's value
 */
int PASCAL NEAR dispvar P2_(int, f, int, n)
/* f: Default flag                              */
/* n: Numeric arg (can overide prompted value)  */
{
    REGISTER int status;        /* status return */
    VDESC vd;                   /* variable num/type */
    char var[NVSIZE+1];         /* name of variable to fetch */

    /* first get the variable to display.. */
    if ( clexec == FALSE ) {
        status = mlreply(TEXT55, &var[0], NVSIZE+1);
/*               "Variable to display: " */
        if ( status != TRUE )
            return (status);
    } else {            /* macro line argument */
        /* grab token and skip it */
        execstr = token(execstr, var, NVSIZE + 1);
    }

    /* check the legality and find the var */
    findvar(var, &vd, NVSIZE + 1, VT_NONE);

    /* if its not legal....bitch */
    if ( vd.v_type == -1 ) {
        mlwrite(TEXT52, var);

/*          "%%No such variable as '%s'" */
        return (FALSE);
    }

    /* and display the value */
    xstrcpy(outline, var);
    xstrcat(outline, " = ");

    /* and lastly the current value */
    xstrcat( outline, fixnull( getval(var) ) );

    /* write out the result */
    mlforce(outline);
    update(TRUE);

    /* and return */
    return (TRUE);
}

/* DESVARS:
 *
 * describe-variables:  Bring up a fake buffer and list the contents
 *                      of all the environment variables
 */
int PASCAL NEAR desvars P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER BUFFER *varbuf;    /* buffer to put variable list into */
    REGISTER int uindex;        /* index into uvar table */
    UTABLE *ut;                 /* user variable table pointer */
    PARG *cur_arg;              /* ptr to buffers argument list */
    char outseq[NSTRING];       /* output buffer for keystroke sequence */

    /* and get a buffer for it */
    varbuf = bfind(TEXT56, TRUE, BFINVS);
/*         "Variable list" */
    if ( varbuf == NULL || bclear(varbuf) == FALSE ) {
        mlwrite(TEXT57);

/*          "Can not display variable list" */
        return (FALSE);
    }

    /* let us know this is in progress */
    mlwrite(TEXT58);
/*      "[Building variable list]" */

    /* build the environment variable list */
    for ( uindex = 0; uindex < NELEM(envars); uindex++ ) {

        /* add in the environment variable name */
        XSTRCPY(outseq, "$");
        XSTRCAT(outseq, envars[uindex]);
        pad(outseq, 14);

        /* add in the value */
        XSTRCAT(outseq, gtenv(envars[uindex]));

        /* and add it as a line into the buffer */
        if ( addline(varbuf, outseq) != TRUE )
            return (FALSE);
    }

    /* build all the user variable lists */
    ut = uv_head;
    while ( ut ) {

        /* a blank line, please.... */
        if ( addline(varbuf, "") != TRUE )
            return (FALSE);

        /* make a header for this list */
        XSTRCPY(outseq, "----- ");
        if ( ut->bufp == (BUFFER *)NULL )
            XSTRCAT(outseq, "Global User Variables");
        else {
            XSTRCAT(outseq, "Defined in ");
            XSTRCAT(outseq, ut->bufp->b_bname);
            if ( ut->bufp->b_numargs > 0 ) {
                XSTRCAT(outseq, "(");
                cur_arg = ut->bufp->b_args;
                while ( cur_arg ) {
                    if ( cur_arg != ut->bufp->b_args )
                        XSTRCAT(outseq, ", ");
                    XSTRCAT(outseq, cur_arg->name);
                    cur_arg = cur_arg->next;
                }
                XSTRCAT(outseq, ")");
            }
        }
        XSTRCAT(outseq, " -----");

        /* and add it as a line into the buffer */
        if ( addline(varbuf, outseq) != TRUE )
            return (FALSE);

        /* build this list */
        for ( uindex = 0; uindex < ut->size; uindex++ ) {
            if ( ut->uv[uindex].u_name[0] == 0 )
                break;

            /* add in the user variable name */
            XSTRCPY(outseq, "%");
            XSTRCAT(outseq, ut->uv[uindex].u_name);
            pad(outseq, 14);

            /* add in the value */
            XSTRCAT(outseq, ut->uv[uindex].u_value);

            /* and add it as a line into the buffer */
            if ( addline(varbuf, outseq) != TRUE )
                return (FALSE);
        }
        ut = ut->next;
    }

    /* display the list */
    wpopup(varbuf);
    mlerase();          /* clear the mode line */

    return (TRUE);
}

/* DESFUNC:
 *
 * describe-functions:  Bring up a fake buffer and list the names
 *                      of all the functions
 */
int PASCAL NEAR desfunc P2_(int, f, int, n)
/* f, n:  Prefix flag and argument  */
{
    REGISTER BUFFER *fncbuf;    /* buffer to put function list into */
    REGISTER int uindex;        /* index into funcs table */
    char outseq[80];            /* output buffer for keystroke sequence */

    /* get a buffer for the function list */
    fncbuf = bfind(TEXT211, TRUE, BFINVS);
/*         "Function list" */
    if ( fncbuf == NULL || bclear(fncbuf) == FALSE ) {
        mlwrite(TEXT212);

/*          "Can not display function list" */
        return (FALSE);
    }

    /* let us know this is in progress */
    mlwrite(TEXT213);
/*      "[Building function list]" */

    /* build the function list */
    for ( uindex = 0; uindex < NELEM(funcs); uindex++ ) {

        /* add in the environment variable name */
        XSTRCPY(outseq, "&");
        XSTRCAT(outseq, funcs[uindex].f_name);

        /* and add it as a line into the buffer */
        if ( addline(fncbuf, outseq) != TRUE )
            return (FALSE);
    }

    if ( addline(fncbuf, "") != TRUE )
        return (FALSE);

    /* display the list */
    wpopup(fncbuf);
    mlerase();          /* clear the mode line */

    return (TRUE);
}

/* PAD:
 *
 * Pad a string to indicated length
 */
VOID PASCAL NEAR pad P2_(char *, s, int, len)
/* s:   String to add spaces to */
/* len: Wanted length of string */
{
    int l = 0;

    ASRT(NULL != s);

    if ( len > (l = STRLEN(s)) )  {
        for ( ; l < len; l++ )  {
            s[l]  = ' ';
        }
        s[l]  = '\0';
    }
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
