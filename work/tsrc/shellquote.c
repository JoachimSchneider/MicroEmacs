#include <stdio.h>
#include <string.h>
#include <assert.h>


#define CASRT(cond)                         \
  extern int casrt_dummy_x_[1];             \
  extern int casrt_dummy_x_[(cond)? 1 : 2]
/**END OF DEFINITION**/

#define ZEROMEM(x)  ( memset(&(x), 0, sizeof(x)) )

CASRT(1 == 1);
/*CASRT(1 == 0);*/


#define CONST     const
#define NSTRING   (10)
#define ASRT      assert
#define SIZEOF(x) ( (int)sizeof(x) )


/* SHELLQUOTE:
 *
 * Protect string from Bourne Shell expansion
 */
static char *shellquote(CONST char *in)
{
#define SQREP_  "'\"'\"'"
#define CHKRES_(j, incr)  ( (j) + (incr) <= NSTRING - 1 )
    static char res[NSTRING];
    int i = 0;
    int j = 0;
  
    ZEROMEM(res); /* NECESSARY! */
    ASRT(NULL != in);

    res[j++]  = '\'';
    for ( ;; )  {
        char  c = in[i];

        if ( '\0' == c )        {
            if ( ! CHKRES_(j, 1) )                  return NULL;
            res[j++]  = '\'';
            res[j++]  = c;

            return res;
        } else if ( '\'' != c ) {
            if ( ! CHKRES_(j, 1) )                  return NULL;
            res[j++]  = c;
        } else                  {
            /* res += SQREP_  */
            if ( ! CHKRES_(j, SIZEOF(SQREP_) - 1) ) return NULL;
            strcat(res + j, SQREP_);
            j += SIZEOF(SQREP_) - 1;
        }
        i++;
    }
#undef  SQREP_
#undef  CHKRES_
}


int main(int argc, char *argv[])
{
    ASRT(2 == argc);

    fprintf(stdout, "<%s>\n", shellquote(argv[1]));

    return 0;
}


/* EOF */

