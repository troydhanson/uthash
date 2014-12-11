#include <unistd.h>    /* write */
#include "utstring.h"

int main() {
    UT_string *s;
    char V_TestStr[] = "The quick brown dog, &c.";
    utstring_new(s);

    utstring_bincpy(s, V_TestStr, sizeof(V_TestStr)-1);
    write(STDOUT_FILENO, utstring_body(s), utstring_len(s));
    write(STDOUT_FILENO, "\n", 1);

    utstring_truncate(s, 10);
    write(STDOUT_FILENO, utstring_body(s), utstring_len(s));
    write(STDOUT_FILENO, "\n", 1);

    utstring_truncate(s, 10000);
    write(STDOUT_FILENO, utstring_body(s), utstring_len(s));
    write(STDOUT_FILENO, "\n", 1);

    utstring_free(s);

    return 0;
}
