#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct al_s {
    int i;
    int j;
    char s[64];
};

/* With DJGPP and `-O2' this leads to the warning:
 *  `dereferencing type-punned pointer will break strict aliasing rules'
 * *and* to generation of wrong code!
 */
#define CLROOM_0(p)   do {          \
    char **q_   = (char **)&(p);    \
                                    \
    if ( NULL != *q_ )  {           \
        free(*q_);                  \
        *q_ = NULL;                 \
    }                               \
} while ( 0 )

#define CLROOM_1(p)   do {              \
    char    *z_ = (char *)&(p);         \
    char    *p_ = NULL;                 \
                                        \
    memcpy(&p_, z_, sizeof(p_));        \
    if ( NULL != p_ )   {               \
        free(p_);                       \
        p_  = NULL;                     \
        memcpy(z_, &(p_), sizeof(p));   \
    }                                   \
} while ( 0 )

#define CLROOM_2(p)   do {          \
    void **q_   = (void **)&(p);    \
                                    \
    if ( NULL != *q_ )  {           \
        free(*q_);                  \
        *q_ = NULL;                 \
    }                               \
} while ( 0 )

int main(int argc, char *argv[])
{
    struct al_s *ap = NULL;
    char        *cp = NULL;

    ap = malloc(sizeof(struct al_s));
    cp = malloc(sizeof(struct al_s));

    (*(struct al_s *)cp).i = 0;
    (*(struct al_s *)cp).j = 1;

    CLROOM_0(ap);

    ap = malloc(sizeof(struct al_s));
    CLROOM_2(ap);

    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);

    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);
    ap = malloc(sizeof(struct al_s));
    CLROOM_1(ap);

    return 0;
}
