#include <stdio.h>
#include "utlist.h"

typedef struct el {
    int id;
    struct el *next, *prev;
} el;

el *head = NULL;

int main(int argc, char *argv[]) {
    int i;
    int count;
    el els[10], *e;
    el *out;
 
    for(i=0;i<10;i++) els[i].id='a'+i;

    /* test CDL macros */
    printf("CDL macros\n");
    CDL_PREPEND(head,&els[0]);
    CDL_PREPEND(head,&els[1]);
    CDL_PREPEND(head,&els[2]);
    CDL_FOREACH(head,e)
        printf("%c ", e->id);
    printf("\n");
    CDL_POP_LAST(head, out);
    CDL_FOREACH(head,e)
        printf("%c ", e->id);
    printf("\n");
    CDL_FOREACH(out,e)
        printf("%c ", e->id);
        
    printf("\n");
    
    // reset
    head = NULL;
    
    /* test DL macros */
    printf("DL macros\n");
    DL_PREPEND(head,&els[0]);
    DL_PREPEND(head,&els[1]);
    DL_PREPEND(head,&els[2]);
    DL_FOREACH(head,e)
        printf("%c ", e->id);
    printf("\n");
    DL_POP_LAST(head, out);
    DL_FOREACH(head,e)
        printf("%c ", e->id);
    printf("\n");
    DL_FOREACH(out,e)
        printf("%c ", e->id);
        
    printf("\n");

    return 0;
}