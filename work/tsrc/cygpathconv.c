#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>


#define REGISTER    register
#define ASRT(x)     do { if ( !(x) ) { abort(); } } while ( 0 )
#define SIZEOF(x)   ( (int)sizeof(x) )
#define NFILEN      (255)
#define ZEROMEM(x)  ( memset(&(x), 0, SIZEOF(x)) )
#define ISALPHA     isalpha
#define ISSPACE     isspace
#define CONST       const
#define xsnprintf   snprintf
#define xstrlcpy(dst, src, dstsiz)  ( strcpy((dst), (src)) )
#define xstrlcat(dst, src, dstsiz)  ( strcat((dst), (src)) )
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


/*======================================================================
 * Naming convention for DOS, CygWin path names:
 *----------------------------------------------------------------------
 *   Type of Path         Example                           Mnemonic
 *----------------------------------------------------------------------
 * - Absolute DOS Path:   Something like `C:\bin'           ads
 * - Relative DOS Path:   Something like `bin'              rds
 * - Rooted DOS Path:     Something like `\bin'             tds
 * - Absolute UNIX Path:  Something like `/var'             anx
 * - Relative UNIX Path:  Something like `var'              rnx
 * - Expanded UNIX Path:  Something like `/cygdrive/d/usr'  enx
 *
 * o There is a bijective mapping between ads and enx
 * o There are aunx paths with no coresponding  {x,r,t}dos path
 *   e.g `/cygdrive', '/proc'
 *====================================================================*/



/*======================================================================
 * Remarks
 *----------------------------------------------------------------------
 * - In the coding below we only use the *simples string functions and
 *   avoid the use of e.g. `xsnprintf()' as its implementation here
 *   might(!) indirectly call one of the cyg*-functions.
 * - We assert that
 *    (i) It is always possible to determine the current working
 *        directory
 *   (ii) It is always possible to swicht back to a directory one came
 *        from
 *   Of course (ii) might be violated by a second process or thread
 *   changing the directory permissions.
 *====================================================================*/


/*======================================================================
 * Features
 *====================================================================*/

#define CYGWIN_USE_WIN32_GETCURDIR  (!0)


/*====================================================================*/
#ifndef __CYGWIN__
# undef   CYGWIN_USE_WIN32_GETCURDIR
# define  CYGWIN_USE_WIN32_GETCURDIR  (0)
#endif
/*====================================================================*/


/*======================================================================
 * Feature dependent includes
 *====================================================================*/
#if ( CYGWIN_USE_WIN32_GETCURDIR  )
# include <w32api/windows.h>
#endif
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

/* Will not fail  */
static CONST char *cygdrive_(void)
{
    static char res[NFILEN];
    static int  FirstCall = !0;

    if ( FirstCall )  {
        CONST char  drives[]  = "CDEFGHIJKLMNOPQRSTUVWXYZAB";
        char        cwd[NFILEN];
        int         i         = 0;
        int         l         = 0;

        ZEROMEM(cwd);

        if ( NULL == getcwd(cwd, SIZEOF(cwd)) ) {
            goto notfound;
        }
        /*------------------------------------------------------------*/

        for ( i = 0; i < SIZEOF(drives) - 1; i++ )  {
            char  dir[4];

            dir[0]  = drives[i];
            dir[1]  = ':';
            dir[2]  = '/';
            dir[3]  = '\0';

            ZEROMEM(res);
            if ( 0 == chdir(dir) )  {
                if ( NULL != getcwd(res, SIZEOF(res)) ) {
                    break;
                }
            }
        }

        /*------------------------------------------------------------*/
        if ( 0 != chdir(cwd) )  {
            goto notfound;
        }

        if ( SIZEOF(drives) - 1 <= i )  {
            goto notfound;
        }

        if ( 0 == (l = strlen(res)) ) {
            goto notfound;
        }

        res[l - 1]  = '\0';

        goto found;
notfound:
        /* This is the default: */
        xstrlcpy(res, "/cygdrive/", SIZEOF(res));
found:

        FirstCall = 0;
    }

    return res;
}

static int cygdrive_len_(void)
{
    static  int len = (-1);

    if ( 0 > len )  {
        len = strlen(cygdrive_());
    }

    return len;
}

#if ( !0 )
# define CYGDRIVE_      ( cygdrive_() )
# define CYGDRIVE_LEN_  ( cygdrive_len_() )
#else
# define CYGDRIVE_      "/cygdrive/"
# define CYGDRIVE_LEN_  ( SIZEOF(CYGDRIVE_) - 1 )
#endif

/* Return NULL if dos is not an absolute DOS path */
static CONST char *cygads2enx_(CONST char *dos)
{
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
        int j = 0;
            j = CYGDRIVE_LEN_;

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

    } else                                                  {
        return NULL;
    }

    return res;
}


/* Return NULL if unx is *not* an expanded UNIX path  */
static CONST char *cygenx2ads_(CONST char *unx)
{
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
    } else  {
        return NULL;
    }

    return res;
}

/* Cannot fail  */
static CONST char *cygpwdads(void)
{
    static char res[NFILEN];

    static int          FirstCall     = !0;
    char                cmd[NFILEN];
    static char         prevcwd[NFILEN];
    char                cwd[NFILEN];

    ZEROMEM(cmd);
    ZEROMEM(cwd);

    /* TODO: Implement this function via
     * #include <w32api/windows.h>
     * #include <stdio.h>
     *
     *
     * #define LEN (1024)
     *
     *
     * int main(int argc, char *argv[])
     * {
     *     char  Buffer[LEN];
     *
     *     if ( 0 < GetCurrentDirectoryA(LEN, Buffer) )  {
     *         fprintf(stdout, "<%s>\n", Buffer);
     *     }
     *
     *
     *     return 0;
     * }
     *
     */

    /* Optimize for:
     * - CWD same as previous call
     * - Return old res
     *  We assume that DOS cwd doesn't change if UNIX cwd stays
     *  the same.
     */

    if ( FirstCall )  {
        FirstCall = 0;
    } else            {
        if ( NULL != getcwd(cwd, SIZEOF(cwd))  &&
            0 == strcmp(prevcwd, cwd)  ) {

            return res;
        }
    }

    xstrlcpy(prevcwd, cwd, SIZEOF(prevcwd));
    ZEROMEM(res);

#if ( CYGWIN_USE_WIN32_GETCURDIR  )
    {
        int rc  = 0;


        rc  = GetCurrentDirectoryA(SIZEOF(res), res);
        /* Return values:
         * - Success:         strlen(CurrenDirectory)
         * - Error:           0
         * - Buffer to small: strlen(CurrentDirectory) + 1
         */
        ASRT(0 < rc);
        ASRT(rc < SIZEOF(res));
    }
#else
    {
        FILE        *pp           = NULL;
        CONST char  *unxwinshell  = NULL;

        unxwinshell = cygads2enx_(wingetshell());
        ASRT( NULL != unxwinshell );
        /**To Avoid:
        xsnprintf(cmd, SIZEOF(cmd), "( %s /c cd )", unxwinshell);
        **/
        xstrlcpy(cmd, "( ",         SIZEOF(cmd));
        xstrlcat(cmd, unxwinshell,  SIZEOF(cmd));
        xstrlcat(cmd, " /c cd )",   SIZEOF(cmd));

        ASRT( NULL != (pp = popen(cmd, "r")) );
        {
            enum { start, get } state = start;
            int                 c     = '\0';
            int                 i     = 0;

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
            i = strlen(res) - 1;
            while ( 0 <= i &&  ISSPACE(res[i]) )  {
                res[i--]  = '\0';
            }

            pclose(pp);
        }
    }
#endif

    return res;
}

/* Cannot fail  */
static CONST char *cygrootads(void)
{
    static int  FirstCall = !0;
    static char res[NFILEN];

    if ( FirstCall )  {
        char  cwd[NFILEN];

        ZEROMEM(cwd);

        ZEROMEM(res);
        ASRT( NULL != getcwd(cwd, SIZEOF(cwd)) );
        ASRT( 0 == chdir("/") );
        xstrlcpy(res, cygpwdads(), SIZEOF(res));
        ASRT( 0 == chdir(cwd) );


        FirstCall = 0;
    }

    return res;
}

/* Cannot fail  */
static CONST char *cygrootenx(void)
{
    static int  FirstCall = !0;
    static char res[NFILEN];

    if ( FirstCall )  {
        CONST char  *cp = NULL;

        ZEROMEM(res);
        ASRT( NULL != (cp = cygads2enx_(cygrootads())) );
        xstrlcpy(res, cp, SIZEOF(res));
    }

    return res;
}

/* Cannot fail  */
static CONST char *cygpwdenx(void)
{
    static char res[NFILEN];

    CONST char  *cp = NULL;

    ZEROMEM(res);

    ASRT( NULL != (cp = cygads2enx_(cygpwdads())) );
    xstrlcpy(res, cp, SIZEOF(res));

    return res;
}

/* Cannot fail  */
static int cygdrvdos(void)
{
    return (unsigned char)cygpwdads()[0];
}

/**TODO: Comment out!**/
/* Return DOS path of an *existing* (`xunx') UNIX directory */
/* Return NULL if unx does not exist as a directory         */
static CONST char *cygxunx2ads_(CONST char *unx)
{
    static char res[NFILEN];
    CONST char  *rc = res;
    char        cwd[NFILEN];

    ZEROMEM(res);
    ZEROMEM(cwd);

    ASRT(NULL != unx);

    ASRT( NULL != getcwd(cwd, SIZEOF(cwd)) );

    if ( 0 != chdir(unx) )  {
        rc  = NULL;
    } else                  {
        xstrlcpy(res, cygpwdads(), SIZEOF(res));
    }

    ASRT( 0 == chdir(cwd) );

    return rc;

}

/* Return expanded UNIX path of an *existing* (`xunx') UNIX directory */
/* Return NULL if unx does not exist as a directory                   */
static CONST char *cygxunx2enx_(CONST char *unx)
{
    static char res[NFILEN];
    CONST char  *rc = res;
    char        cwd[NFILEN];

    ZEROMEM(res);
    ZEROMEM(cwd);

    ASRT(NULL != unx);

    ASRT( NULL != getcwd(cwd, SIZEOF(cwd)) );

    if ( 0 != chdir(unx) )  {
        rc  = NULL;
    } else                  {
        xstrlcpy(res, cygpwdenx(), SIZEOF(res));
    }

    ASRT( 0 == chdir(cwd) );

    return rc;

}

/* Return expanded (`/cygdrive/...') UNIX path of an UNIX directory */
/* Cannot fail                                                      */
static CONST char *cygunx2enx(CONST char *unx)
{
    static char res[NFILEN];

    char        cwd[NFILEN];
    static char prevcwd[NFILEN];
    static char prevunx[NFILEN];

    CONST char  *hp   = NULL;
    int         l     = 0;
    int         i     = 0;


    ZEROMEM(cwd);

    ASRT(NULL != unx);

    /* Optimize for:
     * - CWD and unx same as previous call
     * - Return old res
     */
    if ( *res  /* NOT the first call */    &&
         NULL != getcwd(cwd, SIZEOF(cwd))  &&
         0 == strcmp(prevcwd, cwd)         &&
         0 == strcmp(prevunx, unx) ) {
        return res;
    }

    xstrlcpy(prevcwd, cwd, SIZEOF(prevcwd));
    xstrlcpy(prevunx, unx, SIZEOF(prevunx));
    ZEROMEM(res);

    /* Empty input: Return CWD: */
    if ( 0 == (l = strlen(unx)) ) {
        xstrlcpy(res, cygpwdenx(), SIZEOF(res));

        goto found;
    }
    /* Return existing absolute or relative directory:  */
    if ( NULL != (hp = cygxunx2enx_(unx)) )  {
        xstrlcpy(res, hp, SIZEOF(res));

        goto found;
    }
    for ( i = l - 1; i >= 0; i-- )  {
        char  head[NFILEN];
        char  tail[NFILEN];

        ZEROMEM(head);
        ZEROMEM(tail);

        CASRT(SIZEOF(res) == SIZEOF(head));
        CASRT(SIZEOF(res) == SIZEOF(tail));

        if ( '/' == ((unsigned char *)unx)[i] ) {
            int j = 0;

            for ( j = 0; j < i; j ++ )  {
                head[j] = unx[j];
            }
            for ( j = i + 1; j < l; j ++ )  {
                tail[j - i - 1] = unx[j];
            }
            if        ( ! *head ) {
                /* The case of an exisiting `/<something>' was
                 * handled already above.
                 */
                if ( NULL != (hp = cygxunx2enx_("/")) )  {
                    xstrlcpy(res, hp,   SIZEOF(res));
                    xstrlcat(res, "/",  SIZEOF(res));
                    xstrlcat(res, tail, SIZEOF(res));

                    goto found;
                }
            } else if ( ! *tail ) {
                if ( NULL != (hp = cygxunx2enx_(head)) ) {
                    xstrlcpy(res, hp,   SIZEOF(res));

                    goto found;
                }
            } else                {
                if ( NULL != (hp = cygxunx2enx_(head)) ) {
                    xstrlcpy(res, hp,   SIZEOF(res));
                    xstrlcat(res, "/",  SIZEOF(res));
                    xstrlcat(res, tail, SIZEOF(res));

                    goto found;
                }
            }
        }
    }

    /* When we arrive here it cannot be one of these cases:
     * - `/<something>' existing
     * - `/<something>' not existing
     * - `<something>'  existing
     */
    xstrlcpy(res, cygpwdenx(), SIZEOF(res));
    xstrlcat(res, "/", SIZEOF(res));
    xstrlcat(res, unx, SIZEOF(res));

found:
    return res;
}

/* Return absolute DOS path of an UNIX directory  */
/* Cannot fail                                    */
static CONST char *cygunx2ads(CONST char *unx)
{
    ASRT(NULL != unx);

    return cygenx2ads_(cygunx2enx(unx));
}

/* Cannot fail  */
static CONST char *cygdos2enx(CONST char *dos)
{
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
         xstrlcpy(res, cygads2enx_((CONST char *)in), SIZEOF(res));
    } else {
        char xpath[NFILEN];

        ZEROMEM(xpath);

        if ( '\\' == in[0] )  {
            xpath[0]  = cygdrvdos();
            xpath[1]  = ':';
            CASRT(3 <= SIZEOF(xpath));
            xstrlcat(xpath, (CONST char *)in, SIZEOF(xpath));
        } else                {
            int l = 0;

            xstrlcpy(xpath, cygpwdads(), SIZEOF(xpath) - 1 /* '\\' */);
            l = strlen(xpath);
            xpath[l]  = '\\';
            xstrlcat(xpath, (CONST char *)in, SIZEOF(xpath));
        }
        xstrlcpy(res, cygads2enx_(xpath), SIZEOF(res));
    }

    return res;
}


/* Return absolute DOS path of a DOS directory  */
/* Cannot fail                                  */
static CONST char *cygdos2ads(CONST char *dos)
{
    CONST char  *rc = NULL;

    ASRT(NULL != dos);

    ASRT( NULL != (rc = cygenx2ads_(cygdos2enx(dos))) );

    return rc;
}


/*====================================================================*/


int main(int argc, char *argv[])
{
  ASRT(2 == argc);

  fprintf(stdout, "cygdrive_      ===> %s <<\n", cygdrive_());
  fprintf(stdout, "cygdrive_      ===> %s <<\n", cygdrive_());

#define TST_(x) do  {                                               \
  fprintf(stdout, "cygxunx2ads_ ===> %s <<\n", cygxunx2ads_((x)));  \
  fprintf(stdout, "cygxunx2enx_ ===> %s <<\n", cygxunx2enx_((x)));  \
  fprintf(stdout, "cygunx2ads   ===> %s <<\n", cygunx2ads((x)));    \
  fprintf(stdout, "cygunx2enx   ===> %s <<\n", cygunx2enx((x)));    \
  fprintf(stdout, "cygdos2ads   ===> %s <<\n", cygdos2ads((x)));    \
  fprintf(stdout, "cygdos2enx   ===> %s <<\n", cygdos2enx((x)));    \
} while ( 0 )

  fprintf(stdout, "\n%s:\n", ".");
  TST_(argv[1]);

  fprintf(stdout, "\n%s:\n", ".");
  TST_(argv[1]);

  chdir("/tmp");
  fprintf(stdout, "\n%s:\n", "/tmp");
  TST_(argv[1]);

  chdir("/tmp");
  fprintf(stdout, "\n%s:\n", "/tmp");
  TST_(argv[1]);

  chdir("/var/tmp");
  fprintf(stdout, "\n%s:\n", "/var/tmp");
  TST_(argv[1]);

  chdir("/var/tmp");
  fprintf(stdout, "\n%s:\n", "/var/tmp");
  TST_(argv[1]);

  chdir("/usr");
  fprintf(stdout, "\n%s:\n", "/usr");
  TST_(argv[1]);

  chdir("/usr");
  fprintf(stdout, "\n%s:\n", "/usr");
  TST_(argv[1]);


  return 0;
}


/* EOF */
