#ifndef OBJECT_H
#define OBJECT_H

#include "mem.h"

typedef enum types {INT, FLOAT, STRING, SYMBOL, CONS, EMPTY} type_t;

typedef struct object {
  type_t type;
  void* val;
} object_t;

/* Must be called before any other functions. */
void object_init();

/* Memory management */
object_t *obj_create(type_t type);
void obj_destroy(object_t *o);

void obj_print(object_t *o);

#endif /* OBJECT_H */
