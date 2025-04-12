#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utarray.h"

typedef struct {
    int id;
    char *dept;
} Data;

void Data_copy(void* vdst, const void* vsrc) {
  Data *dst = (Data*)vdst;
  Data *src = (Data*)vsrc;
  dst->id = src->id;
  dst->dept = strdup(src->dept);
}

void Data_deinit(void* vdata) {
    Data *data = (Data*)vdata;
    free(data->dept);
}

int main(void) {
    UT_array *datas;
    int i;
    Data d, *dp;
    char tmp[2] = "A";

    UT_icd data_icd = {sizeof(Data), NULL, Data_copy, Data_deinit};
    utarray_new(datas, &data_icd);

    for(i = 0; i < 10; i++) {
        d.id = i;
        d.dept = strdup(tmp);
        utarray_push_back(datas, &d);
        Data_deinit(&d);
        tmp[0] += 1;
    }

    d.id = 25;
    d.dept = strdup("Z");
    utarray_replace(datas, &d, 5);
    Data_deinit(&d);

    for(dp = (Data*)utarray_front(datas);
        dp != NULL;
        dp = (Data*)utarray_next(datas, dp)) {
        printf("id: %d dept: %s\n", dp->id, dp->dept);
    }

    utarray_free(datas);
    return 0;
}
