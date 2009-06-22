#ifndef __FIRSTFIT_H
#define __FIRSTFIT_H

#include <stddef.h>

void *
ff_alloc(size_t size);

void
ff_free(void *ptr, size_t size);

#endif
