/**SOF(rc5-hash2.c)**/


#include <stdio.h>
#ifdef __STDC__
# include <string.h>
# include <stdlib.h>
#endif
#undef NDEBUG
#include <assert.h>


#include "rc5-hash2.h"


/*====================================================================*/


/*====================================================================*/
/* Include rc5-hash2.inc several times with different defines         */
/*====================================================================*/
/*--------------------------------------------------------------------*/
/* We use WORD/DWORD in the sence used for PPC64 systems: A pointer   */
/* is a DWORD and we have `lw' and `ld' assembly instructions.        */
/*--------------------------------------------------------------------*/
#define RC5_FULL_MODE
#if C_2 <= MIN_ULONG_SIZE
# define  USE_8_BIT_WORDS
#  include  "rc5-hash2.inc"
# undef   USE_8_BIT_WORDS
#endif
#if C_4 <= MIN_ULONG_SIZE
# define  USE_16_BIT_WORDS
#  include  "rc5-hash2.inc"
# undef   USE_16_BIT_WORDS
#endif
#if C_8 <= MIN_ULONG_SIZE
# define  USE_32_BIT_WORDS
#  include  "rc5-hash2.inc"
# undef   USE_32_BIT_WORDS
#endif
/*====================================================================*/


ulong_t mix_dword(ulong_t x)
{
#if C_8 <= MIN_ULONG_SIZE
    /* Assert (ulong_t ---> ulong_t) is bijectiv: */
    CASRT(sizeof(DWORD_32_T) == sizeof(ulong_t));

    return MIX_DWORD_32(x);
#else
#if C_4 <= MIN_ULONG_SIZE
    /* Assert (ulong_t ---> ulong_t) is bijectiv: */
    CASRT(sizeof(DWORD_16_T) == sizeof(ulong_t));

    return MIX_DWORD_16(x);
#else
#if C_2 <= MIN_ULONG_SIZE
    /* Assert (ulong_t ---> ulong_t) is bijectiv: */
    CASRT(sizeof(DWORD_8_T) == sizeof(ulong_t));

    return MIX_DWORD_8(x);
#endif
#endif
#endif
}


static int usage(CONST char *pgm)
{
    fprintf(stderr, "%s: Usage %s [-t <Unsigned Long Int>|-c|-q <num>]\n", pgm, pgm);
    fprintf(stderr, "\t%s\n", "-c: This will generate an infinate random byte stream to stdout");
    fprintf(stderr, "\t%s\n", "-q <num>: Silently generate <num> random ulong_t Valuess --- a performance test");

    exit(USAGE_EXIT);
}


int main(int argc, char *argv[])
{
    int         rc          = 0;
    int         i           = 0;

    if ( 1>= argc ) {
#if C_8 <= MIN_ULONG_SIZE
        {
            WORD_32_T   pt1[2];
            WORD_32_T   pt2[2];
            WORD_32_T   ct[2];

            printf("\n%s\n", "RC5-32/5/0 examples:");
            ZEROMEM(pt1);
            ZEROMEM(pt2);
            ZEROMEM(ct);
            for ( i = 1; i < 6; i++ ) {
                /* Initialize pt1 and key pseudorandomly based on previous ct */
                pt1[0] = ct[0];
                pt1[1] = ct[1];
                /* encrypt, and decrypt */
                RC5_ENCRYPT_32(pt1, ct);
                RC5_DECRYPT_32(ct, pt2);
                /* Print out results, checking for decryption failure */
                printf("\n%d.", (int)i);
                printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
                       (ulong_t)pt1[0], (ulong_t)pt1[1], (ulong_t)ct[0], (ulong_t)ct[1]);
                if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
                    printf("DecryptionError!");
                    rc++;
                }
            }
        }
#endif
#if C_4 <= MIN_ULONG_SIZE
        {
            WORD_16_T   pt1[2];
            WORD_16_T   pt2[2];
            WORD_16_T   ct[2];

            printf("\n%s\n", "RC5-16/5/0 examples:");
            ZEROMEM(pt1);
            ZEROMEM(pt2);
            ZEROMEM(ct);
            for ( i = 1; i < 6; i++ ) {
                /* Initialize pt1 and key pseudorandomly based on previous ct */
                pt1[0] = ct[0];
                pt1[1] = ct[1];
                /* encrypt, and decrypt */
                RC5_ENCRYPT_16(pt1, ct);
                RC5_DECRYPT_16(ct, pt2);
                /* Print out results, checking for decryption failure */
                printf("\n%d.", (int)i);
                printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
                       (ulong_t)pt1[0], (ulong_t)pt1[1], (ulong_t)ct[0], (ulong_t)ct[1]);
                if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
                    printf("DecryptionError!");
                    rc++;
                }
            }
        }
#endif
#if C_2 <= MIN_ULONG_SIZE
        {
            WORD_8_T  pt1[2];
            WORD_8_T  pt2[2];
            WORD_8_T  ct[2];

            printf("\n%s\n", "RC5-8/5/0 examples:");
            ZEROMEM(pt1);
            ZEROMEM(pt2);
            ZEROMEM(ct);
            for ( i = 1; i < 6; i++ ) {
                /* Initialize pt1 and key pseudorandomly based on previous ct */
                pt1[0] = ct[0];
                pt1[1] = ct[1];
                /* encrypt, and decrypt */
                RC5_ENCRYPT_8(pt1, ct);
                RC5_DECRYPT_8(ct, pt2);
                /* Print out results, checking for decryption failure */
                printf("\n%d.", (int)i);
                printf("\n   plaintext %.8lX %.8lX ---> ciphertext %.8lX %.8lX \n",
                       (ulong_t)pt1[0], (ulong_t)pt1[1], (ulong_t)ct[0], (ulong_t)ct[1]);
                if ( pt1[0] != pt2[0] || pt1[1] != pt2[1] ) {
                    printf("DecryptionError!");
                    rc++;
                }
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
                out = mix_dword(in);
                fprintf(stdout, "0x%lX\n", (ulong_t)out);
            } else if ( 0 == strcmp(*argv, "-c") )  {
                ulong_t i = 0;

                for ( ;;i++ ) {
                    ulong_t res = mix_dword(i);
                    int     j   = 0;

                    for ( j = 0; j < sizeof(res); j++ ) {
                        unsigned char c = res & 0xFF;

                        putc(c, stdout);

                        res >>= BYTE_BITS;
                    }
                }
            } else if ( 0 == strcmp(*argv, "-q") )  {
                long_t  count = 0;
                ulong_t in    = 0;

                argv++;
                if ( NULL == *argv )  {
                    usage(progname);
                }

                count  = strtol(*argv, NULL, C_10);
                while ( 0 < count-- ) {
                    in = mix_dword(in);
                }
                fprintf(stdout, "0x%lX\n", (ulong_t)in);
            } else                                  {
                usage(progname);
            }

            argv++;
        }
    }

    return rc;
}



/**EOF(rc5-hash2.c)**/
