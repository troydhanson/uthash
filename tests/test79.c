#include <stdlib.h> 
#include <stdio.h>
#include "uthash.h"

typedef struct hs_t {
    int id;
    int tag;
    UT_hash_handle hh;
} hs_t;


static void pr(hs_t **hdpp){
  hs_t *el, *tmp, *hdp = *hdpp;
  HASH_ITER(hh, hdp, el, tmp){
    printf("id %d, tag %d\n",el->id,el->tag);    
  }
}

int main(int argc, char *argv[]) {

  hs_t *hs_head=NULL, *tmp, *replaced=NULL;
    
  tmp = (hs_t*)malloc(sizeof(hs_t));
  tmp->id = 10;
  tmp->tag = 100;
  HASH_REPLACE_INT(hs_head,id,tmp,replaced);
  if(replaced == NULL)
    printf("added %d %d\n",tmp->id,tmp->tag);
  else
    printf("ERROR, ended up replacing a value, replaced: %p\n",replaced);

  pr(&hs_head);

  tmp = (hs_t*)malloc(sizeof(hs_t));
  tmp->id=11;
  tmp->tag = 101;
  HASH_REPLACE_INT(hs_head,id,tmp,replaced);
  if(replaced == NULL)
    printf("added %d %d\n",tmp->id,tmp->tag);
  else
    printf("ERROR, ended up replacing a value, replaced: %p\n",replaced);

  pr(&hs_head);

  tmp = (hs_t*)malloc(sizeof(hs_t));
  tmp->id=11;
  tmp->tag = 102;
  HASH_REPLACE_INT(hs_head,id,tmp,replaced);
  if(replaced == NULL)
    printf("ERROR, exected to replace a value with key: %d\n",tmp->id);
  else
    printf("replaced %d that had tag %d with tag %d\n",tmp->id,replaced->tag,tmp->tag);

  pr(&hs_head);

  return 0;
}
