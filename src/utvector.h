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

#ifndef __UTVECTOR_H_
#define __UTVECTOR_H_

#include "utmm.h"

/* typical usage e.g. a vector of utstring would have
 *
   .sz = sizeof(UT_string)
   .init = utstring-init
   .fini = utstring-done
   .copy = ustring_concat
   .clear= utstring-clear
 */

typedef struct _UT_vector {
  UT_mm mm;
  unsigned i,n;/* i: index of next available slot, n: num slots */
  char *d;     /* n slots of size icd->sz*/
} UT_vector;


UT_vector *utvector_new(const UT_mm *mm);
void utvector_init(UT_vector *v, const UT_mm *mm);
void utvector_reserve(UT_vector *v, unsigned num);
void utvector_fini(UT_vector *v);
UT_vector * utvector_clone(UT_vector *src);
void utvector_clear(UT_vector *v);
void utvector_copy(UT_vector *dst, UT_vector *src);
void utvector_free(UT_vector *v);
void *utvector_extend(UT_vector *v);
void *utvector_head(UT_vector *v);
void *utvector_tail(UT_vector *v);
void *utvector_next(UT_vector *v, void *cur);
void *utvector_pop(UT_vector *v);
void *utvector_elt(UT_vector *v, unsigned i);
void utvector_shift(UT_vector *v);
void utvector_erase(UT_vector *v, unsigned i);
void *utvector_push(UT_vector *v, void *e);
unsigned utvector_len(UT_vector *v);


#endif /* __UTVECTOR_H_ */
