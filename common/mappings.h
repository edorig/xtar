/*
 * Copyright(c) Stella Consulting Ltd
 *  1998-1999   http://www.stella-consulting.ltd.uk/
 *
 * Copyright(c) The University of Liverpool
 *  1993-1999
 *
 * See COPYRIGHT file for details on copying and distributing this software.
 *
 * FILE NAME:	mappings.h
 * CREATED:	Fri Aug 12 1994
 * AUTHOR:	Rik Turnbull <rik@stella-consulting.ltd.uk>
 * DESCRIPTION:	Header file for mappings.c.
 *
 */

#ifndef COMMON_MAPPINGS_H
#define COMMON_MAPPINGS_H

typedef int (*mappings_func)(char *, char *);

extern void mappings_create(char *);
extern char *mappings_execute(char *, char *, int);

#endif
