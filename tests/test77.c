#include <stdio.h>    /* printf */
#include "utstring.h"

int main()
{
    UT_string *s,*t;
    char V_TestStr[] = "There are two needle\0s in this \0haystack with needle\0s.";
    char V_NeedleStr[] = "needle\0s";
    long *V_KMP_Table;
    long V_FindPos;
    size_t V_StartPos;
    size_t V_FindCnt;


    utstring_new(s);
    utstring_new(t);

    utstring_bincpy(s, V_TestStr, sizeof(V_TestStr)-1);
    printf("\"%s\" len=%u\n", utstring_body(s), utstring_len(s));
    utstring_bincpy(t, V_NeedleStr, sizeof(V_NeedleStr)-1);
    printf("\"%s\" len=%u\n", utstring_body(t), utstring_len(t));

    V_KMP_Table = (long *)malloc(sizeof(long) * (utstring_len(t) + 1));
    if (V_KMP_Table != NULL) {
        _utstring_BuildTableR(utstring_body(t), utstring_len(t), V_KMP_Table);

        V_FindCnt = 0;
        V_FindPos = 0;
        V_StartPos = utstring_len(s) - 1;
        do {
            V_FindPos = _utstring_findR(utstring_body(s),
                                        V_StartPos + 1,
                                        utstring_body(t),
                                        utstring_len(t),
                                        V_KMP_Table);
            if (V_FindPos >= 0) {
                V_FindCnt++;
                V_StartPos = V_FindPos - 1;
            }
            printf("utstring_find()=%ld\n", V_FindPos);
        } while (V_FindPos >= 0);
        printf("FindCnt=%u\n", (unsigned)V_FindCnt);

        free(V_KMP_Table);
    } else {
        printf("malloc() failed...\n");
    }

    utstring_free(t);
    utstring_clear(s);
    utstring_printf(s,"ABC ABCDAB ABCDABCDABDE");
    int o;

    o=utstring_find(  s, -9, "ABC", 3 ) ;
    printf("expect 15 %d\n",o);
    o=utstring_find(  s,  3, "ABC", 3 ) ;
    printf("expect  4 %d\n",o);
    o=utstring_find(  s, 16, "ABC", 3 ) ;
    printf("expect -1 %d\n",o);
    o=utstring_findR( s, -9, "ABC", 3 ) ;
    printf("expect 11 %d\n",o);
    o=utstring_findR( s, 12, "ABC", 3 ) ;
    printf("expect  4 %d\n",o);
    o=utstring_findR( s, 13, "ABC", 3 ) ;
    printf("expect 11 %d\n",o);
    o=utstring_findR( s,  2, "ABC", 3 ) ;
    printf("expect  0 %d\n",o);



    utstring_free(s);

    return 0;
}
