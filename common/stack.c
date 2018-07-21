/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct _Stack
{
    void **stack;    /* Stack data */
    int ptr;         /* Current item */
    int blocksize;   /* Block sizes to increment */
    int size;        /* Total amount of memory allocated */
    int maxptr;      /* Maximum ptr to contain an item */
} Stack;

Stack *StackCreate(int blocksize)
{
    Stack *st;

    st = (Stack *) malloc(sizeof(Stack));

    if(st) {
        st->stack = (void **) calloc(1,blocksize*sizeof(void *));
        if(st->stack) {
            st->blocksize = blocksize;
            st->size = blocksize;
            st->ptr = -1;
            st->maxptr = -1;
        } else {
            free(st);
            st = NULL;
        }
    }

    return(st);
}

int StackPush(Stack *st, void *data)
{
    void *memory;
    int i;

    if(st->ptr+2 >= st->size) {
        memory = realloc(st->stack, (st->size+st->blocksize)*sizeof(void *));
        if(memory) {
            st->stack = memory;
            st->size += st->blocksize;
        } else {
            return(0);
        }
    }

    st->ptr++;
    for(i = st->ptr; i <= st->maxptr; i++)
        free(st->stack[i]);

    st->stack[st->ptr] = data;
    st->maxptr = st->ptr;

    return(1);
}

void *StackPop(Stack *st)
{
    if(st->ptr >= 0)
        return(st->stack[(st->ptr)--]);
    else
        return(NULL);
}

void *StackPeek(Stack *st)
{
    if(st->ptr >= 0)
        return(st->stack[st->ptr]);
    else
        return(NULL);
}

void *StackAdvance(Stack *st)
{
    if(StackCanAdvance(st))
        return(st->stack[++(st->ptr)]);
    else
        return(NULL);
}

int StackCanAdvance(Stack *st)
{
    return((st->ptr >= st->maxptr)?0:1);
}

int StackCanPop(Stack *st)
{
    return(st->ptr > 0);
}

void StackDestroy(Stack *st)
{
    if(st) {
        if(st->stack)
            free(st->stack);
        free(st);
    }
}
