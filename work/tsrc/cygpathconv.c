#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>


#define REGISTER    register
#define ASRT        assert
#define SIZEOF(x)   ( (int)sizeof(x) )
#define NFILEN      (255)
#define ZEROMEM(x)  ( memset(&(x), 0, SIZEOF(x)) )
#define ISALPHA     isalpha
#define ISSPACE     isspace
#define CONST       const
#define xsnprintf   snprintf
#define xstrlcpy(dst, src, dstsiz)  ( strcpy((dst), (src)) )
#define CASRT(x)


int xstrcasecmp(CONST char *s1, CONST char *s2)
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

int xstrncasecmp(CONST char *s1, CONST char *s2, int len)
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

int strcasestart(CONST char *start, CONST char *test)
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


/*====================================================================*/

static CONST char *wingetshell(void)
{
    static char res[NFILEN];

    ZEROMEM(res);

    xsnprintf(res, SIZEOF(res), "%s", "C:\\Windows\\system32\\cmd.exe");

    return res;
}

/*====================================================================*/

#define TO_DOS_SEP_(x)  do  {                   \
    unsigned char *cp__ = (unsigned char *)(x); \
    unsigned char c__   = '\0';                 \
                                                \
    while ( ( c__ = *cp__) )  {                 \
        if ( '/' == c__ ) {                     \
            *cp__ = '\\';                       \
        }                                       \
        cp__++;                                 \
    }                                           \
} while ( 0 )

#define TO_UNX_SEP_(x)  do  {       \
    unsigned char *cp__ = (unsigned char *)(x); \
    unsigned char c__   = '\0';                 \
                                                \
    while ( ( c__ = *cp__) )  {                 \
        if ( '\\' == c__ )  {                   \
            *cp__ = '/';                        \
        }                                       \
        cp__++;                                 \
    }                                           \
} while ( 0 )

static CONST char *cygabsdos2unxpath_(CONST char *dos)
{
#define CYGDRIVE_       "/cygdrive/"
#define CYGDRIVE_LEN_   ( SIZEOF(CYGDRIVE_) - 1 )
    unsigned char in[NFILEN];
    static char   res[NFILEN];
    int           len = 0;
    int           drv = '\0';

    ASRT(NULL != dos);
    ZEROMEM(in);
    ZEROMEM(res);

    xstrlcpy((char *)in, dos, SIZEOF(in));
    TO_DOS_SEP_(in);
    len = strlen((CONST char *)in);

    if ( 2 <= len && ':' == in[1] && ISALPHA(drv = in[0]) ) {
        int i = 2;
        int j = CYGDRIVE_LEN_;

        drv = tolower(drv);
        xstrlcpy(res, CYGDRIVE_, SIZEOF(res));
        res[j++]  = drv;
        if ( 2 == len ) {
            return res;
        }
        res[j++]  = '/';
        CASRT(SIZEOF(res) - 1 >= CYGDRIVE_LEN_ + 1 + 1);
        if ( '\\' == in[i] )  {
            i++;
        }
        for ( ; i < len && j < SIZEOF(res) - 1; i++, j++ )  {
            if ( '\\' == in[i] )  {
                res[j]  = '/';
            } else                {
                res[j]  = in[i];
            }
        }
        
    }

    return res;
# undef CYGDRIVE_LEN_
# undef CYGDRIVE_
}

static CONST char *cygrootdos(void)
{
    static int  FirstCall = !0;
    static char res[NFILEN];

    if ( FirstCall )  {
        int                 c             = '\0';
        FILE                *pp           = NULL;
        CONST char          *unxwinshell  = NULL;
        char                cmd[NFILEN];
        char                doscygroot[NFILEN];
        enum { start, get } state         = start;
        int                 i             = 0;

        ZEROMEM(cmd);
        ZEROMEM(doscygroot);

        unxwinshell = cygabsdos2unxpath_(wingetshell());
        xsnprintf(cmd, SIZEOF(cmd), "( cd / && %s /c cd )", unxwinshell);
        if ( NULL != (pp = popen(cmd, "r")) ) {
            while ( EOF != (c = fgetc(pp)) )  {
                if ( state == start && ISSPACE(c) ) {
                    continue;
                }
                state = get;
                if ( i >= SIZEOF(res) - 1 )   {
                    break;
                }
                if ( '\n' == c || '\r' == c ) {
                    break;
                }
                res[i++]  = c;
            }
            pclose(pp);
        }
        
        FirstCall = 0;
    }

    return res;
}

static CONST char *cygrootunx(void)
{
    static int  FirstCall = !0;
    static char res[NFILEN];

    if ( FirstCall )  {
        xsnprintf(res, SIZEOF(res), "%s", cygabsdos2unxpath_(cygrootdos()));
    }
    
    return res;
}

static CONST char *cygpwddos(void)
{
    static char res[NFILEN];

    int                 c             = '\0';
    FILE                *pp           = NULL;
    CONST char          *unxwinshell  = NULL;
    char                cmd[NFILEN];
    char                doscygroot[NFILEN];
    enum { start, get } state         = start;
    int                 i             = 0;
    static char         prevcwd[NFILEN];
    char                cwd[NFILEN];

    ZEROMEM(cmd);
    ZEROMEM(doscygroot);
    ZEROMEM(cwd);

    /* An optimization:
     *  We assume that DOS cwd doesn't change if UNIX cwd stays
     *  the same.
     */
    if ( NULL != getcwd(cwd, SIZEOF(cwd)) ) {
        if ( 0 == strcmp(prevcwd, cwd) )  {
            return res;
        } else                            {
            xstrlcpy(prevcwd, cwd, SIZEOF(prevcwd));
        }
    }

    unxwinshell = cygabsdos2unxpath_(wingetshell());
    xsnprintf(cmd, SIZEOF(cmd), "( %s /c cd )", unxwinshell);
    if ( NULL != (pp = popen(cmd, "r")) ) {
        while ( EOF != (c = fgetc(pp)) )  {
            if ( state == start && ISSPACE(c) ) {
                continue;
            }
            state = get;
            if ( i >= SIZEOF(res) - 1 )   {
                break;
            }
            if ( '\n' == c || '\r' == c ) {
                break;
            }
            res[i++]  = c;
        }
        pclose(pp);
    }

    return res;
}

static CONST char *cygpwdunx(void)
{
    static int  FirstCall = !0;
    static char res[NFILEN];

    if ( FirstCall )  {
        xsnprintf(res, SIZEOF(res), "%s", cygabsdos2unxpath_(cygpwddos()));
    }
    
    return res;
}

static int cygdrvdos(void)
{
    return (unsigned char)cygpwddos()[0];
}


static CONST char *cygdos2unxpath(CONST char *dos)
{
#define CYGDRIVE_       "/cygdrive/"
#define CYGDRIVE_LEN_   ( SIZEOF(CYGDRIVE_) - 1 )
    unsigned char in[NFILEN];
    static char   res[NFILEN];
    int           len = 0;

    ASRT(NULL != dos);
    ZEROMEM(in);
    ZEROMEM(res);

    xstrlcpy((char *)in, dos, SIZEOF(in));
    TO_DOS_SEP_(in);
    len = strlen((CONST char *)in);
    if ( 2 <= len && ':' == in[1]  && ISALPHA(in[0]) )  {
         xstrlcpy(res, cygabsdos2unxpath_((CONST char *)in), SIZEOF(res));
    } else {
        char xpath[NFILEN];

        ZEROMEM(xpath);
        
        if ( '\\' == in[0] )  {
            xsnprintf(xpath, SIZEOF(xpath), "%c:%s", cygdrvdos(), in);
        } else                {
            xsnprintf(xpath, SIZEOF(xpath), "%s\\%s", cygpwddos(), in);
        }
        xstrlcpy(res, cygabsdos2unxpath_(xpath), SIZEOF(res));
    }

    return res;
# undef CYGDRIVE_LEN_
# undef CYGDRIVE_
}

static CONST char *cygabsunx2dospath_(CONST char *unx)
{
#define CYGDRIVE_       "/cygdrive/"
#define CYGDRIVE_LEN_   ( SIZEOF(CYGDRIVE_) - 1 )
    unsigned char in[NFILEN];
    static char   res[NFILEN];
    int           len = 0;
    int           drv = '\0';

    ASRT(NULL != unx);

    ZEROMEM(in);
    ZEROMEM(res);

    xstrlcpy((char *)in, unx, SIZEOF(in));
    TO_UNX_SEP_(in);
    len = strlen((CONST char *)in);
    if ( strcasestart(CYGDRIVE_, (CONST char *)in)                    &&
         CYGDRIVE_LEN_  < len                                         &&
         ISALPHA(drv = in[CYGDRIVE_LEN_])                             &&
         (CYGDRIVE_LEN_ + 1 == len || '/' == in[CYGDRIVE_LEN_ + 1])
        )   {
        int i = 0;
        int j = 0;

        drv = toupper(drv);
        res[0]  = drv;
        res[1]  = ':';
        res[2]  = '\\';
        CASRT(SIZEOF(res) - 1 >= 3);
        i = CYGDRIVE_LEN_ + 1;
        j = CYGDRIVE_LEN_ + 1 == len ? 3 : 2;
        for ( ; i < len && j < SIZEOF(res) - 1; i++, j++ )  {
            if ( '/' == in[i] ) {
                res[j]  = '\\';
            } else              {
                res[j]  = in[i];
            }
        }
    }

    return res;
# undef CYGDRIVE_LEN_
# undef CYGDRIVE_
}

static CONST char *cygunx2dospath(CONST char *unx)
{
#define CYGDRIVE_       "/cygdrive/"
#define CYGDRIVE_LEN_   ( SIZEOF(CYGDRIVE_) - 1 )
    unsigned char in[NFILEN];
    static char   res[NFILEN];
    int           len = 0;
    int           drv = '\0';

    ASRT(NULL != unx);

    ZEROMEM(in);
    ZEROMEM(res);

    xstrlcpy((char *)in, unx, SIZEOF(in));
    TO_UNX_SEP_(in);
    len = strlen((CONST char *)in);
    
    if ( strcasestart(CYGDRIVE_, (CONST char *)in)                    &&
         CYGDRIVE_LEN_  < len                                         &&
         ISALPHA(drv = in[CYGDRIVE_LEN_])                             &&
         (CYGDRIVE_LEN_ + 1 == len || '/' == in[CYGDRIVE_LEN_ + 1])
        )   {
        xstrlcpy(res, cygabsunx2dospath_((CONST char *)in), SIZEOF(res));
    } else {
        char  xpath[NFILEN];

        ZEROMEM(xpath);

        if ( in[0] == '/' ) {
            xsnprintf(xpath, SIZEOF(xpath), "%s%s",  cygrootunx(), in);
        } else                {
            xsnprintf(xpath, SIZEOF(xpath), "%s/%s", cygpwdunx(),  in);
        }
        xstrlcpy(res, cygabsunx2dospath_(xpath), SIZEOF(res));
    }

    return res;
# undef CYGDRIVE_LEN_
# undef CYGDRIVE_
}


int main(int argc, char *argv[])
{
  ASRT(2 == argc);

  fprintf(stdout, "===> %s <<\n", cygunx2dospath(argv[1]));
  fprintf(stdout, "===> %s <<\n", cygdos2unxpath(argv[1]));

  return 0;
}


/* EOF */
