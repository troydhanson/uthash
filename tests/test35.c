#include "uthash.h"
#include <string.h>   /* strcpy */
#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */

typedef struct elt {
    char *s;
    UT_hash_handle hh;
} elt;

int main(int argc,char *argv[]) {
    int i;
    elt *head = NULL;
    elt elts[10];
    char label[6];
    for(i=0;i<10;i++) { 
      elts[i].s = (char*)malloc(6); 
      strncpy(elts[i].s, "hello",6); 
      elts[i].s[0] = 'a' + i;
      printf("%d: %s\n", i, elts[i].s);
      HASH_ADD_KEYPTR(hh, head, elts[i].s, 6, &elts[i]);
    }

    /* look up each element and verify the result pointer */
    strncpy(label, "hello", 6);
    for(i=0;i<10;i++) { 
      elt *e;
      label[0] = 'a' + i;
      HASH_FIND(hh,head,label,6,e);
      if (e) {
        printf( "found %s\n", e->s);
        printf( "right address? %s\n", (e == &elts[i]) ? "yes" : "no");
      }
    }

    return 0;
}
