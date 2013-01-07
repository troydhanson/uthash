#include <stdio.h>    /* printf */
#include "utstring.h"

int main() {
    UT_string *s,*t;
    char a[] = " text";
    utstring_new(s);
    utstring_new(t);

    utstring_printf(s,"hello %s", "world");
    printf("%s\n", utstring_body(s));
    utstring_bincpy(s,a,sizeof(a)-1);
    printf("%s\n", utstring_body(s));
    utstring_printf(t," second");
    printf("%s\n", utstring_body(t));
    utstring_concat(s,t);
    printf("%s\n", utstring_body(s));
    utstring_clear(t);
    printf("cleared, length t now: %d\n", utstring_len(t));
    printf("length s now: %d\n", utstring_len(s));
    utstring_printf(t,"one %d two %u three %s", 1, 2, "(3)");
    printf("%s\n", utstring_body(t));
    printf("length t now: %d\n", utstring_len(t));


    utstring_free(s);
    return 0;
}
