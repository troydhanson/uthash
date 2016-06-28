#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
Copyright (c) 2003-2016, Troy D. Hanson     http://troydhanson.github.com/uthash/
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

#ifndef __UTMM_H_
#define __UTMM_H_


typedef struct {
   size_t sz;
   void (*init)(void *buf);             //-> utstring-init
   void (*fini)(void *buf);             //-> utstring-done
   void (*copy)(void *dst, void *src);  //-> ustring_concat
   void (*clear)(void *buf);            //-> utstring-clear
} UT_mm;

void *utmm_new(const UT_mm *mm, size_t n);
void utmm_init(const UT_mm *mm, void *buf, size_t n);
void utmm_fini(const UT_mm *mm, void *buf, size_t n);
void utmm_clear(const UT_mm *mm, void *buf, size_t n);
void utmm_copy(const UT_mm *mm, void *dst, void *src, size_t n);

/* convenient predefined mm */
extern UT_mm* utmm_int;
extern UT_mm* utstring_mm;

#endif /* __UTMM_H_ */
