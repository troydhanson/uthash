#include <stdio.h>
#include "utarray.h"

static int strsort(const void *_a, const void *_b)
{
    char *a = *(char**)_a;
    char *b = *(char**)_b;
    return strcmp(a,b);
}

static int revsort(const void *_a, const void *_b)
{
    char *a = *(char**)_a;
    char *b = *(char**)_b;
    return strcmp(b,a);
}

int main()
{
    UT_array *strs,*strs2;
    char *s, **p=NULL;
    utarray_new(strs, &ut_str_icd);
    s="hello";
    utarray_push_back(strs, &s);
    s="world";
    utarray_push_back(strs, &s);
    while ( (p=(char**)utarray_next(strs,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    s="begin";
    utarray_insert(strs,&s,0);
    while ( (p=(char**)utarray_next(strs,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    utarray_new(strs2, &ut_str_icd);
    s="alt";
    utarray_push_back(strs2, &s);
    s="oth";
    utarray_push_back(strs2, &s);
    utarray_inserta(strs2, strs, 1);
    while ( (p=(char**)utarray_next(strs2,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    utarray_erase(strs2,0,2);
    while ( (p=(char**)utarray_next(strs2,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    utarray_pop_back(strs2);
    while ( (p=(char**)utarray_next(strs2,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    utarray_concat(strs2, strs);
    while ( (p=(char**)utarray_next(strs2,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    utarray_clear(strs2);
    utarray_concat(strs2, strs);
    while ( (p=(char**)utarray_next(strs2,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    printf("sorting strs2\n");
    utarray_sort(strs2,strsort);
    while ( (p=(char**)utarray_next(strs2,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    printf("reverse sorting strs2\n");
    utarray_sort(strs2,revsort);
    while ( (p=(char**)utarray_next(strs2,p)) != NULL ) {
        printf("%s ",*p);
    }
    printf("\n");
    utarray_clear(strs2);
    utarray_free(strs2);
    utarray_free(strs);
    return 0;
}
