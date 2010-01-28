#ifndef STR_H
#define STR_H

#include "object.h"

typedef struct str
{
  char *raw;
  char *print;
  size_t len;
} str_t;

/* Must be called before any other functions. */
void str_init ();

/* Memory management. */
str_t *str_create ();
void str_destroy ();
object_t *c_str (char *str, size_t len);
object_t *c_strs (char *str);

void str_genp (object_t *o);

/* String operators */
object_t *str_cat (object_t *ao, object_t *bo);

#define OSTR(o) (((str_t *) (o)->val)->raw)
#define OSTRLEN(o) (((str_t *) (o)->val)->len)
#define OSTRP(o) (str_genp (o), ((str_t *) (o)->val)->print)

#endif /* STR_H */
