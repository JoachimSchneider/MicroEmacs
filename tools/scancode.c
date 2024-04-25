/*
 * Print Keyboard Codes, Quit with 'Q'  (Joachim Schneider, August 2003)
 */

/*=====================================================================

The ASCII(7) man-page:

       Oct   Dec   Hex   Char           Oct   Dec   Hex   Char
       ------------------------------------------------------------
       000   0     00    NUL '\0'       100   64    40    @
       001   1     01    SOH            101   65    41    A
       002   2     02    STX            102   66    42    B
       003   3     03    ETX            103   67    43    C
       004   4     04    EOT            104   68    44    D
       005   5     05    ENQ            105   69    45    E
       006   6     06    ACK            106   70    46    F
       007   7     07    BEL '\a'       107   71    47    G
       010   8     08    BS  '\b'       110   72    48    H
       011   9     09    HT  '\t'       111   73    49    I
       012   10    0A    LF  '\n'       112   74    4A    J
       013   11    0B    VT  '\v'       113   75    4B    K
       014   12    0C    FF  '\f'       114   76    4C    L
       015   13    0D    CR  '\r'       115   77    4D    M
       016   14    0E    SO             116   78    4E    N
       017   15    0F    SI             117   79    4F    O
       020   16    10    DLE            120   80    50    P
       021   17    11    DC1            121   81    51    Q
       022   18    12    DC2            122   82    52    R
       023   19    13    DC3            123   83    53    S
       024   20    14    DC4            124   84    54    T
       025   21    15    NAK            125   85    55    U
       026   22    16    SYN            126   86    56    V
       027   23    17    ETB            127   87    57    W
       030   24    18    CAN            130   88    58    X
       031   25    19    EM             131   89    59    Y
       032   26    1A    SUB            132   90    5A    Z
       033   27    1B    ESC            133   91    5B    [
       034   28    1C    FS             134   92    5C    \   '\\'
       035   29    1D    GS             135   93    5D    ]
       036   30    1E    RS             136   94    5E    ^
       037   31    1F    US             137   95    5F    _
       040   32    20    SPACE          140   96    60    `
       041   33    21    !              141   97    61    a
       042   34    22    "              142   98    62    b
       043   35    23    #              143   99    63    c

       044   36    24    $              144   100   64    d
       045   37    25    %              145   101   65    e

       046   38    26    &              146   102   66    f
       047   39    27    '              147   103   67    g

       050   40    28    (              150   104   68    h
       051   41    29    )              151   105   69    i
       052   42    2A    *              152   106   6A    j
       053   43    2B    +              153   107   6B    k
       054   44    2C    ,              154   108   6C    l
       055   45    2D    -              155   109   6D    m
       056   46    2E    .              156   110   6E    n
       057   47    2F    /              157   111   6F    o
       060   48    30    0              160   112   70    p
       061   49    31    1              161   113   71    q
       062   50    32    2              162   114   72    r
       063   51    33    3              163   115   73    s
       064   52    34    4              164   116   74    t
       065   53    35    5              165   117   75    u
       066   54    36    6              166   118   76    v
       067   55    37    7              167   119   77    w
       070   56    38    8              170   120   78    x
       071   57    39    9              171   121   79    y
       072   58    3A    :              172   122   7A    z
       073   59    3B    ;              173   123   7B    {
       074   60    3C    <              174   124   7C    |
       075   61    3D    =              175   125   7D    }
       076   62    3E    >              176   126   7E    ~
       077   63    3F    ?              177   127   7F    DEL


  =====================================================================*/

#ifdef  __VMS
# include "terminal_vms.h"
#else
# include <termios.h>
#endif
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>


#define PROGNAME      "scancodes"
#define USAGE_EXIT    (42)
#define ERROR_EXIT    (43)
/*
 * This should fit on one line of the screen (worst case: Character needs
 * 3 letters ($FA), line number is 9 digits + ": "
 */
#ifdef  __VMS
/* We must use `1' as long as we do not read with timeout (IO$M_TIMED flag) */
# define RDBUFSIZ     ((80-(9+2))/3)
#else
# define RDBUFSIZ     ((80-(9+2))/3)
#endif

#define CTRL_CHRS     ("@ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define MAX_CTRL_CHRS (sizeof(CTRL_CHRS) - 2)

#define ESCAPE        (0x1B)
#define SPACE         (' ')

#define nl()          ( fputs("\r\n", stdout) )


void usage(void)
{
    fprintf(stderr,
            "%s: Usage: %s [--decode]\n"
            "This will print the scan-codes of your controlling terminal\n"
            "Specials:\n"
            "  $[0-F][0-F]    --> Hexadecimal Character Code\n"
            "  ^[@A-Z]        --> Control Character\n"
            "  \\E             --> Escape\n"
            "  $$             --> $\n"
            "  ^^             --> ^\n"
            "  \\\\             --> \\\n"
            "May also be used to dump binary files:\n"
            "  When dumping from a non-tty the lines (except the last\n"
            "  one) contain exactly %d characters\n"
            "Type 'Q' to end this game\n"
            "With decode you may restore dumped files\n"
            "\n", PROGNAME, PROGNAME, RDBUFSIZ);
    exit(USAGE_EXIT);
}

void outc(unsigned char c)
{
/*
 * Special characters first:
 */
    switch (c)
    {
        case ESCAPE:
            fputs("\\E", stdout);
            goto end_outc;
        case '$':
            fputs("$$", stdout);
            goto end_outc;
        case '^':
            fputs("^^", stdout);
            goto end_outc;
        case '\\':
            fputs("\\\\", stdout);
            goto end_outc;
    }
    if ( c <= MAX_CTRL_CHRS )
    {
        fprintf(stdout, "^%c", CTRL_CHRS[c]);
    }
    else if ( MAX_CTRL_CHRS < c && c <= SPACE )
    {
        fprintf(stdout, "$%02X", c);
    }
    else if ( isalnum(c) || ispunct(c) )
    {
        fputc(c, stdout);
    }
    else
/*
 * The remaining cases:
 */
    {
        fprintf(stdout, "$%02X", (unsigned int)c);
    }

 end_outc:
    return;
}

void outs(char *s, int nread)
{
    while ( nread-- )
    {
        outc(*s++);
    }
    nl();
    fflush(stdout);

    return;
}

int readt(int fd, char *buf, int size)
{
#ifdef  __VMS
    return TerminalReadVMS(buf, size, 0);
#else
    return read(fd, buf, size);
#endif
}

/*
 * read without short reads, that is,
 * res < size only for EOF or error
 */
int readf(int fd, char *buf, int size)
{
    int nread = 0;
    int rdtot = 0;

    assert(NULL != buf);
    assert(size >= 0);
    if ( size <= 0 )
    {
        return 0;
    }

    rdtot = nread = read(fd, buf, size);
    while ( nread < size )
    {
        if ( 0 > nread )
        {
            return (-1);
        }
        else if ( 0 == nread )
        {
            return rdtot;
        }
        /* size > nread > 0 */
        size  -= nread;
        buf   += nread;
        nread = read(fd, buf, size);
        rdtot += nread;
    }

    return rdtot;
}


int main(int argc, char *argv[])
{
#ifdef  __VMS
#else
    struct termios  t_new, t_old;
#endif
    char  buf[RDBUFSIZ];
    int lno     = 0;
    int on_tty  = 0;
    int (*readfunc)(int, char *, int) = NULL;
    extern int decode(FILE *, FILE *);

#ifdef  __VMS
#else
    memset(&t_new, 0, sizeof(t_new));
    memset(&t_old, 0, sizeof(t_old));
#endif
    memset(buf, 0, sizeof(buf));

    if ( 2 == argc && !strcasecmp("--decode", argv[1]) )
    {
        return decode(stdin, stdout);
    }
    else if ( 1 != argc )
    {
        usage();
    }
    on_tty  = isatty(0);

    if ( on_tty )
    {
#ifdef  __VMS
#else
        tcgetattr(0, &t_old);
        t_new = t_old;
# if ( 0 )  /* Not all OS implement this conveniance funtion  */
        cfmakeraw(&t_new);
# else
        t_new.c_iflag &= ~(INLCR|ICRNL|IGNCR);
        t_new.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN);
        t_new.c_cc[VMIN] = 1;
        t_new.c_cc[VTIME] = 0;
# endif
        tcsetattr(0, TCSANOW, &t_new);
#endif
        readfunc  = readt;
    }
    else
    {
        readfunc  = readf;
    }

    for (;;)
    {
        int nread = readfunc(0, buf, sizeof(buf));

        if ( 0 >= nread )
        {
            goto main_end_for_00;
        }
        fprintf(stdout, "%09d: ", ++lno);
        outs(buf, nread);
        if ( on_tty )
        {
            switch (buf[0])
            {
                case 'Q':
                case 'q':
                    goto main_end_for_00;
            }
        }
    }
 main_end_for_00:

    if ( on_tty )
    {
#ifdef  __VMS
#else
        tcsetattr(0, TCSANOW, &t_old);
#endif
    }

    return 0;
}


/*===============================================================*/
/* Decode                                                        */
/*===============================================================*/

#define MAXLINE (1024)
#define decode_err() do                                           \
  {                                                               \
    fprintf(stderr,                                               \
            PROGNAME ": Could not interpret line number %09d\n",  \
            i);                                                   \
    return(ERROR_EXIT);                                           \
  } while (0)

int decode(FILE *infp, FILE *outfp)
{
    char  line[MAXLINE];
    char  inbf[MAXLINE];
    int   lno = 0;
    int   i   = 1;

    for (;; i++)
    {
        int inbflen = 0;
        int j       = 0;
        enum { num, dlr, ctl, esc, normal } state = normal;
        int   number  = 0;

        memset(line, 0, sizeof(line));
        memset(inbf, 0, sizeof(line));
        if ( !fgets(line, sizeof(line), infp) )
        {
            break;
        }
        if ( 2 != sscanf(line, " %d: %s\n", &lno, inbf) )
        {
            decode_err();
        }
        inbflen = strlen(inbf);
        for (j = 0; j < inbflen; j++)
        {
            int next    = inbf[j];

            if ( normal == state )
            {
                switch (next)
                {
                    case '$':
                        state = dlr;
                        continue;
                    case '^':
                        state = ctl;
                        continue;
                    case '\\':
                        state = esc;
                        continue;
                    default:
                        fputc(next, outfp);
                }
            }
            else if ( esc == state )
            {
                switch (next)
                {
                    case 'E':
                        fputc(ESCAPE, outfp);
                        state = normal;
                        continue;
                    case '\\':
                        fputc('\\', outfp);
                        state = normal;
                        continue;
                    default:
                        decode_err();
                }
            }
            else if ( ctl == state )
            {
                switch (next)
                {
                    case 'A': case 'B': case 'C': case 'D': case 'E':
                    case 'F': case 'G': case 'H': case 'I': case 'J':
                    case 'K': case 'L': case 'M': case 'N': case 'O':
                    case 'P': case 'Q': case 'R': case 'S': case 'T':
                    case 'U': case 'V': case 'W': case 'X': case 'Y':
                    case 'Z':
                        fputc(next - 'A' + 1, outfp);
                        state = normal;
                        continue;
                    case 'a': case 'b': case 'c': case 'd': case 'e':
                    case 'f': case 'g': case 'h': case 'i': case 'j':
                    case 'k': case 'l': case 'm': case 'n': case 'o':
                    case 'p': case 'q': case 'r': case 's': case 't':
                    case 'u': case 'v': case 'w': case 'x': case 'y':
                    case 'z':
                        fputc(next - 'a' + 1, outfp);
                        state = normal;
                        continue;
                    case '@':
                        fputc(0, outfp);
                        state = normal;
                        continue;
                    case '^':
                        fputc('^', outfp);
                        state = normal;
                        continue;
                    default:
                        decode_err();
                }
            }
            else if ( dlr == state )
            {
                switch (next)
                {
                    case 'A': case 'B': case 'C': case 'D': case 'E':
                    case 'F':
                        number  = (next - 'A' + 0x0A) << 4;
                        state = num;
                        continue;
                    case 'a': case 'b': case 'c': case 'd': case 'e':
                    case 'f':
                        number  = (next - 'a' + 0x0A) << 4;
                        state = num;
                        continue;
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        number  = (next - '0') << 4;
                        state = num;
                        continue;
                    case '$':
                        fputc('$', outfp);
                        state = normal;
                        continue;
                    default:
                        decode_err();
                }
            }
            else if ( num == state )
            {
                switch (next)
                {
                    case 'A': case 'B': case 'C': case 'D': case 'E':
                    case 'F':
                        number  += (next - 'A' + 0x0A);
                        fputc(number, outfp);
                        state = normal;
                        continue;
                    case 'a': case 'b': case 'c': case 'd': case 'e':
                    case 'f':
                        number  += (next - 'a' + 0x0A);
                        fputc(number, outfp);
                        state = normal;
                        continue;
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        number  += (next - '0');
                        fputc(number, outfp);
                        state = normal;
                        continue;
                    default:
                        decode_err();
                }
            }
        }
    }
    return 0;
}

/*
 * EOF
 */
