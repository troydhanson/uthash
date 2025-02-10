#include <stdio.h>    /* printf */
#include "utstring.h"

int main()
{
    UT_string *s,*t;
    char V_TestStr[] = "There are two needle\0s in this \0haystack with needle\0s.";
    char V_NeedleStr[] = "needle\0s";
    long V_FindPos;
    size_t V_FindCnt;


    utstring_new(s);
    utstring_new(t);

    utstring_bincpy(s, V_TestStr, sizeof(V_TestStr)-1);
    printf("\"%s\" len=%u\n", utstring_body(s), (unsigned)utstring_len(s));
    utstring_bincpy(t, V_NeedleStr, sizeof(V_NeedleStr)-1);
    printf("\"%s\" len=%u\n", utstring_body(t), (unsigned)utstring_len(t));

    V_FindCnt = 0;
    V_FindPos = 0;
    do {
        V_FindPos = utstring_find(s,
                                  V_FindPos,
                                  utstring_body(t),
                                  utstring_len(t));
        printf("utstring_find()=%ld\n", V_FindPos);
        if (V_FindPos >= 0) {
            V_FindPos++;
            V_FindCnt++;
        }
    } while (V_FindPos >= 0);
    printf("FindCnt=%u\n", (unsigned)V_FindCnt);

    utstring_free(s);
    utstring_free(t);

    return 0;
}
