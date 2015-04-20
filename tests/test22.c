#include <stdlib.h>    /* malloc       */
#include <stddef.h>    /* offsetof     */
#include <stdio.h>     /* printf       */
#include <string.h>    /* memset       */
#include "uthash.h"

#define UTF32 '\x1'

typedef struct {
    UT_hash_handle hh;
    size_t len;
    char encoding;      /* these two fields */
    int text[];         /* comprise the key */
} msg_t;

typedef struct {
    char encoding;
    int text[];
} lookup_key_t;

int main(int argc, char *argv[])
{
    unsigned keylen;
    msg_t *msg, *tmp, *msgs = NULL;
    lookup_key_t *lookup_key;

    int beijing[] = {0x5317, 0x4eac};   /* UTF-32LE for 北京 */

    /* allocate and initialize our structure */
    msg = (msg_t*)malloc( sizeof(msg_t) + sizeof(beijing) );
    if (msg == NULL) {
        exit(-1);
    }
    memset(msg, 0, sizeof(msg_t)+sizeof(beijing)); /* zero fill */
    msg->len = sizeof(beijing);
    msg->encoding = UTF32;
    memcpy(msg->text, beijing, sizeof(beijing));

    /* calculate the key length including padding, using formula */
    keylen =   offsetof(msg_t, text)       /* offset of last key field */
               + sizeof(beijing)             /* size of last key field */
               - offsetof(msg_t, encoding);  /* offset of first key field */

    /* add our structure to the hash table */
    HASH_ADD( hh, msgs, encoding, keylen, msg);

    /* look it up to prove that it worked :-) */
    msg=NULL;

    lookup_key = (lookup_key_t*)malloc(sizeof(*lookup_key) + sizeof(beijing));
    if (lookup_key == NULL) {
        exit(-1);
    }
    memset(lookup_key, 0, sizeof(*lookup_key) + sizeof(beijing));
    lookup_key->encoding = UTF32;
    memcpy(lookup_key->text, beijing, sizeof(beijing));
    HASH_FIND( hh, msgs, &lookup_key->encoding, keylen, msg );
    if (msg != NULL) {
        printf("found \n");
    }
    free(lookup_key);

    HASH_ITER(hh, msgs, msg, tmp) {
        HASH_DEL(msgs, msg);
        free(msg);
    }
    return 0;
}
