#ifndef __FMALLOC_H
#define __FMALLOC_H

#include <stddef.h>

typedef struct size_list
{
    size_t size;
    void *ptr;
}

void *
fmalloc(size_t size);

void
ffree(void *ptr);

#endif
