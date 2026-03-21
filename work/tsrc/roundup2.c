/* SOF */


#include <stdio.h>
#include <stdlib.h>


/* Return the smallest power of 2 which is .GT. x */
static int roundup2a(int x)
{
    unsigned int  q = (unsigned int)x;
    unsigned int  n = 1;

    if ( x < 0 )  {
        return ( 0 );
    }

    while ( 0 != q )  {
        q >>= 1;
        n <<= 1;
    }

    return (int)n;
}

/* Return the smallest power of 2 which is .GE. x */
static int roundup2b(int x)
{
    if ( x < 0 )          {
        return ( 0 );
    } else if ( x == 0 )  {
        return ( 1 );
    }

    return roundup2a(x - 1);
}

int main(int argc, char *argv[])
{
    int x = 0;
    if ( argc != 2 )  {
        fprintf(stderr, "Usage: %s <Integer>\n", argv[0]);

        return 42;
    }

    x = atoi(argv[1]);

    fprintf(stdout, "roundup2a(x) = %d, roundup2b(x) = %d\n",
            roundup2a(x), roundup2b(x));

    return 0;
}


/* EOF */
