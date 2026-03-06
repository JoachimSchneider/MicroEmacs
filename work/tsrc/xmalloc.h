/* SOF(xmalloc.h) */
#ifndef XMALLOC_H_
#define XMALLOC_H_
/*====================================================================*/
#ifndef CONST
# define CONST  const
#endif
#ifndef VOID
# define VOID   void
#endif
#ifndef FALSE
# define FALSE  ( 0 )
#endif
#ifndef TRUE
# define TRUE   ( 1 )
#endif
#define MIN2(x, y)  ( (x) < (y) ? (x) : (y) )
#define ZEROMEM(x)  ( memset(&(x), 0, sizeof((x))) )
/*====================================================================*/
extern char *xmalloc_(int size, CONST char *file, int line);
extern VOID xfree_(char *p, CONST char *file, int line);
extern char *xrealloc_(char *q, int size, CONST char *file, int line);
#define xmalloc(size)     xmalloc_((size), __FILE__, __LINE__)
#define xfree(p)          xfree_((p), __FILE__, __LINE__)
#define xrealloc(q, size) xrealloc_((q), (size), __FILE__, __LINE__)
/*====================================================================*/
#endif
/* EOF(xmalloc.h) */
