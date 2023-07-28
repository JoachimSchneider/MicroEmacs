#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#define xsnprintf snprintf
#define CONST     const
#define VOID      void
typedef void      *VOIDP;
#define VOIDCAST  (void)

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

#define ZEROMEM(x)    ( memset(&(x), 0, sizeof((x))) )
#define FREE(p) do  {             \
    char **pp_  = (char **)&(p);  \
                                  \
    if ( NULL != *pp_ ) {         \
        free(*pp_);               \
        *pp_  = NULL;             \
    }                             \
} while ( 0 )
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


char *uitostr_memacs(unsigned int i)
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
#define eputs(s)      VOIDCAST( fputs((s), stderr) )
#define eputi(i)      eputs(uitostr_memacs((unsigned int)(i)))

VOID ASRT_Catch (CONST char * file, int line, CONST char * cond)
{
    int errno_sv_ = errno;

    eputs("File: "); eputs(file); eputs(", Line: ");
    eputi(line); eputs("\n");
    eputs("\tAssertion `"); eputs(cond); eputs("' failed!\n");
    eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");
    eputs("--- abort ...\n");
    abort();
}

#define ASRT(e) do {                                                    \
        if ( !(e) )                                                     \
        {                                                               \
            ASRT_Catch (__FILE__, __LINE__, #e);                        \
        }                                                               \
    } while (0)


#define PROTO (1)

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


/*====================================================================*/
/* The x<ident> functions have the same funcionaly like the <ident>   */
/* functions found in newer C libraries.                              */
/*====================================================================*/


/* strcpy() possibly overlapping regions: */
#if ( 0 )
char *xstrcpy P2_(char *, s1, CONST char *, s2)
{
    char  *s  = NULL;

    ASRT(NULL != s1);
    ASRT(NULL != s2);

    ASRT(NULL != (s = (char *)calloc(strlen(s2) + 1, sizeof (char))));
    strcpy(s, s2);
    strcpy(s1, s);
    FREE(s);

    return s1;
}
#else
char *xstrcpy P2_(char *, s1, CONST char *, s2)
{
    if ( NULL != s1 && NULL != s2 ) {
        if        ( s1 <  s2 )  {
            int i = 0;

            for ( i = 0; '\0' != s2[i]; i++ ) {
                s1[i] = s2[i];
            }
            s1[i] = '\0';
        } else if ( s1 == s2 )  {
            /**EMPTY**/
        } else if ( s1 >  s2 )  {
            int i = 0;

            for ( i = strlen(s2); i >= 0; i-- ) {
                s1[i] = s2[i];
            }
        } else                  { /* Possible on e.g. OS/400 */
            strcpy(s1, s2);
        }
    } else if ( NULL == s1 )  {
        if ( '\0' != *s2 )  {
            abort();
        }
    } else if ( NULL == s2 )  {
        *s1 = '\0';
    }

    return s1;
}
#endif

/* strncpy() possibly overlapping regions:  */
#if ( 0 )
char *xstrncpy P3_(char *, s1, CONST char *, s2, int, n)
{
    int   l2  = 0;
    char *s   = NULL;

    ASRT(0 <= n);
    if ( 0 == n ) {
        return s1;
    }
    ASRT(NULL != s1);
    ASRT(NULL != s2);

    l2  = strlen(s2);
    l2  = MAX2(l2, n);
    ASRT(NULL !=(s  = (char *)calloc( l2 + 1, sizeof (char) )));
    strncpy(s, s2, n);  /* This will always succedd and result in
                         * a '\0'-terminated s. */

    strncpy(s1, s, n);
    FREE(s);

    return s1;
}
#else
char *xstrncpy P3_(char *, s1, CONST char *, s2, int, n)
{
    if ( NULL != s1 && NULL != s2 ) {
        int l2  = 0;

        l2  = strlen(s2) + 1;
        if        ( s1 <  s2 )  {
            int ncpy  = MIN2(l2, n);
            int i     = 0;

            for ( i = 0; i < ncpy; i++ )  {
                s1[i] = s2[i];
            }
            for ( ; i < n; i++ )  {
                s1[i] = '\0';
            }
        } else if ( s1 == s2 )  {
            /**EMPTY**/
        } else if ( s1 >  s2 )  {
            int i = 0;

            for ( i = n - 1; i >= l2; i-- ) {
                s1[i] = '\0';
            }
            for ( ; i >= 0; i-- ) {
                s1[i] = s2[i];
            }
        } else                  { /* Possible on e.g. OS/400 */
            strncpy(s1, s2, n);
        }
    } else if ( NULL == s1 )  {
        if ( '\0' != *s2 )  {
            abort();
        }
    } else if ( NULL == s2 )  {
        *s1 = '\0';
    }

    return s1;
}
#endif

/* Like FreeBSD's strlcpy(): Equivalent semantics:
 *  n = strlcpy(dst, src, len);
 *  n = snprintf(dst, len, "%s", src);
 */
int xstrlcpy P3_(char *, s1, CONST char *, s2, int, n)
{
    int l2  = 0;

    if ( NULL != s1 && NULL != s2 ) {
        int ncpy  = 0;

        l2    = strlen(s2);
        ncpy  = MIN2(l2, n);

        if        ( s1 <  s2 )  {
            int i     = 0;

            for ( i = 0; i < ncpy; i++ )  {
                s1[i] = s2[i];
            }
            s1[i] = '\0';
        } else if ( s1 == s2 )  {
            /**EMPTY**/
        } else if ( s1 >  s2 )  {
            int i = 0;

            s1[ncpy]  = '\0';

            for ( i = ncpy - 1; i >= 0; i-- ) {
                s1[i] = s2[i];
            }
        } else                  { /* Possible on e.g. OS/400 */
            abort();
        }
    } else if ( NULL == s1 )  {
        if ( '\0' != *s2 )  {
            abort();
        }
    } else if ( NULL == s2 )  {
        *s1 = '\0';
    }

    return l2;
}


int main(int argc, char *argv[])
{
    char A0[]  = "abcdefhijklmnopqrstuvwxyz";
    char A1[]  = "abcdefhijklmnopqrstuvwxyz";
    char B0[]  = "ABCDEFHIJKLMNOPQRSTUVWXYZ";
    char B1[]  = "ABCDEFHIJKLMNOPQRSTUVWXYZ";

    char *s1  = &A1[7];   /* 'i'  */
    char *s2  = &A1[10];  /* 'l'  */
    char *s3  = &A1[18];  /* 'u'  */

    xstrncpy(s2, s1, 3);

    fprintf(stdout, "%s\n", A1);

    memcpy(A1, A0, sizeof(A1));

    fprintf(stdout, "%s\n", A1);

    xstrncpy(s1, s2, 4);

    fprintf(stdout, "%s\n", A1);

    memcpy(A1, A0, sizeof(A1));

    fprintf(stdout, "%s\n", A1);

    xstrlcpy(s2, s1, 4);

    fprintf(stdout, "%s\n", s2);

    memcpy(A1, A0, sizeof(A1));

    fprintf(stdout, "%s\n", A1);

    xstrlcpy(s1, s2, 4);

    fprintf(stdout, "%s\n", s1);

    return 0;
}


/* EOF */

