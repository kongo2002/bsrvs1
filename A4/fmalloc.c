#include <stdio.h>

#include "firstfit.h"

/* Anzahl allozierter Bloecke */
static unsigned int num_allocated = 0;

/* Liste der allozierten Block-Groessen */
static size_list[MEM_POOL_SIZE];

void *
fmalloc(size_t size)
{
    void * tmp = NULL;

    if ((tmp = ff_alloc(size)) != NULL)
    {
        ++num_allocated;
        size_list[num_allocated].size = size;
        size_list[num_allocated].ptr = tmp;
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
        ff_free(ptr, size);
    else
        fprintf(stderr, "memory list corrupted\n");
}
