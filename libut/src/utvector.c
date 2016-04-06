/*
Copyright (c) 2003-2015, Troy D. Hanson     http://troydhanson.github.com/uthash/
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

#define INITIAL_SIZE 16

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

UT_vector *utvector_new(const UT_mm *mm) {
  UT_vector *v = malloc(sizeof(UT_vector)); if (!v) return NULL;
  utvector_init(v,mm);
  return v;
}

unsigned utvector_len(UT_vector *v) {
  return v->i;
}

void utvector_init(UT_vector *v, const UT_mm *mm) {
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
  void *b = v->d + (v->n * v->mm.sz); // start of newly allocated area
  utmm_init(&v->mm,b,n);
  v->n = n + v->n;
}

void utvector_fini(UT_vector *v) {
  utmm_fini(&v->mm,v->d,v->n);
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
  utmm_clear(&dst->mm,dst->d,src->i);
  utmm_copy(&dst->mm, dst->d, src->d, src->i);
}

void utvector_free(UT_vector *v) {
  utvector_fini(v);
  free(v);
}

void *utvector_extend(UT_vector *v) {
  utvector_reserve(v,1);
  void *b = v->d + (v->i * v->mm.sz);
  utmm_clear(&v->mm,b,1);
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

void *utvector_head(UT_vector *v) {
  if (v->i == 0) return NULL;
  return v->d;
}

void *utvector_tail(UT_vector *v) {
  if (v->i == 0) return NULL;
  return v->d + ((v->i - 1) * v->mm.sz);
}

void *utvector_pop(UT_vector *v) {
  if (v->i == 0) return NULL;
  return v->d + (--(v->i) * v->mm.sz);
}

void *utvector_elt(UT_vector *v, unsigned i) {
  if (i >= v->i) return NULL;
  return v->d + (i * v->mm.sz);
}

/* shifting is not very efficient. we end up throwing away/fini'ing the
 * head of the vector, then doing a memmove, then having to init a new slot.
 * we don't return the shifted item because its been fini'd, and we have
 * no caller memory to copy it into anyway. a cpy_shift maybe handy */
void utvector_shift(UT_vector *v) {
  assert (v->i);
  utmm_fini(&v->mm,v->d,1);
  v->i--;
  memmove(v->d, v->d + v->mm.sz, (v->n-1)*v->mm.sz);
  char *b = v->d + ((v->n-1) * v->mm.sz);
  utmm_init(&v->mm,b,1);
}

void utvector_erase(UT_vector *v, unsigned i) {
  assert(v->i);
  if (i >= v->i) return;
  utmm_fini(&v->mm, v->d + (i * v->mm.sz), 1);
  v->i--;
  if (v->n - 1 > i) {
    memmove(v->d + (i * v->mm.sz), v->d + ((i+1) * v->mm.sz), (((v->n-1) - i) * v->mm.sz));
  }
  char *b = v->d + ((v->n-1) * v->mm.sz);
  utmm_init(&v->mm,b,1);
}

void *utvector_push(UT_vector *v, void *e) {
  void *b  = utvector_extend(v);
  utmm_copy(&v->mm, b, e, 1);
  return v->d + ((v->i - 1) * v->mm.sz);
}


