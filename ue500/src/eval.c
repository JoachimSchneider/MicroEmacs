/*  EVAL.C: Expresion evaluation functions for
 *               MicroEMACS
 *
 *  written 1993 by Daniel Lawrence
 */

#include <stdio.h>
#include "estruct.h"
#include "eproto.h"
#include "edef.h"
#include "elang.h"
#include "evar.h"


#define RETURN  STATIC_STR_RET_RETURN


/* initialize the entries in one user variable table */

VOID PASCAL NEAR uv_init(ut)

UTABLE *ut;     /* user variable table to initialize */

{
    register int i;

    for ( i=0; i < ut->size; i++ ) {
        ut->uv[i].u_name[0] = 0;
        ut->uv[i].u_value = (char *)NULL;
    }
}

VOID PASCAL NEAR varinit()      /* initialize the global user variable table */
{
    /* allocate the global user variable table */
    uv_global = uv_head =
        (UTABLE *)room( sizeof (UTABLE) + MAXVARS * sizeof (UVAR) );

    /* and set up its fields */
    uv_head->next = (UTABLE *)NULL;
    uv_head->size = MAXVARS;
    uv_head->bufp = (BUFFER *)NULL;
    uv_init(uv_head);
}

VOID PASCAL NEAR uv_clean(ut)   /* discard the contents of a user variable table
                                 */

UTABLE *ut;     /* ptr to table to clear */

{
    register int i;

    /* now clear the entries in this one */
    for ( i=0; i < ut->size; i++ )
        if ( ut->uv[i].u_name[0] != 0 )
            free(ut->uv[i].u_value);

}

VOID PASCAL NEAR varclean(ut)   /* discard and clear all user variable tables */

UTABLE *ut;     /* table to clear */

{
    /* first clean all the ones under this one */
    if ( ut->next != (UTABLE *)NULL )
        varclean(ut->next);

    /* clear the contents of this table */
    uv_clean(ut);

    /* and then deallocate the this table itself */
    free(ut);
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
    register int  fnum    = 0;          /* index to function to eval  */
    register int  arg     = 0;          /* value of some arguments    */
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
    fnum = binary(fnameL, funval, NFUNCS, MINFLEN);

    /* return errorm on a bad reference */
    if ( fnum == -1 ) {
        mlwrite(TEXT244, fnameL);
/*          "%%No such function as '%s'" */
        free(fnameL);

        RETURN ( errorm );
    }
    free(fnameL);

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
        xstrlcpy(&result[1], arg1, sizeof(result) - 1);
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
        strncat(result, arg2, NSTRING);
        result[NSTRING - 1] = 0;

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
        RETURN ( fixnull( flook(arg1, TRUE) ) );

    case UFGREATER:
        RETURN ( ltos( asc_int(arg1) > asc_int(arg2) ) );

    case UFGROUP:
        arg = asc_int(arg1);
#if     MAGIC
        if ( arg < 0 || arg >= MAXGROUPS )
            RETURN ( bytecopy(result, errorm, NSTRING * 2) );

        RETURN ( bytecopy(result, fixnull(grpmatch[arg]), NSTRING * 2) );
#else
        if ( arg == 0 )
            bytecopy(result, patmatch, NSTRING * 2);
        else
            RETURN ( bytecopy(result, errorm, NSTRING * 2) );

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
        RETURN ( bytecopy( result, arg1, asc_int(arg2) ) );

    case UFLENGTH:
        RETURN ( int_asc( strlen(arg1) ) );

    case UFLESS:
        RETURN ( ltos( asc_int(arg1) < asc_int(arg2) ) );

    case UFLOWER:
        RETURN ( mklower(arg1) );

    case UFMID:
        arg = asc_int(arg2);
        if ( arg > strlen(arg1) )
            RETURN ( XSTRCPY(result, "") );

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
        RETURN ( strrev( bytecopy(result, arg1, NSTRING * 2) ) );

    case UFRIGHT:
        arg = asc_int(arg2);
        if ( arg > strlen(arg1) )
            arg = strlen(arg1);

        RETURN ( XSTRCPY(result, &arg1[strlen(arg1) - arg]) );

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
    char            *vnameA = xstrdup(vname);
    register int vnum;          /* ordinal number of user var */
    register char   *vptr;      /* temp pointer to function value */
    register UTABLE *ut;        /* ptr to the current variable table */

    /* limit comparisons to significant length */
    if ( strlen(vnameA) >= NVSIZE )     /* "%" counts, but is not passed */
        vnameA[NVSIZE] = '\0';

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
            if ( strcmp(vnameA, ut->uv[vnum].u_name) == 0 ) {

                free(vnameA);
                /* return its value..... */
                vptr = ut->uv[vnum].u_value;
                if ( vptr )
                    return (vptr);
                else
                    return (errorm);
            }
        }

next_ut:        ut = ut->next;
    }

    /* return errorm if we run off the end */
    free(vnameA);

    return (errorm);
}

char *PASCAL NEAR funval(i)

int i;

{
    return (funcs[i].f_name);
}

char *PASCAL NEAR envval(i)

int i;

{
    return (envars[i]);
}

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

    register int  vnum  = 0;            /* ordinal number of var
                                         * referenced     */
    char          result[2 * NSTRING];  /* string result  */

    ZEROMEM(result);

    ASRT(NULL != vname);

    /* scan the list, looking for the referenced name */
    vnum = binary(vname, envval, NEVARS, NVSIZE);

    /* return errorm on a bad reference */
    if ( vnum == -1 )
        RETURN ( errorm );

    /* otherwise, fetch the appropriate value */
    switch ( vnum ) {
    case EVABBELL:
        RETURN ( ltos(ab_bell) );

    case EVABCAP:
        RETURN ( ltos(ab_cap) );

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
char *PASCAL NEAR getkill P0_(void)
{
    STATIC_STR_RET_PROLOG();

    register int  size    = 0;    /* max num of chars left to return  */
    register char *sp     = NULL; /* ptr into KILL block data chunk   */
    register char *vp     = NULL; /* ptr into return value            */
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

char *PASCAL NEAR trimstr(s)    /* trim whitespace off the end of a string */

char *s;        /* string to trim */

{
    char *sp;           /* backward index */

    sp = s + strlen(s) - 1;
    while ( (sp >= s) && (*sp == ' ' || *sp == '\t') )
        --sp;
    *(sp+1) = 0;

    return (s);
}

int PASCAL NEAR setvar(f, n)            /* set a variable */

int f;          /* default flag */
int n;          /* numeric arg (can overide prompted value) */

{
    register int status;        /* status return */
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

int PASCAL NEAR global_var(f, n)        /* declare a global variable */

int f;          /* default flag */
int n;          /* numeric arg (ignored here) */

{
    register int status;        /* status return */
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

int PASCAL NEAR local_var(f, n) /* declare a local variable */

int f;          /* default flag */
int n;          /* numeric arg (ignored here) */

{
    register int status;        /* status return */
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

/* find a variables type and name */

VOID PASCAL NEAR findvar P4_(
        char  *,  var,  /* name of var to get */
        VDESC *,  vd,   /* structure to hold type and ptr */
        int,      size, /* size of var array */
        int,      scope /* intended scope of any created user variables */
    )
{
    register int    vnum  = 0;      /* subscript in varable arrays */
    register int    vtype = 0;      /* type to return */
    register UTABLE *vut  = NULL;   /* user var table to search */

fvar:   vtype = -1;
    vut = uv_head;

    switch ( var[0] ) {

    case '$':             /* check for legal enviromnent var */
        if ( ( vnum = binary(&var[1], envval, NEVARS, NVSIZE) ) != -1 )
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

int PASCAL NEAR svar(var, value)        /* set a variable */

VDESC *var;     /* variable to set */
char *value;    /* value to set to */

{
    register int vnum;          /* ordinal number of var refrenced */
    register int vtype;         /* type of variable to set */
    register UTABLE *vut;       /* user table pointer */
    register int status;        /* status return */
    register int c;             /* translated character */
    register char *sp;          /* scratch string pointer */

    /* simplify the vd structure (we are gonna look at it a lot) */
    vnum = var->v_num;
    vtype = var->v_type;
    vut = var->v_ut;

    /* and set the appropriate value */
    status = TRUE;
    switch ( vtype ) {
    case TKVAR:     /* set a user variable */
        if ( vut->uv[vnum].u_value != NULL )
            free(vut->uv[vnum].u_value);
        sp = room(strlen(value) + 1);
        if ( sp == NULL )
            return (FALSE);

        xstrcpy(sp, value);
        vut->uv[vnum].u_value = sp;

        /* setting a variable to error stops macro execution */
        if ( strcmp(value, errorm) == 0 )
            status = FALSE;

        break;

    case TKENV:     /* set an environment variable */
        status = TRUE;          /* by default */

        switch ( vnum ) {
        case EVABBELL:
            ab_bell = stol(value);
            break;

        case EVABCAP:
            ab_cap = stol(value);
            break;

        case EVABQUICK:
            ab_quick = stol(value);
            break;

        case EVACOUNT:
            gacount = asc_int(value);
            break;

        case EVASAVE:
            gasave = asc_int(value);
            break;

        case EVBUFHOOK:
            set_key(&bufhook, value);
            break;

        case EVCBFLAGS:
            c = asc_int(value);
            curbp->b_flag = ( curbp->b_flag & ~(BFCHG|BFINVS) )|
                            ( c & (BFCHG|BFINVS) );
            if ( (c & BFCHG) == BFCHG )
                lchange(WFMODE);
            break;

        case EVCBUFNAME:
            XSTRCPY(curbp->b_bname, value);
            curwp->w_flag |= WFMODE;
            break;

        case EVCFNAME:
            XSTRCPY(curbp->b_fname, value);
#if     WINDOW_MSWIN
            fullpathname(curbp->b_fname, NFILEN);
#endif
            curwp->w_flag |= WFMODE;
            break;

        case EVCMDHK:
            set_key(&cmdhook, value);
            break;

        case EVCMODE:
            curbp->b_mode = asc_int(value);
            curwp->w_flag |= WFMODE;
            break;

        case EVCQUOTE:
            cquote = asc_int(value);
            break;

        case EVCURCHAR:
            ldelete(1L, FALSE);                         /* delete 1 char */
            c = asc_int(value);
            if ( c == '\r' )
                lnewline();
            else
                linsert(1, (char)c);
            backchar(FALSE, 1);
            break;

        case EVCURCOL:
            status = setccol( asc_int(value) );
            break;

        case EVCURLINE:
            status = gotoline( TRUE, asc_int(value) );
            break;

        case EVCURWIDTH:
            status = newwidth( TRUE, asc_int(value) );
            break;

        case EVCURWIND:
            nextwind( TRUE, asc_int(value) );
            break;

        case EVCWLINE:
            status = forwline( TRUE, asc_int(value) - getwpos() );
            break;

        case EVDEBUG:
            macbug = stol(value);
            break;

        case EVDESKCLR:
            c = lookup_color( mkupper(value) );
            if ( c != -1 ) {
                deskcolor = c;
#if     WINDOW_TEXT
                refresh_screen(first_screen);
#endif
            }
            break;

        case EVDIAGFLAG:
            diagflag = stol(value);
            break;

        case EVDISCMD:
            discmd = stol(value);
            break;

        case EVDISINP:
            disinp = stol(value);
            break;

        case EVDISPHIGH:
            c = disphigh;
            disphigh = stol(value);
            if ( c != disphigh )
                upwind();
            break;

        case EVDISPUNDO:
            dispundo = stol(value);
            break;

        case EVEXBHOOK:
            set_key(&exbhook, value);
            break;

        case EVEXITHOOK:
            set_key(&exithook, value);
            break;

        case EVFCOL:
            curwp->w_fcol = asc_int(value);
            if ( curwp->w_fcol < 0 )
                curwp->w_fcol = 0;
            curwp->w_flag |= WFHARD | WFMODE;
            break;

        case EVFILLCOL:
            fillcol = asc_int(value);
            break;

        case EVFLICKER:
            flickcode = stol(value);
            break;

        case EVFMTLEAD:
            bytecopy(fmtlead, value, NSTRING);
            break;

        case EVGFLAGS:
            gflags = asc_int(value);
            break;

        case EVGMODE:
            gmode = asc_int(value);
            break;

        case EVHARDTAB:
            if ( ( c = asc_int(value) ) >= 0 ) {
                tabsize = c;
                upwind();
            }
            break;

        case EVHILITE:
            hilite = asc_int(value);
            if ( hilite > NMARKS )
                hilite = 255;
            break;

        case EVHJUMP:
            hjump = asc_int(value);
            if ( hjump < 1 )
                hjump = 1;
            if ( hjump > term.t_ncol - 1 )
                hjump = term.t_ncol - 1;
            break;

        case EVHSCRLBAR:
            hscrollbar = stol(value);
            break;

        case EVHSCROLL:
            hscroll = stol(value);
            lbound = 0;
            break;

        case EVISTERM:
            isterm = stock(value);
            break;

        case EVKILL:
            break;

        case EVLANG:
            break;

        case EVLASTKEY:
            lastkey = asc_int(value);
            break;

        case EVLASTMESG:
            xstrcpy(lastmesg, value);
            break;

        case EVLINE:
            putctext(value);
            break;

        case EVLTERM:
            bytecopy(lterm, value, NSTRING);
            break;

        case EVLWIDTH:
            break;

        case EVMATCH:
            break;

        case EVMMOVE:
            mouse_move = asc_int(value);
            if ( mouse_move < 0 ) mouse_move = 0;
            if ( mouse_move > 2 ) mouse_move = 2;
            break;

        case EVMODEFLAG:
            modeflag = stol(value);
            upwind();
            break;

        case EVMSFLAG:
            mouseflag = stol(value);
            break;

        case EVNEWSCRN:
            newscreenflag = stol(value);
            break;

        case EVNUMWIND:
            break;

        case EVOQUOTE:
            oquote = asc_int(value);
            break;

        case EVORGCOL:
            status = new_col_org( TRUE, asc_int(value) );
            break;

        case EVORGROW:
            status = new_row_org( TRUE, asc_int(value) );
            break;

        case EVOS:
            break;

        case EVOVERLAP:
            overlap = asc_int(value);
            break;

        case EVPARINDENT:
            parindent = asc_int(value);
            break;

        case EVPAGELEN:
            status = newsize( TRUE, asc_int(value) );
            break;

        case EVPALETTE:
            bytecopy(palstr, value, 48);
            spal(palstr);
            break;

        case EVPARALEAD:
            bytecopy(paralead, value, NSTRING);
            break;

        case EVPENDING:
            break;

        case EVPOPFLAG:
            popflag = stol(value);
            break;

        case EVPOPWAIT:
            popwait = stol(value);
            break;

        case EVPOSFLAG:
            posflag = stol(value);
            upmode();
            break;

        case EVPROGNAME:
            break;

        case EVRAM:
            break;

        case EVREADHK:
            set_key(&readhook, value);
            break;

        case EVREGION:
            break;

        case EVREPLACE:
            xstrcpy( (char *)rpat, value );
#if     MAGIC
            rmcclear();
#endif
            break;

        case EVRVAL:
            xstrcpy(rval, value);
            break;

        case EVSCRNAME:
            select_screen(lookup_screen(value), TRUE);
            break;

        case EVSEARCH:
            xstrcpy( (char *)pat, value );
            setjtable();                     /* Set up fast search arrays  */
#if     MAGIC
            mcclear();
#endif
            break;

        case EVSEARCHPNT:
            searchtype = asc_int(value);
            if ( searchtype < SRNORM  || searchtype > SREND )
                searchtype = SRNORM;
            break;

        case EVSEED:
            seed = (long)abs( asc_int(value) );
            break;

        case EVSOFTTAB:
            stabsize = asc_int(value);
            upwind();
            break;

        case EVSRES:
            status = TTrez(value);
            break;

        case EVSSAVE:
            ssave = stol(value);
            break;

        case EVSSCROLL:
            sscroll = stol(value);
            break;

        case EVSTATUS:
            cmdstatus = stol(value);
            break;

        case EVSTERM:
            sterm = stock(value);
            break;

        case EVTARGET:
            curgoal = asc_int(value);
            thisflag = saveflag;
            break;

        case EVTIME:
            break;

        case EVTIMEFLAG:
            timeflag = stol(value);
            upmode();
            break;

        case EVTPAUSE:
            term.t_pause = asc_int(value);
            break;

        case EVUNDOFLAG:
            if ( undoflag != stol(value) )
                undo_dump();
            undoflag = stol(value);
            break;

        case EVVERSION:
            break;

        case EVVSCRLBAR:
            vscrollbar = stol(value);
            break;

        case EVWCHARS:
            setwlist(value);
            break;

        case EVWLINE:
            status = resize( TRUE, asc_int(value) );
            break;

        case EVWRAPHK:
            set_key(&wraphook, value);
            break;

        case EVWRITEHK:
            set_key(&writehook, value);
            break;

        case EVXPOS:
            xpos = asc_int(value);
            break;

        case EVYANKFLAG:
            yankflag = stol(value);
            break;

        case EVYPOS:
            ypos = asc_int(value);
            break;
        }
        break;
    }

    return (status);
}

/* asc_int: ascii string to integer......This is too inconsistant to use the
 *          system's
 */
int PASCAL NEAR asc_int(st)

char *st;

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

    register int  digit = 0;            /* current digit being used */
    register char *sp   = NULL;         /* pointer into result      */
    register int  sign  = 0;            /* sign of resulting number */
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
    sp = result + sizeof(result) - 1;
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

    register int  digit = 0;    /* current digit being used */
    register char *sp   = NULL; /* pointer into result      */
    register int  sign  = 0;    /* sign of resulting number */
    char result[LONGWIDTH+1];   /* resulting string         */

    ZEROMEM(result);

    /* record the sign...*/
    sign = 1;
    if ( num < 0L ) {
        sign = -1;
        num = -num;
    }

    /* and build the string (backwards!) */
    sp = result + sizeof(result) - 1;
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

int PASCAL NEAR gettyp(token)   /* find the type of a passed token */

char *token;    /* token to analyze */

{
    register char c;            /* first char in token */

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

    register int    status  = 0;      /* error return                 */
    register BUFFER *bp     = NULL;   /* temp buffer pointer          */
    register int    blen    = 0;      /* length of buffer argument    */
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
        if ( blen > NSTRING )
            blen = NSTRING;
        bytecopy(buf, ltext(bp->b_dotp) + get_b_doto(bp), blen);
        buf[blen] = 0;

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

int PASCAL NEAR stol(val)       /* convert a string to a numeric logical */

char *val;      /* value to check for stol */

{
    /* check for logical values */
    if ( val[0] == 'F' )
        return (FALSE);

    if ( val[0] == 'T' )
        return (TRUE);

    /* check for numeric truth (!= 0) */
    return ( (asc_int(val) != 0) );
}

CONST char *PASCAL NEAR ltos(val)   /* numeric logical to string logical */

int val;        /* value to translate */

{
    if ( val )
        return (truem);
    else
        return (falsem);
}

char *PASCAL NEAR mkupper(str)    /* make a string upper case */

char *str;              /* string to upper case */

{
    char *sp;

    sp = str;
    while ( *sp )
        uppercase( (unsigned char *)sp++ );

    return (str);
}

char *PASCAL NEAR mklower(str)    /* make a string lower case */

char *str;              /* string to lower case */

{
    char *sp;

    sp = str;
    while ( *sp )
        lowercase( (unsigned char *)sp++ );

    return (str);
}

int PASCAL NEAR absv(x) /* take the absolute value of an integer */

int x;

{
    return (x < 0 ? -x : x);
}

long PASCAL NEAR ernd() /* returns a random integer */
/* This function implements the "minimal standard" RNG from the paper "RNGs:
 * Good Ones are Hard to Find" by Park and Miller, CACM, Volume 31, Number 10,
 * October 1988.
 */
{
    long int a=16807L, m=2147483647L, q=127773L, r=2836L;
    long lo, hi, test;

    hi = seed / q;
    lo = seed % q;
    test = a * lo - r * hi;
    seed = (test > 0) ? test : test + m;

    return (seed);
}

int PASCAL NEAR sindex(source, pattern) /* find pattern within source */

char *source;   /* source string to search */
char *pattern;  /* string to look for */

{
    char *sp;           /* ptr to current position to scan */
    char *csp;          /* ptr to source string during comparison */
    char *cp;           /* ptr to place to check for equality */

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
    register char *sp = NULL;       /* pointer into source table  */
    register char *lp = NULL;       /* pointer into lookup table  */
    register char *rp = NULL;       /* pointer into result        */
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

/*  setwlist:   Set an alternative list of character to be considered "in a word
 */

int PASCAL NEAR setwlist(wclist)

char *wclist;   /* list of characters to consider "in a word" */

{
    register int index;

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

/*  getwlist:   place in a buffer a list of characters considered "in a word"           */

char *PASCAL NEAR getwlist(buf)

char *buf;      /* buffer to place list of characters */

{
    register int index;
    register char *sp;

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

/*  is_num: ascii string is integer......This is too inconsistant to use the
 * system's */

int PASCAL NEAR is_num(st)

char *st;

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

int PASCAL NEAR dispvar(f, n)           /* display a variable's value */

int f;          /* default flag */
int n;          /* numeric arg (can overide prompted value) */

{
    register int status;        /* status return */
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

/* describe-variables:  Bring up a fake buffer and list the contents of all the
 *                      environment variables
 */
int PASCAL NEAR desvars(f, n)

int f, n;        /* prefix flag and argument */

{
    register BUFFER *varbuf;    /* buffer to put variable list into */
    register int uindex;        /* index into uvar table */
    register int olen;          /* current length of output string */
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
    for ( uindex = 0; uindex < NEVARS; uindex++ ) {

        /* add in the environment variable name */
        XSTRCPY(outseq, "$");
        XSTRCAT(outseq, envars[uindex]);
        pad(outseq, 14);

        /* add in the value */
        olen = strlen(outseq);
        strncat(outseq, gtenv(envars[uindex]), NSTRING - olen - 1);
        outseq[NSTRING - 1] = 0;

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
            olen = strlen(outseq);
            strncat(outseq, ut->uv[uindex].u_value, NSTRING - olen - 1);
            outseq[NSTRING - 1] = 0;

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

/* describe-functions:  Bring up a fake buffer and list the names of all the
 *                      functions
 */
int PASCAL NEAR desfunc(f, n)

int f, n;        /* prefix flag and argument */

{
    register BUFFER *fncbuf;    /* buffer to put function list into */
    register int uindex;        /* index into funcs table */
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
    for ( uindex = 0; uindex < NFUNCS; uindex++ ) {

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

VOID PASCAL NEAR pad(s, len)    /* pad a string to indicated length */

char *s;        /* string to add spaces to */
int len;        /* wanted length of string */

{
    while ( strlen(s) < len ) {
        XSTRCAT(s, "          ");
        s[len] = 0;
    }
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
