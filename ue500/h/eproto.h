/*======================================================================
 * EPROTO:  Global function prototypes and declarations MicroEMACS 4.00
 *
 *          written by Daniel Lawrence based on code by Dave G. Conroy,
 *          Steve Wilhite and George Jones
 *====================================================================*/

/*====================================================================*/
#ifndef EPROTO_H_
#define EPROTO_H_
/*====================================================================*/

/*====================================================================*/
/*       1         2         3         4         5         6         7*/
/*34567890123456789012345678901234567890123456789012345678901234567890*/
/*====================================================================*/


/**********************************************************************/
#include <string.h>
#include <stdio.h>
/***#include <stdlib.h>***/
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>
/***#include <string.h>***/
#include <limits.h>
#include <time.h>
/**********************************************************************/

/**********************************************************************/
#include "estruct.h"
/**********************************************************************/
/*....................................................................*/
#if WINXP || WINNT || WINDOW_MSWIN || (MSDOS && (IC || TURBO))    \
    || GCC || VMS || IS_UNIX()
# include <stdlib.h>
# include <string.h>
#else
extern char *getenv DCL((char *));
extern char *strcat DCL((char *, char *));
extern char *strcpy DCL((char *, char *));
extern int  strncmp DCL((char *, char *, int));
extern char *strchr DCL((char *, int));
extern int  strcmp DCL((char *, char *));
# if     XVT == 0 || XVTDRIVER == 0
extern int  strlen DCL((char *));
#  if RAMSIZE == 0
extern char *malloc DCL((int));
extern VOID free DCL((char *));
#  endif
extern char *realloc DCL((char *block, int siz));
# endif
#endif
/*....................................................................*/

/*....................................................................*/
/* Set defaults for settings which *could* be defined in estructc.h:  */
/*....................................................................*/
#ifndef UEMACS_TRC
# ifdef NDEBUG
#   define UEMACS_TRC          (0)
# else
#   define UEMACS_TRC         (!0)
# endif
#endif
#ifndef TRC_FILE_ENVVAR
# define TRC_FILE_ENVVAR  "UEMACS_TRC_FILE"
#endif
#ifndef REPAIR_CODE_LINE
# define REPAIR_CODE_LINE     (!0)
#endif
#ifndef DFT_STATIC_STACKSIZE
# define DFT_STATIC_STACKSIZE (16)
#endif
/*....................................................................*/


/**********************************************************************/
#if     PROTO
# define P0_(x0)                        (void)
# define P1_(t1, x1)                                                                  \
  (t1 x1)
# define P2_(t1, x1, t2, x2)                                                          \
  (t1 x1, t2 x2)
# define P3_(t1, x1, t2, x2, t3, x3)                                                  \
  (t1 x1, t2 x2, t3 x3)
# define P4_(t1, x1, t2, x2, t3, x3, t4, x4)                                          \
  (t1 x1, t2 x2, t3 x3, t4 x4)
# define P5_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5)                                  \
  (t1 x1, t2 x2, t3 x3, t4 x4, t5 x5)
# define P6_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6)                          \
  (t1 x1, t2 x2, t3 x3, t4 x4, t5 x5, t6 x6)
# define P7_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6, t7, x7)                  \
  (t1 x1, t2 x2, t3 x3, t4 x4, t5 x5, t6 x6, t7 x7)
# define P8_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6, t7, x7, t8, x8)          \
  (t1 x1, t2 x2, t3 x3, t4 x4, t5 x5, t6 x6, t7 x7, t8 x8)
# define P9_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6, t7, x7, t8, x8, t9, x9)  \
  (t, x1, t2 x2, t3 x3, t4 x4, t5 x5, t6 x6, t7 x7, t8 x8, t9 x9)
# define DCL(args)                      args
#else
# define P0_(x0)                        ()
# define P1_(t1, x1)                                                                  \
  (x1)                                  t1 x1;
# define P2_(t1, x1, t2, x2)                                                          \
  (x1, x2)                              t1 x1; t2 x2;
# define P3_(t1, x1, t2, x2, t3, x3)                                                  \
  (x1, x2, x3)                          t1 x1; t2 x2; t3 x3;
# define P4_(t1, x1, t2, x2, t3, x3, t4, x4)                                          \
  (x1, x2, x3, x4)                      t1 x1; t2 x2; t3 x3; t4 x4;
# define P5_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5)                                  \
  (x1, x2, x3, x4, x5)                  t1 x1; t2 x2; t3 x3; t4 x4; t5 x5;
# define P6_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6)                          \
  (x1, x2, x3, x4, x5, x6)              t1 x1; t2 x2; t3 x3; t4 x4; t5 x5; t6 x6;
# define P7_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6, t7, x7)                  \
  (x1, x2, x3, x4, x5, x6, x7)          t1 x1; t2 x2; t3 x3; t4 x4; t5 x5; t6 x6; t7 x7;
# define P8_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6, t7, x7, t8, x8)          \
  (x1, x2, x3, x4, x5, x6, x7, x8)      t1 x1; t2 x2; t3 x3; t4 x4; t5 x5; t6 x6; t7 x7; t8 x8;
# define P9_(t1, x1, t2, x2, t3, x3, t4, x4, t5, x5, t6, x6, t7, x7, t8, x8, t9, x9)  \
  (x1, x2, x3, x4, x5, x6, x7, x8, x9)  t1 x1; t2 x2; t3 x3; t4 x4; t5 x5; t6 x6; t7 x7; t8 x8; t9 x9;
# define DCL(args)                      ()
#endif
/**********************************************************************/


/**********************************************************************/
/* strcpy() possibly overlapping regions:   */
extern char *xstrcpy DCL((char *s1, CONST char *s2));

/* strncpy() possibly overlapping regions:  */
extern char *xstrncpy DCL((char *s1, CONST char *s2, int n));

/* Like the C99 vsnprintf():                                            */
/* May be called with NULL == s .AND. 0 == n to get the size that would */
/* be the result of an unrestricted write.                              */
/* Returns the number of characters (not including the trailing '\0')   */
/* that would have been written if n were large enough.                 */
extern int xvsnprintf DCL((char *s, size_t n, CONST char *fmt, va_list ap));

/* Like the C99 snprintf():                                             */
/* May be called with NULL == s .AND. 0 == n to get the size that would */
/* be the result of an unrestricted write.                              */
/* Returns the number of characters (not including the trailing '\0')   */
/* that would have been written if n were large enough.                 */
extern int  xsnprintf(char *s, size_t n, CONST char *fmt, ...);

/* Like GNU C vasprintf:                                        */
/* Allocate (using malloc()) a string large enough to hold the  */
/* resulting string.                                            */
extern int xvasprintf DCL((char **ret, CONST char *fmt, va_list ap));

/* Like GNU C asprintf:                                         */
/* Allocate (using malloc()) a string large enough to hold the  */
/* resulting string.                                            */
extern int xasprintf(char **ret, CONST char *fmt, ...);

extern char *xstrdup DCL((CONST char *str));

extern char *xstrtok_r DCL((char *str, CONST char *sep, char **next));

/* Concatenate character c to string str and malloc the result. */
/* Input string must either be NULL or malloced.                */
extern char *astrcatc DCL((CONST char *str, CONST char c));

/* Concatenate string d to string str and malloc the result.    */
/* Input string must either be NULL or malloced.                */
extern char *astrcat DCL((CONST char *str, CONST char *s));

/*--------------------------------------------------------------------*/
/* A Stack ADT to be used for returning pointers to static variables  */
/* (e.g. char arrays) from functions: It allows (limited) indirect    */
/* recursion of such functions.                                       */
/*--------------------------------------------------------------------*/
/* Return a new stack with stacksize elements each of size len:       */
extern VOIDP  NewStack(int stacksize, int len);   /* Won't fail       */
extern char   *NextStackElem(CONST VOIDP stack);  /* Overflow: NULL   */
extern char   *DecStackPtr(CONST VOIDP stack);    /* Return previous
                                                   * stack element or
                                                   * NULL on error.   */
extern VOID   DelStack(CONST VOIDP stack);        /* Not needed.      */
#if ( 0 )
/* Example usage: */
char *HelloFunc(int n)
{
    char          *RVAL = NULL;
#define RETURN(e) do { RVAL = (e); goto RETURN_L; } while ( 0 )

    CONST char    MSG[] = "Hello World";
    static VOIDP  s     = NULL;
    char          *msg  = NULL;
    CONST int     len   = 3 * ((sizeof(MSG) - 1) + 4) + 1;

    BEGIN_DO_ONCE {
        s = NewStack(3, len);
    } END_DO_ONCE;
    ASRT(NULL != (msg = NextStackElem(s)));
    if ( 1 >= n) {
        xsnprintf(msg, len, "%s_%d", MSG, 1);
    } else                {
        xsnprintf(msg, len, "%s, %s_%d", HelloFunc(n - 1), MSG, n);
    }

    RETURN(msg);

RETURN_L:
#undef RETURN
    ASRT(NULL != DecStackPtr(s));

    return RVAL;
}
#endif
/*--------------------------------------------------------------------*/

/**********************************************************************/

/**********************************************************************/
/**********************************************************************/

/**********************************************************************/
extern FILE *GetTrcFP DCL((void));

extern int         DebugMessage_lnno_;
extern CONST char *DebugMessage_fname_;
extern int         DebugMessage(CONST char *fmt, ...);
#if UEMACS_TRC
# define  TRC(arg)  do {                        \
        DebugMessage_fname_ = __FILE__;         \
        DebugMessage_lnno_ = __LINE__;          \
        DebugMessage arg;                       \
    } while ( 0 )
# define  TRCK(arg, file, line)  do {           \
        DebugMessage_fname_ = (file);           \
        DebugMessage_lnno_ = (line);            \
        DebugMessage arg;                       \
    } while ( 0 )
#else
# define  TRC(arg)              do {} while ( 0 )
# define  TRCK(arg, file, line) do {} while ( 0 )
#endif
/**********************************************************************/

/**********************************************************************/
#define STR(s)        ( ( NULL == (s) )?                                        \
                        ( (CONST char *)"@NIL@" ) : ( (CONST char *)(s) ) )
#define NELEM(A)      ( sizeof ( (A) )/sizeof ( (A)[0] ) )
#define SIZEOF(e)     ( (int)sizeof((e)) )
#define ZEROMEM(x)    ( memset(&(x), 0, sizeof((x))) )
#define MAXIMUM(x, y) ( ((x) < (y))? (y) : (x) )
#define MINIMUM(x, y) ( ((x) > (y))? (y) : (x) )
#define MIN2(x, y)    ( MINIMUM((x), (y)) )
#define MAX2(x, y)    ( MAXIMUM((x), (y)) )
#define MIN3(x, y, z) ( MIN2((x), MIN2((y), (z))) )
#define MAX3(x, y, z) ( MAX2((x), MAX2((y), (z))) )
/**********************************************************************/
#define MKSTRING(t)       #t
#define MKXSTRING(t)      MKSTRING(t)
/**********************************************************************/
/* If possible use XCONCAT* below -- CONCAT* might result in          */
/* undefined behaviour due to unspecified evaluation order for        */
/* multiple '##' operators.                                           */
/**********************************************************************/
#define CONCAT2(x, y)                           \
    x##y
#define CONCAT3(x, y, z)                        \
    x##y##z
#define CONCAT4(x1, x2, x3, x4)                 \
    x1##x2##x3##x4
#define CONCAT5(x1, x2, x3, x4, x5)             \
    x1##x2##x3##x4##x5
#define CONCAT6(x1, x2, x3, x4, x5, x6)         \
    x1##x2##x3##x4##x5##x6
#define CONCAT7(x1, x2, x3, x4, x5, x6, x7)     \
    x1##x2##x3##x4##x5##x6##x7
#define CONCAT8(x1, x2, x3, x4, x5, x6, x7, x8) \
    x1##x2##x3##x4##x5##x6##x7##x8
#define CONCAT9(x1, x2, x3, x4, x5, x6, x7, x8, x9)     \
    x1##x2##x3##x4##x5##x6##x7##x8##x9
/**********************************************************************/
/* Expand argument macros, concatenate from left to right.            */
/**********************************************************************/
#define XCONCAT2(x, y)                          \
    CONCAT2(x, y)
#define XCONCAT3(x, y, z)                       \
    XCONCAT2(XCONCAT2(x, y), z)
#define XCONCAT4(x1, x2, x3, x4)                \
    XCONCAT2(XCONCAT3(x1, x2, x3), x4)
#define XCONCAT5(x1, x2, x3, x4, x5)            \
    XCONCAT2(XCONCAT4(x1, x2, x3, x4), x5)
#define XCONCAT6(x1, x2, x3, x4, x5, x6)        \
    XCONCAT2(XCONCAT5(x1, x2, x3, x4, x5), x6)
#define XCONCAT7(x1, x2, x3, x4, x5, x6, x7)            \
    XCONCAT2(XCONCAT6(x1, x2, x3, x4, x5, x6), x7)
#define XCONCAT8(x1, x2, x3, x4, x5, x6, x7, x8)        \
    XCONCAT2(XCONCAT7(x1, x2, x3, x4, x5, x6, x7), x8)
#define XCONCAT9(x1, x2, x3, x4, x5, x6, x7, x8, x9)            \
    XCONCAT2(XCONCAT8(x1, x2, x3, x4, x5, x6, x7, x8), x9)
/*
 * This macro tests for pointer to something versus array of something.
 * It fails for the definition w. initialization `type *p = (type *)&p;'
 */
#define IS_ARRAY(a) ( (char *)(&(a)) == (char *)(&((a)[0])) )
/**********************************************************************/

/**********************************************************************/
/****************************************************************/
/* We must check if DESTINATION is an array, because it will be */
/* defined inside the function calling VA_COPY while the source */
/* could be given as a function argument which will be          */
/* converted to a pointer to the first argument.                */
/*                                                              */
/* The non-array case must be handled via memcpy() and not via  */
/* assignment, because otherwise the code won't compile for     */
/* array implementations of va_list.                            */
/*                                                              */
/* Tested on many Platforms.                                    */
/****************************************************************/
#if ( MSDOS && TURBO )
/* Something like `typedef void * <spec> va_list' */
# define MY_VA_COPY(d, s)     (               \
      memcpy(&(d), &(s), sizeof((d)))         \
    )
# define MY_VA_END(x)         VOIDCAST(0)
#endif
#ifndef MY_VA_COPY
# define MY_VA_COPY(d, s)     (               \
    IS_ARRAY((d))?                            \
      memcpy((d), (s), sizeof((d)))           \
        :                                     \
      memcpy(&(d), &(s), sizeof((d)))         \
    )
# define MY_VA_END(x)         VOIDCAST(0)
#endif

#if ( 0 )
# define VA_COPY            MY_VA_COPY
# define VA_END             MY_VA_END
#elif ( defined(va_copy) )
# define VA_COPY            va_copy
# define VA_END             va_end
#elif ( defined(__va_copy) )
# define VA_COPY            __va_copy
# define VA_END             va_end
#elif ( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901 )
# define VA_COPY            va_copy
# define VA_END             va_end
#elif ( 0 )
# error CANNOT DEFINE VA_COPY
#else
# define VA_COPY            MY_VA_COPY
# define VA_END             MY_VA_END
#endif
/**********************************************************************/


/**********************************************************************/
#define FSTR_   MKXSTRING(FUNC_)
/**********************************************************************/



/**********************************************************************/
#ifndef maindef
extern char *uitostr_memacs DCL((unsigned int i));
#else
char *uitostr_memacs P1_(unsigned int, i)
{
    unsigned int  base  = 10;

    /*********************************************************************
     * a hat n (> 0) Dezimalstellen <==> 10^(n-1) <= a < 10^n
     *                              <==> n -1 <= log_10(a) < n
     * Also n <= 1 + log_10(a).
     *
     * a hat m (> 0) Stellen im 256-er System
     *                              <==> 256^(m-1) <= a < 256^n
     *                              <==> m -1 <= log_256(a) < m
     * Ausserdem m <= sizeof(a), also log_256(a) < sizeof(a)
     *
     * Umrechnung der Logarithmen:
     *  y = log_b x <==> b^y = x
     *  ==> y*log_a(b) = log_a(x)
     *  ==> log_a(x) = log_a(b)*log_b(x)
     *
     * log_10(a) = log_10(256)*log_256(a) < log_10(256)*sizeof(a)
     *
     * Damit n <= 1 + 3*sizeof(a)
     *
     ********************************************************************/
    static char buf[1 + 3*sizeof(i) + 1];
    int           pos   = sizeof(buf) - 2;
    unsigned int  rest  = 0;

    ZEROMEM(buf);

    for (;;)
    {
        rest  = i % base;
        i     = i / base;
        buf[pos--]  = '0' + rest;
        if ( 0 == i )
        {
            break;
        }
    }

    return buf + pos + 1;
}
#endif
/**********************************************************************/
#define eputs(s)      VOIDCAST( GetTrcFP()? fputs((s), GetTrcFP()) : 0 )
#define eputi(i)      eputs(uitostr_memacs((unsigned int)(i)))
/**********************************************************************/

/**********************************************************************/
/*
 * CASRT(condition):
 *
 * Use this macro to test a condition at compile time instead of
 * runtime.  If condition is false, the compiler will abort with an
 * error.  This is really useful, if you want to check e.g. sizes of
 * structures against given needs.  Example: CASRT( sizeof(int) ==
 * sizeof(long) );
 */
#define CASRT(condition)  typedef int XCONCAT3(dummy_, __LINE__, _)[(condition)?1:-1]

/**********************************************************************/
/*
 * All the ASRT* REPAIT* macros below evaluate their expression (`e')
 * argument exactly *once*. So a use like
 * `ASRT(NULL != (fp = fopen("FileName", "rw")));' is correct!
 */
/**********************************************************************/
/*
 * We will use fputs instead of fprintf, because fprintf
 * might use malloc(), but we want to use ASRT to exit
 * when malloc() fails
 */
#ifndef maindef
extern VOID ASRT_Catch DCL((CONST char *file, int line, CONST char *cond));
#else
VOID ASRT_Catch P3_(CONST char *, file, int, line, CONST char *, cond)
{
    int errno_sv_ = errno;

    eputs("File: "); eputs(file); eputs(", Line: ");
    eputi(line); eputs("\n");
    eputs("\tAssertion `"); eputs(cond); eputs("' failed!\n");
    eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");
    eputs("--- abort ...\n");
    VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );
    abort();
}
#endif

#ifndef maindef
extern VOID ASRTM_Catch DCL((CONST char *file, int line, CONST char *cond, CONST char *msg));
#else
VOID ASRTM_Catch P4_(CONST char *, file, int, line, CONST char *, cond, CONST char *, msg)
{
    int errno_sv_ = errno;

    eputs("File: "); eputs(file); eputs(", Line: ");
    eputi(line); eputs("\n");
    eputs("\t"); eputs(msg); eputs("\n");
    eputs("\tAssertion `"); eputs(cond); eputs("' failed!\n");
    eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");
    eputs("--- abort ...\n");
    VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );
    abort();
}
#endif

#define ASRT(e) do {                                                    \
        if ( !(e) )                                                     \
        {                                                               \
            ASRT_Catch (__FILE__, __LINE__, #e);                        \
        }                                                               \
    } while (0)

#define ASRTM(e, m) do {                                                \
        if ( !(e) )                                                     \
        {                                                               \
            ASRTM_Catch (__FILE__, __LINE__, #e, m);                    \
        }                                                               \
    } while (0)

#define ASRTK(e, file, line) do {                                       \
        if ( !(e) )                                                     \
        {                                                               \
            ASRT_Catch (file, line, #e);                                \
        }                                                               \
    } while (0)

#define ASRTMK(e, m, file, line) do {                                   \
        if ( !(e) )                                                     \
        {                                                               \
            ASRTM_Catch (file, line, #e, m);                            \
        }                                                               \
    } while (0)

/**********************************************************************/
/*
 * We will use fputs instead of fprintf, because fprintf
 * might use malloc(), but we want to use REPAIR to exit
 * when malloc() fails
 */
#define REPAIR(e, r) do {                                               \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs(__FILE__); eputs(", Line: ");        \
            eputi(__LINE__); eputs("\n");                               \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- REPAIRING ...\n");                               \
            eputs("    `" #r "'"); eputs("\n");                         \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
            do { r ; } while ( 0 );                                     \
            eputs("--- ... DONE\n");                                    \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
        }                                                               \
    } while (0)

#define REPAIRM(e, m, r) do {                                           \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs(__FILE__); eputs(", Line: ");        \
            eputi(__LINE__); eputs("\n");                               \
            eputs("\t"); eputs((m)); eputs("\n");                       \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- REPAIRING ...\n");                               \
            eputs("    `" #r "'"); eputs("\n");                         \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
            do { r ; } while ( 0 );                                     \
            eputs("--- ... DONE\n");                                    \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
        }                                                               \
    } while (0)

#define REPAIRK(e, r, file, line) do {                                  \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs((file)); eputs(", Line: ");          \
            eputi((line)); eputs("\n");                                 \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- REPAIRING ...\n");                               \
            eputs("    `" #r "'"); eputs("\n");                         \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
            do { r ; } while ( 0 );                                     \
            eputs("--- ... DONE\n");                                    \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
        }                                                               \
    } while (0)

#define REPAIRMK(e, m, r, file, line) do {                              \
        if ( !(e) )                                                     \
        {                                                               \
            int errno_sv_ = errno;                                      \
                                                                        \
            eputs("File: "); eputs((file)); eputs(", Line: ");          \
            eputi((line)); eputs("\n");                                 \
            eputs("\t"); eputs((m)); eputs("\n");                       \
            eputs("\tAssertion `"); eputs(#e); eputs("' failed!\n");    \
            eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");   \
            eputs("--- REPAIRING ...\n");                               \
            eputs("    `" #r "'"); eputs("\n");                         \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
            do { r ; } while ( 0 );                                     \
            eputs("--- ... DONE\n");                                    \
            VOIDCAST( GetTrcFP()? fflush(GetTrcFP()) : 0 );             \
        }                                                               \
    } while (0)
/**********************************************************************/

/**********************************************************************/
#define FREE(p) do  {             \
    char **pp_  = (char **)&(p);  \
                                  \
    if ( NULL != *pp_ ) {         \
        free(*pp_);               \
        *pp_  = NULL;             \
    }                             \
} while ( 0 )
/**********************************************************************/

/**********************************************************************/
/* Do something only *once*:  */
#define BEGIN_DO_ONCE do {        \
    static int  FirstCall_  = !0; \
                                  \
    if ( FirstCall_ ) {           \
        FirstCall_  = 0;          \
        {
/**END_OF_DEFINITION**/
#define   END_DO_ONCE }           \
    }                             \
} while ( 0 )
/* Example (Yes, additional braces are not necessary):  */
#if ( 0 )
BEGIN_DO_ONCE {
  action0;
  action1;
} END_DO_ONCE;
#endif
/**********************************************************************/

/**********************************************************************/
#define C2I(c)  ( (int)(unsigned char)(c) )
/* REMARK: Using integer promotion another (cryptic) way to achive    */
/*         this would be: C2I(c) := ( (c) & 255 ).                    */
/**********************************************************************/


/**********************************************************************/
CASRT(0 <= 'a');
CASRT(0 <= 'b');
CASRT(0 <= 'c');
CASRT(0 <= 'd');
CASRT(0 <= 'e');
CASRT(0 <= 'f');
CASRT(0 <= 'g');
CASRT(0 <= 'h');
CASRT(0 <= 'i');
CASRT(0 <= 'j');
CASRT(0 <= 'k');
CASRT(0 <= 'l');
CASRT(0 <= 'm');
CASRT(0 <= 'n');
CASRT(0 <= 'o');
CASRT(0 <= 'p');
CASRT(0 <= 'q');
CASRT(0 <= 'r');
CASRT(0 <= 's');
CASRT(0 <= 't');
CASRT(0 <= 'u');
CASRT(0 <= 'v');
CASRT(0 <= 'w');
CASRT(0 <= 'x');
CASRT(0 <= 'y');
CASRT(0 <= 'z');

CASRT(0 <= 'A');
CASRT(0 <= 'B');
CASRT(0 <= 'C');
CASRT(0 <= 'D');
CASRT(0 <= 'E');
CASRT(0 <= 'F');
CASRT(0 <= 'G');
CASRT(0 <= 'H');
CASRT(0 <= 'I');
CASRT(0 <= 'J');
CASRT(0 <= 'K');
CASRT(0 <= 'L');
CASRT(0 <= 'M');
CASRT(0 <= 'N');
CASRT(0 <= 'O');
CASRT(0 <= 'P');
CASRT(0 <= 'Q');
CASRT(0 <= 'R');
CASRT(0 <= 'S');
CASRT(0 <= 'T');
CASRT(0 <= 'U');
CASRT(0 <= 'V');
CASRT(0 <= 'W');
CASRT(0 <= 'X');
CASRT(0 <= 'Y');
CASRT(0 <= 'Z');

CASRT(0 <= '0');
CASRT(0 <= '1');
CASRT(0 <= '2');
CASRT(0 <= '3');
CASRT(0 <= '4');
CASRT(0 <= '5');
CASRT(0 <= '6');
CASRT(0 <= '7');
CASRT(0 <= '8');
CASRT(0 <= '9');

CASRT(0 <= '!');
CASRT(0 <= '@');
CASRT(0 <= '#');
CASRT(0 <= '$');
CASRT(0 <= '%');
CASRT(0 <= '^');
CASRT(0 <= '&');
CASRT(0 <= '*');
CASRT(0 <= '(');
CASRT(0 <= ')');
CASRT(0 <= '_');
CASRT(0 <= '-');
CASRT(0 <= '+');
CASRT(0 <= '=');
CASRT(0 <= '[');
CASRT(0 <= ']');
CASRT(0 <= '{');
CASRT(0 <= '}');
CASRT(0 <= '\\');
CASRT(0 <= '|');
CASRT(0 <= ';');
CASRT(0 <= ':');
CASRT(0 <= '\'');
CASRT(0 <= '"');
CASRT(0 <= ',');
CASRT(0 <= '.');
CASRT(0 <= '<');
CASRT(0 <= '>');
CASRT(0 <= '/');
CASRT(0 <= '?');
CASRT(0 <= '`');
CASRT(0 <= '~');
/**********************************************************************/


/**********************************************************************/
/* colors respresented as parameters to WORDs and Screen positions */

#define CLRVAL  unsigned int

/* formal parameters to procedures are stored as a linked list of argument names
 * using the following simple structure:
 */

typedef struct PARG {
    struct PARG *next;          /* ptr to next linked argument */
    char name[NVSIZE+1];        /* name of the argument */
} PARG;

/* UNDO definitions and types */

typedef int OPTYPE;     /* type of operation being recorded/played back */

#define OP_CMND         1       /* beginning of command */
#define OP_DELC         2       /* delete a single character */
#define OP_INSC         3       /* insert a single character */
#define OP_DSTR         4       /* delete a string of characters */
#define OP_ISTR         5       /* insert a string of characters */
#define OP_REPC         6       /* replace a character */
#define OP_CPOS         7       /* set the cursor position */

/* object to be undone! */

typedef union OBJECT {
    char obj_char;              /* a character */
    char obj_string[1];         /* many characters */
    char *obj_sptr;             /* a ptr to a character */
} OBJECT;

typedef struct UNDO_OBJ {
    struct UNDO_OBJ *next;      /* ptr to next undo object */
    OPTYPE type;                /* type of operation */
    long line_num;              /* line offset from buffer beginning */
    int offset;                 /* offset into that line */
    long count;                 /* repetitions? */
    OBJECT undo_obj;            /* object to be undone */
} UNDO_OBJ;


/*
 * All text is kept in circularly linked lists of "LINE" structures. These begin
 * at the header line (which is the blank line beyond the end of the buffer).
 * This line is pointed to by the "BUFFER". Each line contains a the number of
 * bytes in the line (the "used" size), the size of the text array, and the
 * text. /o The end of line is not stored as a byte; it's implied. o/
 * The routine lalloc() allocates one addtional character which is set to '\0'
 * by the room() routine.
 */

typedef struct  LINE {
    struct  LINE *l_fp;                 /* Link to the next line        */
    struct  LINE *l_bp;                 /* Link to the previous line    */
    int     l_size_;                    /* Allocated size               */
    int     l_used_;                    /* Used size                    */
    char    l_text_[1];                 /* A bunch of characters.       */
}       LINE;

#define lforw(lp)       ( (lp)->l_fp )
#define lback(lp)       ( (lp)->l_bp )

extern char lputc_ DCL((LINE *lp, int n, char c, CONST char *fnam, int lno));
#define lputc(lp, n, c) ( lputc_((lp), (n), (c), __FILE__, __LINE__) )

#if ( IS_UNIX() )
extern unsigned char lgetc_ DCL((LINE *lp, int n, CONST char *fnam, int lno));
#else
extern          char lgetc_ DCL((LINE *lp, int n, CONST char *fnam, int lno));
#endif
#define lgetc(lp, n)    ( lgetc_((lp), (n), __FILE__, __LINE__) )

extern char *lgetcp_ DCL((LINE *lp, int n, CONST char *fnam, int lno));
#define lgetcp(lp, n)   ( lgetcp_((lp), (n), __FILE__, __LINE__) )
#define ltext(lp)       ( lgetcp(lp, 0) )

extern int get_lused_ DCL((LINE *lp, CONST char *fnam, int lno));
#define get_lused(lp)   ( get_lused_((lp), __FILE__, __LINE__) )

extern int set_lused_ DCL((LINE *lp, int used, CONST char *fnam, int lno));
#define set_lused(lp, used) ( set_lused_((lp), (used), __FILE__, __LINE__) )

extern int get_lsize_ DCL((LINE *lp, CONST char *fnam, int lno));
#define get_lsize(lp)   ( get_lsize_((lp), __FILE__, __LINE__) )


/*
 * There is a window structure allocated for every active display window. The
 * windows are kept in a big list, in top to bottom screen order, with the
 * listhead at "wheadp". Each window contains its own values of dot and mark.
 * The flag field contains some bits that are set by commands to guide
 * redisplay. Although this is a bit of a compromise in terms of decoupling, the
 * full blown redisplay is just too expensive to run for every input character.
 */

typedef struct  EWINDOW {
    struct  EWINDOW *w_wndp;            /* Next window                  */
    struct  BUFFER  *w_bufp;            /* Buffer displayed in window   */
    struct  LINE    *w_linep;           /* Top line in the window       */
    struct  LINE    *w_dotp;            /* Line containing "."          */
    int             w_doto_;            /* Byte offset for "."          */
    struct  LINE    *w_markp[NMARKS];   /* Line containing "mark"       */
    int             w_marko[NMARKS];    /* Byte offset for "mark"       */
    char            w_toprow;           /* Origin 0 top row of window   */
    char            w_ntrows;           /* # of rows of text in window  */
    char            w_force;            /* If NZ, forcing row.          */
    char            w_flag;             /* Flags.                       */
#if     COLOR
    char            w_fcolor;           /* current forground color      */
    char            w_bcolor;           /* current background color     */
#endif
    int             w_fcol;             /* first column displayed       */
}       EWINDOW;
/**********************************************************************/
extern int get_w_doto_ DCL((EWINDOW *wp, CONST char *fnam, int lno));
#define get_w_doto(wp)        ( get_w_doto_((wp), __FILE__, __LINE__) )
extern int set_w_doto_ DCL((EWINDOW *wp, int doto, CONST char *fnam, int lno));
#define set_w_doto(wp, doto)  ( set_w_doto_((wp), (doto), __FILE__, __LINE__) )
/**********************************************************************/

#define WFFORCE 0x01                    /* Window needs forced reframe  */
#define WFMOVE  0x02                    /* Movement from line to line   */
#define WFEDIT  0x04                    /* Editing within a line        */
#define WFHARD  0x08                    /* Better to a full display     */
#define WFMODE  0x10                    /* Update mode line.            */
#define WFCOLR  0x20                    /* Needs a color change         */


/*
 * This structure holds the information about each line appearing on the video
 * display. The redisplay module uses an array of virtual display lines. On
 * systems that do not have direct access to display memory, there is also an
 * array of physical display lines used to minimize video updating. In most
 * cases, these two arrays are unique. If WINDOW_MSWIN is 1, there is a pair of
 * such arrays in each SCREEN_T structure.
 */

typedef struct  VIDEO {
    int v_flag;                         /* Flags */
#if     COLOR
    int v_fcolor;                       /* current forground color */
    int v_bcolor;                       /* current background color */
    int v_rfcolor;                      /* requested forground color */
    int v_rbcolor;                      /* requested background color */
#endif
    int v_left;                         /* left edge of reverse video */
    int v_right;                        /* right right of reverse video */
#if     INSDEL && MEMMAP == 0
    int v_rline;                        /* requested screen line # */
#endif
    char v_text[1];                     /* Screen data. */
}       VIDEO;

#define VFNEW   0x0001                  /* contents not meaningful yet  */
#define VFCHG   0x0002                  /* Changed flag                 */
#define VFEXT   0x0004                  /* extended (beyond column 80)  */
#define VFCOL   0x0008                  /* color change requested       */


/*
 * This structure holds the information about each separate "screen" within
 * the current editing session.  On a character based system, these screens
 * overlay each other, and can individually be brought to front. On a
 * windowing system like MicroSoft Windows 3.0, OS/2, the Macintosh, Intuition,
 * Sunview or X-windows, each screen is represented in an OS window. The
 * terminolgy is wrong in emacs.....
 *
 *      EMACS           The outside World screen          window window
 *          pane
 */

typedef struct SCREEN_T {
    struct SCREEN_T *s_next_screen;     /* link to next screen in list */
    EWINDOW *s_first_window;            /* head of linked list of windows */
    EWINDOW *s_cur_window;              /* current window in this screen */
    char *s_screen_name;                /* name of the current window */
    short s_roworg;                     /* row origin of stored screen */
    short s_colorg;                     /* column origin of stored screen */
    short s_nrow;                       /* row width of stored screen */
    short s_ncol;                       /* column origin of stored screen */
#if WINDOW_MSWIN
    VIDEO **s_virtual;                  /* virtual screen contents */
    VIDEO **s_physical;                 /* physical screen contents */
    HWND s_drvhandle;                   /* handle for the "term" driver */
#endif
} SCREEN_T;


/*
 * Text is kept in buffers. A buffer header, described below, exists for every
 * buffer in the system. The buffers are kept in a big list, so that commands
 * that search for a buffer by name can find the buffer header. There is a safe
 * store for the dot and mark in the header, but this is only valid if the
 * buffer is not being displayed (that is, if "b_nwnd" is 0). The text for the
 * buffer is kept in a circularly linked list of lines, with a pointer to the
 * header line in "b_linep".  Buffers may be "Inactive" which means the files
 * associated with them have not been read in yet.  These get read in at "use
 * buffer" time. Some buffers are really procedures and have a little extra
 * information stored with them.
 */

typedef struct  BUFFER {
    struct  BUFFER  *b_bufp;            /* Link to next BUFFER          */
    struct  LINE    *b_dotp;            /* Link to "." LINE structure   */
    int              b_doto_;           /* Offset of "." in above LINE  */
    struct  LINE    *b_markp[NMARKS];   /* The same as the above two,   */
    int              b_marko[NMARKS];   /* but for the "mark"           */
    int              b_fcol;            /* first col to display         */
    struct  LINE    *b_linep;           /* Link to the header LINE      */
    struct  LINE    *b_topline;         /* Link to narrowed top text    */
    struct  LINE    *b_botline;         /* Link to narrowed bottom text */
    char            b_active;           /* window activated flag        */
    char            b_nwnd;             /* Count of windows on buffer   */
    char            b_exec;             /* how many active executions   */
    char            b_flag;             /* Flags                        */
    int             b_mode;             /* editor mode of this buffer   */
    char            b_fname[NFILEN];    /* File name                    */
    char            b_bname[NBUFN];     /* Buffer name                  */
#if     CRYPT
    char            b_key[NPAT];        /* current encrypted key        */
#endif
    int             b_numargs;          /* number of arguments to procedure */
    PARG            *b_args;            /* ptr to the first argument    */
    UNDO_OBJ        *undo_head;         /* head of undo stack for buffer */
    long            undo_count;         /* # of undo operations stacked */
    long            last_access;        /* time of last access          */
}       BUFFER;
/**********************************************************************/
extern int get_b_doto_ DCL((BUFFER *bp, CONST char *fnam, int lno));
#define get_b_doto(bp)        ( get_b_doto_((bp), __FILE__, __LINE__) )
extern int set_b_doto_ DCL((BUFFER *bp, int doto, CONST char *fnam, int lno));
#define set_b_doto(bp, doto)  ( set_b_doto_((bp), (doto), __FILE__, __LINE__) )
/**********************************************************************/

#define BFINVS  0x01                    /* Internal invisable buffer    */
#define BFCHG   0x02                    /* Changed since last write     */
#define BFTRUNC 0x04                    /* buffer was truncated when read */
#define BFNAROW 0x08                    /* buffer has been narrowed     */

#define NOTPROC -1                      /* buffer is not a procedure */

/*      mode flags      */
#define NUMMODES        11              /* # of defined modes           */

#define MDWRAP  0x0001                  /* word wrap                    */
#define MDCMOD  0x0002                  /* C indentation and fence match*/
#define MDSPELL 0x0004                  /* spell error parsing          */
#define MDEXACT 0x0008                  /* Exact matching for searches  */
#define MDVIEW  0x0010                  /* read-only buffer             */
#define MDOVER  0x0020                  /* overwrite mode               */
#define MDMAGIC 0x0040                  /* regular expresions in search */
#define MDCRYPT 0x0080                  /* encrytion mode active        */
#define MDASAVE 0x0100                  /* auto-save mode               */
#define MDREPL  0x0200                  /* replace mode                 */
#define MDABBR  0x0400                  /* abbreviation expansion mode  */


/*
 * The starting position of a region, and the size of the region in characters,
 * is kept in a region structure.  Used by the region commands.
 */
typedef struct  {
    struct  LINE *r_linep;              /* Origin LINE address.         */
    int     r_offset;                   /* Origin LINE offset.          */
    long    r_size;                     /* Length in characters.        */
}       REGION;

/*      This structure is used to hold a user variables name and its current
 * value. These are used for both the global and the local symbol tables.
 */

typedef struct UVAR {
    char u_name[NVSIZE + 1];                   /* name of user variable */
    char *u_value;                             /* value (string)        */
} UVAR;

#define VT_NONE         0       /* don't declare it if not found */
#define VT_LOCAL        1       /* local to the current procedure */
#define VT_GLOBAL       2       /* global to all procedures */


/*
 * A UTABLE is a user variable table.... containing some header information
 * and an array of user variable names and definitions. They are held together
 * in a linked list, the last member of the list being the global user variable
 * table.
 */

typedef struct UTABLE {
    struct UTABLE *next;        /* ptr to next user variable table */
    int size;                   /* max number of variables in table */
    BUFFER *bufp;               /* ptr to buffer holding procedure assosiated
                                 * with this symbol table. */
    UVAR uv[1];                 /* list of variable names/definitions in this
                                 * variable table */
} UTABLE;


/*
 * The editor communicates with the display using a high level interface. A
 * "TERM" structure holds useful variables, and indirect pointers to routines
 * that do useful operations. The low level get and put routines are here too.
 * This lets a terminal, in addition to having non standard commands, have funny
 * get and put character code too. The calls might get changed to
 * "termp->t_field" style in the future, to make it possible to run more than
 * one terminal type.
 */

typedef struct  {
    short t_mrow;                                   /* max number of rows allowable */
    short t_nrow;                                   /* current number of rows used  */
    short t_mcol;                                   /* max Number of columns.       */
    short t_ncol;                                   /* current Number of columns.   */
    short t_roworg;                                 /* origin row (normally zero)   */
    short t_colorg;                                 /* origin column (normally zero)*/
    short t_margin;                                 /* min margin for extended lines*/
    short t_scrsiz;                                 /* size of scroll region "      */
    int t_pause;                                    /* # times thru update to pause */
    int (PASCAL NEAR *t_open) DCL((void));          /* Open terminal at the start.  */
    int (PASCAL NEAR *t_close) DCL((void));         /* Close terminal at end.       */
    int (PASCAL NEAR *t_kopen) DCL((void));         /* Open keyboard                */
    int (PASCAL NEAR *t_kclose) DCL((void));        /* Close keyboard               */
    int (PASCAL NEAR *t_getchar) DCL((void));       /* Get character from keyboard. */
    int (PASCAL NEAR *t_putchar) DCL((int));        /* Put character to display.    */
    int (PASCAL NEAR *t_flush) DCL((void));         /* Flush output buffers.        */
    int (PASCAL NEAR *t_move) DCL((int, int));      /* Move the cursor, origin 0.   */
    int (PASCAL NEAR *t_eeol) DCL((void));          /* Erase to end of line.        */
    int (PASCAL NEAR *t_eeop) DCL((void));          /* Erase to end of page.        */
    int (PASCAL NEAR *t_clrdesk) DCL((void));       /* Clear the page totally       */
    int (PASCAL NEAR *t_beep) DCL((void));          /* Beep.                        */
    int (PASCAL NEAR *t_rev) DCL((int));            /* set reverse video state      */
    int (PASCAL NEAR *t_rez) DCL((char *));         /* change screen resolution     */
#if     COLOR
    int (PASCAL NEAR *t_setfor) DCL((int));         /* set forground color          */
    int (PASCAL NEAR *t_setback) DCL((int));        /* set background color         */
#endif
#if     INSDEL
    int (PASCAL NEAR *t_insline) DCL((int));        /* insert a screen line         */
    int (PASCAL NEAR *t_delline) DCL((int));        /* delete a screen line         */
#endif
#if     WINDOW_MSWIN
    int (PASCAL NEAR *t_sleep) DCL((int));          /* go to sleep for a while      */
    int (PASCAL NEAR *t_newscr) DCL((SCREEN_T *));  /* create new screen display    */
    int (PASCAL NEAR *t_delscr) DCL((SCREEN_T *));  /* destroy screen display       */
    int (PASCAL NEAR *t_selscr) DCL((SCREEN_T *));  /* select screen display        */
    int (PASCAL NEAR *t_sizscr) DCL((SCREEN_T *));  /* resize screen display        */
    int (PASCAL NEAR *t_topscr) DCL((SCREEN_T *));  /* bring screen to top          */
#endif
}       TERM;


/*
 * TEMPORARY macros for terminal I/O  (to be placed in a machine dependant
 * place later)
 */

#define TTopen          (*term.t_open)
#define TTclose         (*term.t_close)
#define TTkopen         (*term.t_kopen)
#define TTkclose        (*term.t_kclose)
#define TTgetc          (*term.t_getchar)
#define TTputc          (*term.t_putchar)
#define TTflush         (*term.t_flush)
#define TTmove          (*term.t_move)
#define TTeeol          (*term.t_eeol)
#define TTeeop          (*term.t_eeop)
#define TTclrdesk       (*term.t_clrdesk)
#define TTbeep          (*term.t_beep)
#define TTrev           (*term.t_rev)
#define TTrez           (*term.t_rez)
#if     COLOR
# define TTforg          (*term.t_setfor)
# define TTbacg          (*term.t_setback)
#endif


/*
 * Structure for the table of current key bindings
 */

ETYPE EPOINTER {
    int (PASCAL NEAR *fp) DCL((int, int));  /* C routine to invoke */
    BUFFER *buf;                            /* buffer to execute */
};

typedef struct  {
    short k_code;               /* Key code                     */
    short k_type;               /* binding type (C function or EMACS buffer) */
    ETYPE EPOINTER k_ptr;       /* ptr to thing to execute */
}       KEYTAB;

/*      structure for the name binding table            */

typedef struct {
    char *n_name;                               /* name of function key */
    int (PASCAL NEAR *n_func) DCL((int, int));  /* function name is bound to */
}       NBIND;


/*
 * The editor holds deleted text chunks in the KILL buffer. The kill buffer is
 * logically a stream of ascii characters, however due to its unpredicatable
 * size, it gets implemented as a linked list of chunks. (The d_ prefix is for
 * "deleted" text, as k_ was taken up by the keycode structure)
 */

typedef struct KILL {
    struct KILL *d_next;       /* link to next chunk, NULL if last */
    char d_chunk[KBLOCK];       /* deleted text */
} KILL;


/*
 * When emacs's command interpetor needs to get a variable's name, rather
 * than  it's value, it is passed back as a VDESC variable description
 * structure. The v_num field is a index into the appropriate variable
 * table.
 */

typedef struct VDESC {
    int v_type;         /* type of variable */
    int v_num;          /* ordinal pointer to variable in list */
    UTABLE *v_ut;       /* ptr to appropriate user table if user var */
} VDESC;


/*
 * The !WHILE directive in the execution language needs to stack references
 * to pending whiles. These are stored linked to each currently open procedure
 * via a linked list of the following structure
 */

typedef struct WHBLOCK {
    LINE *w_begin;              /* ptr to !while statement */
    LINE *w_end;                /* ptr to the !endwhile statement*/
    int w_type;                 /* block type */
    struct WHBLOCK *w_next;     /* next while */
} WHBLOCK;

#define BTWHILE         1
#define BTBREAK         2


/*
 * SWORDs are syntactical words to highlight in a different foreground
 * color. WORDSETs are named lists of these WORDs.
 */

typedef struct SWORD {
    struct SWORD *wd_next;      /* ptr to the next word in the list */
    char *wd_text;              /* the actual word */
    CLRVAL wd_color;            /* color to display in */
    int wd_symflag;             /* TRUE  = symbol overiding word boundries FALSE
                                 * = word w/ space/symbol delimiters */
} SWORD;

typedef struct WORDSET {
    struct WORDSET *next;
    int tmp;
} WORDSET;


/*
 * Abbreviations are short symbols that expand to longer strings when typed
 * into a buffer with no intervening whitespace or commands. This structure
 * grows dynamically as needed.
 */

typedef struct ABBREV {
    struct ABBREV *ab_next;             /* pointer to the next abbreviation */
    char ab_sym[MAXSYM + 1];            /* name to expand */
    char ab_exp[1];                     /* string to expand to */
} ABBREV;


/* Search definitions... */


/*
 * HICHAR - 1 is the largest character we will deal with. BMAPSIZE represents
 * the number of bytes in the bitmap.
 */

#define HICHAR          256
#define BMAPSIZE        HICHAR >> 3


/*
 * Jump table structures.
 */

typedef struct {
    int jump;
    int patlen;
    int delta[HICHAR];
    char patrn[NPAT];
} DELTA;


#if     MAGIC
/*
 * Defines for the metacharacters in the regular expression search routines.
 *  MCNIL and GROUP are used in both search and replace metachar-arrays.
 */
# define MCNIL           0      /* Like the '\0' for strings.*/
# define JMPTABLE        1
# define LITSTRING       2      /* Literal string.*/
# define LITCHAR         3      /* Literal character.*/
# define ANY             4      /* Any character but the <NL>.*/
# define CCL             5
# define NCCL            6
# define BOL             7
# define EOL             8
# define BOWRD           9
# define EOWRD           10
# define GRPBEG          11     /* Signal start of group.*/
# define GRPEND          12     /* Signal end of group.*/
# define GROUP           13     /* String of group match.*/
# define DITTO           14     /* Replacement with match string.*/

# define CLOSURE         0x0100 /* An or-able value for a closure modifier.*/
# define CLOSURE_1       0x0200 /* An or-able value for a closure modifier.*/
# define ZEROONE         0x0400 /* An or-able value for a closure modifier.*/

# define ALLCLOS         (CLOSURE | CLOSURE_1 | ZEROONE)
# define MASKCLO         (~ALLCLOS)

# define MC_ANY          '.'    /* 'Any' character (except newline).*/
# define MC_CCL          '['    /* Character class.*/
# define MC_NCCL         '^'    /* Negate character class.*/
# define MC_RCCL         '-'    /* Range in character class.*/
# define MC_ECCL         ']'    /* End of character class.*/
# define MC_BOL          '^'    /* Beginning of line.*/
# define MC_EOL          '$'    /* End of line.*/
# define MC_CLOSURE      '*'    /* Closure - zero to many characters match.*/
# define MC_CLOSURE_1    '+'    /* Closure - one to many characters match.*/
# define MC_ZEROONE      '?'    /* Closure - zero to one characters match.*/
# define MC_DITTO        '&'    /* Use matched string in replacement.*/
# define MC_GRPBEG       '('    /* Start of group (begun with a backslash).*/
# define MC_GRPEND       ')'    /* End of group (begun with a backslash).*/
# define MC_BOWRD        '<'    /* Beginning of word (begun with a backslash).*/
# define MC_EOWRD        '>'    /* End of word (begun with a backslash).*/
# define MC_ESC          '\\'   /* Escape - suppress meta-meaning.*/

# define MAXGROUPS       10             /* 1 + maximum # of r. e. groups. */
# define BIT(n)          ( 1 << (n) )   /* An integer with one bit set.*/


/*
 * Typedefs that define the bitmap type for searching (EBITMAP), the
 * meta-character structure for MAGIC mode searching (MC), and the
 * meta-character structure for MAGIC mode replacment (RMC).
 */

typedef char    *EBITMAP;

typedef struct {
    short int mc_type;
    union {
        int lchar;
        int group_no;
        char    *lstring;
        DELTA   *jmptable;
        EBITMAP cclmap;
    } u;
} MC;

typedef struct {
    short int mc_type;
    union {
        int group_no;
        char    *rstr;
    } u;
} RMC;
#endif


/**********************************************************************/

/***    global function prototypes      ***/

/* MicroEmacs named function type:  */
typedef int PASCAL NEAR (*ue_fnc_T) DCL((int, int));

/* Filter function used by TransformRegion():
 * Output string must be created by malloc(). */
typedef char  *(*filter_func_T) DCL((CONST char *rstart, CONST char *rtext,
                                VOIDP argp));

#if     WINDOW_MSWIN
extern char * PASCAL fullpathname  DCL((char *PathName, int Nbuf));
extern int PASCAL NEAR vtinitscr  DCL((SCREEN_T *sp, int nrow, int ncol));
extern int PASCAL NEAR vtsizescr  DCL((SCREEN_T *sp, int nrow, int ncol));
extern int PASCAL cutregion  DCL((int f, int n));
extern int PASCAL clipregion  DCL((int f, int n));
extern int PASCAL insertclip  DCL((int f, int n));
extern int PASCAL helpengine  DCL((int f, int n));
extern int PASCAL minimizescreen  DCL((int f, int n));
extern int PASCAL maximizescreen  DCL((int f, int n));
extern int PASCAL restorescreen  DCL((int f, int n));
extern int PASCAL tilescreens  DCL((int f, int n));
extern int PASCAL cascadescreens  DCL((int f, int n));
extern int PASCAL bindtomenu  DCL((int f, int n));
extern int PASCAL macrotomenu  DCL((int f, int n));
extern int PASCAL unbindmenu  DCL((int f, int n));
extern int PASCAL execmenu  DCL((int f, int n));
extern int PASCAL longop  DCL((int f));
extern int PASCAL filenamedlg  DCL((char *prompt, char *buf, int nbuf, int fullpath));
extern int PASCAL NEAR vtfreescr  DCL((SCREEN_T *sp));
extern VOID PASCAL NEAR unlist_screen DCL((SCREEN_T *sp));
extern int PASCAL mlhistory DCL((void));
extern int PASCAL updscrollbars  DCL((SCREEN_T *sp, char w_flag));
extern VOID PASCAL NEAR vtscreen  DCL((SCREEN_T *sp));
#endif

#if CALLED
extern int emacs DCL((int argc, char *argv[]));
#endif

#if HANDLE_WINCH
extern VOID winch_changed DCL((int));
extern VOID winch_new_size DCL((void));
#endif

#if DEBUG_SEARCH
int PASCAL NEAR mc_list DCL((int f, int n));
int PASCAL NEAR rmc_list DCL((int f, int n));
VOID PASCAL NEAR mctype_cat DCL((char pline[], int mc_type));
#endif

extern VOID undo_insert DCL((OPTYPE op_type, long count, OBJECT op_erand));
extern int undo_op DCL((void));
extern VOID undo_dump DCL((void));
extern VOID undo_zot DCL((BUFFER *bp));
extern int PASCAL NEAR undo DCL((int f, int n));
extern int PASCAL NEAR undo_delete DCL((int f, int n));
extern int PASCAL NEAR undo_list DCL((int f, int n));
extern char *room DCL((int));
extern char *reroom DCL((VOIDP, int));

extern int PASCAL NEAR ab_insert DCL((char *sym, char *expansion));
extern char *PASCAL NEAR ab_lookup DCL((char *sym));
extern int PASCAL NEAR ab_delete DCL((char *sym));
extern int PASCAL NEAR ab_clean DCL((void));
extern BUFFER *PASCAL NEAR bfind DCL((char *bname, int cflag, int bflag));
extern BUFFER *PASCAL NEAR getcbuf DCL((char *prompt, char *defval, int createflag));
extern BUFFER *PASCAL NEAR getdefb DCL((void));
extern BUFFER *PASCAL NEAR getoldb DCL((void));
extern SCREEN_T *PASCAL NEAR init_screen DCL((char *, BUFFER *));
extern SCREEN_T *PASCAL NEAR lookup_screen DCL((char *scr_name));
extern SCREEN_T *PASCAL NEAR index_screen DCL((int scr_num));
extern int PASCAL NEAR screen_index DCL((SCREEN_T *sp));
extern int PASCAL NEAR insert_screen DCL((SCREEN_T *sp));
extern int PASCAL NEAR select_screen DCL((SCREEN_T *sp, int announce));
extern VOID PASCAL NEAR free_screen DCL((SCREEN_T *sp));
extern char *Eallocate DCL((unsigned nbytes));
extern char *dolock DCL((CONST char *fname));
extern char *getpath DCL((char *filespec));
extern char *gtname DCL((char *filespec));
extern char *PASCAL NEAR bytecopy DCL((char *dst, CONST char *src, int maxlen));
extern char *PASCAL NEAR cmdstr DCL((int c, char *seq));
extern char *PASCAL NEAR copystr DCL((char *));
extern char *PASCAL NEAR complete DCL((char *prompt,
                                  char *defval,
                                  int  type,
                                  int  maxlen));
extern char *PASCAL NEAR envval DCL((int i));
extern CONST char *PASCAL NEAR fixnull DCL((CONST char *s));
extern CONST char *PASCAL NEAR flook DCL((CONST char *fname, int hflag));
extern char *PASCAL NEAR funval DCL((int i));
extern char *PASCAL NEAR getctext DCL((char *rline));
extern char *PASCAL NEAR getffile DCL((char *fspec));
extern char *PASCAL NEAR getfname DCL((KEYTAB *key));
extern char *PASCAL NEAR getkill DCL((void));
extern char *PASCAL NEAR getnfile DCL((void));
extern CONST char *PASCAL NEAR getreg DCL((char *value));
extern CONST char *PASCAL NEAR getval DCL((char *token));
extern char *PASCAL NEAR getwlist DCL((char *buf));
extern CONST char *PASCAL NEAR gtenv DCL((CONST char *vname));
extern char *PASCAL NEAR gtfilename DCL((char *prompt));
extern CONST char *PASCAL NEAR gtfun DCL((CONST char *fname));
extern CONST char *PASCAL NEAR gtusr DCL((CONST char *vname));
extern char *PASCAL NEAR int_asc DCL((int i));
extern char *PASCAL NEAR long_asc DCL((long num));
extern CONST char *PASCAL NEAR ltos DCL((int val));
extern CONST char *PASCAL NEAR makename DCL((char *bname, CONST char *fname));
extern char *PASCAL NEAR mklower DCL((char *str));
extern char *PASCAL NEAR mkupper DCL((char *str));
extern char *PASCAL NEAR namval DCL((int index));
extern char *PASCAL NEAR timeset DCL((void));
extern char *PASCAL NEAR token DCL((char *src, char *tok, int size));
extern CONST char *PASCAL NEAR transbind DCL((CONST char *skey));
extern char *PASCAL NEAR trimstr DCL((char *s));
extern char *PASCAL NEAR xlat DCL((char *source, char *lookup, char *trans));
extern char *undolock DCL((CONST char *fname));
extern char *PASCAL NEAR regtostr DCL((char *buf, REGION *region));
extern int PASCAL NEAR lowerc DCL((char ch));
extern int PASCAL NEAR cycle_ring DCL((int f, int n));
extern int PASCAL NEAR upperc DCL((char ch));
extern ue_fnc_T fncmatch DCL((char *fname));
extern ue_fnc_T getname  DCL((char *prompt));
extern int PASCAL NEAR asc_int DCL((char *st));
extern VOID dohello DCL((void));
extern int dspram DCL((void));
extern VOID lckerror DCL((char *errstr));
extern VOID lckhello DCL((void));
extern int xlock DCL((CONST char *fname));
extern int lockchk DCL((CONST char *fname));
extern int lockrel DCL((void));
extern int mousehello DCL((void));
extern int nocrypt DCL((void));
extern int PASCAL NEAR absv DCL((int x));
extern int PASCAL NEAR add_abbrev DCL((int f, int n));
extern int PASCAL NEAR del_abbrev DCL((int f, int n));
extern int PASCAL NEAR kill_abbrevs DCL((int f, int n));
extern int PASCAL NEAR ins_abbrevs DCL((int f, int n));
extern int PASCAL NEAR def_abbrevs DCL((int f, int n));
extern int PASCAL NEAR addline DCL((BUFFER *bp, char *text));
extern int PASCAL NEAR amatch DCL((MC *mcptr, int direct, LINE **pcwline,
                              int *pcwoff));
extern int PASCAL NEAR backhunt DCL((int f, int n));
extern int PASCAL NEAR backsearch DCL((int f, int n));
extern int PASCAL NEAR biteq DCL((int bc, EBITMAP cclmap));
extern int PASCAL NEAR bktoshell DCL((int f, int n));
extern int PASCAL NEAR boundry DCL((LINE *curline, int curoff, int dir));
extern int PASCAL NEAR cclmake DCL((char **ppatptr, MC *mcptr));
extern int PASCAL NEAR checknext DCL((int chr, int dir));
extern int PASCAL NEAR clear_ring DCL((int f, int n));
extern int PASCAL NEAR delins DCL((int dlength, char *instr, int use_rmc));
extern int PASCAL NEAR desfunc DCL((int f, int n));
extern int PASCAL NEAR dispvar DCL((int f, int n));
extern int PASCAL NEAR echochar DCL((unsigned char c));
extern int PASCAL NEAR echostring DCL((char *, int, int));
extern int PASCAL NEAR eq DCL((register unsigned char bc, register unsigned char pc));
extern long PASCAL NEAR ernd DCL((void));
extern int PASCAL NEAR execkey DCL((KEYTAB *key, int f, int n));
#if VMS
extern void PASCAL NEAR expandargs DCL((int *pargc, char ***pargv));
#endif
extern int PASCAL NEAR fbound DCL((DELTA *tbl,
                              int   jump,
                              LINE  **pcurline,
                              int   *pcuroff,
                              int   dir));
extern int PASCAL NEAR fexist DCL((char *fname));
extern int PASCAL NEAR findcol DCL((LINE *lp, int pos));
extern int PASCAL NEAR fisearch DCL((int f, int n));
#if     FLABEL
extern int PASCAL NEAR fnclabel DCL((int f, int n));
#endif
extern int PASCAL NEAR forwhunt DCL((int f, int n));
extern int PASCAL NEAR forwsearch DCL((int f, int n));
extern int PASCAL NEAR getcwnum DCL((void));
extern int PASCAL NEAR getgoal DCL((LINE *dlp));
extern int PASCAL NEAR getstring DCL((unsigned char *buf, int nbuf, int eolchar));
extern int PASCAL NEAR gettwnum DCL((void));
extern int PASCAL NEAR gettyp DCL((char *token));
extern int PASCAL NEAR getkey DCL((void));
extern int PASCAL NEAR getwpos DCL((void));
extern int PASCAL NEAR get_char DCL((void));
extern int PASCAL NEAR global_var DCL((int f, int n));
#if VMS
/***TODO***/
extern int PASCAL NEAR grabnowait DCL((void));
extern int PASCAL NEAR grabwait DCL((void));
#else
extern unsigned char   grabwait DCL((void));
extern unsigned char   grabnowait DCL((void));
#endif
#if     DBCS
extern int PASCAL NEAR is2byte DCL((char *sp, char *cp));
#endif
extern int PASCAL NEAR is_letter DCL((char ch));
extern int PASCAL NEAR is_lower DCL((char ch));
extern int PASCAL NEAR is_num DCL((char *st));
extern int PASCAL NEAR isearch DCL((int dir));
extern int PASCAL NEAR is_upper DCL((char ch));
extern int PASCAL NEAR kinsert DCL((int back, char c));
extern int PASCAL NEAR ldelnewline DCL((void));
extern int PASCAL NEAR linstr DCL((CONST char *instr));
extern int PASCAL NEAR liteq DCL((LINE **curline,
                             int  *curpos,
                             int  direct,
                             char *lstring));
extern int PASCAL NEAR litmake DCL((char **ppatptr, MC *mcptr));
extern int PASCAL NEAR lnewline DCL((void));
extern int PASCAL NEAR local_var DCL((int f, int n));
extern int PASCAL NEAR lookup_color DCL((char *sp));
extern int PASCAL NEAR lover DCL((char *ostr));
extern int PASCAL NEAR mceq DCL((unsigned char bc, MC *mt));
extern int PASCAL NEAR mcscanner DCL((MC  *mcpatrn,
                                 int direct,
                                 int beg_or_end,
                                 int repeats));
extern int PASCAL NEAR mcstr DCL((void));
extern int PASCAL NEAR mlprompt DCL((char *, char *, int));
extern int PASCAL NEAR movelocalpoint DCL((int n, int *pcuroff, LINE **pcurline));
extern int PASCAL NEAR nextch DCL((LINE **pcurline, int *pcuroff, int dir));
extern int PASCAL NEAR pop DCL((BUFFER *popbuffer));
extern int PASCAL NEAR qreplace DCL((int f, int n));
extern int PASCAL NEAR readpattern DCL((char *prompt, char apat[], int srch));
#if     WINDOW_TEXT
extern VOID PASCAL NEAR refresh_screen DCL((SCREEN_T *sp));
#endif
extern int PASCAL NEAR reglines DCL((void));
extern int PASCAL NEAR rename_screen DCL((int f, int n));
extern int PASCAL NEAR replaces DCL((int kind, int f, int n));
extern int PASCAL NEAR risearch DCL((int f, int n));
extern int PASCAL NEAR rmcstr DCL((void));
extern int PASCAL NEAR savematch DCL((void));
extern int PASCAL NEAR scanmore DCL((int dir));
#if MAGIC == 0
extern int PASCAL NEAR scanner DCL((int direct, int beg_or_end, int repeats));
#endif
extern int PASCAL NEAR setlower DCL((char *ch, char *val));
extern int PASCAL NEAR setupper DCL((char *ch, char *val));
extern int PASCAL NEAR setvar DCL((int f, int n));
extern int PASCAL NEAR sindex DCL((char *source, char *pattern));
extern int PASCAL NEAR sreplace DCL((int f, int n));
extern int PASCAL NEAR stol DCL((char *val));
#if     DBCS
extern int PASCAL NEAR stopback DCL((void));
extern int PASCAL NEAR stopforw DCL((void));
#endif
extern int PASCAL NEAR svar DCL((VDESC *var, char *value));
extern int PASCAL NEAR tgetc DCL((void));
extern int PASCAL NEAR uneat DCL((void));
extern VOID PASCAL NEAR unlist_screen DCL((SCREEN_T *sp));
extern int PASCAL NEAR upscreen DCL((int f, int n));
extern int PASCAL NEAR vtinit DCL((void));
extern int PASCAL NEAR yank DCL((int f, int n));
extern int PASCAL NEAR yank_pop DCL((int f, int n));
extern int Erelease DCL((char *mp));
extern int set_key DCL((KEYTAB *key, char *name));
extern int xunlock DCL((char *fname));
extern KEYTAB *getbind DCL((int c));
extern LINE *PASCAL NEAR lalloc DCL((int used));
extern LINE *PASCAL NEAR mouseline DCL((EWINDOW *wp, int row));
extern long PASCAL NEAR getlinenum DCL((BUFFER *bp, LINE *sline));
extern int PASCAL NEAR addkey DCL((unsigned char * seq, int fn));
extern int PASCAL NEAR addkeymap DCL((int f, int n));
extern int PASCAL NEAR adjustmode DCL((int kind, int global));
extern int PASCAL NEAR anycb DCL((void));
extern int PASCAL NEAR apro DCL((int f, int n));
extern int PASCAL NEAR backchar DCL((int f, int n));
extern int PASCAL NEAR backdel DCL((int f, int n));
extern int PASCAL NEAR backline DCL((int f, int n));
extern int PASCAL NEAR backpage DCL((register int f, register int n));
extern int PASCAL NEAR backword DCL((int f, int n));
extern int PASCAL NEAR bclear DCL((BUFFER *bp));
extern int PASCAL NEAR binary(CONST char *key,
                              char *(PASCAL NEAR *tval)(),
                              int        tlength,
                              int        klength);
extern int PASCAL NEAR bindtokey DCL((int f, int n));
extern int PASCAL NEAR buildlist DCL((int type, char *mstring));
extern int PASCAL NEAR capword DCL((int f, int n));
extern int PASCAL NEAR cex DCL((int f, int n));
extern int PASCAL NEAR cinsert DCL((void));
extern int PASCAL NEAR clean DCL((void));
extern int PASCAL NEAR clrmes DCL((int f, int n));
extern int PASCAL NEAR copyregion DCL((int f, int n));
extern int PASCAL NEAR ctlxe DCL((int f, int n));
extern int PASCAL NEAR ctlxlp DCL((int f, int n));
extern int PASCAL NEAR ctlxrp DCL((int f, int n));
extern int PASCAL NEAR ctoec DCL((int c));
extern int PASCAL NEAR ctrlg DCL((int f, int n));
extern int PASCAL NEAR cycle_screens DCL((int f, int n));
extern VOID PASCAL NEAR dcline DCL((int argc, char *argv[], int firstflag));
extern int PASCAL NEAR deblank DCL((int f, int n));
extern int PASCAL NEAR debug DCL((BUFFER *bp, char *eline, int *skipflag));
extern int PASCAL NEAR delbword DCL((int f, int n));
extern int PASCAL NEAR delete_screen DCL((int f, int n));
extern int PASCAL NEAR delfword DCL((int f, int n));
extern int PASCAL NEAR delgmode DCL((int f, int n));
extern int PASCAL NEAR delmode DCL((int f, int n));
extern int PASCAL NEAR delwind DCL((int f, int n));
extern int PASCAL NEAR desc_abbrevs DCL((int f, int n));
extern int PASCAL NEAR desbind DCL((int f, int n));
extern int PASCAL NEAR deskey DCL((int f, int n));
extern int PASCAL NEAR desvars DCL((int f, int n));
extern int PASCAL NEAR detab DCL((int f, int n));
extern int PASCAL NEAR dobuf DCL((BUFFER *bp));
extern int PASCAL NEAR docmd DCL((char *cline));
extern int PASCAL NEAR dofile DCL((CONST char *fname));
extern int PASCAL NEAR ectoc DCL((int c));
extern VOID PASCAL NEAR edinit DCL((char bname[]));
extern int PASCAL NEAR editloop DCL((void));
extern int PASCAL NEAR endword DCL((int f, int n));
extern int PASCAL NEAR enlargewind DCL((int f, int n));
extern int PASCAL NEAR entab DCL((int f, int n));
extern int PASCAL NEAR execbuf DCL((int f, int n));
extern int PASCAL NEAR execcmd DCL((int f, int n));
extern int PASCAL NEAR execfile DCL((int f, int n));
extern int PASCAL NEAR execprg DCL((int f, int n));
extern int PASCAL NEAR execproc DCL((int f, int n));
extern int PASCAL NEAR execute DCL((int c, int f, int n));
extern int PASCAL NEAR ffclose DCL((void));
extern int PASCAL NEAR ffgetline DCL((int *nbytes));
extern int PASCAL NEAR ffputline DCL((char buf[], int nbuf));
extern int PASCAL NEAR ffropen DCL((CONST char *fn));
extern int PASCAL NEAR ffwopen DCL((char *fn, char *mode));
extern int PASCAL NEAR fileapp DCL((int f, int n));
extern int PASCAL NEAR find_screen DCL((int f, int n));
extern int PASCAL NEAR filefind DCL((int f, int n));
extern int PASCAL NEAR filename DCL((int f, int n));
extern int PASCAL NEAR fileread DCL((int f, int n));
extern int PASCAL NEAR filesave DCL((int f, int n));
extern int PASCAL NEAR filewrite DCL((int f, int n));
extern int PASCAL NEAR f_filter DCL((int f, int n));
extern VOID PASCAL NEAR findvar DCL((char *var, VDESC *vd, int size, int scope));
extern int PASCAL NEAR fmatch DCL((char ch));
extern int PASCAL NEAR forwchar DCL((int f, int n));
extern int PASCAL NEAR forwdel DCL((int f, int n));
extern int PASCAL NEAR forwline DCL((int f, int n));
extern int PASCAL NEAR forwpage DCL((int f, int n));
extern int PASCAL NEAR forwword DCL((int f, int n));
extern int PASCAL NEAR getccol DCL((int bflg));
extern int PASCAL NEAR getcmd DCL((void));
extern int PASCAL NEAR getfence DCL((int f, int n));
extern int PASCAL NEAR getfile DCL((CONST char *fname, int lockfl));
extern int PASCAL NEAR get_key DCL((void));
extern int PASCAL NEAR getregion DCL((REGION *rp));
extern int PASCAL NEAR gotobob DCL((int f, int n));
extern int PASCAL NEAR gotobol DCL((int f, int n));
extern int PASCAL NEAR gotobop DCL((int f, int n));
extern int PASCAL NEAR gotoeob DCL((int f, int n));
extern int PASCAL NEAR gotoeol DCL((int f, int n));
extern int PASCAL NEAR gotoeop DCL((int f, int n));
extern int PASCAL NEAR gotoline DCL((int f, int n));
extern int PASCAL NEAR gotomark DCL((int f, int n));
extern int PASCAL NEAR help DCL((int f, int n));
extern int PASCAL NEAR ifile DCL((char fname[]));
extern int PASCAL NEAR indent DCL((int f, int n));
extern int PASCAL NEAR insbrace DCL((int n, int c));
extern int PASCAL NEAR insfile DCL((int f, int n));
extern int PASCAL NEAR inspound DCL((void));
extern int PASCAL NEAR insspace DCL((int f, int n));
extern int PASCAL NEAR inword DCL((void));
extern int PASCAL NEAR isinword DCL((char c));
extern int PASCAL NEAR ismodeline DCL((EWINDOW *wp, int row));
extern int PASCAL NEAR istring DCL((int f, int n));
extern int PASCAL NEAR killbuffer DCL((int f, int n));
extern int PASCAL NEAR killpara DCL((int f, int n));
extern int PASCAL NEAR killregion DCL((int f, int n));
extern int PASCAL NEAR killtext DCL((int f, int n));
extern int PASCAL NEAR lchange DCL((register int flag));
extern int PASCAL NEAR ldelete DCL((long n, int kflag));
extern int PASCAL NEAR lfree DCL((LINE *lp));
extern int PASCAL NEAR linsert DCL((int n, char c));
extern int PASCAL NEAR listbuffers DCL((int f, int n));
extern int PASCAL NEAR list_screens DCL((int f, int n));
extern int PASCAL NEAR lowerregion DCL((int f, int n));
extern int PASCAL NEAR lowerword DCL((int f, int n));
extern int PASCAL NEAR lowrite DCL((char c));
extern int PASCAL NEAR macarg DCL((char *tok));
extern int PASCAL NEAR macrotokey DCL((int f, int n));
extern int PASCAL NEAR makelist DCL((int iflag));
extern int PASCAL NEAR movelocalpoint DCL(());
extern int PASCAL NEAR undolist DCL(());
extern VOID PASCAL NEAR mouse_screen DCL((void));
extern int PASCAL NEAR screenlist DCL((int iflag));
extern int PASCAL NEAR meexit DCL((int status));
extern int PASCAL NEAR f_meta DCL((int f, int n));
extern int PASCAL NEAR mlreply DCL((char *prompt, char *buf, int nbuf));
extern int PASCAL NEAR mlyesno DCL((char *prompt));
extern int PASCAL NEAR mouseoffset DCL((EWINDOW *wp, LINE *lp, int col));
extern int PASCAL NEAR movemd DCL((int f, int n));
extern int PASCAL NEAR movemu DCL((int f, int n));
extern int PASCAL NEAR mregdown DCL((int f, int n));
extern int PASCAL NEAR mmove DCL((int f, int n));
extern int PASCAL NEAR mregup DCL((int f, int n));
extern int PASCAL NEAR mvdnwind DCL((int f, int n));
extern int PASCAL NEAR mvupwind DCL((int f, int n));
extern int PASCAL NEAR namebuffer DCL((int f, int n));
extern int PASCAL NEAR namedcmd DCL((int f, int n));
extern int PASCAL NEAR narrow DCL((int f, int n));
extern int PASCAL NEAR newline DCL((int f, int n));
extern int PASCAL NEAR new_col_org DCL((int f, int n));
extern int PASCAL NEAR new_row_org DCL((int f, int n));
extern int PASCAL NEAR newsize DCL((int f, int n));
extern int PASCAL NEAR newwidth DCL((int f, int n));
extern int PASCAL NEAR nextarg DCL((char *prompt,
                               char *buffer,
                               int  size,
                               int  terminator));
extern int PASCAL NEAR nextbuffer DCL((int f, int n));
extern int PASCAL NEAR nextdown DCL((int f, int n));
extern int PASCAL NEAR nextup DCL((int f, int n));
extern int PASCAL NEAR nextwind DCL((int f, int n));
extern int PASCAL NEAR nullproc DCL((int f, int n));
extern int PASCAL NEAR onlywind DCL((int f, int n));
extern int PASCAL NEAR openline DCL((int f, int n));
extern int PASCAL NEAR ostring DCL((char *s));
extern int PASCAL NEAR outstring DCL((char *s));
extern int PASCAL NEAR ovstring DCL((int f, int n));
extern int PASCAL NEAR pipecmd DCL((int f, int n));
extern int PASCAL NEAR popbuffer DCL((int f, int n));
extern int PASCAL NEAR prevwind DCL((int f, int n));
extern int PASCAL NEAR putctext DCL((char *iline));
extern int PASCAL NEAR putline DCL((int row, int col, char buf[]));
extern int PASCAL NEAR quickexit DCL((int f, int n));
extern int PASCAL NEAR quit DCL((int f, int n));
extern int PASCAL NEAR quote DCL((int f, int n));
extern int PASCAL NEAR rdonly DCL((void));
extern int PASCAL NEAR readin DCL((const char *fname, int lockfl));
extern int PASCAL NEAR refresh DCL((int f, int n));
extern int PASCAL NEAR remmark DCL((int f, int n));
extern int PASCAL NEAR reposition DCL((int f, int n));
extern int PASCAL NEAR resetkey DCL((void));
extern int PASCAL NEAR resize DCL((int f, int n));
extern int PASCAL NEAR resizm DCL((int f, int n));
extern int PASCAL NEAR resizm2 DCL((int f, int n));
extern int PASCAL NEAR resterr DCL((void));
extern int PASCAL NEAR restwnd DCL((int f, int n));
extern int PASCAL NEAR savewnd DCL((int f, int n));
extern int PASCAL NEAR scwrite DCL((int  row,
                               char *outstr,
                               int  forg,
                               int  bacg,
                               int  revleft,
                               int  revright));
extern int PASCAL NEAR setccol DCL((int pos));
extern int PASCAL NEAR setekey DCL((int f, int n));
extern int PASCAL NEAR setfillcol DCL((int f, int n));
extern int PASCAL NEAR setgmode DCL((int f, int n));
extern int PASCAL NEAR setmark DCL((int f, int n));
extern int PASCAL NEAR setmod DCL((int f, int n));
extern int PASCAL NEAR setwlist DCL((char *wclist));
extern int PASCAL NEAR shellprog DCL((char *cmd));
extern int PASCAL NEAR showcpos DCL((int f, int n));
extern int PASCAL NEAR showfiles DCL((int f, int n));
extern int PASCAL NEAR listkeymaps DCL((int f, int n));
extern int PASCAL NEAR shrinkwind DCL((int f, int n));
extern int PASCAL NEAR spal DCL((char *pstr));
extern int PASCAL NEAR spawn DCL((int f, int n));
extern int PASCAL NEAR spawncli DCL((int f, int n));
extern int PASCAL NEAR splitwind DCL((int f, int n));
extern int PASCAL NEAR startup DCL((char *sfname));
extern int PASCAL NEAR storeproc DCL((int f, int n));
extern int PASCAL NEAR strinc DCL((char *source, char *sub));
extern int PASCAL NEAR swapmark DCL((int f, int n));
extern int PASCAL NEAR swbuffer DCL((BUFFER *bp));
extern int PASCAL NEAR tab DCL((int f, int n));
extern int TransformBuffer DCL((filter_func_T filter, VOIDP argp));
extern int TransformParagraph DCL((filter_func_T filter, VOIDP argp));
extern int TransformRegion DCL((filter_func_T filter, VOIDP argp));
extern int PASCAL NEAR trBufFill DCL((int f, int n));
extern int PASCAL NEAR trBufTest_ DCL((int f, int n));
extern int PASCAL NEAR trParFill DCL((int f, int n));
extern int PASCAL NEAR trParTest_ DCL((int f, int n));
extern int PASCAL NEAR trRegFill DCL((int f, int n));
extern int PASCAL NEAR trRegIndent DCL((int f, int n));
extern int PASCAL NEAR trRegTest_ DCL((int f, int n));
extern int PASCAL NEAR trRegUndent DCL((int f, int n));
extern int PASCAL NEAR trim DCL((int f, int n));
extern int PASCAL NEAR ttclose DCL((void));
extern int PASCAL NEAR ttflush DCL((void));
extern int PASCAL NEAR ttgetc DCL((void));
extern int PASCAL NEAR ttopen DCL((void));
extern int PASCAL NEAR ttputc DCL((int c));
extern int PASCAL NEAR twiddle DCL((int f, int n));
extern int PASCAL NEAR typahead DCL((void));
extern int PASCAL NEAR unarg DCL((int f, int n));
extern int PASCAL NEAR unbindchar DCL((int c));
extern int PASCAL NEAR unbindkey DCL((int f, int n));
extern int PASCAL NEAR unmark DCL((int f, int n));
extern int PASCAL NEAR upperregion DCL((int f, int n));
extern int PASCAL NEAR upperword DCL((int f, int n));
extern int PASCAL NEAR usebuffer DCL((int f, int n));
extern int PASCAL NEAR viewfile DCL((int f, int n));
extern VOID PASCAL NEAR vteeol DCL((void));
extern VOID PASCAL NEAR vtmove DCL((int row, int col));
extern VOID PASCAL NEAR vtputc DCL((int c));
extern VOID PASCAL NEAR vttidy DCL((void));
extern int PASCAL NEAR widen DCL((int f, int n));
extern int PASCAL NEAR wordcount DCL((int f, int n));
extern int PASCAL NEAR wrapword DCL((int f, int n));
extern int PASCAL NEAR writemsg DCL((int f, int n));
extern int PASCAL NEAR writeout DCL((char *fn, char *mode));
extern int PASCAL NEAR zotbuf DCL((BUFFER *bp));
extern unsigned int PASCAL NEAR chcase DCL((register unsigned int ch));
extern unsigned int PASCAL NEAR getckey DCL((int mflag));
extern unsigned int PASCAL NEAR stock DCL((CONST char *keyname));
#if     VARARG && VARG
# if     GCC
extern VOID CDECL NEAR mlwrite DCL((va_alist));
# else
extern VOID CDECL NEAR mlwrite DCL((va_dcl));
# endif /* GCC */
#else
extern VOID CDECL NEAR mlwrite(char *fmt, ...);
#endif
extern VOID PASCAL NEAR ab_init DCL((void));
extern VOID PASCAL NEAR ab_save DCL((char c));
extern VOID PASCAL NEAR ab_expand DCL((void));
extern VOID PASCAL NEAR clist_buffer DCL((char *name, int *cpos));
extern VOID PASCAL NEAR clist_command DCL((char *name, int *cpos));
extern VOID PASCAL NEAR clist_file DCL((char *name, int *cpos));
extern VOID PASCAL NEAR comp_buffer DCL((char *name, int *cpos));
extern VOID PASCAL NEAR comp_command DCL((char *name, int *cpos));
#if     !WINDOW_MSWIN
extern VOID PASCAL NEAR comp_file DCL((char *name, int *cpos));
#endif
extern VOID PASCAL NEAR ecrypt DCL((char *bptr, unsigned len));
extern VOID PASCAL NEAR errormesg DCL((char *mesg, BUFFER *bp, LINE *lp));
extern VOID PASCAL NEAR flong_asc DCL((char buf[], int width, long num));
extern VOID PASCAL NEAR freewhile DCL((WHBLOCK *wp));
extern VOID PASCAL NEAR getdtime DCL((char *ts));
extern VOID PASCAL NEAR initchars DCL((void));
extern VOID PASCAL NEAR kdelete DCL((void));
extern VOID PASCAL NEAR lowercase DCL((unsigned char *cp));
extern VOID PASCAL NEAR mcclear DCL((void));
extern VOID PASCAL NEAR mlabort DCL((char *s));
extern VOID PASCAL NEAR mlerase DCL((void));
extern VOID PASCAL NEAR mlferase DCL((void));
extern VOID PASCAL NEAR mlforce DCL((char *s));
extern VOID PASCAL NEAR mlout DCL((int c));
extern VOID PASCAL NEAR mlputf DCL((int s));
extern VOID PASCAL NEAR mlputi DCL((int i, int r));
extern VOID PASCAL NEAR mlputli DCL((long l, int r));
extern VOID PASCAL NEAR mlputs DCL((char *s));
extern VOID PASCAL NEAR mlrquery DCL((void));
extern VOID PASCAL NEAR modeline DCL((EWINDOW *wp));
extern VOID PASCAL NEAR movecursor DCL((int row, int col));
extern VOID PASCAL NEAR next_kill DCL((void));
extern VOID PASCAL NEAR pad DCL((char *s, int len));
extern VOID PASCAL NEAR reeat DCL((int c));
extern VOID PASCAL NEAR reframe DCL((EWINDOW *wp));
extern VOID PASCAL NEAR rmcclear DCL((void));
extern VOID PASCAL NEAR setbit DCL((int bc, EBITMAP cclmap));
extern VOID PASCAL NEAR setjtable DCL((void));
extern VOID PASCAL NEAR unbind_buf DCL((BUFFER *bp));
extern VOID PASCAL NEAR unqname DCL((char *name));
extern VOID PASCAL NEAR updall DCL((EWINDOW *wp));
extern VOID PASCAL NEAR update DCL((int force));
extern VOID PASCAL NEAR update_size DCL((void));
extern VOID PASCAL NEAR upddex DCL((void));
extern VOID PASCAL NEAR updext DCL((void));
extern VOID PASCAL NEAR updgar DCL((void));
extern VOID PASCAL NEAR updone DCL((EWINDOW *wp));
extern VOID PASCAL NEAR updpos DCL((void));
extern VOID PASCAL NEAR updupd DCL((int force));
extern VOID PASCAL NEAR upmode DCL((void));
extern VOID PASCAL NEAR uppercase DCL((unsigned char *cp));
extern VOID PASCAL NEAR upwind DCL((void));
extern VOID PASCAL NEAR varinit DCL((void));
extern VOID PASCAL NEAR varclean DCL((UTABLE *ut));
extern VOID PASCAL NEAR uv_init DCL((UTABLE *ut));
extern VOID PASCAL NEAR uv_clean DCL((UTABLE *ut));
extern VOID PASCAL NEAR vtfree DCL((void));
extern VOID cook DCL((void));
extern VOID qin DCL((int ch));
extern VOID qrep DCL((int ch));
extern EWINDOW *PASCAL NEAR mousewindow DCL((int row));
extern int PASCAL NEAR wpopup DCL((BUFFER *popbuf));

#if     CTAGS
extern int PASCAL NEAR tagword DCL((int f, int n));   /* vi-like tagging */
extern int PASCAL NEAR retagword DCL((int f, int n)); /* Try again (if redefined) */
extern int PASCAL NEAR backtagword DCL((int f, int n)); /* return from tagged word */
#endif

/* some library redefinitions */

#if WINXP == 0
extern char *strrev DCL((char *));
#endif

/**********************************************************************/


/**********************************************************************/
/* Some helper macros:                                                */
/**********************************************************************/
#define ISALNUM(x)  isalnum((unsigned char)(x))
#define ISALPHA(x)  isalpha((unsigned char)(x))
#define ISCNTRL(x)  iscntrl((unsigned char)(x))
#define ISDIGIT(x)  isdigit((unsigned char)(x))
#define ISGRAPH(x)  isgraph((unsigned char)(x))
#define ISLOWER(x)  islower((unsigned char)(x))
#define ISPRINT(x)  isprint((unsigned char)(x))
#define ISPUNCT(x)  ispunct((unsigned char)(x))
#define ISSPACE(x)  isspace((unsigned char)(x))
#define ISUPPER(x)  isupper((unsigned char)(x))
#define ISXDIGIT(x) isxdigit((unsigned char)(x))
#define ISASCII(x)  isascii((unsigned char)(x))
#define ISBLANK(x)  isblank((unsigned char)(x))
/**********************************************************************/



/*====================================================================*/
#endif/**#ifndef EPROTO_H_**/
/*====================================================================*/

/**********************************************************************/
/* EOF                                                                */
/**********************************************************************/
