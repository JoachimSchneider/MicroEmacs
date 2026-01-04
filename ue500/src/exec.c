/*======================================================================
 *      This file is for functions dealing with execution of
 *      commands, command lines, buffers, files and startup files
 *
 *      written 1993 by Daniel Lawrence
 *====================================================================*/

/*====================================================================*/
#define EXEC_C_
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


/*====================================================================*/
/* Functions which modify the global `execstr' variable:              */
/*====================================================================*/
/* - namedcmd                                                         */
/* - nextarg                                                          */
/* - storeproc                                                        */
/* Via nextarg:                                                       */
/* - macarc                                                           */
/* Direct and via macarg                                              */
/* - docmd                                                            */
/* - dobuf   % Resets execstr                                         */
/*====================================================================*/


/*====================================================================*/
/* Call dependencies (only a subset):                                 */
/*====================================================================*
 *
 * - nextarg:
 *    eval.c:getval
 *
 * - macarg:
 *    nextarg
 *
 * - dobuf:
 *    docmd
 *    macarg
 *
 * - docmd:
 *    macarg
 *    eval.c:getval
 *
 * - eval.c:gtfun:
 *    dobuf
 *    eval.c:getval
 *
 * - eval.c:getval:
 *    eval.c:gtfun
 *    eval.c:getval
 *
 *====================================================================*/


/*====================================================================*
 * The global flag `execlevel' is set and used in dobuf() and used
 * in docmd().
 * - If .GT. 0 docmd() immediately returns so that code is only
 *   scanned but not evaluated which is used to go over conditional
 *   expressions which evaluate to FALSE.
 * - It is initialized to `0' at the start of dobuf().
 * - It should be initialized to `0' in any function calling docmd().
 *====================================================================*/


/*====================================================================*
 * Unfortuanetly FALSE/TRUE are not the only values that MicroEMACS
 * boolean valued functions return: When used interactively they may
 * also return values like `ABORT'.
 *
 * Just to be shure we define a macro here that remaps such results
 * depending on the interactive ( !clexec ) or non-interactive
 * ( clexec ) state.
 *
 * It should/may be used in the functions that may be called
 * interactively by the user like `execbuf', `execcmd', `execfile',
 * `execfunc' `execproc', `namedcmd'.
 *====================================================================*/
#define STATRET(x)  do  {                 \
    int x_  = (x);                        \
                                          \
    if ( x_ == TRUE || x_ == FALSE )  {   \
        return x_;                        \
    }                                     \
                                          \
    if ( clexec ) {                       \
        return FALSE;                     \
    } else        {                       \
        return x_;                        \
    }                                     \
} while ( 0 )
/**END_OF_DEFINITION**/
/*====================================================================*/


/*====================================================================*/
/* Module local functions                                             */
/*====================================================================*/
static VOID PASCAL NEAR freewhile_ DCL((WHBLOCK **wpp));
#define                 freewhile(wp)   ( freewhile_(&(wp)) )
static int PASCAL NEAR  debug DCL((BUFFER *bp, char *eline, int *skipflag));
/*====================================================================*/


/* NAMEDCMD:
 *
 * Execute a named command (builtin function) even if it is not bound
 */
int PASCAL NEAR namedcmd P2_(int, f, int, n)
/* command arguments [passed through to command executed] */
{
    ue_fnc_T  kfunc   = NULL;   /* ptr to the function to execute */
    BUFFER    *bp     = NULL;
    char      buffer[NSTRING];  /* buffer to store function name  */
    int       status  = 0;

    /* As this function is used for MicroEMACS command execution  */
    /* (bound to `M-x') all subcomands *must* be called in        */
    /* interactive mode to allow a user dialog so save the old    */
    /* clexec flag here:                                          */
    int       oldcle  = clexec;

    ZEROMEM(buffer);


    if ( clexec ) {
        /* ==================== Command Line mode: ================== */
        /* ---------------------------------------------------------- */

        /* grab token and advance past and evaluate token:  */
        if ( macarg(buffer, SIZEOF(buffer)) != TRUE ) {
            return FALSE;
        }

        /* and look it up: */
        if ( ( kfunc = fncmatch(buffer) ) == NULL ) {
            MTC(("`%s' is NOT a function", buffer));

            /* Is it a macro? Construct its buffer name and find the
             * pointer to that buffer:  */
            if ( ( bp = bfind(procbfn(buffer), FALSE, 0) ) == NULL )  {
                return FALSE;
            }
        }
    } else        {
        /* ==================== Interactive mode: =================== */
        /* ---------------------------------------------------------- */

        /* prompt the user to type a named command */
        /* and get the function name to execute */
        CONST char  *cp = getcmdname(": ");

        if ( NULL == cp ) {
            return FALSE;
        }
        BUFCPY(buffer, cp);
        if ( NULL == (kfunc = fncmatch(buffer)) ) {
            MTC(("`%s' is NOT a function", buffer));

            /* Is it a macro? Construct its buffer name and find the
             * pointer to that buffer:  */
            if ( ( bp = bfind(procbfn(buffer), FALSE, 0) ) == NULL )  {
                mlwrite(TEXT244, buffer);
/*                      "%%No such function as '%s'" */

                return FALSE;
            }
        }
    }


    /* ================= and now execute it as asked ================ */
    /* -------------------------------------------------------------- */


    if ( NULL != kfunc )  {
        /* if non-interactive .... force interactive execution        */
        clexec = FALSE;
        status = (*kfunc)(f, n);    /* call the function              */
        clexec = oldcle;            /* restore clexec flag            */

        if ( status != TRUE ) {
            STATRET(status);
        }
    } else                {
        while ( n-- > 0 ) {
            /* if non-interactive .... force interactive execution    */
            clexec = FALSE;
            status = dobuf(bp);     /* execute buffer                 */
            clexec = oldcle;        /* restore clexec flag            */

            if ( status != TRUE ) {
                STATRET(status);
            }
        }
    }

    return (TRUE);
}

/* EXECCMD:
 *
 * Execute a command line command to be typed in by the user.
 * - might be either a procedure (macro) or a function (builtin)
 *
 * This function executes a command line the same way as would be done
 * in macro line: e.g it is possible to type in
 *
 *  print &cat "The answer is " &add 40 2
 *
 */
int PASCAL NEAR execcmd P2_(int, f, int, n)
/* default Flag and Numeric argument  */
{
#define EXECCMD_PROMPT_ "===> "
    REGISTER int  status  = 0;          /* status return      */
    char          cmdstr[NSTRING];      /* command to execute */

    ZEROMEM(cmdstr);

    if ( clexec ) {   /* if we are non-interactive: */
        /* grab token and advance past and evaluate token:  */
        if ( macarg(cmdstr, SIZEOF(cmdstr)) != TRUE ) {
            return FALSE;
        }
    } else                {
        /* find out what command the user wants to execute: */
#if BEGIN_COMMENT_
      /* Old coding without command completion: */
      if ( (status = mlreply(EXECCMD_PROMPT_, cmdstr, SIZEOF(cmdstr)))
            != TRUE ) {
          STATRET(status);
      }
#else   /*END_COMMENT_*/
        CONST char  *cp = getcmdname(EXECCMD_PROMPT_);
        char        buf0[NSTRING];
        char        buf1[NSTRING];
        char        buf2[NSTRING];

        ZEROMEM(buf0);
        ZEROMEM(buf1);
        ZEROMEM(buf2);

        if ( NULL == cp ) {
            return FALSE;
        }
        BUFCPY(buf0, rtrimstr(ltrimstr(cp)));
# if BEGIN_COMMENT_
        xsnprintf(buf1, SIZEOF(buf1), "  [%s] Arguments: ", buf0);
# else  /*END_COMMENT_*/
        xsnprintf(buf1, SIZEOF(buf1), "%s%s ", EXECCMD_PROMPT_, buf0);
# endif
        if ( (status = mlreply(buf1, buf2, SIZEOF(buf2))) != TRUE ) {
            STATRET(status);
        }
        BUFCPY(buf2, rtrimstr(ltrimstr(buf2)));

        BUFCPY(cmdstr, buf0);
        if ( *buf0 && *buf2 ) {
            BUFCAT(cmdstr, " ");
        }
        BUFCAT(cmdstr, buf2);
#endif
    }

    /* and now execute it as asked */
    while ( n-- > 0 ) {
        int oldcle  = clexec;           /* save old clexec flag */

        clexec = TRUE;                  /* in cline execution */
        /*============================================================*
         * Call to docmd() --- initialize `execlevel' to 0:
         *============================================================*/
        execlevel = 0;
        /*============================================================*/
        status = docmd(cmdstr);
        clexec = oldcle;                /* restore clexec flag */

        if ( status != TRUE ) {
            STATRET(status);
        }
    }

    return (TRUE);
#undef  EXECCMD_PROMPT_
}

/* DOCMD:
 *
 * take a passed string as a command line and translate it to be
 * executed as a command. This function will be used by
 * execute-command-line and by all source and startup files.
 * Lastflag/thisflag is also updated.
 *
 * format of the command line is:
 *  {# arg} <command-name> {<argument string(s)>}
 *
 * <command-name> is first tried as a builtin function then as a user
 * defined procedure (macro).
 *
 * This function restores `execstr' to its old value!
 */
#if MTC_ON
# define MTC_docmd(e) do {                                    \
    CONST char  *msg_ = yasprintf e;                          \
                                                              \
    MTC(("docmd{%d}(`%s') [%12s:%04d]: %s",                   \
         docmd_CALL_LEVEL_, STR(cline), xbasenam(file),       \
         line, STR(msg_)));                                   \
    CLROOM(msg_);                                             \
} while ( 0 )
#else
# define MTC_docmd(e) NOOP
#endif
/**END_OF_DEFINITION**/
#define docmd_RET_INIT                                        \
      REGISTER int  docmd_RET_res_    = 0;                    \
      static int    docmd_CALL_LEVEL_ = 0;                    \
                                                              \
      do  {                                                   \
          docmd_CALL_LEVEL_++;                                \
      } while ( 0 )
/**END_OF_DEFINITION**/
#define docmd_RET_EXIT  do {                                  \
      docmd_RET_exit_:                                        \
          docmd_CALL_LEVEL_--;                                \
          MTC(("BEFORE   - execstr: `%s'", execstr));         \
          BUFCPY(execstr, oldestr);                           \
          MTC(("RESTORED - execstr: `%s'", execstr));         \
                                                              \
      return docmd_RET_res_;                                  \
  } while ( 0 )
/**END_OF_DEFINITION**/
#define docmd_RET(x)  do {                                    \
      docmd_RET_res_  = (x);                                  \
      goto docmd_RET_exit_;                                   \
  } while ( 0 )
/**END_OF_DEFINITION**/
int PASCAL NEAR docmd_ P3_(char *, cline /* command line to execute */,
                           CONST char *, file, int, line)
{
    REGISTER int  f           = 0;          /* default argument flag */
    REGISTER int  n           = 0;          /* numeric repeat value */
    ue_fnc_T      fnc         = NULL;       /* function to execute */
    BUFFER        *bp         = NULL;       /* buffer to execute */
    int           status      = 0;          /* return status of function */
    int           oldcle      = 0;          /* old contents of clexec flag */
    char          oldestr[SIZEOF(execstr)]; /* original exec string */
    char          tkn[NSTRING];             /* next token off of command line */

    docmd_RET_INIT;

    ZEROMEM(oldestr);
    ZEROMEM(tkn);

    /* if we are scanning and not executing ... go back here  */
    if ( execlevel )  {
        MTC_docmd(("Only scanning, execlevel = %d", execlevel));

        docmd_RET(TRUE);
    }

    ASRT(NULL != cline);

    MTC_docmd(("PREV execstr: `%s'", execstr));
    /* save last string to execute: */
    BUFCPY(oldestr, rtrimstr(ltrimstr(execstr)));
    /* and set this one as current: */
    BUFCPY(execstr, rtrimstr(ltrimstr(cline)));

    /* Push `execstr' if necessary, this allows constructs like
     * ```
     *   execute-command-line { execute-command-line } print "Hello World"
     * ```
     * in MicroEMACS macros
     */
    if ( *oldestr ) {
        MTC_docmd(("Push `%s' ---> `%s'", execstr, oldestr));
        BUFCAT(execstr, " ");
        BUFCAT(execstr, oldestr);
    }

    MTC_docmd(("NEW execstr: `%s'", execstr));
    /* first set up the default command values: */
    f = FALSE;
    n = 1;
    lastflag = thisflag;
    thisflag = 0;

    /* and grab the first token:  */
    if ( (status = macarg(tkn, SIZEOF(tkn))) != TRUE )  {
        docmd_RET(status);
    }
    MTC_docmd(("First token: `%s'", tkn));

    /* process leadin argument */
    if ( gettyp(tkn) != TKCMD ) {
        MTC_docmd(("First token: `%s' is not a TKCMD", tkn));
        f = TRUE;
        BUFCPY(tkn, fixnull(getval(tkn)));
        n = asc_int(tkn);

        /* and now get the command to execute */
        if ( (status = macarg(tkn, SIZEOF(tkn))) != TRUE )  {
            docmd_RET(status);
        }
    }
    MTC_docmd(("Command to execute: `%s'", tkn));

    /* and match the token to see if it exists: */
    if ( (fnc = fncmatch(tkn)) == NULL )  { /* Not a function */
        MTC_docmd(("`%s' is NOT a function", tkn));
        MTC_docmd(("execstr: `%s'", execstr));
        /* construct the buffer name */

        /* Is it a macro? Construct its buffer name and find the
         * pointer to that buffer:  */
        if ( (bp = bfind(procbfn(tkn), FALSE, 0)) == NULL ) { /* No */
            mlwrite(TEXT244, tkn);
/*                  "%%No such function as '%s'" */

            docmd_RET(FALSE);
        }

        /* execute the buffer */
        oldcle = clexec;                /* save old clexec flag */
        while ( n-- > 0 ) {
            MTC_docmd(("BEFORE(%s) - execstr: `%s'", "dobuf", execstr));
            clexec = TRUE;              /* in cline execution */
            status = dobuf(bp);
            clexec = oldcle;            /* restore clexec flag */
            MTC_docmd(("AFTER(%s)  - execstr: `%s'", "dobuf", execstr));
            if ( status != TRUE ) {
                break;
            }
	}

        cmdstatus = status;             /* save the status */

        docmd_RET(status);
    }


    /* ====================== It is a function ====================== */
    /* -------------------------------------------------------------- */

    MTC_docmd(("`%s' is a function", tkn));
    MTC_docmd(("execstr: `%s'", execstr));
    /* save the arguments and go execute the command */
    oldcle = clexec;                    /* save old clexec flag */
    clexec = TRUE;                      /* in cline execution */
    status = (*fnc)(f, n);              /* call the function */
    cmdstatus = status;                 /* save the status */
    clexec = oldcle;                    /* restore clexec flag */

    docmd_RET(status);


    docmd_RET_EXIT;
}
#undef  docmd_RET_INIT
#undef  docmd_RET_EXIT
#undef  docmd_RET

/* GETHEXDIGVAL:
 *
 * Get the value of a hexadecimal digit or return .LT. 0 if not a
 * hexadecimal digit.
 *
 * Used by TOKEN()
 */
static int  GetHexDigVal P1_(char, d)
{
    static int  FirstCall = 1;
    static int  digtab[0xFF/***(int)(unsigned char)(-1)***/ + 1];
# define SET_DIG_VAL_(d, v) digtab[(int)(unsigned char) (d)]  = (v)

    if ( FirstCall )  {
        int i = 0;

        for ( i = 0; i < NELEM(digtab); i++ ) {
            digtab[i] = (-1) * C_1;
        }
	SET_DIG_VAL_('0', 0);
	SET_DIG_VAL_('1', 1);
	SET_DIG_VAL_('2', 2);
	SET_DIG_VAL_('3', 3);
	SET_DIG_VAL_('4', 4);
	SET_DIG_VAL_('5', 5);
	SET_DIG_VAL_('6', 6);
	SET_DIG_VAL_('7', 7);
	SET_DIG_VAL_('8', 8);
	SET_DIG_VAL_('9', 9);

	SET_DIG_VAL_('a', 10);
	SET_DIG_VAL_('b', 11);
	SET_DIG_VAL_('c', 12);
	SET_DIG_VAL_('d', 13);
	SET_DIG_VAL_('e', 14);
	SET_DIG_VAL_('f', 15);

	SET_DIG_VAL_('A', 10);
	SET_DIG_VAL_('B', 11);
	SET_DIG_VAL_('C', 12);
	SET_DIG_VAL_('D', 13);
	SET_DIG_VAL_('E', 14);
	SET_DIG_VAL_('F', 15);

        FirstCall = 0;
    }

    return digtab[(unsigned char)d];
#undef  SET_DIG_VAL_
}

/* TOKEN:
 *
 * Chop a token off a string return a pointer past the token.
 */
char * PASCAL NEAR token P3_(
        char *, src,  /* source string */
        char *, tok,  /* destination token string */
        int,    size  /* maximum size of token */
    )
{
    REGISTER int  quotef  = 0;      /* is the current string quoted? */
    REGISTER char c       = '\0';   /* temporary character */

    /* first scan past any whitespace in the source string */
    while ( *src == ' ' || *src == '\t' ) {
        ++src;
    }

    /* scan through the source string */
    quotef = FALSE;
    while ( *src ) {
        /* process special characters */
        if ( *src == '~' ) {
            ++src;
            if ( *src == 0 )  {
                break;
            }
            switch ( *src++ ) {
                case 'r':
                    c = 13;
                    break;

                case 'n':
                    c = 13;
                    break;

                case 'l':
                    c = 10;
                    break;

                case 't':
                    c = 9;
                    break;

                case 'b':
                    c = 8;
                    break;

                case 'f':
                    c = 12;
                    break;

                case 'e':
                    c = 27;
                    break;

                case 'x':
                    {
                        /* Exactly two hexadecimal digits following `~x' are
                         * evaluated to the character code:
                         * ~xUV ===> 0xUV, e.g. ~x20 ===> 0x20 ===> ' '.
                         */
                        int d0  = 0;
                        int d1  = 0;

                        if ( 0 <= (d0 = GetHexDigVal(*src)) &&
                             0 <= (d1 = GetHexDigVal(*(src + 1))) ) {
                            unsigned char val = 0;

                            val = C_16 * d0 + d1;
                            c   = *(char *)&val;
                            src  += 2;
                        } else                                      {
                            c = *(src - 1);
                        }
                    }
                    break;

                default:
                    c = *(src - 1);
            }
            if ( --size > 0 ) {
                *tok++ = c;
            }
        } else {
            /* check for the end of the token */
            if ( quotef ) {
                /* A string `"XYZ"' is returned here as `"XYZ': The
                 * gettyp() and getval() routines in eval.c expect it
                 * in this way. This way the first character of a token
                 * marks its type:
                 * - `&': Function
                 * - `!': Directive
                 * - `%': Variable
                 * - `"': String Literal
                 */
                if ( *src == '"' )  {
                    break;
                }
            } else {
                if ( *src == ' ' || *src == '\t' )  {
                    break;
                }
            }

            /* set quote mode if quote found */
            if ( *src == '"' )  {
                quotef = TRUE;
            }

            /* record the character */
            c = *src++;
            if ( --size > 0 ) {
                *tok++ = c;
            }
        }
    }

    /* terminate the token and exit */
    if ( *src ) {
        ++src;
    }
    *tok = '\0';

    return (src);
}

/* MACARG:
 *
 * Get a macro line argument.
 */
int PASCAL NEAR macarg P2_(char *, tok  /* buffer to place argument */,
                           int, size    /* size of buffer */)
{
    int savcle  = 0;    /* buffer to store original clexec */
    int status  = 0;

    savcle = clexec;            /* save execution mode */
    clexec = TRUE;              /* get the argument */
    status = nextarg("", tok, size, ctoec('\r'));
    clexec = savcle;            /* restore execution mode */

    return status;
}

/* NEXTARG:
 *
 * Get the next argument.
 */
int PASCAL NEAR nextarg P4_(
        CONST char *, prompt,     /* prompt to use if we must be interactive */
        char *,       buffer,     /* buffer to put token into */
        int,          size,       /* size of the buffer */
        int,          terminator  /* terminating char to be used on interactive fetch */
    )
{
    REGISTER CONST char *sp = NULL; /* return pointer from getval() */

    /* if we are interactive, go get it! */
    if ( ! clexec ) {
        int rc  = 0;

        /* prompt the user for the input string */
        if ( discmd ) {
            if ( prompt ) {
                mlwrite(prompt);
            }
        } else        {
            movecursor(term.t_nrow, 0);
        }
        terminchr = terminator;
        rc  = getstring((unsigned char *)buffer, size, terminator);
        terminchr = '\0';

        return rc;
    }

    /* grab token and advance past */
    MTC(("BEFORE(token) - execstr: `%s'", execstr));
    BUFCPY(execstr, token(execstr, buffer, size));
    MTC(("AFTER(token)  - execstr: `%s', buffer: `%s'", execstr, buffer));

    /* evaluate it */
    if ( NULL == (sp = getval(buffer)) )  {
        return (FALSE);
    }
    xstrlcpy(buffer, sp, size);
    MTC(("AFTER(getval)  - buffer: `%s'", buffer));

    return (TRUE);
}

/* STOREPROC:
 *
 * Set up a procedure buffer and flag to store all executed command
 * lines there.
 */
int PASCAL NEAR storeproc P2_(
        int, f, /* default flag */
        int, n  /* macro number to use */
    )
{
    REGISTER struct BUFFER  *bp       = NULL;   /* pointer to macro buffer  */
    PARG                    *last_arg = NULL;   /* last macro argument      */
    PARG                    *cur_arg  = NULL;   /* current macro argument   */
    char buffer[NSTRING];                       /* token buffer             */

    ZEROMEM(buffer);

    /* this commands makes no sense interactively */
    if ( ! clexec ) {
        return (FALSE);
    }

    /* get the name of the procedure */
    MTC(("BEFORE(name of procedure) - execstr: `%s'", execstr));
    BUFCPY(execstr, token(execstr, buffer, SIZEOF(buffer)));
    MTC(("AFTER(name of procedure)  - execstr: `%s', buffer: `%s'",
         execstr, buffer));

    /* construct macro buffer name and set up the new macro buffer: */
    if ( (bp = bfind(procbfn(buffer), TRUE, BFINVS)) == NULL )  {
        mlwrite(TEXT113);
        /* "Can not create macro" */

        return (FALSE);
    }

    /* and make sure it is empty */
    bclear(bp);

    /* retrieve and store any formal parameters */
    last_arg = (PARG *)NULL;
    bp->b_numargs = 0;
    MTC(("BEFORE - execstr: `%s'", execstr));
    BUFCPY(execstr, token(execstr, buffer, SIZEOF(buffer)));
    MTC(("AFTER  - execstr: `%s', buffer: `%s'", execstr, buffer));

    while ( *buffer && *buffer != ';' ) {
        /* allocate an argument */
        cur_arg = (PARG *)ROOM( SIZEOF (PARG) );
        if ( cur_arg == (PARG *)NULL ) {
            mlwrite(TEXT113);
            /* "Can not create macro" */

            return (FALSE);
        }

        /* and add it to the linked list of arguments for this buffer */
        BUFCPY(cur_arg->name, buffer);
        cur_arg->next = (PARG *)NULL;
        if ( last_arg == (PARG *)NULL ) {
            bp->b_args = cur_arg;
        } else                          {
            last_arg->next = cur_arg;
        }

        /* and let the buffer total these */
        bp->b_numargs++;
        last_arg = cur_arg;

        /* on to the next parameter */
        MTC(("BEFORE - execstr: `%s'", execstr));
        BUFCPY(execstr, token(execstr, buffer, SIZEOF(buffer)));
        MTC(("AFTER  - execstr: `%s', buffer: `%s'", execstr, buffer));
    }

    /* and set the macro store pointers to it */
    mstore = TRUE;
    bstore = bp;  /* This is the *only* place where bstore is set */

    return (TRUE);
}

/* EXECPROC:
 *
 * Execute a procedure (macro).
 */
int PASCAL NEAR execproc P2_(int, f, int, n)
/* default flag and numeric arg */
{
    REGISTER BUFFER *bp     = NULL;   /* ptr to buffer to execute     */
    REGISTER int    status  = 0;      /* status return                */
    char            procn[NBUFN];     /* name of procedure to execute */

    ZEROMEM(procn);

    if ( clexec ) {                     /* if we are non-interactive: */
        /* grab token and advance past and evaluate token:  */
        if ( macarg(procn, SIZEOF(procn)) != TRUE ) {
            return FALSE;
        }
    } else                {
        /* find out what procedure the user wants to execute: */
        /* getprcname(): Completion with SPACE*/
#if BEGIN_COMMENT_
        CONST char  *cp = getprcname(TEXT115);
/*                                   "Execute procedure: " */
#else   /*END_COMMENT_*/
        CONST char  *cp = getprcname("P?> ");
#endif

        if ( NULL == cp ) {
            return FALSE;
        }
        BUFCPY(procn, cp);
    }

    /* construct the buffer name and find the pointer to that buffer: */
    if ( NULL == (bp = bfind(procbfn(procn), FALSE, 0)) ) {
        char  buffer[NSTRING];

        ZEROMEM(buffer);

        if ( ! clexec ) {
            BUFCPY(buffer, TEXT116);
/*                         "No such procedure" */
            BUFCAT(buffer, ": `");
            BUFCAT(buffer, procn);
            BUFCAT(buffer, "'");
            mlwrite(buffer);
        }

        return (FALSE);
    }

    /* and now execute it as asked */
    while ( n-- > 0 ) {
        int oldcle  = clexec;           /* save old clexec flag */

        clexec = TRUE;                  /* in cline execution */
        status = dobuf(bp);
        clexec = oldcle;                /* restore clexec flag */

        if ( status != TRUE ) {
            STATRET(status);
        }
    }

    return TRUE;
}

/* EXECFUNC:
 *
 * Execute a (builtin) function.
 */
int PASCAL NEAR execfunc P2_(int, f, int, n)
/* default flag and numeric arg */
{
    ue_fnc_T      kfunc   = NULL;   /* ptr to the function to execute */
    REGISTER int  status  = 0;      /* status return                  */
    REGISTER int  oldcle  = clexec; /* Org exec mode (interact./CLI)  */
    char          funcn[NBUFN];     /* name of procedure to execute   */

    ZEROMEM(funcn);

    if ( clexec ) {                     /* if we are non-interactive: */
        /* grab token and advance past and evaluate token:  */
        if ( macarg(funcn, SIZEOF(funcn)) != TRUE ) {
            return FALSE;
        }
    } else                {
        /* find out what function the user wants to execute: */
        /* getfncname(): Completion with SPACE*/
#if BEGIN_COMMENT_
        CONST char  *cp = getfncname(TEXT115);
/*                                   "Execute procedure: " */
#else   /*END_COMMENT_*/
        CONST char  *cp = getfncname("F?> ");
#endif

        if ( NULL == cp ) {
            return FALSE;
        }
        BUFCPY(funcn, cp);
    }

    /* and look it up: */
    if ( (kfunc = fncmatch(funcn)) == NULL )  {
        MTC(("%%No such function as '%s'", funcn));
        if ( ! clexec ) {
            mlwrite(TEXT244, funcn);
/*                  "%%No such function as '%s'" */
        }

        return FALSE;
    }

    /* if interactive .... force non-interactive execution  */
    clexec  = TRUE;
    status  = (*kfunc)(f, n);   /* call the function    */
    clexec  = oldcle;           /* restore clexec flag  */

    if ( status != TRUE ) {
        STATRET(status);
    }

    return TRUE;
}

/* EXECBUF:
 *
 * Execute the contents of a buffer of commands.
 */
int PASCAL NEAR execbuf P2_(int, f, int, n)
/* default flag and numeric arg */
{
    REGISTER BUFFER *bp     = NULL;     /* ptr to buffer to execute */
    REGISTER int    status  = 0;        /* status return */
    char            bufn[NBUFN];

    ZEROMEM(bufn);

    if ( clexec ) {                     /* if we are non-interactive: */
        /* grab token and advance past and evaluate token:  */
        if ( macarg(bufn, SIZEOF(bufn)) != TRUE ) {
            return FALSE;
        }
    } else        {
        /* prompt the user to type the name of the buffer to execute: */
        /* getbufname(): Completion with SPACE*/
        BUFCPY(bufn, getbufname(TEXT117));
/*                              "Execute buffer: " */
    }

    /* find the pointer to that buffer: */
    if ( NULL == (bp = bfind(bufn, FALSE, 0)) ) {
        char  buffer[NSTRING];

        ZEROMEM(buffer);

        if ( ! clexec ) {
            BUFCPY(buffer, TEXT118);
/*                         "No such buffer" */
            BUFCAT(buffer, ": `");
            BUFCAT(buffer, bufn);
            BUFCAT(buffer, "'");
            mlwrite(buffer);
        }

        if ( clexec ) {
            return FALSE;
        } else        {
            return ABORT;
        }
    }

    /* and now execute it as asked */
    while ( n-- > 0 ) {
        int oldcle  = clexec;           /* save old clexec flag */

        clexec = TRUE;                  /* in cline execution */
        status = dobuf(bp);
        clexec = oldcle;                /* restore clexec flag */

        if ( status != TRUE ) {
            STATRET(status);
        }
    }

    return (TRUE);
}

/* DOBUF:
 *
 * Execute the contents of the buffer pointed to by the passed BP
 *
 * Directives start with a "!" and include:
 *
 *  !endm           End a macro
 *  !if (cond)      conditional execution
 *  !else
 *  !endif
 *  !return <rval>  Return (terminating current macro/
 *                  set $rval to and return <rval>)
 *  !goto <label>   Jump to a label in the current macro
 *  !force          Force macro to continue...even if command fails
 *  !while (cond)   Execute a loop if the condition is true
 *  !endwhile
 *
 * Line Labels begin with a "*" as the first nonblank char, like:
 *
 *  *LBL01
 */
#if MTC_ON
# define MTC_dobuf(e) do {                                    \
    CONST char  *msg_ = yasprintf e;                          \
                                                              \
    MTC(("dobuf(`%s') [%12s:%04d]: %s",                       \
         bp->b_bname, xbasenam(file), line, STR(msg_)));      \
    CLROOM(msg_);                                             \
} while ( 0 )
#else
# define MTC_dobuf(e) NOOP
#endif
/**END_OF_DEFINITION**/
#define dobuf_RET_INIT                                        \
      REGISTER int  dobuf_RET_res_  = 0;                      \
                                                              \
      do  {                                                   \
          /* clear IF level flags/while ptr */                \
          execlevel = 0;                                      \
      } while ( 0 )
/**END_OF_DEFINITION**/
#define dobuf_RET_EXIT  do {                                  \
      dobuf_RET_exit_:                                        \
      MTC_dobuf(("%s", "  END"));                             \
      execlevel = 0;                                          \
      freewhile(whlist);                                      \
                                                              \
      return dobuf_RET_res_;                                  \
  } while ( 0 )
/**END_OF_DEFINITION**/
#define dobuf_RET(x)  do {                                    \
      dobuf_RET_res_  = (x);                                  \
      goto dobuf_RET_exit_;                                   \
  } while ( 0 )
/**END_OF_DEFINITION**/
int PASCAL NEAR dobuf_ P3_(BUFFER *, bp /* buffer to execute */,
                           CONST char *, file, int, line)
{
    REGISTER int  status      = 0;    /* status return */
    REGISTER LINE *lp         = NULL; /* pointer to line to execute */
    REGISTER LINE *hlp        = NULL; /* pointer to line header */
    REGISTER LINE *glp        = NULL; /* line to goto */
    LINE          *mp         = NULL; /* Macro line storage temp */
    int           dirnum      = 0;    /* directive index */
    int           linlen      = 0;    /* length of line to execute */
    int           i           = 0;    /* index */
    int           force       = 0;    /* force TRUE result? */
    EWINDOW       *wp         = NULL; /* ptr to windows to scan */
    WHBLOCK       *whlist     = NULL; /* ptr to !WHILE list */
    WHBLOCK       *scanner    = NULL; /* ptr during scan */
    WHBLOCK       *whtemp     = NULL; /* temporary ptr to a WHBLOCK */
    char          *einit      = NULL; /* initial value of eline */
    char          *eline      = NULL; /* text of line to execute */
    char          tkn[NSTRING];       /* buffer to evaluate an expresion in */
    int           num_locals  = 0;    /* number of local variables used in
                                       * procedure  */
    UTABLE        *ut         = NULL; /* new local user variable table */
    int           skipflag    = 0;    /* are we skipping debugging a
                                       * function?  */
    PARG          *cur_arg    = NULL; /* current argument being filled */
    int           cur_index   = 0;    /* index into current user table */
    VDESC         vd;                 /* variable num/type */
    char          value[NSTRING];     /* evaluated argument */
    char          dtv_str[NSTRING];   /* String following a directive */
    char          sav_estr[NSTRING];  /* `execstr' before directive */

    dobuf_RET_INIT;

    ZEROMEM(tkn);
    ZEROMEM(vd);
    ZEROMEM(value);
    ZEROMEM(dtv_str);
    ZEROMEM(sav_estr);

    ASRT(NULL != bp);
    ASRT(NULL != file);
    ASRT(0 <= line);

    MTC_dobuf(("%s", "BEGIN"));

    /* If necessary activate the buffer:  */
    if ( ! bp->b_active ) {   /* buffer not active yet  */
        MTC_dobuf(("%s", "Activate buffer"));
        /* read it in and activate it */
        readinx(bp->b_fname, ((bp->b_mode & MDVIEW) == 0), bp, FALSE);
        bp->b_dotp = lforw(bp->b_linep);
        set_b_doto(bp, 0);
        bp->last_access = access_time;
        bp->b_active = TRUE;
    }

    /* flag we are executing the buffer */
    bp->b_exec += 1;

    /* we are not skipping a function yet (for the debugger) */
    skipflag = FALSE;

    /* scan the buffer to execute, building WHILE header blocks and counting
     * local variables */
    hlp = bp->b_linep;
    lp = lforw(hlp);
    while ( lp != hlp ) {
        /* scan the current line */
        eline = ltext(lp);
        MTC_dobuf(("eline: `%s'", STR(eline)));
        i = get_lused(lp);

        /* trim leading whitespace */
        while ( i > 0 && (*eline == ' ' || *eline == '\t') ) {
            i--;
            ++eline;
        }

        /* if theres nothing here, don't bother */
        if ( i <= 0 ) {
            goto nxtscan;
        }

        /* if we are already in a stored-procedure */
        if ( mstore ) {
            if ( strncmp(eline, "!endm", 5) == 0 )  {
                mstore = FALSE;
            }

            goto nxtscan;
        }

        /* stored procedure? */
        if ( strncmp(eline, "store-procedure", 15) == 0 ) {
            mstore = TRUE;

            goto nxtscan;
        }

        /* local variable declaration? */
        if ( strncmp(eline, "local", 5) == 0 )  {
            ++num_locals;
        }

        /* if is a while directive, make a block... */
        if ( eline[0] == '!' && eline[1] == 'w' && eline[2] == 'h' ) {
            whtemp = (WHBLOCK *)ROOM( SIZEOF (WHBLOCK) );
            if ( whtemp == NULL ) {
noram:          errormesg(TEXT119, bp, lp);
/*                                        "%%Out of memory during while scan" */
failexit:       freewhile(scanner);

                goto eabort;
            }
            whtemp->w_begin = lp;
            whtemp->w_type = BTWHILE;
            whtemp->w_next = scanner;
            scanner = whtemp;
        }

        /* if is a BREAK directive, make a block... */
        if ( eline[0] == '!' && eline[1] == 'b' && eline[2] == 'r' ) {
            if ( scanner == NULL ) {
                errormesg(TEXT120, bp, lp);
/*                                        "%%!BREAK outside of any !WHILE loop"
 */

                goto failexit;
            }
            if ( NULL == (whtemp = (WHBLOCK *)ROOM( SIZEOF (WHBLOCK))) )  {
                goto noram;
            }
            whtemp->w_begin = lp;
            whtemp->w_type = BTBREAK;
            whtemp->w_next = scanner;
            scanner = whtemp;
        }

        /* if it is an endwhile directive, record the spot... */
        if ( eline[0] == '!' && strncmp(&eline[1], "endw", 4) == 0 ) {
            if ( scanner == NULL ) {
                errormesg(TEXT121, bp, lp);
/*                                      "%%!ENDWHILE with no preceding !WHILE"
 */

                goto failexit;
            }
            /* move top records from the scanner list to the whlist until we
             * have moved all BREAK records and one WHILE record */
            do {
                scanner->w_end = lp;
                whtemp = whlist;
                whlist = scanner;
                scanner = scanner->w_next;
                whlist->w_next = whtemp;
            } while (whlist->w_type == BTBREAK);
        }

nxtscan:        /* on to the next line */
        lp = lforw(lp);
    }

    /* while and endwhile should match! */
    if ( scanner != NULL ) {
        errormesg(TEXT122, bp, lp);
/*                      "%%!WHILE with no matching !ENDWHILE" */

        goto failexit;
    }

    /* let the first command inherit the flags from the last one..*/
    thisflag = lastflag;

    /* remember we need room for the procedure arguments among the locals */
    if ( bp->b_numargs == NOTPROC ) {
        bp->b_numargs = 0;
    }
    num_locals += bp->b_numargs;

    /* allocate a local user variable table */
    ut = (UTABLE *)ROOM( SIZEOF (UTABLE) + num_locals * SIZEOF (UVAR) );
    if ( ut == (UTABLE *)NULL ) {
        errormesg("%%Out of memory allocating locals", bp, lp);
        bp->b_exec -= 1;

        dobuf_RET(FALSE);
    }
    ut->next = uv_head;
    ut->size = num_locals;
    ut->bufp = bp;
    uv_init(ut);
    uv_head = ut;

    /* and evaluate the arguments passed, placing them in the local
     * variable table */
    cur_index = 0;
    cur_arg = bp->b_args;
    while ( cur_arg != (PARG *)NULL ) {
        /* ask for argument names */
        if ( (status = mlreply("Argument: ", value, NSTRING)) != TRUE ) {
            dobuf_RET(status);
        }

        /* and create and set these in the local user var table */
        findvar(cur_arg->name, &vd, NVSIZE + 1, VT_LOCAL);
        svar(&vd, value);

        /* on to the next argument */
        cur_arg = cur_arg->next;
        cur_index++;
    }

    /* starting at the beginning of the buffer */
    hlp = bp->b_linep;
    lp = lforw(hlp);
    while ( lp != hlp && eexitflag == FALSE ) {
        /* allocate eline and copy macro line to it */
        linlen = get_lused(lp);
        if ( ( einit = eline = ROOM(linlen+1) ) == NULL ) {
            errormesg(TEXT123, bp, lp);
/*                              "%%Out of Memory during macro execution" */
            bp->b_exec -= 1;

            goto freeut;
        }
        bytecopy(eline, ltext(lp), linlen);
        /* eline[linlen] = 0; /o make sure it ends:
         * Handled by bytecopy o/ */

        /* trim leading whitespace */
        while ( *eline == ' ' || *eline == '\t' ) {
            ++eline;
        }

        /* dump comments and blank lines */
        if ( *eline == ';' || *eline == '\0' )  {
            goto onward;
        }

        /* append the current command to the log file */
        MTC_dobuf(("eline: `%s'", STR(eline)));
        /* only do this if we are debugging */
        if ( macbug && !mstore && (execlevel == 0) )  {
            if ( debug(bp, eline, &skipflag) == FALSE ) {
                errormesg(TEXT54, bp, lp);
/*                                      "[Macro aborted]" */

                goto eabort;
            }
        }

        /* Parse directives here.... */
        dirnum = -1;
        if ( *eline == '!' ) {
            /* Find out which directive this is */
            ++eline;
            MTC_dobuf(("Directive: `%s'", STR(eline - 1)));
            for ( dirnum = 0; dirnum < NUMDIRS; dirnum++ )  {
                if ( strncmp(eline, dname[dirnum], dname_len[dirnum]) == 0 )  {
                    break;
                }
            }

            /* and bitch if it's illegal */
            if ( dirnum == NUMDIRS ) {
                errormesg(TEXT124, bp, lp);
/*                                      "%%Unknown Directive" */

                goto eabort;
            }

            /* service only the !ENDM macro here */
            if ( dirnum == DENDM ) {
                mstore = FALSE;
                bstore = NULL;

                goto onward;
            }

            /* restore the original eline....*/
            --eline;
        }

        /* if macro store is on, just salt this away */
        if ( mstore ) {
            /* allocate the space for the line */
            linlen = STRLEN(eline);
            if ( ( mp=lalloc(linlen) ) == NULL ) {
                errormesg(TEXT125, bp, lp);
/*                                      "Out of memory while storing macro" */

                goto eabort;
            }

            /* copy the text into the new line */
            for ( i=0; i < linlen; ++i )  {
                lputc(mp, i, eline[i]);
            }

            /* attach the line to the end of the buffer */
            if ( NULL == bstore ) {
                errormesg(TEXT113, bp, lp);
/*                                      "Can not create macro"  */

                goto eabort;
            }
            bstore->b_linep->l_bp->l_fp = mp;
            mp->l_bp = bstore->b_linep->l_bp;
            bstore->b_linep->l_bp = mp;
            mp->l_fp = bstore->b_linep;

            goto onward;
        }

        force = FALSE;

        /* dump comments */
        if ( *eline == '*' )  {
            goto onward;
        }

        /* now, execute directives */
        if ( dirnum != -1 ) {
            MTC_dobuf(("Directive: `%s'", STR(eline)));
            /* skip past the directive */
            while ( *eline && *eline != ' ' && *eline != '\t' ) {
                ++eline;
            }

            /*** TODO: Really understand this coding: Unfortunatey  ***/
            /***       my changes have been `debugged into life'.   ***/
            /***                                                    ***/
            /***       The original code simply replaced `execstr'  ***/
            /***       with dtv_str, which made                     ***/
            /***       `&cat &call <Macro> 42' discard the          ***/
            /***       trailing '42' --- in general everything      ***/
            /***       after <Macro>'s arguments is discarded.      ***/
            /***                                                    ***/
            /***       Joachim Schneider, Dec. 2025                 ***/
            BUFCPY(dtv_str, rtrimstr(ltrimstr(eline)));
            MTC_dobuf(("String following directive: `%s'", dtv_str));
            if ( ! *sav_estr )  {
                BUFCPY(sav_estr, rtrimstr(ltrimstr(execstr)));
                MTC_dobuf(("Saved `execstr': `%s'", sav_estr));
            }

            MTC_dobuf(("Old execstr: `%s'", execstr));
            BUFCPY(execstr, dtv_str);
            if ( *sav_estr )  {
                if ( *execstr ) {
                    BUFCAT(execstr, " ");
                }
                BUFCAT(execstr, sav_estr);
            }
            MTC_dobuf(("New execstr: `%s'", execstr));

            switch ( dirnum ) {
                case DIF:                   /* IF directive */
                    /* grab the value of the logical exp */
                    if ( execlevel == 0 ) {
                        if ( macarg(tkn, SIZEOF(tkn)) != TRUE ) {
                            CLROOM(einit);

                            goto eexec;
                        }
                        if ( ! stol(tkn) )  {
                            ++execlevel;
                        }
                    } else  {
                        ++execlevel;
                    }

                    goto onward;

                case DWHILE:                /* WHILE directive */
                    /* grab the value of the logical exp */
                    if ( execlevel == 0 ) {
                        if ( macarg(tkn, SIZEOF(tkn)) != TRUE ) {
                            CLROOM(einit);

                            goto eexec;
                        }
                        if ( stol(tkn) )  {
                            goto onward;
                        }
                    }
                /* drop down and act just like !BREAK */

                case DBREAK:                /* BREAK directive */
                    if ( dirnum == DBREAK && execlevel )  {
                        goto onward;
                    }

                    /* jump down to the endwhile */
                    /* find the right while loop */
                    whtemp = whlist;
                    while ( whtemp ) {
                        if ( whtemp->w_begin == lp )  {
                            break;
                        }
                        whtemp = whtemp->w_next;
                    }

                    if ( whtemp == NULL ) {
                        errormesg(TEXT126, bp, lp);
/*                                              "%%Internal While loop error" */

                        goto eabort;
                    }

                    /* reset the line pointer back.. */
                    lp = whtemp->w_end;

                    goto onward;

                case DELSE:                 /* ELSE directive */
                    if        ( execlevel == 1 )  {
                        --execlevel;
                    } else if ( execlevel == 0 )  {
                        ++execlevel;
                    }

                    goto onward;

                case DENDIF:                /* ENDIF directive */
                    if ( execlevel )  {
                        --execlevel;
                    }

                    goto onward;

                case DGOTO:                 /* GOTO directive */
                    /* .....only if we are currently executing */
                    if ( execlevel == 0 ) {
#if WINDOW_MSWIN
                        longop(TRUE);
#endif
                        /* grab label to jump to */
                        MTC_dobuf(("eline: `%s'", STR(eline)));
                        eline = token(eline, golabel, SIZEOF(golabel));
                        MTC_dobuf(("golabel: `%s'", STR(golabel)));
                        linlen = STRLEN(golabel);
                        glp = lforw(hlp);
                        while ( glp != hlp ) {
                            if ( (get_lused(glp) >= linlen) &&
                                 (lgetc(glp, 0) == '*') &&
                                 (strncmp( ( (char *)ltext(glp) ) + 1, golabel,
                                           linlen ) == 0) ) {
                                lp = glp;

                                goto onward;
                            }
                            glp = lforw(glp);
                        }
                        errormesg(TEXT127, bp, lp);
/*                                              "%%No such label" */

                        goto eabort;
                    }

                    goto onward;

                case DRETURN:               /* RETURN directive */
                    /* if we are executing.... */
                    if ( execlevel == 0 ) {
                        /* check for a return value */
                        if ( macarg(tkn, SIZEOF(tkn)) == TRUE ) {
                            BUFCPY(rval, tkn);
                            MTC_dobuf(("rval: `%s'", fixnull(rval)));
                        }

                        /* and free the line resources */
                        CLROOM(einit);

                        goto eexec;
                    }

                    goto onward;

                case DENDWHILE:             /* ENDWHILE directive */
                    if ( execlevel ) {
                        --execlevel;

                        goto onward;
                    } else {
#if WINDOW_MSWIN
                        longop(TRUE);
#endif
                        /* find the right while loop */
                        whtemp = whlist;
                        while ( whtemp ) {
                            if ( whtemp->w_type == BTWHILE &&whtemp->w_end == lp )  {
                                break;
                            }
                            whtemp = whtemp->w_next;
                        }

                        if ( whtemp == NULL ) {
                            errormesg(TEXT126, bp, lp);
                            /* "%%Internal While loop error"  */

                            goto eabort;
                        }

                        /* reset the line pointer back.. */
                        lp = lback(whtemp->w_begin);

                        goto onward;
                    }

                case DFORCE:                /* FORCE directive */
                    force = TRUE;
            }
        }

        /* execute the statement */
        status = docmd(eline);
        if ( force )  {
            status = TRUE;  /* force the status */
        }
        /* append the current command to the log file */
        MTC_dobuf(("%s", ". . . done"));

        /* check for a command error */
        if ( status != TRUE ) {
            /* look if buffer is showing */
            wp = wheadp;
            while ( wp != NULL ) {
                if ( wp->w_bufp == bp ) {
                    /* and point it */
                    wp->w_dotp = lp;
                    set_w_doto(wp, 0);
                    wp->w_flag |= WFHARD;
                }
                wp = wp->w_wndp;
            }

            /* in any case set the buffer . */
            bp->b_dotp = lp;
            set_b_doto(bp, 0);

            errormesg(TEXT219, bp, lp);
/*                "%%Macro Failed" */

            bp->b_exec -= 1;
            CLROOM(einit);

            /* discard the local user variable table */
            uv_head = ut->next;
            uv_clean(ut);
            CLROOM(ut);

            dobuf_RET(status);
        }

onward: /* on to the next line */
        CLROOM(einit);
        lp = lforw(lp);
        if ( skipflag ) {
            macbug = TRUE;
        }
    }

eexec:  /* exit the current function */
    bp->b_exec -= 1;

    /* discard the local user variable table */
    uv_head = ut->next;
    uv_clean(ut);
    CLROOM(ut);

    dobuf_RET(TRUE);

eabort: /* exit the current function with a failure */
    bp->b_exec -= 1;
    CLROOM(einit);

    /* discard the local user variable table */
freeut: uv_head = ut->next;
    uv_clean(ut);
    CLROOM(ut);

    dobuf_RET(FALSE);


    dobuf_RET_EXIT;
}
#undef  MTC_dobuf
#undef  dobuf_RET_INIT
#undef  dobuf_RET_EXIT
#undef  dobuf_RET

/* ERRORMESG:
 *
 * Display a macro execution error along with the buffer and line
 * currently being executed.
 */
VOID PASCAL NEAR errormesg P3_(CONST char *, mesg, BUFFER *, bp, LINE *, lp)
/* mesg:  Error message to display        */
/* bp:    Buffer error occured in   AAAA  */
/* lp:    Line                            */
{
    char buf[NSTRING];

    ZEROMEM(buf);

    exec_error = TRUE;

    /* build error message line */
    BUFCPY(buf, "\n");
    BUFCAT(buf, mesg);
    BUFCAT(buf, rtrimstr(TEXT229));
/*                       " in < " */
    BUFCAT(buf, bp->b_bname);
    BUFCAT(buf, TEXT230);
/*              "> at line " */
    BUFCAT(buf, long_asc( getlinenum(bp, lp)));
    mlforce(buf);

    return;
}

/* INTERACTIVE DEBUGGER
 *
 * If $debug == TRUE, the interactive debugger is invoked.
 *
 * Commands are listed out with the ? key.
 */
int PASCAL NEAR debug P3_(
        BUFFER *, bp,       /* buffer to execute          */
        char *,   eline,    /* text of line to debug      */
        int *,    skipflag  /* are we skipping debugging? */
    )
{
    REGISTER int    oldcmd          = 0;    /* original command display flag  */
    REGISTER int    oldinp          = 0;    /* original connamd input flag    */
    REGISTER int    oldelevel       = 0;    /* original execlevel             */
    REGISTER int    oldstatus       = 0;    /* status of last command         */
    REGISTER int    c               = '\0'; /* temp character                 */
    REGISTER KEYTAB *key            = NULL; /* ptr to a key entry             */
    static char     track[NSTRING]  = "";   /* expression to track value of   */
    char            temp[NSTRING];          /* command or expression          */

    ZEROMEM(temp);

dbuild: /* Build the information line to be presented to the user */

    BUFCPY(outline, "<<<");

    /* display the tracked expression */
    if ( track[0] != 0 ) {
        oldstatus = cmdstatus;
        /** TODO: Is `execlevel' handling correct here?           **/
        /**       In the old coding it wasn't saved/restored.     **/
        /**       ===> The old coding lets execlevel *unchanged*  **/
        oldelevel = execlevel;
        execlevel = 0;
        docmd(track);
        execlevel = oldelevel;
        cmdstatus = oldstatus;
        BUFCAT(outline, "[=");
        BUFCAT(outline, gtusr("track"));
        BUFCAT(outline, "]");
    }

    /* debug macro name */
    BUFCAT(outline, bp->b_bname);
    BUFCAT(outline, ":");

    /* and lastly the line */
    BUFCAT(outline, eline);
    BUFCAT(outline, ">>>");

    /* write out the debug line */
dinput: outline[MIN2(SIZEOF(outline) - 1, MAX2(0, term.t_ncol - 1))] = '\0';
    mlforce(outline);
    update(TRUE);

    /* and get the keystroke */
    c = get_key();

    /* ignore the mouse here */
    if ( c & MOUS ) {
        goto dinput;
    }

    /* META key turns off debugging */
    key = getbind(c);
    if        ( key                     &&
                key->k_type == BINDFNC  &&
                key->k_ptr.fp == f_meta )   {
        macbug = FALSE;
    } else if ( c == abortc )               {
        return (FALSE);
    } else                                  {
        switch ( c ) {

            case '?':         /* list commands */
                BUFCPY(outline, TEXT128);
/*"(e)val exp, (c/x)ommand, (t)rack exp, (^G)abort, <SP>exec, <META> stop
 * debug"*/

                goto dinput;

            case 'c':         /* execute statement */
                oldcmd = discmd;
                discmd = TRUE;
                oldinp = disinp;
                disinp = TRUE;
                execcmd(FALSE, 1);
                discmd = oldcmd;
                disinp = oldinp;

                goto dbuild;

            case 'x':         /* execute extended command */
                oldcmd = discmd;
                discmd = TRUE;
                oldinp = disinp;
                disinp = TRUE;
                oldstatus = cmdstatus;
                namedcmd(FALSE, 1);
                cmdstatus = oldstatus;
                discmd = oldcmd;
                disinp = oldinp;

                goto dbuild;

            case 'e':         /* evaluate expresion */
                BUFCPY(temp, "set %track ");
                oldinp = disinp;
                disinp = TRUE;
                mlwrite("Exp:");
                getstring( (unsigned char *)&temp[11], NSTRING, ctoec(RETCHAR) );
                disinp = oldinp;
                oldstatus = cmdstatus;
                /** TODO: Is `execlevel' handling correct here?           **/
                /**       In the old coding it wasn't saved/restored.     **/
                /**       ===> The old coding lets execlevel *unchanged*  **/
                oldelevel = execlevel;
                execlevel = 0;
                docmd(temp);
                execlevel = oldelevel;
                cmdstatus = oldstatus;
                BUFCPY(temp, " = [");
                BUFCAT(temp, gtusr("track"));
                BUFCAT(temp, "]");
                mlforce(temp);
                c = get_key();

                goto dinput;

            case 't':         /* track expresion */
                oldinp = disinp;
                disinp = TRUE;
                mlwrite("Exp: ");
                getstring( (unsigned char *)temp, NSTRING, ctoec(RETCHAR) );
                disinp = oldinp;
                BUFCPY(track, "set %track ");
                BUFCAT(track, temp);

                goto dbuild;

            case 's':         /* execute a function */
                *skipflag = TRUE;
                macbug = FALSE;

                break;

            case ' ':         /* execute a statement */
                break;

            default:          /* illegal command */
                TTbeep();

                goto dbuild;
        }
    }

    return (TRUE);
}

/* FREEWHILE:
 *
 * Free a list of while block pointers.
 */
VOID PASCAL NEAR  freewhile_ P1_(WHBLOCK **, wpp)
/* wpp: ptr to head of structure to free  */
{
    if ( *wpp != NULL ) {
        freewhile_(&((*wpp)->w_next));
        CLROOM(*wpp);
        *wpp = NULL;
    }
}

/* EXECFILE:
 *
 * Execute a series of commands in a file.
 */
int PASCAL NEAR execfile P2_(
        int, f,
        int, n
    )
/* default flag and numeric arg to pass on to file */
{
    REGISTER int  status  = FALSE;  /* return status of name query  */
    char          fname[NSTRING];   /* name of file to execute      */
    CONST char    *fspec  = NULL;   /* full file spec               */
    int           oldcle  = clexec; /* save old clexec flag         */


    ZEROMEM(fname);

    if ( clexec ) {                     /* if we are non-interactive: */
        /* grab token and advance past and evaluate token:  */
        if ( macarg(fname, SIZEOF(fname)) != TRUE ) {
            return FALSE;
        }
    } else        {
#if WINDOW_MSWIN
        /* special case: we want filenamedlg to refrain from stuffing a full
         * pathname so that flook() can be put to use a few lines down the road...
         */
        if ( (status = filenamedlg(TEXT129, fname, NSTRING - 1, FALSE)) != TRUE )
#else
        {
            char  prompt[NSTRING];
            int   len = 0;

            ZEROMEM(prompt);

            len = strlen(TEXT129) - STRLEN(": ") + 1;
/*                       "File to execute: " */
            len = MIN2(len, SIZEOF(prompt));
            xstrlcpy(prompt, TEXT129, len);
            BUFCPY(fname, getfilname(prompt));
        }
        if ( ! *fname )   /* status initialized to FALSE */
#endif
        {
            STATRET(status);
        }
    }

    /* look up the path for the file */
    fspec = flook(fname, TRUE, TRUE);

    /* if it isn't around */
    if ( fspec == NULL ) {
        /* try to default the extension */
        if ( sindex(fname, ".") == 0 ) {
            BUFCAT(fname, ".cmd");
            fspec = flook(fname, TRUE, TRUE);
            if ( fspec != NULL )  {
                goto exec1;
            }
        }

        /* complain if we are interactive */
        if ( ! clexec ) {
            mlwrite(TEXT214, fname);
/*                  "%%No such file as %s" */
        }

        return (FALSE);
    }

exec1:
    /* otherwise, execute it */
    while ( n-- > 0 ) {
        clexec = TRUE;                  /* in cline execution */
        status = dofile(fspec);
        clexec = oldcle;                /* restore clexec flag */

        if ( status != TRUE ) {
            STATRET(status);
        }
    }

    return (TRUE);
}

/* DOFILE:
 *
 * Yank a file into a buffer and execute it if there are no errors,
 * delete the buffer on exit.
 */
int PASCAL NEAR dofile P1_(CONST char *, fname  /* file name to execute */)
{
    REGISTER BUFFER *bp     = NULL; /* buffer to place file to exeute         */
    REGISTER int    status  = 0;    /* results of various calls               */
    char bname[NBUFN];              /* name of buffer                         */

    ZEROMEM(bname);

    makename(bname, fname);               /* derive the name of the buffer    */
    unqname(bname);                       /* make sure we don't stomp things  */
    if ( ( bp = bfind(bname, TRUE, 0) ) == NULL ) { /* get the needed buffer  */
        return (FALSE);
    }
    bp->b_mode = MDVIEW;                  /* mark the buffer as read only     */

    /* try to read in the file to execute */
    if ( (status = readinx(fname, FALSE, bp, TRUE)) != TRUE ) {
        return (status);
    }

    /* go execute it! */
    status = dobuf(bp);
    if ( status != TRUE ) {
        return (status);
    }

    /* if not displayed, remove the now unneeded buffer and exit */
    if ( bp->b_nwnd == 0 )  {
        zotbuf(bp);
    }

    return (TRUE);
}



/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
