#include <stdlib.h>   /* malloc */
#include <stdio.h>    /* printf */
#include <string.h>
#include "uthash.h"
#include "utlist.h"
#include "utstring.h"

typedef struct example_user_t {
    int id;
    int cookie;
    UT_hash_handle hh;
} example_user_t;

#define BUFLEN 20

typedef struct el {
    char bname[BUFLEN];
    struct el *next, *prev;
} el;

static int namecmp(void *_a, void *_b)
{
    el *a = (el*)_a;
    el *b = (el*)_b;
    return strcmp(a->bname,b->bname);
}

int main(int argc, char *argv[])
{
    el *name, *elt, *tmp, etmp;
    int i;
    example_user_t *user, *users=NULL;
    el *head = NULL; /* important- initialize to NULL! */

    char linebuf[BUFLEN];
    FILE *file;

    UT_string *s;
    char binary[] = "\xff\xff";

    file = fopen( "test11.dat", "r" );
    if (file == NULL) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf,BUFLEN,file) != NULL) {
        name = (el*)malloc(sizeof(el));
        if (name == NULL) {
            exit(-1);
        }
        strncpy(name->bname,linebuf,sizeof(name->bname));
        DL_APPEND(head, name);
    }
    DL_SORT(head, namecmp);
    DL_FOREACH(head,elt) {
        printf("%s", elt->bname);
    }

    memcpy(etmp.bname, "WES\n", 5UL);
    DL_SEARCH(head,elt,&etmp,namecmp);
    if (elt != NULL) {
        printf("found %s\n", elt->bname);
    }

    /* now delete each element, use the safe iterator */
    DL_FOREACH_SAFE(head,elt,tmp) {
        DL_DELETE(head,elt);
    }

    fclose(file);

    /* create elements */
    for(i=0; i<10; i++) {
        user = (example_user_t*)malloc(sizeof(example_user_t));
        if (user == NULL) {
            exit(-1);
        }
        user->id = i;
        user->cookie = i*i;
        HASH_ADD_INT(users,id,user);
    }

    for(user=users; user != NULL; user=(example_user_t*)(user->hh.next)) {
        printf("user %d, cookie %d\n", user->id, user->cookie);
    }

    utstring_new(s);
    utstring_bincpy(s, binary, sizeof(binary));
    printf("length is %u\n", utstring_len(s));

    utstring_clear(s);
    utstring_printf(s,"number %d", 10);
    printf("%s\n", utstring_body(s));

    utstring_free(s);
    return 0;
}
