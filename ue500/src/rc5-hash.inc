/*====================================================================*/
/* SOF(rc5-hash2.inc)                                                 */
/*====================================================================*/



/*============================================================*/
/* Ronald L. Rivest: The RC5 Encryption Algorithm.            */
/*                                                            */
/* MIT Laboratory for Computer Science                        */
/* 545 Technology Square, Cambridge, Mass. 02139              */
/* rivest @theory.its.mit.edu                                 */
/*------------------------------------------------------------*/
/* RC5REF.C -- Reference implementation of RC5-32/12/16 in C. */
/* Copyright (C) 1995 RSA Data Security, Inc.                 */
/* R.L. Rivest                                                */
/*============================================================*/


/*====================================================================*/
/* Stripped down version: RC5-##/05/00, ## = 32, 16, 8. A key length  */
/* of zero (i.e. no key) is used as we are only interested in getting */
/* *one* of the many possible permutations of { 0, .., 2^##-1 }.      */
/* The RC5_DECRYPT() is only included if `RC5_FULL_MODE' is defined:  */
/* It's pure existence proves that RC5_ENCRYPT is bijectiv.           */
/*====================================================================*/
#if ( ! defined(USE_8_BIT_WORDS)  &&      \
      ! defined(USE_16_BIT_WORDS) &&      \
      ! defined(USE_32_BIT_WORDS) )
  CRASH();
#endif


/*--------------------------------------------------------------------*/
/* These helper macros won't change during consecutive inclusions     */
/* --- no need to undefine and redefine them with every include.      */
/*--------------------------------------------------------------------*/
#ifndef RC5_HASH_INC_
# define RC5_HASH_INC_
/*....................................................................*/

#define NUM_ROUNDS    (C_5)
#define TAB_SIZE      (2 * (NUM_ROUNDS + 1))


/* Rotation operators, x must be unsigned, to get logical right shift */
#define ROTL_(x,y, w)  ( ((x)<<((y)&((w)-1))) | ((x)>>((w)-((y)&((w)-1)))) )
#define ROTR_(x,y, w)  ( ((x)>>((y)&((w)-1))) | ((x)<<((w)-((y)&((w)-1)))) )
#define ADD_(x, y, m)  ( ((x) + (y)) & (m) )
#define SUB_(x, y, m)  ( ((x) - (y)) & (m) )

#define ROTL(x,y)     ROTL_((x),(y), WORD_BITS)
#define ROTR(x,y)     ROTR_((x),(y), WORD_BITS)
#define ADD(x, y)     ADD_((x), (y), WORD_MASK)
#define SUB(x, y)     SUB_((x), (y), WORD_MASK)


static char *xmemset_(char *cp, int c, int len)
{
    int   i   = 0;

    for ( i = 0; i < len; i++ ) {
        ((unsigned char *)cp)[i]  = (unsigned char)c;
    }

    return cp;
}
#define xmemset(cp, c, len)   xmemset_((char *)(cp), (c), (len))


#endif  /**RC5_HASH_INC_**/
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/* These macros will be redefined with every include:                 */
/*--------------------------------------------------------------------*/

/*====================================================================*/
/* We use WORD/DWORD in the sence used for PPC64 systems: A pointer   */
/* is a DWORD and we have `lw' and `ld' assembly instructions.        */
/*====================================================================*/

#if defined(USE_8_BIT_WORDS)
# define WORD_BITS    (C_8)
# define WORD_MASK    ((WORD_T_)0xFF)
# define MAGIC_P      ((WORD_T_)0xb7)
# define MAGIC_Q      ((WORD_T_)0x9f)
# define WORD_T_      byte_t
# define DWORD_T_     ushort_t

# define RC5_INIT     RC5_INIT_8
# define RC5_ENCRYPT  RC5_ENCRYPT_8
# define RC5_DECRYPT  RC5_DECRYPT_8
# define MIX_DWORD    MIX_DWORD_8

typedef WORD_T_       WORD_8_T;
typedef DWORD_T_      DWORD_8_T;
#else
#if defined(USE_16_BIT_WORDS)
# define WORD_BITS    (C_16)
# define WORD_MASK    ((WORD_T_)0xFFFF)
# define MAGIC_P      ((WORD_T_)0xb7e1)
# define MAGIC_Q      ((WORD_T_)0x9e37)
# define WORD_T_      ushort_t
# if b_IS_ANSI_C
#  undef  MIN_ULONG_MAX
#  define MIN_ULONG_MAX   4294967295UL          /* 32-Bit max ulong_t */
#  include <limits.h>                           /* ANSI-C has it!     */
#  if  MIN_ULONG_MAX <= UINT_MAX                /* uint_t has 32 Bits */
#   define DWORD_T_   uint_t
#  else
#   define DWORD_T_   ulong_t
#  endif
# else
#  define DWORD_T_    ulong_t
# endif

# define RC5_INIT     RC5_INIT_16
# define RC5_ENCRYPT  RC5_ENCRYPT_16
# define RC5_DECRYPT  RC5_DECRYPT_16
# define MIX_DWORD    MIX_DWORD_16

typedef WORD_T_       WORD_16_T;
typedef DWORD_T_      DWORD_16_T;
#else
#if defined(USE_32_BIT_WORDS)
# define WORD_BITS   (C_32)
# define WORD_MASK   ((WORD_T_)0xFFFFFFFF)
# define MAGIC_P     ((WORD_T_)0xb7e15163)
# define MAGIC_Q     ((WORD_T_)0x9e3779b9)
# if b_IS_ANSI_C
#  undef  MIN_ULONG_MAX
#  define MIN_ULONG_MAX   4294967295UL          /* 32-Bit max ulong_t */
#  include <limits.h>                           /* ANSI-C has it!     */
#  if  MIN_ULONG_MAX <= UINT_MAX                /* uint_t has 32 Bits */
#   define WORD_T_    uint_t
#  else
#   define WORD_T_    ulong_t
#  endif
# else
#  define WORD_T_     ulong_t
# endif
# define DWORD_T_     ulong_t

# define RC5_INIT     RC5_INIT_32
# define RC5_ENCRYPT  RC5_ENCRYPT_32
# define RC5_DECRYPT  RC5_DECRYPT_32
# define MIX_DWORD    MIX_DWORD_32

typedef WORD_T_       WORD_32_T;
typedef DWORD_T_      DWORD_32_T;
#endif
#endif
#endif

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/


CASRT(sizeof(DWORD_T_) * BITS_IN_BYTE >= 2 * WORD_BITS);


/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/

static VOID RC5_INIT(WORD_T_ *S)
{
    int     i = 0;
    int     k = 0;
    WORD_T_ A = 0;
    WORD_T_ B = 0;

    xmemset(S, 0, sizeof(WORD_T_) * TAB_SIZE);
    for ( S[0] = MAGIC_P, i = 1; i < TAB_SIZE; i++ ) {
        S[i] = ADD(S[i-1], MAGIC_Q);
    }
    for ( A = B = i = k = 0;
          k < 3 * TAB_SIZE;
          k++, i = (i + 1) % TAB_SIZE )
    {
        A = S[i] = ROTL(ADD(S[i], ADD(A, B)), C_3);
        B =        ROTL(ADD(A, B), ADD(A, B));
    }
}

static VOID RC5_ENCRYPT(WORD_T_ *pt,  /* 2 Word input pt  */
                        WORD_T_ *ct   /* 2 Word output ct */
                       )
{
    static int      valid = FALSE;  /* boolean: data have been set      */
    static WORD_T_  S[TAB_SIZE];    /* expanded key table               */

    if ( !valid ) {
        RC5_INIT(S);
        valid = TRUE;
    }
    {
        int             i     = 0;
        WORD_T_         A     = ADD(pt[0], S[0]);
        WORD_T_         B     = ADD(pt[1], S[1]);

        for ( i = 1; i <= NUM_ROUNDS; i++ ) {
            A = ADD(ROTL(A ^ B, B), S[2*i]);
            B = ADD(ROTL(B ^ A, A), S[2*i+1]);
        }
        ct[0] = A;
        ct[1] = B;
    }
}

#ifdef RC5_FULL_MODE
static VOID RC5_DECRYPT(WORD_T_ *ct,  /* 2 Word input ct  */
                        WORD_T_ *pt   /* 2 Word output pt */
                       )
{
    static int      valid = FALSE;  /* boolean: data have been set      */
    static WORD_T_  S[TAB_SIZE];    /* expanded key table               */

    if ( !valid ) {
        RC5_INIT(S);
        valid = TRUE;
    }
    {
        int     i = 0;
        WORD_T_ A = ct[0];
        WORD_T_ B = ct[1];

        for ( i = NUM_ROUNDS; i > 0; i-- )  {
            B = ROTR(SUB(B, S[2*i+1]), A) ^ A;
            A = ROTR(SUB(A, S[2*i]),   B) ^ B;
        }
        pt[0] = SUB(A, S[0]);
        pt[1] = SUB(B, S[1]);
    }
}
#endif/**RC5_FULL_MODE**/


/* MIX_DWORD:
 *
 * A bijective hash function (i.e. a permutation) on
 * (DWORD_T ---> DWORD_T): We may use it for e.g.
 * - Hashing ordered numbers into "white noise" to
 *   store them into trees that won't degenerate
 *   into lists.
 * - Generate high quality randum numbers by applying
 *   the MIX_DWORD to { n, n+1, n+2, ... }
 */
static DWORD_T_ MIX_DWORD(DWORD_T_ x)
{
    DWORD_T_  res = 0;
    WORD_T_   in[2];
    WORD_T_   out[2];

    in[1] = (x >> WORD_BITS);
    in[0] = x & WORD_MASK;

    RC5_ENCRYPT(in, out);

    res |=  out[1];
    res <<= WORD_BITS;
    res |=  out[0];

    return res;
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/* Undefine these macros to be ready for the next include ...         */
/*--------------------------------------------------------------------*/
#undef WORD_BITS
#undef WORD_MASK
#undef MAGIC_P
#undef MAGIC_Q
#undef WORD_T_
#undef DWORD_T_

#undef RC5_INIT
#undef RC5_ENCRYPT
#undef RC5_DECRYPT
#undef MIX_DWORD
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/



/*====================================================================*/
/* EOF(rc5-hash2.inc)                                                 */
/*====================================================================*/
