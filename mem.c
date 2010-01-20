#include "common.h"
#include "mem.h"

void mm_fill_stack (mmanager_t * mm)
{
  void *p =
    xmalloc ((mm->size - (mm->stack - mm->base)) * mm->osize);
  for (; mm->stack < mm->base + mm->size;
       mm->stack++, p += mm->osize)
    {
      mm->clearf (p);
      *(mm->stack) = p;
    }
  mm->stack--;
}

void mm_resize_stack (mmanager_t * mm)
{
  mm->size *= 2;
  size_t count = mm->stack - mm->base;
  mm->base = xrealloc (mm->base, mm->size);
  mm->stack = mm->base + count * sizeof (void *);
}

mmanager_t *mm_create (size_t osize, void (*clear_func) (void *o))
{
  mmanager_t *mm = xmalloc (sizeof (mmanager_t));
  mm->osize = osize;
  mm->clearf = clear_func;
  mm->size = 4;
  mm->stack = mm->base = xmalloc (sizeof (void *) * mm->size);
  mm_fill_stack (mm);
  return mm;
}

void mm_destroy (mmanager_t * mm)
{
  free (mm->stack);
  free (mm);
}

void *mm_alloc (mmanager_t * mm)
{
  if (mm->stack == mm->base)
    mm_fill_stack (mm);
  void *p = *(mm->stack);
  mm->stack--;// -= sizeof (void *);
  return p;
}

void mm_free (mmanager_t * mm, void *o)
{
  if (mm->stack == mm->base + mm->size)
    mm_resize_stack (mm);
  mm->stack += sizeof (void *);
  mm->stack = o;
}
