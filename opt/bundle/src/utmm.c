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

/* utmm
 *
 * low level memory primitives
 *
 */

#include "utmm.h"

void *utmm_new(const UT_mm *mm, size_t n) {
  void *o = calloc(n, mm->sz);
  if (o) return o;
  fprintf(stderr,"oom"); exit(-1); 
}

void utmm_init(const UT_mm *mm, void *_buf, size_t n) {
  char *buf = (char*)_buf;
  if (mm->init == NULL) {
    memset(buf,0,n*mm->sz);
    return;
  }
  while(n--) {
    mm->init(buf);
    buf += mm->sz;
  }
}

void utmm_fini(const UT_mm *mm, void *_buf, size_t n) {
  char *buf = (char*)_buf;
  if (mm->fini == NULL) return;
  while(n--) {
    mm->fini(buf);
    buf += mm->sz;
  }
}

void utmm_clear(const UT_mm *mm, void *_buf, size_t n) {
  char *buf = (char*)_buf;
  if (mm->clear == NULL) {
    memset(buf,0,n*mm->sz);
    return;
  }
  while(n--) {
    mm->clear(buf);
    buf += mm->sz;
  }
}

void utmm_copy(const UT_mm *mm, void *_dst, void *_src, size_t n) {
  char *dst = (char*)_dst;
  char *src = (char*)_src;
  if (mm->copy == NULL) {
    memcpy(dst,src,n*mm->sz);
    return;
  }
  while(n--) {
    mm->copy(dst,src);
    dst += mm->sz;
    src += mm->sz;
  }
}

/* convenient predefined utmm */
static UT_mm _utmm_int = {.sz = sizeof(int)};
UT_mm* utmm_int = &_utmm_int;
