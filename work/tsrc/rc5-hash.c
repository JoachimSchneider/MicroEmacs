/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C. */
/* Copyright (C) 1995 RSA Data Security, Inc.                 */
/* R.L. Rivest                                                */
/* Small change - ADD(), SUB() - working on 64-bit systems.   */


#include <stdio.h>
#ifdef __STDC__
# include <string.h>
# include <stdlib.h>
#endif
#undef NDEBUG
#include <assert.h>


#define BYTE_BITS      (8)    /* Bits In Byte */
#define T_MAX         (64)
#define C_1            1
#define C_2            2
#define C_3            3
#define C_4            4
#define C_8            8
#define C_16          16
#define C_32          32
#define FALSE         (0)
#define TRUE          (1)
#define BEGIN_COMMENT (0)
#ifdef __STDC__
typedef void *        voidp_t;
#else
typedef char *        voidp_t;
#endif
#define VOIDP         voidp_t
#define VOID          void
#define CONST         const


#define CRASH(x)  (0 = 0)
/**********************************************************************/
/*....................................................................*/
/* You may use                                                        */
/* - `CASRT'  at places where a declaration is syntactically correct  */
/* - `CASRTS' at places where a statement is syntactically correct    */
/*....................................................................*/
#define CASRT(cond)                         \
  extern int casrt_dummy_x_[1];             \
  extern int casrt_dummy_x_[(cond)? 1 : 2]
/**END_OF_DEFINITION**/
#define CASRTS(cond)  do { CASRT((cond)); } while ( 0 )
/**********************************************************************/

typedef unsigned char      byte_t;
typedef unsigned short int ushort_t;
typedef unsigned       int uint_t;
typedef unsigned long  int ulong_t;

/* - K&R-1978 does *not* require this, but it's example platforms
 *   respect these restrictions.
 * - K&R-1989 require these restrictions.
 */
CASRT(C_1              == sizeof(byte_t));
CASRT(C_2              <= sizeof(ushort_t));
CASRT(sizeof(ushort_t) <= sizeof(uint_t));
CASRT(sizeof(uint_t)   <= sizeof(ulong_t));
CASRT(C_4              <= sizeof(ulong_t));
CASRT(sizeof(VOIDP) == sizeof(ulong_t));        /* Excludes OS/400    */


#ifdef __STDC__
# define MIN_ULONG_MAX  4294967295UL            /* 32-Bit max ulong_t */
# include <limits.h>                            /* ANSI-C has it!     */
# if  MIN_ULONG_MAX < ULONG_MAX          /* Conclusion: 64-Bit System */
   CASRT(C_8 == sizeof(VOIDP));
#  define PTR_SIZE    (C_8)
#  define WORD_BITS   (C_32)
# if  MIN_ULONG_MAX <= UINT_MAX                 /* uint_t has 32 Bits */
#  define WORD_T_     uint_t
# else
#  define WORD_T_     ulong_t
# endif
# else
# if  MIN_ULONG_MAX == ULONG_MAX
   CASRT(C_4== sizeof(VOIDP));
#  define PTR_SIZE    (C_4)
#  define WORD_BITS   (C_16)
#  define WORD_T_     ushort_t
# else
    CRASH();
# endif
# endif
#else
  CASRT(C_4 == sizeof(VOIDP));
# define PTR_SIZE     (C_4)
# define WORD_BITS    (C_16)
# define WORD_T_      ushort_t
#endif
CASRT(WORD_BITS * 2 / BYTE_BITS == PTR_SIZE);


typedef struct  rc5_parm_s_ {
    int     valid;  /* boolean: data have been set                */
    int     w;      /* word size in bits                          */
    WORD_T_ m;      /* Bitmask for word                           */
    int     r;      /* number of rounds                           */
    int     t;      /* (<= T_MAX) nelem of table S: 2*(r+1) words */
    WORD_T_ P;      /* magic constant                             */
    WORD_T_ Q;      /* magic constant                             */
    WORD_T_ S[T_MAX]; /* expanded key table                       */
} rc5_parm_t;


static  rc5_parm_t  rc5_8_parm;
static  rc5_parm_t  rc5_16_parm;
static  rc5_parm_t  rc5_32_parm;


/* Rotation operators, x must be unsigned, to get logical right shift */
#define ROTL(x,y, w)  ( ((x)<<((y)&((w)-1))) | ((x)>>((w)-((y)&((w)-1)))) )
#define ROTR(x,y, w)  ( ((x)>>((y)&((w)-1))) | ((x)<<((w)-((y)&((w)-1)))) )
#define ADD(x, y, m)  ( ((x) + (y)) & (m) )
#define SUB(x, y, m)  ( ((x) - (y)) & (m) )
#define MAX2(x, y)    ( (x) > (y) ? (x) : (y) )


static int xmemset(VOIDP x, char c, int len)
{
    char  *cp = (char *)x;
    int   i   = 0;

    for ( i = 0; i < len; i++ ) {
        cp[i] = c;
    }

    return 0;
}

#define ZEROMEM(x)    ( xmemset(&(x), 0, sizeof((x))) )
#define MIN2(x, y)    ( (x) < (y) ? (x) : (y) )


static VOID RC5_ENCRYPT(rc5_parm_t *R,
                        WORD_T_    *pt,   /* 2 Word input pt  */
                        WORD_T_    *ct    /* 2 Word output ct */
                       )
{
    int     i = 0;
    WORD_T_ A = ADD(pt[0], R->S[0], R->m);
    WORD_T_ B = ADD(pt[1], R->S[1], R->m);

    for ( i = 1; i <= R->r; i++ ) {
        A = ADD(ROTL(A ^ B, B, R->w), R->S[2*i], R->m);
        B = ADD(ROTL(B ^ A, A, R->w), R->S[2*i+1], R->m);
    }
    ct[0] = A;
    ct[1] = B;
}

static VOID RC5_DECRYPT(rc5_parm_t *R,
                        WORD_T_    *ct, /* 2 Word input ct  */
                        WORD_T_    *pt  /* 2 Word output pt */
                       )
{
    int     i = 0;
    WORD_T_ A = ct[0];
    WORD_T_ B = ct[1];

    for ( i = R->r; i > 0; i-- )  {
        B = ROTR(SUB(B, R->S[2*i+1], R->m), A, R->w) ^ A;
        A = ROTR(SUB(A, R->S[2*i], R->m),   B, R->w) ^ B;
    }
    pt[0] = SUB(A, R->S[0], R->m);
    pt[1] = SUB(B, R->S[1], R->m);
}

VOID RC5_INIT(rc5_parm_t *R, int w, int r)
{
    int     i = 0;
    int     k = 0;
    WORD_T_ A = 0;
    WORD_T_ B = 0;

    assert(NULL != R);
    assert(0 < w);
    assert(0 == (w % 2));
    assert(0 == (w % 8));
    assert(w <= 32);
    assert(0 <= r);

#if BEGIN_COMMENT /* Fields to initialize:  */
    int     valid;  /* boolean: data have been set                */
    int     w;      /* word size in bits                          */
    WORD_T_ m;      /* Bitmask for word                           */
    int     r;      /* number of rounds                           */
    int     t;      /* (<= T_MAX) nelem of table S: 2*(r+1) words */
    WORD_T_ P;      /* magic constant                             */
    WORD_T_ Q;      /* magic constant                             */
    WORD_T_ S[T_MAX]; /* expanded key table                       */
#endif

    R->valid  = TRUE;
    R->w      = w;
    R->r      = r;
    R->t      = 2 * (r + 1);
    assert(R->t <= T_MAX);

    switch ( w )  {
#if C_8 <= WORD_BITS
        case  C_8:
            R->m  = 0xFF;
            R->P  = 0xb7;
            R->Q  = 0x9F;

            break;
#endif

#if C_16 <= WORD_BITS
        case C_16:
            R->m  = 0xFFFF;
            R->P  = 0xb7e1;
            R->Q  = 0x9e37;

            break;
#endif

#if C_32 <= WORD_BITS
        case C_32:
            R->m  = 0xFFFFFFFF;
            R->P  = 0xb7e15163;
            R->Q  = 0x9e3779b9;

            break;
#endif

        default:
            assert(0);
            break;
    }
    ZEROMEM(R->S);
    for ( R->S[0] = R->P, i = 1; i < R->t; i++ )  {
        R->S[i] = ADD(R->S[i-1], R->Q, R->m);
    }
    for ( A = B = i = k = 0;
          k < 3 * R->t;
          k++, i = (i + 1) % R->t )
    {
        A = R->S[i] = ROTL(ADD(R->S[i], ADD(A, B, R->m), R->m), C_3, R->w);
        B =           ROTL(ADD(A, B, R->m), ADD(A, B, R->m), R->w);
    }
}


ulong_t mix_ulong(ulong_t x)
{
    ulong_t           res     = 0;
    WORD_T_           in[2];
    WORD_T_           out[2];
    static rc5_parm_t parm;

    if ( ! parm.valid )  {
        RC5_INIT(&parm, WORD_BITS, C_4);
    }
    in[1] = (x >> WORD_BITS);
    in[0] = x & parm.m;

#if 1
    RC5_ENCRYPT(&parm, in, out);
#else
    memcpy(out, in, MIN2(sizeof(out), sizeof(in)));
#endif

    res |=  out[1];
    res <<= WORD_BITS;
    res |=  out[0];

    return res;
}


#define USAGE_EXIT  (42)

static int usage(CONST char *pgm)
{
    fprintf(stderr, "%s: Usage %s [-t <Unsigned Long Int>|-c]\n", pgm, pgm);
    fprintf(stderr, "\t%s\n", "-c: This will generate an infinate random byte stream to stdout");

    exit(USAGE_EXIT);
}


int main(int argc, char *argv[])
{
    int         rc          = 0;
    int         i           = 0;
    WORD_T_     pt1[2];
    WORD_T_     pt2[2];
    WORD_T_     ct[2];

    if ( 1>= argc ) {
        /* Setup parameters */
#if C_8 <= WORD_BITS
        RC5_INIT(&rc5_8_parm,   8, 4);
#endif
#if C_16 <= WORD_BITS
        RC5_INIT(&rc5_16_parm, 16, 6);
#endif
#if C_32 <= WORD_BITS
        RC5_INIT(&rc5_32_parm, 32, 8);
#endif

#if C_32 <= WORD_BITS
        printf("%s\n", "RC5-32/8/0 examples:");
        ZEROMEM(pt1);
        ZEROMEM(pt2);
        ZEROMEM(ct);
        for ( i = 1; i < 6; i++ ) {
            /* Initialize pt1 and key pseudorandomly based on previous ct */
            pt1[0] = ct[0];
            pt1[1] = ct[1];
            /* encrypt, and decrypt */
            RC5_ENCRYPT(&rc5_32_parm, pt1, ct);
            RC5_DECRYPT(&rc5_32_parm, ct, pt2);
            /* Print out results, checking for decryption failure */
            printf("\n%d.", (int)i);
            printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
                   (ulong_t)pt1[0], (ulong_t)pt1[1], (ulong_t)ct[0], (ulong_t)ct[1]);
            if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
                printf("DecryptionError!");
                rc++;
            }
        }
#endif

#if C_16 <= WORD_BITS
        printf("%s\n", "RC5-16/6/0 examples:");
        ZEROMEM(pt1);
        ZEROMEM(pt2);
        ZEROMEM(ct);
        for ( i = 1; i < 6; i++ ) {
            /* Initialize pt1 and key pseudorandomly based on previous ct */
            pt1[0] = ct[0];
            pt1[1] = ct[1];
            /* encrypt, and decrypt */
            RC5_ENCRYPT(&rc5_16_parm, pt1, ct);
            RC5_DECRYPT(&rc5_16_parm, ct, pt2);
            /* Print out results, checking for decryption failure */
            printf("\n%d.", (int)i);
            printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
                   (ulong_t)pt1[0], (ulong_t)pt1[1], (ulong_t)ct[0], (ulong_t)ct[1]);
            if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
                printf("DecryptionError!");
                rc++;
            }
        }
#endif

#if C_8 <= WORD_BITS
        printf("%s\n", "RC5-8/4/0 examples:");
        ZEROMEM(pt1);
        ZEROMEM(pt2);
        ZEROMEM(ct);
        for ( i = 1; i < 6; i++ ) {
            /* Initialize pt1 and key pseudorandomly based on previous ct */
            pt1[0] = ct[0];
            pt1[1] = ct[1];
            /* encrypt, and decrypt */
            RC5_ENCRYPT(&rc5_8_parm, pt1, ct);
            RC5_DECRYPT(&rc5_8_parm, ct, pt2);
            /* Print out results, checking for decryption failure */
            printf("\n%d.", (int)i);
            printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
                   (ulong_t)pt1[0], (ulong_t)pt1[1], (ulong_t)ct[0], (ulong_t)ct[1]);
            if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
                printf("DecryptionError!");
                rc++;
            }
        }
#endif
    } else            {
        CONST char  *progname = argv[0];

        argv++;

        while ( NULL != *argv ) {
            if ( 0 == strcmp(*argv, "-t") ) {
                ulong_t in  = 0;
                ulong_t out = 0;

                argv++;
                if ( NULL == *argv )  {
                    usage(progname);
                }
                in  = strtoul(*argv, NULL, C_16);
                out = mix_ulong(in);
                fprintf(stdout, "0x%lX\n", out);
            } else if ( 0 == strcmp(*argv, "-c") )  {
                ulong_t i = 0;

                for ( ;;i++ ) {
                    ulong_t res = mix_ulong(i);
                    int     j   = 0;

                    for ( j = 0; j < sizeof(res); j++ ) {
                        unsigned char c = res & 0xFF;

                        putc(c, stdout);

                        res >>= BYTE_BITS;
                    }
                }
            } else                                  {
                usage(progname);
            }

            argv++;
        }
    }

    return rc;
}


/* EOF */
