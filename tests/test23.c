#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

typedef struct {
  int key;
  int data;
  UT_hash_handle hh;
} item;

int main() {
  item *i, *j, *items=NULL;
  int k;

  /* first item */
  k = 12345;
  i = (item*)malloc(sizeof(item));
  i->key = k; i->data = 0;
  HASH_ADD_INT(items,key,i);

  /* second item */
  k = 6789;
  i = (item*)malloc(sizeof(item));
  i->key = k; i->data = 0;
  HASH_ADD_INT(items,key,i);

  /* third item */
  k = 98765;
  i = (item*)malloc(sizeof(item));
  i->key = k; i->data = 0;
  HASH_ADD_INT(items,key,i);

  /* look them all up */
  k = 12345; HASH_FIND_INT(items, &k, j); if (j) printf("found %d\n",k);
  k = 6789;  HASH_FIND_INT(items, &k, j); if (j) printf("found %d\n",k);
  k = 98765; HASH_FIND_INT(items, &k, j); if (j) printf("found %d\n",k);

  /* delete them not the way we prefer but it works */
  for(j=items; j != NULL; j=(item*)j->hh.next) {
    printf("deleting %d\n", j->key);
    HASH_DEL(items,j);
  }
  return 0;
}
