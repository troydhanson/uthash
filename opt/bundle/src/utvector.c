/*
Copyright (c) 2003-2014, Troy D. Hanson     http://troydhanson.github.com/uthash/
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "utvector.h"

/* utvector
 * 
 * maintain a contiguous buffer of 'n' elements ('i' occupied)
 * the 'n' buffers are deep-inited at the time of allocation
 * the vector leaves popped slots as-is, clearing them on re-use
 * the memory management helper mm is used to define the size and
 * deep-init, deep-fini, deep-copy (into inited slots) and deep-clear.
 * deep-clear prepares a slot for re-use e.g. reset slot state.
 *
 */

void oom(void) {
  //fprintf(stderr,"out of memory\n");
  exit(-1);
}

UT_vector *utvector_new(UT_vector_mm *mm) {
  UT_vector *v = malloc(sizeof(UT_vector)); if (!v) return NULL;
  utvector_init(v,mm);
  return v;
}

unsigned utvector_len(UT_vector *v) {
  return v->i;
}

void utvector_init(UT_vector *v, UT_vector_mm *mm) {
  v->mm = *mm;  // struct copy
  v->i = v->n = 0;
  v->d = NULL;
  utvector_reserve(v, INITIAL_SIZE); // also inits them, sets v->n
}

void utvector_reserve(UT_vector *v, unsigned num) {
  if (v->n - v->i >= num) return;     // space is big enough, return
  unsigned n = num - (v->n - v->i);   // minimum we need to grow by
  if (n < (v->n * 2)) n = (v->n * 2); // grow by at least double current size
  char *d = realloc(v->d, (n + v->n) * v->mm.sz);
  if (!d) oom();
  v->d = d;
  if (v->mm.init) v->mm.init(&v->d[v->n], n); 
  else            memset(&v->d[v->n], 0, n*v->mm.sz);
  v->n = n + v->n;
}

void utvector_fini(UT_vector *v) {
  if (v->mm.fini) v->mm.fini(v->d, v->n);
  free(v->d);
  v->d = NULL;
  v->i = v->n = 0;
}

UT_vector * utvector_clone(UT_vector *src) {
  UT_vector *v = utvector_new(&src->mm);
  utvector_copy(v, src);
  return v;
}

void utvector_clear(UT_vector *v) {
  v->i = 0;
}

void utvector_copy(UT_vector *dst, UT_vector *src) { /* dst, src both inited */
  assert(dst->mm.sz == src->mm.sz); // double check that its inited 
  utvector_clear(dst);
  utvector_reserve(dst, src->i);
  dst->i = src->i;
  if (dst->mm.clear) dst->mm.clear(dst->d, src->i);
  if (src->mm.copy) src->mm.copy(dst->d, src->d, src->i);
  else                    memcpy(dst->d, src->d, src->mm.sz * src->i);
}

void utvector_free(UT_vector *v) {
  utvector_fini(v);
  free(v);
}

void *utvector_extend(UT_vector *v) {
  utvector_reserve(v,1);
  void *b = v->d + (v->i * v->mm.sz);
  if (v->mm.clear) v->mm.clear(b,1);
  v->i++;
  return b;
}

void *utvector_next(UT_vector *v, void *cur) {
  if (cur == NULL) return v->i ? v->d : NULL;
  assert(cur >= (void*)(v->d));  // user pointer must be inside our data area
  char *n = (char*)cur + v->mm.sz; // next slot address
  if (n >= v->d + (v->i * v->mm.sz)) n=NULL; // only if next slot occupied
  return n;
}

void utvector_pop(UT_vector *v) {
  assert(v->i);
  v->i--;
}

void utvector_push(UT_vector *v, void *e) {
  void *b  = utvector_extend(v);
  if (v->mm.copy) v->mm.copy(b, e, 1);
  else            memcpy(b, e, v->mm.sz);
}


/* a few basic vector types as described via mm that can be passed to utvector_init/new */
static UT_vector_mm utvector_int_mm = {.sz = sizeof(int)};
UT_vector_mm* utvector_int = &utvector_int_mm;
