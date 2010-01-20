#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

/* Wrapper for malloc() and realloc(). Will exit() the program on error. */
void *xmalloc (size_t size);
void *xrealloc (void *p, size_t size);

#endif /* COMMON_H */
