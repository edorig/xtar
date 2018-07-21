/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 */
#ifndef _COMMON_STACK_H
#define _COMMON_STACK_H

typedef struct _Stack Stack;

extern Stack *StackCreate(int);
extern int StackPush(Stack *, void *);
extern void *StackPop(Stack *);
extern void *StackPeek(Stack *);
extern void *StackAdvance(Stack *);
extern int StackCanAdvance(Stack *);
extern int StackCanPop(Stack *);
extern void StackDestroy(Stack *);

#endif
