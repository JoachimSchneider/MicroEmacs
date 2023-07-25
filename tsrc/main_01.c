#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#define xsnprintf snprintf
#define CONST     const
#define VOID      void
typedef void      *VOIDP;
#define VOIDCAST  (void)

#define ZEROMEM(x)    ( memset(&(x), 0, sizeof((x))) )
#define FREE(p) do  {             \
    char **pp_  = (char **)&(p);  \
                                  \
    if ( NULL != *pp_ ) {         \
        free(*pp_);               \
        *pp_  = NULL;             \
    }                             \
} while ( 0 )
/* Do something only *once*:  */
#define BEGIN_DO_ONCE do {        \
    static int  FirstCall_  = !0; \
                                  \
    if ( FirstCall_ ) {           \
        FirstCall_  = 0;          \
        {
/**END_OF_DEFINITION**/
#define   END_DO_ONCE }           \
    }                             \
} while ( 0 )
/* Example (Yes, additional braces are not necessary):  */
#if ( 0 )
BEGIN_DO_ONCE {
  action0;
  action1;
} END_DO_ONCE;
#endif


char *uitostr_memacs(unsigned int i)
{
    unsigned int  base  = 10;

    /*********************************************************************
     * a hat n (> 0) Dezimalstellen <==> 10^(n-1) <= a < 10^n
     *                              <==> n -1 <= log_10(a) < n
     * Also n <= 1 + log_10(a).
     *
     * a hat m (> 0) Stellen im 256-er System
     *                              <==> 256^(m-1) <= a < 256^n
     *                              <==> m -1 <= log_256(a) < m
     * Ausserdem m <= sizeof(a), also log_256(a) < sizeof(a)
     *
     * Umrechnung der Logarithmen:
     *  y = log_b x <==> b^y = x
     *  ==> y*log_a(b) = log_a(x)
     *  ==> log_a(x) = log_a(b)*log_b(x)
     *
     * log_10(a) = log_10(256)*log_256(a) < log_10(256)*sizeof(a)
     *
     * Damit n <= 1 + 3*sizeof(a)
     *
     ********************************************************************/
    static char buf[1 + 3*sizeof(i) + 1];
    int           pos   = sizeof(buf) - 2;
    unsigned int  rest  = 0;

    ZEROMEM(buf);

    for (;;)
    {
        rest  = i % base;
        i     = i / base;
        buf[pos--]  = '0' + rest;
        if ( 0 == i )
        {
            break;
        }
    }

    return buf + pos + 1;
}
#define eputs(s)      VOIDCAST( fputs((s), stderr) )
#define eputi(i)      eputs(uitostr_memacs((unsigned int)(i)))

VOID ASRT_Catch (CONST char * file, int line, CONST char * cond)
{
    int errno_sv_ = errno;

    eputs("File: "); eputs(file); eputs(", Line: ");
    eputi(line); eputs("\n");
    eputs("\tAssertion `"); eputs(cond); eputs("' failed!\n");
    eputs("OS: `"); eputs(strerror(errno_sv_)); eputs("'\n");
    eputs("--- abort ...\n");
    abort();
}

#define ASRT(e) do {                                                    \
        if ( !(e) )                                                     \
        {                                                               \
            ASRT_Catch (__FILE__, __LINE__, #e);                        \
        }                                                               \
    } while (0)



/*--------------------------------------------------------------------*/
/* A Stack ADT to be used for returning pointers to static variables  */
/* (e.g. char arrays) from functions: It allows (limited) indirect    */
/* recursion of such functions.                                       */
/*--------------------------------------------------------------------*/
/* Return a new stack with stacksize elements each of size len:       */
extern VOIDP  NewStack(int stacksize, int len);   /* Won't fail       */
extern char   *NextStackElem(CONST VOIDP stack);  /* Overflow: NULL   */
extern char   *DecStackPtr(CONST VOIDP stack);    /* Return previous
                                                   * stack element or
                                                   * NULL on error.   */
extern VOID   DelStack(CONST VOIDP stack);        /* Not needed.      */
#if ( 1 )
/* Example usage: */
char *HelloFunc(int n)
{
  char          *RVAL = NULL;
#define RETURN(e) do { RVAL = (e); goto RETURN_L; } while ( 0 )

  CONST char    MSG[] = "Hello World";
  static VOIDP  s     = NULL;
  char          *msg  = NULL;
  CONST int     len   = 3 * ((sizeof(MSG) - 1) + 4) + 1;

  BEGIN_DO_ONCE {
    s = NewStack(3, len);
  } END_DO_ONCE;
  ASRT(NULL != (msg = NextStackElem(s)));
  if ( 1 >= n) {
    xsnprintf(msg, len, "%s_%d", MSG, 1);
  } else                {
    xsnprintf(msg, len, "%s, %s_%d", HelloFunc(n - 1), MSG, n);
  }

  RETURN(msg);

RETURN_L:
#undef RETURN
  ASRT(NULL != DecStackPtr(s));

  return RVAL;
}
#endif


typedef struct  STACK_S_  {
  int   stacksize;
  int   len;
  int   sp;
  char  *arr;
} STACK_T_;

VOIDP  NewStack(int stacksize, int len)
{
  STACK_T_  *stack = NULL;

  ASRT(0 < stacksize);
  ASRT(0 < len);
  ASRT(NULL != (stack = calloc(1, sizeof(*stack))));
  stack->stacksize = stacksize;
  stack->len       = len;
  stack->sp        = (-1);
  ASRT(NULL != (stack->arr = calloc(stacksize, len)));

  return (VOIDP)stack;
}

char  *NextStackElem(CONST VOIDP stack)
{
    ASRT(NULL != stack);

    STACK_T_  *stk  = (STACK_T_ *)stack;

    if ( (-1) > stk->sp ) {
      return NULL;
    }
    stk->sp++;
    if ( stk->stacksize <= stk->sp ) {
      return NULL;
    }

    return &stk->arr[stk->sp * stk->len];
}

char  *DecStackPtr(CONST VOIDP stack)
{
    ASRT(NULL != stack);

    STACK_T_  *stk  = (STACK_T_ *)stack;

    stk->sp--;
    if ( (-1) > stk->sp ) {
      return NULL;
    }

    return &stk->arr[(stk->sp + 1) * stk->len];
}

VOID  DelStack(CONST VOIDP stack)
{
  ASRT(NULL != stack);

  STACK_T_  *stk  = (STACK_T_ *)stack;

  FREE(stk->arr);
  FREE(stk);
}


int main(int argc, char *argv[])
{
  int num = argc > 1 ? atoi(argv[1]) : 0;

  fprintf(stdout, "%s\n", HelloFunc(num));
  fprintf(stdout, "%s\n", HelloFunc(num));
  fprintf(stdout, "%s\n", HelloFunc(num));

  return 0;
}


/* EOF */

