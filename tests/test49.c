#include <stdio.h>
#include "utarray.h"

int main()
{
    UT_array *strs;
    char *s, **p;

    utarray_new(strs,&ut_str_icd);

    s = "hello";
    utarray_push_back(strs, &s);
    s = "world";
    utarray_push_back(strs, &s);
    p = NULL;
    while ( (p=(char**)utarray_next(strs,p)) != NULL ) {
        printf("%s\n",*p);
    }

    utarray_free(strs);

    return 0;
}
