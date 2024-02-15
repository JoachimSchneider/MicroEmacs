#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>


#define REGISTER    register
#define ASRT        assert
#define SIZEOF(x)   ( (int)sizeof(x) )

int xstrcasecmp(const char *s1, const char *s2)
{
    REGISTER int  i = 0;

    ASRT(NULL != s1);
    ASRT(NULL != s2);
  
# define us1_ ( (unsigned char *)s1 )
# define us2_ ( (unsigned char *)s2 )

    for ( i = 0; ; i++ )  {
        REGISTER int  c1  = '\0';
        REGISTER int  c2  = '\0';
        REGISTER int  lc1 = '\0';
        REGISTER int  lc2 = '\0';
        
        c1  = us1_[i];
        c2  = us2_[i];
        lc1 = tolower(c1);
        lc2 = tolower(c2);
        if ( '\0' == c1 || '\0' == c2 || lc1 != lc2 ) {
            return ( lc1 - lc2 );
        }
    }
# undef us1_
# undef us2_
}

int xstrncasecmp(const char *s1, const char *s2, int len)
{
    REGISTER int  i = 0;

    ASRT(NULL != s1);
    ASRT(NULL != s2);
    ASRT(0    <= len);
  
# define us1_ ( (unsigned char *)s1 )
# define us2_ ( (unsigned char *)s2 )

    for ( i = 0; i < len; i++ ) {
        REGISTER int  c1  = '\0';
        REGISTER int  c2  = '\0';
        REGISTER int  lc1 = '\0';
        REGISTER int  lc2 = '\0';
        
        c1  = us1_[i];
        c2  = us2_[i];
        lc1 = tolower(c1);
        lc2 = tolower(c2);
        if ( '\0' == c1 || '\0' == c2 || lc1 != lc2 ) {
            return ( lc1 - lc2 );
        }
    }

    return ( 0 );
# undef us1_
# undef us2_
}

int strcasestart(const char *start, const char *test)
{
    REGISTER int  slen  = 0;
    REGISTER int  tlen  = 0;

    ASRT(NULL != start);
    ASRT(NULL != test);

    slen  = strlen(start);
    tlen  = strlen(test);

    if ( tlen < slen )  {
        return ( 0 );
    } else              {
        return ( 0 == xstrncasecmp(start, test, slen) );
    }
}


int main(int argc, char *argv[])
{
  int res = 0;
  int RES = 0;
  int len = 0;

  ASRT(4 == argc);
  
  res = xstrcasecmp(argv[1], argv[2]);
  len = atoi(argv[3]);
  RES = xstrncasecmp(argv[1], argv[2], len);

  fprintf(stdout, "strcasestart(\"ABC\", %s): %d\n",
      argv[1], strcasestart("ABC", argv[1]));
  fprintf(stdout, " xstrcasecmp(%s, %s)     = %d\n", argv[1], argv[2], res);
  fprintf(stdout, "xstrncasecmp(%s, %s, %d) = %d\n", argv[1], argv[2], len, RES);

  return 0;
}  


/* EOF */
