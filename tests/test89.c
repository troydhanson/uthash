/* Minified version of code from tinydtls 0.9               */
/* See https://projects.eclipse.org/projects/iot.tinydtls   */

#include "utlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int clock_time_t;

typedef struct netq_t {
    struct netq_t *next;
    clock_time_t t;
} netq_t;

void dump_queue(struct netq_t *queue)
{
    struct netq_t *p;
    int i = 0;

    if (!queue) {
        printf("(null)\n");
    } else {
        LL_FOREACH(queue, p) {
            printf("node #%d, timeout: %d\n", i++, p->t);
        }
    }
}

int netq_insert_node(netq_t **queue, netq_t *node)
{
    netq_t *p = *queue;
    while (p && p->t <= node->t) {
        p = p->next;
    }
    /* *INDENT-OFF* */
    if (p)
        LL_PREPEND_ELEM(*queue, p, node);
    else
        LL_APPEND(*queue, node);
    /* *INDENT-ON* */
    return 1;
}

int main()
{
    struct netq_t *nq = NULL;
    int i;

    clock_time_t timestamps[] = { 300, 100, 200, 400, 500 };

    for (i = 0; i < sizeof(timestamps)/sizeof(clock_time_t); i++) {
        struct netq_t *node = malloc(sizeof *node);
        memset(node, '\0', sizeof *node);
        node->t = timestamps[i];

        if (netq_insert_node(&nq, node) != 1) {
            puts("ERROR");
        }
    }

    dump_queue(nq);
}
