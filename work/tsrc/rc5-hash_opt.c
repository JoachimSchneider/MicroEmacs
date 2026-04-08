/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C. */
/* Copyright (C) 1995 RSA Data Security, Inc.                 */
/* R.L. Rivest                                                */
/* Small change - ADD(), SUB() - working on 64-bit systems.   */


#include <stdio.h>
#include <string.h>
#undef NDEBUG
#include <assert.h>


#define BIB           (8)   /* Bits In Byte */
#define T_MAX         (64)
#define C_1           1
#define C_2           2
#define C_3           3
#define C_4           4
#define FALSE         (0)
#define TRUE          (1)
#define BEGIN_COMMENT (0)

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
typedef void *             voidp;
#define VOIDP              voidp

/* - K&R-1978 does *not* require this, but it's example platforms
 *   respect these restrictions.
 * - K&R-1989 require these restrictions.
 */
CASRT(C_1              == sizeof(byte_t));
CASRT(C_2              <= sizeof(ushort_t));
CASRT(sizeof(ushort_t) <= sizeof(uint_t));
CASRT(sizeof(uint_t)   <= sizeof(ulong_t));
CASRT(C_4              <= sizeof(ulong_t));


typedef struct *p_rc5_parm_t;

/*====================================================================*/
#define WORD_T_         byte_t
#define WORDVALS_X_     WORDVALS_8
#define RC5_ENCRYPT_X_  RC5_ENCRYPT_8
#define RC5_DECRYPT_X_  RC5_DECRYPT_8
#define RC5_ENCRYPT_X_T RC5_ENCRYPT_8_T
#define RC5_DECRYPT_X_T RC5_DECRYPT_8_T
#include "rc5-hash.inc"

#undef WORD_T_
#undef WORDVALS_X_
#undef RC5_ENCRYPT_X_
#undef RC5_DECRYPT_X_
#undef RC5_ENCRYPT_X_T
#undef RC5_DECRYPT_X_T
#define WORD_T_         ushort_t
#define WORDVALS_X_     WORDVALS_16
#define RC5_ENCRYPT_X_  RC5_ENCRYPT_16
#define RC5_DECRYPT_X_  RC5_DECRYPT_16
#define RC5_ENCRYPT_X_T RC5_ENCRYPT_16_T
#define RC5_DECRYPT_X_T RC5_DECRYPT_16_T
#include "rc5-hash.inc"

#undef WORD_T_
#undef WORDVALS_X_
#undef RC5_ENCRYPT_X_
#undef RC5_DECRYPT_X_
#undef RC5_ENCRYPT_X_T
#undef RC5_DECRYPT_X_T
#define WORD_T_         ulong_t
#define WORDVALS_X_     WORDVALS_32
#define RC5_ENCRYPT_X_  RC5_ENCRYPT_32
#define RC5_DECRYPT_X_  RC5_DECRYPT_32
#define RC5_ENCRYPT_X_T RC5_ENCRYPT_32_T
#define RC5_DECRYPT_X_T RC5_DECRYPT_32_T
#include "rc5-hash.inc"

#undef WORD_T_
#undef WORDVALS_X_
#undef RC5_ENCRYPT_X_
#undef RC5_DECRYPT_X_
#undef RC5_ENCRYPT_X_T
#undef RC5_DECRYPT_X_T
/*====================================================================*/

typedef struct  rc5_parm_s_ {
    int     valid;  /* boolean: data have been set                */
    int     w;      /* word size in bits                          */
    int     r;      /* number of rounds                           */
    int     t;      /* (<= T_MAX) nelem of table S: 2*(r+1) words */
    union {
        WORDVALS_8  v8;
        WORDVALS_16 v32;
        WORDVALS_32 v32;
    } u;
    VOIDP encrypt;
    VOIDP decrypt;
} rc5_parm_t, *p_rc5_parm_t;

static  rc5_parm_t  rc5_8_parm;
static  rc5_parm_t  rc5_16_parm;
static  rc5_parm_t  rc5_32_parm;


/* Rotation operators, x must be unsigned, to get logical right shift */
#define ROTL(x,y, w)  ( ((x)<<((y)&((w)-1))) | ((x)>>((w)-((y)&((w)-1)))) )
#define ROTR(x,y, w)  ( ((x)>>((y)&((w)-1))) | ((x)<<((w)-((y)&((w)-1)))) )
#define ADD(x, y, m)  ( ((x) + (y)) & (m) )
#define SUB(x, y, m)  ( ((x) - (y)) & (m) )
#define MAX2(x, y)    ( (x) > (y) ? (x) : (y) )

#define ZEROMEM(x)    ( memset(&(x), 0, sizeof((x))) )


#define CodeBlock_(m_val, P_val, Q_val) do  {                                   \
    WORD_T_  A = 0;                                                             \
    WORD_T_  B = 0;                                                             \
                                                                                \
    R->m_  = (m_val);                                                           \
    R->P_  = (P_val);                                                           \
    R->Q_  = (Q_val);                                                           \
                                                                                \
    for ( R->S_[0] = R->P_, i = 1; i < R->t; i++ )  {                           \
        R->S_[i] = ADD(R->S_[i-1], R->Q_, R->m_);                               \
    }                                                                           \
    for ( i = k = 0;                                                            \
          k < 3 * R->t;                                                         \
          k++, i = (i + 1) % R->t )                                             \
    {                                                                           \
        A = R->S_[i] = ROTL(ADD(R->S_[i], ADD(A, B, R->m_), R->m_), C_3, R->w); \
        B            = ROTL(ADD(A, B, R->m_), ADD(A, B, R->m_), R->w);          \
    }                                                                           \
    R->encrypt  = (VOIDP)&RC5_ENCRYPT_X_;
    R->decrypt  = (VOIDP)&RC5_DECRYPT_X_;
} while ( 0 )


void RC5_SETUP(rc5_parm_t *R, int w, int r)
{
    int     i = 0;
    int     k = 0;

    assert(NULL != R);
    assert(0 < w);
    assert(0 == (w % 2));
    assert(0 == (w % 8));
    assert(w <= 32);
    assert(0 <= r);

    ZEROMEM(*R);

#if BEGIN_COMMENT /* Fields to initialize:  */
    int     valid;  /* boolean: data have been set                */
    int     w;      /* word size in bits                          */
    int     r;      /* number of rounds                           */
    int     t;      /* (<= T_MAX) nelem of table S: 2*(r+1) words */

    typedef struct  {
        WORD_T_ m;      /* Bitmask for word                           */
        WORD_T_ P;      /* magic constant                             */
        WORD_T_ Q;      /* magic constant                             */
        WORD_T_ S[T_MAX]; /* expanded key table                       */
    } WORDVALS_X_;
    union {
        WORDVALS_8  v8;
        WORDVALS_16 v32;
        WORDVALS_32 v32;
    } u;
    VOIDP encrypt;
    VOIDP decrypt;
#endif

    R->valid  = TRUE;
    R->w      = w;
    R->r      = r;
    R->t      = 2 * (r + 1);
    assert(R->t <= T_MAX);

    switch ( w )  {
        case  8:
            {
#define WORD_T_         byte_t
#define m_              u.v8.m
#define P_              u.v8.P
#define Q_              u.v8.Q
#define S_              u.v8.S
#define RC5_ENCRYPT_X_  RC5_ENCRYPT_8
#define RC5_DECRYPT_X_  RC5_DECRYPT_8

                CodeBlock_(0xFF, 0xb7, 0x9F);

#undef WORD_T_
#undef m_
#undef P_
#undef Q_
#undef S_
#undef RC5_ENCRYPT_X_
#undef RC5_DECRYPT_X_
            }
            
            break;

        case 16:
            {
#define WORD_T_         ushort_t
#define m_              u.v16.m
#define P_              u.v16.P
#define Q_              u.v16.Q
#define S_              u.v16.S
#define RC5_ENCRYPT_X_  RC5_ENCRYPT_16
#define RC5_DECRYPT_X_  RC5_DECRYPT_16

                CodeBlock_(0xFFFF, 0xb7e1, 0x9e37);

#undef WORD_T_
#undef m_
#undef P_
#undef Q_
#undef S_
#undef RC5_ENCRYPT_X_
#undef RC5_DECRYPT_X_
            }

            break;

        case 32:
            {
#define WORD_T_         ulong_t
#define m_              u.v32.m
#define P_              u.v32.P
#define Q_              u.v32.Q
#define S_              u.v32.S
#define RC5_ENCRYPT_X_  RC5_ENCRYPT_32
#define RC5_DECRYPT_X_  RC5_DECRYPT_32

                CodeBlock_(0xFFFFFFFF, 0xb7e15163, 0x9e3779b9);

#undef WORD_T_
#undef m_
#undef P_
#undef Q_
#undef S_
#undef RC5_ENCRYPT_X_
#undef RC5_DECRYPT_X_
            }

            break;
        default:
            assert(0);
            break;
    }
}
#undef CodeBlock_


int main(int argc, char *argv[])
{
    int         rc          = 0;
    int         i           = 0;
    ulong_t     pt1[2]      = { 0, 0 };
    ulong_t     pt2[2]      = { 0, 0 };
    ulong_t     ct[2]       = { 0, 0 };

    /* Setup parameters */
    RC5_SETUP(&rc5_8_parm,   8, 4);
    RC5_SETUP(&rc5_16_parm, 16, 6);
    RC5_SETUP(&rc5_32_parm, 32, 8);

    printf("%s\n", "RC5-32/8/0 examples:");
    ZEROMEM(pt1);
    ZEROMEM(pt2);
    ZEROMEM(ct);
    for ( i = 1; i < 6; i++ ) {
        /* Initialize pt1 and key pseudorandomly based on previous ct */
        pt1[0] = ct[0];
        pt1[1] = ct[1];
        /* encrypt, and decrypt */
        (#define RC5_ENCRYPT_X_T RC5_ENCRYPT_16_T
        (RC5_ENCRYPT_32_T)(rc5_32_parm->encrypt)(&rc5_32_parm, pt1, ct);
        (RC5_DECRYPT_32_T)(rc5_32_parm->decrypt)(&rc5_32_parm, pt1, ct);
        /* Print out results, checking for decryption failure */
        printf("\n%d.", (int)i);
        printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
               pt1[0], pt1[1], ct[0], ct[1]);
        if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
            printf("DecryptionError!");
            rc++;
        }
    }

    printf("%s\n", "RC5-16/6/0 examples:");
    ZEROMEM(pt1);
    ZEROMEM(pt2);
    ZEROMEM(ct);
    for ( i = 1; i < 6; i++ ) {
        /* Initialize pt1 and key pseudorandomly based on previous ct */
        pt1[0] = ct[0];
        pt1[1] = ct[1];
        /* encrypt, and decrypt */
        (RC5_ENCRYPT_16_T)(rc5_16_parm->encrypt)(&rc5_16_parm, pt1, ct);
        (RC5_DECRYPT_16_T)(rc5_16_parm->decrypt)(&rc5_16_parm, pt1, ct);
        /* Print out results, checking for decryption failure */
        printf("\n%d.", (int)i);
        printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
               pt1[0], pt1[1], ct[0], ct[1]);
        if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
            printf("DecryptionError!");
            rc++;
        }
    }

    printf("%s\n", "RC5-8/4/0 examples:");
    ZEROMEM(pt1);
    ZEROMEM(pt2);
    ZEROMEM(ct);
    for ( i = 1; i < 6; i++ ) {
        /* Initialize pt1 and key pseudorandomly based on previous ct */
        pt1[0] = ct[0];
        pt1[1] = ct[1];
        /* encrypt, and decrypt */
        (RC5_ENCRYPT_8_T)(rc5_8_parm->encrypt)(&rc5_8_parm, pt1, ct);
        (RC5_DECRYPT_8_T)(rc5_8_parm->decrypt)(&rc5_8_parm, pt1, ct);
        /* Print out results, checking for decryption failure */
        printf("\n%d.", (int)i);
        printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
               pt1[0], pt1[1], ct[0], ct[1]);
        if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
            printf("DecryptionError!");
            rc++;
        }
    }

    return rc;
}


/* EOF */
