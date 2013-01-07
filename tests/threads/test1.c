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

typedef struct {
  int i;
  UT_hash_handle hh;
} elt;

elt *elts=NULL; /* this is our hash table which two threads will use */
pthread_rwlock_t lock;

void *thread_routine_r( void *arg ) {
    int i;
    long num_found=0;
    elt *e;

    for(i=0;i<LOOPS;i++) {
      if (pthread_rwlock_rdlock(&lock) != 0) {
        fprintf(stderr,"can't acquire readlock\n");
        exit(-1);
      }
      HASH_FIND_INT(elts, &i, e);
      pthread_rwlock_unlock(&lock);
      if (e) num_found++;
    }
    return (void*)num_found;
}
void *thread_routine_w( void *arg ) {
    int i;
    long num_deld=0;
    elt *e;

    for(i=0;i<LOOPS;i++) {
      if (pthread_rwlock_wrlock(&lock) != 0) {
        fprintf(stderr,"can't acquire write lock\n");
        exit(-1);
      }
      HASH_FIND_INT(elts, &i, e);
      if (e) {
        /* HASH_DEL(elts, e); */
        /* num_deld++; */
      } else {
        e = malloc(sizeof(elt));
        if (!e) exit(-1);
        e->i = i;
        HASH_ADD_INT(elts, i, e);
      }
      pthread_rwlock_unlock(&lock);
    }
    return (void*)num_deld;
}

int main() {
    unsigned i;
    long num_added=0;
    int status;
    pthread_t thread_r1,thread_r2,thread_w1,thread_w2;
    void *thread_result;
    elt tmp, *e;

    if (pthread_rwlock_init(&lock,NULL) != 0) {
      fprintf(stderr,"lock init failed\n");
      exit(-1);
    }

    if ( status = pthread_create( &thread_r1, NULL, thread_routine_r, NULL )) {
        printf("failure: status %d\n", status);
        exit(-1);
    }
    if ( status = pthread_create( &thread_r2, NULL, thread_routine_r, NULL )) {
        printf("failure: status %d\n", status);
        exit(-1);
    }
    if ( status = pthread_create( &thread_w1, NULL, thread_routine_w, NULL )) {
        printf("failure: status %d\n", status);
        exit(-1);
    }
    if ( status = pthread_create( &thread_w2, NULL, thread_routine_w, NULL )) {
        printf("failure: status %d\n", status);
        exit(-1);
    }

    status = pthread_join( thread_r1, &thread_result );
    printf("thread result: %d %ld\n", status, (long)thread_result);

    status = pthread_join( thread_r2, &thread_result );
    printf("thread result: %d %ld\n", status, (long)thread_result);

    status = pthread_join( thread_w1,  &thread_result );
    printf("thread result: %d %ld\n", status, (long)thread_result);

    status = pthread_join( thread_w2,  &thread_result );
    printf("thread result: %d %ld\n", status, (long)thread_result);

    i = HASH_COUNT(elts);
    printf("final count of items in hash: %u\n", i);
}
