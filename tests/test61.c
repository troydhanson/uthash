#include <stdio.h>
#include "utarray.h"

static int strsort(const void *_a, const void *_b)
{
    char *a = *(char**)_a;
    char *b = *(char**)_b;
    return strcmp(a,b);
}

int main()
{
    UT_array *strs;
    char *s, **p;

    utarray_new(strs,&ut_str_icd);

    s = "hello";
    utarray_push_back(strs, &s);
    s = "world";
    utarray_push_back(strs, &s);
    s = "one";
    utarray_push_back(strs, &s);
    s = "two";
    utarray_push_back(strs, &s);
    s = "three";
    utarray_push_back(strs, &s);

    p = NULL;
    while ( (p=(char**)utarray_next(strs,p)) != NULL ) {
        s = *p;
        printf("%s\n",s);
    }

    printf("sorting\n");
    utarray_sort(strs,strsort);

    p = NULL;
    while ( (p=(char**)utarray_next(strs,p)) != NULL ) {
        s = *p;
        printf("finding %s\n",s);
        p = utarray_find(strs,&s,strsort);
        printf(" %s\n", (p != NULL) ? (*p) : "failed");
    }

    utarray_free(strs);

    return 0;
}
