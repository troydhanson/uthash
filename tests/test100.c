#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utarray.h"

typedef struct {
    int id;
    char *dept;
} Data;

void Data_init(void* _data) {
    Data *data = (Data*)_data;
    data->dept = (char*)malloc(sizeof(char) * 2);
}

void Data_deinit(void* _data) {
    Data *data = (Data*)_data;
    free(data->dept);
}

int main(void) {
    UT_array *datas;
    int i;
    Data d, *dp;
    char tmp[] = {'A', 0};

    UT_icd data_icd = {sizeof(Data), Data_init, NULL, Data_deinit};
    utarray_new(datas,&data_icd);

    for(i=0; i < 10; i++) {
        Data_init(&d);
        d.id = i;
        memcpy(d.dept, tmp, sizeof(char) * 2);
        utarray_push_back(datas,&d);
        tmp[0] += 1;
    }

    Data_init(&d);
    d.id = 25;
    tmp[0] = 'Z';
    memcpy(d.dept, tmp, sizeof(char)*2);
    utarray_replace(datas, 5, &d);

    for(dp=(Data*)utarray_front(datas);
        dp!=NULL;
        dp=(Data*)utarray_next(datas,dp)) {
        printf("id: %d dept: %s\n",dp->id, dp->dept);
    }

    utarray_free(datas);

    return 0;
}
