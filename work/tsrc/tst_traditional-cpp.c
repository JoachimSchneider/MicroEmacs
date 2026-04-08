/* SOF */

/* This may be transl;ated using GCC's `cpp -traditional <file>'
 * command line.
 */

#if 0
# define CONCAT_(x,y)  CONCAT__(x,y)
# define CONCAT__(x,y) x/**/y
#else
# define ID_(x)         x
# define CONCAT_(x,y)   ID_(x)ID_(y)
#endif

#define a A
#define b B

CONCAT_(a,b)
/* EOF */
