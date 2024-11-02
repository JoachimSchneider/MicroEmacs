/*
 * Read a SIMH-Tape into multiple files
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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>


#ifdef PATH_MAX
# define FNAMELEN (PATH_MAX)
#else
# define FNAMELEN (512)
#endif
/** Test of error handling
#undef FNAMELEN
#define FNAMELEN (5)
**/


#define STMT(s) do { s } while (0)
#define ASRT(s) STMT(                               \
    if ( !(s) )                                     \
    {                                               \
      message(                                      \
        "Condition `%s' violated at line %4.4d!\n", \
        #s, __LINE__);                              \
      abort();                                      \
    }                                               \
    )
#define EXPT(s, e) STMT(                            \
    if ( !(s) )                                     \
    {                                               \
      message(                                      \
        "Condition `%s' violated at line %4.4d!\n", \
        #s, __LINE__);                              \
      longjmp(exception, (e));                      \
    }                                               \
    )
#define RAISE(e) STMT(                              \
    message(                                        \
      "Exception `%s' raised at line %4.4d!\n",     \
      #e, __LINE__);                                \
    longjmp(exception, (e));                        \
    )
#ifdef DEBUG
# define DBGMSG(s) ( message s )
#else
# define DBGMSG(s) /* EMPTY */
#endif


typedef enum {
  NBITS         =  8,
  SUCCESS_EXIT  = 00,
  USAGE_EXIT    = 42,
  ERROR_EXIT    = 43,
  CANT_OPEN     = 62,
  ZERO_LENGTH   = 63,
  BAD_TAPE      = 64,

  dummy_constant_
} constants;

typedef enum { FALSE = 0, TRUE = !FALSE } bool;
typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long ulng;


static char     *myname;
static jmp_buf  exception;


bool usage(void)
{
  fprintf(stderr, "Usage: %s -o <start of output file name>\n",
          myname);
  exit(ERROR_EXIT);

  return TRUE;
}

int message(char *fmt, ...)
{
  va_list arg_ptr;

  va_start(arg_ptr, fmt);
  vfprintf(stderr, fmt, arg_ptr);
  va_end(arg_ptr);

  return SUCCESS_EXIT;
}

ulng readlen(FILE *inp, byte *class/*OUT*/)
{
  ulng l0     = 0;
  ulng l1     = 0;
  ulng l2     = 0;
  ulng l3     = 0;
  ulng len    = 0;
  byte class_ = 0;

  ASRT( EOF != (l0 = fgetc(inp)) );
  ASRT( EOF != (l1 = fgetc(inp)) );
  ASRT( EOF != (l2 = fgetc(inp)) );
  ASRT( EOF != (l3 = fgetc(inp)) );

  class_  = l0;
  len     =   (l2 << (2 * NBITS))
            + (l1 << (1 * NBITS))
            + (l0 << (0 * NBITS));

  DBGMSG(("class = 0x%2.2X\n",  (uint)class_));
  DBGMSG(("  len = 0x%8.8lX\n", len));

  *class  = class_;
  
  return len;
}

bool readrecord(FILE *inp, FILE *outp, ulng len)
{
  int c = 0;
  int i = 0;

  for ( i = 0; i < len; i++ )
  {
    EXPT( EOF != (c = fgetc(inp)), BAD_TAPE );
    ASRT( EOF != fputc(c, outp) );
  }
  if ( len % 2 )
  {
    EXPT( EOF != (c = fgetc(inp)), BAD_TAPE );
  }

  return TRUE;
}

bool writefile(FILE *inp, const char *outfname)
{
  ulng  len   = 0;
  FILE  *outp = NULL;
  byte  class = 0;

  len = readlen(inp, &class);
  if      ( 0xFF == class && 0xFFFFFF == len)
  {
    DBGMSG(("%s\n", "Got EOM"));
    
    return FALSE;
  }
  else if ( 0x00 == class && 0x000000 == len )
  {
    DBGMSG(("%s\n", "Got Software EOM")); /* Two consecutive tape marks */
    
    return FALSE;
  }
  EXPT( 0 == class, BAD_TAPE );
  EXPT( NULL != (outp = fopen(outfname, "w")), CANT_OPEN );
  for(;;)
  {
    readrecord(inp, outp, len);
    /* Read trailing length record: */
    EXPT( len == readlen(inp, &class), BAD_TAPE );
    len = readlen(inp, &class);
    if ( 0 == class && 0 == len )
    {
      break;
    }
  }
  fclose(outp);

  return TRUE;
}

int main(int argc, char *argv[])
{
  char  *outfname = NULL;
  int   j = 0;
  FILE  *fpin = stdin;
  int   exret;

  myname = argv[0];

  /*******************************************************************
   * START EXCEPTION HANDLING
   ******************************************************************/
  switch ( exret = setjmp(exception) )
  {
    case ZERO_LENGTH:
      message("%s\n", "Got file with zero length! (Maybee OK: Could mean EOT)");
      return SUCCESS_EXIT;
      break;

    case BAD_TAPE:
      message("%s\n", "Tape error!");
      return ERROR_EXIT;
      break;

    case CANT_OPEN:
      message("Could not open file\n");
      return ERROR_EXIT;
      break;

    default:
      ASRT( 0 == exret );
      break;
  }
  /*******************************************************************
   * END EXCEPTION HANDLING
   ******************************************************************/

  while ( *++argv )
  {
    if ( !strcmp("-o", *argv) )
    {
      outfname = *++argv;
      (void)((NULL != outfname) || usage());
    }
  }
  (void)((NULL != outfname) || usage());
  message("Output file name pattern: %s####\n", outfname);

  for (j = 0;; j++)
  {
    char fname[FNAMELEN];

    ASRT( strlen(outfname) + 4 + 1 <= sizeof(fname) );
    sprintf(fname, "%s%4.4d", outfname, j);
    message("Working on: %s\n", fname);
    if ( !writefile(fpin, fname) )
    {
      message("Reached End Of Medium\n");
      break;
    }
  }

  return SUCCESS_EXIT;
}



/*
 * EOF
 */
