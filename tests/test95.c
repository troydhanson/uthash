#include <assert.h>
#include <stddef.h>   /* size_t, NULL */
#include "utstack.h"

typedef struct el {
    int id, score;
    struct el *next;
    struct el *next2;
} el;

int main()
{
    el alpha = {1, 100, NULL, NULL};
    el beta = {2, 100, NULL, NULL};
    el gamma = {3, 100, NULL, NULL};
    el delta = {4, 100, NULL, NULL};

    el *stack1 = NULL;
    el *stack2 = NULL;
    el *dummy;
    int size1;
    size_t size2;

    STACK_COUNT(stack1, dummy, size1); assert(size1 == 0);
    STACK_COUNT(stack2, dummy, size2); assert(size2 == 0);
    assert(STACK_EMPTY(stack1));
    assert(STACK_EMPTY(stack2));

    STACK_PUSH(stack1, &alpha);
    STACK_COUNT(stack1, dummy, size1); assert(size1 == 1);
    STACK_PUSH(stack1, &beta);
    STACK_COUNT(stack1, dummy, size1); assert(size1 == 2);
    STACK_PUSH(stack1, &gamma);
    STACK_PUSH2(stack1, &delta, next);
    STACK_COUNT(stack1, dummy, size1); assert(size1 == 4);
    assert(stack1 == &delta);

    STACK_PUSH2(stack2, &alpha, next2);
    assert(stack2 == &alpha);
    assert(alpha.next2 == NULL);
    STACK_PUSH2(stack2, &delta, next2);
    assert(stack2 == &delta);
    assert(delta.next2 == &alpha);
    STACK_COUNT2(stack2, dummy, size2, next2); assert(size2 == 2);
    assert(!STACK_EMPTY(stack2));
    assert(stack2 == &delta);

    assert(!STACK_EMPTY(stack1));
    assert(!STACK_EMPTY(stack2));

    STACK_POP(stack1, dummy); assert(stack1 == &gamma); assert(dummy == &delta);
    STACK_POP(stack1, dummy); assert(stack1 == &beta); assert(dummy == &gamma);
    STACK_POP(stack1, dummy); assert(stack1 == &alpha); assert(dummy == &beta);
    STACK_COUNT(stack1, dummy, size1); assert(size1 == 1);
    STACK_POP(stack1, dummy); assert(stack1 == NULL); assert(dummy == &alpha);

    assert(STACK_TOP(stack2) == &delta);
    while (!STACK_EMPTY(stack2)) {
        STACK_POP2(stack2, dummy, next2);
    }

    assert(STACK_EMPTY(stack1));
    assert(STACK_EMPTY(stack2));

    return 0;
}
