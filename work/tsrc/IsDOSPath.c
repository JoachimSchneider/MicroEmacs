#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define ISALPHA(x)  isalpha((unsigned char)(x))
#define CONST const


/* ISDOSPATH:
 *
 * Classify path:
 * .GT. 0:  DOS Path
 * .EQ. 0:  Unknown
 * .LT. 0:  UNIX Path
 */
static int  IsDOSPath (CONST char * path)
{
    int         len       = 0;
    CONST char  *pFSlash  = NULL; /* Points to '/'  */
    CONST char  *pBSlash  = NULL; /* Points to '\\' */

    if ( NULL == path || '\0' == *path )  {
        return 0;
    }

    len = strlen(path);
    assert(1 <= len);

    if        ( '\\' == *path ) {
        return  ( 1 );
    } else if ( '/' == *path )  {
        return  ( -1 );
    } else if ( 1 == len )      {
        return ( 0 );
    }


    /* 2 .GE. len .AND. *path .NOT. .IN. { '\\', '/' }  */

    /* Path with drive letter, e.g. "C:": */
    if  ( ISALPHA(*path) && ':' == *(path + 1) )  {
        return  ( 1 );
    }

    pFSlash = strchr(path, '/');
    pBSlash = strchr(path, '\\');
    if ( NULL == pFSlash  ) {
        if ( NULL == pBSlash )  {
            return ( 0 );
        } else                  {
            return ( 1 );
        }
    } else                  {
        if ( NULL == pBSlash )  {
            return ( -1 );
        } else                  {
            /* '/' and '\\' occur in path:  */
            if ( pBSlash < pFSlash )  {
                return ( 1 );
            } else if ( pBSlash > pFSlash ) {
                return ( - 1 );
            } else                          {
                return ( 0 );
            }
        }
    }
}


int main(int argc, char *argv[])
{
    int res = 0;

    res = IsDOSPath(argv[1]);

    if ( 0 < res )        {
        printf("%s\n", "DOS");
    } else if ( 0 > res ) {
        printf("%s\n", "UNX");
    } else                {
        printf("%s\n", "---");
    }

    return res;
}



/* EOF */

