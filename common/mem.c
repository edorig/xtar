/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	mem.c
 * CREATED:     Tue 26 Jan 1999
 * AUTHOR:	Rik Turnbull (rik@stella-consulting.ltd.uk)
 * DESCRIPTION: Memory allocation routines that cheat and exit the program if 
 *              we are out of memory.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *mem_strdup(char *);
void *mem_calloc(int);

/*----------------------------------------------------------------------------
  mem_strdup()
  Allocate memory using strdup() and copy a string.                    
  ---------------------------------------------------------------------------*/
char *mem_strdup(char *string)
{
    char *memory;

    memory = strdup(string);
    if(memory == NULL) {
        fprintf(stderr, "Failed to allocate memory! Exiting...\n");
        exit(1);
    }

    return(memory);
}

/*----------------------------------------------------------------------------
  mem_calloc()
  Allocate memory using calloc() and initialise memory area to zeros.
  ---------------------------------------------------------------------------*/
void *mem_calloc(int size)
{
    void *memory;

    memory = calloc(1,size);
    if(memory == NULL) {
        fprintf(stderr, "Failed to allocate memory! Exiting...\n");
        exit(1);
    }

    return(memory);
}
