
/* SOF(permute.c) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CONST const

typedef unsigned char     byte_t;
typedef unsigned int      uint_t;
typedef long int          long_t;
typedef unsigned long int ulong_t;


static CONST long_t a = 16807L;
static CONST long_t m = 2147483647L;
static CONST long_t q = 127773L;
static CONST long_t r = 2836L;


/* ERND:
 *
 * Returns a random integer
 *
 *
 * This function implements the "minimal standard" RNG from the paper "RNGs:
 * Good Ones are Hard to Find" by Park and Miller, CACM, Volume 31, Number 10,
 * October 1988.
 *
 * See also William. H. Press et.al.: Numerical Recipes in C, 2nd Ed.
 * Cambridge 1992, Chapter 7.1:
 *
 * - m = 2^31 - 1, the function uses Schrage's algorithm.
 *
 * - seed must be an integer int the range 1 .. m - 1
 *
 * - It returns an integer in the range 1 ... m - 1 which fits into the
 *   range of a signed four byte integer.
 *
 * - It has the maximum possible period of m - 1
 */
long_t  ernd0(long_t seed)
{
    long_t lo = 0, hi = 0, test = 0;

    hi = seed / q;
    lo = seed % q;
    test = a * lo - r * hi;
    seed = (test > 0) ? test : test + m;

    return seed;
}


ulong_t ernd1(ulong_t seed)
{
    if        ( 0 == seed )       {
        return m/2;
    } else if ( m == seed )       {
        return m/2 + 1;
    } else if ( m/2 == seed )     {
        return 0;
    } else if (m/2 + 1 == seed )  {
        return m;
    } else                        {
        return ernd0(seed);
    }
}

#define BITS_IN_BYTE  (8)
/* Only defined for 0 <= pos < sizeof(ulong_t)    */
#define BYTE(pos, l)  ( ((ulong_t)(l) & ((ulong_t)0xFF << (BITS_IN_BYTE * (pos)))) >> (BITS_IN_BYTE * (pos)) )
/* CASRT( 0 == sizeof(ulong_t) % 2 );             */
/* Only defined for 0 <= pos < sizeof(ulong_t)/2  */
#define DBYTE(pos, l) ( ((ulong_t)(l) & ((ulong_t)0xFFFF << (2 * BITS_IN_BYTE * (pos)))) >> (2 * BITS_IN_BYTE * (pos)) )
#define ROR(x)        ( ((ulong_t)(x) >> BITS_IN_BYTE) | (BYTE(0, (x)) << (BITS_IN_BYTE * (sizeof(ulong_t) - 1))) )
#define ROL(x)        ( ((ulong_t)(x) << BITS_IN_BYTE) | BYTE(sizeof(ulong_t) - 1, (x)) )
#define rol(x)        ( (((ulong_t)(x) & 0x7FFF) << 1) | (((ulong_t)(x) & 0x8000) >> 15) )
#define ROR4(x)       ( ((ulong_t)(x) >> BITS_IN_BYTE) | (BYTE(0, (x)) << (BITS_IN_BYTE * 3)) )
ulong_t ernd2(ulong_t seed)
{
    ulong_t temp  = ernd1(seed);

    temp  = BYTE(2, temp) | BYTE(3, temp) | BYTE(0, temp) | BYTE(1, temp);

    return ernd1(temp);
}

/*====================================================================== 
 * In 1978 P. Grogono published an introductory Pascal text and included
 * in it a random number generator defined by
 * 
 * f(z) = (25173 * z + 13849) mod 2^16, 0 <= z <= 2^16 - 1
 * 
 * Grogono, P.: Programming in Pascal. 2nd Ed. Addison-Wesley, Reading,
 * Mass., 1984, pp. 135--137.
 * 
 * as cited in
 * 
 * Stephen K. Park and Keith W. Miller: Random Number Generators: Good
 * Ones Are Hard To Find
 * Commun. ACM 31, 10 (October 1988), 1192 -- 1201.
 *
 * It has a full period of 65536 (verified by experiment).
 *====================================================================*/

ulong_t ernd3(ulong_t seed)
{
    CONST ulong_t a = 25173;
    CONST ulong_t c = 13849;
    CONST ulong_t m = (ulong_t)1 << 16;

    assert( m > seed );

    return (a * seed + c) % m;
}

#define NTAB      (32)
#define NTAB_I     (1)
#define MAX_iy    (((ulong_t)1 << 16) - 1)
#define NELEM(a)  ( sizeof((a))/sizeof(*(a)) )
ulong_t ernd4(ulong_t seed)
{
    static int      firstcall = 1;
    static ulong_t  iv[NTAB];
    ulong_t         iy        = 0;
    ulong_t         ret       = 0;
    int             j         =0;

    assert(0 <= seed);
    assert(seed <= MAX_iy);

    if ( firstcall )  {
        int i = 0;

        ulong_t x = seed;

        for( i = 0; i < NELEM(iv); i++ )  {
            iv[i] = x = ernd3(x);
        }

        firstcall = 0;
    }
    iy  = iv[0];
    j = iy * (NELEM(iv) - 1)/ MAX_iy;
    ret = iv[j];
    iv[j] = ernd3(seed);

    return ret;
}

ulong_t ernd5(ulong_t seed)
{
    ulong_t k = ernd3(seed);
    
    k = ((ernd3(k) * ernd3(seed)) >> 16);
    
    return k;
}

int main0(int argc, char *argv[])
{
    long_t  seed  = 1;
    ulong_t n     = 1;

    for ( ;; n++ )  {
        if  ( 1 == (seed = ernd0(seed)) ) {
            break;
        }
    }
    fprintf(stdout, "n = %lu\n", n);
    
    return 0;
}

int main1(int argc, char *argv[])
{
    ulong_t l   = 0;
    int     pos = 0;

    if ( 3 <= argc )  {
        pos = (int)strtol(argv[1], NULL, 10);
        l = strtoul(argv[2], NULL, 16);
        fprintf(stdout, "l = 0x%016lX, BYTE = 0x%02X\n",  l, (uint_t)BYTE(pos, l));
        fprintf(stdout, "l = 0x%016lX, DBYTE = 0x%04X\n", l, (uint_t)DBYTE(pos, l));
        fprintf(stdout, "ROR(0x%016lX) = 0x%016lX\n", l, ROR(l));
    }

    return 0;
}

int main2(int argc, char *argv[])
{
    ulong_t seed  = 0;
    ulong_t n     = 1;

    for ( ;; n++ )  {
        if  ( 0 == (seed = ernd2(seed)) ) {
            break;
        }
        printf("Seed = %lu\n", seed);
    }
    fprintf(stdout, "n = %lu\n", n);
    
    return 0;
}

int main3(int argc, char *argv[])
{
    ulong_t seed  = 0;
    ulong_t n     = 1;

    for ( ;; n++ )  {
        if  ( 0 == (seed = ernd3(seed)) ) {
            break;
        }
        printf("Seed = %lu\n", seed);
    }
    fprintf(stdout, "n = %lu\n", n);
    
    return 0;
}

int main(int argc, char *argv[])
{
    ulong_t seed  = 0;
    ulong_t n     = 1;

    for ( ;; n++ )  {
        if  ( 0 == (seed = ernd5(seed)) ) {
            break;
        }
        printf("Seed = %lu\n", seed);
    }
    fprintf(stdout, "n = %lu\n", n);
    
    return 0;
}

ulong_t scrumble(ulong_t in)
{
    CONST int ror_count  = 1/*sizeof(ulong_t)*/;
    CONST int dbyte_count = sizeof(ulong_t)/2;
    CONST int rnd_count   = 42;

    ulong_t   temp  = 0;

    int       i     = 0;

    for ( i = 0; i < ror_count; i++ )  {
        int j = 0;

        for ( j = 0; j < dbyte_count; j++ ) {
            ulong_t x = DBYTE(j, in);
            int     k = 0;

            for ( k = 0; k < rnd_count; k++ ) {
                x = ernd3(x);
            }
            temp  |= (x << (j * 2 * BITS_IN_BYTE));
            fprintf(stderr, "temp(0) = 0x%016lX\n", temp);
        }
        temp  = ROL(temp);
        fprintf(stderr, "temp(1) = 0x%016lX\n", temp);
    }

    return temp;
}

int main4(int argc, char *argv[])
{
    char  buf[1024];

    memset(buf, 0, sizeof(buf));
    
    while ( NULL != fgets(buf, sizeof(buf), stdin) )
    {
        ulong_t l = 0;
        sscanf(buf, " %lx ", &l);

        fprintf(stdout, "0x%016lX: 0x%016lX\n", l, scrumble(l));
    }
    
    return 0;
}


/* SOF(permute.c) */
