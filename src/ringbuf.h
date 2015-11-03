#ifndef _RINGBUF_H_
#define _RINGBUF_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* simple ring buffer */

typedef struct _ringbuf {
    size_t n; /* allocd size */
    size_t u; /* used space */
    size_t i; /* input pos */
    size_t o; /* output pos */
    char d[]; /* C99 flexible array member */
} ringbuf;

ringbuf *ringbuf_new(size_t sz);
int ringbuf_put(ringbuf *, const void *data, size_t len);
size_t ringbuf_get_pending_size(ringbuf *);
size_t ringbuf_get_next_chunk(ringbuf *, char **data);
void ringbuf_mark_consumed(ringbuf *, size_t);
void ringbuf_free(ringbuf*);
void ringbuf_clear(ringbuf*);

#endif /* _RINGBUF_H_ */
