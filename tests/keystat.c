#include <sys/types.h>  /* for 'open' */
#include <sys/stat.h>   /* for 'open' */
#include <fcntl.h>      /* for 'open' */
#include <stdlib.h>     /* for 'malloc' */
#include <stdio.h>      /* for 'printf' */
#include <unistd.h>     /* for 'read' */
#include <errno.h>      /* for 'sterror' */
#include <sys/time.h>   /* for 'gettimeofday' */
#include "uthash.h"

#undef uthash_noexpand_fyi
#define uthash_noexpand_fyi(t) die()
#define UNALIGNED_KEYS 0

void die() {
  fprintf(stderr,"expansion inhibited\n");
  exit(-1);
}

/* Windows doesn't have gettimeofday. While Cygwin and some 
 * versions of MinGW supply one, it is very coarse. This substitute
 * gives much more accurate elapsed times under Windows. */
#if (( defined __CYGWIN__ ) || ( defined __MINGW32__ ))
#include <windows.h>
void win_gettimeofday(struct timeval* p, void* tz /* IGNORED */) {
  LARGE_INTEGER q;
  static long long freq;
  static long long cyg_timer;
  QueryPerformanceFrequency(&q);
  freq = q.QuadPart;
  QueryPerformanceCounter(&q);
  cyg_timer = q.QuadPart;
  p->tv_sec = (long)(cyg_timer / freq);
  p->tv_usec = (long)(((cyg_timer % freq) * 1000000) / freq);
}
#define gettimeofday win_gettimeofday
#define MODE (O_RDONLY|O_BINARY)
#else
#define MODE (O_RDONLY)
#endif

#ifndef timersub
#define timersub(a, b, result)                                                \
  do {                                                                        \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                             \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;                          \
    if ((result)->tv_usec < 0) {                                              \
      --(result)->tv_sec;                                                     \
      (result)->tv_usec += 1000000;                                           \
    }                                                                         \
  } while (0)
#endif

typedef struct stat_key {
    char *key;
    unsigned len;
    UT_hash_handle hh, hh2;
} stat_key;

#define CHAIN_0  0
#define CHAIN_5  1
#define CHAIN_10  2
#define CHAIN_20  3
#define CHAIN_100 4
#define CHAIN_MAX 5
void hash_chain_len_histogram(UT_hash_table *tbl) {
  unsigned i, bkt_hist[CHAIN_MAX+1];
  double pct = 100.0/tbl->num_buckets;
  memset(bkt_hist,0,sizeof(bkt_hist));
  for(i=0; i < tbl->num_buckets; i++) {
      unsigned count = tbl->buckets[i].count;
      if (count == 0) bkt_hist[CHAIN_0]++;
      else if (count < 5) bkt_hist[CHAIN_5]++;
      else if (count < 10) bkt_hist[CHAIN_10]++;
      else if (count < 20) bkt_hist[CHAIN_20]++;
      else if (count < 100) bkt_hist[CHAIN_100]++;
      else bkt_hist[CHAIN_MAX]++;
  }
  fprintf(stderr, "Buckets with     0 items: %.1f%%\n", bkt_hist[CHAIN_0 ]*pct);
  fprintf(stderr, "Buckets with <   5 items: %.1f%%\n", bkt_hist[CHAIN_5 ]*pct);
  fprintf(stderr, "Buckets with <  10 items: %.1f%%\n", bkt_hist[CHAIN_10]*pct);
  fprintf(stderr, "Buckets with <  20 items: %.1f%%\n", bkt_hist[CHAIN_20]*pct);
  fprintf(stderr, "Buckets with < 100 items: %.1f%%\n", bkt_hist[CHAIN_100]*pct);
  fprintf(stderr, "Buckets with > 100 items: %.1f%%\n", bkt_hist[CHAIN_MAX]*pct);
}

int main(int argc, char *argv[]) {
    int dups=0, rc, fd, done=0, err=0, want, i=0, padding=0, v=1, percent=100;
    unsigned keylen, max_keylen=0, verbose=0;
    const char *filename = "/dev/stdin";
    char *dst; 
    stat_key *keyt, *keytmp, *keys=NULL, *keys2=NULL;
    struct timeval start_tm, end_tm, elapsed_tm, elapsed_tm2, elapsed_tm3;

    if ((argc >= 3) && !strcmp(argv[1],"-p")) {percent = atoi(argv[2]); v = 3;}
    if ((argc >= v) && !strcmp(argv[v],"-v")) {verbose=1; v++;}
    if (argc >= v) filename=argv[v];
    fd=open(filename,MODE);

    if ( fd == -1 ) {
        fprintf(stderr,"open failed %s: %s\n", filename, strerror(errno));
        return -1;
    }

    for(i=0; !done; i++) {

          want = sizeof(int);
          dst = (char*)&keylen;
          readmore1:
          rc = read(fd,dst,want);
          if (rc != want) {
              if (rc == 0) done=1;
              else if (rc == -1) {
                fprintf(stderr,"read failed: %s\n", strerror(errno));
                err=1;
              }
              else if (rc > 0) { want -= rc; dst += rc; goto readmore1; }
          }

          if (done || err) break;
          if (keylen > max_keylen) max_keylen=keylen;
  
          if ( (keyt = (stat_key*)malloc(sizeof(stat_key))) == NULL) {
              fprintf(stderr,"out of memory\n");
              exit(-1);
          }
  
          /* read key */
#ifdef UNALIGNED_KEYS
          padding = i%8;
#endif
          if ( (keyt->key = (char*)malloc(padding+keylen)) == NULL) {
              fprintf(stderr,"out of memory\n");
              exit(-1);
          }
          keyt->key += padding; /* forcibly alter the alignment of key */
          keyt->len = keylen;
  
          want = keylen;
          dst = keyt->key;
          readmore2:
          rc = read(fd,dst,want);
          if (rc != want) {
              if (rc == -1) {
                fprintf(stderr,"read failed: %s\n", strerror(errno));
                err=1;
              } else if (rc == 0) {
                fprintf(stderr,"incomplete file\n");
                err=1;
              } else if (rc >= 0) { want -= rc; dst += rc; goto readmore2; }
          }
          if (err) break;
          /* if percent was set to something less than 100%, skip some keys*/
          if (((rand()*1.0) / RAND_MAX) > ((percent*1.0)/100)) { 
            free(keyt->key-padding);
            free(keyt);
            continue;
          }
  
          /* eliminate dups */
          HASH_FIND(hh,keys,keyt->key,keylen,keytmp);
          if (keytmp) {
              dups++;
              free(keyt->key - padding);
            free(keyt);
          } else {
            HASH_ADD_KEYPTR(hh,keys,keyt->key,keylen,keyt);
          }
    }

    if (verbose) {
      unsigned key_count = HASH_COUNT(keys);
      fprintf(stderr,"max key length: %u\n", max_keylen);
      fprintf(stderr,"number unique keys: %u\n", key_count);
      fprintf(stderr,"keystats memory: %u\n", 
        (unsigned)((sizeof(stat_key)+max_keylen)*key_count));
      hash_chain_len_histogram(keys->hh.tbl);
    }

    /* add all keys to a new hash, so we can measure add time w/o malloc */
    gettimeofday(&start_tm,NULL);
    for(keyt = keys; keyt != NULL; keyt=(stat_key*)keyt->hh.next) {
        HASH_ADD_KEYPTR(hh2,keys2,keyt->key,keyt->len,keyt);
    }
    gettimeofday(&end_tm,NULL);
    timersub(&end_tm, &start_tm, &elapsed_tm);

    /* now look up all keys in the new hash, again measuring elapsed time */
    gettimeofday(&start_tm,NULL);
    for(keyt = keys; keyt != NULL; keyt=(stat_key*)keyt->hh.next) {
        HASH_FIND(hh2,keys2,keyt->key,keyt->len,keytmp);
        if (!keytmp) fprintf(stderr,"internal error, key not found\n");
    }
    gettimeofday(&end_tm,NULL);
    timersub(&end_tm, &start_tm, &elapsed_tm2);

    /* now delete all items in the new hash, measuring elapsed time */
    gettimeofday(&start_tm,NULL);
    while (keys2) {
        keytmp = keys2;
        HASH_DELETE(hh2,keys2,keytmp);
    }
    gettimeofday(&end_tm,NULL);
    timersub(&end_tm, &start_tm, &elapsed_tm3);

    if (!err) {
        printf("%.3f,%d,%d,%d,%s,%ld,%ld,%ld\n",
        1-(1.0*keys->hh.tbl->nonideal_items/keys->hh.tbl->num_items), 
        keys->hh.tbl->num_items, 
        keys->hh.tbl->num_buckets, 
        dups,
        (keys->hh.tbl->noexpand ? "nx" : "ok"),
        (elapsed_tm.tv_sec * 1000000) + elapsed_tm.tv_usec,
        (elapsed_tm2.tv_sec * 1000000) + elapsed_tm2.tv_usec,
        (elapsed_tm3.tv_sec * 1000000) + elapsed_tm3.tv_usec );
    }
  return 0;
}

