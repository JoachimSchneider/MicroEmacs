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
