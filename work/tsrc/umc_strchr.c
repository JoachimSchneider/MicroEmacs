#include <stdio.h>
#include <string.h>
#include <assert.h>


#define CASRT(cond)                         \
  extern int casrt_dummy_x_[1];             \
  extern int casrt_dummy_x_[(cond)? 1 : 2]
/**END OF DEFINITION**/

#define ZEROMEM(x)  ( memset(&(x), 0, sizeof(x)) )

#define CONST     const
#define NSTRING   (128)
#define ASRT      assert
#define SIZEOF(x) ( (int)sizeof(x) )


char  *umc_strchr (CONST char * s, int c)
{
    if ( !s || !*s )  {
        return NULL;
    }

    do  {
        if ( c == *s )  {
            return (char *)s;
        }
    } while ( *++s );

    return NULL;
}

int main(int argc, char *argv[])
{
    ASRT(3 == argc);

    fprintf(stdout, "<%s>\n", umc_strchr(argv[1], argv[2][0]));

    return 0;
}


/* EOF */

