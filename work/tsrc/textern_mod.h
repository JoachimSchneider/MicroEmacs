#ifdef __cplusplus
# define EXTERN extern "C"
#else
# define EXTERN extern
#endif

#define DUMMYSZ 1

EXTERN const int  arr_s_;
#ifdef IN_MOD
EXTERN char       arr[];
#else
EXTERN char       arr[DUMMYSZ];
#endif

