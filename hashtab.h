#ifndef HASHTAB_H
#define HASHTAB_H

#include <stdlib.h>
#include <string.h>

typedef struct hashtab_node_t
{
  void *key;			/* key for the node */
  size_t keylen;		/* length of the key */
  void *value;			/* value for this node */
  size_t vallen;		/* length of the value */

  struct hashtab_node_t *next;	/* next node (open hashtable) */
} hashtab_node_t;

typedef struct hashtab_t
{
  hashtab_node_t **arr;
  size_t size;			/* size of the hash */
  int count;			/* number if items in this table */
  int (*hash_func) (void *, size_t, size_t);	/* hash function */
} hashtab_t;

/* Iterator type for iterating through the hashtable. */
typedef struct hashtab_iter_t
{
  /* key and value of current item */
  void *key;
  void *value;
  size_t keylen;
  size_t vallen;

  /* bookkeeping data */
  struct hashtab_internal_t
  {
    hashtab_t *hashtable;
    hashtab_node_t *node;
    int index;
  } internal;

} hashtab_iter_t;

/* Initialize a new hashtable (set bookingkeeping data) and return a
 * pointer to the hashtable. A hash function may be provided. If no
 * function pointer is given (a NULL pointer), then the built in hash
 * function is used. A NULL pointer returned if the creation of the
 * hashtable failed due to a failed malloc(). */
hashtab_t *ht_init (size_t size,
		    int (*hash_func)
		    (void *key, size_t keylen, size_t ht_size));

/* Fetch a value from table matching the key. Returns a pointer to
 * the value matching the given key. */
void *ht_search (hashtab_t * hashtable, void *key, size_t keylen);

/* Put a value into the table with the given key. Returns NULL if
 * malloc() fails to allocate memory for the new node. */
void *ht_insert (hashtab_t * hashtable,
		 void *key, size_t keylen, void *value, size_t vallen);

/* Delete the given key and value pair from the hashtable. If the key
 * does not exist, no error is given. */
void ht_remove (hashtab_t * hashtable, void *key, size_t keylen);

/* Change the size of the hashtable. It will allocate a new hashtable
 * and move all keys and values over. The pointer to the new hashtable
 * is returned. Will return NULL if the new hashtable fails to be
 * allocated. If this happens, the old hashtable will not be altered
 * in any way. The old hashtable is destroyed upon a successful
 * grow. */
void *ht_grow (hashtab_t * hashtable, size_t new_size);

/* Free all resources used by the hashtable. */
void ht_destroy (hashtab_t * hashtable);

/* Initialize the given iterator. It will point to the first element
 * in the hashtable. */
void ht_iter_init (hashtab_t * hashtable, hashtab_iter_t * ii);

/* Increment the iterator to the next element. The iterator key and
 * value will point to NULL values when the iterator has reached the
 * end of the hashtable.  */
void ht_iter_inc (hashtab_iter_t * ii);

/* Default hashtable hash function. */
int ht_hash (void *key, size_t key_size, size_t hashtab_size);

#endif
