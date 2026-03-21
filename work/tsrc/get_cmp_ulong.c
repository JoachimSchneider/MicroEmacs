/* SOF */

#include <stdio.h>
#include <stdlib.h>


static unsigned long int  get_cmp_ulong(char *p)
{
#define B0_ZMASK_     ( (unsigned long int)(-1) ^ 0x00FF )
#define B1_ZMASK_     ( (unsigned long int)(-1) ^ 0xFF00 )
#define BITS_IN_BYTE_ (8)
    unsigned long int res       = 0;
    unsigned long int x         = (unsigned long int)p;
    unsigned long int byte0     = x & 0x00FF;
    unsigned long int byte1     = (x & 0xFF00) >> BITS_IN_BYTE_;
    int               i         = 0;

    /* Swap byte0 and byte1 because byte0 might be restricted by
     * alignement conditions */
    x = (x & B0_ZMASK_) | byte1;
    x = (x & B1_ZMASK_) | (byte0 << BITS_IN_BYTE_);

    /* Revert order of bytes in x:  */
    for ( i = 0; i < sizeof(x); i++ ) {
        unsigned long int res_byte  = x & 0xFF;

        res <<= BITS_IN_BYTE_;
        x   >>= BITS_IN_BYTE_;

        res |= res_byte;
    }

    return res;
#undef B0_ZMASK_
#undef B1_ZMASK_
#undef BITS_IN_BYTE_
}


int main(int argc, char *argv[])
{
    unsigned long int in  = 0;
    unsigned long int out = 0;

    if ( 2 != argc )  {
        fprintf(stderr, "Usage %s <Hex Number>\n", argv[0]);

        return 42;
    }
    
    in  = strtoul(argv[1], NULL, 16);
    out = get_cmp_ulong((char *)in);
    fprintf(stdout, "0x%016lX\n", out);

    return 0;
}


/* EOF */
