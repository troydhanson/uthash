#include <stdio.h>
#include "utarray.h"

static int strsort(const void *_a, const void *_b)
{
    const char *a = *(const char* const *)_a;
    const char *b = *(const char* const *)_b;
    return strcmp(a,b);
}

int main()
{
    UT_array *strs;
    const char *s, **p;

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
    while ( (p=(const char**)utarray_next(strs,p)) != NULL ) {
        s = *p;
        printf("%s\n",s);
    }

    printf("sorting\n");
    utarray_sort(strs,strsort);

    p = NULL;
    while ( (p=(const char**)utarray_next(strs,p)) != NULL ) {
        s = *p;
        printf("finding %s\n",s);
#ifdef __cplusplus
        p = (const char**)utarray_find(strs,&s,strsort);
#else
        p = utarray_find(strs,&s,strsort);
#endif
        printf(" %s\n", (p != NULL) ? (*p) : "failed");
    }

    utarray_free(strs);

    return 0;
}
