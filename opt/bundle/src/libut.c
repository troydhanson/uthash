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

#include "libut.h"

static void utstring_init_wrapper(void *_buf, unsigned num) {
  UT_string *buf = (UT_string*)_buf;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_init(buf);
    buf++;
  }
}

static void utstring_done_wrapper(void *_buf, unsigned num) {
  UT_string *buf = (UT_string*)_buf;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_done(buf);
    buf++;
  }
}

static void utstring_clear_wrapper(void *_buf, unsigned num) {
  UT_string *buf = (UT_string*)_buf;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_clear(buf);
    buf++;
  }
}

static void utstring_copy_wrapper(void *_dst, void *_src, unsigned num) {
  UT_string *dst = (UT_string*)_dst;
  UT_string *src = (UT_string*)_src;
  unsigned i;
  for(i=0; i < num; i++) {
    utstring_concat(dst,src);
    dst++;
    src++;
  }
}

/* a few basic vector types as described via mm that can be passed to utvector_init/new */
static UT_vector_mm utvector_utstring_mm = {
  .sz = sizeof(UT_string),
  .init = utstring_init_wrapper,
  .fini = utstring_done_wrapper,
  .copy = utstring_copy_wrapper,
  .clear = utstring_clear_wrapper
};
UT_vector_mm* utvector_utstring = &utvector_utstring_mm;

