#include <stdio.h>
#include "utstring.h"

int main()
{
    UT_string *s;
    char binary[] = "\xff\xff";

    utstring_new(s);
    utstring_bincpy(s, binary, sizeof(binary));
    printf("length is %u\n", utstring_len(s));

    utstring_clear(s);
    utstring_printf(s,"number %d", 10);
    printf("%s\n", utstring_body(s));

    utstring_free(s);
    return 0;
}
