/*
 * simhtap: Write the files given ans aruments into a SIMH tape to stdout.
 */


/*======================================================================
Citation out of

`SIMH Magtape Representation and Handling', Bob Supnik, 03-Mar-03 (simh_magtape.pdf)

```

Magtape Representation

SIMH represents magnetic tapes as disk files. Each disk file contains a
series of objects. Objects are either metadata markers, like tape mark
or end of medium, or they are data records. Location 0 of the file is
interpreted as beginning of tape; end of file is interpreted as end of
medium. Pictorially:



Location 0:     +--------+
                | data |
                | record |
                +--------+
                | data |
                | record |
                +--------+
                :
                +--------+
                | tape |
                | mark |
                +--------+
                | data |
                | record |
                +--------+
                :
end of file:


Metadata markers are 4 bytes stored in little -endian order. The
currently defined metadata markers are:

0xFFFFFFFF                end of medium
0xFF000000:0xFFFFFFFE     reserved
0x00000000                tape mark


Data records are consist of an initial 4 byte record length n,
(n + 1) & ~1 bytes of data, and a trailing 4 byte record length n that
must be the same as the initial record length:

bytes 0:3       +--------+
                | record |
                | length |
                +--------+
bytes 4:n+3     | data |
                |
                :
                |
                |
                :
                |
                +--------+
bytes n+4:n+7   | record |
                | length |
                +--------+

Note that the data is rounded to an even number of bytes. If the record
length is odd, the extra byte is undefined but should be 0.

Record lengths are 4 bytes stored in little-endian order. The high
order bit is flag, indicating that the record contains an error; the
next 7b must be zero; the low 24 bits are the record length:

bit<31>         1 = record contains error
                0 = record is error-free
bits<30:24>     must be zero
bits<23:0>      record length, must be non-zero

The leading and trailing record lengths allow a record to be accessed
either forward or backward.

```
======================================================================*/

static const char usage_str[] =
"/*\n"
" * simhtap: create a SIMH-Tape (see \"SIMH Magtape Representation\n"
" *          and Handling\" by Bob Supnik) out of the list of files\n"
" *          given on the command line:\n"
" *\n"
" * simhtap <file_1> <blf_1> .. <file_N> <blf_N>\n"
" *\n"
" *          This will create a tape which is written to stdout:\n"
" *\n"
" * |file1|tape mark|file2|tape mark| .. |fileN|end of tape|end of medium|\n"
" *\n"
" * The file_X will be written in records of 512 * blf_X.\n"
" * Set the environment variable 'SIMHTAP_RAW' to something to switch off the\n"
" * blocking for the last record --- than it won't be filled up with zeroes.\n"
" */\n";
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


#define NBITS (8)
#define BSIZE (512)

#define USAGE_EXIT  (42)
#define ERROR_EXIT  (43)

typedef unsigned long uulong;
typedef unsigned int  uuint;
typedef unsigned char byte;
typedef enum { FALSE = 0, TRUE = !0 } bool;


/*
 * Set this environment variable to something, to switch fillup to FALSE
 */
#define SIMHTAP_RAW "SIMHTAP_RAW"
bool fillup = TRUE;


int error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    abort();
    exit(ERROR_EXIT);
    return 0;
}

int usage(void)
{
    fprintf(stderr, "\n%s\n", usage_str);

    exit(USAGE_EXIT);
    return 0;
}

int write_BS(FILE *fp, int bs)
{
    uulong  sz;
    byte    b0, b1, b2, b3;

    assert(NULL != fp);
    assert(0 <= bs);

    sz  = (uulong)bs;
    b0 = sz % (1 << NBITS);
    sz >>= NBITS;
    b1 = sz % (1 << NBITS);
    sz >>= NBITS;
    b2 = sz % (1 << NBITS);
    sz >>= NBITS;
    b3 = sz % (1 << NBITS);

    fputc(b0, fp);
    fputc(b1, fp);
    fputc(b2, fp);
    fputc(b3, fp);

    return bs;
}

int write_TM(FILE *fp)
{
    assert(NULL != fp);

    fputc(0x00, fp);
    fputc(0x00, fp);
    fputc(0x00, fp);
    fputc(0x00, fp);

    return 0;
}

int write_EOT(FILE *fp)
{
    assert(NULL != fp);

    write_TM(fp);
    write_TM(fp);

    return 0;
}

int write_EOM(FILE *fp)
{
    assert(NULL != fp);

    fputc(0xFF, fp);
    fputc(0xFF, fp);
    fputc(0xFF, fp);
    fputc(0xFF, fp);

    return 0;
}

int write_record(FILE *fp, int bs, char buf[])
{
    int i;

    assert(NULL != fp);
    assert(0 < bs);       /* need bs > 0 here!  */
    assert(NULL != buf);

    write_BS(fp, bs);
    for (i = 0; i < bs; i++)
    {
        fputc(buf[i], fp);
    }
    if ( bs % 2 )
    {
        fputc((char)0, fp);
    }
    write_BS(fp, bs);

    return bs;
}

int write_file(FILE *fp, int bs, FILE *fin)
{
    int  c;
    char *buf;
    int  i;
    int  nwritten;

    assert(NULL != fp);
    assert(0 < bs);
    assert(NULL != fin);
    buf = (char *)malloc(bs);
    assert(NULL != buf);

    nwritten = 0;
    memset(buf, 0, bs);
    i        = 0;
    while (EOF != (c = fgetc(fin)))
    {
        buf[i++] = c; nwritten++;
        if ( !(i % bs) )
        {
            write_record(fp, bs, buf);
            memset(buf, 0, bs);
            i = 0;
            continue;
        }
    }
    if ( i )
    {
        if ( fillup )
        {
            /*
             * Write last record where missing bytes are filled up with
             * zeros:
             */
            write_record(fp, bs, buf);
        }
        else
        {
            /*
             * Write last record shorter than the others:
             */
            write_record(fp, i, buf);
        }
    }
    free(buf);
    write_TM(fp);

    return nwritten;
}

int main(int argc, char *argv[])
{
    if ( getenv(SIMHTAP_RAW) )
    {
        fillup = FALSE;
    }
    else
    {
        fillup = TRUE;
    }
    for(;;)
    {
        char *name  = NULL;
        char *blf_s = NULL;
        int  blf;
        FILE *fin;
        int  nwritten;
        uulong sz;
        byte   b0, b1, b2, b3;

        name  = *++argv;
        if ( NULL == name )
        {
            break;
        }
        blf_s = *++argv;
        if ( NULL == blf_s )
        {
            usage();
        }
        blf = atoi(blf_s);

        if ( blf < 0 )
        {
            error("Could not get blocking factor %s of %s\n", blf_s,
                  name);
        }
        if ( NULL == (fin = fopen(name, "rb")) )
        {
            error("Could not open %s for reading\n", name);
        }
        nwritten = write_file(stdout, blf * BSIZE, fin);
        fclose(fin);

        fprintf(stderr, "%24s: %12d: ", name, (int)nwritten);
        sz = (uulong)nwritten;
        b0 = sz % (1 << NBITS);
        sz >>= NBITS;
        b1 = sz % (1 << NBITS);
        sz >>= NBITS;
        b2 = sz % (1 << NBITS);
        sz >>= NBITS;
        b3 = sz % (1 << NBITS);
        sz >>= NBITS;
        fprintf(stderr, "%02X%02X%02X%02X\n",
                (uuint)b3, (uuint)b2, (uuint)b1, (uuint)b0);

    }
    write_EOT(stdout);
    /* THINK: How to create a tape not completely filled? */
    write_EOM(stdout);

    return 0;
}



/*
 * EOF
 */
