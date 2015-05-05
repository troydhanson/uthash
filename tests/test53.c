#include <stdio.h>
#include "utstring.h"

int main()
{
    UT_string *s;

    utstring_new(s);
    utstring_printf(s, "hello world!" );
    printf("%s\n", utstring_body(s));

    utstring_free(s);
    return 0;
}
