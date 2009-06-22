#include <stdio.h>

#include "fmalloc.h"
#include "firstfit.h"

#define SIZE_LIST_DIM 1024

/* Anzahl allozierter Bloecke */
static unsigned int num_allocated = 0;

/* Liste der allozierten Block-Groessen */
static slist size_list[SIZE_LIST_DIM];

void *
fmalloc(size_t size)
{
    int i;
    void * tmp = NULL;

    if ((tmp = ff_alloc(size)) != NULL)
    {
        /* nach freigewordenen Plaetzen suchen */
        for (i=0; i<num_allocated; ++i)
        {
            if (size_list[i].ptr == NULL && size_list[i].size == 0)
            {
                size_list[i].size = size;
                size_list[i].ptr = tmp;
                return tmp;
            }
        }

        if (num_allocated >= SIZE_LIST_DIM)
        {
            fprintf(stderr, "size list exceeded\n");
            return NULL;
        }

        size_list[num_allocated].size = size;
        size_list[num_allocated].ptr = tmp;
        ++num_allocated;
    }

    return tmp;
}

void
ffree(void *ptr)
{
    int i;
    size_t size = 0;

    for (i=0; i<num_allocated; ++i)
    {
        if (ptr == size_list[i].ptr)
        {
            size = size_list[i].size;
            break;
        }
    }

    if (size != 0)
    {
        ff_free(ptr, size);
        size_list[i].ptr = NULL;
        size_list[i].size = 0;
    }
    else
        fprintf(stderr, "memory list corrupted\n");
}
