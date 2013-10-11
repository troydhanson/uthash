#include <stdio.h>    /* printf */
#include "utstring.h"

int main() {
    UT_string *s,*t;
    char a[] = " text";
	char b[] = " 3.14159 26535 89793 23846 26433 83279 50288 41971 69399 37510 58209 74944 59230 78164 06286 20899 86280 34825 34211 70679";
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
	utstring_append_len(s,b,sizeof(b)-1);
    printf("%s\n", utstring_body(s));
	utstring_append_c(s,'p');
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
