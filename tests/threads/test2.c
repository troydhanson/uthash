#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "uthash.h"

#undef uthash_noexpand_fyi
#define uthash_noexpand_fyi fprintf(stderr,"warning: bucket expansion inhibited\n");

#define LOOPS 100000
#define NTHREADS 2

typedef struct {
  int i;
  int v;
  UT_hash_handle hh;
} elt;

elt *elts=NULL; /* this is our hash table which two threads will use */
pthread_rwlock_t lock;

void *thread_routine( void *arg ) {
  int keepgoing=0;
  /* TODO write me */
  return NULL;
}

int main() {
    unsigned i;
    long num_added=0;
    int status;
    pthread_t thread[NTHREADS];
    void *thread_result;
    elt tmp, *e;

    if (pthread_rwlock_init(&lock,NULL) != 0) {
      fprintf(stderr,"lock init failed\n");
      exit(-1);
    }

    /* populate it to start */
    for(i=0; i<LOOPS; i++) {
      e = malloc(sizeof(elt));
      if (!e) exit(-1);
      e->i = i;
      e->v = 0;
      HASH_ADD_INT(elts, i, e);
    }

    for(i=0; i<NTHREADS; i++) {
      if ( status = pthread_create( &thread[i], NULL, thread_routine, NULL )) {
          printf("failure: status %d\n", status);
          exit(-1);
      }
    }

    for(i=0; i<NTHREADS; i++) {
      status = pthread_join( thread[i], &thread_result );
      printf("thread result: %d %ld\n", status, (long)thread_result);
    }

    i = HASH_COUNT(elts);
    printf("final count of items in hash: %u\n", i);
}
