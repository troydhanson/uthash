#include <stdio.h>    /* printf */
#include "utstring.h"

int main()
{
    UT_string *s,*t;
    char V_TestStr[] = "There are two needle\0s in this \0haystack with needle\0s.";
    char V_NeedleStr[] = "needle\0s";
    long *V_KMP_Table;
    long V_FindPos;
    size_t V_StartPos = 0;
    size_t V_FindCnt = 0;


    utstring_new(s);
    utstring_new(t);

    utstring_bincpy(s, V_TestStr, sizeof(V_TestStr)-1);
    printf("\"%s\" len=%u\n", utstring_body(s), (unsigned)utstring_len(s));
    utstring_bincpy(t, V_NeedleStr, sizeof(V_NeedleStr)-1);
    printf("\"%s\" len=%u\n", utstring_body(t), (unsigned)utstring_len(t));

    V_KMP_Table = (long *)malloc(sizeof(long) * (utstring_len(t) + 1));
    if (V_KMP_Table != NULL) {
        _utstring_BuildTable(utstring_body(t), utstring_len(t), V_KMP_Table);

        do {
            V_FindPos = _utstring_find(utstring_body(s) + V_StartPos,
                                       utstring_len(s) - V_StartPos,
                                       utstring_body(t),
                                       utstring_len(t),
                                       V_KMP_Table);
            if (V_FindPos >= 0) {
                V_FindPos += V_StartPos;
                V_FindCnt++;
                V_StartPos = V_FindPos + 1;
            }
            printf("utstring_find()=%ld\n", V_FindPos);
        } while (V_FindPos >= 0);
        printf("FindCnt=%u\n", (unsigned)V_FindCnt);

        free(V_KMP_Table);
    } else {
        printf("malloc() failed...\n");
    }

    utstring_free(s);
    utstring_free(t);

    return 0;
}
