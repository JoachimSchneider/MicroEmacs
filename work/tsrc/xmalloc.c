/* SOF(xmalloc.c) */
/*====================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef NDEBUG
#include <assert.h>
#include "xmalloc.h"


#ifndef UEMACS_DEBUG_XMALLOC


char *xmalloc_(int size, CONST char *file, int line)
{
    char  *res    = NULL;

    if ( size <= 0 )  {
        return NULL;
    }
    res = (char *)malloc(size);
    assert(NULL != res);
    memset(res, 0, size);

    return  res;
}

VOID  xfree_(char *p, CONST char *file, int line)
{
    if ( NULL == p )  {
        return;
    }

    free(p);

    return;
}

char *xrealloc_(char *q, int size, CONST char *file, int line)
{
    char  *res    = NULL;

    if ( size < 0 ) {
        return NULL;
    }
    if ( size == 0 )  {   /* As in GNU extension  */
        xfree_(q, file, line);
    }
    res = (char *)realloc(q, size);
    assert(NULL != res);

    return res;
}


#else


typedef struct  ptr_info_s {
    char  *p;
    int   size;
} ptr_info_t;


/*====================================================================*/
/* By using the data structures and functions below i.e.              */
/* - p_list_node_t                                                    */
/* - {get,ins,del}_xmalloc_info()                                     */
/* it wiil ll be possible to switch the linked list implementation to */
/* a more efficient hash table implemnentation if this is needed in   */
/* the future.                                                        */
/*====================================================================*/


#if ( 0 )


typedef struct p_list_node_s  p_list_node_t;

typedef struct  p_list_node_s {
    ptr_info_t    info;
    p_list_node_t *next;
} p_list_node_t;


static p_list_node_t  *g_p_list = NULL;


static ptr_info_t *get_xmalloc_info(char *q)
{
    p_list_node_t *node = g_p_list;

    assert(NULL != q);

    while ( NULL != node )  {
        if ( q == node->info.p )  {
            return &node->info;
        } else              {
            node  = node->next;
        }
    }

    return NULL;
}

static int  del_xmalloc_info(char *q)
{
    p_list_node_t *node = g_p_list;
    p_list_node_t *prev = NULL;

    assert(NULL != q);

    /* Special handling at start of search: */
    if ( NULL == node ) {
        return FALSE;
    } else              {
        if ( q == node->info.p )  {
            g_p_list  = node->next;
            memset(node, 0, sizeof(*node));
            free(node);

            return TRUE;
        } else                    {
            prev  = node;
        }
    }

    while ( NULL != prev->next )  {
        if ( q == prev->next->info.p )  {
            node        = prev->next;
            prev->next  = node->next;

            memset(node, 0, sizeof(*node));
            free(node);

            return TRUE;
        } else              {
            prev  = prev->next;
        }
    }

    return FALSE;
}

static int  ins_xmalloc_info(ptr_info_t *p_info)
{
    p_list_node_t *node = NULL;

    assert(NULL != p_info);

    if ( NULL != get_xmalloc_info(p_info->p) )  {
        return FALSE;
    }
    node  = (p_list_node_t *)malloc(sizeof(*node));
    assert(NULL != node);
    memset(node, 0, sizeof(*node));
    memcpy(&node->info, p_info, sizeof(node->info));
    node->next  = g_p_list;
    g_p_list  = node;

    return TRUE;
}


#else


typedef struct p_tree_node_s  p_tree_node_t;

typedef struct  p_tree_node_s {
    ptr_info_t    info;
    int           info_is_valid;  /* For lazy deletion  */
    p_tree_node_t *left;
    p_tree_node_t *right;
} p_tree_node_t;

static p_tree_node_t  *g_p_tree = NULL;


/* We expect the pointers to come more or less numerically ordered from
 * the OS's malloc(). So we modify the values bijectively in a way that
 * we expect to get them unordered: We want to create a binary tree
 * that should not degenerate into a linked list.
 */
static unsigned long int  get_cmp_ulong(char *p)
{
#define B0_ZMASK_     ( (unsigned long int)(-1) ^ 0x00FF )
#define B1_ZMASK_     ( (unsigned long int)(-1) ^ 0xFF00 )
#define BITS_IN_BYTE_ (8)
    unsigned long int res       = 0;
    unsigned long int x         = (unsigned long int)p;
    unsigned long int byte0     = x & 0x00FF;
    unsigned long int byte1     = (x & 0xFF00) >> BITS_IN_BYTE_;
    int               i         = 0;

    /* Swap byte0 and byte1 because byte0 might be restricted by
     * alignement conditions */
    x = (x & B0_ZMASK_) | byte1;
    x = (x & B1_ZMASK_) | (byte0 << BITS_IN_BYTE_);

    /* Revert order of bytes in x:  */
    for ( i = 0; i < sizeof(x); i++ ) {
        unsigned long int res_byte  = x & 0xFF;

        res <<= BITS_IN_BYTE_;
        x   >>= BITS_IN_BYTE_;

        res |= res_byte;
    }

    return res;
#undef B0_ZMASK_
#undef B1_ZMASK_
#undef BITS_IN_BYTE_
}

static int  info_EQ(char *p1, char *p2)
{
    return get_cmp_ulong(p1) == get_cmp_ulong(p2);
}

static int  info_LT(char *p1, char *p2)
{
    return get_cmp_ulong(p1) < get_cmp_ulong(p2);
}

static int  info_GT(char *p1, char *p2)
{
    return get_cmp_ulong(p1) > get_cmp_ulong(p2);
}


static p_tree_node_t  *get_xmalloc_info_(char *q, p_tree_node_t *node)
{
    assert(NULL != q);

    if ( NULL == node ) {
        return NULL;
    }

    if        ( info_LT(q, node->info.p) ) {
        return get_xmalloc_info_(q, node->left);
    } else if ( info_GT(q, node->info.p) ) {
        return get_xmalloc_info_(q, node->right);
    } else                                 {
        assert(info_EQ(q, node->info.p));

        return node;
    }
}

static ptr_info_t *get_xmalloc_info(char *q)
{
    p_tree_node_t *node = NULL;

    assert(NULL != q);

    if ( NULL == (node = get_xmalloc_info_(q, g_p_tree)) )  {
        return NULL;
    }

    if ( node->info_is_valid )  {
        return  &node->info;
    } else                      {
        return NULL;
    }
}


static int  ins_xmalloc_info_(ptr_info_t *p_info, p_tree_node_t **p_node)
{
    assert(NULL != p_info);

    if        ( NULL == *p_node )                     {
        *p_node = (p_tree_node_t *)malloc(sizeof(**p_node));
        assert(NULL != *p_node);
        memset(*p_node, 0, sizeof(**p_node));
        memcpy(&(*p_node)->info, p_info, sizeof((*p_node)->info));
        (*p_node)->info_is_valid  = TRUE;
        (*p_node)->left           = NULL;
        (*p_node)->right          = NULL;

        return TRUE;
    } else if ( info_LT(p_info->p, (*p_node)->info.p) ) {
        return ins_xmalloc_info_(p_info, &(*p_node)->left);
    } else if ( info_GT(p_info->p, (*p_node)->info.p) ) {
        return ins_xmalloc_info_(p_info, &(*p_node)->right);
    } else                                            {
        assert(info_EQ(p_info->p, (*p_node)->info.p));

        if ( (*p_node)->info_is_valid ) {
            return FALSE;
        } else                        {
            memcpy(&(*p_node)->info, p_info, sizeof((*p_node)->info));
            (*p_node)->info_is_valid  = TRUE;

            return TRUE;
        }
    }
}

static int  ins_xmalloc_info(ptr_info_t *p_info)
{
    assert(NULL != p_info);

    return ins_xmalloc_info_(p_info, &g_p_tree);
}

static int  del_xmalloc_info(char *q)
{
    p_tree_node_t *node = NULL;

    assert(NULL != q);

    if ( NULL == (node = get_xmalloc_info_(q, g_p_tree)) )  {
        return FALSE;
    }

    if ( node->info_is_valid )  {
        node->info_is_valid = FALSE;

        return TRUE;
    } else                      {
        return FALSE;
    }
}


#endif


/*====================================================================*/


char *xmalloc_(int size, CONST char *file, int line)
{
    char        *res  = NULL;
    ptr_info_t  info;

    ZEROMEM(info);

    if ( size <= 0 )  {
        return NULL;
    }
    res = (char *)malloc(size);
    assert(NULL != res);
    memset(res, 0, size);
    info.p    = res;
    info.size = size;

    if ( ! ins_xmalloc_info(&info) )  {
        fflush(NULL);
        fprintf(stderr,
                "*** ERROR: xmalloc(), %s:%d:\n*** \t%s.\n*** \t--- calling abort()\n",
                file, line, "Pointer already registered");
        abort();
    }

    return  res;
}

VOID  xfree_(char *p, CONST char *file, int line)
{
    ptr_info_t  *p_info = NULL;
    ptr_info_t  info;

    ZEROMEM(info);

    if ( NULL == p )  {
        return;
    }

    if ( NULL == (p_info = get_xmalloc_info(p)) ) {
        fflush(NULL);
        fprintf(stderr,
                "*** ERROR: xfree(), %s:%d:\n*** \t%s.\n*** \t--- calling abort()\n",
                file, line, "Pointer to free not found");
        abort();
    }
    memcpy(&info, p_info, sizeof(info));
    if ( ! del_xmalloc_info(p) )  {
        fflush(NULL);
        fprintf(stderr,
                "*** ERROR: xfree(), %s:%d:\n*** \t%s.\n*** \t--- calling abort()\n",
                file, line, "Pointer info not found");
        abort();
    }

    /* Try to force SIGSEGV when accessing free'd memory: */
    memset(info.p, 0, info.size);
    free(info.p);

    return;
}

char *xrealloc_(char *q, int size, CONST char *file, int line)
{
    char        *res    = NULL;
    ptr_info_t  *p_info = NULL;

    if ( size < 0 ) {
        return NULL;
    }
    if ( size == 0 )  {   /* As in GNU extension  */
        xfree_(q, file, line);
    }
    if ( NULL == q )  {
        return  xmalloc(size);
    } else            {
        if ( NULL == (p_info = get_xmalloc_info(q)) ) {
            fflush(NULL);
            fprintf(stderr,
                    "*** ERROR: xrealloc(), %s:%d:\n*** \t%s.\n*** \t--- calling abort()\n",
                    file, line, "Pointer to re-allocate not found");
            abort();
        }
        res = xmalloc(size);
        memcpy(res, q, MIN2(size, p_info->size));
        xfree_(q, file, line);

        return res;
    }
}


#endif



/*====================================================================*/
/* EOF(xmalloc.c) */
