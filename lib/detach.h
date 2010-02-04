#ifndef DETACH_H
#define DETACH_H

#include <unistd.h>
#include "object.h"
#include "reader.h"

typedef struct detach
{
  int in, out;
  pid_t proc;
  reader_t *read;
} detach_t;

/* Creation and destruction */
object_t *c_detach (object_t * o);
detach_t *detach_create ();
void detach_destroy (object_t * o);

/* Basic type functions */
uint8_t detach_hash (object_t * o);
void detach_print (object_t * o);

/* Info on parent process. */
extern object_t *parent_detach;

/* lisp-space functions */
object_t *lisp_detach (object_t * lst);
object_t *lisp_receive (object_t * lst);
object_t *lisp_send (object_t * lst);

#define OPROC(o) (((detach_t *) OVAL (o))->proc);
#define OREAD(o) (((detach_t *) OVAL (o))->read);
#define DETACHP(o) (o->type == DETACH)

#endif /* DETACH_H */
