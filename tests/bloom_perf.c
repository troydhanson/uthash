#include <stdlib.h>   /* malloc */
#include <sys/time.h> /* gettimeofday */
#include <errno.h>    /* perror */
#include <stdio.h>    /* printf */
#include "uthash.h"

#define BUFLEN 20
#if 0
#undef uthash_expand_fyi
#define uthash_expand_fyi(tbl) printf("expanding to %d buckets\n", tbl->num_buckets)
#endif

typedef struct name_rec {
    char boy_name[BUFLEN];
    UT_hash_handle hh;
} name_rec;

int main(int argc,char *argv[])
{
    name_rec *name, *names=NULL;
    char linebuf[BUFLEN];
    FILE *file;
    int i=0,j,nloops=3,loopnum=0,miss;
    struct timeval tv1,tv2;
    long elapsed_usec;
    if (argc > 1) {
        nloops = atoi(argv[1]);
    }

    if ( (file = fopen( "test14.dat", "r" )) == NULL ) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        i++;
        if ( (name = (name_rec*)malloc(sizeof(name_rec))) == NULL) {
            exit(-1);
        }
        strcpy(name->boy_name, linebuf);
        HASH_ADD_STR(names,boy_name,name);
    }

again:
    if (fseek(file,0,SEEK_SET) == -1) {
        fprintf(stderr,"fseek failed: %s\n", strerror(errno));
    }
    j=0;

    if (gettimeofday(&tv1,NULL) == -1) {
        perror("gettimeofday: ");
    }
    while (fgets(linebuf,BUFLEN,file) != NULL) {
        /* if we do 10 loops, the first has a 0% miss rate,
         * the second has a 10% miss rate, etc */
        miss = ((rand()*1.0/RAND_MAX) < (loopnum*1.0/nloops)) ? 1 : 0;
        /* generate a miss if we want one */
        if (miss) {
            linebuf[0]++;
            if (linebuf[1] != '\0') {
                linebuf[1]++;
            }
        }
        HASH_FIND_STR(names,linebuf,name);
        if (name) {
            j++;
        }
    }
    if (gettimeofday(&tv2,NULL) == -1) {
        perror("gettimeofday: ");
    }
    elapsed_usec = ((tv2.tv_sec - tv1.tv_sec) * 1000000) + (tv2.tv_usec - tv1.tv_usec);
    printf("lookup on %d of %d (%.2f%%) names succeeded (%.2f usec)\n", j, i,
           j*100.0/i, (double)(elapsed_usec));
    if (++loopnum < nloops) {
        goto again;
    }
    fclose(file);

    return 0;
}

