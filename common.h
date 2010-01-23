#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

/* Wrapper for malloc() and realloc(). Will exit() the program on error. */
void *xmalloc (size_t size);
void *xrealloc (void *p, size_t size);
char *xstrdup (char *str);

void error (char *str);

#endif /* COMMON_H */
