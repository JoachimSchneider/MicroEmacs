#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


#define TRC_FILE_ENVVAR "EMACS_TRC_FILE"
static  int          DebugMessage_lnno_   = 0;
static  const char  *DebugMessage_fname_  = "";
int DebugMessage(const char *fmt, ...)
{
   va_list  ap;
   int      rc;
   static FILE  *fp = NULL;

   if ( NULL == fp )
   {
       char *fname  = getenv(TRC_FILE_ENVVAR);

       if ( NULL != fname )
       {
           fp = fopen(fname, "a");
       }

       if ( NULL == fp )
       {
           fp = stderr;
       }
   }
   
   fprintf(fp, "%s (%s/%03d): ", "TRC", DebugMessage_fname_, DebugMessage_lnno_);
   va_start(ap, fmt);
   rc = vfprintf(fp, fmt, ap);
   va_end(ap);
   fprintf(fp, "%s", "\n");
   fflush(fp);
   
   return rc;
}
#undef  TRC_FILE_ENVVAR


#if ( defined(EMACS_TRC) )
# define  TRC(arg)  do {                        \
        DebugMessage_fname_ = __FILE__;         \
        DebugMessage_lnno_ = __LINE__;          \
        DebugMessage arg;                       \
    } while ( 0 )
#else
# define  TRC(arg)  do {} while ( 0 )
#endif


int main(int argc, char *argv[])
{
   TRC(("Hello %s ---", "You"));
   TRC(("Hello %d/%d/%d ---", 1, 2, 3));

   return 0;
}
